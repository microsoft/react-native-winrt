#pragma once

#include "MetadataHelpers.h"

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
        auto paramBegin = method_def.ParamList().first;
        if (has_return_value)
            ++paramBegin;
        return param_iterator(signature.Params().first, paramBegin);
    }

    param_iterator param_end() const
    {
        return param_iterator(signature.Params().second, method_def.ParamList().second);
    }

    // For whatever reason, the metadata puts the return value name in the ParamList (which is conditionally present...)
    // and for whatever reason it is *first* in the list!?!
    std::pair<winmd::reader::Param, winmd::reader::Param> param_list() const
    {
        auto result = method_def.ParamList();
        if (has_return_value)
            ++result.first;
        return result;
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
    bool has_getter = false;
    bool has_setter = false;
    winmd::reader::TypeSig type;

    property_data(std::string_view name, winmd::reader::TypeSig type) : name(name), type(std::move(type))
    {
    }
};

struct event_data
{
    std::string_view name;
    winmd::reader::TypeSig type;

    event_data(std::string_view name, winmd::reader::TypeSig type) : name(name), type(std::move(type))
    {
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

struct interface_projection_data
{
    interface_projection_data(const winmd::reader::TypeDef& typeDef) : type_def(typeDef), methods(typeDef)
    {
    }

    winmd::reader::TypeDef type_def;
    interface_method_data methods;
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

    // Global lists
    std::vector<interface_projection_data*> interfaces;
};

void parse_metadata(const Settings& settings, projection_data& data);
