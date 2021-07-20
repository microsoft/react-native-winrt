#include "pch.h"

#include "MetadataHelpers.h"
#include "Settings.h"

using namespace winmd::reader;

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
    RnWinRTOutputFolder(std::filesystem::path(commandReader.Value("output"sv)) / "rnwinrt"sv),
    NodeWinRTOutputFolder(std::filesystem::path(commandReader.Value("output"sv)) / "nodewinrt"sv),
    TypescriptOutputFolder(std::filesystem::path(commandReader.Value("tsoutput"sv))), Cache(GetInput(commandReader)),
    Filter(GetInclude(commandReader), GetExclude(commandReader)), PchFileName(commandReader.Value("pch"sv, "pch.h"sv)),
    FilterToAllowForWeb(commandReader.Exists("allowforwebexclusively"sv)),
    IncludeDeprecated(commandReader.Exists("deprecatedincluded"sv)),
    IncludeWebHostHidden(commandReader.Exists("webhosthiddenincluded"sv)), Verbose(commandReader.Exists("verbose"sv))
{
}

bool is_type_allowed(const Settings& settings, const TypeDef& typeDef, bool isClass)
{
    if (!settings.Filter.Includes(typeDef))
    {
        return false;
    }

    if (isClass && settings.FilterToAllowForWeb && !is_allow_for_web(typeDef))
    {
        return false;
    }

    if (!settings.IncludeDeprecated && is_deprecated(typeDef))
    {
        return false;
    }

    if (!settings.IncludeWebHostHidden && is_web_host_hidden(typeDef))
    {
        // Special-case Windows.Foundation.IPropertyValueStatics which should not be WebHostHidden as it breaks basic
        // language features by omission.
        if (typeDef.TypeNamespace() == foundation_namespace)
        {
            if (isClass ? (typeDef.TypeName() == "PropertyValue"sv) : (typeDef.TypeName() == "IPropertyValueStatics"sv))
            {
                return true;
            }
        }
        return false;
    }

    return true;
}

bool is_namespace_allowed(const Settings& settings, const cache::namespace_members& members)
{
    for (const auto& typeDef : members.interfaces)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.classes)
    {
        if (is_type_allowed(settings, typeDef, true /*isClass*/))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.enums)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.structs)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    for (const auto& typeDef : members.delegates)
    {
        if (is_type_allowed(settings, typeDef))
        {
            return true;
        }
    }

    return false;
}

bool is_method_allowed(const Settings& settings, const MethodDef& methodDef)
{
    if (!settings.IncludeDeprecated && has_attribute(methodDef, metadata_namespace, "DeprecatedAttribute"sv))
    {
        return false;
    }

    if (!settings.IncludeWebHostHidden && has_attribute(methodDef, metadata_namespace, "WebHostHiddenAttribute"sv))
    {
        return false;
    }

    return true;
}
