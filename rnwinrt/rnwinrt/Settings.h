// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once
#include "CommandReader.h"
#include "TypeFilter.h"

struct Settings
{
    Settings(const CommandReader& commandReader);

    const std::filesystem::path OutputFolder;
    const std::filesystem::path JsOutputFolder;
    const std::filesystem::path TypescriptOutputFolder;
    winmd::reader::cache Cache;
    const TypeFilter Filter;
    const std::string PchFileName;
    const bool FilterToAllowForWeb;
    const bool IncludeDeprecated;
    const bool IncludeWebHostHidden;
    const bool GenerateModule;
    const bool Verbose;
};

bool is_type_allowed(const Settings& settings, const winmd::reader::TypeDef& typeDef);
bool is_namespace_allowed(const Settings& settings, const winmd::reader::cache::namespace_members& members);
bool is_method_allowed(const Settings& settings, const winmd::reader::MethodDef& methodDef);
