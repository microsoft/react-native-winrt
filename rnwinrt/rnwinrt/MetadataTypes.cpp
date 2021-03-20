#include "pch.h"

#include "MetadataTypes.h"

using namespace std::literals;
using namespace winmd::reader;

function_signature::function_signature(const MethodDef& def) :
    method_def(def), signature(def.Signature())
{
    has_return_value = static_cast<bool>(signature.ReturnType());

    auto begin = param_begin();
    auto end = param_end();
    assert(std::distance(begin.sig_iterator, end.sig_iterator) == std::distance(begin.param, end.param));
    for (; begin != end; ++begin)
    {
        if (begin.is_output() && begin.by_ref())
        {
            has_out_params = true;
        }
        else
        {
            ++param_count;
        }

#ifdef _DEBUG
        if (begin.is_output() && !begin.by_ref())
        {
            // The only time we should have a discrepency between being 'OutByRef', but not 'ByRef' is for fill arays
            assert(begin.type().is_szarray());
        }
#endif
    }
}

method_class classify_method(const MethodDef& def)
{
    auto name = def.Name();
    if (!def.SpecialName())
    {
        return method_class{ name, method_class::function };
    }
    else if (starts_with(name, "get_"))
    {
        return method_class{ name.substr(4), method_class::property_getter };
    }
    else if (starts_with(name, "put_"))
    {
        return method_class{ name.substr(4), method_class::property_setter };
    }
    else if (starts_with(name, "add_"))
    {
        return method_class{ name.substr(4), method_class::event_add };
    }
    else if (starts_with(name, "remove_"))
    {
        return method_class{ name.substr(7), method_class::event_remove };
    }
    else if (name == ".ctor"sv)
    {
        return method_class{ "", method_class::constructor };
    }
    else
    {
        throw std::runtime_error("Unknown method type");
    }
}

template <bool IsClass>
type_method_data<IsClass>::type_method_data(const TypeDef& typeDef)
{
    for (auto&& method : typeDef.MethodList())
    {
        auto methodClass = classify_method(method);
        if constexpr (IsClass)
        {
            // Skip if not constructor or static for classes
            if ((methodClass.kind != method_class::constructor) && !method.Flags().Static())
            {
                continue;
            }
        }
        else
        {
            assert(!method.Flags().Static());
        }

        switch (methodClass.kind)
        {
        case method_class::function: {
            // We can resolve functions of the same arity on the same type here, instead of at runtime
            auto [nameItr, nameFound] = reverse_sorted_find(
                functions, [&](const function_data& data) { return data.name.compare(methodClass.name); });
            if (!nameFound)
            {
                nameItr = functions.emplace(nameItr, methodClass.name);
            }

            function_overload_data fn(method);
            auto fnItr = std::find_if(nameItr->overloads.begin(), nameItr->overloads.end(),
                [&](const function_overload_data& data) { return data.arity == fn.arity; });
            if (fnItr == nameItr->overloads.end())
            {
                nameItr->overloads.push_back(std::move(fn));
            }
            else if (fn.is_default_overload)
            {
                assert(!fnItr->is_default_overload);
                *fnItr = std::move(fn);
            }
            break;
        }

        case method_class::property_getter: {
            auto [itr, found] = reverse_sorted_find(
                properties, [&](const property_data& data) { return data.name.compare(methodClass.name); });
            if (!found)
            {
                itr = properties.emplace(itr, methodClass.name);
            }

            assert(!itr->getter);
            itr->getter = method;
            break;
        }

        case method_class::property_setter: {
            auto [itr, found] = reverse_sorted_find(
                properties, [&](const property_data& data) { return data.name.compare(methodClass.name); });
            if (!found)
            {
                itr = properties.emplace(itr, methodClass.name);
            }

            assert(!itr->setter);
            itr->setter = method;
            break;
        }

        case method_class::event_add: {
            auto [itr, found] = reverse_sorted_find(
                events, [&](const event_data& data) { return data.name.compare(methodClass.name); });
            assert(!found);
            if (!found)
            {
                events.emplace(itr, methodClass.name, method.Signature().Params().first->Type());
            }
            break;
        }

        case method_class::event_remove:
            // NOTE: We only handle event add and not remove since they always come in pairs, and it makes it easier
            // for us to get the delegate type
            break;

        case method_class::constructor: {
            assert(IsClass);
            function_signature fn(method);
            auto itr = std::find_if(constructors.begin(), constructors.end(),
                [&](const constructor_data& data) { return data.method.param_count == fn.param_count; });

            if (itr == constructors.end())
            {
                constructors.emplace_back(std::move(fn));
            }
            else
            {
                // TODO: Validate if this is allowed. If not, the 'std::find_if' above is not needed
                // TODO: If not, then we need to figure out how to select the correct one
                assert(false);
            }
            break;
        }
        }
    }
}

static bool is_type_allowed(const Settings& settings, const TypeDef& typeDef, bool isClass = false)
{
    if (!settings.Filter.Includes(typeDef))
    {
        return false;
    }

    if (isClass && settings.FilterToAllowForWeb && !is_allow_for_web(typeDef))
    {
        return false;
    }

    if (!settings.IncludeDeprecated && is_deprecated(typeDef))
    {
        return false;
    }

    if (!settings.IncludeWebHostHidden && is_web_host_hidden(typeDef))
    {
        // Special-case Windows.Foundation.IPropertyValueStatics which should not be WebHostHidden as it breaks basic
        // language features by omission.
        if (typeDef.TypeNamespace() == foundation_namespace)
        {
            if (isClass ? (typeDef.TypeName() == "PropertyValue"sv) : (typeDef.TypeName() == "IPropertyValueStatics"sv))
            {
                return true;
            }
        }
        return false;
    }

    return true;
}

static bool is_namespace_allowed(const Settings& settings, const cache::namespace_members& members)
{
    for (const auto& typeDef : members.interfaces)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.classes)
    {
        if (is_type_allowed(settings, typeDef, true /*isClass*/))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.enums)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.structs)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.delegates)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    return false;
}

static std::string_view next_namespace_part(std::string_view& name)
{
    auto pos = name.find_first_of('.');
    auto result = name.substr(0, pos);
    name = (pos == std::string_view::npos) ? std::string_view{} : name.substr(pos + 1);
    return result;
}

static namespace_projection_data* get_namespace(
    projection_data& data, std::string_view name, namespace_projection_data* prev)
{
    namespace_projection_data* parent = nullptr;
    auto remainder = name;

    if (prev && is_child_namespace(prev->full_name, name))
    {
        // Our optimization worked; we can use the previous namespace as a base
        parent = prev;
        remainder = name.substr(prev->full_name.size() + 1);
    }
    else
    {
        // "Miss" on our optimization; start at a root namespace
        auto rootName = next_namespace_part(remainder);
        auto [itr, found] = reverse_sorted_find(data.root_namespaces, [&](auto&& nsData) {
            return nsData->name.compare(rootName);
        });

        if (found)
        {
            parent = itr->get();
        }
        else
        {
            parent = data.root_namespaces.insert(itr, std::make_unique<namespace_projection_data>(rootName))->get();
        }
    }

    while (!remainder.empty())
    {
        // NOTE: We keep all of these lists sorted. Since the metadata should be sorted, we should do at most one
        // comparison in the typical case (inspect the last element) and insertions should be fast as well
        auto nextName = next_namespace_part(remainder);
        bool found = false;
        auto itr = parent->namespace_children.rbegin();
        for (; itr != parent->namespace_children.rend(); ++itr)
        {
            auto cmp = nextName.compare((*itr)->name);
            if (cmp == 0)
            {
                parent = itr->get();
                found = true;
                break;
            }
            else if (cmp > 0)
            {
                break;
            }
        }

        if (!found)
        {
            auto fullName = remainder.empty() ? name : name.substr(0, name.size() - remainder.size() - 1);
            auto newNs =
                parent->namespace_children.insert(itr.base(), std::make_unique<namespace_projection_data>(fullName))
                    ->get();
            parent->named_children.push_back(newNs);
            parent = newNs;
        }
    }

    return parent;
}

template <typename T, typename Func>
static void parse_typedefs(const Settings& settings, projection_data& data,
    const std::vector<TypeDef>& typeDefs, std::vector<std::unique_ptr<T>>& list, Func&& callback)
{
    bool isFoundationNs = false, isNumericsNs = false;
    if (!typeDefs.empty())
    {
        auto ns = typeDefs[0].TypeNamespace();
        if (ns == foundation_namespace)
            isFoundationNs = true;
        else if (ns == "Windows.Foundation.Numerics"sv)
            isNumericsNs = true;
    }

    for (auto&& typeDef : typeDefs)
    {
        if (!is_type_allowed(settings, typeDef))
            continue;

        // Ignore generic type *definitions*. We care about *instantiations*
        if (is_generic(typeDef))
            continue;

        if constexpr (std::is_same_v<T, interface_projection_data>)
        {
            // Static/activation factory interfaces don't need to be tracked; we handle them differently
            if (is_factory_interface(typeDef))
                continue;
        }
        else if constexpr (std::is_same_v<T, struct_projection_data>)
        {
            // Some types are manually projected
            if (isFoundationNs)
            {
                auto name = typeDef.TypeName();
                if ((name == "HResult"sv) || (name == "DateTime"sv) || (name == "TimeSpan"sv) ||
                    (name == "EventRegistrationToken"sv))
                    continue;
            }
            else if (isNumericsNs)
            {
                auto name = typeDef.TypeName();
                auto itr = std::find_if(std::begin(numerics_mappings), std::end(numerics_mappings),
                    [&](auto& pair) { return pair.first == name; });
                if (itr != std::end(numerics_mappings))
                    continue;
            }
        }

        list.push_back(std::make_unique<T>(typeDef));
        callback(list.back().get());
    }
}

void parse_metadata(const Settings& settings, projection_data& data)
{
    // NOTE: The metadata *should* be sorted, and therefore the probability of one namespace being a "child" of the
    // previous one is moderately decent, so there's a possible optimization we can do by remembering the previous
    // namespace we saw
    namespace_projection_data* currNs = nullptr;

    for (auto&& [fullName, members] : settings.Cache.namespaces())
    {
        if (!is_namespace_allowed(settings, members))
        {
            continue;
        }

        currNs = get_namespace(data, fullName, currNs);
        parse_typedefs(settings, data, members.classes, currNs->class_children, [&](class_projection_data* classData) {
            currNs->named_children.push_back(classData);
            // TODO
        });
        parse_typedefs(settings, data, members.enums, currNs->enum_children, [&](enum_projection_data* enumData) {
            currNs->named_children.push_back(enumData);
        });
        parse_typedefs(
            settings, data, members.structs, currNs->struct_children, [&](struct_projection_data* structData) {
                // NOTE: Structs can have 'IReference<T>' members, so we need to check its fields
                (void)structData; // TODO
            });
        parse_typedefs(
            settings, data, members.delegates, currNs->delegate_children, [&](delegate_projection_data* delegateData) {
                (void)delegateData; // TODO
            });
        parse_typedefs(
            settings, data, members.interfaces, currNs->interface_children, [&](interface_projection_data* ifaceData) {
                data.interfaces.push_back(ifaceData);
                // TODO
            });
    }
}
