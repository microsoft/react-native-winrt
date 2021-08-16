#pragma once
#include "CommandReader.h"
#include "TypeFilter.h"

struct Settings
{
    Settings(const CommandReader& commandReader);

    const std::filesystem::path RnWinRTOutputFolder;
    const std::filesystem::path NodeWinRTOutputFolder;
    const std::filesystem::path TypescriptOutputFolder;
    winmd::reader::cache Cache;
    const TypeFilter Filter;
    const std::string PchFileName;
    const bool FilterToAllowForWeb;
    const bool IncludeDeprecated;
    const bool IncludeWebHostHidden;
    const bool Verbose;
};

bool is_type_allowed(const Settings& settings, const winmd::reader::TypeDef& typeDef);
bool is_namespace_allowed(const Settings& settings, const winmd::reader::cache::namespace_members& members);
bool is_method_allowed(const Settings& settings, const winmd::reader::MethodDef& methodDef);
