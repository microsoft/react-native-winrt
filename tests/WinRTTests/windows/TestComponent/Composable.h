#pragma once
//#include "Composable.g.h"
//
//namespace winrt::TestComponent::implementation
//{
//    struct Composable : ComposableT<Composable>
//    {
//        Composable() = default;
//
//        Composable(int32_t init);
//        int32_t Value();
//        void Value(int32_t);
//
//        int32_t m_value{};
//
//        int32_t One();
//        int32_t Two();
//        int32_t Three();
//        int32_t Four();
//
//        static int32_t ExpectComposable(TestComponent::Composable const& t);
//        static int32_t ExpectRequiredOne(TestComponent::IRequiredOne const& t);
//        static int32_t ExpectRequiredTwo(TestComponent::IRequiredTwo const& t);
//        static int32_t ExpectRequiredThree(TestComponent::IRequiredThree const& t);
//        static int32_t ExpectRequiredFour(TestComponent::IRequiredFour const& t);
//    };
//}
//
//namespace winrt::TestComponent::factory_implementation
//{
//    struct Composable : ComposableT<Composable, implementation::Composable>
//    {
//    };
//}
