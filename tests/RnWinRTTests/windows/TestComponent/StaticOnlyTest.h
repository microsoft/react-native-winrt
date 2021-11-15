// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#pragma once

#include "StaticOnlyTest.g.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Foundation.h"

namespace winrt::TestComponent::implementation
{
    struct StaticOnlyTest
    {
        StaticOnlyTest() = delete;
        static bool BoolProperty();
        static void BoolProperty(bool value);
        static char16_t CharProperty();
        static void CharProperty(char16_t value);
        static uint8_t U8Property();
        static void U8Property(uint8_t value);
        static uint16_t U16Property();
        static void U16Property(uint16_t value);
        static uint32_t U32Property();
        static void U32Property(uint32_t value);
        static uint64_t U64Property();
        static void U64Property(uint64_t value);
        static int16_t S16Property();
        static void S16Property(int16_t value);
        static int32_t S32Property();
        static void S32Property(int32_t value);
        static int64_t S64Property();
        static void S64Property(int64_t value);
        static float F32Property();
        static void F32Property(float value);
        static double F64Property();
        static void F64Property(double value);
        static hstring StringProperty();
        static void StringProperty(hstring const& value);
        static winrt::guid GuidProperty();
        static void GuidProperty(winrt::guid const& value);
        static TestEnum EnumProperty();
        static void EnumProperty(TestEnum const& value);
        static NumericTypes NumericsStructProperty();
        static void NumericsStructProperty(NumericTypes const& value);
        static StringTypes StringsStructProperty();
        static void StringsStructProperty(StringTypes const& value);
        static BooleanTypes BooleansStructProperty();
        static void BooleansStructProperty(BooleanTypes const& value);
        static CompositeType CompositeStructProperty();
        static void CompositeStructProperty(CompositeType const& value);
        static Windows::Foundation::IReference<bool> RefBooleanProperty();
        static void RefBooleanProperty(Windows::Foundation::IReference<bool> const& value);
        static Windows::Foundation::IReference<char16_t> RefCharProperty();
        static void RefCharProperty(Windows::Foundation::IReference<char16_t> const& value);
        static Windows::Foundation::IReference<int32_t> RefNumericProperty();
        static void RefNumericProperty(Windows::Foundation::IReference<int32_t> const& value);
        static Windows::Foundation::IReference<TestEnum> RefEnumProperty();
        static void RefEnumProperty(Windows::Foundation::IReference<TestEnum> const& value);
        static TestComponent::TestObject ObjectProperty();
        static void ObjectProperty(TestComponent::TestObject const& value);
        static hresult HResultProperty();
        static void HResultProperty(hresult value);

        static winrt::hstring CopyString(winrt::hstring value);

        static winrt::event_token ObjectEventHandler(
            Windows::Foundation::EventHandler<TestComponent::TestObject> const& handler);
        static void ObjectEventHandler(winrt::event_token const& token) noexcept;
        static void RaiseObjectEvent(TestComponent::TestObject const& value);

    private:
        inline static bool m_boolProperty = false;
        inline static char16_t m_charProperty = 0;
        inline static uint8_t m_u8Property = 0;
        inline static uint16_t m_u16Property = 0;
        inline static uint32_t m_u32Property = 0;
        inline static uint64_t m_u64Property = 0;
        inline static int16_t m_s16Property = 0;
        inline static int32_t m_s32Property = 0;
        inline static int64_t m_s64Property = 0;
        inline static float m_f32Property = 0;
        inline static double m_f64Property = 0;
        inline static hstring m_stringProperty;
        inline static guid m_guidProperty = {};
        inline static TestEnum m_enumProperty = TestEnum::First;
        inline static NumericTypes m_numericsStructProperty = {};
        inline static StringTypes m_stringsStructProperty = {};
        inline static BooleanTypes m_booleansStructProperty = {};
        inline static CompositeType m_compositeStructProperty = {};
        inline static winrt::Windows::Foundation::IReference<bool> m_refBoolProperty;
        inline static Windows::Foundation::IReference<char16_t> m_refCharProperty;
        inline static Windows::Foundation::IReference<int32_t> m_refNumericProperty;
        inline static Windows::Foundation::IReference<TestEnum> m_refEnumProperty;
        inline static TestComponent::TestObject m_objectProperty{ nullptr };
        inline static std::vector<bool> m_boolArrayProperty;
        inline static std::vector<char16_t> m_charArrayProperty;
        inline static std::vector<int32_t> m_numericArrayProperty;
        inline static std::vector<hstring> m_stringArrayProperty;
        inline static std::vector<guid> m_guidArrayProperty;
        inline static std::vector<TestEnum> m_enumArrayProperty;
        inline static std::vector<CompositeType> m_compositeStructArrayProperty;
        inline static std::vector<Windows::Foundation::IReference<int32_t>> m_refArrayProperty;
        inline static std::vector<Windows::Foundation::Collections::IVector<int32_t>> m_objectArrayProperty;
        inline static hresult m_hresultProperty;
        inline static event<Windows::Foundation::EventHandler<TestComponent::TestObject>> s_objectEventSource;
    };

}

namespace winrt::TestComponent::factory_implementation
{
    struct StaticOnlyTest : StaticOnlyTestT<StaticOnlyTest, implementation::StaticOnlyTest>
    {
    };
}
