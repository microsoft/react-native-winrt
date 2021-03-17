#include "pch.h"

#include "MetadataHelpers.h"

using namespace std::literals;
using namespace winmd::reader;

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
        auto itr = data.root_namespaces.rbegin();
        for (; itr != data.root_namespaces.rend(); ++itr)
        {
            auto cmp = rootName.compare((*itr)->name);
            if (cmp == 0)
            {
                parent = itr->get();
                break;
            }
            else if (cmp > 0)
            {
                // We keep the arrays sorted, so this means that the namespace has not yet been added
                break;
            }
        }

        if (!parent)
        {
            parent =
                data.root_namespaces.insert(itr.base(), std::make_unique<namespace_projection_data>(rootName))->get();
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






#if 1
std::vector<std::shared_ptr<static_projection_data>> _interfaces;
std::vector<std::shared_ptr<static_projection_data>> _structs;
std::vector<std::shared_ptr<static_projection_data>> _delegates;

std::vector<std::shared_ptr<static_projection_data>> static_projection_data::ParseMetaData(const Settings& settings)
{
    std::vector<std::shared_ptr<static_projection_data>> roots;

    for (const auto& [fullName, members] : settings.Cache.namespaces())
    {
        if (!IsNamespaceAllowed(settings, fullName, members))
        {
            continue;
        }

        std::string currentFullName;
        std::string_view remainingNameSegments = fullName;
        auto parent = std::shared_ptr<static_projection_data>{};
        auto children = &roots;
        while (!remainingNameSegments.empty())
        {
            const auto delim = remainingNameSegments.find(".");
            const bool isFinalSegment = (delim == std::string_view::npos);

            if (currentFullName.length() > 0)
            {
                currentFullName += ".";
            }
            currentFullName += remainingNameSegments.substr(0, delim);

            std::string_view currentNameSegment;
            if (isFinalSegment)
            {
                std::swap(currentNameSegment, remainingNameSegments);
            }
            else
            {
                currentNameSegment = remainingNameSegments.substr(0, delim);
                remainingNameSegments.remove_prefix(delim + 1);
            }

            bool found = false;
            for (auto i = children->size() - 1; i >= 0; i--)
            {
                auto match = children->at(i);
                if (std::dynamic_pointer_cast<static_namespace_data>(match) != nullptr)
                {
                    if (match->Name() == currentNameSegment)
                    {
                        found = true;
                        parent = match;
                        children = parent->Children();
                        break;
                    }
                }
            }
            if (!found)
            {
                std::shared_ptr<static_namespace_data> newNamespace =
                    std::make_shared<static_namespace_data>(std::string(currentNameSegment),
                        currentFullName, isFinalSegment ? &members : nullptr, settings);
                children->push_back(newNamespace);
            }
        }
    }

    SortGlobalLists();

    return roots;
}

void static_namespace_data::ParseChildren(
    const winmd::reader::cache::namespace_members* members, const Settings& settings)
{
    if (members)
    {
        ParseTypeDefs<static_projection_data, static_class_data>(m_children, members->classes, settings, nullptr);
        ParseTypeDefs<static_projection_data, static_enum_data>(m_children, members->enums, settings, nullptr);
        ParseTypeDefs<static_struct_data, static_struct_data>(m_structs, members->structs, settings, &_structs);
        ParseTypeDefs<static_delegate_data, static_delegate_data>(m_delegates, members->delegates, settings, &_delegates);
        ParseTypeDefs<static_interface_data, static_interface_data>(m_interfaces, members->interfaces, settings, &_interfaces);
    }
}

template <typename T, typename U>
void static_namespace_data::ParseTypeDefs(std::vector<std::shared_ptr<T>>& items,
    const std::vector<winmd::reader::TypeDef>& typeDefs, const Settings& settings,
    std::vector<std::shared_ptr<static_projection_data>>* globalList)
{
    for (const auto& typeDef : typeDefs)
    {
        if (IsTypeAllowed(settings, typeDef))
        {
            if (auto pair = typeDef.GenericParam(); std::distance(pair.first, pair.second) == 0)
            {
                std::string fullName = std::string(typeDef.TypeNamespace()) + "." + std::string(typeDef.TypeName());
                auto item = std::make_shared<U>(typeDef.TypeName(), fullName, typeDef);
                items.push_back(item);
                if (globalList != nullptr)
                {
                    globalList->push_back(item);
                }

//                CheckForGenericMembers(typeDef);


                // 1) call typeDef->Extends recursively - this just checks if it's generic somewhere in the implements chain
            }
        }
    }
}
/*
CheckForGenericMembers(typeDef, (optional)templateArray)
{
    // look at properties/events/methods/fields of typeDef
    //  properties/events/fields: look at type and check if generic
    //  methods: look at input/output/return values
    // if generic: HandleGenericInst()
}

HandleGenericInstantiation(type, templateArray)
{
    // type always interface type
    // maintain 1 entry per type/template combo
    // add too global interface vector
    // CheckForGenericMembers on this type/template
}
*/
const std::string& static_projection_data::Name() const noexcept
{
    return m_name;
}

const std::string& static_projection_data::FullName(bool useCppDelim) const
{
    if (!useCppDelim)
    {
        return m_fullName;
    }

    if (m_fullNameCpp.empty())
    {
        m_fullNameCpp = m_fullName;
        size_t pos = m_fullNameCpp.find(".");

        while (pos != std::string::npos)
        {
            m_fullNameCpp.replace(pos, 1, "::");
            pos = m_fullNameCpp.find(".", pos + 2);
        }
    }

    return m_fullNameCpp;
}

void static_projection_data::SortGlobalLists()
{
    std::sort(_interfaces.begin(), _interfaces.end(),
        [](auto const& lhs, auto const& rhs) { return (lhs->Name() < rhs->Name()); });
}










std::map<std::string_view, std::shared_ptr<Namespace>> Namespace::GetRoots(const Settings& settings)
{
    std::map<std::string_view, std::shared_ptr<Namespace>> roots;
    std::shared_ptr<Namespace> previousNamespace;
    for (const auto& [currentFullName, members] : settings.Cache.namespaces())
    {
        if (!IsNamespaceAllowed(settings, currentFullName, members))
        {
            continue;
        }

        std::string_view previousFullName = previousNamespace ? previousNamespace->FullName() : ""sv;
        if (previousNamespace && ((currentFullName.length() < previousFullName.length() + 1) ||
                                     (currentFullName[previousFullName.length()] != L'.') ||
                                     (currentFullName.compare(0, previousFullName.length(), previousFullName))))
        {
            // This namespace is not a child of 'previousNamespace' so ensure it from the root.
            previousNamespace = nullptr;
            previousFullName = ""sv;
        }

        // Namespaces returned from the cache are in order, so if there are any gaps we need to ensure an empty
        // parent namespace. e.g. if "a" is followed by "a.b.c" we should ensure "b" too. We also not need to handle
        // going from "a.b.c" to an unrelated "x.y".
        std::string_view remainingNameSegments =
            previousFullName.empty() ? currentFullName : currentFullName.substr(previousFullName.length() + 1);

        while (!remainingNameSegments.empty())
        {
            const auto delim = remainingNameSegments.find(".");
            const bool isFinalSegment = (delim == std::string_view::npos);

            std::string_view currentNameSegment;
            if (isFinalSegment)
            {
                std::swap(currentNameSegment, remainingNameSegments);
            }
            else
            {
                currentNameSegment = remainingNameSegments.substr(0, delim);
                remainingNameSegments.remove_prefix(delim + 1);
            }

            auto& targetMap = previousNamespace ? previousNamespace->m_children : roots;
            if (auto it = targetMap.find(currentNameSegment); it != targetMap.end())
            {
                previousNamespace = it->second;
            }
            else
            {
                previousNamespace = std::make_shared<Namespace>(
                    previousNamespace, std::string(currentNameSegment), isFinalSegment ? &members : nullptr);
                targetMap[previousNamespace->Name()] = previousNamespace;
            }
        }
    }
    return roots;
}

Namespace::Namespace(const std::shared_ptr<Namespace>& parent, std::string&& name,
    const winmd::reader::cache::namespace_members* members) :
    m_name(std::forward<std::string>(name)),
    m_members(members),
    m_parent(parent ? std::optional<std::weak_ptr<Namespace>>(parent) : std::optional<std::weak_ptr<Namespace>>())
{
}

const std::string& Namespace::Name() const noexcept
{
    return m_name;
}

const std::string& Namespace::FullName(bool useCppDelim) const
{
    auto& fullName = useCppDelim ? m_fullNameCpp : m_fullName;
    if (fullName.empty())
    {
        if (const auto parent = Parent())
        {
            fullName = parent->FullName(useCppDelim) + (useCppDelim ? "::" : ".") + m_name;
        }
        else
        {
            fullName = m_name;
        }
    }
    return fullName;
}

const winmd::reader::cache::namespace_members& Namespace::Members() const noexcept
{
    if (!m_members)
    {
        static const winmd::reader::cache::namespace_members s_empty{};
        return s_empty;
    }

    return *m_members;
}

bool Namespace::HasProjectedTypes() const noexcept
{
    return m_members;
}

std::shared_ptr<Namespace> Namespace::Parent() const
{
    if (m_parent)
    {
        if (auto parent = m_parent->lock())
        {
            return parent;
        }
        ThrowInvalidArg("Parent Namespace node has expired!");
    }
    return nullptr;
}

const std::map<std::string_view, std::shared_ptr<Namespace>>& Namespace::Children() const noexcept
{
    return m_children;
}

uint32_t Namespace::Depth() const
{
    if (!m_depth)
    {
        if (const auto parent = Parent())
        {
            m_depth = 1 + parent->Depth();
        }
        else
        {
            m_depth = 0;
        }
    }

    return *m_depth;
}

bool Namespace::Enumerate(const std::function<bool(const Namespace&)>& fn, bool postorder) const
{
    if (!postorder)
    {
        if (!fn(*this))
        {
            return false;
        }
    }

    for (const auto& child : m_children)
    {
        if (!child.second->Enumerate(fn, postorder))
        {
            return false;
        }
    }

    if (postorder)
    {
        if (!fn(*this))
        {
            return false;
        }
    }

    return true;
}

bool Namespace::EnumerateAll(const std::map<std::string_view, std::shared_ptr<Namespace>>& namespaces,
    const std::function<bool(const Namespace&)>& fn, bool postorder)
{
    for (const auto& child : namespaces)
    {
        if (!child.second->Enumerate(fn, postorder))
        {
            return false;
        }
    }
    return true;
}

bool HasAllowedTypes(const Settings& settings, const winmd::reader::cache::namespace_members& members)
{
    for (const auto& typeDef : members.interfaces)
    {
        if (IsTypeAllowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.classes)
    {
        if (IsTypeAllowed(settings, typeDef, true /*isClass*/))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.enums)
    {
        if (IsTypeAllowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.structs)
    {
        if (IsTypeAllowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.delegates)
    {
        if (IsTypeAllowed(settings, typeDef))
        {
            return true;
        }
    }

    return false;
}

bool IsNamespaceAllowed(const Settings& settings, const std::string_view& namespaceFullName,
    const winmd::reader::cache::namespace_members& members)
{
    if (!HasAllowedTypes(settings, members))
    {
        return false;
    }

    return true;
}

bool IsMethodAllowed(const Settings& settings, const winmd::reader::MethodDef& methodDef)
{
    if (!settings.IncludeDeprecated &&
        HasAttribute(methodDef, "Windows.Foundation.Metadata"sv, "DeprecatedAttribute"sv))
    {
        return false;
    }

    if (!settings.IncludeWebHostHidden &&
        HasAttribute(methodDef, "Windows.Foundation.Metadata"sv, "WebHostHiddenAttribute"sv))
    {
        return false;
    }

    return true;
}

bool IsTypeAllowed(const Settings& settings, const winmd::reader::TypeDef& typeDef, const bool isClass)
{
    if (!settings.Filter.Includes(typeDef))
    {
        return false;
    }

    if (isClass && settings.FilterToAllowForWeb &&
        !HasAttribute(typeDef, "Windows.Foundation.Metadata"sv, "AllowForWebAttribute"sv))
    {
        return false;
    }

    if (!settings.IncludeDeprecated && HasAttribute(typeDef, "Windows.Foundation.Metadata"sv, "DeprecatedAttribute"sv))
    {
        return false;
    }

    if (!settings.IncludeWebHostHidden &&
        HasAttribute(typeDef, "Windows.Foundation.Metadata"sv, "WebHostHiddenAttribute"sv))
    {
        // Special-case Windows.Foundation.IPropertyValueStatics which should not be WebHostHidden as it breaks basic
        // language features by omission.
        if (typeDef.TypeNamespace() == "Windows.Foundation"sv)
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

std::vector<ActivationFactoryInterfaceInfo> GetActivationFactoryInterfaces(const winmd::reader::TypeDef& classDef)
{
    auto GetSystemTypeDef = [&](const winmd::reader::CustomAttributeSig& signature) -> winmd::reader::TypeDef {
        for (const auto& arg : signature.FixedArgs())
        {
            if (const auto typeParam =
                    std::get_if<winmd::reader::ElemSig::SystemType>(&std::get<winmd::reader::ElemSig>(arg.value).value))
            {
                return classDef.get_cache().find_required(typeParam->name);
            }
        }
        return {};
    };

    std::vector<ActivationFactoryInterfaceInfo> result;

    for (auto&& attribute : classDef.CustomAttribute())
    {
        auto attribute_name = attribute.TypeNamespaceAndName();

        if (attribute_name.first != "Windows.Foundation.Metadata"sv)
        {
            continue;
        }

        auto signature = attribute.Value();
        ActivationFactoryInterfaceInfo info;

        if (attribute_name.second == "ActivatableAttribute"sv)
        {
            info.TypeDef = GetSystemTypeDef(signature);
            info.IsActivatable = true;
        }
        else if (attribute_name.second == "StaticAttribute"sv)
        {
            info.TypeDef = GetSystemTypeDef(signature);
            info.IsStatics = true;
        }
        else if (attribute_name.second == "ComposableAttribute"sv)
        {
            info.TypeDef = GetSystemTypeDef(signature);
            info.IsComposable = true;

            for (auto&& arg : signature.FixedArgs())
            {
                if (auto visibility = std::get_if<winmd::reader::ElemSig::EnumValue>(
                        &std::get<winmd::reader::ElemSig>(arg.value).value))
                {
                    info.IsVisible = std::get<int32_t>(visibility->value) == 2;
                    break;
                }
            }
        }
        else
        {
            continue;
        }

        std::string name;
        if (info.TypeDef)
        {
            name = std::string(info.TypeDef.TypeNamespace()) + "::" + std::string(info.TypeDef.TypeName());
        }

        // If activatable and without a TypeDef, then it it activable with zero-params with the ClassicCOM
        // IActivationFactory.
        result.push_back(std::move(info));
    }

    return result;
}

MethodOverloadInfo::MethodOverloadInfo(winmd::reader::MethodDef def) : Def(std::move(def))
{
    const auto signature = def.Signature();
    const auto returnType = signature.ReturnType();
    const auto paramSigs = signature.Params();
    auto paramList = def.ParamList();

    // The first param might be the return value unlike the first param signature.
    const auto paramCount = static_cast<uint32_t>(std::distance(paramSigs.first, paramSigs.second));
    if (returnType && (paramList.first != paramList.second) && (paramList.first.Sequence() == 0))
    {
        ++paramList.first;
    }

    // Special-case out-params corresponding to out-param arrays since the caller passes in the array to write to. For
    // example: "HRESULT II2cDevice::Read([range(0x00000000, 0x7FFFFFFF)] [in] UINT32 __bufferSize, [out]
    // [size_is(__bufferSize)] BYTE* buffer)" is treated like "WINRT_IMPL_AUTO(void) Read(array_view<uint8_t> buffer)
    // const" by C++/WinRT and similarly JS would be expected to pass in an-array.

    for (uint32_t i = 0; i < paramCount; ++i)
    {
        const auto& param = paramList.first[i];
        const auto& paramSig = paramSigs.first[i];
        if ((param.Flags().In()) || (paramSig.Type().is_szarray() && !paramSig.ByRef()))
        {
            ++InParamCount;
        }
    }

    OutParamCount = static_cast<uint32_t>(std::distance(paramSigs.first, paramSigs.second)) - InParamCount;
    IsDefaultOverload = HasAttribute(Def, "Windows.Foundation.Metadata"sv, "DefaultOverloadAttribute"sv);
}

InterfaceInfo GetInterfaceInfo(const winmd::reader::TypeDef& interfaceDef)
{
    InterfaceInfo result;

    for (const auto& methodDef : interfaceDef.MethodList())
    {
        auto name = methodDef.Name();
        if (methodDef.SpecialName())
        {
            const auto delim = name.find('_');
            if (delim != std::string_view::npos)
            {
                const auto prefix = name.substr(0, delim);
                name = name.substr(delim + 1);

                if (prefix == "get"sv)
                {
                    result.Properties[name].GetterDef = methodDef;
                }
                else if (prefix == "put"sv)
                {
                    result.Properties[name].SetterDef = methodDef;
                }
                else if (prefix == "add"sv)
                {
                    result.Events[name].AddDef = methodDef;
                }
                else if (prefix == "remove"sv)
                {
                    result.Events[name].RemoveDef = methodDef;
                }
            }
        }
        else
        {
            MethodOverloadInfo info(methodDef);
            result.Methods[name][info.InParamCount].push_back(std::move(info));
        }
    }

    return result;
}

bool IsSpecialOmittedStruct(const winmd::reader::TypeDef& type)
{
    // C++/WinRT tries to make things easier in some cases but that would break code gen for value conversion.
    // There are also standard types that we may need to project differently anyway for easy of use in JS.

    if (type.TypeNamespace() == "Windows.Foundation"sv)
    {
        // clang-format off
        static const std::set<std::string_view> c_typeNames = {
            // DateTime and TimeSpan are converted to C++ types unlike the original projections so they need to
            // be custom implemented not just because of JS's incompatible handling of time (i.e., in ms).
            "Windows.Foundation", "DateTime"sv,
            "Windows.Foundation", "TimeSpan"sv,

            // C++/WinRT projects this as winrt::event_token but this will not be exposed to JS as events are projected
            // as addEventListener/removeEventListener matching HTML DOM semantics.
            "Windows.Foundation", "EventRegistrationToken"sv,

            // winrt::hresult is used instead of Windows.Foundation.HResult but JS projects this as a plain number.
            "Windows.Foundation", "HResult"sv,

            // Point, Rect, Size can be code-gen'd without any problems. C++/WinRT does implement helpers structs but
            // it keeps field names matching the IDL so there is not need to special-case them.
        };
        // clang-format on
        return c_typeNames.find(type.TypeName()) != c_typeNames.end();
    }
    return false;
}

bool UseLowerCamelCaseCodeGenForStruct(const winmd::reader::TypeDef& type)
{
    // The following could almost be code-gen'd as-is except the C++/WinRT equivalents use lower camel case
    // rather than upper camel (Pascal) case (e.g. Matrix4x4.M22 is float4x4.m22)
    if (type.TypeNamespace() == "Windows.Foundation.Numerics"sv)
    {
        static const std::set<std::string_view> c_typeNames = {
            "Matrix3x2"sv,
            "Matrix4x4"sv,
            "Plane"sv,
            "Quaternion"sv,
            "Vector2"sv,
            "Vector3"sv,
            "Vector4"sv,
        };

        return c_typeNames.find(type.TypeName()) != c_typeNames.end();
    }
    return false;
}
#endif
