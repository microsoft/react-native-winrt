#include "pch.h"
#include "HierarchyBase.h"
#include "HierarchyBase.g.cpp"

namespace winrt::TestComponent::implementation
{

    winrt::hstring HierarchyBase::NonOverriddenHierarchyBaseMethod()
    { 
        return L"HierarchyBase.NonOverriddenHierarchyBaseMethod";
    }

    winrt::hstring HierarchyBase::OverriddenHierarchyBaseMethod()
    {
        return L"HierarchyBase.OverriddenHierarchyBaseMethod";
    }
}