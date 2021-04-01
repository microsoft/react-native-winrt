#pragma once
#include "HierarchyBase.g.h"

namespace winrt::TestComponent::implementation 
{
    struct HierarchyBase : HierarchyBaseT<HierarchyBase>
    {
        HierarchyBase() = default;
        winrt::hstring NonOverriddenHierarchyBaseMethod();
        virtual winrt::hstring OverriddenHierarchyBaseMethod();
    };
}

namespace winrt::TestComponent::factory_implementation
{
    struct HierarchyBase : HierarchyBaseT<HierarchyBase, implementation::HierarchyBase>
    {
    };
}