#include "pch.h"

#include <cwctype>
#include <numeric>

#include "Test.h"

#include "Test.g.cpp"

winrt::hstring to_lower(const winrt::hstring& hstr)
{
    std::wstring str(hstr.data(), hstr.size());
    for (auto& ch : str)
    {
        ch = std::towlower(ch);
    }
    return winrt::hstring(str);
}

winrt::hstring to_upper(const winrt::hstring& hstr)
{
    std::wstring str(hstr.data(), hstr.size());
    for (auto& ch : str)
    {
        ch = std::towupper(ch);
    }
    return winrt::hstring(str);
}

template <typename T>
winrt::com_array<T> rotate_array(const winrt::array_view<const T>& values, int amt)
{
    assert(values.size() > static_cast<std::size_t>(amt));
    winrt::com_array<T> result(values.size());
    std::rotate_copy(values.rbegin(), values.rbegin() + amt, values.rend(), result.rbegin());
    return result;
}

template <typename T>
winrt::com_array<T> reverse_array(const winrt::array_view<const T>& values)
{
    return winrt::com_array<T>(values.rbegin(), values.rend());
}

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

    bool Test::StaticOrAll(array_view<bool const> values)
    {
        bool result = false;
        for (auto val : values)
        {
            result = result || val;
        }

        return result;
    }

    int32_t Test::StaticAdd(int32_t lhs, int32_t rhs)
    {
        return lhs + rhs;
    }

    int32_t Test::StaticAddAll(array_view<int32_t const> values)
    {
        return std::accumulate(values.begin(), values.end(), 0);
    }

    hstring Test::StaticAppend(hstring const& a, char16_t b, hstring const& c)
    {
        return a + b + c;
    }

    hstring Test::StaticAppendAll(array_view<hstring const> values)
    {
        std::wstring result;
        for (auto& val : values)
        {
            result.append(val.data(), val.size());
        }

        return hstring(result);
    }

    bool Test::StaticBoolOutParam(bool lhs, bool rhs, bool& andResult, bool& or)
    {
        andResult = lhs && rhs;
        or = lhs || rhs;
        return lhs ^ rhs;
    }

    char16_t Test::StaticCharOutParam(char16_t value, char16_t& next, char16_t& prev)
    {
        next = value + 1;
        prev = value - 1;
        return std::towupper(value);
    }

    int32_t Test::StaticNumericOutParam(int32_t value, int32_t& doubledValue, int32_t& tripledValue)
    {
        doubledValue = value * 2;
        tripledValue = value * 3;
        return value * 4;
    }

    hstring Test::StaticStringOutParam(hstring const& value, hstring& lower, hstring& upper)
    {
        std::wstring temp(value);
        lower = to_lower(value);
        upper = to_upper(value);

        temp.assign(value.c_str(), value.size());
        std::reverse(temp.begin(), temp.end());
        return hstring(temp);
    }

    winrt::guid Test::StaticGuidOutParam(winrt::guid const& value, winrt::guid& zero, winrt::guid& allSet)
    {
        zero = {};
        allSet = { 0xFFFFFFFF, 0xFFFF, 0xFFFF, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
        return value;
    }

    TestEnum Test::StaticEnumOutParam(TestEnum const& value, TestEnum& plusOne, TestEnum& plusTwo)
    {
        plusOne = static_cast<TestEnum>(static_cast<int>(value) + 1);
        plusTwo = static_cast<TestEnum>(static_cast<int>(value) + 2);
        return static_cast<TestEnum>(static_cast<int>(value) + 3);
    }

    CompositeType Test::StaticCompositeStructOutParam(CompositeType const& input, CompositeType& first, CompositeType& second)
    {
        auto& num = input.numerics;
        auto& str = input.strings;

        first.numerics = { static_cast<uint8_t>(num.u8 + 1), static_cast<uint16_t>(num.u16 + 1), num.u32 + 1,
            num.u64 + 1, num.s16 + 1, num.s32 + 1, num.s64 + 1, num.f32 + 1, num.f64 + 1,
            static_cast<TestEnum>(static_cast<int>(num.e) + 1) };
        first.strings = { static_cast<char16_t>(str.ch + 1), to_lower(str.str), {} };
        first.bools = { !input.bools.b };

        second.numerics = { static_cast<uint8_t>(num.u8 - 1), static_cast<uint16_t>(num.u16 - 1), num.u32 - 1,
            num.u64 - 1, num.s16 - 1, num.s32 - 1, num.s64 - 1, num.f32 - 1, num.f64 - 1,
            static_cast<TestEnum>(static_cast<int>(num.e) - 1) };
        second.strings = { static_cast<char16_t>(str.ch - 1), to_upper(str.str),
            { 0xFFFFFFFF, 0xFFFF, 0xFFFF, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } } };
        second.bools = { input.bools.b };

        return input;
    }

    Windows::Foundation::IReference<int32_t> Test::StaticRefOutParam(
        Windows::Foundation::IReference<int32_t> const& value, Windows::Foundation::IReference<int32_t>& doubledValue,
        Windows::Foundation::IReference<int32_t>& tripledValue)
    {
        doubledValue = value.Value() * 2;
        tripledValue = value.Value() * 3;
        return value.Value() * 4;
    }

    Windows::Foundation::Collections::IVector<int32_t> Test::StaticObjectOutParam(
        Windows::Foundation::Collections::IVector<int32_t> const& values,
        Windows::Foundation::Collections::IVector<int32_t>& doubledValues,
        Windows::Foundation::Collections::IVector<int32_t>& tripledValues)
    {
        doubledValues = single_threaded_vector<int32_t>();
        tripledValues = single_threaded_vector<int32_t>();
        for (auto val : values)
        {
            doubledValues.Append(val * 2);
            tripledValues.Append(val * 3);
        }

        return values;
    }

    com_array<bool> Test::StaticBoolArrayOutParam(
        array_view<bool const> values, com_array<bool>& rot1, com_array<bool>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<char16_t> Test::StaticCharArrayOutParam(
        array_view<char16_t const> values, com_array<char16_t>& rot1, com_array<char16_t>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<int32_t> Test::StaticNumericArrayOutParam(
        array_view<int32_t const> values, com_array<int32_t>& rot1, com_array<int32_t>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<hstring> Test::StaticStringArrayOutParam(
        array_view<hstring const> values, com_array<hstring>& rot1, com_array<hstring>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<winrt::guid> Test::StaticGuidArrayOutParam(
        array_view<winrt::guid const> values, com_array<winrt::guid>& rot1, com_array<winrt::guid>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<TestEnum> Test::StaticEnumArrayOutParam(
        array_view<TestEnum const> values, com_array<TestEnum>& rot1, com_array<TestEnum>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<CompositeType> Test::StaticCompositeStructArrayOutParam(
        array_view<CompositeType const> values, com_array<CompositeType>& rot1, com_array<CompositeType>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<Windows::Foundation::Collections::IVector<int32_t>> Test::StaticObjectArrayOutParam(
        array_view<Windows::Foundation::Collections::IVector<int32_t> const> values,
        com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot1,
        com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    void Test::StaticBoolFillParam(array_view<bool> values)
    {
        bool next = false;
        for (auto& val : values)
        {
            val = next;
            next = !next;
        }
    }

    void Test::StaticCharFillParam(array_view<char16_t> values)
    {
        std::iota(values.begin(), values.end(), u'a');
    }

    void Test::StaticNumericFillParam(array_view<int32_t> values)
    {
        std::iota(values.begin(), values.end(), 0);
    }

    void Test::StaticStringFillParam(array_view<hstring> values)
    {
        std::size_t len = 0;
        for (auto& val : values)
        {
            val = hstring(std::wstring(len++, L'a'));
        }
    }

    void Test::StaticGuidFillParam(array_view<winrt::guid> values)
    {
        guid next = {};
        for (auto& val : values)
        {
            val = next;
            ++next.Data1;
            ++next.Data2;
            ++next.Data3;
            for (auto& byte : next.Data4) ++byte;
        }
    }

    void Test::StaticEnumFillParam(array_view<TestEnum> values)
    {
        auto next = TestEnum::First;
        for (auto& val : values)
        {
            val = next;
            next = static_cast<TestEnum>(static_cast<int>(next) + 1);
        }
    }

    void Test::StaticCompositeStructFillParam(array_view<CompositeType> values)
    {
        CompositeType next = {};
        for (auto& val : values)
        {
            val = next;
            ++next.numerics.u8;
            ++next.numerics.u16;
            ++next.numerics.u32;
            ++next.numerics.u64;
            ++next.numerics.s16;
            ++next.numerics.s32;
            ++next.numerics.s64;
            ++next.numerics.f32;
            ++next.numerics.f64;
            next.numerics.e = static_cast<TestEnum>(static_cast<int>(next.numerics.e) + 1);
            ++next.strings.ch;
            next.strings.str = std::wstring(next.strings.str.c_str(), next.strings.str.size()) + L"a";
            ++next.strings.guid.Data1;
            ++next.strings.guid.Data2;
            ++next.strings.guid.Data3;
            for (auto& byte : next.strings.guid.Data4) ++byte;
            next.bools.b = !next.bools.b;
        }
    }

    void Test::StaticObjectFillParam(array_view<Windows::Foundation::Collections::IVector<int32_t>> values)
    {
        int32_t next = 0;
        std::vector<int32_t> v;
        for (auto& val : values)
        {
            val = single_threaded_vector<int32_t>(std::vector{ v });
            v.push_back(next++);
        }
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

    bool Test::OrAll(array_view<bool const> values)
    {
        bool result = false;
        for (auto val : values)
        {
            result = result || val;
        }

        return result;
    }

    int32_t Test::Add(int32_t lhs, int32_t rhs)
    {
        return lhs + rhs;
    }

    int32_t Test::AddAll(array_view<int32_t const> values)
    {
        return std::accumulate(values.begin(), values.end(), 0);
    }

    hstring Test::Append(hstring const& a, char16_t b, hstring const& c)
    {
        return a + b + c;
    }

    hstring Test::AppendAll(array_view<hstring const> values)
    {
        std::wstring result;
        for (auto& val : values)
        {
            result.append(val.data(), val.size());
        }

        return hstring(result);
    }

    bool Test::BoolOutParam(bool lhs, bool rhs, bool& andResult, bool& or)
    {
        andResult = lhs && rhs;
        or = lhs || rhs;
        return lhs ^ rhs;
    }

    char16_t Test::CharOutParam(char16_t value, char16_t& next, char16_t& prev)
    {
        next = value + 1;
        prev = value - 1;
        return std::towupper(value);
    }

    int32_t Test::NumericOutParam(int32_t value, int32_t& doubledValue, int32_t& tripledValue)
    {
        doubledValue = value * 2;
        tripledValue = value * 3;
        return value * 4;
    }

    hstring Test::StringOutParam(hstring const& value, hstring& lower, hstring& upper)
    {
        return StaticStringOutParam(value, lower, upper);
    }

    winrt::guid Test::GuidOutParam(winrt::guid const& value, winrt::guid& zero, winrt::guid& allSet)
    {
        zero = {};
        allSet = { 0xFFFFFFFF, 0xFFFF, 0xFFFF, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
        return value;
    }

    TestEnum Test::EnumOutParam(TestEnum const& value, TestEnum& plusOne, TestEnum& plusTwo)
    {
        plusOne = static_cast<TestEnum>(static_cast<int>(value) + 1);
        plusTwo = static_cast<TestEnum>(static_cast<int>(value) + 2);
        return static_cast<TestEnum>(static_cast<int>(value) + 3);
    }

    CompositeType Test::CompositeStructOutParam(CompositeType const& input, CompositeType& first, CompositeType& second)
    {
        return StaticCompositeStructOutParam(input, first, second);
    }

    Windows::Foundation::IReference<int32_t> Test::RefOutParam(Windows::Foundation::IReference<int32_t> const& value,
        Windows::Foundation::IReference<int32_t>& doubledValue, Windows::Foundation::IReference<int32_t>& tripledValue)
    {
        doubledValue = value.Value() * 2;
        tripledValue = value.Value() * 3;
        return value.Value() * 4;
    }

    Windows::Foundation::Collections::IVector<int32_t> Test::ObjectOutParam(
        Windows::Foundation::Collections::IVector<int32_t> const& values,
        Windows::Foundation::Collections::IVector<int32_t>& doubledValues,
        Windows::Foundation::Collections::IVector<int32_t>& tripledValues)
    {
        doubledValues = single_threaded_vector<int32_t>();
        tripledValues = single_threaded_vector<int32_t>();
        for (auto val : values)
        {
            doubledValues.Append(val * 2);
            tripledValues.Append(val * 3);
        }

        return values;
    }

    com_array<bool> Test::BoolArrayOutParam(
        array_view<bool const> values, com_array<bool>& rot1, com_array<bool>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<char16_t> Test::CharArrayOutParam(
        array_view<char16_t const> values, com_array<char16_t>& rot1, com_array<char16_t>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<int32_t> Test::NumericArrayOutParam(
        array_view<int32_t const> values, com_array<int32_t>& rot1, com_array<int32_t>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<hstring> Test::StringArrayOutParam(
        array_view<hstring const> values, com_array<hstring>& rot1, com_array<hstring>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<winrt::guid> Test::GuidArrayOutParam(
        array_view<winrt::guid const> values, com_array<winrt::guid>& rot1, com_array<winrt::guid>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<TestEnum> Test::EnumArrayOutParam(
        array_view<TestEnum const> values, com_array<TestEnum>& rot1, com_array<TestEnum>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<CompositeType> Test::CompositeStructArrayOutParam(
        array_view<CompositeType const> values, com_array<CompositeType>& rot1, com_array<CompositeType>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<Windows::Foundation::Collections::IVector<int32_t>> Test::ObjectArrayOutParam(
        array_view<Windows::Foundation::Collections::IVector<int32_t> const> values,
        com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot1,
        com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    void Test::BoolFillParam(array_view<bool> values)
    {
        bool next = false;
        for (auto& val : values)
        {
            val = next;
            next = !next;
        }
    }

    void Test::CharFillParam(array_view<char16_t> values)
    {
        std::iota(values.begin(), values.end(), u'a');
    }

    void Test::NumericFillParam(array_view<int32_t> values)
    {
        std::iota(values.begin(), values.end(), 0);
    }

    void Test::StringFillParam(array_view<hstring> values)
    {
        std::size_t len = 0;
        for (auto& val : values)
        {
            val = hstring(std::wstring(len++, L'a'));
        }
    }

    void Test::GuidFillParam(array_view<winrt::guid> values)
    {
        guid next = {};
        for (auto& val : values)
        {
            val = next;
            ++next.Data1;
            ++next.Data2;
            ++next.Data3;
            for (auto& byte : next.Data4) ++byte;
        }
    }

    void Test::EnumFillParam(array_view<TestEnum> values)
    {
        auto next = TestEnum::First;
        for (auto& val : values)
        {
            val = next;
            next = static_cast<TestEnum>(static_cast<int>(next) + 1);
        }
    }

    void Test::CompositeStructFillParam(array_view<CompositeType> values)
    {
        CompositeType next = {};
        for (auto& val : values)
        {
            val = next;
            ++next.numerics.u8;
            ++next.numerics.u16;
            ++next.numerics.u32;
            ++next.numerics.u64;
            ++next.numerics.s16;
            ++next.numerics.s32;
            ++next.numerics.s64;
            ++next.numerics.f32;
            ++next.numerics.f64;
            next.numerics.e = static_cast<TestEnum>(static_cast<int>(next.numerics.e) + 1);
            ++next.strings.ch;
            next.strings.str = std::wstring(next.strings.str.c_str(), next.strings.str.size()) + L"a";
            ++next.strings.guid.Data1;
            ++next.strings.guid.Data2;
            ++next.strings.guid.Data3;
            for (auto& byte : next.strings.guid.Data4) ++byte;
            next.bools.b = !next.bools.b;
        }
    }

    void Test::ObjectFillParam(array_view<Windows::Foundation::Collections::IVector<int32_t>> values)
    {
        int32_t next = 0;
        std::vector<int32_t> v;
        for (auto& val : values)
        {
            val = single_threaded_vector<int32_t>(std::vector{ v });
            v.push_back(next++);
        }
    }
}
