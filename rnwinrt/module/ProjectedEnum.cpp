#include "pch.h"
#include "ProjectedEnum.h"
#include "JsiHelpers.h"

namespace WinRTTurboModule
{
    std::shared_ptr<IProjectedValueProvider> ProjectedEnum::Create(const std::string_view& name, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedHostObjectProvider>(name, std::make_shared<ProjectedEnum>(name, initializer));
    }

    ProjectedEnum::ProjectedEnum(const std::string_view& name, InitializerFunction initializer)
        : m_name(name), m_initializer(initializer)
    {
    }

    jsi::Value ProjectedEnum::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        const auto it = m_propertyMap.find(propName.utf8(runtime));
        if (it != m_propertyMap.end())
        {
            // jsi::Value uses double for int and float constructors and there doesn't appear to be BigInt support yet.
            return jsi::Value(static_cast<double>(it->second));
        }

        return jsi::Value::undefined();
    }

    void ProjectedEnum::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value)
    {
        throw jsi::JSError(runtime, std::string("TypeError: Cannot assign to property '") + propName.utf8(runtime) + "' to a projected WinRT enum");
    }

    std::vector<jsi::PropNameID> ProjectedEnum::getPropertyNames(jsi::Runtime& runtime)
    {
        // PERF: It would be nice if these were copyable but PropNameID is only moveable. It might be possible to store and reuse jsi::String,
        // which might avoid deep copies. Could we return an empty vector like TurboModule given it inherits it from HostObject?
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(m_propertyMap.size());

        for (const auto& property : m_propertyMap)
        {
            propNames.push_back(CreatePropNameID(runtime, property.first));
        }

        return propNames;
    }

    void ProjectedEnum::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            m_propertyMap = m_initializer();
            m_isInitialized = true;
        }
    }
}