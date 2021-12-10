// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once
#include "Settings.h"

struct file_writer;

inline constexpr const std::string_view foundation_namespace = "Windows.Foundation";
inline constexpr const std::string_view metadata_namespace = "Windows.Foundation.Metadata";

inline constexpr const std::string_view activatable_attribute = "ActivatableAttribute";
inline constexpr const std::string_view static_attribute = "StaticAttribute";
inline constexpr const std::string_view protected_attribute = "ProtectedAttribute";
inline constexpr const std::string_view overridable_attribute = "OverridableAttribute";

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

bool should_project_interface(const winmd::reader::TypeDef& iface);

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
    std::snprintf(data.data(), data.size(), "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", guid.Data1, guid.Data2,
        guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);
    return data;
}

GUID get_interface_guid(const winmd::reader::TypeDef& typeDef);

// Function parameters in metadata can be quite annoying to deal with since info is split into two lists, when really we
// want it to behave as one. This is an iterator type that makes it appear as though the two are represented as one,
// though the responsibility of constructing it correctly falls on the consumer.
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
        // Not everything is marked in/out...
        return !is_output();
    }

    bool is_output() const
    {
        return param.Flags().Out();
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

#if 0 // TODO: Until https://github.com/microsoft/winmd/pull/20 is available via NuGet
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
#endif

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
struct system_guid_t
{
};
inline constexpr system_guid_t system_guid{};

// Genric types can reference types that consume the generic arguments in semi-non-direct ways, which makes resolving
// each a bit tricky. E.g. 'IMapView<K, V>' derives from 'IIterable<IKeyValuePair<K, V>>', which has the function
// 'First' which returns an 'IIterator<IKeyValuePair<K, V>>'. From the 'First' function's perspective, it just returns
// an 'IIterator<T>' where the 'T' is the one provided to 'IIterable<T>'. In order to resolve that that 'T' is
// 'IKeyValuePair<K, V>', we have to "walk backwards" all the way back to the original 'IMapView<K, V>' to understand
// what 'K' and 'V' are.
struct generic_param_stack
{
    const winmd::reader::GenericTypeInstSig* first_parent;
    const winmd::reader::GenericTypeInstSig* last_parent;

    generic_param_stack parent_stack() const noexcept
    {
        if (first_parent == last_parent)
            return { nullptr, nullptr };
        return { first_parent - 1, last_parent };
    }

    std::size_t size() const noexcept
    {
        return (first_parent == nullptr) ? 0 : first_parent - last_parent + 1;
    }

    bool empty() const noexcept
    {
        // NOTE: Unlike "normal" ranges, this points to the first and last valid elements, meaning that "begin == end"
        // might actually imply a size of one. We use null values to disambiguate this
        return first_parent == nullptr;
    }
};

struct generic_instantiation
{
    std::vector<winmd::reader::GenericTypeInstSig> signatures;

    generic_instantiation(winmd::reader::GenericTypeInstSig sig)
    {
        signatures.push_back(std::move(sig));
    }

    generic_instantiation(winmd::reader::GenericTypeInstSig sig, const generic_param_stack& genericParamStack)
    {
        auto stackSize = genericParamStack.size();
        signatures.reserve(stackSize + 1);

        if (stackSize)
        {
            for (auto begin = genericParamStack.last_parent; begin <= genericParamStack.first_parent; ++begin)
            {
                signatures.push_back(*begin);
            }
            assert(signatures.size() == stackSize);
        }

        signatures.push_back(std::move(sig));
    }

    const winmd::reader::GenericTypeInstSig& signature() const noexcept
    {
        return signatures.back();
    }

    generic_param_stack stack() const noexcept
    {
        return { &signatures.back(), &signatures.front() };
    }

    generic_param_stack parent_stack() const noexcept
    {
        return stack().parent_stack();
    }
};

// NOTE: 'resolve_type' will invoke the callback with one of the following sets of arguments, depending on resolved tye
//      * (ElementType[, bool])
//      * (TypeDef[, bool])
//      * (GenericTypeInstSig, generic_param_stack[, bool])
//      * (system_guid_t[, bool])
// Note that each callback conditionally contains a bool, depending on whether or not the given signature can indicate
// if the type is an array or not
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
inline void resolve_type(
    const winmd::reader::GenericTypeInstSig& sig, const generic_param_stack& genericParamStack, Func&& fn)
{
    fn(sig, genericParamStack);
}

template <typename Func>
inline void resolve_type(const winmd::reader::coded_index<winmd::reader::TypeDefOrRef>& defOrRef,
    const generic_param_stack& genericParamStack, Func&& fn)
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
        resolve_type(defOrRef.TypeSpec().Signature().GenericTypeInst(), genericParamStack, std::forward<Func>(fn));
        break;
    }
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeSig& sig, generic_param_stack genericParamStack, Func&& fn);

inline const winmd::reader::TypeSig& get_generic_param_signature(
    winmd::reader::GenericTypeIndex idx, const generic_param_stack& genericParamStack)
{
    if (genericParamStack.empty())
    {
        assert(false);
        throw std::runtime_error("Generic param stack empty");
    }

    auto& genericTypeSig = *genericParamStack.first_parent;
    if (idx.index >= genericTypeSig.GenericArgCount())
    {
        assert(false);
        throw std::range_error("GenericTypeIndex out of range");
    }

    return *(genericTypeSig.GenericArgs().first + idx.index);
}

template <typename Func>
inline void resolve_type(winmd::reader::GenericTypeIndex idx, const generic_param_stack& genericParamStack, Func&& fn)
{
    using namespace winmd::reader;
    resolve_type(get_generic_param_signature(idx, genericParamStack), genericParamStack.parent_stack(),
        overloaded{
            [&](const GenericTypeInstSig& sig, const generic_param_stack& newStack, bool isArray) {
                assert(!isArray);
                fn(sig, newStack);
            },
            [&](auto&& value, bool isArray) {
                assert(!isArray);
                fn(value);
            },
        });
}

template <typename Func>
inline void resolve_type(const winmd::reader::TypeSig& sig, generic_param_stack genericParamStack, Func&& fn)
{
    using namespace winmd::reader;

    auto tgt = &sig;
    while (true)
    {
        auto stop = std::visit(
            overloaded{
                [&](ElementType type) {
                    fn(type, sig.is_szarray());
                    return true;
                },
                [&](const coded_index<TypeDefOrRef>& defOrRef) {
                    resolve_type(defOrRef, genericParamStack,
                        [&](auto&&... args) { fn(std::forward<decltype(args)>(args)..., sig.is_szarray()); });
                    return true;
                },
                [&](GenericTypeIndex index) {
                    tgt = &get_generic_param_signature(index, genericParamStack);
                    genericParamStack = genericParamStack.parent_stack();
                    return false;
                },
                [&](const GenericTypeInstSig& genericSig) {
                    fn(genericSig, genericParamStack, sig.is_szarray());
                    return true;
                },
                [&](GenericMethodTypeIndex) -> bool { throw std::runtime_error("Unexpected GenericMethodTypeIndex"); },
            },
            tgt->Type());
        if (stop)
            break;
    }
}

template <typename Func>
inline void resolve_type(const winmd::reader::ParamSig& sig, const generic_param_stack& genericParamStack, Func&& fn)
{
    resolve_type(sig.Type(), genericParamStack, std::forward<Func>(fn));
}
