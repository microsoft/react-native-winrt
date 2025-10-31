#pragma once

#include "base.h"

namespace rnwinrt::namespaces::TestComponent
{
    extern const static_namespace_data data;
}

namespace rnwinrt::enums::TestComponent
{
    namespace TestEnum
    {
        extern const static_enum_data data;
    }
}

namespace rnwinrt::classes::TestComponent
{
    namespace HierarchyBase
    {
        extern const static_activatable_class_data data;
    }

    namespace HierarchyDerived
    {
        extern const static_activatable_class_data data;
    }

    namespace StaticOnlyTest
    {
        extern const static_class_data data;
    }

    namespace Test
    {
        extern const static_activatable_class_data data;
    }

    namespace TestObject
    {
        extern const static_activatable_class_data data;
    }
}

namespace rnwinrt::interfaces::TestComponent
{
    namespace IHierarchyBase
    {
        extern const static_interface_data data;
    }

    namespace IHierarchyBaseFactory
    {
        extern const static_interface_data data;
    }

    namespace IHierarchyDerived
    {
        extern const static_interface_data data;
    }

    namespace IHierarchyDerivedFactory
    {
        extern const static_interface_data data;
    }

    namespace IHierarchyV2Contract
    {
        extern const static_interface_data data;
    }

    namespace ITest
    {
        extern const static_interface_data data;
    }

    namespace ITest2
    {
        extern const static_interface_data data;
    }

    namespace ITestInterface
    {
        extern const static_interface_data data;
    }

    namespace ITestObject
    {
        extern const static_interface_data data;
    }
}
