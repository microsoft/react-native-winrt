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

    void Write(const namespace_projection_data& ns, TextWriter& textWriter)
    {
        textWriter.Write("%"sv, "//tslint:disable"sv);
        textWriter.WriteBlankLine();
        textWriter.WriteIndentedLine("declare namespace % {%}"sv, ns.full_name, [&]() {
            textWriter.AddIndent();
            if (ns.members)
            {
                for (auto const& type : ns.members->types)
                {
                    WriteTypeDefiniton(type.second, textWriter);
                }
            }
            textWriter.ReduceIndent();
            textWriter.WriteIndentedLine();
        });
    }

    void WriteTypeDefiniton(winmd::reader::TypeDef const& type, TextWriter& textWriter)
    {
        if (!is_type_allowed(settings, type))
            return;

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
        if (get_category(type) == winmd::reader::category::interface_type && exclusiveto_class(type))
        {
            return;
        }
        textWriter.WriteIndentedLine(
            "%% %%% {%}"sv,
            [&]() // Format: ['abstract?' 'class/interface' 'name' 'extends baseclass' 'implements interface1,
                  // interface2' {classcontents}]
            {
                if (type.TypeNamespace() == "Windows.Foundation" && type.TypeName() == "IAsyncInfo")
                {
                    textWriter.Write(
                        "%", R"(interface WinRTPromiseBase<TResult, TProgress> extends PromiseLike<TResult> {
        then<U, V>(success?: (value: TResult) => Promise<U>, error?: (error: any) => Promise<U>, progress?: (progress: TProgress) => void): Promise<U>;
        then<U, V>(success?: (value: TResult) => Promise<U>, error?: (error: any) => U, progress?: (progress: TProgress) => void): Promise<U>;
        then<U, V>(success?: (value: TResult) => U, error?: (error: any) => Promise<U>, progress?: (progress: TProgress) => void): Promise<U>;
        then<U, V>(success?: (value: TResult) => U, error?: (error: any) => U, progress?: (progress: TProgress) => void): Promise<U>;
        done<U, V>(success?: (value: TResult) => any, error?: (error: any) => any, progress?: (progress: TProgress) => void): void;
        cancel(): void;
        operation: IAsyncInfo;
    }

    type WinRTPromise<TResult, TProgress> = WinRTPromiseBase<TResult, TProgress> & Promise<TResult>;
)");
                    textWriter.WriteIndentedLine();
                }
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
                // filter out exclusiveto interfaces
                std::vector<jswinrt::typeparser::type_semantics> filteredInterfaces;
                for (auto&& interfaceimpl : type.InterfaceImpl())
                {
                    auto const& implementsTypeSem = jswinrt::typeparser::get_type_semantics(interfaceimpl.Interface());
                    if (std::holds_alternative<jswinrt::typeparser::type_definition>(implementsTypeSem))
                    {
                        if (exclusiveto_class(std::get<jswinrt::typeparser::type_definition>(implementsTypeSem)))
                        {
                            continue;
                        }
                    }
                    if (std::holds_alternative<jswinrt::typeparser::generic_type_instance>(implementsTypeSem))
                    {
                        if (exclusiveto_class(
                                std::get<jswinrt::typeparser::generic_type_instance>(implementsTypeSem).generic_type))
                        {
                            continue;
                        }
                    }
                    filteredInterfaces.push_back(implementsTypeSem);
                }
                if (filteredInterfaces.size() == 0)
                {
                    return;
                }
                textWriter.Write(
                    get_category(type) == winmd::reader::category::interface_type ? " extends " : " implements ");
                for (auto&& implementsTypeSem : filteredInterfaces)
                {
                    WriteTypeSemantics(implementsTypeSem, type, textWriter, false, false);
                    textWriter.Write(", ");
                }
                textWriter.DeleteLast(2);
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
                std::vector<winmd::reader::MethodDef> eventListeners;
                for (auto&& method : type.MethodList())
                {
                    if (!is_method_allowed(settings, method))
                        continue;
                    else if (!method.SpecialName() || (method.Name() == ".ctor"sv))
                    {
                        textWriter.WriteIndentedLine();
                        WriteMethod(method, type, textWriter);
                    }
                    else if (starts_with(method.Name(), "add_"))
                    {
                        eventListeners.push_back(method);
                    }
                    // NOTE: If there's an add there must be a remove and vice-versa
                    // NOTE: Properties handled later
                }

                // Event Listeners:
                for (auto&& method : eventListeners)
                {
                    WriteEventListener(method, type, textWriter);
                }

                WriteSpecialPropertiesAndMethods(textWriter, type);

                textWriter.ReduceIndent();
                textWriter.WriteIndentedLine();
            });
        textWriter.WriteBlankLine();
    }

    void WriteSpecialPropertiesAndMethods(TextWriter& textWriter, winmd::reader::TypeDef const& type)
    {
        if (type.TypeNamespace() == "Windows.Foundation.Collections" && type.TypeName() == "IMapView`2")
        {
            textWriter.Write("%", R"(
        readonly [key: string]: any;)");
        }
        else if (type.TypeNamespace() == "Windows.Foundation.Collections" && type.TypeName() == "IMap`2")
        {
            textWriter.Write("%", R"(
        [key: string]: any;)");
        }
        else if (type.TypeNamespace() == "Windows.Foundation.Collections" && type.TypeName() == "IVector`1")
        {
            textWriter.Write("%", R"(
        length: number;
        [index: number]: T;
        concat(...items: (T | ConcatArray<T>)[]): T[];
        copyWithin(target: number, start: number, end?: number): this;
        every<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): this is S[];
        some(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): boolean;
        filter<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): S[];
        filter(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): T[];
        forEach(callbackfn: (value: T, index: number, array: T[]) => void, thisArg?: any): void;
        lastIndexOf(searchElement: T, fromIndex?: number): number;
        pop(): T | undefined;
        push(...items: T[]): number;
        map<U>(callbackfn: (value: T, index: number, array: T[]) => U, thisArg?: any): U[];
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        reverse(): T[];
        shift(): T | undefined;
        slice(start?: number, end?: number): T[];
        sort(compareFn?: (a: T, b: T) => number): this;
        splice(start: number, deleteCount?: number): T[];
        some(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): boolean;
        unshift(...items: T[]): number;)");
        }
        else if (type.TypeNamespace() == "Windows.Foundation.Collections" && type.TypeName() == "IVectorView`1")
        {
            textWriter.Write("%", R"(
        length: number;
        readonly [index: number]: T;
        concat(...items: (T | ConcatArray<T>)[]): T[];
        every<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): this is S[];
        filter<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): S[];
        filter(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): T[];
        forEach(callbackfn: (value: T, index: number, array: T[]) => void, thisArg?: any): void;
        lastIndexOf(searchElement: T, fromIndex?: number): number;
        map<U>(callbackfn: (value: T, index: number, array: T[]) => U, thisArg?: any): U[];
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        slice(start?: number, end?: number): T[];
        some(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): boolean;)");
        }
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
        winmd::reader::TypeDef const& containerType, TextWriter& textWriter)
    {
        std::string name{ addEventListener.Name().substr(4) }; // Remove 'add_'
        std::transform(
            name.begin(), name.end(), name.begin(), [](char ch) { return static_cast<char>(::tolower(ch)); });

        const char* accessType = addEventListener.Flags().Static() ? "static " : "";

        textWriter.WriteIndentedLine();
        textWriter.Write(R"^-^(%addEventListener(type: "%", listener: %): void;)^-^", accessType, name, [&]() {
            jswinrt::typeparser::method_signature methodSignature(addEventListener);
            for (auto&& [param, paramSignature] : methodSignature.params())
            {
                WriteTypeSemantics(jswinrt::typeparser::get_type_semantics(paramSignature->Type()), containerType,
                    textWriter, paramSignature->Type().is_szarray(), false);
            }
        });

        textWriter.WriteIndentedLine();
        textWriter.Write(R"^-^(%removeEventListener(type: "%", listener: %): void;)^-^", accessType, name, [&]() {
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
            if (!is_type_allowed(settings, typeDef))
            {
                textWriter.Write("any");
                return;
            }
            if (typeDef.TypeNamespace() == "Windows.Foundation" && typeDef.TypeName() == "HResult")
            {
                textWriter.Write("number");
                return;
            }
            if (typeDef.TypeNamespace() == "Windows.Foundation" && typeDef.TypeName() == "IAsyncAction")
            {
                textWriter.Write("Windows.Foundation.WinRTPromise<void, void>");
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
            if (!is_type_allowed(settings, generic_type_instance.generic_type))
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
            if (generic_type_instance.generic_type.TypeNamespace() == "Windows.Foundation" &&
                generic_type_instance.generic_type.TypeName() == "IAsyncOperationWithProgress`2")
            {
                textWriter.Write("%<", "Windows.Foundation.WinRTPromise");
                WriteTypeSemantics(generic_type_instance.generic_args[0], containerType, textWriter, false, isNullable);
                textWriter.Write(", ");
                WriteTypeSemantics(generic_type_instance.generic_args[1], containerType, textWriter, false, isNullable);
                textWriter.Write(">");
                return;
            }
            if (generic_type_instance.generic_type.TypeNamespace() == "Windows.Foundation" &&
                generic_type_instance.generic_type.TypeName() == "IAsyncOperation`1")
            {
                textWriter.Write("%<", "Windows.Foundation.WinRTPromise");
                WriteTypeSemantics(generic_type_instance.generic_args[0], containerType, textWriter, false, isNullable);
                textWriter.Write(", void>");
                return;
            }
            if (generic_type_instance.generic_type.TypeNamespace() == "Windows.Foundation" &&
                generic_type_instance.generic_type.TypeName() == "IAsyncActionWithProgress`1")
            {
                textWriter.Write("%<", "Windows.Foundation.WinRTPromise");
                textWriter.Write("void, ");
                WriteTypeSemantics(generic_type_instance.generic_args[0], containerType, textWriter, false, isNullable);
                textWriter.Write(">");
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

    static bool IsGeneric(std::string_view name)
    {
        return name.find("`") != std::string::npos;
    }
};

inline void write_typescript_file(const Settings& settings, const namespace_projection_data& ns)
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

inline void write_typescript_files(const Settings& settings, const projection_data& data)
{
    assert(!settings.TypescriptOutputFolder.empty());
    std::filesystem::remove_all(settings.TypescriptOutputFolder);
    std::filesystem::create_directory(settings.TypescriptOutputFolder);

    for (auto& ns : data.root_namespaces)
    {
        write_typescript_file(settings, *ns);
    }
}
