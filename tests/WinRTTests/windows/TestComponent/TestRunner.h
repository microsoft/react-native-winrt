#pragma once
#include "TestRunner.g.h"

namespace winrt::TestComponent::implementation
{
    struct TestRunner
    {
        TestRunner() = default;

#if 0
        static void TestProducer(ITests const& callee);
        static uint32_t TestConsumer(TestHandler const& caller);
        static void TestSelf();
#endif
        static ITests MakeTests();
#if 0
        static Windows::Foundation::Collections::IVector<int32_t> CreateInt32Vector();
        static Windows::Foundation::Collections::IVector<hstring> CreateStringVector();
        static Windows::Foundation::Collections::IVector<Windows::Foundation::IStringable> CreateStringableVector();
        static Windows::Foundation::TimeSpan CreateTimeSpan(uint32_t milliseconds);
        static Windows::Foundation::IAsyncAction CreateAsyncAction(uint32_t milliseconds);
        static hstring ExpectObject(Windows::Foundation::IInspectable const& value);
#endif
    };
}

namespace winrt::TestComponent::factory_implementation
{
    struct TestRunner : TestRunnerT<TestRunner, implementation::TestRunner>
    {
    };
}
