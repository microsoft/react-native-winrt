#include "pch.h"

#include <atomic>
#include <concurrent_vector.h>
#include <cwctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <winrt/Windows.Storage.h>

#include "Test.h"

#include "Test.g.cpp"

using namespace std::literals;

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
}

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
    void Test::LogFailures(hstring const& failures)
    {
        auto storageFolder = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
        auto storageFile = storageFolder
                               .CreateFileAsync(winrt::hstring(L"FailureLog.txt"),
                                   winrt::Windows::Storage::CreationCollisionOption::ReplaceExisting)
                               .get();

        winrt::Windows::Storage::FileIO::WriteTextAsync(storageFile, failures).get();
    }

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

    CompositeType Test::StaticCompositeStructOutParam(
        CompositeType const& input, CompositeType& first, CompositeType& second)
    {
        auto& num = input.Numerics;
        auto& str = input.Strings;

        first.Numerics = { static_cast<uint8_t>(num.U8 + 1), static_cast<uint16_t>(num.U16 + 1), num.U32 + 1,
            num.U64 + 1, num.S16 + 1, num.S32 + 1, num.S64 + 1, num.F32 + 1, num.F64 + 1,
            static_cast<TestEnum>(static_cast<int>(num.Enum) + 1) };
        first.Strings = { static_cast<char16_t>(str.Char + 1), to_lower(str.String), {} };
        first.Bools = { !input.Bools.Value };

        second.Numerics = { static_cast<uint8_t>(num.U8 - 1), static_cast<uint16_t>(num.U16 - 1), num.U32 - 1,
            num.U64 - 1, num.S16 - 1, num.S32 - 1, num.S64 - 1, num.F32 - 1, num.F64 - 1,
            static_cast<TestEnum>(static_cast<int>(num.Enum) - 1) };
        second.Strings = { static_cast<char16_t>(str.Char - 1), to_upper(str.String),
            { 0xFFFFFFFF, 0xFFFF, 0xFFFF, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } } };
        second.Bools = { input.Bools.Value };

        return input;
    }

    IReference<int32_t> Test::StaticRefOutParam(
        IReference<int32_t> const& value, IReference<int32_t>& doubledValue, IReference<int32_t>& tripledValue)
    {
        doubledValue = value.Value() * 2;
        tripledValue = value.Value() * 3;
        return value.Value() * 4;
    }

    IVector<int32_t> Test::StaticObjectOutParam(
        IVector<int32_t> const& values, IVector<int32_t>& doubledValues, IVector<int32_t>& tripledValues)
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

    com_array<IReference<int32_t>> Test::StaticRefArrayOutParam(array_view<IReference<int32_t> const> values,
        com_array<IReference<int32_t>>& rot1, com_array<IReference<int32_t>>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<IVector<int32_t>> Test::StaticObjectArrayOutParam(
        array_view<IVector<int32_t> const> values, com_array<IVector<int32_t>>& rot1, com_array<IVector<int32_t>>& rot2)
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
            for (auto& byte : next.Data4)
                ++byte;
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
            ++next.Numerics.U8;
            ++next.Numerics.U16;
            ++next.Numerics.U32;
            ++next.Numerics.U64;
            ++next.Numerics.S16;
            ++next.Numerics.S32;
            ++next.Numerics.S64;
            ++next.Numerics.F32;
            ++next.Numerics.F64;
            next.Numerics.Enum = static_cast<TestEnum>(static_cast<int>(next.Numerics.Enum) + 1);
            ++next.Strings.Char;
            next.Strings.String = std::wstring(next.Strings.String.c_str(), next.Strings.String.size()) + L"a";
            ++next.Strings.Guid.Data1;
            ++next.Strings.Guid.Data2;
            ++next.Strings.Guid.Data3;
            for (auto& byte : next.Strings.Guid.Data4)
                ++byte;
            next.Bools.Value = !next.Bools.Value;
        }
    }

    void Test::StaticRefFillParam(array_view<IReference<int32_t>> values)
    {
        std::iota(values.begin(), values.end(), 0);
    }

    void Test::StaticObjectFillParam(array_view<IVector<int32_t>> values)
    {
        int32_t next = 0;
        std::vector<int32_t> v;
        for (auto& val : values)
        {
            val = single_threaded_vector<int32_t>(std::vector{ v });
            v.push_back(next++);
        }
    }

    winrt::event_token Test::StaticBoolEventHandler(EventHandler<bool> const& handler)
    {
        return s_boolEventSource.add(handler);
    }

    void Test::StaticBoolEventHandler(winrt::event_token const& token) noexcept
    {
        s_boolEventSource.remove(token);
    }

    winrt::event_token Test::StaticCharEventHandler(EventHandler<char16_t> const& handler)
    {
        return s_charEventSource.add(handler);
    }

    void Test::StaticCharEventHandler(winrt::event_token const& token) noexcept
    {
        s_charEventSource.remove(token);
    }

    winrt::event_token Test::StaticNumericEventHandler(EventHandler<int32_t> const& handler)
    {
        return s_numericEventSource.add(handler);
    }

    void Test::StaticNumericEventHandler(winrt::event_token const& token) noexcept
    {
        s_numericEventSource.remove(token);
    }

    winrt::event_token Test::StaticStringEventHandler(EventHandler<hstring> const& handler)
    {
        return s_stringEventSource.add(handler);
    }

    void Test::StaticStringEventHandler(winrt::event_token const& token) noexcept
    {
        s_stringEventSource.remove(token);
    }

    winrt::event_token Test::StaticGuidEventHandler(EventHandler<winrt::guid> const& handler)
    {
        return s_guidEventSource.add(handler);
    }

    void Test::StaticGuidEventHandler(winrt::event_token const& token) noexcept
    {
        s_guidEventSource.remove(token);
    }

    winrt::event_token Test::StaticEnumEventHandler(EventHandler<TestEnum> const& handler)
    {
        return s_enumEventSource.add(handler);
    }

    void Test::StaticEnumEventHandler(winrt::event_token const& token) noexcept
    {
        s_enumEventSource.remove(token);
    }

    winrt::event_token Test::StaticCompositeStructEventHandler(EventHandler<CompositeType> const& handler)
    {
        return s_compositeStructEventSource.add(handler);
    }

    void Test::StaticCompositeStructEventHandler(winrt::event_token const& token) noexcept
    {
        s_compositeStructEventSource.remove(token);
    }

    winrt::event_token Test::StaticRefEventHandler(EventHandler<IReference<int32_t>> const& handler)
    {
        return s_refEventSource.add(handler);
    }

    void Test::StaticRefEventHandler(winrt::event_token const& token) noexcept
    {
        s_refEventSource.remove(token);
    }

    winrt::event_token Test::StaticObjectEventHandler(EventHandler<IVector<int32_t>> const& handler)
    {
        return s_objectEventSource.add(handler);
    }

    void Test::StaticObjectEventHandler(winrt::event_token const& token) noexcept
    {
        s_objectEventSource.remove(token);
    }

    void Test::RaiseStaticBoolEvent(bool value)
    {
        s_boolEventSource(nullptr, value);
    }

    void Test::RaiseStaticCharEvent(char16_t value)
    {
        s_charEventSource(nullptr, value);
    }

    void Test::RaiseStaticNumericEvent(int32_t value)
    {
        s_numericEventSource(nullptr, value);
    }

    void Test::RaiseStaticStringEvent(hstring const& value)
    {
        s_stringEventSource(nullptr, value);
    }

    void Test::RaiseStaticGuidEvent(winrt::guid const& value)
    {
        s_guidEventSource(nullptr, value);
    }

    void Test::RaiseStaticEnumEvent(TestEnum const& value)
    {
        s_enumEventSource(nullptr, value);
    }

    void Test::RaiseStaticCompositeStructEvent(CompositeType const& value)
    {
        s_compositeStructEventSource(nullptr, value);
    }

    void Test::RaiseStaticRefEvent(IReference<int32_t> const& value)
    {
        s_refEventSource(nullptr, value);
    }

    void Test::RaiseStaticObjectEvent(IVector<int32_t> const& value)
    {
        s_objectEventSource(nullptr, value);
    }

    bool Test::StaticInvokeBoolDelegate(bool inputValue, BoolDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    char16_t Test::StaticInvokeCharDelegate(char16_t inputValue, CharDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    int32_t Test::StaticInvokeNumericDelegate(int32_t inputValue, NumericDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    hstring Test::StaticInvokeStringDelegate(hstring const& inputValue, StringDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    winrt::guid Test::StaticInvokeGuidDelegate(winrt::guid const& inputValue, GuidDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    TestEnum Test::StaticInvokeEnumDelegate(TestEnum const& inputValue, EnumDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    CompositeType Test::StaticInvokeCompositeStructDelegate(
        CompositeType const& inputValue, CompositeStructDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    IReference<int32_t> Test::StaticInvokeRefDelegate(
        IReference<int32_t> const& inputValue, RefDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    IVector<int32_t> Test::StaticInvokeObjectDelegate(
        IVector<int32_t> const& inputValue, ObjectDelegate const& targetFn)
    {
        return targetFn(inputValue);
    }

    bool Test::StaticInvokeBoolDelegateWithOutParam(bool inputValue, BoolDelegateWithOutParam const& targetFn)
    {
        bool result = false;
        targetFn(inputValue, result);
        return result;
    }

    char16_t Test::StaticInvokeCharDelegateWithOutParam(char16_t inputValue, CharDelegateWithOutParam const& targetFn)
    {
        char16_t result = 0;
        targetFn(inputValue, result);
        return result;
    }

    int32_t Test::StaticInvokeNumericDelegateWithOutParam(
        int32_t inputValue, NumericDelegateWithOutParam const& targetFn)
    {
        int32_t result = 0;
        targetFn(inputValue, result);
        return result;
    }

    hstring Test::StaticInvokeStringDelegateWithOutParam(
        hstring const& inputValue, StringDelegateWithOutParam const& targetFn)
    {
        hstring result;
        targetFn(inputValue, result);
        return result;
    }

    winrt::guid Test::StaticInvokeGuidDelegateWithOutParam(
        winrt::guid const& inputValue, GuidDelegateWithOutParam const& targetFn)
    {
        winrt::guid result;
        targetFn(inputValue, result);
        return result;
    }

    TestEnum Test::StaticInvokeEnumDelegateWithOutParam(
        TestEnum const& inputValue, EnumDelegateWithOutParam const& targetFn)
    {
        TestEnum result = {};
        targetFn(inputValue, result);
        return result;
    }

    CompositeType Test::StaticInvokeCompositeStructDelegateWithOutParam(
        CompositeType const& inputValue, CompositeStructDelegateWithOutParam const& targetFn)
    {
        CompositeType result = {};
        targetFn(inputValue, result);
        return result;
    }

    IReference<int32_t> Test::StaticInvokeRefDelegateWithOutParam(
        IReference<int32_t> const& inputValue, RefDelegateWithOutParam const& targetFn)
    {
        IReference<int32_t> result;
        targetFn(inputValue, result);
        return result;
    }

    IVector<int32_t> Test::StaticInvokeObjectDelegateWithOutParam(
        IVector<int32_t> const& inputValue, ObjectDelegateWithOutParam const& targetFn)
    {
        IVector<int32_t> result;
        targetFn(inputValue, result);
        return result;
    }

    template <typename T, typename Delegate>
    static bool DoInvokeArrayDelegate(array_view<const T> values, const Delegate& targetFn)
    {
        T fillResult[5];
        if (values.size() < std::size(fillResult))
            throw hresult_invalid_argument(); // Since we aren't reporting result size; assume complete fill

        com_array<T> outResult;
        auto result = targetFn(values, fillResult, outResult);

        // Check the fill result
        for (uint32_t i = 0; i < std::size(fillResult); ++i)
        {
            if (values[i] != fillResult[i])
                return false;
        }

        // Check the output arg
        if (outResult.size() != values.size())
            return false;
        for (uint32_t i = 0; i < values.size(); ++i)
        {
            if (values[i] != outResult[values.size() - i - 1])
                return false;
        }

        // Return value should be a copy
        if (result.size() != values.size())
            return false;
        for (uint32_t i = 0; i < values.size(); ++i)
        {
            if (values[i] != result[i])
                return false;
        }

        return true;
    }

    bool Test::StaticInvokeBoolArrayDelegate(array_view<bool const> values, BoolArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeCharArrayDelegate(array_view<char16_t const> values, CharArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeNumericArrayDelegate(array_view<int32_t const> values, NumericArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeStringArrayDelegate(array_view<hstring const> values, StringArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeGuidArrayDelegate(array_view<winrt::guid const> values, GuidArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeEnumArrayDelegate(array_view<TestEnum const> values, EnumArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeCompositeStructArrayDelegate(
        array_view<CompositeType const> values, CompositeStructArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeRefArrayDelegate(
        array_view<IReference<int32_t> const> values, RefArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    bool Test::StaticInvokeObjectArrayDelegate(
        array_view<IVector<int32_t> const> values, ObjectArrayDelegate const& targetFn)
    {
        return DoInvokeArrayDelegate(values, targetFn);
    }

    template <typename T>
    static IVector<T> CopyToVector(array_view<T const> values)
    {
        return single_threaded_vector(std::vector<T>(values.begin(), values.end()));
    }

    IVector<bool> Test::CopyBoolsToVector(array_view<bool const> values)
    {
        return CopyToVector(values);
    }

    IVector<char16_t> Test::CopyCharsToVector(array_view<char16_t const> values)
    {
        return CopyToVector(values);
    }

    IVector<int32_t> Test::CopyNumericsToVector(array_view<int32_t const> values)
    {
        return CopyToVector(values);
    }

    IVector<hstring> Test::CopyStringsToVector(array_view<hstring const> values)
    {
        return CopyToVector(values);
    }

    IVector<winrt::guid> Test::CopyGuidsToVector(array_view<winrt::guid const> values)
    {
        return CopyToVector(values);
    }

    IVector<TestEnum> Test::CopyEnumValuesToVector(array_view<TestEnum const> values)
    {
        return CopyToVector(values);
    }

    IVector<CompositeType> Test::CopyCompositeStructsToVector(array_view<CompositeType const> values)
    {
        return CopyToVector(values);
    }

    IVector<IReference<int32_t>> Test::CopyRefsToVector(array_view<IReference<int32_t> const> values)
    {
        return CopyToVector(values);
    }

    IVector<IInspectable> Test::CopyObjectsToVector(array_view<IInspectable const> values)
    {
        return CopyToVector(values);
    }

    template <typename T>
    struct VectorViewImpl : winrt::implements<VectorViewImpl<T>, IVectorView<T>, IIterable<T>>
    {
        VectorViewImpl(IVector<T> value) : m_value(std::move(value))
        {
        }

        // IVectorView
        uint32_t Size() const noexcept
        {
            return m_value.Size();
        }

        T GetAt(uint32_t index) const
        {
            return m_value.GetAt(index);
        }

        uint32_t GetMany(uint32_t startIndex, const array_view<T>& items) const
        {
            return m_value.GetMany(startIndex, items);
        }

        bool IndexOf(const T& value, uint32_t& index) const
        {
            return m_value.IndexOf(value, index);
        }

        // IIterable
        IIterator<T> First() const
        {
            return m_value.First();
        }

    private:
        IVector<T> m_value;
    };

    template <typename T>
    static IVectorView<T> CopyToVectorView(array_view<T const> values)
    {
        return make<VectorViewImpl<T>>(CopyToVector(values));
    }

    IVectorView<bool> Test::CopyBoolsToVectorView(array_view<bool const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<char16_t> Test::CopyCharsToVectorView(array_view<char16_t const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<int32_t> Test::CopyNumericsToVectorView(array_view<int32_t const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<hstring> Test::CopyStringsToVectorView(array_view<hstring const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<winrt::guid> Test::CopyGuidsToVectorView(array_view<winrt::guid const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<TestEnum> Test::CopyEnumValuesToVectorView(array_view<TestEnum const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<CompositeType> Test::CopyCompositeStructsToVectorView(array_view<CompositeType const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<IReference<int32_t>> Test::CopyRefsToVectorView(array_view<IReference<int32_t> const> values)
    {
        return CopyToVectorView(values);
    }

    IVectorView<IInspectable> Test::CopyObjectsToVectorView(array_view<IInspectable const> values)
    {
        return CopyToVectorView(values);
    }

    IVector<bool> Test::ReturnSameBoolVector(IVector<bool> const& vector)
    {
        return vector;
    }
    IVector<char16_t> Test::ReturnSameCharVector(IVector<char16_t> const& vector)
    {
        return vector;
    }
    IVector<int32_t> Test::ReturnSameNumericVector(IVector<int32_t> const& vector)
    {
        return vector;
    }
    IVector<hstring> Test::ReturnSameStringVector(IVector<hstring> const& vector)
    {
        return vector;
    }
    IVector<winrt::guid> Test::ReturnSameGuidVector(IVector<winrt::guid> const& vector)
    {
        return vector;
    }
    IVector<TestEnum> Test::ReturnSameEnumVector(IVector<TestEnum> const& vector)
    {
        return vector;
    }
    IVector<CompositeType> Test::ReturnSameCompositeStructVector(IVector<CompositeType> const& vector)
    {
        return vector;
    }
    IVector<IReference<int32_t>> Test::ReturnSameRefVector(IVector<IReference<int32_t>> const& vector)
    {
        return vector;
    }
    IVector<IInspectable> Test::ReturnSameObjectVector(IVector<IInspectable> const& vector)
    {
        return vector;
    }

    IAsyncAction Test::PauseAsync(int32_t milliseconds)
    {
        co_await winrt::resume_background();
        co_await std::chrono::milliseconds{ milliseconds };
    }

    IAsyncActionWithProgress<int32_t> Test::CountToNumberAsync(int32_t value)
    {
        co_await winrt::resume_background();
        auto progress = co_await winrt::get_progress_token();
        for (int32_t i = 0; i <= value; ++i)
        {
            co_await 50ms;
            progress(i);
        }
    }

    IAsyncOperation<int32_t> Test::AddAsync(int32_t lhs, int32_t rhs)
    {
        co_await winrt::resume_background();
        co_await 50ms;
        co_return lhs + rhs;
    }

    IAsyncOperationWithProgress<int32_t, int32_t> Test::CountDoubleAsync(int32_t value)
    {
        co_await winrt::resume_background();
        auto progress = co_await winrt::get_progress_token();
        int32_t result = 0;
        for (; result <= (value * 2); ++result)
        {
            co_await 50ms;
            progress(result);
        }

        co_return result - 1; // Because of the ending '++'
    }

    IAsyncAction Test::ThrowAsyncException()
    {
        co_await winrt::resume_background();
        co_await 50ms;
        throw std::invalid_argument("test");
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

    IReference<bool> Test::RefBooleanProperty()
    {
        return m_refBoolProperty;
    }

    void Test::RefBooleanProperty(IReference<bool> const& value)
    {
        m_refBoolProperty = value;
    }

    IReference<char16_t> Test::RefCharProperty()
    {
        return m_refCharProperty;
    }

    void Test::RefCharProperty(IReference<char16_t> const& value)
    {
        m_refCharProperty = value;
    }

    IReference<int32_t> Test::RefNumericProperty()
    {
        return m_refNumericProperty;
    }

    void Test::RefNumericProperty(IReference<int32_t> const& value)
    {
        m_refNumericProperty = value;
    }

    IReference<TestEnum> Test::RefEnumProperty()
    {
        return m_refEnumProperty;
    }

    void Test::RefEnumProperty(IReference<TestEnum> const& value)
    {
        m_refEnumProperty = value;
    }

    IVector<int32_t> Test::ObjectProperty()
    {
        return m_objectProperty;
    }

    void Test::ObjectProperty(IVector<int32_t> const& value)
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

    com_array<IReference<int32_t>> Test::RefArrayProperty()
    {
        return com_array<IReference<int32_t>>(m_refArrayProperty);
    }

    void Test::RefArrayProperty(array_view<IReference<int32_t> const> value)
    {
        m_refArrayProperty.assign(value.begin(), value.end());
    }

    com_array<IVector<int32_t>> Test::ObjectArrayProperty()
    {
        return com_array<IVector<int32_t>>(m_objectArrayProperty);
    }

    void Test::ObjectArrayProperty(array_view<IVector<int32_t> const> value)
    {
        m_objectArrayProperty.assign(value.begin(), value.end());
    }

    DateTime Test::DateTimeProperty()
    {
        return m_dateTimeProperty;
    }

    void Test::DateTimeProperty(DateTime value)
    {
        m_dateTimeProperty = value;
    }

    hstring Test::DateTimePropertyCppValue()
    {
        return winrt::to_hstring(m_dateTimeProperty.time_since_epoch().count());
    }

    TimeSpan Test::TimeSpanProperty()
    {
        return m_timeSpanProperty;
    }

    void Test::TimeSpanProperty(TimeSpan value)
    {
        m_timeSpanProperty = value;
    }

    hstring Test::TimeSpanPropertyCppValue()
    {
        return winrt::to_hstring(m_timeSpanProperty.count());
    }

    hresult Test::HResultProperty()
    {
        return m_hresultProperty;
    }

    void Test::HResultProperty(hresult value)
    {
        this->m_hresultProperty = value;
    }

    IPropertyValue Test::PropertyValue()
    {
        return m_propertyValue;
    }

    void Test::PropertyValue(IPropertyValue value)
    {
        m_propertyValue = value;
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

    IReference<int32_t> Test::RefOutParam(
        IReference<int32_t> const& value, IReference<int32_t>& doubledValue, IReference<int32_t>& tripledValue)
    {
        doubledValue = value.Value() * 2;
        tripledValue = value.Value() * 3;
        return value.Value() * 4;
    }

    IVector<int32_t> Test::ObjectOutParam(
        IVector<int32_t> const& values, IVector<int32_t>& doubledValues, IVector<int32_t>& tripledValues)
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

    com_array<bool> Test::BoolArrayOutParam(array_view<bool const> values, com_array<bool>& rot1, com_array<bool>& rot2)
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

    com_array<IReference<int32_t>> Test::RefArrayOutParam(array_view<IReference<int32_t> const> values,
        com_array<IReference<int32_t>>& rot1, com_array<IReference<int32_t>>& rot2)
    {
        rot1 = rotate_array(values, 1);
        rot2 = rotate_array(values, 2);
        return reverse_array(values);
    }

    com_array<IVector<int32_t>> Test::ObjectArrayOutParam(
        array_view<IVector<int32_t> const> values, com_array<IVector<int32_t>>& rot1, com_array<IVector<int32_t>>& rot2)
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
            for (auto& byte : next.Data4)
                ++byte;
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
            ++next.Numerics.U8;
            ++next.Numerics.U16;
            ++next.Numerics.U32;
            ++next.Numerics.U64;
            ++next.Numerics.S16;
            ++next.Numerics.S32;
            ++next.Numerics.S64;
            ++next.Numerics.F32;
            ++next.Numerics.F64;
            next.Numerics.Enum = static_cast<TestEnum>(static_cast<int>(next.Numerics.Enum) + 1);
            ++next.Strings.Char;
            next.Strings.String = std::wstring(next.Strings.String.c_str(), next.Strings.String.size()) + L"a";
            ++next.Strings.Guid.Data1;
            ++next.Strings.Guid.Data2;
            ++next.Strings.Guid.Data3;
            for (auto& byte : next.Strings.Guid.Data4)
                ++byte;
            next.Bools.Value = !next.Bools.Value;
        }
    }

    void Test::RefFillParam(array_view<IReference<int32_t>> values)
    {
        std::iota(values.begin(), values.end(), 0);
    }

    void Test::ObjectFillParam(array_view<IVector<int32_t>> values)
    {
        int32_t next = 0;
        std::vector<int32_t> v;
        for (auto& val : values)
        {
            val = single_threaded_vector<int32_t>(std::vector{ v });
            v.push_back(next++);
        }
    }

    winrt::event_token Test::BoolEventHandler(EventHandler<bool> const& handler)
    {
        return m_boolEventSource.add(handler);
    }

    void Test::BoolEventHandler(winrt::event_token const& token) noexcept
    {
        m_boolEventSource.remove(token);
    }

    winrt::event_token Test::CharEventHandler(EventHandler<char16_t> const& handler)
    {
        return m_charEventSource.add(handler);
    }

    void Test::CharEventHandler(winrt::event_token const& token) noexcept
    {
        m_charEventSource.remove(token);
    }

    winrt::event_token Test::NumericEventHandler(EventHandler<int32_t> const& handler)
    {
        return m_numericEventSource.add(handler);
    }

    void Test::NumericEventHandler(winrt::event_token const& token) noexcept
    {
        m_numericEventSource.remove(token);
    }

    winrt::event_token Test::StringEventHandler(EventHandler<hstring> const& handler)
    {
        return m_stringEventSource.add(handler);
    }

    void Test::StringEventHandler(winrt::event_token const& token) noexcept
    {
        m_stringEventSource.remove(token);
    }

    winrt::event_token Test::GuidEventHandler(EventHandler<winrt::guid> const& handler)
    {
        return m_guidEventSource.add(handler);
    }

    void Test::GuidEventHandler(winrt::event_token const& token) noexcept
    {
        m_guidEventSource.remove(token);
    }

    winrt::event_token Test::EnumEventHandler(EventHandler<TestEnum> const& handler)
    {
        return m_enumEventSource.add(handler);
    }

    void Test::EnumEventHandler(winrt::event_token const& token) noexcept
    {
        m_enumEventSource.remove(token);
    }

    winrt::event_token Test::CompositeStructEventHandler(EventHandler<CompositeType> const& handler)
    {
        return m_compositeStructEventSource.add(handler);
    }

    void Test::CompositeStructEventHandler(winrt::event_token const& token) noexcept
    {
        m_compositeStructEventSource.remove(token);
    }

    winrt::event_token Test::RefEventHandler(EventHandler<IReference<int32_t>> const& handler)
    {
        return m_refEventSource.add(handler);
    }

    void Test::RefEventHandler(winrt::event_token const& token) noexcept
    {
        m_refEventSource.remove(token);
    }

    winrt::event_token Test::ObjectEventHandler(EventHandler<IVector<int32_t>> const& handler)
    {
        return m_objectEventSource.add(handler);
    }

    void Test::ObjectEventHandler(winrt::event_token const& token) noexcept
    {
        m_objectEventSource.remove(token);
    }

    void Test::RaiseBoolEvent(bool value)
    {
        m_boolEventSource(*this, value);
    }

    void Test::RaiseCharEvent(char16_t value)
    {
        m_charEventSource(*this, value);
    }

    void Test::RaiseNumericEvent(int32_t value)
    {
        m_numericEventSource(*this, value);
    }

    void Test::RaiseStringEvent(hstring const& value)
    {
        m_stringEventSource(*this, value);
    }

    void Test::RaiseGuidEvent(winrt::guid const& value)
    {
        m_guidEventSource(*this, value);
    }

    void Test::RaiseEnumEvent(TestEnum const& value)
    {
        m_enumEventSource(*this, value);
    }

    void Test::RaiseCompositeStructEvent(CompositeType const& value)
    {
        m_compositeStructEventSource(*this, value);
    }

    void Test::RaiseRefEvent(IReference<int32_t> const& value)
    {
        m_refEventSource(*this, value);
    }

    void Test::RaiseObjectEvent(IVector<int32_t> const& value)
    {
        m_objectEventSource(*this, value);
    }

    Windows::Foundation::Collections::IMap<winrt::hstring, int32_t> Test::CreateStringToNumberMap()
    {
        return single_threaded_map<winrt::hstring, int32_t>();
    }

    template <typename K, typename V>
    struct MapViewImpl : winrt::implements<MapViewImpl<K, V>, IMapView<K, V>>
    {
        MapViewImpl(IMap<K, V> value) : m_value(std::move(value))
        {
        }

        uint32_t Size() const noexcept
        {
            return m_value.Size();
        }

        bool HasKey(K key)
        {
            return m_value.HasKey(key);
        }

        V Lookup(K key)
        {
            return m_value.Lookup(key);
        }

        void Split(IMapView<K, V>& first, IMapView<K, V>& second)
        {
            first = make<MapViewImpl<winrt::hstring, int32_t>>(single_threaded_map<K, V>());
            second = make<MapViewImpl<winrt::hstring, int32_t>>(m_value);
        }

    private:
        IMap<K, V> m_value;
    };

    Windows::Foundation::Collections::IMapView<winrt::hstring, int32_t> Test::CopyToMapView(
        Windows::Foundation::Collections::IMap<winrt::hstring, int32_t> const& stringToNumberMap)
    {
        return make<MapViewImpl<winrt::hstring, int32_t>>(stringToNumberMap);
    }

}
