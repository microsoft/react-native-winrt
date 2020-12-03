#include "pch.h"

#include <cwctype>

#include "Test.h"

#include "Test.g.cpp"

namespace winrt::TestComponent::implementation
{
    bool Test::StaticBoolProperty()
    {
        return s_boolProperty;
    }

    void Test::StaticBoolProperty(bool value)
    {
        s_boolProperty = value;
    }

    bool Test::StaticOr(bool lhs, bool rhs)
    {
        return lhs || rhs;
    }

    int32_t Test::StaticAdd(int32_t lhs, int32_t rhs)
    {
        return lhs + rhs;
    }

    hstring Test::StaticAppend(hstring const& a, char16_t b, hstring const& c)
    {
        return a + b + c;
    }

    void Test::StaticBoolOutParam(bool lhs, bool rhs, bool& andResult)
    {
        andResult = lhs && rhs;
    }

    void Test::StaticBoolOutParam2(bool lhs, bool rhs, bool& andResult, bool& or)
    {
        andResult = lhs && rhs;
        or = lhs || rhs;
    }

    bool Test::StaticBoolOutParamWithReturn(bool lhs, bool rhs, bool& andResult, bool& or)
    {
        andResult = lhs && rhs;
        or = lhs || rhs;
        return lhs ^ rhs;
    }

    void Test::StaticCharOutParam(char16_t value, char16_t& next)
    {
        next = value + 1;
    }

    void Test::StaticCharOutParam2(char16_t value, char16_t& next, char16_t& prev)
    {
        next = value + 1;
        prev = value - 1;
    }

    char16_t Test::StaticCharOutParamWithReturn(char16_t value, char16_t& next, char16_t& prev)
    {
        next = value + 1;
        prev = value - 1;
        return std::towupper(value);
    }

    void Test::StaticNumericOutParam(int32_t value, int32_t& doubledValue)
    {
        doubledValue = value * 2;
    }

    void Test::StaticNumericOutParam2(int32_t value, int32_t& doubledValue, int32_t& tripledValue)
    {
        doubledValue = value * 2;
        tripledValue = value * 3;
    }

    int32_t Test::StaticNumericOutParamWithReturn(int32_t value, int32_t& doubledValue, int32_t& tripledValue)
    {
        doubledValue = value * 2;
        tripledValue = value * 3;
        return value * 4;
    }

    template <typename Func>
    std::wstring& TransformString(std::wstring& value, Func&& fn)
    {
        std::transform(value.begin(), value.end(), value.begin(), fn);
        return value;
    }

    void Test::StaticStringOutParam(hstring const& value, hstring& lower)
    {
        std::wstring temp(value);
        lower = TransformString(temp, [](wchar_t ch) { return std::towlower(ch); });
    }

    void Test::StaticStringOutParam2(hstring const& value, hstring& lower, hstring& upper)
    {
        std::wstring temp(value);
        lower = TransformString(temp, [](wchar_t ch) { return std::towlower(ch); });
        upper = TransformString(temp, [](wchar_t ch) { return std::towupper(ch); });
    }

    hstring Test::StaticStringOutParamWithReturn(hstring const& value, hstring& lower, hstring& upper)
    {
        std::wstring temp(value);
        lower = TransformString(temp, [](wchar_t ch) { return std::towlower(ch); });
        upper = TransformString(temp, [](wchar_t ch) { return std::towupper(ch); });

        temp.assign(value.c_str(), value.size());
        std::reverse(temp.begin(), temp.end());
        return hstring(temp);
    }

    Windows::Foundation::Collections::IVector<int32_t> Test::MakeNumericVector(array_view<int32_t const> values)
    {
        return single_threaded_vector(std::vector<int32_t>(values.begin(), values.end()));
    }

    bool Test::BoolProperty()
    {
        return m_boolProperty;
    }

    void Test::BoolProperty(bool value)
    {
        m_boolProperty = value;
    }

    char16_t Test::CharProperty()
    {
        return m_charProperty;
    }

    void Test::CharProperty(char16_t value)
    {
        m_charProperty = value;
    }

    uint8_t Test::U8Property()
    {
        return m_u8Property;
    }

    void Test::U8Property(uint8_t value)
    {
        m_u8Property = value;
    }

    uint16_t Test::U16Property()
    {
        return m_u16Property;
    }

    void Test::U16Property(uint16_t value)
    {
        m_u16Property = value;
    }

    uint32_t Test::U32Property()
    {
        return m_u32Property;
    }

    void Test::U32Property(uint32_t value)
    {
        m_u32Property = value;
    }

    uint64_t Test::U64Property()
    {
        return m_u64Property;
    }

    void Test::U64Property(uint64_t value)
    {
        m_u64Property = value;
    }

    int16_t Test::S16Property()
    {
        return m_s16Property;
    }

    void Test::S16Property(int16_t value)
    {
        m_s16Property = value;
    }

    int32_t Test::S32Property()
    {
        return m_s32Property;
    }

    void Test::S32Property(int32_t value)
    {
        m_s32Property = value;
    }

    int64_t Test::S64Property()
    {
        return m_s64Property;
    }

    void Test::S64Property(int64_t value)
    {
        m_s64Property = value;
    }

    float Test::F32Property()
    {
        return m_f32Property;
    }

    void Test::F32Property(float value)
    {
        m_f32Property = value;
    }

    double Test::F64Property()
    {
        return m_f64Property;
    }

    void Test::F64Property(double value)
    {
        m_f64Property = value;
    }

    hstring Test::StringProperty()
    {
        return m_stringProperty;
    }

    void Test::StringProperty(hstring const& value)
    {
        m_stringProperty = value;
    }

    winrt::guid Test::GuidProperty()
    {
        return m_guidProperty;
    }

    void Test::GuidProperty(winrt::guid const& value)
    {
        m_guidProperty = value;
    }

    TestEnum Test::EnumProperty()
    {
        return m_enumProperty;
    }

    void Test::EnumProperty(TestEnum const& value)
    {
        m_enumProperty = value;
    }

    NumericTypes Test::NumericsStructProperty()
    {
        return m_numericsStructProperty;
    }

    void Test::NumericsStructProperty(NumericTypes const& value)
    {
        m_numericsStructProperty = value;
    }

    StringTypes Test::StringsStructProperty()
    {
        return m_stringsStructProperty;
    }

    void Test::StringsStructProperty(StringTypes const& value)
    {
        m_stringsStructProperty = value;
    }

    BooleanTypes Test::BooleansStructProperty()
    {
        return m_booleansStructProperty;
    }

    void Test::BooleansStructProperty(BooleanTypes const& value)
    {
        m_booleansStructProperty = value;
    }

    CompositeType Test::CompositeStructProperty()
    {
        return m_compositeStructProperty;
    }

    void Test::CompositeStructProperty(CompositeType const& value)
    {
        m_compositeStructProperty = value;
    }

    Windows::Foundation::IReference<bool> Test::RefBooleanProperty()
    {
        return m_refBoolProperty;
    }

    void Test::RefBooleanProperty(Windows::Foundation::IReference<bool> const& value)
    {
        m_refBoolProperty = value;
    }

    Windows::Foundation::IReference<char16_t> Test::RefCharProperty()
    {
        return m_refCharProperty;
    }

    void Test::RefCharProperty(Windows::Foundation::IReference<char16_t> const& value)
    {
        m_refCharProperty = value;
    }

    Windows::Foundation::IReference<int32_t> Test::RefNumericProperty()
    {
        return m_refNumericProperty;
    }

    void Test::RefNumericProperty(Windows::Foundation::IReference<int32_t> const& value)
    {
        m_refNumericProperty = value;
    }

    Windows::Foundation::IReference<TestEnum> Test::RefEnumProperty()
    {
        return m_refEnumProperty;
    }

    void Test::RefEnumProperty(Windows::Foundation::IReference<TestEnum> const& value)
    {
        m_refEnumProperty = value;
    }

    Windows::Foundation::Collections::IVector<int32_t> Test::ObjectProperty()
    {
        return m_objectProperty;
    }

    void Test::ObjectProperty(Windows::Foundation::Collections::IVector<int32_t> const& value)
    {
        m_objectProperty = value;
    }

    com_array<bool> Test::BooleanArrayProperty()
    {
        return com_array<bool>(m_boolArrayProperty);
    }

    void Test::BooleanArrayProperty(array_view<bool const> value)
    {
        m_boolArrayProperty.assign(value.begin(), value.end());
    }

    com_array<char16_t> Test::CharArrayProperty()
    {
        return com_array<char16_t>(m_charArrayProperty);
    }

    void Test::CharArrayProperty(array_view<char16_t const> value)
    {
        m_charArrayProperty.assign(value.begin(), value.end());
    }

    com_array<int32_t> Test::NumericArrayProperty()
    {
        return com_array<int32_t>(m_numericArrayProperty);
    }

    void Test::NumericArrayProperty(array_view<int32_t const> value)
    {
        m_numericArrayProperty.assign(value.begin(), value.end());
    }

    com_array<hstring> Test::StringArrayProperty()
    {
        return com_array<hstring>(m_stringArrayProperty);
    }

    void Test::StringArrayProperty(array_view<hstring const> value)
    {
        m_stringArrayProperty.assign(value.begin(), value.end());
    }

    com_array<winrt::guid> Test::GuidArrayProperty()
    {
        return com_array<guid>(m_guidArrayProperty);
    }

    void Test::GuidArrayProperty(array_view<winrt::guid const> value)
    {
        m_guidArrayProperty.assign(value.begin(), value.end());
    }

    com_array<TestEnum> Test::EnumArrayProperty()
    {
        return com_array<TestEnum>(m_enumArrayProperty);
    }

    void Test::EnumArrayProperty(array_view<TestEnum const> value)
    {
        m_enumArrayProperty.assign(value.begin(), value.end());
    }

    com_array<CompositeType> Test::CompositeStructArrayProperty()
    {
        return com_array<CompositeType>(m_compositeStructArrayProperty);
    }

    void Test::CompositeStructArrayProperty(array_view<CompositeType const> value)
    {
        m_compositeStructArrayProperty.assign(value.begin(), value.end());
    }

    com_array<Windows::Foundation::IReference<int32_t>> Test::RefArrayProperty()
    {
        return com_array<Windows::Foundation::IReference<int32_t>>(m_refArrayProperty);
    }

    void Test::RefArrayProperty(array_view<Windows::Foundation::IReference<int32_t> const> value)
    {
        m_refArrayProperty.assign(value.begin(), value.end());
    }

    com_array<Windows::Foundation::Collections::IVector<int32_t>> Test::ObjectArrayProperty()
    {
        return com_array<Windows::Foundation::Collections::IVector<int32_t>>(m_objectArrayProperty);
    }

    void Test::ObjectArrayProperty(array_view<Windows::Foundation::Collections::IVector<int32_t> const> value)
    {
        m_objectArrayProperty.assign(value.begin(), value.end());
    }

    bool Test::Or(bool lhs, bool rhs)
    {
        return lhs || rhs;
    }

    int32_t Test::Add(int32_t lhs, int32_t rhs)
    {
        return lhs + rhs;
    }

    hstring Test::Append(hstring const& a, char16_t b, hstring const& c)
    {
        return a + b + c;
    }

    void Test::BoolOutParam(bool lhs, bool rhs, bool& andResult)
    {
        andResult = lhs && rhs;
    }

    void Test::BoolOutParam2(bool lhs, bool rhs, bool& andResult, bool& or)
    {
        andResult = lhs && rhs;
        or = lhs || rhs;
    }

    bool Test::BoolOutParamWithReturn(bool lhs, bool rhs, bool& andResult, bool& or)
    {
        andResult = lhs && rhs;
        or = lhs || rhs;
        return lhs ^ rhs;
    }

    void Test::CharOutParam(char16_t value, char16_t& next)
    {
        next = value + 1;
    }

    void Test::CharOutParam2(char16_t value, char16_t& next, char16_t& prev)
    {
        next = value + 1;
        prev = value - 1;
    }

    char16_t Test::CharOutParamWithReturn(char16_t value, char16_t& next, char16_t& prev)
    {
        next = value + 1;
        prev = value - 1;
        return std::towupper(value);
    }

    void Test::NumericOutParam(int32_t value, int32_t& doubledValue)
    {
        doubledValue = value * 2;
    }

    void Test::NumericOutParam2(int32_t value, int32_t& doubledValue, int32_t& tripledValue)
    {
        doubledValue = value * 2;
        tripledValue = value * 3;
    }

    int32_t Test::NumericOutParamWithReturn(int32_t value, int32_t& doubledValue, int32_t& tripledValue)
    {
        doubledValue = value * 2;
        tripledValue = value * 3;
        return value * 4;
    }

    void Test::StringOutParam(hstring const& value, hstring& lower)
    {
        StaticStringOutParam(value, lower);
    }

    void Test::StringOutParam2(hstring const& value, hstring& lower, hstring& upper)
    {
        StaticStringOutParam2(value, lower, upper);
    }

    hstring Test::StringOutParamWithReturn(hstring const& value, hstring& lower, hstring& upper)
    {
        return StaticStringOutParamWithReturn(value, lower, upper);
    }
}
