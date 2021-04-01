#include "pch.h"
#include "HierarchyDerived.h"
#include "HierarchyDerived.g.cpp"

namespace winrt::TestComponent::implementation
{

    winrt::hstring HierarchyDerived::OverriddenHierarchyBaseMethod()
    { 
        return L"HierarchyDerived.OverriddenHierarchyBaseMethod";
    }

    winrt::hstring HierarchyDerived::HierarchyDerivedMethod()
    {
        return L"HierarchyDerived.HierarchyDerivedMethod";
    }

}