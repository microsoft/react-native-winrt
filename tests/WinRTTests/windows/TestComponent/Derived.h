#pragma once

// NOTE: Testing not yet complete; kept here for easier testing in the future
#if 0

#include "Composable.h"
#include "Derived.g.h"

namespace winrt::TestComponent::implementation
{
   struct Derived : DerivedT<Derived, TestComponent::implementation::Composable>
   {
       Derived() = default;
   };
}
namespace winrt::TestComponent::factory_implementation
{
   struct Derived : DerivedT<Derived, implementation::Derived>
   {
   };
}

#endif
