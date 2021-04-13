#include "pch.h"

#include "FileGenerator.h"
#include "MetadataHelpers.h"
#include "TextWriter.h"
#include "TypescriptWriter.h"
#include "Writer.h"

#include <strings.h>

using namespace winmd::reader;

static void write_projections_cpp_single_namespace_include(jswinrt_writer& writer, const namespace_projection_data& ns)
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

static void write_projections_cpp_namespace_includes(jswinrt_writer& writer, const namespace_projection_data& ns)
{
    write_projections_cpp_single_namespace_include(writer, ns);

    for (auto& childNs : ns.namespace_children)
    {
        write_projections_cpp_namespace_includes(writer, *childNs);
    }
}

static void write_projections_cpp_namespace_includes(jswinrt_writer& writer, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_projections_cpp_namespace_includes(writer, *ns);
    }
}

static void write_projections_cpp_file(const Settings& settings, const projection_data& data)
{
    jswinrt_file_writer writer(settings.OutputFolder / "Projections.g.cpp");
    writer.write_fmt(R"^-^(#include "%"

#include "base.h"

)^-^",
        settings.PchFileName);

    write_projections_cpp_namespace_includes(writer, data);

    // 'root_namespaces' definition
    writer.write(R"^-^(
namespace jswinrt
{
    static constexpr const static_namespace_data* const root_namespace_data[] = {
)^-^");

    for (auto& ns : data.root_namespaces)
    {
        writer.write_fmt("        &jswinrt::namespaces::%::data,\n", ns->name);
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
            "\n        { winrt::guid_of<winrt::%>(), &jswinrt::interfaces::% },",
            [&](jswinrt_writer& w) { iface->write_cpp_name(w, ""sv); },
            [&](jswinrt_writer& w) { iface->write_cpp_name(w, "::data"sv); });
#if 1 // For IID gen validation
        writer.write_fmt(" // %", fmt_guid{ iface->iid });
#endif
    }

    writer.write(R"^-^(
    };

    constexpr const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map(global_interface_map_data);
}
)^-^");
}

void write_value_converters_decls(jswinrt_writer& writer, const namespace_projection_data& ns)
{
    if (!ns.struct_children.empty() || !ns.delegate_children.empty())
    {
        // Forward declare the C++/WinRT types (because this file gets included in all cpp files)
        writer.write_fmt("namespace winrt::%\n{\n", cpp_namespace{ &ns });

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

namespace jswinrt
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
                cpp_typename{ structDef->type_def }, cpp_typename{ structDef->type_def },
                cpp_typename{ structDef->type_def });
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
                cpp_typename{ delegateDef->type_def }, cpp_typename{ delegateDef->type_def },
                cpp_typename{ delegateDef->type_def });
        }

        writer.write("}\n\n");
    }

    for (auto& childNs : ns.namespace_children)
    {
        write_value_converters_decls(writer, *childNs);
    }
}

void write_value_converters_decls(jswinrt_writer& writer, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_value_converters_decls(writer, *ns);
    }
}

void write_value_converters_header(const Settings& settings, const projection_data& data)
{
    jswinrt_file_writer writer(settings.OutputFolder / "ProjectedValueConverters.g.h");

    writer.write(R"^-^(#pragma once

#include "base.h"

)^-^");

    write_value_converters_decls(writer, data);
}

void write_namespace_headers(const Settings& settings, const namespace_projection_data& ns)
{
    jswinrt_file_writer writer(settings.OutputFolder / (std::string{ ns.full_name } + ".g.h"));
    writer.write_fmt(R"^-^(#pragma once

#include "base.h"

namespace jswinrt::namespaces::%
{
    extern const static_namespace_data data;
}
)^-^",
        cpp_namespace{ &ns });

    // Static enum data declarations
    if (!ns.enum_children.empty())
    {
        writer.write_fmt(R"^-^(
namespace jswinrt::enums::%
{)^-^",
            cpp_namespace{ &ns });

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
namespace jswinrt::classes::%
{)^-^",
            cpp_namespace{ &ns });

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
namespace jswinrt::interfaces::%
{)^-^",
            cpp_namespace{ &ns });

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
        write_namespace_headers(settings, *childNs);
    }
}

void write_namespace_headers(const Settings& settings, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_namespace_headers(settings, *ns);
    }
}

static void write_cppwinrt_type(jswinrt_writer& writer, ElementType elemType)
{
    std::string_view str;
    switch (elemType)
    {
    case ElementType::Boolean:
        str = "bool"sv;
        break;
    case ElementType::Char:
        str = "char16_t"sv;
        break;
    case ElementType::I1:
        str = "int8_t"sv;
        break;
    case ElementType::U1:
        str = "uint8_t"sv;
        break;
    case ElementType::I2:
        str = "int16_t"sv;
        break;
    case ElementType::U2:
        str = "uint16_t"sv;
        break;
    case ElementType::I4:
        str = "int32_t"sv;
        break;
    case ElementType::U4:
        str = "uint32_t"sv;
        break;
    case ElementType::I8:
        str = "int64_t"sv;
        break;
    case ElementType::U8:
        str = "uint64_t"sv;
        break;
    case ElementType::R4:
        str = "float"sv;
        break;
    case ElementType::R8:
        str = "double"sv;
        break;
    case ElementType::String:
        str = "winrt::hstring"sv;
        break;
    case ElementType::Object:
        str = "winrt::Windows::Foundation::IInspectable"sv;
        break;
    default:
        assert(false);
        throw std::runtime_error("Unexpected ElementType");
    }
    writer.write(str);
}

static void write_cppwinrt_type(jswinrt_writer& writer, const TypeDef& typeDef)
{
    assert(!is_generic(typeDef));
    writer.write_fmt("winrt::%", cpp_typename{ typeDef });
}

static void write_cppwinrt_type(jswinrt_writer& writer, system_guid_t)
{
    writer.write("winrt::guid");
}

static void write_cppwinrt_type(
    jswinrt_writer& writer, const TypeSig& sig, const generic_param_stack& genericParamStack);

static void write_cppwinrt_type(
    jswinrt_writer& writer, const GenericTypeInstSig& sig, const generic_param_stack& genericParamStack)
{
    auto [ns, name] = get_type_namespace_and_name(sig.GenericType());
    writer.write_fmt("winrt::%<", cpp_typename{ ns, remove_tick(name) });

    std::string_view prefix;
    for (auto&& arg : sig.GenericArgs())
    {
        writer.write(prefix);
        write_cppwinrt_type(writer, arg, genericParamStack);
        prefix = ", ";
    }

    writer.write(">");
}

static void write_cppwinrt_type(
    jswinrt_writer& writer, const TypeSig& sig, const generic_param_stack& genericParamStack)
{
    resolve_type(sig, genericParamStack,
        overloaded{
            [&](const GenericTypeInstSig& genericSig, const generic_param_stack& newStack, bool isArray) {
                assert(!isArray);
                write_cppwinrt_type(writer, genericSig, newStack);
            },
            [&](auto&& value, bool isArray) {
                assert(!isArray);
                write_cppwinrt_type(writer, value);
            },
        });
}

static void write_cppwinrt_type(jswinrt_writer& writer, const param_iterator& param)
{
    auto writeArrayBegin = [&](bool isArray) {
        if (isArray)
        {
            if (param.is_input())
            {
                assert(!param.by_ref()); // What would the semantics of this even be?
                writer.write("winrt::array_view<const ");
            }
            else if (!param.by_ref())
            {
                // Out, but not ByRef is how fill arrays are indicated
                writer.write("winrt::array_view<");
            }
            else
            {
                writer.write("winrt::com_array<");
            }
        }
    };

    auto writeArrayEnd = [&](bool isArray) {
        if (isArray)
        {
            writer.write(">");
        }
    };

    // NOTE: We don't manually project generic types, so we can get away with assuming an empty generic param stack
    resolve_type(param.signature(), {},
        overloaded{
            [&](const GenericTypeInstSig& sig, const generic_param_stack& newStack, bool isArray) {
                writeArrayBegin(isArray);
                write_cppwinrt_type(writer, sig, newStack);
                writeArrayEnd(isArray);
            },
            [&](auto&& value, bool isArray) {
                writeArrayBegin(isArray);
                write_cppwinrt_type(writer, value);
                writeArrayEnd(isArray);
            },
        });
}

static void write_cppwinrt_type_initializer(jswinrt_writer& writer, const TypeSig& type)
{
    // Classes need to be initialized with 'nullptr' because otherwise it's a constructor invocation
    // NOTE: Similar to the above, we aren't projecting generic types, so we can get away with an empty param stack
    resolve_type(type, {},
        overloaded{
            [&](const TypeDef& type) {
                auto c = get_category(type);
                if (c == category::class_type)
                {
                    writer.write("{ nullptr }");
                }
            },
            [&](const TypeDef& type, bool isArray) {
                if (isArray)
                    return;

                auto c = get_category(type);
                if (c == category::class_type)
                {
                    writer.write("{ nullptr }");
                }
            },
            [&](auto&&...) {},
        });
}

static void write_params_value_to_native(jswinrt_writer& writer, const function_signature& fn, int indentLevel)
{
    auto params = fn.params();
    int paramNum = 0;
    for (int argNum = 0; params.first != params.second; ++params.first, ++argNum)
    {
        if (!params.first.by_ref())
        {
            writer.write_fmt(
                "\n%auto arg% = convert_value_to_native<%>(runtime, args[%]);", indent{ indentLevel }, argNum,
                [&](jswinrt_writer& w) { write_cppwinrt_type(w, params.first); }, paramNum++);
        }
        else
        {
            writer.write_fmt(
                "\n%% arg%%;", indent{ indentLevel }, [&](jswinrt_writer& w) { write_cppwinrt_type(w, params.first); },
                argNum, [&](jswinrt_writer& w) { write_cppwinrt_type_initializer(w, params.first.type()); });
        }
    }
}

static void write_make_return_struct(jswinrt_writer& writer, const function_signature& fn)
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
            writer.write_fmt(R"^-^(, "%", arg%)^-^", camel_case{ param.name() }, argNum);
        }
        ++argNum;
    }

    writer.write(")");
}

static void write_native_function_params(jswinrt_writer& writer, const function_signature& fn)
{
    std::string_view prefix;
    int argNum = 0;
    for (auto&& param : fn.params())
    {
        writer.write(prefix);
        prefix = ", ";

        write_cppwinrt_type(writer, param);
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

static void write_params_native_to_value(jswinrt_writer& writer, const function_signature& fn, int indentLevel)
{
    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_input() || !param.by_ref())
        {
            writer.write_fmt(
                "\n%auto arg% = convert_native_to_value(runtime, param%);", indent{ indentLevel }, argNum, argNum);
        }
        ++argNum;
    }
}

static void write_native_out_params(jswinrt_writer& writer, const function_signature& fn, int indentLevel)
{
    assert(fn.has_out_params);
    writer.write_fmt("\n%auto obj = result.asObject(runtime);", indent{ indentLevel });

    int argNum = 0;
    for (auto&& param : fn.params())
    {
        if (param.is_output() && param.by_ref())
        {
            writer.write_fmt(
                R"^-^(
%param% = convert_value_to_native<%>(runtime, obj.getProperty(runtime, "%"));)^-^",
                indent{ indentLevel }, argNum, [&](jswinrt_writer& w) { write_cppwinrt_type(w, param); },
                camel_case{ param.name() });
        }
        ++argNum;
    }
}

static void write_enum_projection_data(jswinrt_writer& writer, const enum_projection_data& enumData)
{
    writer.write_fmt(R"^-^(
namespace jswinrt::enums::%
{
    static constexpr const static_enum_data::value_mapping mappings[] = {)^-^",
        cpp_typename{ enumData.type_def });

    for (auto&& field : enumData.type_def.FieldList())
    {
        if (!field.Constant()) // Enums have a 'value__' data member we need to ignore
            continue;

        writer.write_fmt(R"^-^(
        { "%"sv, static_cast<double>(winrt::%::%) },)^-^",
            camel_case{ field.Name() }, cpp_typename{ enumData.type_def }, field.Name());
    }

    writer.write_fmt(R"^-^(
    };

    constexpr const static_enum_data data{ "%"sv, mappings };
}
)^-^",
        enumData.name);
}

static void write_class_projection_data(jswinrt_writer& writer, const class_projection_data& classData)
{
    writer.write_fmt(R"^-^(
namespace jswinrt::classes::%
{)^-^",
        cpp_typename{ classData.type_def });

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
                camel_case{ data.name }, cpp_typename{ classData.type_def }, data.name);

            if (data.setter)
            {
                writer.write_fmt(R"^-^(
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::%::%(convert_value_to_native<%>(runtime, value));
            },)^-^",
                    cpp_typename{ classData.type_def }, data.name,
                    [&](jswinrt_writer& w) { write_cppwinrt_type(w, data.setter->params().first); });
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
                event_name{ data.name }, cpp_typename{ classData.type_def }, data.name,
                [&](jswinrt_writer& w) { write_cppwinrt_type(w, data.type, {}); }, cpp_typename{ classData.type_def },
                data.name);
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
                camel_case{ data.name });

            for (auto& overload : data.overloads)
            {
                writer.write_fmt(R"^-^(
                if (count == %)
                {)^-^",
                    overload.arity);

                write_params_value_to_native(writer, overload.method, 5);

                writer.write_fmt(R"^-^(
                    %winrt::%::%()^-^",
                    overload.method.has_return_value ? "auto result = " : "", cpp_typename{ classData.type_def },
                    data.name);

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
                        [&](jswinrt_writer& w) { write_make_return_struct(w, overload.method); });
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
                throw jsi::JSError(runtime, "TypeError: No function overload exists for "s + "%." + "%" +
                    " with " + std::to_string(count) + " args");
            }
        },)^-^",
                classData.type_def.TypeNamespace(), classData.type_def.TypeName());
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

            write_params_value_to_native(writer, ctorData.method, 3);

            writer.write_fmt(
                "\n            return convert_native_to_value(runtime, winrt::%(", cpp_typename{ classData.type_def });

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
        throw jsi::JSError(runtime, "TypeError: No constructor overload exists for "s + "%." + "%" +
            " with " + std::to_string(count) + " args");
    }

    constexpr const static_activatable_class_data data{ "%"sv, constructor_function, %, %, % };
}
)^-^",
            classData.type_def.TypeNamespace(), classData.type_def.TypeName(), classData.name,
            classData.methods.properties.empty() ? "{}"sv : "property_data"sv,
            classData.methods.events.empty() ? "{}"sv : "event_data"sv,
            classData.methods.functions.empty() ? "{}"sv : "function_data"sv);
    }
}

static void write_interface_projection_data(jswinrt_writer& writer, const interface_projection_data& ifaceData)
{
    writer.write_fmt(R"^-^(
namespace jswinrt::interfaces::%
{)^-^",
        cpp_typename{ ifaceData.type_def });

    if (!ifaceData.methods.properties.empty())
    {
        writer.write(R"^-^(
    static constexpr const static_interface_data::property_mapping property_data[] = {)^-^");

        for (auto& data : ifaceData.methods.properties)
        {
            writer.write_fmt(R"^-^(
        { "%",)^-^",
                camel_case{ data.name });

            if (data.getter)
            {
                writer.write_fmt(R"^-^(
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::%>().%());
            },)^-^",
                    cpp_typename{ ifaceData.type_def }, data.name);
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
                    cpp_typename{ ifaceData.type_def }, data.name,
                    [&](jswinrt_writer& w) { write_cppwinrt_type(w, data.setter->params().first); });
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
                event_name{ data.name }, cpp_typename{ ifaceData.type_def }, data.name,
                [&](jswinrt_writer& w) { write_cppwinrt_type(w, data.type, {}); }, cpp_typename{ ifaceData.type_def },
                data.name);
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
                    camel_case{ data.name });

                write_params_value_to_native(writer, overload.method, 5);

                writer.write_fmt(R"^-^(
                    %thisValue.as<winrt::%>().%()^-^",
                    overload.method.has_return_value ? "auto result = " : "", cpp_typename{ ifaceData.type_def },
                    data.name);

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
                        [&](jswinrt_writer& w) { write_make_return_struct(w, overload.method); });
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
        cpp_typename{ ifaceData.type_def }, ifaceData.methods.properties.empty() ? "{}"sv : "property_data"sv,
        ifaceData.methods.events.empty() ? "{}"sv : "event_data"sv,
        ifaceData.methods.functions.empty() ? "{}"sv : "function_data"sv);
}

void write_namespace_cpp_files(const Settings& settings, const namespace_projection_data& ns)
{
    jswinrt_file_writer writer(settings.OutputFolder / (std::string{ ns.full_name } + ".g.cpp"));
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
namespace jswinrt::namespaces::%
{)^-^",
        cpp_namespace{ &ns });

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
            writer.write_fmt("        &jswinrt::%::%::%::data,\n", kindNs, cpp_namespace{ &ns }, child->name);
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
        write_enum_projection_data(writer, *enumData);
    }

    // Static class data
    for (auto& classData : ns.class_children)
    {
        write_class_projection_data(writer, *classData);
    }

    // Static interface data
    for (auto& ifaceData : ns.interface_children)
    {
        write_interface_projection_data(writer, *ifaceData);
    }

    // projected_value_traits definitions
    if (!ns.struct_children.empty() || !ns.delegate_children.empty())
    {
        writer.write("\nnamespace jswinrt\n{");

        for (auto& structData : ns.struct_children)
        {
            writer.write_fmt(R"^-^(
    jsi::Value projected_value_traits<winrt::%>::as_value(jsi::Runtime& runtime, const winrt::%& value)
    {
        jsi::Object result(runtime);
)^-^",
                cpp_typename{ structData->type_def }, cpp_typename{ structData->type_def });

            for (auto&& field : structData->type_def.FieldList())
            {
                writer.write_fmt(
                    R"^-^(        result.setProperty(runtime, "%", convert_native_to_value(runtime, value.%));
)^-^",
                    camel_case{ field.Name() }, field.Name());
            }

            writer.write_fmt(R"^-^(        return result;
    }

    winrt::% projected_value_traits<winrt::%>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::% result{};
        auto obj = value.asObject(runtime);
)^-^",
                cpp_typename{ structData->type_def }, cpp_typename{ structData->type_def },
                cpp_typename{ structData->type_def });

            for (auto&& field : structData->type_def.FieldList())
            {
                auto sig = field.Signature();
                writer.write_fmt(R"^-^(        if (auto field = obj.getProperty(runtime, "%"); !field.isUndefined())
            result.% = convert_value_to_native<%>(runtime, field);
)^-^",
                    camel_case{ field.Name() }, field.Name(),
                    [&](jswinrt_writer& w) { write_cppwinrt_type(w, sig.Type(), {}); });
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
                    throw jsi::JSError(runtime, "TypeError: Invalid number of arguments to %");
                }
)^-^",
                cpp_typename{ delegateData->type_def }, cpp_typename{ delegateData->type_def },
                delegateData->type_def.TypeName(), fn.param_count, fn.param_count, delegateData->type_def.TypeName());

            write_params_value_to_native(writer, fn, 4);
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
                    [&](jswinrt_writer& w) { write_make_return_struct(w, fn); });
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
                cpp_typename{ delegateData->type_def }, cpp_typename{ delegateData->type_def },
                [&](jswinrt_writer& w) { write_native_function_params(w, fn); });

            auto writeReturnType = [&](jswinrt_writer& w) {
                resolve_type(fn.signature.ReturnType().Type(), {},
                    overloaded{
                        [&](const GenericTypeInstSig& sig, const generic_param_stack& genStack, bool isArray) {
                            if (isArray)
                                w.write("winrt::com_array<"sv);
                            write_cppwinrt_type(w, sig, genStack);
                            if (isArray)
                                w.write(">");
                        },
                        [&](auto&& value, bool isArray) {
                            if (isArray)
                                w.write("winrt::com_array<"sv);
                            write_cppwinrt_type(w, value);
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
                    [&](jswinrt_writer& w) { write_cppwinrt_type_initializer(w, fn.signature.ReturnType().Type()); });
            }

            writer.write(R"^-^(
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;)^-^");

            write_params_native_to_value(writer, fn, 4);

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
                write_native_out_params(writer, fn, 4);

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
        write_namespace_cpp_files(settings, *childNs);
    }
}

void write_namespace_cpp_files(const Settings& settings, const projection_data& data)
{
    for (auto& ns : data.root_namespaces)
    {
        write_namespace_cpp_files(settings, *ns);
    }
}

void write_base_files(const Settings& settings)
{
    for (auto& data : file_strings)
    {
        jswinrt_file_writer writer(settings.OutputFolder / data.file_name);
        for (auto& str : data.file_contents)
        {
            writer.write(str);
        }
    }
}

void write_typescript_file(const Settings& settings, const namespace_projection_data& ns)
{
    TextWriter textWriter;
    TypescriptWriter typescriptWriter(settings);
    typescriptWriter.Write(ns, textWriter);
    textWriter.FlushToFile(settings.TypescriptOutputFolder / (std::string(ns.full_name) + ".d.ts"));

    for (auto& childNs : ns.namespace_children)
    {
        write_typescript_file(settings, *childNs);
    }
}

void write_typescript_files(const Settings& settings, const projection_data& data)
{
    if (settings.TypescriptOutputFolder.empty())
    {
        return;
    }

    std::filesystem::remove_all(settings.TypescriptOutputFolder);
    std::filesystem::create_directory(settings.TypescriptOutputFolder);

    for (auto& ns : data.root_namespaces)
    {
        write_typescript_file(settings, *ns);
    }
}

void write_files(const Settings& settings, const projection_data& data)
{
    // Projections.g.cpp
    write_projections_cpp_file(settings, data);
    // ProjectedValueConverters.g.h
    write_value_converters_header(settings, data);
    // <Namespace>.g.h
    write_namespace_headers(settings, data);
    // <Namespace>.g.cpp
    write_namespace_cpp_files(settings, data);

    // base.h/base.cpp
    write_base_files(settings);

    // TypeScript files, if necessary
    write_typescript_files(settings, data);
}

static void write_cpp_namespace(jswinrt_writer& writer, std::string_view ns)
{
    while (true)
    {
        auto pos = ns.find_first_of(".");
        if (pos == std::string_view::npos)
        {
            break;
        }

        writer.write(ns.substr(0, pos));
        writer.write("::"sv);
        ns = ns.substr(pos + 1);
    }

    writer.write(ns);
}

void cpp_namespace::operator()(jswinrt_writer& writer) const
{
    if (!data->base_namespace.empty())
    {
        write_cpp_namespace(writer, data->base_namespace);
        writer.write("::"sv);
    }

    writer.write(data->name);
}

void cpp_typename::operator()(jswinrt_writer& writer) const
{
    if (type_namespace == foundation_namespace)
    {
        if (type_name == "EventRegistrationToken"sv)
        {
            return writer.write("event_token"sv);
        }
        else if (type_name == "HResult"sv)
        {
            return writer.write("hresult"sv);
        }
    }
    else if (type_namespace == "Windows.Foundation.Numerics"sv)
    {
        auto itr = std::find_if(std::begin(numerics_mappings), std::end(numerics_mappings),
            [&](auto& pair) { return pair.first == type_name; });
        if (itr != std::end(numerics_mappings))
        {
            return writer.write_fmt("Windows::Foundation::Numerics::%", itr->second);
        }
    }

    write_cpp_namespace(writer, type_namespace);
    writer.write("::"sv);
    writer.write(type_name);
}

void interface_projection_data::write_cpp_name(jswinrt_writer& writer, std::string_view typeNameMod)
{
    cpp_typename{ type_def }(writer);
    writer.write(typeNameMod);
}

void generic_interface_instantiation::write_cpp_name(jswinrt_writer& writer, std::string_view typeNameMod)
{
    auto& sig = instantiation.signature();
    auto typeDef = generic_type_def(sig);
    writer.write_fmt("%%<", cpp_typename{ typeDef.TypeNamespace(), remove_tick(typeDef.TypeName()) }, typeNameMod);

    std::string_view prefix;
    auto genericParamStack = instantiation.parent_stack();
    for (auto&& param : sig.GenericArgs())
    {
        writer.write_fmt("%%", prefix, [&](jswinrt_writer& w) { write_cppwinrt_type(w, param, genericParamStack); });
        prefix = ", "sv;
    }

    writer.write(">");
}
