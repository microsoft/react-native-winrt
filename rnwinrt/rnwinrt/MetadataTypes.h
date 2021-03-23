#pragma once

#include <unordered_set>

#include "MetadataHelpers.h"

namespace std
{
    template <>
    struct hash<GUID>
    {
        std::size_t operator()(const GUID& value) const noexcept
        {
            using array_type = std::array<std::uint64_t, 2>;
            std::hash<std::uint64_t> hasher;

            auto& array = reinterpret_cast<const array_type&>(value);
            return hasher(array[0]) ^ (hasher(array[1]) << 1);
        }
    };
}

struct function_signature
{
    winmd::reader::MethodDef method_def;
    winmd::reader::MethodDefSig signature;

    bool has_return_value;

    // NOTE: Both of these members reflect params as they behave in JS, not necessarily how they are marked in metadata
    bool has_out_params = false;
    int param_count = 0;

    function_signature(const winmd::reader::MethodDef& def);

    ptrdiff_t native_param_count() const
    {
        return winmd::reader::distance(signature.Params());
    }

    param_iterator param_begin() const
    {
        auto [paramBegin, paramEnd] = method_def.ParamList();
        if (has_return_value && (paramBegin != paramEnd))
            ++paramBegin;
        return param_iterator(signature.Params().first, paramBegin);
    }

    param_iterator param_end() const
    {
        return param_iterator(signature.Params().second, method_def.ParamList().second);
    }

    std::pair<param_iterator, param_iterator> params() const
    {
        return { param_begin(), param_end() };
    }
};

struct function_overload_data
{
    function_signature method;
    int arity;
    bool is_default_overload;

    function_overload_data(const winmd::reader::MethodDef& methodDef) :
        method(methodDef), is_default_overload(::is_default_overload(methodDef))
    {
        arity = method.param_count;
    }
};

struct function_data
{
    std::string_view name;
    std::vector<function_overload_data> overloads;

    function_data(std::string_view name) : name(name)
    {
    }
};

struct property_data
{
    std::string_view name;
    std::optional<function_signature> getter;
    std::optional<function_signature> setter;

    property_data(std::string_view name) : name(name)
    {
    }
};

struct event_data
{
    std::string_view name;
    winmd::reader::TypeSig type;

    event_data(std::string_view name, winmd::reader::TypeSig type) : name(name), type(std::move(type))
    {
#ifdef _DEBUG
        using namespace winmd::reader;
        resolve_type(type, {},
            overloaded{
                [&](const TypeDef& typeDef, bool isArray) {
                    assert(!isArray && (get_category(typeDef) == category::delegate_type));
                },
                [&](generic_instantiation&& inst, bool isArray) {
                    assert(!isArray);
                    auto genericType = generic_type_def(inst.signature);
                    assert(get_category(genericType) == category::delegate_type);
                },
                [&](auto&&, bool) { assert(false); },
            });
#endif
    }
};

struct constructor_data
{
    function_signature method;

    constructor_data(function_signature sig) : method(std::move(sig))
    {
        assert(!method.has_out_params);
    }
};

struct method_class
{
    std::string_view name;
    enum
    {
        function,
        property_getter,
        property_setter,
        event_add,
        event_remove,
        constructor,
    } kind;
};

method_class classify_method(const winmd::reader::MethodDef& def);

template <bool IsClass>
struct type_method_data
{
    std::vector<function_data> functions;
    std::vector<property_data> properties;
    std::vector<event_data> events;
    std::vector<constructor_data> constructors;

    type_method_data(const winmd::reader::TypeDef& typeDef);
};

using interface_method_data = type_method_data<false>;
using class_method_data = type_method_data<true>;

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

    named_projection_data(kind dataKind, const std::string_view& name, const std::string_view& baseNamespace) :
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
    enum_projection_data(const winmd::reader::TypeDef& typeDef) :
        named_projection_data(kind::enum_data, typeDef.TypeName(), typeDef.TypeNamespace()),
        type_def(typeDef)
    {
    }

    winmd::reader::TypeDef type_def;
};

struct class_projection_data : named_projection_data
{
    class_projection_data(const winmd::reader::TypeDef& typeDef) :
        named_projection_data(kind::class_data, typeDef.TypeName(), typeDef.TypeNamespace()), type_def(typeDef),
        methods(typeDef)
    {
    }

    winmd::reader::TypeDef type_def;
    class_method_data methods;
};

struct jswinrt_writer;

struct interface_instance
{
    GUID iid;

    interface_instance(const GUID& iid) : iid(iid)
    {
    }

    virtual void write_cpp_name(jswinrt_writer& writer) = 0;
};

struct interface_projection_data : interface_instance
{
    interface_projection_data(const winmd::reader::TypeDef& typeDef) :
        interface_instance(get_interface_guid(typeDef)), type_def(typeDef), methods(typeDef)
    {
    }

    virtual void write_cpp_name(jswinrt_writer& writer) override;

    winmd::reader::TypeDef type_def;
    interface_method_data methods;
};

struct generic_interface_instantiation : interface_instance
{
    // generic_interface_instantiation(const winmd::reader::GenericTypeInstSig& sig);

    virtual void write_cpp_name(jswinrt_writer& writer) override;

    generic_instantiation instantiation;
    // TODO: ? winmd::reader::TypeDef type_def;
};

struct struct_projection_data
{
    struct_projection_data(winmd::reader::TypeDef typeDef) : type_def(typeDef)
    {
    }

    winmd::reader::TypeDef type_def;
};

struct delegate_projection_data
{
    delegate_projection_data(const winmd::reader::TypeDef& typeDef) :
        type_def(typeDef), invoke(delegate_invoke_function(typeDef))
    {
    }

    winmd::reader::TypeDef type_def;
    function_signature invoke;
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
    std::vector<interface_instance*> interfaces;

    // In theory, having to go all the way and calculate each instantiation's GUID is potentially inefficient - e.g.
    // compared to just generating a string representing the type - however it's only just one more step, and arguably
    // the hash function is faster.
    std::unordered_set<GUID> generic_instantiations;
};

void parse_metadata(const Settings& settings, projection_data& data);
