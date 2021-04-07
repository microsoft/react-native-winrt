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
        return OverriddenHierarchyBaseMethodOverride();
    }

    winrt::hstring HierarchyBase::OverriddenHierarchyBaseMethodOverride()
    {
        return L"HierarchyBase.OverriddenHierarchyBaseMethod";
    }

    winrt::hstring HierarchyBase::OverloadedHierarchyBaseMethod(winrt::hstring param1)
    {
        return L"HierarchyBase.OverloadedHierarchyBaseMethod";
    }
    
    winrt::hstring HierarchyBase::StaticHierarchyBaseMethod()
    {
        return L"HierarchyBase.StaticHierarchyBaseMethod";
    }
    
    winrt::hstring HierarchyBase::IHierarchyV2ContractMethod()
    {
        return L"HierarchyBase.IHierarchyV2ContractMethod";
    }
}