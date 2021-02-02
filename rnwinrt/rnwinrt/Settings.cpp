#include "pch.h"

#include "Settings.h"

namespace
{
    std::vector<std::string> GetInput(const CommandReader& commandReader)
    {
        auto input = commandReader.Files("input"sv, winmd::reader::database::is_database);
        auto reference = commandReader.Files("reference"sv, winmd::reader::database::is_database);

        std::vector<std::string> result;
        result.resize(input.size() + reference.size());
        std::set_union(std::make_move_iterator(input.begin()), std::make_move_iterator(input.end()),
            std::make_move_iterator(reference.begin()), std::make_move_iterator(reference.end()), result.begin());

        return result;
    }

    std::set<std::string> GetInclude(const CommandReader& commandReader)
    {
        std::set<std::string> includes;

        for (auto&& include : commandReader.Values("include"sv))
        {
            includes.insert(include);
        }

        for (auto&& include : commandReader.Values("filter"sv))
        {
            includes.insert(include);
        }

        return includes;
    }

    std::set<std::string> GetExclude(const CommandReader& commandReader)
    {
        std::set<std::string> excludes;
        for (auto&& exclude : commandReader.Values("exclude"sv))
        {
            excludes.insert(exclude);
        }
        return excludes;
    }
}

Settings::Settings(const CommandReader& commandReader) :
    OutputFolder(std::filesystem::path(commandReader.Value("output"sv)) / "rnwinrt"sv),
    TypescriptOutputFolder(std::filesystem::path(commandReader.Value("tsoutput"sv))),
    Cache(GetInput(commandReader)),
    Filter(GetInclude(commandReader), GetExclude(commandReader)), PchFileName(commandReader.Value("pch"sv, "pch.h"sv)),
    FilterToAllowForWeb(commandReader.Exists("allowforwebexclusively"sv)),
    IncludeDeprecated(commandReader.Exists("deprecatedincluded"sv)),
    IncludeWebHostHidden(commandReader.Exists("webhosthiddenincluded"sv)), Verbose(commandReader.Exists("verbose"sv))
{
    std::filesystem::create_directories(OutputFolder);
}