#include "pch.h"

#include "MetadataHelpers.h"

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