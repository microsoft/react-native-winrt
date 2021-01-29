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
            try
            {
                WriteDelegate(type, textWriter);
            } 
            catch (std::invalid_argument e)
            {
                std::cout << e.what() << std::endl;
            }
            break;
        }
    }

    static void WriteDelegate(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        textWriter.WriteIndentedLine("type % = %", [&]() -> void {
            WriteGenericTypeName(type, textWriter);
        }, [&]() -> void {
            for (auto&& method : type.MethodList())
            {
                if (method.SpecialName() && (method.Name() == "Invoke"))
                {
                    WriteMethod(method, textWriter, true);
                }
            }
        });
    }

    static void WriteEnum(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        textWriter.WriteIndentedLine("enum % {%}"sv, type.TypeName(), [&]() -> void {
            textWriter.AddIndent();
            for (auto field : type.FieldList())
            {
                textWriter.WriteIndentedLine("%,"sv, TextWriter::ToLowerCamelCase(field.Name()));
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
                WriteGenericTypeName(type, textWriter);
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
                    WriteTypeSemantics(typeSem, textWriter, false, false);
                }
                catch (std::invalid_argument e){}
            },
            [&]() -> void 
            {
                textWriter.AddIndent();
                //Fields:
                for (auto&& field: type.FieldList())
                {
                    textWriter.WriteIndentedLine(
                        "% %: ", [&]() { WriteAccess(field.Flags().Access(), textWriter); }, TextWriter::ToLowerCamelCase(field.Name()));
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(field.Signature().Type()), textWriter,
                        field.Signature().Type().is_szarray(), true);
                    textWriter.Write(";");
                }
                //Properties:
                for (auto&& prop : type.PropertyList())
                {
                    textWriter.WriteIndentedLine("");
                    if (HasGetterWithName(type, prop.Name()) && !HasSetterWithName(type, prop.Name()))
                    {
                        textWriter.Write("readonly ");
                    }
                    textWriter.Write("%: ", TextWriter::ToLowerCamelCase(prop.Name()));
                    WriteTypeSemantics(
                        jswinrt::typeparser::get_type_semantics(prop.Type().Type()), textWriter, prop.Type().Type().is_szarray(), true);
                    textWriter.Write(";");
                }
                //Methods:
                std::map<std::string_view, winmd::reader::MethodDef> eventListeners;
                for (auto&& method : type.MethodList())
                {
                    if (method.SpecialName() && (method.Name()._Starts_with("get_") || method.Name()._Starts_with("put_")))
                        continue;
                    if (method.SpecialName() && (method.Name()._Starts_with("add_") || method.Name()._Starts_with("remove_")))
                    {
                        eventListeners[method.Name()] = method;
                    }
                    else
                    {
                        textWriter.WriteIndentedLine("");
                        WriteMethod(method, textWriter);   
                    }
                }
                //Event Listeners:
                for (auto const& [name, method]: eventListeners)
                {
                    textWriter.WriteIndentedLine("");
                    if (name._Starts_with("add_"))
                    {
                        WriteEventListenerAsProperty(method, textWriter);
                        textWriter.WriteIndentedLine("");
                        WriteEventListener(method, textWriter, true);
                    }
                    else 
                    {
                        auto addListenerName = "add_" + std::string(method.Name().substr(7));
                        WriteEventListener(eventListeners[addListenerName], textWriter, false);
                    }
                }

                textWriter.ReduceIndent();
            }
        );
    }

    static bool HasGetterWithName(winmd::reader::TypeDef const& type, std::string_view const& name)
    {
        for (auto&& method : type.MethodList())
        {
            if (method.SpecialName() && method.Name()._Starts_with("get_") && method.Name().substr(4) == name)
                return true;
        }
        return false;
    }

    static bool HasSetterWithName(winmd::reader::TypeDef const& type, std::string_view const& name)
    {
        for (auto&& method : type.MethodList())
        {
            if (method.SpecialName() && method.Name()._Starts_with("put_") && method.Name().substr(4) == name)
                return true;
        }
        return false;
    }

    static void WriteEventListenerAsProperty(winmd::reader::MethodDef const& addEventListener, TextWriter& textWriter)
    {
        textWriter.Write("on%: %;", TextWriter::ToLowerCamelCase(addEventListener.Name().substr(4)), [&]() -> void {
            jswinrt::typeparser::method_signature methodSignature(addEventListener);
            for (auto&& [param, paramSignature] : methodSignature.params())
            {
                if (param.Name() != "handler")
                    continue;
                WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(paramSignature->Type()), textWriter,
                    paramSignature->Type().is_szarray(), false);
            }
        });
    }

    static void WriteEventListener(winmd::reader::MethodDef const& addEventListener, TextWriter& textWriter, bool shouldCreateAddListener)
    {
        textWriter.Write("%EventListener(type: \"%\", listener: %): void",
            shouldCreateAddListener ? "add" : "remove",
            TextWriter::ToLowerCamelCase(addEventListener.Name().substr(4)), 
            [&]() -> void {
                jswinrt::typeparser::method_signature methodSignature(addEventListener);
                for (auto&& [param, paramSignature] : methodSignature.params())
                {
                    if (param.Name() != "handler")
                        continue;
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(paramSignature->Type()), textWriter,
                        paramSignature->Type().is_szarray(), false);
                }
            }
        );
        
    }

    static void WriteMethod(winmd::reader::MethodDef const& method, TextWriter& textWriter, bool isAnonymousFunction = false)
    {
        jswinrt::typeparser::method_signature methodSignature(method);
        textWriter.Write("%%%(%)%;"sv, 
            [&]() -> void { 
                WriteAccess(method.Flags().Access(), textWriter);
            },           
            [&]() -> void {
                if (!isAnonymousFunction && jswinrt::typeparser::is_static(method))
                {
                    textWriter.Write("static ");
                }
            },
            [&]() -> void
            {
                if (isAnonymousFunction) return; 
                if (jswinrt::typeparser::is_constructor(method))
                {
                    textWriter.Write("%", "constructor");
                }
                else
                {
                    textWriter.Write("%", TextWriter::ToLowerCamelCase(method.Name()));
                }
            }, 
            [&]() -> void {
                auto idx = 0;
                for (auto&& [param, paramSignature] : methodSignature.params())
                {
                    textWriter.Write("%: ", param.Name());
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(paramSignature->Type()), textWriter,
                        paramSignature->Type().is_szarray(), true);
                    if (idx < methodSignature.params().size() - 1)
                    {
                        textWriter.Write("%", ", ");
                    }
                    idx++;
                }
            },
            [&]() -> void {
                if (jswinrt::typeparser::is_constructor(method))
                {
                    return;
                }
                isAnonymousFunction ? textWriter.Write(" => ") : textWriter.Write(": ");
                if (methodSignature.return_signature().Type().element_type() == winmd::reader::ElementType::Void)
                {
                    textWriter.Write("void");
                    return;
                }
                WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(methodSignature.return_signature().Type()),
                    textWriter, methodSignature.return_signature().Type().is_szarray(), true);
            }
        );
    }

    static void WriteTypeSemantics(
        jswinrt::typeparser::type_semantics const& typeSemantics, TextWriter& textWriter, bool isArray, bool isNullable)
    {
        if (std::holds_alternative<jswinrt::typeparser::type_definition>(typeSemantics))
        {
            auto typeDef = std::get<jswinrt::typeparser::type_definition>(typeSemantics);
            textWriter.Write("%.%%%", typeDef.TypeNamespace(), typeDef.TypeName(), isArray ? "[]" : "", isNullable ? " | null" : "");
        }
        else if (std::holds_alternative<jswinrt::typeparser::fundamental_type>(typeSemantics))
        {
            auto typeDef = std::get<jswinrt::typeparser::fundamental_type>(typeSemantics);
            switch (typeDef)
            {
            case jswinrt::typeparser::fundamental_type::Int8:
            case jswinrt::typeparser::fundamental_type::Int16:
            case jswinrt::typeparser::fundamental_type::Int32:
            case jswinrt::typeparser::fundamental_type::Int64:
            case jswinrt::typeparser::fundamental_type::Double:
            case jswinrt::typeparser::fundamental_type::Float:
            case jswinrt::typeparser::fundamental_type::UInt16:
            case jswinrt::typeparser::fundamental_type::UInt8:
            case jswinrt::typeparser::fundamental_type::UInt32:
            case jswinrt::typeparser::fundamental_type::UInt64:
                textWriter.Write("number");
                break;
            case jswinrt::typeparser::fundamental_type::Char:
            case jswinrt::typeparser::fundamental_type::String:
                textWriter.Write("string");
                break;
            case jswinrt::typeparser::fundamental_type::Boolean:
                textWriter.Write("boolean");
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
                WriteTypeSemantics(generic_type_instance.generic_args[i], textWriter, false, isNullable);
                if (i < generic_type_instance.generic_args.size() - 1)
                {
                    textWriter.Write(", ");
                }
            }
            textWriter.Write(">%", isNullable ? " | null" : "");
        }
        else if (std::holds_alternative<jswinrt::typeparser::generic_type_param>(typeSemantics))
        {
            auto generic_type_param = std::get<jswinrt::typeparser::generic_type_param>(typeSemantics);
            textWriter.Write("%", generic_type_param.Name());
        }
        else if (std::holds_alternative<jswinrt::typeparser::guid_type>(typeSemantics))
        {
            textWriter.Write("string");
        }
        else if (std::holds_alternative<jswinrt::typeparser::object_type>(typeSemantics))
        {
            textWriter.Write("any");
        }
        else if (std::holds_alternative<jswinrt::typeparser::type_type>(typeSemantics))
        {
            textWriter.Write("any");
        }
        else if (std::holds_alternative<jswinrt::typeparser::generic_type_index>(typeSemantics))
        {
            textWriter.Write("any");
        }
        else
        {
            textWriter.Write("void");
        }
    }

    static void WriteGenericTypeName(winmd::reader::TypeDef const& type, TextWriter& textWriter)
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
    }

    static void WriteAccess(winmd::reader::MemberAccess const& access, TextWriter& textWriter, bool skipIfPublic = true)
    {
        switch (access)
        {
        case winmd::reader::MemberAccess::Private:
            textWriter.Write("private ");
            break;
        case winmd::reader::MemberAccess::Public:
            if (!skipIfPublic) textWriter.Write("public ");
            break;
        case winmd::reader::MemberAccess::Family:
            textWriter.Write("protected ");
            break;
        default:
            textWriter.Write("private ");
            break;
        }
    }

    static bool IsGeneric(std::string_view name)
    {
        return name.find("`") != std::string::npos;
    }

};
