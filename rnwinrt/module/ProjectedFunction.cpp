#pragma once
#include "pch.h"

#include "ProjectedFunction.h"

namespace WinRTTurboModule
{
    std::shared_ptr<ProjectedFunction> ProjectedFunction::Create(std::string_view name, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedFunction>(name, initializer);
    }

    std::shared_ptr<ProjectedFunction> ProjectedFunction::Create(
        std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation)
    {
        return std::make_shared<ProjectedFunction>(name, std::move(implementation));
    }

    ProjectedFunction::ProjectedFunction(std::string_view name, InitializerFunction initializer) :
        m_name(name), m_initializer(initializer)
    {
    }

    ProjectedFunction::ProjectedFunction(
        std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation) :
        m_name(name),
        m_initializer(nullptr), m_maxArity(implementation->Arity()), m_isInitialized(true)
    {
        m_overloads.emplace(implementation->Arity(), std::move(implementation));
    }

    std::string_view ProjectedFunction::Name() const noexcept
    {
        return m_name;
    }

    bool ProjectedFunction::CanInvoke(uint16_t arity)
    {
        EnsureInitialized();
        return m_overloads.find(arity) != m_overloads.end();
    }

    std::shared_ptr<IProjectedFunctionOverloadBase>& ProjectedFunction::GetOverload(
        jsi::Runtime& runtime, uint16_t numArgs)
    {
        EnsureInitialized();
        const auto it = m_overloads.find(numArgs);
        if (it == m_overloads.end())
        {
            throw jsi::JSError(runtime, "TypeError: No function overload for " + std::string(m_name) + " exists with " +
                                            std::to_string(numArgs) + " args");
        }
        return it->second;
    }

    void ProjectedFunction::EnsureInitialized()
    {
        if (m_overloads.empty())
        {
            // PERF: Consider optimizing the trivial case of a single declaration with no overloads.
            auto overloads = m_initializer();
            for (auto&& overload : overloads)
            {
                const auto arity = overload->Arity();
                m_maxArity = std::max(m_maxArity, arity);

                // JS only supports overloading by the number of function params so code generation should either
                // have omitted all ambiguous functions or picked one to use. If not we will take the last one.
                WI_ASSERT(m_overloads.find(arity) == m_overloads.end());
                m_overloads[arity] = std::move(overload);
            }
            m_isInitialized = true;
        }
    }
}