#pragma once
#include "CommandReader.h"
#include "TypeFilter.h"

struct Settings
{
    Settings(const CommandReader& commandReader);

    const std::filesystem::path OutputFolder;
    winmd::reader::cache Cache;
    const TypeFilter Filter;
    const std::string PchFileName;
    const bool FilterToAllowForWeb;
    const bool IncludeDeprecated;
    const bool IncludeWebHostHidden;
    const bool Verbose;
};