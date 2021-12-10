// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once
#include "HierarchyBase.h"
#include "HierarchyDerived.g.h"

namespace winrt::TestComponent::implementation
{
    struct HierarchyDerived : HierarchyDerivedT<HierarchyDerived, HierarchyBase>
    {
        HierarchyDerived(winrt::hstring name);
        winrt::hstring HierarchyDerivedMethod();
        winrt::hstring OverloadedHierarchyBaseMethod(winrt::hstring param1, winrt::hstring param2);
        winrt::hstring OverriddenHierarchyBaseMethodOverride();
    };
}

namespace winrt::TestComponent::factory_implementation
{
    struct HierarchyDerived : HierarchyDerivedT<HierarchyDerived, implementation::HierarchyDerived>
    {
    };
}