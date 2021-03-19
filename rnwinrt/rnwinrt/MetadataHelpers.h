#pragma once
#include "Settings.h"

struct file_writer;

inline constexpr const std::string_view foundation_namespace = "Windows.Foundation";
inline constexpr const std::string_view metadata_namespace = "Windows.Foundation.Metadata";

inline constexpr const std::string_view activatable_attribute = "ActivatableAttribute";
inline constexpr const std::string_view static_attribute = "StaticAttribute";

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

template <typename T>
bool is_activatable(const T& row)
{
    return has_attribute(row, metadata_namespace, activatable_attribute);
}

template <typename T>
bool is_default_overload(const T& row)
{
    return has_attribute(row, metadata_namespace, "DefaultOverloadAttribute"sv);
}

std::optional<winmd::reader::TypeDef> exclusiveto_class(winmd::reader::TypeDef iface);

bool is_factory_interface(winmd::reader::TypeDef iface);

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

winmd::reader::MethodDef delegate_invoke_function(const winmd::reader::TypeDef& typeDef);

// .NET metadata is a pretty poor solution for representing WinRT metadata, and the complexity of parameters is one
// place where this is obvious.
struct param_iterator
{
    using sig_iterator_t = std::vector<winmd::reader::ParamSig>::const_iterator;

    using difference_type = int32_t;
    using value_type = param_iterator;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    param_iterator() = default;

    param_iterator(sig_iterator_t sigItr, const winmd::reader::Param& param) : sig_iterator(sigItr), param(param)
    {
    }

    // Easy accessors
    std::string_view name() const
    {
        return param.Name();
    }

    bool is_input() const
    {
        auto result = param.Flags().In();
        assert(result != param.Flags().Out()); // Otherwise could just return '!is_output()'
        return result;
    }

    bool is_output() const
    {
        auto result = param.Flags().Out();
        assert(result != param.Flags().In());
        return result;
    }

    bool by_ref() const
    {
        return sig_iterator->ByRef();
    }

    const winmd::reader::ParamSig& signature() const
    {
        return *sig_iterator;
    }

    const winmd::reader::TypeSig& type() const
    {
        return sig_iterator->Type();
    }

    // RandomAccessIterator functions
    param_iterator& operator*()
    {
        return *this;
    }

    param_iterator* operator->()
    {
        return this;
    }

    bool operator!=(const param_iterator& other) const
    {
        auto result = sig_iterator != other.sig_iterator;
        assert(result == (param != other.param));
        return result;
    }

    bool operator==(const param_iterator& other) const
    {
        auto result = sig_iterator == other.sig_iterator;
        assert(result == (param == other.param));
        return result;
    }

    bool operator<(const param_iterator& other) const
    {
        auto result = sig_iterator < other.sig_iterator;
        assert(result == (param < other.param));
        return result;
    }

    bool operator<=(const param_iterator& other) const
    {
        auto result = sig_iterator <= other.sig_iterator;
        assert(result == (param <= other.param));
        return result;
    }

    bool operator>(const param_iterator& other) const
    {
        auto result = sig_iterator > other.sig_iterator;
        assert(result == (param > other.param));
        return result;
    }

    bool operator>=(const param_iterator& other) const
    {
        auto result = sig_iterator >= other.sig_iterator;
        assert(result == (param >= other.param));
        return result;
    }

    param_iterator& operator++()
    {
        ++sig_iterator;
        ++param;
        return *this;
    }

    param_iterator operator++(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    param_iterator& operator+=(difference_type count)
    {
        sig_iterator += count;
        param += count;
        return *this;
    }

    friend param_iterator operator+(difference_type count, param_iterator itr)
    {
        itr += count;
        return itr;
    }

    friend param_iterator operator+(param_iterator itr, difference_type count)
    {
        itr += count;
        return itr;
    }

    param_iterator& operator--()
    {
        --sig_iterator;
        --param;
        return *this;
    }

    param_iterator operator--(int)
    {
        auto copy = *this;
        --(*this);
        return copy;
    }

    param_iterator& operator-=(difference_type count)
    {
        sig_iterator -= count;
        param -= count;
        return *this;
    }

    param_iterator operator-(difference_type count) const
    {
        auto copy = *this;
        copy -= count;
        return copy;
    }

    difference_type operator-(const param_iterator& other) const
    {
        auto result = sig_iterator - other.sig_iterator;
        assert(result == (param - other.param));
        return static_cast<difference_type>(result);
    }

    param_iterator operator[](difference_type pos) const
    {
        auto copy = *this;
        copy += pos;
        return copy;
    }

    sig_iterator_t sig_iterator;
    winmd::reader::Param param;
};

// NOTE: All of the below 'resolve_type' calls will get you one of:
//      * ElementType
//      * system_guid_t
//      * TypeDef
//      * GenericTypeInstSig
struct system_guid_t {};
inline constexpr system_guid_t system_guid{};

template <typename Func>
inline void resolve_type(winmd::reader::ElementType type, Func&& fn)
{
    fn(type);
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeDef& typeDef, Func&& fn)
{
    fn(typeDef);
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeRef& typeRef, Func&& fn)
{
    if ((typeRef.TypeNamespace() == "System") && (typeRef.TypeName() == "Guid"))
    {
        fn(system_guid);
    }
    else
    {
        resolve_type(winmd::reader::find_required(typeRef), std::forward<Func>(fn));
    }
}

template <typename Func>
inline void resolve_type(const winmd::reader::GenericTypeInstSig& sig, Func&& fn)
{
    fn(sig);
}

template <typename Func>
inline void resolve_type(const winmd::reader::coded_index<winmd::reader::TypeDefOrRef>& defOrRef, Func&& fn)
{
    using namespace winmd::reader;
    switch (defOrRef.type())
    {
    case TypeDefOrRef::TypeDef:
        resolve_type(defOrRef.TypeDef(), std::forward<Func>(fn));
        break;
    case TypeDefOrRef::TypeRef:
        resolve_type(defOrRef.TypeRef(), std::forward<Func>(fn));
        break;
    case TypeDefOrRef::TypeSpec:
        resolve_type(defOrRef.TypeSpec().Signature().GenericTypeInst(), std::forward<Func>(fn));
        break;
    }
}

template <typename Func>
inline void resolve_type(const winmd::reader::GenericTypeIndex&, Func&&)
{
    throw std::runtime_error("Unexpected GenericTypeIndex");
}

template <typename Func>
inline void resolve_type(const winmd::reader::GenericMethodTypeIndex&, Func&&)
{
    throw std::runtime_error("Unexpected GenericMethodTypeIndex");
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeSig& sig, Func&& fn)
{
    std::visit([&](auto&& value) {
        resolve_type(value, [&](auto&& resolvedValue) {
            fn(std::forward<decltype(resolvedValue)>(resolvedValue), sig.is_szarray());
        });
    }, sig.Type());
}

template <typename Func>
inline void resolve_type(const winmd::reader::ParamSig& sig, Func&& fn)
{
    resolve_type(sig.Type(), std::forward<Func>(fn));
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
