// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <pch.h>

#include "../MetadataHelpers.h"
#include "../Writer.h"

#include <rnwinrt_strings.h>

using namespace winmd::reader;

static void write_rnwinrt_projections_cpp_single_namespace_include(
    rnwinrt::writer& writer, const namespace_projection_data& ns)
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

static void write_rnwinrt_projections_cpp_namespace_includes(
    rnwinrt::writer& writer, const namespace_projection_data& ns)
{
    write_rnwinrt_projections_cpp_single_namespace_include(writer, ns);

    for (auto& childNs : ns.namespace_children)
    {
        write_rnwinrt_projections_cpp_namespace_includes(writer, *childNs);
    }
}

static void write_rnwinrt_projections_cpp_namespace_includes(rnwinrt::writer& writer, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_rnwinrt_projections_cpp_namespace_includes(writer, *ns);
    }
}

static void write_rnwinrt_projections_cpp_file(const Settings& settings, const projection_data& data)
{
    rnwinrt::file_writer writer(settings.OutputFolder / "Projections.g.cpp");
    writer.write_fmt(R"^-^(#include "%"

#include "base.h"

)^-^",
        settings.PchFileName);

    write_rnwinrt_projections_cpp_namespace_includes(writer, data);

    // 'root_namespaces' definition
    writer.write(R"^-^(
namespace rnwinrt
{
    static constexpr const static_namespace_data* const root_namespace_data[] = {
)^-^");

    for (auto& ns : data.root_namespaces)
    {
        writer.write_fmt("        &rnwinrt::namespaces::%::data,\n", ns->name);
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
            "\n        { winrt::guid_of<winrt::%>(), &rnwinrt::interfaces::% },",
            [&](rnwinrt::writer& w) { iface->write_cpp_name(w, ""sv); },
            [&](rnwinrt::writer& w) { iface->write_cpp_name(w, "::data"sv); });
#if 1 // For IID gen validation
        writer.write_fmt(" // %", rnwinrt::fmt_guid{ iface->iid });
#endif
    }

    writer.write(R"^-^(
    };

    constexpr const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map(global_interface_map_data);
}
)^-^");
}

static void write_rnwinrt_value_converters_decls(rnwinrt::writer& writer, const namespace_projection_data& ns)
{
    if (!ns.struct_children.empty() || !ns.delegate_children.empty())
    {
        // Forward declare the C++/WinRT types (because this file gets included in all cpp files)
        writer.write_fmt("namespace winrt::%\n{\n", rnwinrt::cpp_namespace{ &ns });

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

namespace rnwinrt
{)^-^");

        for (auto& structDef : ns.struct_children)
        {
            writer.write_fmt(R"^-^(
    template <>
    struct projected_value_traits<winrt::%>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::%& value);
        static winrt::% as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };
)^-^",
                rnwinrt::cpp_typename{ structDef->type_def }, rnwinrt::cpp_typename{ structDef->type_def },
                rnwinrt::cpp_typename{ structDef->type_def });
        }

        for (auto& delegateDef : ns.delegate_children)
        {
            writer.write_fmt(R"^-^(
    template <>
    struct projected_value_traits<winrt::%>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::%& value);
        static winrt::% as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };
)^-^",
                rnwinrt::cpp_typename{ delegateDef->type_def }, rnwinrt::cpp_typename{ delegateDef->type_def },
                rnwinrt::cpp_typename{ delegateDef->type_def });
        }

        writer.write("}\n\n");
    }

    for (auto& childNs : ns.namespace_children)
    {
        write_rnwinrt_value_converters_decls(writer, *childNs);
    }
}

static void write_rnwinrt_value_converters_decls(rnwinrt::writer& writer, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_rnwinrt_value_converters_decls(writer, *ns);
    }
}

static void write_rnwinrt_value_converters_header(const Settings& settings, const projection_data& data)
{
    rnwinrt::file_writer writer(settings.OutputFolder / "ProjectedValueConverters.g.h");

    writer.write(R"^-^(#pragma once

#include "base.h"

)^-^");

    write_rnwinrt_value_converters_decls(writer, data);
}

static void write_rnwinrt_namespace_headers(const Settings& settings, const namespace_projection_data& ns)
{
    rnwinrt::file_writer writer(settings.OutputFolder / (std::string{ ns.full_name } + ".g.h"));
    writer.write_fmt(R"^-^(#pragma once

#include "base.h"

namespace rnwinrt::namespaces::%
{
    extern const static_namespace_data data;
}
)^-^",
        rnwinrt::cpp_namespace{ &ns });

    // Static enum data declarations
    if (!ns.enum_children.empty())
    {
        writer.write_fmt(R"^-^(
namespace rnwinrt::enums::%
{)^-^",
            rnwinrt::cpp_namespace{ &ns });

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
namespace rnwinrt::classes::%
{)^-^",
            rnwinrt::cpp_namespace{ &ns });

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
namespace rnwinrt::interfaces::%
{)^-^",
            rnwinrt::cpp_namespace{ &ns });

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
        write_rnwinrt_namespace_headers(settings, *childNs);
    }
}

static void write_rnwinrt_namespace_headers(const Settings& settings, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_rnwinrt_namespace_headers(settings, *ns);
    }
}

static void write_rnwinrt_params_value_to_native(rnwinrt::writer& writer, const function_signature& fn, int indentLevel)
{
    auto params = fn.params();
    int paramNum = 0;
    for (int argNum = 0; params.first != params.second; ++params.first, ++argNum)
    {
        if (!params.first.by_ref())
        {
            writer.write_fmt(
                "\n%auto arg% = convert_value_to_native<%>(runtime, args[%]);", rnwinrt::indent{ indentLevel }, argNum,
                [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, params.first); }, paramNum++);
        }
        else
        {
            writer.write_fmt(
                "\n%% arg%%;", rnwinrt::indent{ indentLevel },
                [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, params.first); }, argNum,
                [&](rnwinrt::writer& w) { write_cppwinrt_type_initializer(w, params.first.type()); });
        }
    }
}

static void write_rnwinrt_make_return_struct(rnwinrt::writer& writer, const function_signature& fn)
{
    if (fn.has_return_value)
    {
        writer.write("make_return_struct(runtime, result");
    }
    else
    {
        writer.write("make_void_return_struct(runtime");
    }

    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_output() && param.by_ref())
        {
            writer.write_fmt(R"^-^(, "%", arg%)^-^", rnwinrt::camel_case{ param.name() }, argNum);
        }
        ++argNum;
    }

    writer.write(")");
}

static void write_rnwinrt_native_function_params(rnwinrt::writer& writer, const function_signature& fn)
{
    std::string_view prefix;
    int argNum = 0;
    for (auto&& param : fn.params())
    {
        writer.write(prefix);
        prefix = ", ";

        rnwinrt::write_cppwinrt_type(writer, param);
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

static void write_rnwinrt_params_native_to_value(rnwinrt::writer& writer, const function_signature& fn, int indentLevel)
{
    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_input() || !param.by_ref())
        {
            writer.write_fmt("\n%auto arg% = convert_native_to_value(runtime, param%);", rnwinrt::indent{ indentLevel },
                argNum, argNum);
        }
        ++argNum;
    }
}

static void write_rnwinrt_native_out_params(rnwinrt::writer& writer, const function_signature& fn, int indentLevel)
{
    assert(fn.has_out_params);
    writer.write_fmt("\n%auto obj = result.asObject(runtime);", rnwinrt::indent{ indentLevel });

    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_output() && param.by_ref())
        {
            writer.write_fmt(
                R"^-^(
%param% = convert_value_to_native<%>(runtime, obj.getProperty(runtime, "%"));)^-^",
                rnwinrt::indent{ indentLevel }, argNum,
                [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, param); },
                rnwinrt::camel_case{ param.name() });
        }
        ++argNum;
    }
}

static void write_rnwinrt_enum_projection_data(rnwinrt::writer& writer, const enum_projection_data& enumData)
{
    writer.write_fmt(R"^-^(
namespace rnwinrt::enums::%
{
    static constexpr const static_enum_data::value_mapping mappings[] = {)^-^",
        rnwinrt::cpp_typename{ enumData.type_def });

    for (auto&& field : enumData.type_def.FieldList())
    {
        if (!field.Constant()) // Enums have a 'value__' data member we need to ignore
            continue;

        writer.write_fmt(R"^-^(
        { "%"sv, static_cast<double>(winrt::%::%) },)^-^",
            rnwinrt::camel_case{ field.Name() }, rnwinrt::cpp_typename{ enumData.type_def }, field.Name());
    }

    writer.write_fmt(R"^-^(
    };

    constexpr const static_enum_data data{ "%"sv, mappings };
}
)^-^",
        enumData.name);
}

static void write_rnwinrt_class_projection_data(rnwinrt::writer& writer, const class_projection_data& classData)
{
    writer.write_fmt(R"^-^(
namespace rnwinrt::classes::%
{)^-^",
        rnwinrt::cpp_typename{ classData.type_def });

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
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::%::%());
            },)^-^",
                rnwinrt::camel_case{ data.name }, rnwinrt::cpp_typename{ classData.type_def }, data.name);

            if (data.setter)
            {
                writer.write_fmt(R"^-^(
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::%::%(convert_value_to_native<%>(runtime, value));
            },)^-^",
                    rnwinrt::cpp_typename{ classData.type_def }, data.name,
                    [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, data.setter->params().first); });
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
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::%::%(convert_value_to_native<%>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::%::%(token);
            }
        },)^-^",
                rnwinrt::event_name{ data.name }, rnwinrt::cpp_typename{ classData.type_def }, data.name,
                [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, data.type, {}); },
                rnwinrt::cpp_typename{ classData.type_def }, data.name);
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
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {)^-^",
                rnwinrt::camel_case{ data.name });

            for (auto& overload : data.overloads)
            {
                writer.write_fmt(R"^-^(
                if (count == %)
                {)^-^",
                    overload.arity);

                write_rnwinrt_params_value_to_native(writer, overload.method, 5);

                writer.write_fmt(R"^-^(
                    %winrt::%::%()^-^",
                    overload.method.has_return_value ? "auto result = " : "",
                    rnwinrt::cpp_typename{ classData.type_def }, data.name);

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
                    return %;)^-^",
                        [&](rnwinrt::writer& w) { write_rnwinrt_make_return_struct(w, overload.method); });
                }
                else if (overload.method.has_return_value)
                {
                    writer.write(R"^-^(
                    return convert_native_to_value(runtime, result);)^-^");
                }
                else
                {
                    writer.write(R"^-^(
                    return jsi::Value::undefined();)^-^");
                }

                writer.write(R"^-^(
                })^-^");
            }

            writer.write_fmt(R"^-^(
                throw_no_function_overload(runtime, "%"sv, "%"sv, "%"sv, count);
            }
        },)^-^",
                classData.type_def.TypeNamespace(), classData.type_def.TypeName(), rnwinrt::camel_case{ data.name });
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
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {)^-^");

        for (auto& ctorData : classData.methods.constructors)
        {
            writer.write_fmt(R"^-^(
        if (count == %)
        {)^-^",
                ctorData.method.param_count);

            write_rnwinrt_params_value_to_native(writer, ctorData.method, 3);

            writer.write_fmt("\n            return convert_native_to_value(runtime, winrt::%(",
                rnwinrt::cpp_typename{ classData.type_def });

            // NOTE: Already asserted no out params
            std::string_view prefix;
            for (int i = 0; i < ctorData.method.param_count; ++i)
            {
                writer.write_fmt("%arg%", prefix, i);
                prefix = ", "sv;
            }
            writer.write("));");

            writer.write("\n        }");
        }

        // NOTE: We use string concatenation here - instead of embedding the type name - to reduce the impact on
        // disk footprint. This isn't an expected/common code path, so the impact should hopefully be minimal
        // TODO: Actually validate this
        // TODO: Make throwing function a separate non-inlined function? Would also decrease disk footprint
        writer.write_fmt(R"^-^(
        throw_no_constructor(runtime, "%"sv, "%"sv, count);
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

static void write_rnwinrt_interface_projection_data(rnwinrt::writer& writer, const interface_projection_data& ifaceData)
{
    writer.write_fmt(R"^-^(
namespace rnwinrt::interfaces::%
{)^-^",
        rnwinrt::cpp_typename{ ifaceData.type_def });

    if (!ifaceData.methods.properties.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_interface_data::property_mapping property_data[] = {)^-^");

        for (auto& data : ifaceData.methods.properties)
        {
            writer.write_fmt(R"^-^(
        { "%",)^-^",
                rnwinrt::camel_case{ data.name });

            if (data.getter)
            {
                writer.write_fmt(R"^-^(
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::%>().%());
            },)^-^",
                    rnwinrt::cpp_typename{ ifaceData.type_def }, data.name);
            }
            else
            {
                writer.write("\n            nullptr,");
            }

            if (data.setter)
            {
                writer.write_fmt(R"^-^(
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::%>().%(convert_value_to_native<%>(runtime, value));
            },)^-^",
                    rnwinrt::cpp_typename{ ifaceData.type_def }, data.name,
                    [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, data.setter->params().first); });
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
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::%>().%(convert_value_to_native<%>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::%>().%(token);
            }
        },)^-^",
                rnwinrt::event_name{ data.name }, rnwinrt::cpp_typename{ ifaceData.type_def }, data.name,
                [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, data.type, {}); },
                rnwinrt::cpp_typename{ ifaceData.type_def }, data.name);
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
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {)^-^",
                    rnwinrt::camel_case{ data.name });

                write_rnwinrt_params_value_to_native(writer, overload.method, 5);

                writer.write_fmt(R"^-^(
                    %thisValue.as<winrt::%>().%()^-^",
                    overload.method.has_return_value ? "auto result = " : "",
                    rnwinrt::cpp_typename{ ifaceData.type_def }, data.name);

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
                    return %;)^-^",
                        [&](rnwinrt::writer& w) { write_rnwinrt_make_return_struct(w, overload.method); });
                }
                else if (overload.method.has_return_value)
                {
                    writer.write(R"^-^(
                    return convert_native_to_value(runtime, result);)^-^");
                }
                else
                {
                    writer.write(R"^-^(
                    return jsi::Value::undefined();)^-^");
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
        rnwinrt::cpp_typename{ ifaceData.type_def }, ifaceData.methods.properties.empty() ? "{}"sv : "property_data"sv,
        ifaceData.methods.events.empty() ? "{}"sv : "event_data"sv,
        ifaceData.methods.functions.empty() ? "{}"sv : "function_data"sv);
}

static void write_rnwinrt_namespace_cpp_files(const Settings& settings, const namespace_projection_data& ns)
{
    rnwinrt::file_writer writer(settings.OutputFolder / (std::string{ ns.full_name } + ".g.cpp"));
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
namespace rnwinrt::namespaces::%
{)^-^",
        rnwinrt::cpp_namespace{ &ns });

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
            writer.write_fmt("        &rnwinrt::%::%::%::data,\n", kindNs, rnwinrt::cpp_namespace{ &ns }, child->name);
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
        write_rnwinrt_enum_projection_data(writer, *enumData);
    }

    // Static class data
    for (auto& classData : ns.class_children)
    {
        write_rnwinrt_class_projection_data(writer, *classData);
    }

    // Static interface data
    for (auto& ifaceData : ns.interface_children)
    {
        write_rnwinrt_interface_projection_data(writer, *ifaceData);
    }

    // projected_value_traits definitions
    if (!ns.struct_children.empty() || !ns.delegate_children.empty())
    {
        writer.write("\nnamespace rnwinrt\n{");

        for (auto& structData : ns.struct_children)
        {
            writer.write_fmt(R"^-^(
    jsi::Value projected_value_traits<winrt::%>::as_value(jsi::Runtime& runtime, const winrt::%& value)
    {
        jsi::Object result(runtime);
)^-^",
                rnwinrt::cpp_typename{ structData->type_def }, rnwinrt::cpp_typename{ structData->type_def });

            for (auto&& field : structData->type_def.FieldList())
            {
                writer.write_fmt(
                    R"^-^(        result.setProperty(runtime, "%", convert_native_to_value(runtime, value.%));
)^-^",
                    rnwinrt::camel_case{ field.Name() }, field.Name());
            }

            writer.write_fmt(R"^-^(        return result;
    }

    winrt::% projected_value_traits<winrt::%>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::% result{};
        auto obj = value.asObject(runtime);
)^-^",
                rnwinrt::cpp_typename{ structData->type_def }, rnwinrt::cpp_typename{ structData->type_def },
                rnwinrt::cpp_typename{ structData->type_def });

            for (auto&& field : structData->type_def.FieldList())
            {
                auto sig = field.Signature();
                writer.write_fmt(R"^-^(        if (auto field = obj.getProperty(runtime, "%"); !field.isUndefined())
            result.% = convert_value_to_native<%>(runtime, field);
)^-^",
                    rnwinrt::camel_case{ field.Name() }, field.Name(),
                    [&](rnwinrt::writer& w) { rnwinrt::write_cppwinrt_type(w, sig.Type(), {}); });
            }

            writer.write("        return result;\n    }\n");
        }

        for (auto& delegateData : ns.delegate_children)
        {
            function_signature fn(delegate_invoke_function(delegateData->type_def));

            writer.write_fmt(R"^-^(
    jsi::Value projected_value_traits<winrt::%>::as_value(jsi::Runtime& runtime, const winrt::%& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "%"), %,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != %)
                {
                    throw_invalid_delegate_arg_count(runtime, "%"sv, "%"sv);
                }
)^-^",
                rnwinrt::cpp_typename{ delegateData->type_def }, rnwinrt::cpp_typename{ delegateData->type_def },
                delegateData->type_def.TypeName(), fn.param_count, fn.param_count,
                delegateData->type_def.TypeNamespace(), delegateData->type_def.TypeName());

            write_rnwinrt_params_value_to_native(writer, fn, 4);
            writer.write_fmt("\n                %value(", fn.has_return_value ? "auto result = " : "");

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
                writer.write_fmt(R"^-^(
                return %;)^-^",
                    [&](rnwinrt::writer& w) { write_rnwinrt_make_return_struct(w, fn); });
            }
            else if (fn.has_return_value)
            {
                writer.write("\n                return convert_native_to_value(runtime, result);");
            }
            else
            {
                writer.write("\n                return jsi::Value::undefined();");
            }

            writer.write_fmt(R"^-^(
            });
    }

    winrt::% projected_value_traits<winrt::%>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](%) {)^-^",
                rnwinrt::cpp_typename{ delegateData->type_def }, rnwinrt::cpp_typename{ delegateData->type_def },
                [&](rnwinrt::writer& w) { write_rnwinrt_native_function_params(w, fn); });

            auto writeReturnType = [&](rnwinrt::writer& w) {
                resolve_type(fn.signature.ReturnType().Type(), {},
                    overloaded{
                        [&](const GenericTypeInstSig& sig, const generic_param_stack& genStack, bool isArray) {
                            if (isArray)
                                w.write("winrt::com_array<"sv);
                            rnwinrt::write_cppwinrt_type(w, sig, genStack);
                            if (isArray)
                                w.write(">");
                        },
                        [&](auto&& value, bool isArray) {
                            if (isArray)
                                w.write("winrt::com_array<"sv);
                            rnwinrt::write_cppwinrt_type(w, value);
                            if (isArray)
                                w.write(">");
                        },
                    });
            };

            if (fn.has_return_value)
            {
                //
                writer.write_fmt(R"^-^(
            % returnValue%;)^-^",
                    writeReturnType,
                    [&](rnwinrt::writer& w) { write_cppwinrt_type_initializer(w, fn.signature.ReturnType().Type()); });
            }

            writer.write(R"^-^(
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;)^-^");

            write_rnwinrt_params_native_to_value(writer, fn, 4);

            writer.write_fmt(R"^-^(
                %fn.call(runtime)^-^",
                (fn.has_return_value || fn.has_out_params) ? "auto result = " : "");

            int argNum = 0;
            for (auto&& param : fn.params())
            {
                if (param.is_input())
                {
                    writer.write_fmt(", arg%", argNum);
                }
                else if (!param.by_ref())
                {
                    // Out & not by ref is "fill array". Due to the way JSI handles function invocation, it doesn't
                    // realize that the argument we pass it is convertible to jsi::Value
                    writer.write_fmt(", arg%.value()", argNum);
                }
                ++argNum;
            }
            writer.write_fmt(");");

            if (fn.has_out_params)
            {
                write_rnwinrt_native_out_params(writer, fn, 4);

                if (fn.has_return_value)
                {
                    // This is a bit different than below since we need to "extract" the value
                    writer.write_fmt(R"^-^(
                returnValue = convert_value_to_native<%>(runtime, obj.getProperty(runtime, "returnValue"));)^-^",
                        writeReturnType);
                }
            }
            else if (fn.has_return_value)
            {
                writer.write_fmt(
                    "\n                returnValue = convert_value_to_native<%>(runtime, result);", writeReturnType);
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
        write_rnwinrt_namespace_cpp_files(settings, *childNs);
    }
}

static void write_rnwinrt_namespace_cpp_files(const Settings& settings, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_rnwinrt_namespace_cpp_files(settings, *ns);
    }
}

static void write_rnwinrt_base_files(const Settings& settings)
{
    for (auto& data : rnwinrt::file_strings)
    {
        rnwinrt::file_writer writer(settings.OutputFolder / data.file_name);
        for (auto& str : data.file_contents)
        {
            writer.write(str);
        }
    }
}

void write_rnwinrt_js_module_file(const Settings& settings, const projection_data& data)
{
    // Write the 'module.js' file that exports root namespaces as well as the default module
    rnwinrt::file_writer writer(settings.JsOutputFolder / "module.js");
    writer.write(R"^-^(
import { TurboModuleRegistry } from 'react-native';

const module = TurboModuleRegistry.get('WinRTTurboModule');

export default module;
)^-^");

    for (auto& ns : data.root_namespaces)
    {
        writer.write_fmt("export const % = module ? module.% : undefined;\n", ns->full_name, ns->full_name);
    }
}

void write_rnwinrt_files(const Settings& settings, const projection_data& data)
{
    // Projections.g.cpp
    write_rnwinrt_projections_cpp_file(settings, data);
    // ProjectedValueConverters.g.h
    write_rnwinrt_value_converters_header(settings, data);
    // <Namespace>.g.h
    write_rnwinrt_namespace_headers(settings, data);
    // <Namespace>.g.cpp
    write_rnwinrt_namespace_cpp_files(settings, data);

    // base.h/base.cpp
    write_rnwinrt_base_files(settings);

    if (settings.GenerateModule)
    {
        // Projections.js
        write_rnwinrt_js_module_file(settings, data);
    }
}
