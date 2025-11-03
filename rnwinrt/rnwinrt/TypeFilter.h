// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

// winmd::reader::filter has a rather unfortunate design that if no include filters are specified and only includes,
// it treats the outcome as an exclude even if no exclude filter matches. That is it doesn't assume a default include
// if only excludes are specified, where-as the opposite does correctly imply a default exclude. Hence, we would have
// to generate an include list that includes the full set of types which is extremely inefficient for the sake of
// filtering. C++/WinRT has this problem, but depending on usage, it only applies it only has to generate include
// rules for the relatively smaller set of types it generates component implementations for rather than the full set
// of references. In the case of RN/WinRT it would be applying the filter to the union of inputs and references
// making such a filter inadvisable. Hence this forks the original implementation and assume default include with only
// excludes.

struct TypeFilter
{
    TypeFilter() noexcept = default;

    template <typename T>
    TypeFilter(const T& includes, const T& excludes)
    {
        // If there are no rules, or only exclude rules, we should assuming including by default. Where if we have
        // include rules, regardless of exclude rules, we should exclude by default.
        m_defaultInclude = includes.empty();

        m_rules.reserve(includes.size() + excludes.size());

        for (auto&& include : includes)
        {
            m_rules.push_back({ include, true });
        }

        for (auto&& exclude : excludes)
        {
            m_rules.push_back({ exclude, false });
        }

        // Sort rules so that:
        // 1. All exclude (false) rules come before include (true) rules.
        // 2. Within each group (false or true), rules are ordered alphabetically by their string.
        // This groups exclusion patterns first (for early rejection) while keeping deterministic order.
        std::sort(m_rules.begin(), m_rules.end(), [](auto const& lhs, auto const& rhs) noexcept {
            if (lhs.second != rhs.second)
            {
                // false (exclude) before true (include)
                return lhs.second == false && rhs.second == true;
            }
            return lhs.first < rhs.first; // alphabetical within same include/exclude group
        });
    }

    bool Includes(const winmd::reader::TypeDef& type) const;
    bool Includes(const std::string_view& type) const;
    bool Includes(const std::vector<winmd::reader::TypeDef>& types) const;
    bool Includes(const winmd::reader::cache::namespace_members& members) const;

private:
    bool Includes(std::string_view const& typeNamespace, std::string_view const& typeName) const noexcept;
    static bool Match(const std::string_view& typeNamespace, const std::string_view& typeName,
        std::string_view const& match) noexcept;

    std::vector<std::pair<std::string, bool>> m_rules;
    bool m_defaultInclude = true;
};
