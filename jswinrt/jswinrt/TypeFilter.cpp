#include "pch.h"

#include "MetadataHelpers.h"
#include "TypeFilter.h"

bool TypeFilter::Includes(const winmd::reader::TypeDef& type) const
{
    return Includes(type.TypeNamespace(), type.TypeName());
}

bool TypeFilter::Includes(const std::string_view& type) const
{
    auto position = type.find_last_of('.');
    return Includes(type.substr(0, position), type.substr(position + 1));
}

bool TypeFilter::Includes(const std::vector<winmd::reader::TypeDef>& types) const
{
    if (m_rules.empty())
    {
        return m_defaultInclude;
    }

    for (auto&& type : types)
    {
        if (Includes(type.TypeNamespace(), type.TypeName()))
        {
            return true;
        }
    }

    return m_defaultInclude;
}

bool TypeFilter::Includes(const winmd::reader::cache::namespace_members& members) const
{
    if (m_rules.empty())
    {
        return m_defaultInclude;
    }

    for (auto&& type : members.types)
    {
        if (Includes(type.second.TypeNamespace(), type.second.TypeName()))
        {
            return true;
        }
    }

    return m_defaultInclude;
}

bool TypeFilter::Includes(std::string_view const& typeNamespace, std::string_view const& typeName) const noexcept
{
    if (m_rules.empty())
    {
        return m_defaultInclude;
    }

    for (auto&& rule : m_rules)
    {
        if (Match(typeNamespace, typeName, rule.first))
        {
            return rule.second;
        }
    }

    return m_defaultInclude;
}

bool TypeFilter::Match(
    const std::string_view& typeNamespace, const std::string_view& typeName, const std::string_view& match) noexcept
{
    if (match.size() <= typeNamespace.size())
    {
        return starts_with(typeNamespace, match);
    }

    if (!starts_with(match, typeNamespace))
    {
        return false;
    }

    if (match[typeNamespace.size()] != '.')
    {
        return false;
    }

    return starts_with(typeName, match.substr(typeNamespace.size() + 1));
}
