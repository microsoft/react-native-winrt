#include "pch.h"

#include "MetadataTypes.h"
#include "sha1.h"

using namespace std::literals;
using namespace winmd::reader;

function_signature::function_signature(const MethodDef& def) : method_def(def), signature(def.Signature())
{
    has_return_value = static_cast<bool>(signature.ReturnType());

    for (auto&& param : params())
    {
        if (param.is_output() && param.by_ref())
        {
            has_out_params = true;
        }
        else
        {
            ++param_count;
        }

#ifdef _DEBUG
        if (param.is_output() && !param.by_ref())
        {
            // The only time we should have a discrepency between being 'Out', but not 'ByRef' is for fill arays
            assert(param.type().is_szarray());
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
type_method_data<IsClass>::type_method_data(const Settings& settings, const TypeDef& typeDef)
{
    for (auto&& method : typeDef.MethodList())
    {
        if (!is_method_allowed(settings, method))
            continue;

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
                functions, [&](const function_data& data) { return camel_case_compare(data.name, methodClass.name); });
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
                assert(!fnItr->is_default_overload); // Two default overloads of the same arity
                *fnItr = std::move(fn);
            }
            break;
        }

        case method_class::property_getter: {
            auto [itr, found] = reverse_sorted_find(
                properties, [&](const property_data& data) { return camel_case_compare(data.name, methodClass.name); });
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
                properties, [&](const property_data& data) { return camel_case_compare(data.name, methodClass.name); });
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
                events, [&](const event_data& data) { return lower_case_compare(data.name, methodClass.name); });
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

            // TODO: Validate that MIDLRT dis-allows constructors of the same arity
#ifdef _DEBUG
            auto itr = std::find_if(constructors.begin(), constructors.end(),
                [&](const constructor_data& data) { return data.method.param_count == fn.param_count; });
            assert(itr == constructors.end());
#endif

            constructors.emplace_back(std::move(fn));
            break;
        }
        }
    }
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
        auto [itr, found] =
            reverse_sorted_find(data.root_namespaces, [&](auto&& nsData) { return nsData->name.compare(rootName); });

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

static void append_signature(sha1& hash, ElementType elemType)
{
    std::string_view sig;
    switch (elemType)
    {
    case ElementType::Boolean:
        sig = "b1"sv;
        break;
    case ElementType::Char:
        sig = "c2"sv;
        break;
    case ElementType::U1:
        sig = "u1"sv;
        break;
    case ElementType::I2:
        sig = "i2"sv;
        break;
    case ElementType::U2:
        sig = "u2"sv;
        break;
    case ElementType::I4:
        sig = "i4"sv;
        break;
    case ElementType::U4:
        sig = "u4"sv;
        break;
    case ElementType::I8:
        sig = "i8"sv;
        break;
    case ElementType::U8:
        sig = "u8"sv;
        break;
    case ElementType::R4:
        sig = "f4"sv;
        break;
    case ElementType::R8:
        sig = "f8"sv;
        break;
    case ElementType::String:
        sig = "string"sv;
        break;
    case ElementType::Object:
        sig = "cinterface(IInspectable)"sv;
        break;
    default:
        throw std::runtime_error("Unexpected ElementType");
    }

    hash.append(sig);
}

static void append_signature(sha1& hash, system_guid_t)
{
    hash.append("g16"sv);
}

static void append_interface_guid(sha1& hash, const TypeDef& typeDef)
{
    auto guid = get_interface_guid(typeDef);
    auto data = guid_to_string(guid);
    hash.append(std::string_view{ data.data(), data.size() - 1 });
}

static void append_signature(sha1& hash, const TypeSig& sig, const generic_param_stack& genericParamStack);

static void append_signature(sha1& hash, const GenericTypeInstSig& sig, const generic_param_stack& genericParamStack)
{
    // 'pinterface({guid};params...)'
    hash.append("pinterface({"sv);

    auto typeDef = generic_type_def(sig);
    append_interface_guid(hash, typeDef);
    hash.append("}"sv);

    for (auto&& param : sig.GenericArgs())
    {
        hash.append(";"sv);
        append_signature(hash, param, genericParamStack);
    }

    hash.append(")"sv);
}

static void append_signature(sha1& hash, const TypeDef& typeDef)
{
    assert(!is_generic(typeDef));

    switch (get_category(typeDef))
    {
    case category::class_type: { // 'rc(name;defaultIface)'
        auto defaultIface = default_interface(typeDef);
        if (!defaultIface)
        {
            throw std::runtime_error("Class with no default interface can't be used as a generic parameter");
        }

        hash.append("rc("sv);
        hash.append(typeDef.TypeNamespace());
        hash.append("."sv);
        hash.append(typeDef.TypeName());
        hash.append(";"sv);
        resolve_type(defaultIface, {},
            overloaded{
                [&](const TypeDef& ifaceDef) { append_signature(hash, ifaceDef); },
                [&](const GenericTypeInstSig& sig, const generic_param_stack& genericParamStack) {
                    append_signature(hash, sig, genericParamStack);
                },
                [&](system_guid_t) { append_signature(hash, system_guid); },
            });
        hash.append(")"sv);
        break;
    }

    case category::delegate_type: // 'delegate({iid})'
        hash.append("delegate({"sv);
        append_interface_guid(hash, typeDef);
        hash.append("})"sv);
        break;

    case category::enum_type: // 'enum(name;type)'
        hash.append("enum("sv);
        hash.append(typeDef.TypeNamespace());
        hash.append("."sv);
        hash.append(typeDef.TypeName());
        hash.append(";"sv);
        append_signature(hash, underlying_enum_type(typeDef));
        hash.append(")"sv);
        break;

    case category::interface_type: // '{iid}'
        hash.append("{"sv);
        append_interface_guid(hash, typeDef);
        hash.append("}"sv);
        break;

    case category::struct_type: { // 'struct(name;types...)'
        hash.append("struct("sv);
        hash.append(typeDef.TypeNamespace());
        hash.append("."sv);
        hash.append(typeDef.TypeName());
        for (auto&& field : typeDef.FieldList())
        {
            hash.append(";"sv);
            append_signature(hash, field.Signature().Type(), {});
        }
        hash.append(")"sv);
        break;
    }
    }
}

static void append_signature(sha1& hash, const TypeSig& sig, const generic_param_stack& genericParamStack)
{
    resolve_type(sig, genericParamStack,
        overloaded{
            [&](const GenericTypeInstSig& genericSig, const generic_param_stack& newStack, bool isArray) {
                assert(!isArray);
                append_signature(hash, genericSig, newStack);
            },
            [&](const auto& value, bool isArray) {
                assert(!isArray);
                append_signature(hash, value);
            },
        });
}

static GUID get_interface_guid(const generic_instantiation& inst)
{
    sha1 hash;
    static constexpr std::uint8_t namespaceGuidBytes[] = { 0x11, 0xf4, 0x7a, 0xd5, 0x7b, 0x73, 0x42, 0xc0, 0xab, 0xae,
        0x87, 0x8b, 0x1e, 0x16, 0xad, 0xee };
    hash.append(namespaceGuidBytes);

    append_signature(hash, inst.signature(), inst.parent_stack());

    auto iidHash = hash.finalize();
    iidHash[6] = (iidHash[6] & 0x0F) | 0x50;
    iidHash[8] = (iidHash[8] & 0x3F) | 0x80;

    // Bytes 0-15 are the guid, however the data is in big-endian
    std::swap(iidHash[0], iidHash[3]);
    std::swap(iidHash[1], iidHash[2]);
    std::swap(iidHash[4], iidHash[5]);
    std::swap(iidHash[6], iidHash[7]);
    return *reinterpret_cast<const GUID*>(iidHash.data());
}

static void handle_generic_instantiation(projection_data& data, generic_instantiation&& inst);

static void check_generic_base_types(
    projection_data& data, const TypeDef& typeDef, const generic_param_stack& genericParamStack)
{
    for (auto&& type : typeDef.InterfaceImpl())
    {
        resolve_type(type.Interface(), genericParamStack,
            overloaded{
                [&](const GenericTypeInstSig& sig, const generic_param_stack& newStack) {
                    // NOTE: This will follow base types
                    handle_generic_instantiation(data, generic_instantiation(sig, newStack));
                },
                [&](const TypeDef& baseType) {
                    // NOTE: Deriving from non-generic type, so param stack irrelevant
                    check_generic_base_types(data, baseType, {});
                },
                [](auto&&) {},
            });
    }
}

static void check_generic_function_outputs(
    projection_data& data, const TypeDef& typeDef, const generic_param_stack& genericParamStack)
{
    auto checkType = [&](const TypeSig& sig) {
        resolve_type(sig, genericParamStack,
            overloaded{
                [&](const GenericTypeInstSig& sig, const generic_param_stack& newStack, bool) {
                    handle_generic_instantiation(data, generic_instantiation(sig, newStack));
                },
                // TODO: In theory, we could be returning a non-projected type that derives from a generic type. In that
                // case, it would be ideal if those methods were still visible to JS, but that might be too much work to
                // do here for the benefit
                [](auto&&, bool) {},
            });
    };

    for (auto&& method : typeDef.MethodList())
    {
        // We don't care about function inputs since they need to be provided by the caller, and even if this is coming
        // from JS (e.g. array-to-vector conversion), the created object does not need to be observable by JS
        function_signature fn(method);
        if (fn.has_return_value)
        {
            checkType(fn.signature.ReturnType().Type());
        }

        for (auto&& param : fn.params())
        {
            // NOTE: Purposefully not checking by_ref since fill arrays still produce observable output
            if (param.is_output())
            {
                checkType(param.type());
            }
        }
    }
}

static void handle_generic_instantiation(projection_data& data, generic_instantiation&& inst)
{
    // Some generic interfaces are projected away or handled differently
    auto typeDef = generic_type_def(inst.signature());
    if (get_category(typeDef) == category::delegate_type)
        return;

    if (typeDef.TypeNamespace() == foundation_namespace)
    {
        if (typeDef.TypeName() == "IReference`1"sv)
            return;
    }

    // First check to see if we've handled this type before
    auto guid = get_interface_guid(inst);
    if (data.generic_instantiations.find(guid) != data.generic_instantiations.end())
        return;

    auto itr = data.generic_instantiations.emplace(guid, nullptr).first;

    // NOTE: The generic params could, themselves, be generic. E.g. an 'IVector<IReference<T>>', however we don't
    // actually care about that unless that fact is actually observable in JS, which the following will catch
    check_generic_base_types(data, typeDef, inst.stack());
    check_generic_function_outputs(data, typeDef, inst.stack());

    itr->second = std::make_unique<generic_interface_instantiation>(std::move(inst), guid);
    data.interfaces.push_back(itr->second.get());
}

template <typename T, typename Func>
static void parse_typedefs(const Settings& settings, projection_data& data, const std::vector<TypeDef>& typeDefs,
    std::vector<std::unique_ptr<T>>& list, Func&& callback)
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

        list.push_back(std::make_unique<T>(settings, typeDef));
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
        currNs->members = &members;

        parse_typedefs(settings, data, members.classes, currNs->class_children, [&](class_projection_data* classData) {
            currNs->named_children.push_back(classData);
            check_generic_base_types(data, classData->type_def, {});
            check_generic_function_outputs(data, classData->type_def, {});
        });
        parse_typedefs(settings, data, members.enums, currNs->enum_children,
            [&](enum_projection_data* enumData) { currNs->named_children.push_back(enumData); });
        parse_typedefs(
            settings, data, members.structs, currNs->struct_children, [&](struct_projection_data* structData) {
                // Structs can have 'IReference<T>' members, so we need to check their fields
                for (auto&& field : structData->type_def.FieldList())
                {
                    auto sig = field.Signature();
                    resolve_type(sig.Type(), {},
                        overloaded{
                            [&](const GenericTypeInstSig& sig, const generic_param_stack&, bool isArray) {
                                assert(!isArray);
                                handle_generic_instantiation(data, generic_instantiation(sig));
                            },
                            [](auto&&, bool) {},
                        });
                }
            });
        parse_typedefs(
            settings, data, members.delegates, currNs->delegate_children, [&](delegate_projection_data* delegateData) {
                // Delegates can have out/return values for the Invoke function
                check_generic_function_outputs(data, delegateData->type_def, {});
            });
        parse_typedefs(
            settings, data, members.interfaces, currNs->interface_children, [&](interface_projection_data* ifaceData) {
                data.interfaces.push_back(ifaceData);
                check_generic_base_types(data, ifaceData->type_def, {});
                check_generic_function_outputs(data, ifaceData->type_def, {});
            });
    }

    // We sort most everything as we go (since most should be sorted anyway), but we wait until the end to sort
    // interfaces, which we sort by guid
    std::sort(data.interfaces.begin(), data.interfaces.end(),
        [](const interface_instance* lhs, const interface_instance* rhs) {
            return compare_guid(lhs->iid, rhs->iid) < 0;
        });
}
