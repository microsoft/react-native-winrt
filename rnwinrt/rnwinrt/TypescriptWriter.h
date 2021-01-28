#pragma once
#include "TextWriter.h"
#include "MetadataHelpers.h"
#include "Parser.h"

class TypescriptWriter
{

public:

    static void Write(Namespace const& node, TextWriter& textWriter)
    {
        textWriter.Write("declare namespace %{%}"sv, node.FullName(), [&]() -> void 
            {
                for (auto const& type : node.Members().types)
                {
                    textWriter.AddIndent();
                    Write(type.second, textWriter);
                    textWriter.ReduceIndent();
                }
            }
        );
    }

    static void Write(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        switch (get_category(type))
        {
        case winmd::reader::category::struct_type:
        case winmd::reader::category::interface_type:
        case winmd::reader::category::class_type:
            WriteClassOrInterface(type, textWriter);
            break;
        case winmd::reader::category::enum_type:
            WriteEnum(type, textWriter);
            break;
        case winmd::reader::category::delegate_type:
            
            break;
        }
    }

    static void WriteEnum(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        textWriter.WriteIndentedLine("enum % {%}"sv, type.TypeName(), [&]() -> void {
            textWriter.AddIndent();
            for (auto field : type.FieldList())
            {
                textWriter.WriteIndentedLine("%,"sv, field.Name());
            }
            textWriter.ReduceIndent();
        });
        textWriter.WriteIndentedLine(""sv);
    }

    static void WriteClassOrInterface(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        textWriter.WriteIndentedLine("%% %% {%\n}"sv, [&]() -> void //Format: ['abstract/nonabstract' 'class/interface' 'name' 'baseclass' {classcontents}]
            {
                if (type.Flags().Abstract() && get_category(type) != winmd::reader::category::interface_type)
                {
                    textWriter.Write("abstract ");
                }
            },
            [&]() -> void {
                switch (get_category(type))
                {
                case winmd::reader::category::interface_type:
                    textWriter.Write("interface");
                    break;
                case winmd::reader::category::class_type:
                    textWriter.Write("class");
                    break;
                case winmd::reader::category::struct_type:
                    textWriter.Write("interface");
                    break;
                }
            },
            [&]() -> void 
            {
                if (IsGeneric(type.TypeName()))
                {
                    textWriter.Write("%"sv, type.TypeName().substr(0, type.TypeName().size() - 2));
                    textWriter.Write("<"sv);
                    for (auto&& param : type.GenericParam())
                    {
                        textWriter.Write("%,"sv, param.Name());
                    }
                    textWriter.DeleteLastCharacter();
                    textWriter.Write(">"sv);
                }
                else
                {
                    textWriter.Write(type.TypeName());
                }
            },
            [&]() -> void 
            {
                if (!type.Extends())
                {
                    return;
                }
                try
                {
                    auto const& typeSem = jswinrt::typeparser::get_type_semantics(type.Extends());
                    if (std::holds_alternative<jswinrt::typeparser::object_type>(typeSem))
                    {
                        return;
                    }
                    textWriter.Write(" extends ");
                    WriteTypeSemantics(typeSem, textWriter);
                }
                catch (std::invalid_argument e)
                {
                    std::cout << "Error in " << type.TypeName() << e.what() << std::endl;
                }
            },
            [&]() -> void 
            {
                textWriter.AddIndent();

                for (auto&& field: type.FieldList())
                {
                    textWriter.WriteIndentedLine("%: ", field.Name());
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(field.Signature().Type()), textWriter);
                    textWriter.Write(";");
                }

                for (auto&& prop : type.PropertyList())
                {
                    textWriter.WriteIndentedLine("%: ", prop.Name());
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(prop.Type().Type()), textWriter);
                    textWriter.Write(";");
                }

                for (auto&& method : type.MethodList())
                {
                    WriteMethod(method, textWriter);
                }
                textWriter.ReduceIndent();
            }
        );
    }

    static void WriteMethod(winmd::reader::MethodDef const& method, TextWriter& textWriter)
    {
        jswinrt::typeparser::method_signature methodSignature(method);
        textWriter.WriteIndentedLine("%%(%)%;"sv, [&]() -> void {
                if (jswinrt::typeparser::is_static(method))
                {
                    textWriter.Write("static ");
                }
            },
            jswinrt::typeparser::is_constructor(method) ? "constructor": method.Name(), 
            [&]() -> void {
                auto idx = 0;
                for (auto&& [param, paramSignature] : methodSignature.params())
                {
                    textWriter.Write("%: ", param.Name());
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(paramSignature->Type()), textWriter);
                    if (idx < methodSignature.params().size() - 1)
                    {
                        textWriter.Write(", ");
                    }
                    idx++;
                }
            },
            [&]() -> void {
                if (jswinrt::typeparser::is_constructor(method))
                {
                    return;
                }
                textWriter.Write(": ");
                WriteTypeSemantics(
                    jswinrt::typeparser::get_type_semantics(methodSignature.return_signature().Type()), textWriter);
            }
        );
    }

    static void WriteTypeSemantics(jswinrt::typeparser::type_semantics const& typeSemantics, TextWriter& textWriter)
    {
        if (std::holds_alternative<jswinrt::typeparser::type_definition>(typeSemantics))
        {
            auto typeDef = std::get<jswinrt::typeparser::type_definition>(typeSemantics);
            textWriter.Write(typeDef.TypeNamespace());
            textWriter.Write(".");
            textWriter.Write(typeDef.TypeName());
        }
        else if (std::holds_alternative<jswinrt::typeparser::fundamental_type>(typeSemantics))
        {
            auto typeDef = std::get<jswinrt::typeparser::fundamental_type>(typeSemantics);
            switch (typeDef)
            {
            case jswinrt::typeparser::fundamental_type::String:
                textWriter.Write("string");
                break;
            default:
                textWriter.Write("any");
                break;
            }

        }
        else if (std::holds_alternative<jswinrt::typeparser::generic_type_instance>(typeSemantics))
        {
            auto generic_type_instance = std::get<jswinrt::typeparser::generic_type_instance>(typeSemantics);
            textWriter.Write("%.%<", generic_type_instance.generic_type.TypeNamespace(),            
                generic_type_instance.generic_type.TypeName().substr(0, generic_type_instance.generic_type.TypeName().size() - 2));
            for (int i = 0; i < generic_type_instance.generic_args.size(); i++)
            {
                WriteTypeSemantics(generic_type_instance.generic_args[i], textWriter);
                if (i < generic_type_instance.generic_args.size() - 1)
                {
                    textWriter.Write(", ");
                }
            }
            textWriter.Write(">");
        }
        else
        {
            textWriter.Write("any");
        }
    }

    static bool IsGeneric(std::string_view name)
    {
        return name.find("`") != std::string::npos;
    }

};
