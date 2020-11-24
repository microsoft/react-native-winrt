#include "pch.h"

#include "ProjectedNamespace.h"
#include "ProjectedValueConverters.h"

namespace WinRTTurboModule
{
    std::shared_ptr<IProjectedValueProvider> ProjectedNamespace::Create(const std::string_view& name,
        const std::shared_ptr<ProjectionsContext>& context, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedHostObjectProvider>(
            name, std::make_shared<ProjectedNamespace>(name, context, initializer));
    }

    ProjectedNamespace::ProjectedNamespace(const std::string_view& name,
        const std::shared_ptr<ProjectionsContext>& context, InitializerFunction initializer) :
        m_name(name),
        m_context(context), m_initializer(initializer)
    {
    }

    jsi::Value ProjectedNamespace::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        const auto it = m_propertyMap.find(propName.utf8(runtime));
        if (it != m_propertyMap.end())
        {
            return it->second->Value(runtime);
        }

        return jsi::Value::undefined();
    }

    void ProjectedNamespace::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value)
    {
        throw jsi::JSError(runtime,
            "TypeError: Cannot assign to property '" + propName.utf8(runtime) + "' to a projected WinRT namespace");
    }

    std::vector<jsi::PropNameID> ProjectedNamespace::getPropertyNames(jsi::Runtime& runtime)
    {
        EnsureInitialized();

        // PERF: It would be nice if these were copyable but PropNameID is only moveable. It might be possible to store
        // and reuse jsi::String, which might avoid deep copies. Could we return an empty vector like TurboModule given
        // it inherits it from HostObject?
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(m_propertyMap.size());

        for (const auto& property : m_propertyMap)
        {
            propNames.push_back(CreatePropNameID(runtime, property.first));
        }

        return propNames;
    }

    void ProjectedNamespace::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto providers = m_initializer(m_context);
            m_propertyMap.reserve(providers.size());
            for (auto&& provider : providers)
            {
                m_propertyMap[provider->Name()] = std::move(provider);
            }
            m_isInitialized = true;
        }
    }
}