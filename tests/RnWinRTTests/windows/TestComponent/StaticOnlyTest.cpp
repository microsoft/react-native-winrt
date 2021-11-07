// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#include "pch.h"

#include "StaticOnlyTest.h"

#include "StaticOnlyTest.g.cpp"

#include <numeric>

namespace winrt::TestComponent::implementation
{
    bool StaticOnlyTest::BoolProperty()
    {
        return m_boolProperty;
    }

    void StaticOnlyTest::BoolProperty(bool value)
    {
        m_boolProperty = value;
    }

    char16_t StaticOnlyTest::CharProperty()
    {
        return m_charProperty;
    }

    void StaticOnlyTest::CharProperty(char16_t value)
    {
        m_charProperty = value;
    }

    uint8_t StaticOnlyTest::U8Property()
    {
        return m_u8Property;
    }

    void StaticOnlyTest::U8Property(uint8_t value)
    {
        m_u8Property = value;
    }

    uint16_t StaticOnlyTest::U16Property()
    {
        return m_u16Property;
    }

    void StaticOnlyTest::U16Property(uint16_t value)
    {
        m_u16Property = value;
    }

    uint32_t StaticOnlyTest::U32Property()
    {
        return m_u32Property;
    }

    void StaticOnlyTest::U32Property(uint32_t value)
    {
        m_u32Property = value;
    }

    uint64_t StaticOnlyTest::U64Property()
    {
        return m_u64Property;
    }

    void StaticOnlyTest::U64Property(uint64_t value)
    {
        m_u64Property = value;
    }

    int16_t StaticOnlyTest::S16Property()
    {
        return m_s16Property;
    }

    void StaticOnlyTest::S16Property(int16_t value)
    {
        m_s16Property = value;
    }

    int32_t StaticOnlyTest::S32Property()
    {
        return m_s32Property;
    }

    void StaticOnlyTest::S32Property(int32_t value)
    {
        m_s32Property = value;
    }

    int64_t StaticOnlyTest::S64Property()
    {
        return m_s64Property;
    }

    void StaticOnlyTest::S64Property(int64_t value)
    {
        m_s64Property = value;
    }

    float StaticOnlyTest::F32Property()
    {
        return m_f32Property;
    }

    void StaticOnlyTest::F32Property(float value)
    {
        m_f32Property = value;
    }

    double StaticOnlyTest::F64Property()
    {
        return m_f64Property;
    }

    void StaticOnlyTest::F64Property(double value)
    {
        m_f64Property = value;
    }

    hstring StaticOnlyTest::StringProperty()
    {
        return m_stringProperty;
    }

    void StaticOnlyTest::StringProperty(hstring const& value)
    {
        m_stringProperty = value;
    }

    winrt::guid StaticOnlyTest::GuidProperty()
    {
        return m_guidProperty;
    }

    void StaticOnlyTest::GuidProperty(winrt::guid const& value)
    {
        m_guidProperty = value;
    }

    TestEnum StaticOnlyTest::EnumProperty()
    {
        return m_enumProperty;
    }

    void StaticOnlyTest::EnumProperty(TestEnum const& value)
    {
        m_enumProperty = value;
    }

    NumericTypes StaticOnlyTest::NumericsStructProperty()
    {
        return m_numericsStructProperty;
    }

    void StaticOnlyTest::NumericsStructProperty(NumericTypes const& value)
    {
        m_numericsStructProperty = value;
    }

    StringTypes StaticOnlyTest::StringsStructProperty()
    {
        return m_stringsStructProperty;
    }

    void StaticOnlyTest::StringsStructProperty(StringTypes const& value)
    {
        m_stringsStructProperty = value;
    }

    BooleanTypes StaticOnlyTest::BooleansStructProperty()
    {
        return m_booleansStructProperty;
    }

    void StaticOnlyTest::BooleansStructProperty(BooleanTypes const& value)
    {
        m_booleansStructProperty = value;
    }

    CompositeType StaticOnlyTest::CompositeStructProperty()
    {
        return m_compositeStructProperty;
    }

    void StaticOnlyTest::CompositeStructProperty(CompositeType const& value)
    {
        m_compositeStructProperty = value;
    }

    Windows::Foundation::IReference<bool> StaticOnlyTest::RefBooleanProperty()
    {
        return m_refBoolProperty;
    }

    void StaticOnlyTest::RefBooleanProperty(Windows::Foundation::IReference<bool> const& value)
    {
        m_refBoolProperty = value;
    }

    Windows::Foundation::IReference<char16_t> StaticOnlyTest::RefCharProperty()
    {
        return m_refCharProperty;
    }

    void StaticOnlyTest::RefCharProperty(Windows::Foundation::IReference<char16_t> const& value)
    {
        m_refCharProperty = value;
    }

    Windows::Foundation::IReference<int32_t> StaticOnlyTest::RefNumericProperty()
    {
        return m_refNumericProperty;
    }

    void StaticOnlyTest::RefNumericProperty(Windows::Foundation::IReference<int32_t> const& value)
    {
        m_refNumericProperty = value;
    }

    Windows::Foundation::IReference<TestEnum> StaticOnlyTest::RefEnumProperty()
    {
        return m_refEnumProperty;
    }

    void StaticOnlyTest::RefEnumProperty(Windows::Foundation::IReference<TestEnum> const& value)
    {
        m_refEnumProperty = value;
    }

    TestComponent::TestObject StaticOnlyTest::ObjectProperty()
    {
        return m_objectProperty;
    }

    void StaticOnlyTest::ObjectProperty(TestComponent::TestObject const& value)
    {
        m_objectProperty = value;
    }

    hresult StaticOnlyTest::HResultProperty()
    {
        return m_hresultProperty;
    }

    void StaticOnlyTest::HResultProperty(hresult value)
    {
        m_hresultProperty = value;
    }

    winrt::hstring StaticOnlyTest::CopyString(winrt::hstring value)
    {
        return value;
    }

    winrt::event_token StaticOnlyTest::ObjectEventHandler(
        Windows::Foundation::EventHandler<TestComponent::TestObject> const& handler)
    {
        return s_objectEventSource.add(handler);
    }

    void StaticOnlyTest::ObjectEventHandler(winrt::event_token const& token) noexcept
    {
        s_objectEventSource.remove(token);
    }

    void StaticOnlyTest::RaiseObjectEvent(TestComponent::TestObject const& value)
    {
        s_objectEventSource(nullptr, value);
    }

}