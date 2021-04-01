#pragma once
#include "HierarchyDerived.g.h"
#include "HierarchyBase.h"

namespace winrt::TestComponent::implementation 
{
    struct HierarchyDerived : HierarchyDerivedT<HierarchyDerived, HierarchyBase>
    {
        HierarchyDerived() = default;
        winrt::hstring OverriddenHierarchyBaseMethod();
        winrt::hstring HierarchyDerivedMethod();
    };
}

namespace winrt::TestComponent::factory_implementation
{
    struct HierarchyDerived : HierarchyDerivedT<HierarchyDerived, implementation::HierarchyDerived>
    {
    };
}