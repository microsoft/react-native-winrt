#pragma once
#include "ProjectedEvent.h"

namespace WinRTTurboModule
{
    struct ProjectionsContext;
    
    // The purpose of this component is to ensure that event registrations can out-live a given instance of ProjectedRuntimeClassInstance
    // or ProjectedInterfaceInstance but can still be unregistered later. That is, a WinRT object might outlive the HostObject that we
    // return to JS as its lifetime may be preserved by a native component and the API usage assumes this. So if one queries an object
    // from a static activation factory, lets the garbage collector free the ProjectedRuntimeClassInstance (ProjectedRuntimeClassInstanceFactory
    // only holds weak references) and then queries again later obtaining a new ProjectedRuntimeClassInstance corresponding to the same
    // WinRT object, it should be able to unregister any extant event handlers all the same. If we automatically unregistered for
    // events whenever the HostObject is freed, it would impose a requirement to preserve the object unlike what exists in EdgeHTML.

    struct ProjectedEventRegistrar
    {
        ProjectedEventRegistrar(ProjectionsContext& context);

        // WARNING: Use default IInspectable implementation (i.e., that returned by QueryInterface) rather than static cast on another
        //          interface derived from IInspectable as we use the ABI pointer to key off of. This key should ideally match that
        //          used for caching with ProjectedRuntimeClassInstanceFactory::Get.
        std::shared_ptr<ProjectedEventInstance> Get(const winrt::Windows::Foundation::IInspectable& inspectable, const std::shared_ptr<IProjectedEventBase>& event);

    private:
        // TODO: How should we garbage collect the map? Should we store wint::weak_ref here and periodically sweap like the code tries to
        //       do below opportunistically? Another option might be to try releasing on ProjectedRuntimeClassInstance releasing pointers
        //       as it will often be the last holder of strong references, but there is no guarantee since per above we are using something
        //       like ProjectedEventRegistrar because WinRT objects and outstanding event handlers may outlive the ProjectedRuntimeClassInstance.
        void CleanupMapIfApplicable();

        struct Entry
        {
            winrt::weak_ref<winrt::Windows::Foundation::IInspectable> WeakRef;
            std::vector<std::shared_ptr<ProjectedEventInstance>> EventInstances;
        };

        ProjectionsContext& m_context;
        std::unordered_map<void*, Entry> m_map;
        uint64_t m_lastCleanUp{ GetTickCount64() };
        uint32_t m_requestsSinceCleanup{};
    };
}