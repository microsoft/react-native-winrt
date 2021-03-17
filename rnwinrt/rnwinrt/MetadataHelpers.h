#pragma once
#include "Settings.h"

struct file_writer;

inline constexpr const std::string_view foundation_namespace = "Windows.Foundation";
inline constexpr const std::string_view metadata_namespace = "Windows.Foundation.Metadata";

inline constexpr const std::string_view activatable_attribute = "ActivatableAttribute";
inline constexpr const std::string_view static_attribute = "StaticAttribute";

struct named_projection_data
{
    enum class kind
    {
        enum_data = 1,
        class_data = 2,
        namespace_data = 3,
    };

    named_projection_data(kind dataKind) : data_kind(dataKind)
    {
    }

    named_projection_data(kind dataKind, std::string_view name, std::string_view baseNamespace) :
        data_kind(dataKind), name(name), base_namespace(baseNamespace)
    {
    }

    virtual ~named_projection_data() {}

    // E.g. if the name is 'Foo.Bar.Baz':
    kind data_kind;
    std::string_view name; // 'Baz'
    std::string_view base_namespace; // 'Foo.Bar'
};

struct enum_projection_data : named_projection_data
{
    enum_projection_data(winmd::reader::TypeDef typeDef) :
        named_projection_data(kind::enum_data, typeDef.TypeName(), typeDef.TypeNamespace()),
        type_def(typeDef)
    {
    }

    winmd::reader::TypeDef type_def;
};

struct class_projection_data : named_projection_data
{
    class_projection_data(winmd::reader::TypeDef typeDef) :
        named_projection_data(kind::class_data, typeDef.TypeName(), typeDef.TypeNamespace()),
        type_def(typeDef)
    {
    }

    winmd::reader::TypeDef type_def;
};

struct interface_projection_data
{
    interface_projection_data(winmd::reader::TypeDef typeDef) : type_def(typeDef)
    {
    }

    // TODO

    winmd::reader::TypeDef type_def;
};

struct struct_projection_data
{
    struct_projection_data(winmd::reader::TypeDef typeDef) : type_def(typeDef)
    {
    }

    // TODO

    winmd::reader::TypeDef type_def;
};

struct delegate_projection_data
{
    delegate_projection_data(winmd::reader::TypeDef typeDef) : type_def(typeDef)
    {
    }

    // TODO

    winmd::reader::TypeDef type_def;
};

struct namespace_projection_data : named_projection_data
{
    namespace_projection_data(std::string_view fullName) :
        named_projection_data(kind::namespace_data), full_name(fullName)
    {
        if (auto pos = fullName.find_last_of('.'); pos == std::string_view::npos)
        {
            name = fullName;
        }
        else
        {
            base_namespace = fullName.substr(0, pos);
            name = fullName.substr(pos + 1);
        }
    }

    std::string_view full_name;

    std::vector<named_projection_data*> named_children;
    std::vector<std::unique_ptr<namespace_projection_data>> namespace_children;
    std::vector<std::unique_ptr<enum_projection_data>> enum_children;
    std::vector<std::unique_ptr<class_projection_data>> class_children;

    std::vector<std::unique_ptr<struct_projection_data>> struct_children;
    std::vector<std::unique_ptr<delegate_projection_data>> delegate_children;
    std::vector<std::unique_ptr<interface_projection_data>> interface_children;
};

struct projection_data
{
    std::vector<std::unique_ptr<namespace_projection_data>> root_namespaces;

    // Global lists
    std::vector<interface_projection_data*> interfaces;
};

void parse_metadata(const Settings& settings, projection_data& data);

inline constexpr std::pair<std::string_view, std::string_view> numerics_mappings[] = {
    { "Matrix3x2", "float3x2" },
    { "Matrix4x4", "float4x4" },
    { "Plane", "plane" },
    { "Quaternion", "quaternion" },
    { "Vector2", "float2" },
    { "Vector3", "float3" },
    { "Vector4", "float4" },
};

inline std::pair<std::string_view, std::string_view> separate_typename(std::string_view fullName)
{
    auto pos = fullName.find_last_of('.');
    if (pos == std::string_view::npos)
    {
        return { {}, fullName };
    }

    return { fullName.substr(0, pos), fullName.substr(pos + 1) };
}

inline std::string_view remove_tick(std::string_view str)
{
    return str.substr(0, str.find_first_of('`'));
}

template <typename T>
bool has_attribute(const T& row, std::string_view attributeNamespace, std::string_view attributeName)
{
    return static_cast<bool>(winmd::reader::get_attribute(row, attributeNamespace, attributeName));
}

template <typename T>
bool is_allow_for_web(const T& row)
{
    using namespace std::literals;
    return has_attribute(row, metadata_namespace, "AllowForWebAttribute"sv);
}

template <typename T>
bool is_deprecated(const T& row)
{
    using namespace std::literals;
    return has_attribute(row, metadata_namespace, "DeprecatedAttribute"sv);
}

template <typename T>
bool is_web_host_hidden(const T& row)
{
    using namespace std::literals;
    return has_attribute(row, metadata_namespace, "WebHostHiddenAttribute"sv);
}

inline std::optional<winmd::reader::TypeDef> exclusiveto_class(winmd::reader::TypeDef iface)
{
    using namespace std::literals;
    using namespace winmd::reader;
    auto attr = get_attribute(iface, metadata_namespace, "ExclusiveToAttribute"sv);
    if (!attr)
        return std::nullopt;

    auto sig = attr.Value();
    assert(sig.FixedArgs().size() == 1);
    auto sysType = std::get<ElemSig::SystemType>(std::get<ElemSig>(sig.FixedArgs()[0].value).value);

    return iface.get_database().get_cache().find_required(sysType.name);
}

template <typename T, typename Func>
inline void for_each_attribute(const T& type, std::string_view expectNs, std::string_view expectName, Func&& callback)
{
    for (auto&& attr : type.CustomAttribute())
    {
        auto [ns, name] = attr.TypeNamespaceAndName();
        if ((ns == expectNs) && (name == expectName))
        {
            callback(attr);
        }
    }
}

inline bool is_factory_interface(winmd::reader::TypeDef iface)
{
    using namespace std::literals;
    using namespace winmd::reader;
    auto classDef = exclusiveto_class(iface);
    if (!classDef)
    {
        return false;
    }

    for (auto&& attr : classDef->CustomAttribute())
    {
        auto [attrNs, attrName] = attr.TypeNamespaceAndName();
        if (attrNs != metadata_namespace)
            continue;

        if ((attrName != static_attribute) && (attrName != activatable_attribute))
            continue;

        // NOTE: Both static and activatable attribute have the type as the first parameter to the constructor. Also
        // note that types can be activatable w/ no factory interface (i.e. use IActivationFactory) and therefore the
        // first parameter may not be a type
        auto sig = attr.Value();
        auto& args = sig.FixedArgs();
        if (args.empty())
            continue;

        auto type = std::get_if<ElemSig::SystemType>(&std::get<ElemSig>(args[0].value).value);
        if (!type)
            continue;

        auto [typeNs, typeName] = separate_typename(type->name);
        if ((typeNs == iface.TypeNamespace()) && (typeName == iface.TypeName()))
        {
            return true;
        }
    }

    return false;
}

inline constexpr bool is_child_namespace(std::string_view expectedChild, std::string_view expectedParent)
{
    if (expectedChild.size() >= expectedParent.size())
    {
        return false;
    }

    if (expectedChild != expectedParent.substr(0, expectedChild.size()))
    {
        return false;
    }

    return expectedParent[expectedChild.size()] == '.';
}

inline bool is_generic(winmd::reader::TypeDef typeDef)
{
    auto pair = typeDef.GenericParam();
    return std::distance(pair.first, pair.second) != 0;
}











#if 1
struct static_projection_data : public std::enable_shared_from_this<static_projection_data>
{
    static std::vector<std::shared_ptr<static_projection_data>> ParseMetaData(const Settings& settings);

    static void SortGlobalLists();

    static_projection_data(std::string_view name, std::string_view fullName) : m_name(name), m_fullName(fullName)
    {
    }

    const std::string& Name() const noexcept;
    const std::string& FullName(bool useCppDelim = false) const;

    // virtual jsi::Value create(jsi::Runtime& runtime) const override;
    virtual std::vector<std::shared_ptr<static_projection_data>>* Children()
    {
        return {};
    }

private:
    const std::string m_name;
    const std::string m_fullName;

    mutable std::string m_fullNameCpp;
};

struct static_class_data final : static_projection_data
{
    static_class_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_enum_data final : static_projection_data
{
    static_enum_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_struct_data final : static_projection_data
{
    static_struct_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_delegate_data final : static_projection_data
{
    static_delegate_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_interface_data final : static_projection_data
{
    static_interface_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};
/*
struct static_generic_interface_data final : static_projection_data
{
    static_interface_data(std::string_view name, std::string_view fullName) :
        static_projection_data(name, fullName)
    {
    }

private:

};
*/
struct static_namespace_data final : static_projection_data
{
    static_namespace_data(std::string_view name, std::string_view fullName,
        const winmd::reader::cache::namespace_members* members, const Settings& settings) :
        static_projection_data(name, fullName)
    {
        ParseChildren(members, settings);
    }

    virtual std::vector<std::shared_ptr<static_projection_data>>* Children() override
    {
        return &m_children;
    }

    std::vector<std::shared_ptr<static_projection_data>> m_children;
    std::vector<std::shared_ptr<static_interface_data>> m_interfaces;
    std::vector<std::shared_ptr<static_struct_data>> m_structs;
    std::vector<std::shared_ptr<static_delegate_data>> m_delegates;

private:
    void ParseChildren(const winmd::reader::cache::namespace_members* members, const Settings& settings);

    template <typename T, typename U>
    void ParseTypeDefs(std::vector<std::shared_ptr<T>>& items, const std::vector<winmd::reader::TypeDef>& typeDefs,
        const Settings& settings, std::vector<std::shared_ptr<static_projection_data>>* globalList);
};

extern std::vector<std::shared_ptr<static_projection_data>> _interfaces;
extern std::vector<std::shared_ptr<static_projection_data>> _structs;
extern std::vector<std::shared_ptr<static_projection_data>> _delegates;










struct Namespace : public std::enable_shared_from_this<Namespace>
{
    static std::map<std::string_view, std::shared_ptr<Namespace>> GetRoots(const Settings& settings);

    Namespace(const std::shared_ptr<Namespace>& parent, std::string&& name,
        const winmd::reader::cache::namespace_members* members = nullptr);

    const std::string& Name() const noexcept;
    const std::string& FullName(bool useCppDelim = false) const;
    const winmd::reader::cache::namespace_members& Members() const noexcept;

    bool HasProjectedTypes() const noexcept;

    std::shared_ptr<Namespace> Parent() const;
    const std::map<std::string_view, std::shared_ptr<Namespace>>& Children() const noexcept;
    uint32_t Depth() const;

    bool Enumerate(const std::function<bool(const Namespace&)>& fn, bool postorder = false) const;
    static bool EnumerateAll(const std::map<std::string_view, std::shared_ptr<Namespace>>& namespaces,
        const std::function<bool(const Namespace&)>& fn, bool postorder = false);

private:
    const std::optional<std::weak_ptr<Namespace>> m_parent;
    const std::string m_name;
    const winmd::reader::cache::namespace_members* m_members;

    std::map<std::string_view, std::shared_ptr<Namespace>> m_children;
    mutable std::optional<uint32_t> m_depth;
    mutable std::string m_fullName;
    mutable std::string m_fullNameCpp;
};

bool IsNamespaceAllowed(const Settings& settings, const std::string_view& namespaceFullName,
    const winmd::reader::cache::namespace_members& members);
bool IsMethodAllowed(const Settings& settings, const winmd::reader::MethodDef& methodDef);
bool IsTypeAllowed(const Settings& settings, const winmd::reader::TypeDef& typeDef, const bool isClass = false);

template <typename T>
bool HasAttribute(T const& row, const std::string_view& attributeNamespace, const std::string_view& attributeName)
{
    return static_cast<bool>(winmd::reader::get_attribute(row, attributeNamespace, attributeName));
}

struct ActivationFactoryInterfaceInfo
{
    winmd::reader::TypeDef TypeDef;
    bool IsActivatable{};
    bool IsStatics{};
    bool IsComposable{};
    bool IsVisible{};
};
std::vector<ActivationFactoryInterfaceInfo> GetActivationFactoryInterfaces(const winmd::reader::TypeDef& classDef);

struct PropertyInfo
{
    winmd::reader::MethodDef GetterDef;
    winmd::reader::MethodDef SetterDef;
};

struct EventInfo
{
    winmd::reader::MethodDef AddDef;
    winmd::reader::MethodDef RemoveDef;
};

struct MethodOverloadInfo
{
    MethodOverloadInfo(winmd::reader::MethodDef def);

    winmd::reader::MethodDef Def;
    uint32_t InParamCount{};
    uint32_t OutParamCount{};
    bool IsDefaultOverload{};
};

struct InterfaceInfo
{
    std::map<std::string_view, PropertyInfo> Properties;
    std::map<std::string_view, EventInfo> Events;
    std::map<std::string_view, std::map<uint32_t, std::vector<MethodOverloadInfo>>> Methods;
};

InterfaceInfo GetInterfaceInfo(const winmd::reader::TypeDef& interfaceDef);

bool IsSpecialOmittedStruct(const winmd::reader::TypeDef& type);
bool UseLowerCamelCaseCodeGenForStruct(const winmd::reader::TypeDef& type);

// TODO: Replace with C++20's built-in method.
inline bool StartsWith(const std::string_view& lhs, const std::string_view& rhs)
{
    return !lhs.compare(0, rhs.length(), rhs);
};
#endif
