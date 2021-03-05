#pragma once
#include "MetadataHelpers.h"
#include "Parser.h"
#include "TextWriter.h"

class TypescriptWriter
{
private:
    Settings const& settings;

public:
    TypescriptWriter(Settings const& settings) : settings(settings)
    {
    }

    void Write(Namespace const& node, TextWriter& textWriter)
    {
        textWriter.Write("%"sv, "//tslint:disable"sv);
        textWriter.WriteBlankLine();
        textWriter.WriteIndentedLine("declare namespace % {%}"sv, node.FullName(), [&]() {
            textWriter.AddIndent();
            for (auto const& type : node.Members().types)
            {
                WriteTypeDefiniton(type.second, textWriter);
            }
            textWriter.ReduceIndent();
            textWriter.WriteIndentedLine();
        });
    }

    void WriteTypeDefiniton(winmd::reader::TypeDef const& type, TextWriter& textWriter)
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
            WriteDelegate(type, textWriter);
            break;
        }
    }

    void WriteDelegate(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        textWriter.WriteIndentedLine(
            "type % = %", [&]() { WriteGenericTypeName(type, textWriter); },
            [&]() {
                for (auto&& method : type.MethodList())
                {
                    if (method.SpecialName() && (method.Name() == "Invoke"))
                    {
                        WriteMethod(method, type, textWriter, true);
                    }
                }
                textWriter.WriteIndentedLine();
            });
    }

    void WriteEnum(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        textWriter.WriteIndentedLine("enum % {%}"sv, type.TypeName(), [&]() {
            textWriter.AddIndent();
            for (auto field : type.FieldList())
            {
                if (field.Name() == "value__")
                {
                    continue;
                };
                textWriter.WriteIndentedLine("%,"sv, TextWriter::ToCamelCase(std::string(field.Name())));
            }
            textWriter.ReduceIndent();
            textWriter.WriteIndentedLine();
        });
        textWriter.WriteBlankLine();
    }

    void WriteClassOrInterface(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        textWriter.WriteIndentedLine(
            "%% %%% {%}"sv,
            [&]() // Format: ['abstract?' 'class/interface' 'name' 'extends baseclass' 'implements interface1,
                  // interface2' {classcontents}]
            {
                if (type.Flags().Abstract() && get_category(type) != winmd::reader::category::interface_type)
                {
                    textWriter.Write("abstract ");
                }
            },
            [&]() {
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
            [&]() { WriteGenericTypeName(type, textWriter); },
            [&]() {
                if (!type.Extends())
                {
                    return;
                }
                try
                {
                    auto const& extendsTypeSem = jswinrt::typeparser::get_type_semantics(type.Extends());
                    if (std::holds_alternative<jswinrt::typeparser::object_type>(extendsTypeSem))
                    {
                        return;
                    }
                    textWriter.Write(" extends ");
                    WriteTypeSemantics(extendsTypeSem, type, textWriter, false, false);
                }
                catch (std::invalid_argument e)
                {
                }
            },
            [&]() {
                if (type.InterfaceImpl().first == type.InterfaceImpl().second)
                {
                    return;
                }
                textWriter.Write(
                    get_category(type) == winmd::reader::category::interface_type ? " extends " : " implements ");
                for (auto&& interfaceimpl : type.InterfaceImpl())
                {
                    auto const& implementsTypeSem = jswinrt::typeparser::get_type_semantics(interfaceimpl.Interface());
                    WriteTypeSemantics(implementsTypeSem, type, textWriter, false, false);
                    textWriter.Write(", ");
                }
                textWriter.DeleteLast(2);
                if (type.TypeNamespace() == "Windows.Foundation" && type.TypeName() == "IAsyncInfo")
                {
                    textWriter.Write(", Promise<any>");
                }
                else if (type.TypeNamespace() == "Windows.Foundation" &&
                         type.TypeName() == "IAsyncActionWithProgress`1")
                {
                    textWriter.Write(", Promise<TProgress>");
                }
                else if (type.TypeNamespace() == "Windows.Foundation" &&
                         (type.TypeName() == "IAsyncOperationWithProgress`2" || type.TypeName() == "IAsyncOperation`1"))
                {
                    textWriter.Write(", Promise<TResult>");
                }
            },
            [&]() {
                textWriter.AddIndent();
                // Fields:
                for (auto&& field : type.FieldList())
                {
                    textWriter.WriteIndentedLine(
                        "% %: ", [&]() { WriteAccess(field.Flags().Access(), textWriter); },
                        TextWriter::ToCamelCase(std::string(field.Name())));
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(field.Signature().Type()), type,
                        textWriter, field.Signature().Type().is_szarray(), false);
                    textWriter.Write(";");
                }
                // Properties:
                for (auto&& prop : type.PropertyList())
                {
                    textWriter.WriteIndentedLine();
                    if (prop.MethodSemantic().first.Method().Flags().Static())
                    {
                        textWriter.Write("static ");
                    }
                    if (HasGetterWithName(type, prop.Name()) && !HasSetterWithName(type, prop.Name()))
                    {
                        textWriter.Write("readonly ");
                    }
                    textWriter.Write("%: ", TextWriter::ToCamelCase(std::string(prop.Name())));
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(prop.Type().Type()), type, textWriter,
                        prop.Type().Type().is_szarray(), false);

                    textWriter.Write(";");
                }
                // Methods:
                std::map<std::string_view, winmd::reader::MethodDef> eventListeners;
                for (auto&& method : type.MethodList())
                {
                    if (!IsMethodAllowed(settings, method))
                        continue;
                    if (method.SpecialName() &&
                        (StartsWith(method.Name(), "get_") || StartsWith(method.Name(), "put_")))
                        continue;
                    if (method.SpecialName() &&
                        (StartsWith(method.Name(), "add_") || StartsWith(method.Name(), "remove_")))
                    {
                        eventListeners[method.Name()] = method;
                    }
                    else
                    {
                        textWriter.WriteIndentedLine();
                        WriteMethod(method, type, textWriter);
                    }
                }
                // Event Listeners:
                for (auto const& [name, method] : eventListeners)
                {
                    if (!IsMethodAllowed(settings, method))
                        continue;
                    textWriter.WriteIndentedLine();
                    if (name._Starts_with("add_"))
                    {
                        WriteEventListener(method, type, textWriter, true);
                    }
                    else
                    {
                        auto addListenerName = "add_" + std::string(method.Name().substr(7));
                        WriteEventListener(eventListeners[addListenerName], type, textWriter, false);
                    }
                }

                textWriter.ReduceIndent();
                textWriter.WriteIndentedLine();
            });
        textWriter.WriteBlankLine();
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

    void WriteEventListener(winmd::reader::MethodDef const& addEventListener,
        winmd::reader::TypeDef const& containerType, TextWriter& textWriter, bool shouldCreateAddListener)
    {
        textWriter.Write("%EventListener(type: \"%\", listener: %): void", shouldCreateAddListener ? "add" : "remove",
            TextWriter::ToCamelCase(TextWriter::ToLowerAllCase(std::string(addEventListener.Name().substr(4)))), [&]() {
                jswinrt::typeparser::method_signature methodSignature(addEventListener);
                for (auto&& [param, paramSignature] : methodSignature.params())
                {
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(paramSignature->Type()), containerType,
                        textWriter, paramSignature->Type().is_szarray(), false);
                }
            });
    }

    void WriteMethod(winmd::reader::MethodDef const& method, winmd::reader::TypeDef const& containerType,
        TextWriter& textWriter, bool isAnonymousFunction = false)
    {
        jswinrt::typeparser::method_signature methodSignature(method);
        std::vector<std::pair<std::string_view, winmd::reader::TypeSig>> returnNameTypePairs;
        textWriter.Write(
            "%%%%(%)%;"sv, [&]() { WriteAccess(method.Flags().Access(), textWriter); },
            [&]() {
                if (method.Flags().Abstract() && !isAnonymousFunction &&
                    get_category(containerType) == winmd::reader::category::class_type)
                {
                    textWriter.Write("abstract ");
                }
            },
            [&]() {
                if (!isAnonymousFunction && jswinrt::typeparser::is_static(method))
                {
                    textWriter.Write("static ");
                }
            },
            [&]() -> void {
                if (isAnonymousFunction)
                    return;
                if (jswinrt::typeparser::is_constructor(method))
                {
                    textWriter.Write("%", "constructor");
                }
                else
                {
                    textWriter.Write("%", TextWriter::ToCamelCase(std::string(method.Name())));
                }
            },
            [&]() {
                bool hasAtleastOneInParam = false;
                for (auto&& [param, paramSignature] : methodSignature.params())
                {
                    if (param.Flags().Out())
                    {
                        returnNameTypePairs.push_back(std::make_pair(param.Name(), paramSignature->Type()));
                        continue;
                    }
                    textWriter.Write("%: ", param.Name());
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(paramSignature->Type()), containerType,
                        textWriter, paramSignature->Type().is_szarray(), false);
                    textWriter.Write("%", ", ");
                    hasAtleastOneInParam = true;
                }
                if (hasAtleastOneInParam)
                    textWriter.DeleteLast(2);
            },
            [&]() {
                if (jswinrt::typeparser::is_constructor(method))
                {
                    return;
                }
                isAnonymousFunction ? textWriter.Write(" => ") : textWriter.Write(": ");

                if (methodSignature.return_signature() &&
                    methodSignature.return_signature().Type().element_type() != winmd::reader::ElementType::Void)
                {
                    returnNameTypePairs.push_back(
                        std::make_pair("returnValue", methodSignature.return_signature().Type()));
                }

                if (returnNameTypePairs.size() == 0)
                {
                    textWriter.Write("void");
                }
                else if (returnNameTypePairs.size() == 1)
                {
                    WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(returnNameTypePairs.at(0).second),
                        containerType, textWriter, returnNameTypePairs.at(0).second.is_szarray(), false);
                }
                else
                {
                    textWriter.Write("{ ");
                    for (auto&& [returnName, returnType] : returnNameTypePairs)
                    {
                        textWriter.Write("%: ", returnName);
                        WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(returnType), containerType,
                            textWriter, returnType.is_szarray(), false);
                        textWriter.Write("; ");
                    }
                    textWriter.DeleteLast(2);
                    textWriter.Write(" }");
                }
            });
    }

    void WriteTypeSemantics(jswinrt::typeparser::type_semantics const& typeSemantics,
        winmd::reader::TypeDef const& containerType, TextWriter& textWriter, bool isArray, bool isNullable)
    {
        if (std::holds_alternative<jswinrt::typeparser::type_definition>(typeSemantics))
        {
            winmd::reader::TypeDef typeDef = std::get<jswinrt::typeparser::type_definition>(typeSemantics);
            if (!IsTypeDefAllowed(typeDef))
            {
                textWriter.Write("any");
                return;
            }
            if (typeDef.TypeNamespace() == "Windows.Foundation" && typeDef.TypeName() == "HResult")
            {
                textWriter.Write("number");
                return;
            }
            bool isStruct = get_category(typeDef) == winmd::reader::category::struct_type;
            textWriter.Write("%.%%%", typeDef.TypeNamespace(), typeDef.TypeName(), isArray ? "[]" : "",
                isNullable && !isArray && !isStruct ? " | null" : "");
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
            if (isArray)
            {
                textWriter.Write("[]");
            }
        }
        else if (std::holds_alternative<jswinrt::typeparser::generic_type_instance>(typeSemantics))
        {
            auto generic_type_instance = std::get<jswinrt::typeparser::generic_type_instance>(typeSemantics);
            if (!IsTypeDefAllowed(generic_type_instance.generic_type))
            {
                textWriter.Write("any");
                return;
            }
            if (generic_type_instance.generic_type.TypeNamespace() == "Windows.Foundation" &&
                generic_type_instance.generic_type.TypeName() == "IReference`1")
            {
                WriteTypeSemantics(generic_type_instance.generic_args[0], containerType, textWriter, false, isNullable);
                textWriter.Write("% | null", isArray ? "[]"sv : ""sv);
                return;
            }
            textWriter.Write("%.%<", generic_type_instance.generic_type.TypeNamespace(),
                generic_type_instance.generic_type.TypeName().substr(
                    0, generic_type_instance.generic_type.TypeName().size() - 2));
            for (int i = 0; i < generic_type_instance.generic_args.size(); i++)
            {
                WriteTypeSemantics(generic_type_instance.generic_args[i], containerType, textWriter, false, isNullable);
                if (i < generic_type_instance.generic_args.size() - 1)
                {
                    textWriter.Write(", ");
                }
            }
            textWriter.Write(">%%", isArray ? "[]" : "", isNullable ? " | null" : "");
        }
        else if (std::holds_alternative<jswinrt::typeparser::generic_type_param>(typeSemantics))
        {
            auto generic_type_param = std::get<jswinrt::typeparser::generic_type_param>(typeSemantics);
            textWriter.Write("%%", generic_type_param.Name(), isArray ? "[]" : "");
        }
        else if (std::holds_alternative<jswinrt::typeparser::guid_type>(typeSemantics))
        {
            textWriter.Write("%%"sv, "string", isArray ? "[]" : "");
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
            auto& index = std::get<jswinrt::typeparser::generic_type_index>(typeSemantics).index;
            textWriter.Write("%%", containerType.GenericParam().first[index].Name(), isArray ? "[]" : "");
        }
        else
        {
            textWriter.Write("void");
        }
    }

    void WriteGenericTypeName(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        if (IsGeneric(type.TypeName()))
        {
            textWriter.Write("%"sv, type.TypeName().substr(0, type.TypeName().size() - 2));
            textWriter.Write("<"sv);
            for (auto&& param : type.GenericParam())
            {
                textWriter.Write("%, "sv, param.Name());
            }
            textWriter.DeleteLast(2);
            textWriter.Write(">"sv);
        }
        else
        {
            textWriter.Write(type.TypeName());
        }
    }

    void WriteAccess(winmd::reader::MemberAccess const& access, TextWriter& textWriter, bool skipIfPublic = true)
    {
        switch (access)
        {
        case winmd::reader::MemberAccess::Private:
            textWriter.Write("private ");
            break;
        case winmd::reader::MemberAccess::Public:
            if (!skipIfPublic)
                textWriter.Write("public ");
            break;
        case winmd::reader::MemberAccess::Family:
            textWriter.Write("protected ");
            break;
        default:
            textWriter.Write("private ");
            break;
        }
    }

    bool IsTypeDefAllowed(winmd::reader::TypeDef const& type)
    {
        return IsTypeAllowed(settings, type, get_category(type) == winmd::reader::category::class_type);
    }

    static bool IsGeneric(std::string_view name)
    {
        return name.find("`") != std::string::npos;
    }
};
