#pragma once
#include "HierarchyBase.g.h"

namespace winrt::TestComponent::implementation 
{
    struct HierarchyBase : HierarchyBaseT<HierarchyBase>
    {
        HierarchyBase() = default;
        winrt::hstring NonOverriddenHierarchyBaseMethod();
        winrt::hstring OverriddenHierarchyBaseMethod();        
        winrt::hstring OverloadedHierarchyBaseMethod(winrt::hstring param1);
        static winrt::hstring StaticHierarchyBaseMethod();
        winrt::hstring IHierarchyV2ContractMethod();
        void InaccessibleHierarchyBaseMethod();
        virtual winrt::hstring OverriddenHierarchyBaseMethodOverride();
    };
}

namespace winrt::TestComponent::factory_implementation
{
    struct HierarchyBase : HierarchyBaseT<HierarchyBase, implementation::HierarchyBase>
    {
    };
}