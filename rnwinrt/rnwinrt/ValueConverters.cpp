#include "pch.h"
#include "ValueConverters.h"

ProjectedValueKind GetProjectedValueKind(const winmd::reader::TypeDef& typeDef)
{
    switch (winmd::reader::get_category(typeDef))
    {
    case winmd::reader::category::interface_type:
    case winmd::reader::category::class_type:
        return ProjectedValueKind::Interface;

    case winmd::reader::category::enum_type:
        return ProjectedValueKind::Enum;

    case winmd::reader::category::struct_type:
        return ProjectedValueKind::Struct;

    case winmd::reader::category::delegate_type:
        return ProjectedValueKind::Delegate;

    default:
        FAIL_FAST();
    }
}

ProjectedValueKind GetProjectedValueKind(const winmd::reader::coded_index<winmd::reader::TypeDefOrRef>& type)
{
    const auto& [typeNamespace, typeName] = winmd::reader::get_type_namespace_and_name(type);
    if (typeNamespace == "Windows.Foundation"sv)
    {
        if (typeName == "IAsyncAction"sv)
        {
            return ProjectedValueKind::AsyncAction;
        }
    }

    const auto typeDefOrRef = type.type();
    if (typeDefOrRef == winmd::reader::TypeDefOrRef::TypeSpec)
    {
        return ProjectedValueKind::Interface;
    }

    winmd::reader::TypeDef typeDef{};
    if (typeDefOrRef == winmd::reader::TypeDefOrRef::TypeDef)
    {
        typeDef = type.TypeDef();
    }
    else
    {
        FAIL_FAST_IF(typeDefOrRef !=  winmd::reader::TypeDefOrRef::TypeRef);
        const auto typeRef = type.TypeRef();
        if ((typeRef.TypeNamespace() == "System"sv) && (typeRef.TypeName() == "Guid"sv))
        {
            return ProjectedValueKind::Guid;
        }
        typeDef = winmd::reader::find_required(type.TypeRef());
    }

    return GetProjectedValueKind(typeDef);
}

ProjectedValueKind GetProjectedValueKind(const winmd::reader::ElementType& elementType)
{
    switch (elementType)
    {
    case winmd::reader::ElementType::Boolean:
        return ProjectedValueKind::Boolean;

    case winmd::reader::ElementType::Char:
        return ProjectedValueKind::Char;

    case winmd::reader::ElementType::I1:
    case winmd::reader::ElementType::U1:
    case winmd::reader::ElementType::I2:
    case winmd::reader::ElementType::U2:
    case winmd::reader::ElementType::I4:
    case winmd::reader::ElementType::U4:
    case winmd::reader::ElementType::I8:
    case winmd::reader::ElementType::U8:
    case winmd::reader::ElementType::R4:
    case winmd::reader::ElementType::R8:
        return ProjectedValueKind::Number;

    case winmd::reader::ElementType::String:
        return ProjectedValueKind::String;

    case winmd::reader::ElementType::Object:
        return ProjectedValueKind::Interface;

    default:
        FAIL_FAST();
    }
}

ProjectedValueKind GetProjectedValueKind(const winmd::reader::GenericTypeInstSig& genericTypeSig)
{
    const auto& [typeNamespace, typeName] = winmd::reader::get_type_namespace_and_name(genericTypeSig.GenericType());
    if (typeNamespace == "Windows.Foundation"sv)
    {
        if (typeName == "IAsyncActionWithProgress`1"sv)
        {
            return ProjectedValueKind::AsyncActionWithProgress;
        }
        else if (typeName == "IAsyncOperation`1"sv)
        {
            return ProjectedValueKind::AsyncOperation;
        }
        else if (typeName == "IAsyncOperationWithProgress`2"sv)
        {
            return ProjectedValueKind::AsyncOperationWithProgress;
        }
        else if (typeName == "IReference`1"sv)
        {
            return ProjectedValueKind::Reference;
        }
        else if (typeName == "IReferenceArray`1"sv)
        {
            return ProjectedValueKind::ReferenceArray;
        }
        else if (typeName == "EventHandler`1"sv)
        {
            return ProjectedValueKind::EventHandler;
        }
        else if (typeName == "TypedEventHandler`2"sv)
        {
            return ProjectedValueKind::TypedEventHandler;
        }
    }
    else if (typeNamespace == "Windows.Foundation.Collections"sv)
    {
        return ProjectedValueKind::GenericCollectionInterface;
    }

    FAIL_FAST();
}

ProjectedValueKind GetProjectedValueKind(const winmd::reader::TypeSig& typeSig)
{
    ProjectedValueKind kind = ProjectedValueKind::Unknown;

    std::string_view result;
    std::visit([&](auto&& type)
    {
        using T = std::decay_t<decltype(type)>;
        if constexpr (std::is_same_v<T, winmd::reader::ElementType> || std::is_same_v<T, winmd::reader::coded_index<winmd::reader::TypeDefOrRef>> ||
            std::is_same_v<T, winmd::reader::GenericTypeInstSig>)
        {
            kind = GetProjectedValueKind(type);
        }
        else if constexpr (std::is_same_v<T, winmd::reader::GenericTypeIndex>)
        {
            FAIL_FAST();
        }
        else if constexpr (std::is_same_v<T, winmd::reader::GenericMethodTypeIndex>)
        {
            FAIL_FAST();
        }
        else
        {
            FAIL_FAST();
        }
    }, typeSig.Type());

    return kind;
}

std::string_view GetConverterName(ProjectedValueKind kind)
{
    switch (kind)
    {
    case ProjectedValueKind::Boolean:
        return "Boolean"sv;

    case ProjectedValueKind::Number:
        return "Number"sv;

    case ProjectedValueKind::Enum:
        return "Enum"sv;

    case ProjectedValueKind::Guid:
        return "Guid"sv;

    case ProjectedValueKind::Char:
        return "Char"sv;

    case ProjectedValueKind::String:
        return "String"sv;

    case ProjectedValueKind::Struct:
        return "Struct"sv;

    case ProjectedValueKind::Interface:
    case ProjectedValueKind::GenericCollectionInterface:
        return "Interface"sv;

    case ProjectedValueKind::AsyncAction:
        return "AsyncAction"sv;

    case ProjectedValueKind::AsyncActionWithProgress:
        return "AsyncActionWithProgress"sv;

    case ProjectedValueKind::AsyncOperation:
        return "AsyncOperation"sv;

    case ProjectedValueKind::AsyncOperationWithProgress:
        return "AsyncOperationWithProgress"sv;

    case ProjectedValueKind::Reference:
        return "Reference"sv;

    case ProjectedValueKind::ReferenceArray:
        return "ReferenceArray"sv;

    case ProjectedValueKind::Delegate:
        return "Delegate"sv;

    case ProjectedValueKind::EventHandler:
        return "EventHandler"sv;

    case ProjectedValueKind::TypedEventHandler:
        return "TypedEventHandler"sv;

    default:
        FAIL_FAST();
    }
}

bool IsTemplatedKind(ProjectedValueKind kind)
{
    switch (kind)
    {
    case ProjectedValueKind::Boolean:
    case ProjectedValueKind::Char:
    case ProjectedValueKind::Guid:
    case ProjectedValueKind::String:
    case ProjectedValueKind::AsyncAction:
        return false;

    case ProjectedValueKind::Number:
    case ProjectedValueKind::Enum:
    case ProjectedValueKind::Struct:
    case ProjectedValueKind::Interface:
    case ProjectedValueKind::GenericCollectionInterface:
    case ProjectedValueKind::Delegate:
        return true;

    case ProjectedValueKind::AsyncActionWithProgress:
    case ProjectedValueKind::AsyncOperation:
    case ProjectedValueKind::AsyncOperationWithProgress:
    case ProjectedValueKind::Reference:
    case ProjectedValueKind::ReferenceArray:
    case ProjectedValueKind::EventHandler:
    case ProjectedValueKind::TypedEventHandler:
    default:
        FAIL_FAST();
    }
}

bool IsAsyncOperation(ProjectedValueKind kind)
{
    switch (kind)
    {
    case ProjectedValueKind::AsyncAction:
    case ProjectedValueKind::AsyncActionWithProgress:
    case ProjectedValueKind::AsyncOperation:
    case ProjectedValueKind::AsyncOperationWithProgress:
        return true;

    default:
        return false;
    }
}

bool IsReference(ProjectedValueKind kind)
{
    switch (kind)
    {
    case ProjectedValueKind::Reference:
    case ProjectedValueKind::ReferenceArray:
        return true;

    default:
        return false;
    }
}

bool IsEventHandler(ProjectedValueKind kind)
{
    switch (kind)
    {
    case ProjectedValueKind::EventHandler:
    case ProjectedValueKind::TypedEventHandler:
        return true;

    default:
        return false;
    }
}

bool IsGeneric(ProjectedValueKind kind)
{
    switch (kind)
    {
    case ProjectedValueKind::AsyncActionWithProgress:
    case ProjectedValueKind::AsyncOperation:
    case ProjectedValueKind::AsyncOperationWithProgress:
    case ProjectedValueKind::Reference:
    case ProjectedValueKind::ReferenceArray:
    case ProjectedValueKind::GenericCollectionInterface:
    case ProjectedValueKind::EventHandler:
    case ProjectedValueKind::TypedEventHandler:
        return true;

    default:
        return false;
    }
}

bool IsValueType(ProjectedValueKind kind)
{
    switch (kind)
    {
    case ProjectedValueKind::Boolean:
    case ProjectedValueKind::Number:
    case ProjectedValueKind::Enum:
    case ProjectedValueKind::Guid:
    case ProjectedValueKind::Char:
    case ProjectedValueKind::String:
    case ProjectedValueKind::Struct:
        return true;

    default:
        return false;
    }
}

std::vector<winmd::reader::TypeSig> GetGenericParamTypeSigs(const winmd::reader::TypeSig& typeSig)
{
    std::vector<winmd::reader::TypeSig> result;
    result.reserve(2);

    std::visit([&](auto&& type)
    {
        using T = std::decay_t<decltype(type)>;
        if constexpr (std::is_same_v<T, winmd::reader::GenericTypeInstSig>)
        {
            const auto args = type.GenericArgs();
            result.insert(result.begin(), args.first, args.second);
        }
        else
        {
            FAIL_FAST();
        }
    }, typeSig.Type());

    return result;
}

std::vector<std::string> GetValueConverters(FileWriter& writer, const std::vector<winmd::reader::TypeSig>& paramTypeSigs, bool toValue,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>* specializedInterfaceReferences)
{
    std::vector<std::string> result;
    result.reserve(paramTypeSigs.size());

    for (const auto& typeSig : paramTypeSigs)
    {
        if (toValue)
        {
            result.push_back(GetNativeToValueConverter(writer, typeSig, specializedInterfaceReferences));
        }
        else
        {
            result.push_back(GetValueToNativeConverter(writer, typeSig, false /*isInout*/, specializedInterfaceReferences));
        }
    }

    return result;
}

std::string GetNativeToValueConverter(FileWriter& writer, const winmd::reader::TypeSig& typeSig, std::map<std::string, std::vector<winmd::reader::TypeSig>>* specializedInterfaceReferences)
{
    const auto kind = GetProjectedValueKind(typeSig);
    const auto converterName = GetConverterName(kind);

    if (IsGeneric(kind))
    {
        const auto specializedInterfaceName = writer.WriteTemp("%"sv, typeSig);
        auto paramTypeSigs = GetGenericParamTypeSigs(typeSig);

        // Even if we are not using it for an AsyncOperation or Reference/ReferenceArray, we should recursively evaluate to ensure
        // we account for all specialized interfaces in 'specializedInterfaceReferences'.
        const auto genericsParamConverters = GetValueConverters(writer, paramTypeSigs, !IsEventHandler(kind) /*toValue*/, specializedInterfaceReferences);

        if (specializedInterfaceReferences && (specializedInterfaceReferences->find(specializedInterfaceName) == specializedInterfaceReferences->end()))
        {
            specializedInterfaceReferences->emplace(specializedInterfaceName, std::move(paramTypeSigs));
        }

        if (IsAsyncOperation(kind) || IsReference(kind) || IsEventHandler(kind))
        {
            return writer.WriteTemp("Convert%ToValue<%, %>"sv, converterName, specializedInterfaceName, Writer::BindList(", "sv, genericsParamConverters));
        }
    }

    auto typeConverter = IsTemplatedKind(kind) ? writer.WriteTemp("Convert%ToValue<%>"sv, converterName, typeSig.Type()) : writer.WriteTemp("Convert%ToValue"sv, converterName);
    if (typeSig.is_szarray())
    {
        return writer.WriteTemp("ConvertComArrayToValue<%, %>"sv, typeSig.Type(), typeConverter);
    }
    return typeConverter;
}

std::string GetValueToNativeConverter(FileWriter& writer, const winmd::reader::TypeSig& typeSig, bool isInout,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>* specializedInterfaceReferences)
{
    FAIL_FAST_IF(isInout && !typeSig.is_szarray());

    auto kind = GetProjectedValueKind(typeSig);
    if (IsGeneric(kind))
    {
        const auto specializedInterfaceName = writer.WriteTemp("%"sv, typeSig);
        auto paramTypeSigs = GetGenericParamTypeSigs(typeSig);

        // Even if we are not using it for an AsyncOperation or Reference/ReferenceArray, we should recursively evaluate to ensure
        // we account for all specialized interfaces in 'specializedInterfaceReferences'.
        const auto genericsParamConverters = GetValueConverters(writer, paramTypeSigs, IsEventHandler(kind) /*toValue*/, specializedInterfaceReferences);

        if (specializedInterfaceReferences && (specializedInterfaceReferences->find(specializedInterfaceName) == specializedInterfaceReferences->end()))
        {
            specializedInterfaceReferences->emplace(specializedInterfaceName, std::move(paramTypeSigs));
        }

        if (IsReference(kind) || IsEventHandler(kind))
        {
            return writer.WriteTemp("ConvertValueTo%<%, %>"sv, GetConverterName(kind), specializedInterfaceName, Writer::BindList(", ", genericsParamConverters));
        }
    }

    if (IsAsyncOperation(kind))
    {
        // Treat as Interface for the purpose of ConvertValueToNative.
        kind = ProjectedValueKind::Interface;
    }

    const auto converterName = GetConverterName(kind);
    const auto converterTemplateArgs = IsTemplatedKind(kind) ? writer.WriteTemp("<%>"sv, typeSig.Type()) : "";

    if (typeSig.is_szarray())
    {
        if (isInout)
        {
            return writer.WriteTemp("ConvertValueToWriteOnlyArrayView<%, Convert%ToValue%>"sv, typeSig.Type(), converterName, converterTemplateArgs);
        }
        return writer.WriteTemp("ConvertValueToReadOnlyArrayView<%, ConvertValueTo%%>"sv, typeSig.Type(), converterName, converterTemplateArgs);
    }
    return writer.WriteTemp("ConvertValueTo%%"sv, converterName, converterTemplateArgs);
}
