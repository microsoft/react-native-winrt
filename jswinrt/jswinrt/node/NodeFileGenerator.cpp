
#include <pch.h>

#include "../MetadataHelpers.h"
#include "../Writer.h"

#include <nodewinrt_strings.h>

using namespace winmd::reader;

static void write_nodewinrt_projections_cpp_single_namespace_include(
    jswinrt::writer& writer, const namespace_projection_data& ns)
{
    // We only need includes if the namespace is a root namespace (for the global root namespace array), or if it has
    // interfaces (for the global interface map)
    if (ns.base_namespace.empty() || !ns.interface_children.empty())
    {
        writer.write_fmt("#include \"%.g.h\"\n", ns.full_name);
    }

    // We also need the C++/WinRT header if the namespace has interfaces (for 'guid_of' specialization)
    if (!ns.interface_children.empty())
    {
        writer.write_fmt("#include <winrt/%.h>\n", ns.full_name);
    }
}

static void write_nodewinrt_projections_cpp_namespace_includes(
    jswinrt::writer& writer, const namespace_projection_data& ns)
{
    write_nodewinrt_projections_cpp_single_namespace_include(writer, ns);

    for (auto& childNs : ns.namespace_children)
    {
        write_nodewinrt_projections_cpp_namespace_includes(writer, *childNs);
    }
}

static void write_nodewinrt_projections_cpp_namespace_includes(jswinrt::writer& writer, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_nodewinrt_projections_cpp_namespace_includes(writer, *ns);
    }
}

static void write_nodewinrt_projections_cpp_file(const Settings& settings, const projection_data& data)
{
    jswinrt::file_writer writer(settings.NodeWinRTOutputFolder / "Projections.g.cpp");
    writer.write_fmt(R"^-^(#include "%"

#include "base.h"

)^-^",
        settings.PchFileName);

    write_nodewinrt_projections_cpp_namespace_includes(writer, data);

    // 'root_namespaces' definition
    writer.write(R"^-^(
namespace nodewinrt
{
    static constexpr const static_namespace_data* const root_namespace_data[] = {
)^-^");

    for (auto& ns : data.root_namespaces)
    {
        writer.write_fmt("        &nodewinrt::namespaces::%::data,\n", ns->name);
    }

    writer.write(R"^-^(    };

    constexpr const span<const static_namespace_data* const> root_namespaces{ root_namespace_data };

)^-^");

    // 'global_interface_map' definition
    writer.write(
        R"^-^(    static constexpr const std::pair<winrt::guid, const static_interface_data*> global_interface_map_data[] = {)^-^");

    for (auto iface : data.interfaces)
    {
        writer.write_fmt(
            "\n        { winrt::guid_of<winrt::%>(), &nodewinrt::interfaces::% },",
            [&](jswinrt::writer& w) { iface->write_cpp_name(w, ""sv); },
            [&](jswinrt::writer& w) { iface->write_cpp_name(w, "::data"sv); });
#if 1 // For IID gen validation
        writer.write_fmt(" // %", jswinrt::fmt_guid{ iface->iid });
#endif
    }

    writer.write(R"^-^(
    };

    constexpr const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map(global_interface_map_data);
}
)^-^");
}

static void write_nodewinrt_value_converters_decls(jswinrt::writer& writer, const namespace_projection_data& ns)
{
    if (!ns.struct_children.empty() || !ns.delegate_children.empty())
    {
        // Forward declare the C++/WinRT types (because this file gets included in all cpp files)
        writer.write_fmt("namespace winrt::%\n{\n", jswinrt::cpp_namespace{ &ns });

        for (auto& structDef : ns.struct_children)
        {
            writer.write_fmt("    struct %;\n", structDef->type_def.TypeName());
        }

        for (auto& delegateDef : ns.delegate_children)
        {
            writer.write_fmt("    struct %;\n", delegateDef->type_def.TypeName());
        }

        // Now write the specializations; the function definitions will go in each namespace's cpp file
        writer.write(R"^-^(}

namespace nodewinrt
{)^-^");

        for (auto& structDef : ns.struct_children)
        {
            writer.write_fmt(R"^-^(
    template <>
    struct projected_value_traits<winrt::%>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, const winrt::%& value);
        static winrt::% as_native(runtime_context* context, v8::Local<v8::Value> value);
    };
)^-^",
                jswinrt::cpp_typename{ structDef->type_def }, jswinrt::cpp_typename{ structDef->type_def },
                jswinrt::cpp_typename{ structDef->type_def });
        }

        for (auto& delegateDef : ns.delegate_children)
        {
            writer.write_fmt(R"^-^(
    template <>
    struct projected_value_traits<winrt::%>
    {
        using delegate_type = winrt::%;

        static v8::Local<v8::Value> as_value(runtime_context* context, const delegate_type& value);
        static delegate_type as_native(runtime_context* context, v8::Local<v8::Value> value);
    };
)^-^",
                jswinrt::cpp_typename{ delegateDef->type_def }, jswinrt::cpp_typename{ delegateDef->type_def });
        }

        writer.write("}\n\n");
    }

    for (auto& childNs : ns.namespace_children)
    {
        write_nodewinrt_value_converters_decls(writer, *childNs);
    }
}

static void write_nodewinrt_value_converters_decls(jswinrt::writer& writer, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_nodewinrt_value_converters_decls(writer, *ns);
    }
}

static void write_nodewinrt_value_converters_header(const Settings& settings, const projection_data& data)
{
    jswinrt::file_writer writer(settings.NodeWinRTOutputFolder / "ProjectedValueConverters.g.h");

    writer.write(R"^-^(#pragma once

#include "base.h"

)^-^");

    write_nodewinrt_value_converters_decls(writer, data);
}

static void write_nodewinrt_namespace_headers(const Settings& settings, const namespace_projection_data& ns)
{
    jswinrt::file_writer writer(settings.NodeWinRTOutputFolder / (std::string{ ns.full_name } + ".g.h"));
    writer.write_fmt(R"^-^(#pragma once

#include "base.h"

namespace nodewinrt::namespaces::%
{
    extern const static_namespace_data data;
}
)^-^",
        jswinrt::cpp_namespace{ &ns });

    // Static enum data declarations
    if (!ns.enum_children.empty())
    {
        writer.write_fmt(R"^-^(
namespace nodewinrt::enums::%
{)^-^",
            jswinrt::cpp_namespace{ &ns });

        for (auto& enumDef : ns.enum_children)
        {
            writer.write_fmt(R"^-^(
    namespace %
    {
        extern const static_enum_data data;
    }
)^-^",
                enumDef->type_def.TypeName());
        }

        writer.write("}\n");
    }

    // Static class data declarations
    if (!ns.class_children.empty())
    {
        writer.write_fmt(R"^-^(
namespace nodewinrt::classes::%
{)^-^",
            jswinrt::cpp_namespace{ &ns });

        for (auto& classDef : ns.class_children)
        {
            writer.write_fmt(R"^-^(
    namespace %
    {
        extern const % data;
    }
)^-^",
                classDef->type_def.TypeName(),
                classDef->methods.constructors.empty() ? "static_class_data"sv : "static_activatable_class_data"sv);
        }

        writer.write("}\n");
    }

    // Static interface data declarations
    if (!ns.interface_children.empty())
    {
        writer.write_fmt(R"^-^(
namespace nodewinrt::interfaces::%
{)^-^",
            jswinrt::cpp_namespace{ &ns });

        for (auto& ifaceDef : ns.interface_children)
        {
            writer.write_fmt(R"^-^(
    namespace %
    {
        extern const static_interface_data data;
    }
)^-^",
                ifaceDef->type_def.TypeName());
        }

        writer.write("}\n");
    }

    for (auto& childNs : ns.namespace_children)
    {
        write_nodewinrt_namespace_headers(settings, *childNs);
    }
}

static void write_nodewinrt_namespace_headers(const Settings& settings, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_nodewinrt_namespace_headers(settings, *ns);
    }
}

static void write_nodewinrt_params_value_to_native(jswinrt::writer& writer, const function_signature& fn, int indentLevel)
{
    auto params = fn.params();
    int paramNum = 0;
    for (int argNum = 0; params.first != params.second; ++params.first, ++argNum)
    {
        if (!params.first.by_ref())
        {
            writer.write_fmt(
                "\n%auto arg% = convert_value_to_native<%>(context, info[%]);", jswinrt::indent{ indentLevel }, argNum,
                [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, params.first); }, paramNum++);
        }
        else
        {
            writer.write_fmt(
                "\n%% arg%%;", jswinrt::indent{ indentLevel },
                [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, params.first); }, argNum,
                [&](jswinrt::writer& w) { write_cppwinrt_type_initializer(w, params.first.type()); });
        }
    }
}

static void write_nodewinrt_make_return_struct(jswinrt::writer& writer, const function_signature& fn)
{
    if (fn.has_return_value)
    {
        writer.write("make_return_struct(context, result");
    }
    else
    {
        writer.write("make_void_return_struct(context");
    }

    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_output() && param.by_ref())
        {
            writer.write_fmt(R"^-^(, "%", arg%)^-^", jswinrt::camel_case{ param.name() }, argNum);
        }
        ++argNum;
    }

    writer.write(")");
}

static void write_nodewinrt_native_function_params(jswinrt::writer& writer, const function_signature& fn)
{
    std::string_view prefix;
    int argNum = 0;
    for (auto&& param : fn.params())
    {
        writer.write(prefix);
        prefix = ", ";

        jswinrt::write_cppwinrt_type(writer, param);
        if (param.by_ref())
        {
            writer.write("&");
        }
        else
        {
            resolve_type(param.signature(), {},
                overloaded{
                    [&](ElementType type, bool) {
                        if ((type == ElementType::String) || (type == ElementType::Object))
                        {
                            writer.write(" const&");
                        }
                    },
                    [&](const TypeDef& typeDef, bool) {
                        auto c = get_category(typeDef);
                        if ((c != category::enum_type))
                        {
                            writer.write(" const&");
                        }
                    },
                    [&](const GenericTypeInstSig&, const generic_param_stack&, bool) { writer.write(" const&"); },
                    [&](system_guid_t, bool) {},
                });
        }

        writer.write_fmt(" param%", argNum);
        ++argNum;
    }
}

static void write_nodewinrt_params_native_to_value(
    jswinrt::writer& writer, const function_signature& fn, int indentLevel)
{
    if (fn.param_count == 0)
    {
        writer.write_fmt("%v8::Local<v8::Value>* argv = nullptr;", jswinrt::indent{ indentLevel });
        return;
    }

    writer.write_fmt("%v8::Local<v8::Value> argv[] = {", jswinrt::indent{ indentLevel });

    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_input() || !param.by_ref())
        {
            writer.write_fmt(
                "\n%convert_native_to_value(context, param%),", jswinrt::indent{ indentLevel + 1 }, argNum);
        }
        ++argNum;
    }

    writer.write_fmt("\n%};", jswinrt::indent{ indentLevel });
}

static void write_nodewinrt_native_out_params(jswinrt::writer& writer, const function_signature& fn, int indentLevel)
{
    assert(fn.has_out_params);
    writer.write_fmt("\n%auto obj = cast_object(context, result);", jswinrt::indent{ indentLevel });

    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_output() && param.by_ref())
        {
            writer.write_fmt(
                R"^-^(
%param% = convert_value_to_native<%>(runtime, obj.getProperty(runtime, "%"));)^-^",
                jswinrt::indent{ indentLevel }, argNum,
                [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, param); },
                jswinrt::camel_case{ param.name() });
        }
        ++argNum;
    }
}

static void write_nodewinrt_enum_projection_data(jswinrt::writer& writer, const enum_projection_data& enumData)
{
    writer.write_fmt(R"^-^(
namespace nodewinrt::enums::%
{
    static constexpr const static_enum_data::value_mapping mappings[] = {)^-^",
        jswinrt::cpp_typename{ enumData.type_def });

    for (auto&& field : enumData.type_def.FieldList())
    {
        if (!field.Constant()) // Enums have a 'value__' data member we need to ignore
            continue;

        writer.write_fmt(R"^-^(
        { "%"sv, static_cast<double>(winrt::%::%) },)^-^",
            jswinrt::camel_case{ field.Name() }, jswinrt::cpp_typename{ enumData.type_def }, field.Name());
    }

    writer.write_fmt(R"^-^(
    };

    constexpr const static_enum_data data{ "%"sv, mappings };
}
)^-^",
        enumData.name);
}

static void write_nodewinrt_class_projection_data(jswinrt::writer& writer, const class_projection_data& classData)
{
    writer.write_fmt(R"^-^(
namespace nodewinrt::classes::%
{)^-^",
        jswinrt::cpp_typename{ classData.type_def });

    if (!classData.methods.properties.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_class_data::property_mapping property_data[] = {)^-^");

        for (auto& data : classData.methods.properties)
        {
            // Class properties should always have a getter
            if (!data.getter)
            {
                assert(false);
                throw std::runtime_error("Static class property with no getter");
            }

            writer.write_fmt(R"^-^(
        { "%",
            [](runtime_context* context) {
                return convert_native_to_value(context, winrt::%::%());
            },)^-^",
                jswinrt::camel_case{ data.name }, jswinrt::cpp_typename{ classData.type_def }, data.name);

            if (data.setter)
            {
                writer.write_fmt(R"^-^(
            [](runtime_context* context, v8::Local<v8::Value> value) {
                winrt::%::%(convert_value_to_native<%>(context, value));
            },)^-^",
                    jswinrt::cpp_typename{ classData.type_def }, data.name,
                    [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, data.setter->params().first); });
            }
            else
            {
                writer.write("\n            nullptr");
            }

            writer.write(R"^-^(
        },)^-^");
        }

        writer.write("\n    };\n");
    }

    if (!classData.methods.events.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_class_data::event_mapping event_data[] = {)^-^");

        for (auto& data : classData.methods.events)
        {
            writer.write_fmt(R"^-^(
        { "%",
            [](runtime_context* context, v8::Local<v8::Value> callback) {
                return winrt::%::%(convert_value_to_native<%>(context, callback));
            },
            [](winrt::event_token token) {
                winrt::%::%(token);
            }
        },)^-^",
                jswinrt::event_name{ data.name }, jswinrt::cpp_typename{ classData.type_def }, data.name,
                [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, data.type, {}); },
                jswinrt::cpp_typename{ classData.type_def }, data.name);
        }

        writer.write("\n    };\n");
    }

    if (!classData.methods.functions.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_class_data::function_mapping function_data[] = {)^-^");

        for (auto& data : classData.methods.functions)
        {
            writer.write_fmt(R"^-^(
        { "%",
            [](runtime_context* context, const v8::FunctionCallbackInfo<v8::Value>& info) {
                auto count = info.Length();)^-^",
                jswinrt::camel_case{ data.name });

            for (auto& overload : data.overloads)
            {
                writer.write_fmt(R"^-^(
                if (count == %)
                {)^-^",
                    overload.arity);

                write_nodewinrt_params_value_to_native(writer, overload.method, 5);

                writer.write_fmt(R"^-^(
                    %winrt::%::%()^-^",
                    overload.method.has_return_value ? "auto result = " : "",
                    jswinrt::cpp_typename{ classData.type_def }, data.name);

                auto argCount = overload.method.native_param_count();
                std::string_view prefix;
                for (int i = 0; i < argCount; ++i)
                {
                    writer.write_fmt("%arg%", prefix, i);
                    prefix = ", ";
                }
                writer.write(");");

                if (overload.method.has_out_params)
                {
                    writer.write_fmt(R"^-^(
                    return info.GetReturnValue().Set(%);)^-^",
                        [&](jswinrt::writer& w) { write_nodewinrt_make_return_struct(w, overload.method); });
                }
                else if (overload.method.has_return_value)
                {
                    writer.write(R"^-^(
                    return info.GetReturnValue().Set(convert_native_to_value(context, result));)^-^");
                }
                else
                {
                    writer.write(R"^-^(
                    return;)^-^");
                }

                writer.write(R"^-^(
                })^-^");
            }

            writer.write_fmt(R"^-^(
                throw_no_function_overload(context->isolate, "%"sv, "%"sv, "%"sv, count);
            }
        },)^-^",
                classData.type_def.TypeNamespace(), classData.type_def.TypeName(), jswinrt::camel_case{ data.name });
        }

        writer.write("\n    };\n");
    }

    if (classData.methods.constructors.empty())
    {
        writer.write_fmt(R"^-^(
    constexpr const static_class_data data{ "%"sv, %, %, % };
}
)^-^",
            classData.name, classData.methods.properties.empty() ? "{}"sv : "property_data"sv,
            classData.methods.events.empty() ? "{}"sv : "event_data"sv,
            classData.methods.functions.empty() ? "{}"sv : "function_data"sv);
    }
    else
    {
        writer.write_fmt(R"^-^(
    static void constructor_function(runtime_context* context, const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        auto count = info.Length();)^-^");

        for (auto& ctorData : classData.methods.constructors)
        {
            writer.write_fmt(R"^-^(
        if (count == %)
        {)^-^",
                ctorData.method.param_count);

            write_nodewinrt_params_value_to_native(writer, ctorData.method, 3);

            writer.write_fmt("\n            return info.GetReturnValue().Set(convert_native_to_value(context, winrt::%(",
                jswinrt::cpp_typename{ classData.type_def });

            // NOTE: Already asserted no out params
            std::string_view prefix;
            for (int i = 0; i < ctorData.method.param_count; ++i)
            {
                writer.write_fmt("%arg%", prefix, i);
                prefix = ", "sv;
            }
            writer.write(")));");

            writer.write("\n        }");
        }

        // NOTE: We use string concatenation here - instead of embedding the type name - to reduce the impact on
        // disk footprint. This isn't an expected/common code path, so the impact should hopefully be minimal
        // TODO: Actually validate this
        // TODO: Make throwing function a separate non-inlined function? Would also decrease disk footprint
        writer.write_fmt(R"^-^(
        throw_no_constructor(context->isolate, "%"sv, "%"sv, count);
    }

    constexpr const static_activatable_class_data data{ "%"sv, "%"sv, constructor_function, %, %, % };
}
)^-^",
            classData.type_def.TypeNamespace(), classData.type_def.TypeName(), classData.type_def.TypeNamespace(),
            classData.name, classData.methods.properties.empty() ? "{}"sv : "property_data"sv,
            classData.methods.events.empty() ? "{}"sv : "event_data"sv,
            classData.methods.functions.empty() ? "{}"sv : "function_data"sv);
    }
}

static void write_nodewinrt_interface_projection_data(jswinrt::writer& writer, const interface_projection_data& ifaceData)
{
    writer.write_fmt(R"^-^(
namespace nodewinrt::interfaces::%
{)^-^",
        jswinrt::cpp_typename{ ifaceData.type_def });

    if (!ifaceData.methods.properties.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_interface_data::property_mapping property_data[] = {)^-^");

        for (auto& data : ifaceData.methods.properties)
        {
            writer.write_fmt(R"^-^(
        { "%",)^-^",
                jswinrt::camel_case{ data.name });

            if (data.getter)
            {
                writer.write_fmt(R"^-^(
            [](runtime_context* context, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(context, thisValue.as<winrt::%>().%());
            },)^-^",
                    jswinrt::cpp_typename{ ifaceData.type_def }, data.name);
            }
            else
            {
                writer.write("\n            nullptr,");
            }

            if (data.setter)
            {
                writer.write_fmt(R"^-^(
            [](runtime_context* context, const winrt::Windows::Foundation::IInspectable& thisValue, v8::Local<v8::Value> value) {
                thisValue.as<winrt::%>().%(convert_value_to_native<%>(context, value));
            },)^-^",
                    jswinrt::cpp_typename{ ifaceData.type_def }, data.name,
                    [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, data.setter->params().first); });
            }
            else
            {
                writer.write("\n            nullptr");
            }

            writer.write(R"^-^(
        },)^-^");
        }

        writer.write("\n    };");
    }

    if (!ifaceData.methods.events.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_interface_data::event_mapping event_data[] = {)^-^");

        for (auto& data : ifaceData.methods.events)
        {
            writer.write_fmt(R"^-^(
        { "%",
            [](runtime_context* context, const winrt::Windows::Foundation::IInspectable& thisValue, v8::Local<v8::Value> callback) {
                return thisValue.as<winrt::%>().%(convert_value_to_native<%>(context, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::%>().%(token);
            }
        },)^-^",
                jswinrt::event_name{ data.name }, jswinrt::cpp_typename{ ifaceData.type_def }, data.name,
                [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, data.type, {}); },
                jswinrt::cpp_typename{ ifaceData.type_def }, data.name);
        }

        writer.write("\n    };\n");
    }

    if (!ifaceData.methods.functions.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_interface_data::function_mapping function_data[] = {)^-^");

        for (auto& data : ifaceData.methods.functions)
        {
            for (auto& overload : data.overloads)
            {
                writer.write_fmt(R"^-^(
        { "%",
            [](runtime_context* context, const winrt::Windows::Foundation::IInspectable& thisValue, const v8::FunctionCallbackInfo<v8::Value>& info) {
                assert(info.Length() == %);)^-^",
                    jswinrt::camel_case{ data.name }, overload.arity);

                write_nodewinrt_params_value_to_native(writer, overload.method, 5);

                writer.write_fmt(R"^-^(
                    %thisValue.as<winrt::%>().%()^-^",
                    overload.method.has_return_value ? "auto result = " : "",
                    jswinrt::cpp_typename{ ifaceData.type_def }, data.name);

                auto argCount = overload.method.native_param_count();
                std::string_view prefix;
                for (int i = 0; i < argCount; ++i)
                {
                    writer.write_fmt("%arg%", prefix, i);
                    prefix = ", ";
                }
                writer.write(");");

                if (overload.method.has_out_params)
                {
                    writer.write_fmt(R"^-^(
                    info.GetReturnValue().Set(%);)^-^",
                        [&](jswinrt::writer& w) { write_nodewinrt_make_return_struct(w, overload.method); });
                }
                else if (overload.method.has_return_value)
                {
                    writer.write(R"^-^(
                    info.GetReturnValue().Set(convert_native_to_value(context, result));)^-^");
                }

                writer.write_fmt(R"^-^(
                },
                %, % },)^-^",
                    overload.arity, overload.is_default_overload);
            }
        }

        writer.write("\n    };\n");
    }

    writer.write_fmt(R"^-^(
    constexpr const static_interface_data data{ winrt::guid_of<winrt::%>(), %, %, % };
}
)^-^",
        jswinrt::cpp_typename{ ifaceData.type_def }, ifaceData.methods.properties.empty() ? "{}"sv : "property_data"sv,
        ifaceData.methods.events.empty() ? "{}"sv : "event_data"sv,
        ifaceData.methods.functions.empty() ? "{}"sv : "function_data"sv);
}

static void write_nodewinrt_namespace_cpp_files(const Settings& settings, const namespace_projection_data& ns)
{
    jswinrt::file_writer writer(settings.NodeWinRTOutputFolder / (std::string{ ns.full_name } + ".g.cpp"));
    writer.write_fmt(R"^-^(#include "%"

#include "base.h"

#include "%.g.h"
)^-^",
        settings.PchFileName, ns.full_name);

    // We also need the namespace headers for child namespaces
    for (auto& childNs : ns.namespace_children)
    {
        writer.write_fmt(R"^-^(#include "%.g.h"
)^-^",
            childNs->full_name);
    }

    // NOTE: There's only a C++/WinRT header for us to include if there are non-namespace types
    if ((ns.namespace_children.size() < ns.named_children.size()) || !ns.struct_children.empty() ||
        !ns.delegate_children.empty() || !ns.interface_children.empty())
    {
        writer.write_fmt("\n#include <winrt/%.h>\n", ns.full_name);
    }

    // Namespace data
    // TODO: Some namespaces don't have any class/enum/namespace children and therefore could be left out, however we
    // would need to do this recursively "up the chain". For now we'll avoid this complexity. The net result is that
    // we'll have some JS objects representing "empty" namespaces, but that's not a huge deal
    writer.write_fmt(R"^-^(
namespace nodewinrt::namespaces::%
{)^-^",
        jswinrt::cpp_namespace{ &ns });

    if (!ns.named_children.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_projection_data* const children[] = {
)^-^");

        for (auto child : ns.named_children)
        {
            std::string_view kindNs;
            switch (child->data_kind)
            {
            case named_projection_data::kind::namespace_data:
                kindNs = "namespaces";
                break;

            case named_projection_data::kind::enum_data:
                kindNs = "enums";
                break;

            case named_projection_data::kind::class_data:
                kindNs = "classes";
                break;
            }
            writer.write_fmt("        &nodewinrt::%::%::%::data,\n", kindNs, jswinrt::cpp_namespace{ &ns }, child->name);
        }

        writer.write("    };\n");
    }

    writer.write_fmt(R"^-^(
    constexpr const static_namespace_data data{ "%"sv, % };
}
)^-^",
        ns.name, ns.named_children.empty() ? "{}" : "children");

    // Static enum data
    for (auto& enumData : ns.enum_children)
    {
        write_nodewinrt_enum_projection_data(writer, *enumData);
    }

    // Static class data
    for (auto& classData : ns.class_children)
    {
        write_nodewinrt_class_projection_data(writer, *classData);
    }

    // Static interface data
    for (auto& ifaceData : ns.interface_children)
    {
        write_nodewinrt_interface_projection_data(writer, *ifaceData);
    }

    // projected_value_traits definitions
    if (!ns.struct_children.empty() || !ns.delegate_children.empty())
    {
        writer.write("\nnamespace nodewinrt\n{");

        for (auto& structData : ns.struct_children)
        {
            writer.write_fmt(R"^-^(
    v8::Local<v8::Value> projected_value_traits<winrt::%>::as_value(runtime_context* context, const winrt::%& value)
    {
        auto ctxt = context->isolate->GetCurrentContext();
        auto result = v8::Object::New(context->isolate);
)^-^",
                jswinrt::cpp_typename{ structData->type_def }, jswinrt::cpp_typename{ structData->type_def });

            for (auto&& field : structData->type_def.FieldList())
            {
                writer.write_fmt(
                    R"^-^(        set_property(context, ctxt, result, "%"sv, value.%);
)^-^",
                    jswinrt::camel_case{ field.Name() }, field.Name());
            }

            writer.write_fmt(R"^-^(        return result;
    }

    winrt::% projected_value_traits<winrt::%>::as_native(runtime_context* context, v8::Local<v8::Value> value)
    {
        winrt::% result{};
        auto obj = cast_object(context->isolate, value);
        auto ctxt = context->isolate->GetCurrentContext();
)^-^",
                jswinrt::cpp_typename{ structData->type_def }, jswinrt::cpp_typename{ structData->type_def },
                jswinrt::cpp_typename{ structData->type_def });

            for (auto&& field : structData->type_def.FieldList())
            {
                auto sig = field.Signature();
                writer.write_fmt(R"^-^(        if (auto field = get_property(context->isolate, ctxt, obj, "%"sv); !field->IsUndefined())
            result.% = convert_value_to_native<%>(context, field);
)^-^",
                    jswinrt::camel_case{ field.Name() }, field.Name(),
                    [&](jswinrt::writer& w) { jswinrt::write_cppwinrt_type(w, sig.Type(), {}); });
            }

            writer.write("        return result;\n    }\n");
        }

        for (auto& delegateData : ns.delegate_children)
        {
            function_signature fn(delegate_invoke_function(delegateData->type_def));

            writer.write_fmt(R"^-^(
    v8::Local<v8::Value> projected_value_traits<winrt::%>::as_value(runtime_context* context, const delegate_type& value)
    {
        auto ptr = std::make_unique<projected_delegate<delegate_type>>(context, value,
            [](runtime_context* context, const delegate_type& target, const v8::FunctionCallbackInfo<v8::Value>& info) {
                if (info.Length() != %)
                {
                    throw_invalid_delegate_arg_count(context->isolate, "%"sv, "%"sv);
                }
)^-^",
                jswinrt::cpp_typename{ delegateData->type_def }, fn.param_count, delegateData->type_def.TypeNamespace(),
                delegateData->type_def.TypeName());

            write_nodewinrt_params_value_to_native(writer, fn, 4);
            writer.write_fmt("\n                %target(", fn.has_return_value ? "auto result = " : "");

            auto argCount = fn.native_param_count();
            std::string_view prefix;
            for (int i = 0; i < argCount; ++i)
            {
                writer.write_fmt("%arg%", prefix, i);
                prefix = ", ";
            }
            writer.write(");");

            if (fn.has_out_params)
            {
                writer.write_fmt("\n                info.GetReturnValue().Set(%);",
                    [&](jswinrt::writer& w) { write_nodewinrt_make_return_struct(w, fn); });
            }
            else if (fn.has_return_value)
            {
                writer.write("\n                info.GetReturnValue().Set(convert_native_to_value(context, result));");
            }

            writer.write_fmt(R"^-^(
            });
        auto fn = check_maybe(v8::Function::New(context->isolate->GetCurrentContext(), &projected_delegate<delegate_type>::call,
            v8::External::New(context->isolate, ptr.get()), %, v8::ConstructorBehavior::kThrow));
        ptr.release()->track_lifetime(fn);
        return fn;
    }

    winrt::% projected_value_traits<winrt::%>::as_native(runtime_context* context, v8::Local<v8::Value> value)
    {
        if (!value->IsFunction())
        {
            throw v8_exception::type_error(context->isolate, "Delegate must be a function"sv);
        }

        // TODO: Need to take a strong reference to the context
        return [context, fn = v8::Global<v8::Function>(context->isolate, value.As<v8::Function>())](%) {)^-^",
                fn.param_count, jswinrt::cpp_typename{ delegateData->type_def },
                jswinrt::cpp_typename{ delegateData->type_def },
                [&](jswinrt::writer& w) { write_nodewinrt_native_function_params(w, fn); });

            auto writeReturnType = [&](jswinrt::writer& w) {
                resolve_type(fn.signature.ReturnType().Type(), {},
                    overloaded{
                        [&](const GenericTypeInstSig& sig, const generic_param_stack& genStack, bool isArray) {
                            if (isArray)
                                w.write("winrt::com_array<"sv);
                            jswinrt::write_cppwinrt_type(w, sig, genStack);
                            if (isArray)
                                w.write(">");
                        },
                        [&](auto&& value, bool isArray) {
                            if (isArray)
                                w.write("winrt::com_array<"sv);
                            jswinrt::write_cppwinrt_type(w, value);
                            if (isArray)
                                w.write(">");
                        },
                    });
            };

            if (fn.has_return_value)
            {
                writer.write_fmt(R"^-^(
            % returnValue%;)^-^",
                    writeReturnType,
                    [&](jswinrt::writer& w) { write_cppwinrt_type_initializer(w, fn.signature.ReturnType().Type()); });
            }

            writer.write(R"^-^(
            context->call_sync([&]() {
                auto ctxt = context->isolate->GetCurrentContext();
)^-^");

            write_nodewinrt_params_native_to_value(writer, fn, 4);

            writer.write_fmt(R"^-^(
                %check_maybe(fn.Get(context->isolate)->Call(ctxt, ctxt->Global(), %, argv));)^-^",
                (fn.has_return_value || fn.has_out_params) ? "auto result = " : "", fn.param_count);

            if (fn.has_out_params)
            {
                write_nodewinrt_native_out_params(writer, fn, 4);

                if (fn.has_return_value)
                {
                    // This is a bit different than below since we need to "extract" the value
                    writer.write_fmt(R"^-^(
                returnValue = convert_value_to_native<%>(context, get_property(context->isolate, ctxt, obj, "returnValue"sv));)^-^",
                        writeReturnType);
                }
            }
            else if (fn.has_return_value)
            {
                writer.write_fmt(
                    "\n                returnValue = convert_value_to_native<%>(context, result);", writeReturnType);
            }

            writer.write(R"^-^(
            });)^-^");

            if (fn.has_return_value)
            {
                writer.write("\n            return returnValue;");
            }

            writer.write(R"^-^(
        };
    }
)^-^");
        }

        writer.write("}\n");
    }

    for (auto& childNs : ns.namespace_children)
    {
        write_nodewinrt_namespace_cpp_files(settings, *childNs);
    }
}

static void write_nodewinrt_namespace_cpp_files(const Settings& settings, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_nodewinrt_namespace_cpp_files(settings, *ns);
    }
}

static void write_nodewinrt_base_files(const Settings& settings)
{
    for (auto& data : nodewinrt::file_strings)
    {
        jswinrt::file_writer writer(settings.NodeWinRTOutputFolder / data.file_name);
        for (auto& str : data.file_contents)
        {
            writer.write(str);
        }
    }
}

void write_nodewinrt_files(const Settings& settings, const projection_data& data)
{
    std::filesystem::create_directories(settings.NodeWinRTOutputFolder);

    // Projections.g.cpp
    write_nodewinrt_projections_cpp_file(settings, data);
    // ProjectedValueConverters.g.h
    write_nodewinrt_value_converters_header(settings, data);
    // <Namespace>.g.h
    write_nodewinrt_namespace_headers(settings, data);
    // <Namespace>.g.cpp
    write_nodewinrt_namespace_cpp_files(settings, data);

    // base.h/base.cpp
    write_nodewinrt_base_files(settings);
}
