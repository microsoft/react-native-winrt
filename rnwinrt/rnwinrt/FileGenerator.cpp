#include "pch.h"

#include "FileGenerator.h"
#include "MetadataHelpers.h"
#include "TextWriter.h"
#include "TypescriptWriter.h"
#include "ValueConverters.h"
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

)^-^", settings.PchFileName);

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
        R"^-^(    static constexpr const std::pair<winrt::guid, const static_interface_data*> global_interface_map_data[] = {
)^-^");

    for (auto iface : data.interfaces)
    {
        writer.write_fmt("        { winrt::guid_of<winrt::%>(), &jswinrt::interfaces::%::data },\n",
            cpp_typename{ iface->type_def }, cpp_typename{ iface->type_def });
    }

    writer.write(R"^-^(    };

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
                is_activatable(classDef->type_def) ? "static_activatable_class_data"sv : "static_class_data"sv);
        }

        writer.write("}\n");
    }

    // Static interface data declarations (TODO? Can this be moved to the cpp file, right?)
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
        break;
    }
    writer.write(str);
}

static void write_cppwinrt_type(jswinrt_writer& writer, const TypeDef& typeDef)
{
    assert(distance(typeDef.GenericParam()) == 0);
    writer.write_fmt("winrt::%", cpp_typename{ typeDef });
}

static void write_cppwinrt_type(jswinrt_writer& writer, system_guid_t)
{
    writer.write("winrt::guid");
}

static void write_cppwinrt_type(jswinrt_writer& writer, const TypeSig& sig);

static void write_cppwinrt_type(jswinrt_writer& writer, const GenericTypeInstSig& sig)
{
    auto [ns, name] = get_type_namespace_and_name(sig.GenericType());
    writer.write_fmt("winrt::%<", cpp_typename{ ns, remove_tick(name) });

    std::string_view prefix;
    for (auto&& arg : sig.GenericArgs())
    {
        writer.write(prefix);
        write_cppwinrt_type(writer, arg);
        prefix = ", ";
    }

    writer.write(">");
}

static void write_cppwinrt_type(jswinrt_writer& writer, const TypeSig& sig)
{
    resolve_type(sig, [&](auto&& value, bool isArray) {
        if (isArray) __debugbreak(); // TODO
        write_cppwinrt_type(writer, value);
    });
}

static void write_cppwinrt_type(jswinrt_writer& writer, const param_iterator& param)
{
    resolve_type(param.signature(), [&](auto&& value, bool isArray) {
        if (isArray)
        {
            if (param.is_input())
            {
                assert(!param.by_ref());
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

        write_cppwinrt_type(writer, value);

        if (isArray)
        {
            writer.write(">");
        }
    });
}

template <typename T>
static void write_cppwinrt_type_initializer(jswinrt_writer& writer, T&& type)
{
    // Classes need to be initialized with 'nullptr' because otherwise it's a constructor invocation
    resolve_type(type, overloaded{
                           [&](const TypeDef& type) {
                               auto c = get_category(type);
                               if (c == category::class_type)
                               {
                                   writer.write("{ nullptr }");
                               }
                           },
                           [&](const TypeDef& type, bool isArray) {
                               if (isArray)
                                   return; // TODO: Safe to do this?

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
    auto begin = fn.param_begin();
    auto end = fn.param_end();
    for (int argNum = 0; begin != end; ++begin, ++argNum)
    {
        if (!begin.by_ref())
        {
            writer.write_fmt(
                "\n%auto arg% = convert_value_to_native<%>(runtime, args[%]);", indent{ indentLevel }, argNum,
                [&](jswinrt_writer& w) { write_cppwinrt_type(w, begin); }, argNum);
        }
        else
        {
            writer.write_fmt(
                "\n%% arg%%;", indent{ indentLevel }, [&](jswinrt_writer& w) { write_cppwinrt_type(w, begin); }, argNum,
                [&](jswinrt_writer& w) { write_cppwinrt_type_initializer(w, begin.type()); });
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

    auto begin = fn.param_begin();
    auto end = fn.param_end();
    for (int argNum = 0; begin != end; ++begin, ++argNum)
    {
        if (begin.is_output())
        {
            writer.write_fmt(R"^-^(, "%", arg%)^-^", camel_case{ begin.name() }, argNum);
        }
    }

    writer.write(")");
}

static void write_native_function_params(jswinrt_writer& writer, const function_signature& fn)
{
    auto begin = fn.param_begin();
    auto end = fn.param_end();
    std::string_view prefix;
    for (int argNum = 0; begin != end; ++begin, ++argNum)
    {
        writer.write(prefix);
        prefix = ", ";

        write_cppwinrt_type(writer, begin);
        if (begin.by_ref())
        {
            writer.write("&");
        }
        else
        {
            resolve_type(begin.signature(), overloaded{
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
                                    [&](const GenericTypeInstSig&, bool) { writer.write(" const&"); },
                                    [&](system_guid_t, bool) {},
                                });
        }

        writer.write_fmt(" param%", argNum);
    }
}

static void write_params_native_to_value(jswinrt_writer& writer, const function_signature& fn, int indentLevel)
{
    auto begin = fn.param_begin();
    auto end = fn.param_end();
    for (int argNum = 0; begin != end; ++begin, ++argNum)
    {
        if (begin.is_input())
        {
            writer.write_fmt(
                "\n%auto arg% = convert_native_to_value(runtime, param%);", indent{ indentLevel }, argNum, argNum);
        }
    }
}

static void write_native_out_params(jswinrt_writer& writer, const function_signature& fn, int indentLevel)
{
    assert(fn.has_out_params);
    writer.write_fmt("\n%auto obj = result.asObject(runtime);", indent{ indentLevel });

    auto begin = fn.param_begin();
    auto end = fn.param_end();
    for (int argNum = 0; begin != end; ++begin, ++argNum)
    {
        if (begin.is_output())
        {
            writer.write_fmt(
                R"^-^(
%param% = convert_value_to_native<%>(runtime, obj.getProperty(runtime, "%"));)^-^",
                indent{ indentLevel }, argNum, [&](jswinrt_writer& w) { write_cppwinrt_type(w, begin); },
                camel_case{ begin.name() });
        }
    }
}

static void write_enum_projection_data(jswinrt_writer& writer, const enum_projection_data& enumData)
{
    writer.write_fmt(R"^-^(
namespace jswinrt::enums::%
{
    static constexpr const static_enum_data::value_mapping mappings[] = {)^-^", cpp_typename{ enumData.type_def });

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
)^-^", enumData.name);
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
            writer.write_fmt(R"^-^(
        { "%",)^-^", camel_case{ data.name });

            if (data.has_getter)
            {
                writer.write_fmt(R"^-^(
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::%::%());
            },)^-^",
                    cpp_typename{ classData.type_def }, data.name);
            }
            else
            {
                writer.write("\n            nullptr,");
            }

            if (data.has_setter)
            {
                writer.write_fmt(R"^-^(
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::%::%(convert_value_to_native<%>(runtime, value));
            },)^-^",
                    cpp_typename{ classData.type_def }, data.name,
                    [&](jswinrt_writer& w) { write_cppwinrt_type(w, data.type); });
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
                [&](jswinrt_writer& w) { write_cppwinrt_type(w, data.type); }, cpp_typename{ classData.type_def },
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
                    return %;)^-^", [&](jswinrt_writer& w) { write_make_return_struct(w, overload.method); });
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
        },)^-^", classData.type_def.TypeNamespace(), classData.type_def.TypeName());
        }

        writer.write("\n    };\n");
    }

    if (classData.methods.constructors.empty())
    {
        writer.write_fmt(R"^-^(
    constexpr const static_class_data data{ "%"sv, %, %, % };
}
)^-^",
            classData.name, !classData.methods.properties.empty() ? "property_data"sv : "{}"sv,
            !classData.methods.events.empty() ? "event_data"sv : "{}"sv,
            !classData.methods.functions.empty() ? "function_data"sv : "{}"sv);
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
            !classData.methods.properties.empty() ? "property_data"sv : "{}"sv,
            !classData.methods.events.empty() ? "event_data"sv : "{}"sv,
            !classData.methods.functions.empty() ? "function_data"sv : "{}"sv);
    }
}

static void write_interface_projection_data(jswinrt_writer& writer, const interface_projection_data& ifaceData)
{
    writer.write_fmt(R"^-^(
namespace jswinrt::interfaces::%
{)^-^",
        cpp_typename{ ifaceData.type_def });

    bool hasProperties = false, hasEvents = false, hasFunctions = false;
    // TODO: data

    writer.write_fmt(R"^-^(
    constexpr const static_interface_data data{ winrt::guid_of<winrt::%>(), %, %, % };
}
)^-^",
        cpp_typename{ ifaceData.type_def }, hasProperties ? "property_data"sv : "{}"sv,
        hasEvents ? "event_data"sv : "{}"sv, hasFunctions ? "function_data"sv : "{}"sv);
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
                    camel_case{ field.Name() }, field.Name(), [&](jswinrt_writer& w) { write_cppwinrt_type(w, sig.Type()); });
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
                delegateData->type_def.TypeName(), fn.param_count, fn.param_count,
                delegateData->type_def.TypeName());

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
                return %;)^-^", [&](jswinrt_writer& w) { write_make_return_struct(w, fn); });
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

            if (fn.has_return_value)
            {
                //
                writer.write_fmt(R"^-^(
            % returnValue%;)^-^",
                    [&](jswinrt_writer& w) { write_cppwinrt_type(w, fn.signature.ReturnType().Type()); },
                    [&](jswinrt_writer& w) { write_cppwinrt_type_initializer(w, fn.signature.ReturnType().Type()); });
            }

            writer.write(R"^-^(
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;)^-^");

            write_params_native_to_value(writer, fn, 4);

            writer.write_fmt(R"^-^(
                %fn.call(runtime)^-^", (fn.has_return_value || fn.has_out_params) ? "auto result = " : "");
            for (int i = 0; i < fn.param_count; ++i)
            {
                writer.write_fmt(", arg%", i);
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
                        [&](jswinrt_writer& w) { write_cppwinrt_type(w, fn.signature.ReturnType().Type()); });
                }
            }
            else if (fn.has_return_value)
            {
                writer.write_fmt("\n                returnValue = convert_value_to_native<%>(runtime, result);",
                    [&](jswinrt_writer& w) { write_cppwinrt_type(w, fn.signature.ReturnType().Type()); });
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
        FileWriter writer(settings.OutputFolder / data.file_name);
        for (auto& str : data.file_contents)
        {
            writer.Write("%", str);
        }
    }
}

void write_files(const Settings& settings, const projection_data& data)
{
    // TODO: Write the following files
    //  Projections.g.h
    //      TODO: Needed? I think we forward declare all of these necessary things in base.h/base.cpp
    //  Projections.g.cpp
    //      * Global array of root namespaces [extern const span<const static_namespace_data* const> root_namespaces]
    //      * Global array of guid -> interface data mapping [extern const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map]
    write_projections_cpp_file(settings, data);
    //  ProjectedValueConverters.g.h
    //      * projected_value_traits specializations for structs/delegates
    write_value_converters_header(settings, data);
    //  <Namespace>.g.h
    //      * Static namespace data
    //      * Static enum data
    //      * Static class data
    //      * Static interface data
    write_namespace_headers(settings, data);
    //  <Namespace>.g.cpp
    //      * projected_value_traits definitions
    write_namespace_cpp_files(settings, data);


    // Write base.h/base.cpp
    write_base_files(settings);

    // TODO: Typescript
}






#if 1
constexpr auto c_codegenNamespace = "WinRTTurboModule"sv;

struct ValueConverters
{
    std::set<std::string> Structs;
    std::set<std::string> Delegates;

    operator bool() const
    {
        return !(Structs.empty() && Delegates.empty());
    }
};

ValueConverters WriteValueConverterDeclarations(const Settings& settings, FileWriter& writer, const Namespace& node)
{
    const auto namespaceFullName = node.FullName(true /*useCppDelim*/);

    ValueConverters converters;
    for (const auto& structDef : node.Members().structs)
    {
        if (IsTypeAllowed(settings, structDef) && !IsSpecialOmittedStruct(structDef))
        {
            converters.Structs.insert(writer.WriteTemp("%"sv, structDef));
        }
    }

    for (const auto& delegateDef : node.Members().delegates)
    {
        // Generics delegates are special-cased rather than code-gen'd.
        if (IsTypeAllowed(settings, delegateDef) && winmd::reader::empty(delegateDef.GenericParam()))
        {
            converters.Delegates.insert(writer.WriteTemp("%"sv, delegateDef));
        }
    }

    if (!converters)
    {
        return converters;
    }

    writer.Write(R"(
namespace winrt::%
{
)"sv,
        namespaceFullName);

    // Forward declare the struct and delegate values because the winrt headers are not included in the generated header
    // or the pch.
    for (const auto& structFullName : converters.Structs)
    {
        const auto delim = structFullName.find_last_of(':');
        if (delim != std::string::npos)
        {
            writer.Write("    struct %;\n", std::string_view(structFullName).substr(delim + 1));
        }
    }

    for (const auto& delegateFullName : converters.Delegates)
    {
        const auto delim = delegateFullName.find_last_of(':');
        if (delim != std::string::npos)
        {
            writer.Write("    struct %;\n", std::string_view(delegateFullName).substr(delim + 1));
        }
    }

    writer.Write(R"(} // winrt::%

namespace %
{)",
        namespaceFullName, c_codegenNamespace);

    for (const auto& structFullName : converters.Structs)
    {
        writer.Write(R"(
    template<>
    jsi::Value ConvertStructToValue<%>(const std::shared_ptr<ProjectionsContext>& context, const %& value);
    template<>
    % ConvertValueToStruct<%>(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);
)",
            structFullName, structFullName, structFullName, structFullName);
    }

    for (const auto& delegateFullName : converters.Delegates)
    {
        writer.Write(R"(
    template<>
    jsi::Value ConvertDelegateToValue<%>(const std::shared_ptr<ProjectionsContext>& context, const %& value);
    template<>
    % ConvertValueToDelegate<%>(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);
)",
            delegateFullName, delegateFullName, delegateFullName, delegateFullName);
    }

    writer.Write(R"(} // %
)"sv,
        c_codegenNamespace);

    return converters;
}

void TryWriteStructConverterDefinition(FileWriter& writer, const std::set<std::string>& structs,
    const winmd::reader::TypeDef& structDef,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    const auto structFullName = writer.WriteTemp("%"sv, structDef);
    if (structs.find(structFullName) == structs.end())
    {
        return;
    }

    const bool useLowerCamelCase = UseLowerCamelCaseCodeGenForStruct(structDef);

    writer.Write(R"(
    template<>
    jsi::Value ConvertStructToValue<%>(const std::shared_ptr<ProjectionsContext>& context, const %& value)
    {
        auto& runtime = context->Runtime;
        jsi::Object result(runtime);
)",
        structFullName, structFullName);

    for (const auto& field : structDef.FieldList())
    {
        const auto name = field.Name();
        const auto jsName = Writer::ToLowerCamelCase(name);
        const std::string_view nativeName = useLowerCamelCase ? jsName : name;

        writer.Write(R"(
        result.setProperty(runtime, "%", %(context, value.%));)",
            jsName, GetNativeToValueConverter(writer, field.Signature().Type(), &specializedInterfaceReferences),
            nativeName);
    }

    writer.Write(R"(

        return jsi::Value(runtime, result);
    }

    template<>
    % ConvertValueToStruct<%>(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        % result{};

        auto& runtime = context->Runtime;
        const auto object = value.asObject(runtime);)",
        structFullName, structFullName, structFullName);

    for (const auto& field : structDef.FieldList())
    {
        const auto name = field.Name();
        const auto jsName = Writer::ToLowerCamelCase(name);
        const std::string_view nativeName = useLowerCamelCase ? jsName : name;

        writer.Write(R"(
        if (const auto fieldValue = object.getProperty(runtime, "%"); !value.isUndefined())
        {
            result.% = %(context, fieldValue);
        }
)",
            jsName, nativeName,
            GetValueToNativeConverter(
                writer, field.Signature().Type(), false /*isInout*/, &specializedInterfaceReferences));
    }

    writer.Write(R"(
        return result;
    }
)");
}

void TryWriteDelegateConverterDefinition(const Settings& settings, FileWriter& writer,
    const std::set<std::string>& delegates, const winmd::reader::TypeDef& delegateDef,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    const auto delegateFullName = writer.WriteTemp("%"sv, delegateDef);
    if (delegates.find(delegateFullName) == delegates.end())
    {
        return;
    }

    // We don't expect any overloads or really more than one method in fact.
    const auto methods = delegateDef.MethodList();
    const auto methodDef =
        std::find_if(begin(methods), end(methods), [](auto&& method) { return method.Name() == "Invoke"; });
    FAIL_FAST_IF(methodDef == winmd::reader::end(methods));

    const auto signature = methodDef.Signature();
    const auto returnType = signature.ReturnType();
    const auto paramSigs = signature.Params();
    auto paramList = methodDef.ParamList();

    if (returnType && (paramList.first != paramList.second) && (paramList.first.Sequence() == 0))
    {
        ++paramList.first;
    }

    const auto paramCount = std::distance(paramSigs.first, paramSigs.second);
    FAIL_FAST_IF(paramCount != std::distance(paramList.first, paramList.second));

    std::vector<std::string> functionOverloadCreateParams;
    functionOverloadCreateParams.reserve(2 + paramCount);
    functionOverloadCreateParams.push_back(writer.WriteTemp("&%::operator()"sv, delegateFullName));

    if (returnType)
    {
        // Like for methods, we assume that return values for delegate are ByRef so we ignore
        // 'getterReturnType.ByRef()'.
        functionOverloadCreateParams.push_back(
            "&" + GetNativeToValueConverter(writer, returnType.Type(), &specializedInterfaceReferences));
    }

    std::vector<std::string> delegateLambdaParams;
    delegateLambdaParams.reserve(paramCount);

    std::vector<std::string> delegateCallParams;
    delegateCallParams.reserve(paramCount + 1);
    delegateCallParams.emplace_back("context->Runtime"sv);

    for (uint32_t i = 0; i < paramCount; ++i)
    {
        const auto param = paramList.first[i];
        const auto paramSig = paramSigs.first[i];
        const auto paramType = paramSig.Type();

        if (param.Flags().Out())
        {
            if (settings.Verbose)
            {
                std::cout << "TODO: Skipping " << delegateFullName
                          << " as inout-params and out-params are not implemented\n";
            }
            return;
        }

        functionOverloadCreateParams.push_back(writer.WriteTemp(
            R"(&%)", GetValueToNativeConverter(writer, paramType, false /*isInout*/, &specializedInterfaceReferences)));
        delegateLambdaParams.push_back(writer.WriteTemp(R"(const %& %)", paramType, param.Name()));
        delegateCallParams.push_back(writer.WriteTemp(R"(%(context, %))",
            GetNativeToValueConverter(writer, paramType, &specializedInterfaceReferences), param.Name()));
    }

    // CallSync is used instead of winrt::impl::make_agile_delegate as the latter would only work if JS is running in an
    // ASTA while it appears that RNW uses ChakraCore and V8 using MTA so marshaling back to an MTA would likely use a
    // different thread than that of JS. Hence, if necessary the call (including value conversions) will be dispatched
    // on the JS thread while the original thread blocks. If the thread is already correct then the call can be made
    // directly.
    std::string functionCall;
    if (returnType)
    {
        const auto& returnTypeSig = returnType.Type();
        const auto returnTypeConverter =
            GetValueToNativeConverter(writer, returnTypeSig, false /*isInout*/, &specializedInterfaceReferences);
        const bool isReturnTypeValueType = IsValueType(GetProjectedValueKind(returnTypeSig));
        functionCall = writer.WriteTemp(R"(
                % value%;
                context->Invoker->CallSync(
                    [&]()
                    {
                        value = %(context, function.call(%));
                    }
                );
                return value;)"sv,
            returnTypeSig, isReturnTypeValueType ? "{}"sv : "{ nullptr }"sv, returnTypeConverter,
            Writer::BindList(","sv, delegateCallParams));
    }
    else
    {
        functionCall = writer.WriteTemp(R"(
                context->Invoker->CallSync(
                    [&]()
                    {
                        function.call(%);
                    }
                );)"sv,
            Writer::BindList(", "sv, delegateCallParams));
    }

    // TODO: Is there any way to hold the runtime safely in the lambda?
    writer.Write(R"(
    template<>
    jsi::Value ConvertDelegateToValue<%>(const std::shared_ptr<ProjectionsContext>& context, const %& value)
    {
        static auto s_function = ProjectedFunction::Create("invoke"sv, ProjectedFunctionOverload::Create<%>(%));
        return jsi::Value(context->Runtime, s_function->GetFunction<%>(value, context));
    }

    template<>
    % ConvertValueToDelegate<%>(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return %(
            [context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](%)
            {%
            }
        );
    }
)",
        delegateFullName, delegateFullName, delegateFullName, Writer::BindList(", "sv, functionOverloadCreateParams),
        delegateFullName, delegateFullName, delegateFullName, delegateFullName,
        Writer::BindList(", "sv, delegateLambdaParams), functionCall);
}

void WriteValueConverterDefintions(const Settings& settings, FileWriter& writer, const Namespace& node,
    const ValueConverters& valueConverters,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    const auto& structs = node.Members().structs;
    const auto& delegateDefs = node.Members().delegates;

    writer.Write(R"(
namespace %
{)"sv,
        c_codegenNamespace);

    for (const auto& structDef : structs)
    {
        TryWriteStructConverterDefinition(writer, valueConverters.Structs, structDef, specializedInterfaceReferences);
    }

    for (const auto& delegateDef : delegateDefs)
    {
        TryWriteDelegateConverterDefinition(
            settings, writer, valueConverters.Delegates, delegateDef, specializedInterfaceReferences);
    }

    writer.Write(R"(} // %
)",
        c_codegenNamespace);
}

std::optional<std::string> TryWriteEnum(const Settings& settings, FileWriter& writer,
    const std::string namespaceFullName, const winmd::reader::TypeDef& enumDef)
{
    if (!IsTypeAllowed(settings, enumDef))
    {
        return {};
    }

    const auto enumName = enumDef.TypeName();
    writer.Write(R"(
    std::unordered_map<std::string_view, int64_t> %_InitEnum()
    {
        return
        {
)",
        enumName);

    for (const auto& field : enumDef.FieldList())
    {
        if (auto constant = field.Constant())
        {
#if 0
            writer.Write(R"(            { "%"sv, % },
)"sv, Writer::ToLowerCamelCase(field.Name()), *constant);
#else
            writer.Write(R"(            { "%"sv, static_cast<int64_t>(winrt::%::%::%) },
)"sv,
                Writer::ToLowerCamelCase(field.Name()), namespaceFullName, enumName, field.Name());
#endif
        }
    }

    writer.Write(R"(        };
    }
)"sv);

    return writer.WriteTemp(R"(ProjectedEnum::Create("%"sv, &::%::%::%_InitEnum))"sv, enumName, c_codegenNamespace,
        namespaceFullName, enumName);
}

std::vector<std::string> WriteEnums(const Settings& settings, FileWriter& writer, const Namespace& node)
{
    const auto enumDefs = node.Members().enums;
    const auto namespaceFullName = node.FullName(true /*useCppDelim*/);

    std::vector<std::string> projectedEnumCreates;
    projectedEnumCreates.reserve(enumDefs.size());

    for (const auto& enumDef : enumDefs)
    {
        if (auto projectedEnumCreate = TryWriteEnum(settings, writer, namespaceFullName, enumDef))
        {
            projectedEnumCreates.push_back(std::move(*projectedEnumCreate));
        }
    }

    return projectedEnumCreates;
}

std::vector<std::string> WriteProperties(const Settings& settings, FileWriter& writer,
    const std::string interfaceFullName, const std::map<std::string_view, PropertyInfo>& properties,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    std::vector<std::string> propertyCreates;
    propertyCreates.reserve(properties.size());

    for (const auto property : properties)
    {
        const auto& propertyInfo = property.second;
        if (!propertyInfo.GetterDef)
        {
            continue;
        }

        if (!IsMethodAllowed(settings, propertyInfo.GetterDef))
        {
            continue;
        }

        const auto getterSig = propertyInfo.GetterDef.Signature();
        const auto getterReturnType = getterSig.ReturnType();
        FAIL_FAST_IF(!getterReturnType);

        const auto getterReturnTypeSig = getterReturnType.Type();

        const auto propertyFullNameWinRT = writer.WriteTemp("%::%", interfaceFullName, property.first);
        if (propertyInfo.SetterDef && IsMethodAllowed(settings, propertyInfo.SetterDef))
        {
            const auto setterSig = propertyInfo.SetterDef.Signature();
            const auto setterParamSigs = setterSig.Params();
            FAIL_FAST_IF(std::distance(setterParamSigs.first, setterParamSigs.second) != 1);

            // Like for methods, we assume that return values for properties are ByRef so we ignore
            // 'getterReturnType.ByRef()'.
            propertyCreates.push_back(
                writer.WriteTemp(R"(ProjectedProperty::Create<winrt::%>("%"sv, &winrt::%, &%, &winrt::%, &%))",
                    interfaceFullName, Writer::ToLowerCamelCase(property.first), propertyFullNameWinRT,
                    GetNativeToValueConverter(writer, getterReturnTypeSig, &specializedInterfaceReferences),
                    propertyFullNameWinRT,
                    GetValueToNativeConverter(
                        writer, setterParamSigs.first->Type(), false /*isInout*/, &specializedInterfaceReferences)));
        }
        else
        {
            // Like for methods, we assume that return values for properties are ByRef so we ignore
            // 'getterReturnType.ByRef()'.
            propertyCreates.push_back(writer.WriteTemp(R"(ProjectedProperty::Create<winrt::%>("%"sv, &winrt::%, &%))",
                interfaceFullName, Writer::ToLowerCamelCase(property.first), propertyFullNameWinRT,
                GetNativeToValueConverter(writer, getterReturnTypeSig, &specializedInterfaceReferences)));
        }
    }

    return propertyCreates;
}

std::optional<std::string> TryWriteMethodOverload(const Settings& settings, FileWriter& writer,
    const std::string interfaceFullName, const std::string_view& methodName, const MethodOverloadInfo& methodOverload,
    const bool useConstrainFunctionOverload,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    if (!IsMethodAllowed(settings, methodOverload.Def))
    {
        return {};
    }

    if (useConstrainFunctionOverload)
    {
        if (settings.Verbose)
        {
            std::cout << "TODO: Skipping an overload of " << interfaceFullName << "::" << methodName << " with arity "
                      << methodOverload.InParamCount << " and " << methodOverload.OutParamCount
                      << " out-params since code-gen for ConstrainFunctionOverload is not implemented yet\n";
        }
        return {};
    }

    const auto signature = methodOverload.Def.Signature();
    const auto returnType = signature.ReturnType();
    const auto paramSigs = signature.Params();
    auto paramList = methodOverload.Def.ParamList();

    if (returnType && (paramList.first != paramList.second) && (paramList.first.Sequence() == 0))
    {
        ++paramList.first;
    }

    const auto paramCount = std::distance(paramSigs.first, paramSigs.second);
    FAIL_FAST_IF(paramCount != std::distance(paramList.first, paramList.second));

    std::vector<std::string> createParams;
    createParams.reserve(2 + paramCount);

    createParams.push_back(writer.WriteTemp("&winrt::impl::consume_t<winrt::%>::%", interfaceFullName, methodName));
    if (returnType)
    {
        const auto returnTypeSig = returnType.Type();

        // Since this is a return value, if it is an array, we should ignore returnType.ByRef() which will be false and
        // assume it is ByRef.
        createParams.push_back("&" + GetNativeToValueConverter(writer, returnTypeSig, &specializedInterfaceReferences));
    }

    const bool hasNoOutParams = methodOverload.OutParamCount == 0;
    for (uint32_t i = 0; i < paramCount; ++i)
    {
        const auto param = paramList.first[i];
        const auto paramSig = paramSigs.first[i];
        const auto paramType = paramSig.Type();

        // "hasNoOutParams" is set to true if something is an inout param rather than a true out-param.
        const bool isInout = param.Flags().Out() && paramType.is_szarray() && !paramSig.ByRef();

        if (hasNoOutParams)
        {
            createParams.push_back(writer.WriteTemp(
                R"(&%)", GetValueToNativeConverter(writer, paramType, isInout, &specializedInterfaceReferences)));
        }
        else if (param.Flags().In() || isInout)
        {
            createParams.push_back(writer.WriteTemp(R"(ProjectedFunctionParamInfo(ProjectedFunctionInParam, &%))"sv,
                GetValueToNativeConverter(writer, paramType, isInout, &specializedInterfaceReferences)));
        }
        else
        {
            FAIL_FAST_IF(!param.Flags().Out() || hasNoOutParams);
            FAIL_FAST_IF(paramType.is_szarray() && !paramSig.ByRef());

            createParams.push_back(
                writer.WriteTemp(R"(ProjectedFunctionParamInfo(ProjectedFunctionOutParam, &%, "%"))"sv,
                    GetNativeToValueConverter(writer, paramType, &specializedInterfaceReferences),
                    Writer::ToLowerCamelCase(param.Name())));
        }
    }

    return writer.WriteTemp(
        R"(ProjectedFunctionOverload::Create<winrt::%>(%))", interfaceFullName, Writer::BindList(", "sv, createParams));
}

std::optional<std::string> TryWriteMethod(const Settings& settings, FileWriter& writer,
    const std::string_view interfaceName, const std::string interfaceFullName, const std::string_view& methodName,
    const std::map<uint32_t, std::vector<MethodOverloadInfo>>& methodOverloads,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    // Determine which overloads needs to have the unfortunate ConstrainFunctionOverload to disambiguate.
    std::map<uint32_t, bool> isTotalParamCountUnique;
    for (const auto& methodOverloadArities : methodOverloads)
    {
        for (const auto& methodOverload : methodOverloadArities.second)
        {
            const auto totalParamCount = methodOverload.InParamCount + methodOverload.OutParamCount;
            auto it = isTotalParamCountUnique.find(totalParamCount);
            if (it == isTotalParamCountUnique.end())
            {
                isTotalParamCountUnique[totalParamCount] = true;
            }
            else
            {
                isTotalParamCountUnique[totalParamCount] = false;
            }
        }
    }

    std::vector<std::string> projectedMethodOverloadCreates;
    projectedMethodOverloadCreates.reserve(methodOverloads.size());

    for (const auto& methodOverloadArities : methodOverloads)
    {
        FAIL_FAST_IF(methodOverloadArities.second.empty());

        // If one of these is the default, make sure to use it else fix the first.
        uint32_t defaultIndex = 0;
        for (uint32_t i = 0; i < methodOverloadArities.second.size(); ++i)
        {
            if (methodOverloadArities.second[i].IsDefaultOverload)
            {
                defaultIndex = i;
                break;
            }
        }

        // JS cannot overload by types, only by the number of in-params so take the first method with that number of
        // params found and don't codegen for the other(s). Note that if there are overloads with the same arity on
        // different interfaces, it will be resolved by picking the first it finds at runtime. In the public SDK,
        // probematic overloads include:
        //  Windows::Devices::Enumeration::DeviceInformation::FindAllAsync
        //  Windows::Globalization::NumberFormatting::INumberFormatter::Format
        //  Windows::Networking::Sockets::IStreamSocket::ConnectAsync

        if (settings.Verbose)
        {
            for (uint32_t i = 0; i < methodOverloadArities.second.size(); ++i)
            {
                if (i != defaultIndex)
                {
                    std::cout << "INFO: Skipping an ambiguous overload of " << interfaceFullName << "::" << methodName
                              << " with arity " << methodOverloadArities.first << " and "
                              << methodOverloadArities.second[i].OutParamCount << " out-params\n";
                }
            }
        }

        const auto& methodOverload = methodOverloadArities.second[defaultIndex];
        if (auto projectedMethodOverloadCreate = TryWriteMethodOverload(settings, writer, interfaceFullName, methodName,
                methodOverloadArities.second[defaultIndex],
                !isTotalParamCountUnique[methodOverload.InParamCount + methodOverload.OutParamCount],
                specializedInterfaceReferences))
        {
            projectedMethodOverloadCreates.push_back(std::move(*projectedMethodOverloadCreate));
        }
    }

    if (projectedMethodOverloadCreates.empty())
    {
        return {};
    }

    if (projectedMethodOverloadCreates.size() == 1)
    {
        return writer.WriteTemp(R"(ProjectedFunction::Create("%"sv, %))", Writer::ToLowerCamelCase(methodName),
            projectedMethodOverloadCreates[0]);
    }

    writer.Write(R"(
    std::vector<std::shared_ptr<IProjectedFunctionOverloadBase>> %_%_InitMethod()
    {
        return
        {
)"sv,
        interfaceName, methodName);

    for (const auto& projectedMethodOverloadCreate : projectedMethodOverloadCreates)
    {
        writer.Write("            %,\n", projectedMethodOverloadCreate);
    }
    writer.Write(R"(        };
    }
)"sv);

    return writer.WriteTemp(R"(ProjectedFunction::Create("%"sv, &::%::%_%_InitMethod))",
        Writer::ToLowerCamelCase(methodName), c_codegenNamespace, interfaceFullName, methodName);
}

std::vector<std::string> WriteMethods(const Settings& settings, FileWriter& writer,
    const std::string_view interfaceName, const std::string interfaceFullName,
    const std::map<std::string_view, std::map<uint32_t, std::vector<MethodOverloadInfo>>>& methods,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    std::vector<std::string> projectedMethodCreates;
    projectedMethodCreates.reserve(methods.size());

    for (const auto& method : methods)
    {
        if (auto projectedMethodCreate = TryWriteMethod(settings, writer, interfaceName, interfaceFullName,
                method.first, method.second, specializedInterfaceReferences))
        {
            projectedMethodCreates.push_back(std::move(*projectedMethodCreate));
        }
    }
    return projectedMethodCreates;
}

std::vector<std::string> WriteEvents(const Settings& settings, FileWriter& writer, const std::string_view interfaceName,
    const std::string interfaceFullName, const std::map<std::string_view, EventInfo>& events,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    std::vector<std::string> eventCreates;
    eventCreates.reserve(events.size());

    for (const auto event : events)
    {
        const auto& eventInfo = event.second;
        if (!eventInfo.AddDef || !eventInfo.RemoveDef || !IsMethodAllowed(settings, eventInfo.AddDef) ||
            !IsMethodAllowed(settings, eventInfo.RemoveDef))
        {
            continue;
        }

        const auto addSig = eventInfo.AddDef.Signature();
        FAIL_FAST_IF(!addSig.ReturnType());

        auto addParamSigs = addSig.Params();
        FAIL_FAST_IF(std::distance(addParamSigs.first, addParamSigs.second) != 1);

        const auto delegateTypeSig = addParamSigs.first->Type();

        const auto removeSig = eventInfo.RemoveDef.Signature();
        FAIL_FAST_IF(removeSig.ReturnType());

        auto removeParamSigs = removeSig.Params();
        FAIL_FAST_IF(std::distance(removeParamSigs.first, removeParamSigs.second) != 1);

        const auto eventFullName = writer.WriteTemp("%::%"sv, interfaceFullName, event.first);

        eventCreates.push_back(writer.WriteTemp(
            R"(ProjectedEvent::Create<winrt::%>("%"sv, &winrt::%, &winrt::%, &%))"sv, interfaceFullName,
            Writer::ToLowerCase(event.first), eventFullName, eventFullName,
            GetValueToNativeConverter(writer, delegateTypeSig, false /*isInout*/, &specializedInterfaceReferences)));
    }

    return eventCreates;
}

std::optional<std::string> TryWriteInterface(const Settings& settings, FileWriter& writer,
    const std::string& namespaceFullName, const winmd::reader::TypeDef& interfaceDef,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    if (!IsTypeAllowed(settings, interfaceDef))
    {
        return {};
    }

    const auto interfaceInfo = GetInterfaceInfo(interfaceDef);
    const auto interfaceName = interfaceDef.TypeName();
    const auto interfaceFullName = writer.WriteTemp("%::%", namespaceFullName, interfaceName);

    // Omit the Windows.Foundation generic-param types like IAsyncOperation<T> and IVectorView<T> as we need to include
    // the specialized versions in the interface tables in order to be usable since they each have a unique IID per
    // specialization.
    if (!winmd::reader::empty(interfaceDef.GenericParam()))
    {
        return {};
    }

    const auto projectedPropertyCreates =
        WriteProperties(settings, writer, interfaceFullName, interfaceInfo.Properties, specializedInterfaceReferences);
    const auto projectedMethodCreates = WriteMethods(
        settings, writer, interfaceName, interfaceFullName, interfaceInfo.Methods, specializedInterfaceReferences);
    const auto projectedEventCreates = WriteEvents(
        settings, writer, interfaceName, interfaceFullName, interfaceInfo.Events, specializedInterfaceReferences);

    if (projectedPropertyCreates.empty() && projectedMethodCreates.empty() && projectedEventCreates.empty())
    {
        return {};
    }

    writer.Write(R"(
    ProjectedInterfaceData %_InitInterface()
    {
        return
        {
            {
)"sv,
        interfaceName);

    if (projectedPropertyCreates.empty())
    {
        writer.Write("                // No Properties\n");
    }
    else
    {
        for (const auto propertyCreate : projectedPropertyCreates)
        {
            writer.Write("                %,\n", propertyCreate);
        }
    }

    writer.Write(R"(            },
            {
)"sv);

    if (projectedMethodCreates.empty())
    {
        writer.Write("                // No Methods\n");
    }
    else
    {
        for (const auto methodCreate : projectedMethodCreates)
        {
            writer.Write("                %,\n", methodCreate);
        }
    }

    writer.Write(R"(            },
            {
)"sv);

    if (projectedEventCreates.empty())
    {
        writer.Write("                // No Events\n");
    }
    else
    {
        for (const auto eventCreate : projectedEventCreates)
        {
            writer.Write("                %,\n", eventCreate);
        }
    }

    writer.Write(R"(            }
        };
    }
)"sv);

    return writer.WriteTemp(R"(ProjectedInterface::Create<winrt::%::%>(&::%::%::%_InitInterface))", namespaceFullName,
        interfaceName, c_codegenNamespace, namespaceFullName, interfaceName);
}

std::vector<std::string> WriteInterfaces(const Settings& settings, FileWriter& writer, const Namespace& node,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    const auto interfaceDefs = node.Members().interfaces;
    const auto namespaceFullName = node.FullName(true /*useCppDelim*/);

    std::vector<std::string> projectedInterfaceCreates;
    projectedInterfaceCreates.reserve(interfaceDefs.size());

    for (const auto& interfaceDef : interfaceDefs)
    {
        if (auto projectedInterfaceCreate =
                TryWriteInterface(settings, writer, namespaceFullName, interfaceDef, specializedInterfaceReferences))
        {
            projectedInterfaceCreates.push_back(std::move(*projectedInterfaceCreate));
        }
    }

    return projectedInterfaceCreates;
}

void SpecializeIterator(
    FileWriter& writer, const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IIterator<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIIteratorInterface<%, %>)"sv,
        valueTypeSig, valueTypeSig,
        GetNativeToValueConverter(writer, valueTypeSig, nullptr /*specializedInterfaceReferences*/)));
}

void SpecializeIterable(
    FileWriter& writer, const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IIterable<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIIterableInterface<%>)"sv,
        valueTypeSig, valueTypeSig));
    SpecializeIterator(writer, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeVectorView(
    FileWriter& writer, const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IVectorView<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIVectorViewInterface<%, %, %>)"sv,
        valueTypeSig, valueTypeSig,
        GetNativeToValueConverter(writer, valueTypeSig, nullptr /*specializedInterfaceReferences*/),
        GetValueToNativeConverter(
            writer, valueTypeSig, false /*isInout*/, nullptr /*specializedInterfaceReferences*/)));
    SpecializeIterable(writer, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeVector(
    FileWriter& writer, const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IVector<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIVectorInterface<%, %, %>)"sv,
        valueTypeSig, valueTypeSig,
        GetNativeToValueConverter(writer, valueTypeSig, nullptr /*specializedInterfaceReferences*/),
        GetValueToNativeConverter(
            writer, valueTypeSig, false /*isInout*/, nullptr /*specializedInterfaceReferences*/)));
    SpecializeVectorView(writer, valueTypeSig, projectedInterfaceCreates);
    SpecializeIterable(writer, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeObservableVector(
    FileWriter& writer, const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IObservableVector<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIObservableVectorInterface<%>)"sv,
        valueTypeSig, valueTypeSig));
    SpecializeVector(writer, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeKeyValuePair(FileWriter& writer, const winmd::reader::TypeSig& keyTypeSig,
    const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IKeyValuePair<%, %>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIKeyValuePairInterface<%, %, %, %>)"sv,
        keyTypeSig, valueTypeSig, keyTypeSig, valueTypeSig,
        GetNativeToValueConverter(writer, keyTypeSig, nullptr /*specializedInterfaceReferences*/),
        GetNativeToValueConverter(writer, valueTypeSig, nullptr /*specializedInterfaceReferences*/)));
}

void SpecializeKeyValuePairIterator(FileWriter& writer, const winmd::reader::TypeSig& keyTypeSig,
    const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    const auto keyValuePairInterface =
        writer.WriteTemp("winrt::Windows::Foundation::Collections::IKeyValuePair<%, %>"sv, keyTypeSig, valueTypeSig);
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IIterator<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIIteratorInterface<%, ConvertInterfaceToValue<%>>)"sv,
        keyValuePairInterface, keyValuePairInterface, keyValuePairInterface));
    SpecializeKeyValuePair(writer, keyTypeSig, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeKeyValuePairIterable(FileWriter& writer, const winmd::reader::TypeSig& keyTypeSig,
    const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    const auto keyValuePairInterface =
        writer.WriteTemp("winrt::Windows::Foundation::Collections::IKeyValuePair<%, %>"sv, keyTypeSig, valueTypeSig);
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IIterable<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIIterableInterface<%>)"sv,
        keyValuePairInterface, keyValuePairInterface));
    SpecializeKeyValuePairIterator(writer, keyTypeSig, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeMap(FileWriter& writer, const winmd::reader::TypeSig& keyTypeSig,
    const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IMap<%, %>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIMapInterface<%, %, %, %, %>)"sv,
        keyTypeSig, valueTypeSig, keyTypeSig, valueTypeSig,
        GetValueToNativeConverter(writer, keyTypeSig, false /*isInout*/, nullptr /*specializedInterfaceReferences*/),
        GetNativeToValueConverter(writer, valueTypeSig, nullptr /*specializedInterfaceReferences*/),
        GetValueToNativeConverter(
            writer, valueTypeSig, false /*isInout*/, nullptr /*specializedInterfaceReferences*/)));
    SpecializeKeyValuePairIterable(writer, keyTypeSig, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeMapView(FileWriter& writer, const winmd::reader::TypeSig& keyTypeSig,
    const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IMapView<%, %>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIMapViewInterface<%, %, %, %>)"sv,
        keyTypeSig, valueTypeSig, keyTypeSig, valueTypeSig,
        GetValueToNativeConverter(writer, keyTypeSig, false /*isInout*/, nullptr /*specializedInterfaceReferences*/),
        GetNativeToValueConverter(writer, valueTypeSig, nullptr /*specializedInterfaceReferences*/)));
    SpecializeKeyValuePairIterable(writer, keyTypeSig, valueTypeSig, projectedInterfaceCreates);
}

void SpecializeMapChangedEventArgs(
    FileWriter& writer, const winmd::reader::TypeSig& keyTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<%>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIMapChangedEventArgsInterface<%, %>)"sv,
        keyTypeSig, keyTypeSig,
        GetNativeToValueConverter(writer, keyTypeSig, nullptr /*specializedInterfaceReferences*/)));
}

void SpecializeObservableMap(FileWriter& writer, const winmd::reader::TypeSig& keyTypeSig,
    const winmd::reader::TypeSig& valueTypeSig, std::set<std::string>& projectedInterfaceCreates)
{
    projectedInterfaceCreates.insert(writer.WriteTemp(
        "ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IObservableMap<%, %>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIObservableMapInterface<%, %>)"sv,
        keyTypeSig, valueTypeSig, keyTypeSig, valueTypeSig));
    SpecializeMap(writer, keyTypeSig, valueTypeSig, projectedInterfaceCreates);
    SpecializeMapChangedEventArgs(writer, keyTypeSig, projectedInterfaceCreates);
}

std::set<std::string> SpecializeInterfaces(FileWriter& writer,
    const std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    // Interface dependency relations:
    //  IMapView<K, T> -> IIterable<IKeyValuePair<K, T>>
    //  IMap<K, T> -> IMapView<K, T>, IIterable<IKeyValuePair<K, T>>
    //  IObservableMap<K, T> -> IMap<K, T>, IMapChangedEventArgs<K>
    //  IObservableVector<T> -> IVector<T>
    //  IVector<T> -> IIterable<T>, IVectorView<T>
    //  IVectorView<T> -> IIterable<T>
    //  IIterable<T> -> IIterator<T>

    // Specialized format looks like.
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IIterable<T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIIterableInterface<T>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IIterator<T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIIteratorInterface<T,CTV>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IKeyValuePair<K,T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIKeyValuePairInterface<K,T,CKV,CTV>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IMap<K,T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIMapInterface<K,T,CVK,CTV,CVT>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIMapChangedEventArgsInterface<K,CKV>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IMapView<K,T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIMapViewInterface<K,T,CVK,CTV>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IObservableMap<K,T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIObservableMapInterface<K,T>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IObservableVector<T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIObservableVectorInterface<T>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IVector<T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIVectorInterface<T,CTV,CVT>);
    //  ProjectedInterface::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>(&::WinRTTurboModule::Windows::Foundation::Collections::InitIVectorViewInterface<T,CTV,CVT>);

    // PERF: It would likely be more performant to remove redundant types (e.g. IIterable<T> if we find a IVector<T>)
    // before specializing.
    //       And it would also like be more performant to increase re-use of type and converter string conversions.
    std::set<std::string> projectedInterfaceCreates;
    for (const auto& reference : specializedInterfaceReferences)
    {
        if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IIterable<"sv))
        {
            SpecializeIterable(writer, reference.second[0], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IIterator<"sv))
        {
            SpecializeIterator(writer, reference.second[0], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IKeyValuePair<"sv))
        {
            SpecializeKeyValuePair(writer, reference.second[0], reference.second[1], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IMap<"sv))
        {
            SpecializeMap(writer, reference.second[0], reference.second[1], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IMapChangedEventArgs<"sv))
        {
            SpecializeMapChangedEventArgs(writer, reference.second[0], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IMapView<"sv))
        {
            SpecializeMapView(writer, reference.second[0], reference.second[1], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IObservableMap<"sv))
        {
            SpecializeObservableMap(writer, reference.second[0], reference.second[1], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IObservableVector<"sv))
        {
            SpecializeObservableVector(writer, reference.second[0], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IVector<"sv))
        {
            SpecializeVector(writer, reference.second[0], projectedInterfaceCreates);
        }
        else if (StartsWith(reference.first, "winrt::Windows::Foundation::Collections::IVectorView<"sv))
        {
            SpecializeVectorView(writer, reference.second[0], projectedInterfaceCreates);
        }

        // IAsyncActionWithProgress, IAsyncOperation, IAsyncOperationWithProgress are not specialized since we already
        // generate a wrapper to make them act like special promises so there wouldn't be any more functionality made
        // available by doing so. We generate converters for IReference and IReferenceArray so they also do not need to
        // be specialized.
    }

    // std::set<std::string>
    return projectedInterfaceCreates;
}

std::optional<std::string> TryWriteRuntimeClass(const Settings& settings, FileWriter& writer,
    const std::string& namespaceFullName, const winmd::reader::TypeDef& classDef,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    if (!IsTypeAllowed(settings, classDef, true /*isClass*/))
    {
        return {};
    }

    // Because there is no code-generation for the runtime class (instance) itself and only interfaces, we need to make
    // sure that if classes implement generic interfaces that the specialized implementations are included in the
    // interface table. While the 'specializedInterfaceReferences' otherwise accumulates specialized interfaces
    // accumulated when generating value converters, if a specialized interface is implemented by a runtime class and
    // not otherwise referenced by non generic interfaces, it might not otherwise found. e.g IMapView<winrt::hstring,
    // Windows::ApplicationModel::Resources::Core::NamedResource>
    for (auto& interfaceImpl : classDef.InterfaceImpl())
    {
        const auto type = interfaceImpl.Interface();
        if (type.type() == winmd::reader::TypeDefOrRef::TypeSpec)
        {
            const auto genericTypeSig = type.TypeSpec().Signature().GenericTypeInst();
            auto specializedInterfaceName = writer.WriteTemp("%", genericTypeSig);
            if (specializedInterfaceReferences.find(specializedInterfaceName) == specializedInterfaceReferences.end())
            {
                std::vector<winmd::reader::TypeSig> genericParamTypeSigs;
                genericParamTypeSigs.reserve(2);

                const auto genericArgs = genericTypeSig.GenericArgs();
                genericParamTypeSigs.insert(genericParamTypeSigs.begin(), genericArgs.first, genericArgs.second);
                specializedInterfaceReferences.emplace(
                    std::move(specializedInterfaceName), std::move(genericParamTypeSigs));
            }
        }
    }

    const auto factoryInterfaces = GetActivationFactoryInterfaces(classDef);

    std::vector<std::string> activationFactoryInterfaceIds;
    activationFactoryInterfaceIds.reserve(activationFactoryInterfaceIds.size());

    bool hasStatics = false;
    for (const auto& factoryInterface : factoryInterfaces)
    {
        if (factoryInterface.IsActivatable)
        {
            if (factoryInterface.TypeDef)
            {
                activationFactoryInterfaceIds.push_back(
                    writer.WriteTemp(R"(winrt::guid_of<%>())", factoryInterface.TypeDef));
            }
            else
            {
                activationFactoryInterfaceIds.emplace_back("__uuidof(::IActivationFactory)"sv);
            }
        }
        else if (factoryInterface.IsStatics)
        {
            hasStatics = true;
        }
    }

    if (activationFactoryInterfaceIds.empty() && !hasStatics)
    {
        return {};
    }

    const auto className = classDef.TypeName();
    return writer.WriteTemp(R"(ProjectedActivationFactory::Create<winrt::%::%>("%"sv, context, { % }))",
        namespaceFullName, className, className, Writer::BindList(", "sv, activationFactoryInterfaceIds));
}

std::vector<std::string> WriteRuntimeClasses(const Settings& settings, FileWriter& writer, const Namespace& node,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>& specializedInterfaceReferences)
{
    const auto classDefs = node.Members().classes;
    const auto namespaceFullName = node.FullName(true /*useCppDelim*/);

    std::vector<std::string> projectedActivationFactoryCreates;
    projectedActivationFactoryCreates.reserve(classDefs.size());

    for (const auto& classDef : classDefs)
    {
        if (auto projectedActivationFactoryCreate =
                TryWriteRuntimeClass(settings, writer, namespaceFullName, classDef, specializedInterfaceReferences))
        {
            projectedActivationFactoryCreates.push_back(std::move(*projectedActivationFactoryCreate));
        }
    }

    return projectedActivationFactoryCreates;
}

void WriteNamespace(const Settings& settings, FileWriter& writer, const Namespace& node,
    const ValueConverters& valueConverters, std::set<std::string>& specializedInterfaceCreates, uint32_t& numInterfaces,
    bool& includeCppWinRtHeader)
{
    numInterfaces = 0;
    includeCppWinRtHeader = false;

    std::map<std::string, std::vector<winmd::reader::TypeSig>> specializedInterfaceReferences;
    if (valueConverters)
    {
        WriteValueConverterDefintions(settings, writer, node, valueConverters, specializedInterfaceReferences);
    }

    const auto namespaceFullName = node.FullName(true /*useCppDelim*/);
    writer.Write(R"(
namespace %::%
{)"sv,
        c_codegenNamespace, namespaceFullName);

    const auto projectedEnumCreates = WriteEnums(settings, writer, node);
    auto projectedInterfaceCreates = WriteInterfaces(settings, writer, node, specializedInterfaceReferences);
    const auto projectedFactoryCreates = WriteRuntimeClasses(settings, writer, node, specializedInterfaceReferences);

    const auto specializedInterfaceCreateStartIndex = projectedInterfaceCreates.size();
    if (!specializedInterfaceReferences.empty())
    {
        // We generate the create statements for all the interfaces referenced in this namespace and then determine
        // which have already been included by a different namespaces. The intent is to collocate specialized interfaces
        // with their use and to avoid collecting them all and generating one huge GetInterfaces() in
        // Windows.Foundation.Collections.g.cpp as that would create a massive object file and have slow compilation
        // speeds (single-threaded compilation per translation unit) while using a lot of memory.
        auto localSpecializedInterfaceCreates = SpecializeInterfaces(writer, specializedInterfaceReferences);
        projectedInterfaceCreates.resize(
            specializedInterfaceCreateStartIndex + localSpecializedInterfaceCreates.size());
        projectedInterfaceCreates.resize(std::distance(projectedInterfaceCreates.begin(),
            std::set_difference(std::make_move_iterator(localSpecializedInterfaceCreates.begin()),
                std::make_move_iterator(localSpecializedInterfaceCreates.end()), specializedInterfaceCreates.cbegin(),
                specializedInterfaceCreates.cend(),
                projectedInterfaceCreates.begin() + specializedInterfaceCreateStartIndex)));
    }

    includeCppWinRtHeader = valueConverters || !projectedEnumCreates.empty() || !projectedInterfaceCreates.empty() ||
                            !projectedFactoryCreates.empty();

    writer.Write(R"(
    std::vector<std::shared_ptr<IProjectedValueProvider>> InitNamespace(const std::shared_ptr<ProjectionsContext>& context)
    {
        return
        {
)"sv);

    for (const auto& projectedEnumCreate : projectedEnumCreates)
    {
        writer.Write(R"(            %,
)"sv,
            projectedEnumCreate);
    }

    for (const auto& projectedFactoryCreate : projectedFactoryCreates)
    {
        writer.Write(R"(            %,
)"sv,
            projectedFactoryCreate);
    }

    // Namespaces are enumerated in post-order so the child namespace is already defined. It is guaranteed to be emitted
    // even if it ends up empty.
    for (const auto& child : node.Children())
    {
        const auto& childNode = *child.second;
        writer.Write(R"(            ProjectedNamespace::Create("%"sv, context, &::%::%::InitNamespace),
)"sv,
            childNode.Name(), c_codegenNamespace, childNode.FullName(true /*useCppDelim*/));
    }

    writer.Write(R"(        };
    }
)"sv);

    if (!projectedInterfaceCreates.empty())
    {
        writer.Write(R"(
    std::vector<std::shared_ptr<ProjectedInterface>> GetInterfaces()
    {
        return
        {
)"sv);

        for (const auto& projectedInterface : projectedInterfaceCreates)
        {
            writer.Write(R"(            %,
)",
                projectedInterface);
        }

        numInterfaces = static_cast<uint32_t>(projectedInterfaceCreates.size());

        writer.Write(R"(        };
    }
)"sv);
    }

    if (specializedInterfaceCreateStartIndex != projectedInterfaceCreates.size())
    {
        projectedInterfaceCreates.insert(projectedInterfaceCreates.end(),
            std::make_move_iterator(projectedInterfaceCreates.begin() + specializedInterfaceCreateStartIndex),
            std::make_move_iterator(projectedInterfaceCreates.end()));
    }

    writer.Write(R"(}
)"sv);
}

void WriteProjectionsGeneratedH(
    const Settings& settings, const std::map<std::string_view, std::shared_ptr<Namespace>>& roots)
{
    FileWriter writer(settings.OutputFolder / "Projections.g.h");
    writer.Write("#pragma once\n\n");
    writer.WriteInclude("base.h"sv);

    writer.Write(R"(
namespace %
{
    std::vector<std::shared_ptr<IProjectedValueProvider>> GetProjectedNamespaces(const std::shared_ptr<ProjectionsContext>& context);
    std::vector<std::shared_ptr<ProjectedInterface>> GetProjectedInterfaces();
} // %
)"sv,
        c_codegenNamespace, c_codegenNamespace);
}

void WriteTypescriptDefinitions(
    const Settings& settings, const std::map<std::string_view, std::shared_ptr<Namespace>>& roots)
{
    if (settings.TypescriptOutputFolder.empty())
    {
        return;
    }
    std::filesystem::remove_all(settings.TypescriptOutputFolder);
    std::filesystem::create_directory(settings.TypescriptOutputFolder);
    Namespace::EnumerateAll(roots, [&](const Namespace& node) {
        TextWriter textWriter;
        TypescriptWriter typescriptWriter(settings);
        typescriptWriter.Write(node, textWriter);
        textWriter.FlushToFile(settings.TypescriptOutputFolder / (node.FullName() + ".d.ts"));
        return true;
    });
}

std::map<std::string, ValueConverters> WriteProjectedValueConvertersGeneratedH(
    const Settings& settings, const std::map<std::string_view, std::shared_ptr<Namespace>>& roots)
{
    FileWriter writer(settings.OutputFolder / "ProjectedValueConverters.g.h");

    writer.Write("#pragma once\n\n");
    writer.WriteInclude("base.h"sv);
    writer.WriteNewLine();

    std::map<std::string, ValueConverters> valueConverters;
    Namespace::EnumerateAll(roots, [&](const Namespace& node) {
        valueConverters.emplace(
            node.FullName(true /*usCppDelim*/), WriteValueConverterDeclarations(settings, writer, node));
        return true;
    });
    return valueConverters;
}

void WriteNamespaceGenerateCppH(const Settings& settings, const Namespace& node,
    const std::map<std::string, ValueConverters> valueConverters, std::set<std::string>& specializedInterfaceCreates,
    std::map<std::string, uint32_t>& namespaceInterfaceCounts)
{
    const auto namespaceFullName = node.FullName(true /*useCppDelim*/);
    const auto namespaceFileName = node.FullName() + ".g";
    const auto localValueConverters = valueConverters.find(namespaceFullName);

    uint32_t numInterfaces = 0;
    bool includeCppWinRtHeader = false;

    {
        FileWriter writer(settings.OutputFolder / (namespaceFileName + ".cpp"));
        WriteNamespace(settings, writer, node,
            (localValueConverters == valueConverters.end()) ? ValueConverters{} : localValueConverters->second,
            specializedInterfaceCreates, numInterfaces, includeCppWinRtHeader);

        writer.Swap();
        writer.WriteInclude(settings.PchFileName);
        writer.WriteNewLine();
        writer.WriteInclude(namespaceFileName + ".h");
        writer.WriteNewLine();
        writer.WriteInclude("base.h"sv);

        if (includeCppWinRtHeader)
        {
            writer.WriteNewLine();
            writer.WriteInclude("ProjectedValueConverters.g.h"sv);
            writer.WriteNewLine();
            writer.WriteInclude("winrt\\" + node.FullName(false /*useCppDelim*/) + ".h");
        }

        namespaceInterfaceCounts.emplace(namespaceFullName, numInterfaces);
    }

    {
        FileWriter writer(settings.OutputFolder / (namespaceFileName + ".h"));
        writer.Write("#pragma once\n\n");
        writer.WriteInclude("base.h"sv);

        for (const auto& childNode : node.Children())
        {
            writer.WriteInclude(childNode.second->FullName(false /*useCppDelim*/) + ".g.h");
        }

        writer.Write(R"(
namespace %::%
{
    std::vector<std::shared_ptr<IProjectedValueProvider>> InitNamespace(const std::shared_ptr<ProjectionsContext>& context);
)",
            c_codegenNamespace, namespaceFullName);

        if (numInterfaces > 0)
        {
            writer.Write(R"(    std::vector<std::shared_ptr<ProjectedInterface>> GetInterfaces();
)");
        }

        writer.Write(R"(} // %::%
)"sv,
            c_codegenNamespace, namespaceFullName);
    }
}

void WriteNamespacesGenerateCppH(const Settings& settings,
    const std::map<std::string_view, std::shared_ptr<Namespace>>& roots,
    const std::map<std::string, ValueConverters> valueConverters,
    std::map<std::string, uint32_t>& namespaceInterfaceCounts)
{
    std::set<std::string> specializedInterfaceCreates;
    Namespace::EnumerateAll(roots, [&](const Namespace& node) {
        WriteNamespaceGenerateCppH(
            settings, node, valueConverters, specializedInterfaceCreates, namespaceInterfaceCounts);
        return true;
    });
}

void WriteProjectionsGeneratedCpp(const Settings& settings,
    const std::map<std::string_view, std::shared_ptr<Namespace>>& roots,
    const std::map<std::string, uint32_t>& namespaceInterfaceCounts)
{
    FileWriter writer(settings.OutputFolder / "Projections.g.cpp");
    writer.WriteInclude(settings.PchFileName);
    writer.WriteNewLine();
    writer.WriteInclude("base.h"sv);
    writer.WriteInclude("Projections.g.h"sv);
    writer.WriteNewLine();

    // Write includes for all generated namespaces since we need access to namespace and interface initializer
    // functions.
    Namespace::EnumerateAll(roots, [&](const Namespace& node) {
        writer.WriteInclude(node.FullName() + ".g.h");
        return true;
    });

    uint32_t totalInterfaceCount = 0;
    for (const auto& namespaceInterfaceCount : namespaceInterfaceCounts)
    {
        totalInterfaceCount += namespaceInterfaceCount.second;
    }

    writer.Write(R"(
namespace %
{
    std::vector<std::shared_ptr<IProjectedValueProvider>> GetProjectedNamespaces(const std::shared_ptr<ProjectionsContext>& context)
    {
        return
        {
)"sv,
        c_codegenNamespace);

    for (const auto& root : roots)
    {
        writer.Write(R"(            ProjectedNamespace::Create("%"sv, context, &::%::%::InitNamespace),
)"sv,
            root.first, c_codegenNamespace, root.first);
    }

    writer.Write(R"(        };
    }

    std::vector<std::shared_ptr<ProjectedInterface>> GetProjectedInterfaces()
    {
        std::vector<std::shared_ptr<ProjectedInterface>> interfaces;
        interfaces.reserve(%);

        const auto AddInterfaces = [&](std::vector<std::shared_ptr<ProjectedInterface>>&& namespaceInterfaces)
        {
            interfaces.insert(interfaces.end(), std::make_move_iterator(namespaceInterfaces.begin()), std::make_move_iterator(namespaceInterfaces.end()));
        };

)"sv,
        totalInterfaceCount);

    for (const auto& namespaceInterfaceCount : namespaceInterfaceCounts)
    {
        if (namespaceInterfaceCount.second > 0)
        {
            writer.Write(R"(        AddInterfaces(%::%::GetInterfaces());
)",
                c_codegenNamespace, namespaceInterfaceCount.first);
        }
    }

    writer.Write(R"(
        return interfaces;
    }
} // %
)"sv,
        c_codegenNamespace);
}

void WriteBaseFiles(const Settings& settings)
{
    for (auto& data : file_strings)
    {
        FileWriter writer(settings.OutputFolder / data.file_name);
        for (auto& str : data.file_contents)
        {
            writer.Write("%", str);
        }
    }
}

void WriteFiles(const Settings& settings, const std::map<std::string_view, std::shared_ptr<Namespace>>& roots)
{
    WriteProjectionsGeneratedH(settings, roots);
    WriteTypescriptDefinitions(settings, roots);

    const auto valueConverters = WriteProjectedValueConvertersGeneratedH(settings, roots);

    std::map<std::string, uint32_t> namespaceInterfaceCounts;
    WriteNamespacesGenerateCppH(settings, roots, valueConverters, namespaceInterfaceCounts);

    WriteProjectionsGeneratedCpp(settings, roots, namespaceInterfaceCounts);

    WriteBaseFiles(settings);
}










void WriteChildren(const std::vector<std::shared_ptr<static_projection_data>>* items, FileWriter& writer)
{
    if (items)
    {
        for (int i = 0; i < items->size(); i++)
        {
            writer.Write("%    %\n", items->at(i)->Name(), items->at(i)->FullName());
            WriteChildren(items->at(i)->Children(), writer);
        }
    }
}

void WriteFiles(const Settings& settings, const std::vector<std::shared_ptr<static_projection_data>>& roots)
{
    FileWriter writer(settings.OutputFolder / "TEST_TREE_VIEW.txt");

    writer.Write("Namespaces\n----------\n");
    WriteChildren(&roots, writer);

    writer.Write("\nStructs\n-------\n");
    //if (_structs)
    {
        for (int i = 0; i < _structs.size(); i++)
        {
            writer.Write("%\n", _structs.at(i)->Name());
        }
    }

    writer.Write("\nDelegates\n---------\n");
    //if (_delegates)
    {
        for (int i = 0; i < _delegates.size(); i++)
        {
            writer.Write("%\n", _delegates.at(i)->Name());
        }
    }

    writer.Write("\nInterfaces\n----------\n");
    //if (_interfaces)
    {
        for (int i = 0; i < _interfaces.size(); i++)
        {
            writer.Write("%\n", _interfaces.at(i)->Name());
        }
    }
}
#endif
