#include "pch.h"

#include "HierarchyDerived.g.cpp"
#include "HierarchyDerived.h"

namespace winrt::TestComponent::implementation
{
    HierarchyDerived::HierarchyDerived(winrt::hstring name)
    {
    }

    winrt::hstring HierarchyDerived::OverriddenHierarchyBaseMethodOverride()
    {
        return L"HierarchyDerived.OverriddenHierarchyBaseMethod";
    }

    winrt::hstring HierarchyDerived::HierarchyDerivedMethod()
    {
        return L"HierarchyDerived.HierarchyDerivedMethod";
    }

    winrt::hstring HierarchyDerived::OverloadedHierarchyBaseMethod(winrt::hstring param1, winrt::hstring param2)
    {
        return L"HierarchyDerived.OverloadedHierarchyBaseMethod";
    }

}