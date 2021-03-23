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
inline bool has_attribute(const T& row, std::string_view attributeNamespace, std::string_view attributeName)
{
    return static_cast<bool>(winmd::reader::get_attribute(row, attributeNamespace, attributeName));
}

template <typename T>
inline bool is_allow_for_web(const T& row)
{
    using namespace std::literals;
    return has_attribute(row, metadata_namespace, "AllowForWebAttribute"sv);
}

template <typename T>
inline bool is_deprecated(const T& row)
{
    using namespace std::literals;
    return has_attribute(row, metadata_namespace, "DeprecatedAttribute"sv);
}

template <typename T>
inline bool is_web_host_hidden(const T& row)
{
    using namespace std::literals;
    return has_attribute(row, metadata_namespace, "WebHostHiddenAttribute"sv);
}

template <typename T>
inline bool is_activatable(const T& row)
{
    return has_attribute(row, metadata_namespace, activatable_attribute);
}

template <typename T>
inline bool is_default_overload(const T& row)
{
    using namespace std::literals;
    return has_attribute(row, metadata_namespace, "DefaultOverloadAttribute"sv);
}

inline bool is_default_interface(const winmd::reader::InterfaceImpl& ifaceImpl)
{
    using namespace std::literals;
    return has_attribute(ifaceImpl, metadata_namespace, "DefaultAttribute"sv);
}

winmd::reader::coded_index<winmd::reader::TypeDefOrRef> default_interface(const winmd::reader::TypeDef& classType);
winmd::reader::TypeDef exclusiveto_class(winmd::reader::TypeDef iface);

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
    using namespace winmd::reader;
    return distance(typeDef.GenericParam()) != 0;
}

winmd::reader::TypeDef generic_type_def(const winmd::reader::GenericTypeInstSig& sig);

winmd::reader::MethodDef delegate_invoke_function(const winmd::reader::TypeDef& typeDef);

inline winmd::reader::ElementType underlying_enum_type(const winmd::reader::TypeDef& type)
{
    return std::get<winmd::reader::ElementType>(type.FieldList().first.Signature().Type().Type());
}

inline std::array<char, 37> guid_to_string(const GUID& guid)
{
    std::array<char, 37> data;
    std::snprintf(data.data(), data.size(), "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", guid.Data1, guid.Data2,
        guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);
    return data;
}

GUID get_interface_guid(const winmd::reader::TypeDef& typeDef);

// .NET metadata is a pretty poor solution for representing WinRT metadata, and the complexity of parameters is one
// place where this is obvious as the data about parameters is split up into two separate, and diferently sized lists
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

inline param_iterator begin(const std::pair<param_iterator, param_iterator>& pair)
{
    return pair.first;
}

inline param_iterator end(const std::pair<param_iterator, param_iterator>& pair)
{
    return pair.second;
}

// Tag type used to represent the 'System.Guid' a.k.a. 'Windows.Foundation.Guid' type, which has no 'TypeDef'
struct system_guid_t {};
inline constexpr system_guid_t system_guid{};

using resolved_type =
    std::variant<winmd::reader::ElementType, winmd::reader::TypeDef, winmd::reader::GenericTypeInstSig, system_guid_t>;

// Generic instantiations is another place where WinRT metadata fails to provide simplicity, instead making things far
// more complex and difficult to deal with. This attempts to unwind some of that so that instantiations can be
// stored/observed separately, instead of requiring full state of some "root" instantiation
struct generic_param_stack
{
    // NOTE: Generic params need to be represented as a "stack" since individual params may may need to reference
    // earlier params. E.g. for the type 'IIterable<IKeyValuePair<String, String>>', the 'IIterable' type has a single
    // generic param, which itself is another generic instantiation. Instead of doing the sane, obvious thing and have
    // that second instantiation reference the two types it needs - [ String, String ] - it may be the case that these
    // two types reference previous generic params, before the representation of 'IIterable'. E.g. 'IMapView<K, V>',
    // which derives from 'IIterable<IKeyValuePair<K, V>>'
    std::vector<std::vector<resolved_type>> values;
#if 0
    using value_type = std::variant<winmd::reader::ElementType, winmd::reader::TypeDef,
        winmd::reader::GenericTypeInstSig, winmd::reader::GenericTypeIndex, system_guid_t>;
    std::vector<std::vector<value_type>> values;

    // Helper for accessing a param by index
    template <typename Visitor>
    auto visit(uint32_t index, Visitor&& visitor)
    {
        using namespace winmd::reader;
        for (auto itr = values.rbegin(); itr != values.rend(); ++itr)
        {
            auto& list = *itr;
            if (index >= list.size())
                throw std::range_error("GenericTypeIndex out of range");

            auto& elem = list[index];
            if (std::holds_alternative<GenericTypeIndex>(elem))
            {
                index = std::get<GenericTypeIndex>(elem).index;
            }
            else
            {
                return std::visit(overloaded{
                                      [&](auto&& value) { return visitor(std::forward<decltype(value)>(value)); },
                                      [](GenericTypeIndex) { std::terminate(); }, // Unreachable
                                  },
                    elem);
            }
        }

        throw std::runtime_error("GenericTypeIndex reference a type that does not exist");
    }

    template <typename Visitor>
    auto visit(winmd::reader::GenericTypeIndex index, Visitor&& visitor)
    {
        return visit(index.index, std::forward<Visitor>(visitor));
    }
#endif
};

struct generic_instantiation
{
    winmd::reader::GenericTypeInstSig signature;
    generic_param_stack params;

    generic_instantiation(winmd::reader::GenericTypeInstSig signature, const generic_param_stack& parentStack);
};

// NOTE: All of the below 'resolve_type' calls will get you one of:
//      * ElementType
//      * TypeDef
//      * generic_instantiation
//      * system_guid_t
template <typename Func>
inline void resolve_type(winmd::reader::ElementType type, const generic_param_stack&, Func&& fn)
{
    fn(type);
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeDef& typeDef, const generic_param_stack&, Func&& fn)
{
    fn(typeDef);
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeRef& typeRef, const generic_param_stack& genericParamStack, Func&& fn)
{
    if ((typeRef.TypeNamespace() == "System") && (typeRef.TypeName() == "Guid"))
    {
        fn(system_guid);
    }
    else
    {
        resolve_type(winmd::reader::find_required(typeRef), genericParamStack, std::forward<Func>(fn));
    }
}

template <typename Func>
inline void resolve_type(
    const winmd::reader::GenericTypeInstSig& sig, const generic_param_stack& parentStack, Func&& fn)
{
    fn(generic_instantiation(sig, parentStack));
}

template <typename Func>
inline void resolve_type(const winmd::reader::coded_index<winmd::reader::TypeDefOrRef>& defOrRef,
    const generic_param_stack& parentStack, Func&& fn)
{
    using namespace winmd::reader;
    switch (defOrRef.type())
    {
    case TypeDefOrRef::TypeDef:
        resolve_type(defOrRef.TypeDef(), parentStack, std::forward<Func>(fn));
        break;
    case TypeDefOrRef::TypeRef:
        resolve_type(defOrRef.TypeRef(), parentStack, std::forward<Func>(fn));
        break;
    case TypeDefOrRef::TypeSpec:
        resolve_type(defOrRef.TypeSpec().Signature().GenericTypeInst(), parentStack, std::forward<Func>(fn));
        break;
    }
}

template <typename Func>
inline void resolve_type(
    const winmd::reader::GenericTypeIndex& idx, const generic_param_stack& genericParamStack, Func&& fn)
{
    auto& paramList = genericParamStack.values.back();
    if (idx.index >= paramList.size())
    {
        __debugbreak();
        throw std::runtime_error("GenericTypeIndex out of range");
    }

    std::visit(overloaded{
                   [&](const winmd::reader::GenericTypeInstSig& sig) {
                       __debugbreak();
                       auto stack = genericParamStack;
                       stack.values.pop_back();
                       fn(generic_instantiation(sig, std::move(stack)));
                   },
                   [&](auto&& value) { fn(value); },
               },
        paramList[idx.index]);
}

template <typename Func>
inline void resolve_type(const winmd::reader::GenericMethodTypeIndex&, const generic_param_stack&, Func&&)
{
    throw std::runtime_error("Unexpected GenericMethodTypeIndex");
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeSig& sig, const generic_param_stack& genericParamStack, Func&& fn)
{
    std::visit(
        [&](auto&& value) {
            resolve_type(value, genericParamStack, [&](auto&& resolvedValue) {
                fn(std::forward<decltype(resolvedValue)>(resolvedValue), sig.is_szarray());
            });
        },
        sig.Type());
}

template <typename Func>
inline void resolve_type(const winmd::reader::ParamSig& sig, const generic_param_stack& genericParamStack, Func&& fn)
{
    resolve_type(sig.Type(), genericParamStack, std::forward<Func>(fn));
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
