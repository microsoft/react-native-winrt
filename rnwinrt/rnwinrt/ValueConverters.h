#pragma once
#include "MetadataHelpers.h"
#include "Writer.h"

enum class ProjectedValueKind
{
    Unknown,
    Boolean,
    Number,
    Enum,
    Guid,
    Char,
    String,
    Struct,
    Interface,
    AsyncAction,
    AsyncActionWithProgress,
    AsyncOperation,
    AsyncOperationWithProgress,
    Reference,
    ReferenceArray,
    GenericCollectionInterface,
    Delegate,
    EventHandler,
    TypedEventHandler,
};

ProjectedValueKind GetProjectedValueKind(const winmd::reader::TypeSig& typeSig);
bool IsValueType(ProjectedValueKind kind);

std::string GetNativeToValueConverter(FileWriter& writer, const winmd::reader::TypeSig& typeSig,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>* specializedInterfaceReferences);
std::string GetValueToNativeConverter(FileWriter& write, const winmd::reader::TypeSig& typeSig, bool isInout,
    std::map<std::string, std::vector<winmd::reader::TypeSig>>* specializedInterfaceReferences);