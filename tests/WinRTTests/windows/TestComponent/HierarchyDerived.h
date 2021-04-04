#pragma once
#include "HierarchyDerived.g.h"
#include "HierarchyBase.h"

namespace winrt::TestComponent::implementation 
{
    struct HierarchyDerived : HierarchyDerivedT<HierarchyDerived, HierarchyBase>
    {
        HierarchyDerived(winrt::hstring name);
        winrt::hstring OverriddenHierarchyBaseMethod();
        winrt::hstring HierarchyDerivedMethod();
        winrt::hstring OverloadedHierarchyBaseMethod(winrt::hstring param1, winrt::hstring param2);
    };
}

namespace winrt::TestComponent::factory_implementation
{
    struct HierarchyDerived : HierarchyDerivedT<HierarchyDerived, implementation::HierarchyDerived>
    {
    };
}