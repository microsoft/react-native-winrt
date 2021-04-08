#pragma once

#include "Test.g.h"

namespace winrt::TestComponent::implementation
{
    struct Test : TestT<Test>
    {
        Test() : m_constructorParamCount(0)
        {
        }

        Test(int32_t) : m_constructorParamCount(1)
        {
        }

        Test(int32_t, hstring const&) : m_constructorParamCount(2)
        {
        }

        int32_t ConstructorParamCount()
        {
            return m_constructorParamCount;
        }

        static void LogFailures(hstring const& failures);

        static bool StaticBoolProperty();
        static void StaticBoolProperty(bool value);

        static bool StaticOr(bool lhs, bool rhs);
        static bool StaticOrAll(array_view<bool const> values);
        static int32_t StaticAdd(int32_t lhs, int32_t rhs);
        static int32_t StaticAddAll(array_view<int32_t const> values);
        static hstring StaticAppend(hstring const& a, char16_t b, hstring const& c);
        static hstring StaticAppendAll(array_view<hstring const> values);

        static hstring StaticArityOverload();
        static hstring StaticArityOverload(hstring const& str);
        static hstring StaticArityOverload(hstring const& first, hstring const& second);
        static hstring StaticDefaultOverload(hstring const& str, bool val);
        static hstring StaticDefaultOverload(hstring const& str, int32_t repeat);
        static hstring StaticDefaultOverload(hstring const& str, uint32_t val);
        static hstring StaticOutParamOverload(hstring const& str);
        static hstring StaticOutParamOverload(hstring const& str, hstring& outParam);
        static hstring StaticOutParamOverload(hstring const& str, hstring const& other);
        static hstring StaticContractArityOverload();
        static hstring StaticContractArityOverload(hstring const& str);
        static hstring StaticContractDefaultOverloadV1(hstring const& str, bool val);
        static hstring StaticContractDefaultOverloadV1(hstring const& str, int32_t repeat);
        static hstring StaticContractDefaultOverloadV1(hstring const& str, uint32_t val);
        static hstring StaticContractDefaultOverloadV1(hstring const& str, double val);
        static hstring StaticContractDefaultOverloadV2(hstring const& str, double val);
        static hstring StaticContractDefaultOverloadV2(hstring const& str, bool val);
        static hstring StaticContractDefaultOverloadV2(hstring const& str, int32_t val);
        static hstring StaticContractDefaultOverloadV2(hstring const& str, uint32_t val);
        static hstring StaticContractOutParamOverloadV1(hstring const& str);
        static hstring StaticContractOutParamOverloadV1(hstring const& str, hstring& outParam);
        static hstring StaticContractOutParamOverloadV1(hstring const& str, hstring const& other);
        static hstring StaticContractOutParamOverloadV1(char16_t ch);
        static hstring StaticContractOutParamOverloadV2(hstring const& str);
        static hstring StaticContractOutParamOverloadV2(hstring const& str, hstring& outParam);
        static hstring StaticContractOutParamOverloadV2(hstring const& str, hstring const& other);
        static hstring StaticContractOutParamOverloadV2(char16_t ch);

        static bool StaticBoolOutParam(bool lhs, bool rhs, bool& andResult, bool& or);
        static char16_t StaticCharOutParam(char16_t value, char16_t& next, char16_t& prev);
        static int32_t StaticNumericOutParam(int32_t value, int32_t& doubledValue, int32_t& tripledValue);
        static hstring StaticStringOutParam(hstring const& value, hstring& lower, hstring& upper);
        static winrt::guid StaticGuidOutParam(winrt::guid const& value, winrt::guid& zero, winrt::guid& allSet);
        static TestEnum StaticEnumOutParam(TestEnum const& value, TestEnum& plusOne, TestEnum& plusTwo);
        static CompositeType StaticCompositeStructOutParam(
            CompositeType const& input, CompositeType& first, CompositeType& second);
        static Windows::Foundation::IReference<int32_t> StaticRefOutParam(
            Windows::Foundation::IReference<int32_t> const& value,
            Windows::Foundation::IReference<int32_t>& doubledValue,
            Windows::Foundation::IReference<int32_t>& tripledValue);
        static Windows::Foundation::Collections::IVector<int32_t> StaticObjectOutParam(
            Windows::Foundation::Collections::IVector<int32_t> const& values,
            Windows::Foundation::Collections::IVector<int32_t>& doubledValues,
            Windows::Foundation::Collections::IVector<int32_t>& tripledValues);

        static com_array<bool> StaticBoolArrayOutParam(
            array_view<bool const> values, com_array<bool>& rot1, com_array<bool>& rot2);
        static com_array<char16_t> StaticCharArrayOutParam(
            array_view<char16_t const> values, com_array<char16_t>& rot1, com_array<char16_t>& rot2);
        static com_array<int32_t> StaticNumericArrayOutParam(
            array_view<int32_t const> values, com_array<int32_t>& rot1, com_array<int32_t>& rot2);
        static com_array<hstring> StaticStringArrayOutParam(
            array_view<hstring const> values, com_array<hstring>& rot1, com_array<hstring>& rot2);
        static com_array<winrt::guid> StaticGuidArrayOutParam(
            array_view<winrt::guid const> values, com_array<winrt::guid>& rot1, com_array<winrt::guid>& rot2);
        static com_array<TestEnum> StaticEnumArrayOutParam(
            array_view<TestEnum const> values, com_array<TestEnum>& rot1, com_array<TestEnum>& rot2);
        static com_array<CompositeType> StaticCompositeStructArrayOutParam(
            array_view<CompositeType const> values, com_array<CompositeType>& rot1, com_array<CompositeType>& rot2);
        static com_array<Windows::Foundation::IReference<int32_t>> StaticRefArrayOutParam(
            array_view<Windows::Foundation::IReference<int32_t> const> values,
            com_array<Windows::Foundation::IReference<int32_t>>& rot1,
            com_array<Windows::Foundation::IReference<int32_t>>& rot2);
        static com_array<Windows::Foundation::Collections::IVector<int32_t>> StaticObjectArrayOutParam(
            array_view<Windows::Foundation::Collections::IVector<int32_t> const> values,
            com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot1,
            com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot2);

        static void StaticBoolFillParam(array_view<bool> values);
        static void StaticCharFillParam(array_view<char16_t> values);
        static void StaticNumericFillParam(array_view<int32_t> values);
        static void StaticStringFillParam(array_view<hstring> values);
        static void StaticGuidFillParam(array_view<winrt::guid> values);
        static void StaticEnumFillParam(array_view<TestEnum> values);
        static void StaticCompositeStructFillParam(array_view<CompositeType> values);
        static void StaticRefFillParam(array_view<Windows::Foundation::IReference<int32_t>> values);
        static void StaticObjectFillParam(array_view<Windows::Foundation::Collections::IVector<int32_t>> values);

        static winrt::event_token StaticBoolEventHandler(Windows::Foundation::EventHandler<bool> const& handler);
        static void StaticBoolEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticCharEventHandler(Windows::Foundation::EventHandler<char16_t> const& handler);
        static void StaticCharEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticNumericEventHandler(Windows::Foundation::EventHandler<int32_t> const& handler);
        static void StaticNumericEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticStringEventHandler(Windows::Foundation::EventHandler<hstring> const& handler);
        static void StaticStringEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticGuidEventHandler(Windows::Foundation::EventHandler<winrt::guid> const& handler);
        static void StaticGuidEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticEnumEventHandler(Windows::Foundation::EventHandler<TestEnum> const& handler);
        static void StaticEnumEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticCompositeStructEventHandler(
            Windows::Foundation::EventHandler<CompositeType> const& handler);
        static void StaticCompositeStructEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticRefEventHandler(
            Windows::Foundation::EventHandler<Windows::Foundation::IReference<int32_t>> const& handler);
        static void StaticRefEventHandler(winrt::event_token const& token) noexcept;
        static winrt::event_token StaticObjectEventHandler(
            Windows::Foundation::EventHandler<Windows::Foundation::Collections::IVector<int32_t>> const& handler);
        static void StaticObjectEventHandler(winrt::event_token const& token) noexcept;

        static void RaiseStaticBoolEvent(bool value);
        static void RaiseStaticCharEvent(char16_t value);
        static void RaiseStaticNumericEvent(int32_t value);
        static void RaiseStaticStringEvent(hstring const& value);
        static void RaiseStaticGuidEvent(winrt::guid const& value);
        static void RaiseStaticEnumEvent(TestEnum const& value);
        static void RaiseStaticCompositeStructEvent(CompositeType const& value);
        static void RaiseStaticRefEvent(Windows::Foundation::IReference<int32_t> const& value);
        static void RaiseStaticObjectEvent(Windows::Foundation::Collections::IVector<int32_t> const& value);

        static bool StaticInvokeBoolDelegate(bool inputValue, BoolDelegate const& targetFn);
        static char16_t StaticInvokeCharDelegate(char16_t inputValue, CharDelegate const& targetFn);
        static int32_t StaticInvokeNumericDelegate(int32_t inputValue, NumericDelegate const& targetFn);
        static hstring StaticInvokeStringDelegate(hstring const& inputValue, StringDelegate const& targetFn);
        static winrt::guid StaticInvokeGuidDelegate(winrt::guid const& inputValue, GuidDelegate const& targetFn);
        static TestEnum StaticInvokeEnumDelegate(TestEnum const& inputValue, EnumDelegate const& targetFn);
        static CompositeType StaticInvokeCompositeStructDelegate(
            CompositeType const& inputValue, CompositeStructDelegate const& targetFn);
        static Windows::Foundation::IReference<int32_t> StaticInvokeRefDelegate(
            Windows::Foundation::IReference<int32_t> const& inputValue, RefDelegate const& targetFn);
        static Windows::Foundation::Collections::IVector<int32_t> StaticInvokeObjectDelegate(
            Windows::Foundation::Collections::IVector<int32_t> const& inputValue, ObjectDelegate const& targetFn);

        static bool StaticInvokeBoolDelegateWithOutParam(bool inputValue, BoolDelegateWithOutParam const& targetFn);
        static char16_t StaticInvokeCharDelegateWithOutParam(
            char16_t inputValue, CharDelegateWithOutParam const& targetFn);
        static int32_t StaticInvokeNumericDelegateWithOutParam(
            int32_t inputValue, NumericDelegateWithOutParam const& targetFn);
        static hstring StaticInvokeStringDelegateWithOutParam(
            hstring const& inputValue, StringDelegateWithOutParam const& targetFn);
        static winrt::guid StaticInvokeGuidDelegateWithOutParam(
            winrt::guid const& inputValue, GuidDelegateWithOutParam const& targetFn);
        static TestEnum StaticInvokeEnumDelegateWithOutParam(
            TestEnum const& inputValue, EnumDelegateWithOutParam const& targetFn);
        static CompositeType StaticInvokeCompositeStructDelegateWithOutParam(
            CompositeType const& inputValue, CompositeStructDelegateWithOutParam const& targetFn);
        static Windows::Foundation::IReference<int32_t> StaticInvokeRefDelegateWithOutParam(
            Windows::Foundation::IReference<int32_t> const& inputValue, RefDelegateWithOutParam const& targetFn);
        static Windows::Foundation::Collections::IVector<int32_t> StaticInvokeObjectDelegateWithOutParam(
            Windows::Foundation::Collections::IVector<int32_t> const& inputValue,
            ObjectDelegateWithOutParam const& targetFn);

        static bool StaticInvokeBoolArrayDelegate(array_view<bool const> values, BoolArrayDelegate const& targetFn);
        static bool StaticInvokeCharArrayDelegate(array_view<char16_t const> values, CharArrayDelegate const& targetFn);
        static bool StaticInvokeNumericArrayDelegate(
            array_view<int32_t const> values, NumericArrayDelegate const& targetFn);
        static bool StaticInvokeStringArrayDelegate(
            array_view<hstring const> values, StringArrayDelegate const& targetFn);
        static bool StaticInvokeGuidArrayDelegate(
            array_view<winrt::guid const> values, GuidArrayDelegate const& targetFn);
        static bool StaticInvokeEnumArrayDelegate(array_view<TestEnum const> values, EnumArrayDelegate const& targetFn);
        static bool StaticInvokeCompositeStructArrayDelegate(
            array_view<CompositeType const> values, CompositeStructArrayDelegate const& targetFn);
        static bool StaticInvokeRefArrayDelegate(
            array_view<Windows::Foundation::IReference<int32_t> const> values, RefArrayDelegate const& targetFn);
        static bool StaticInvokeObjectArrayDelegate(
            array_view<Windows::Foundation::Collections::IVector<int32_t> const> values,
            ObjectArrayDelegate const& targetFn);

        static Windows::Foundation::Collections::IVector<bool> CopyBoolsToVector(array_view<bool const> values);
        static Windows::Foundation::Collections::IVector<char16_t> CopyCharsToVector(array_view<char16_t const> values);
        static Windows::Foundation::Collections::IVector<int32_t> CopyNumericsToVector(
            array_view<int32_t const> values);
        static Windows::Foundation::Collections::IVector<hstring> CopyStringsToVector(array_view<hstring const> values);
        static Windows::Foundation::Collections::IVector<winrt::guid> CopyGuidsToVector(
            array_view<winrt::guid const> values);
        static Windows::Foundation::Collections::IVector<TestEnum> CopyEnumValuesToVector(
            array_view<TestEnum const> values);
        static Windows::Foundation::Collections::IVector<CompositeType> CopyCompositeStructsToVector(
            array_view<CompositeType const> values);
        static Windows::Foundation::Collections::IVector<Windows::Foundation::IReference<int32_t>> CopyRefsToVector(
            array_view<Windows::Foundation::IReference<int32_t> const> values);
        static Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> CopyObjectsToVector(
            array_view<Windows::Foundation::IInspectable const> values);

        static Windows::Foundation::Collections::IVector<bool> ReturnSameBoolVector(
            Windows::Foundation::Collections::IVector<bool> const& vector);
        static Windows::Foundation::Collections::IVector<char16_t> ReturnSameCharVector(
            Windows::Foundation::Collections::IVector<char16_t> const& vector);
        static Windows::Foundation::Collections::IVector<int32_t> ReturnSameNumericVector(
            Windows::Foundation::Collections::IVector<int32_t> const& vector);
        static Windows::Foundation::Collections::IVector<hstring> ReturnSameStringVector(
            Windows::Foundation::Collections::IVector<hstring> const& vector);
        static Windows::Foundation::Collections::IVector<winrt::guid> ReturnSameGuidVector(
            Windows::Foundation::Collections::IVector<winrt::guid> const& vector);
        static Windows::Foundation::Collections::IVector<TestEnum> ReturnSameEnumVector(
            Windows::Foundation::Collections::IVector<TestEnum> const& vector);
        static Windows::Foundation::Collections::IVector<CompositeType> ReturnSameCompositeStructVector(
            Windows::Foundation::Collections::IVector<CompositeType> const& vector);
        static Windows::Foundation::Collections::IVector<Windows::Foundation::IReference<int32_t>> ReturnSameRefVector(
            Windows::Foundation::Collections::IVector<Windows::Foundation::IReference<int32_t>> const& vector);
        static Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> ReturnSameObjectVector(
            Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> const& vector);

        static Windows::Foundation::IAsyncAction PauseAsync(int32_t milliseconds);
        static Windows::Foundation::IAsyncActionWithProgress<int32_t> CountToNumberAsync(int32_t value);
        static Windows::Foundation::IAsyncOperation<int32_t> AddAsync(int32_t lhs, int32_t rhs);
        static Windows::Foundation::IAsyncOperationWithProgress<int32_t, int32_t> CountDoubleAsync(int32_t value);
        static Windows::Foundation::IAsyncAction ThrowAsyncException();

        bool BoolProperty();
        void BoolProperty(bool value);
        char16_t CharProperty();
        void CharProperty(char16_t value);
        uint8_t U8Property();
        void U8Property(uint8_t value);
        uint16_t U16Property();
        void U16Property(uint16_t value);
        uint32_t U32Property();
        void U32Property(uint32_t value);
        uint64_t U64Property();
        void U64Property(uint64_t value);
        int16_t S16Property();
        void S16Property(int16_t value);
        int32_t S32Property();
        void S32Property(int32_t value);
        int64_t S64Property();
        void S64Property(int64_t value);
        float F32Property();
        void F32Property(float value);
        double F64Property();
        void F64Property(double value);
        hstring StringProperty();
        void StringProperty(hstring const& value);
        winrt::guid GuidProperty();
        void GuidProperty(winrt::guid const& value);
        TestEnum EnumProperty();
        void EnumProperty(TestEnum const& value);
        NumericTypes NumericsStructProperty();
        void NumericsStructProperty(NumericTypes const& value);
        StringTypes StringsStructProperty();
        void StringsStructProperty(StringTypes const& value);
        BooleanTypes BooleansStructProperty();
        void BooleansStructProperty(BooleanTypes const& value);
        CompositeType CompositeStructProperty();
        void CompositeStructProperty(CompositeType const& value);
        Windows::Foundation::IReference<bool> RefBooleanProperty();
        void RefBooleanProperty(Windows::Foundation::IReference<bool> const& value);
        Windows::Foundation::IReference<char16_t> RefCharProperty();
        void RefCharProperty(Windows::Foundation::IReference<char16_t> const& value);
        Windows::Foundation::IReference<int32_t> RefNumericProperty();
        void RefNumericProperty(Windows::Foundation::IReference<int32_t> const& value);
        Windows::Foundation::IReference<TestEnum> RefEnumProperty();
        void RefEnumProperty(Windows::Foundation::IReference<TestEnum> const& value);
        Windows::Foundation::Collections::IVector<int32_t> ObjectProperty();
        void ObjectProperty(Windows::Foundation::Collections::IVector<int32_t> const& value);
        Windows::Foundation::DateTime DateTimeProperty();
        void DateTimeProperty(Windows::Foundation::DateTime value);
        hstring DateTimePropertyCppValue();
        Windows::Foundation::TimeSpan TimeSpanProperty();
        void TimeSpanProperty(Windows::Foundation::TimeSpan value);
        hstring TimeSpanPropertyCppValue();
        hresult HResultProperty();
        void HResultProperty(hresult value);
        Windows::Foundation::IPropertyValue PropertyValue();
        void PropertyValue(Windows::Foundation::IPropertyValue value);

        com_array<bool> BooleanArrayProperty();
        void BooleanArrayProperty(array_view<bool const> value);
        com_array<char16_t> CharArrayProperty();
        void CharArrayProperty(array_view<char16_t const> value);
        com_array<int32_t> NumericArrayProperty();
        void NumericArrayProperty(array_view<int32_t const> value);
        com_array<hstring> StringArrayProperty();
        void StringArrayProperty(array_view<hstring const> value);
        com_array<winrt::guid> GuidArrayProperty();
        void GuidArrayProperty(array_view<winrt::guid const> value);
        com_array<TestEnum> EnumArrayProperty();
        void EnumArrayProperty(array_view<TestEnum const> value);
        com_array<CompositeType> CompositeStructArrayProperty();
        void CompositeStructArrayProperty(array_view<CompositeType const> value);
        com_array<Windows::Foundation::IReference<int32_t>> RefArrayProperty();
        void RefArrayProperty(array_view<Windows::Foundation::IReference<int32_t> const> value);
        com_array<Windows::Foundation::Collections::IVector<int32_t>> ObjectArrayProperty();
        void ObjectArrayProperty(array_view<Windows::Foundation::Collections::IVector<int32_t> const> value);

        bool Or(bool lhs, bool rhs);
        bool OrAll(array_view<bool const> values);
        int32_t Add(int32_t lhs, int32_t rhs);
        int32_t AddAll(array_view<int32_t const> values);
        hstring Append(hstring const& a, char16_t b, hstring const& c);
        hstring AppendAll(array_view<hstring const> values);

        hstring ArityOverload();
        hstring ArityOverload(hstring const& str);
        hstring ArityOverload(hstring const& first, hstring const& second);
        hstring DefaultOverload(hstring const& str, bool val);
        hstring DefaultOverload(hstring const& str, int32_t repeat);
        hstring DefaultOverload(hstring const& str, uint32_t val);
        hstring OutParamOverload(hstring const& str);
        hstring OutParamOverload(hstring const& str, hstring& outParam);
        hstring OutParamOverload(hstring const& str, hstring const& other);
        hstring ContractArityOverload();
        hstring ContractArityOverload(hstring const& str);
        hstring ContractDefaultOverloadV1(hstring const& str, bool val);
        hstring ContractDefaultOverloadV1(hstring const& str, int32_t repeat);
        hstring ContractDefaultOverloadV1(hstring const& str, uint32_t val);
        hstring ContractDefaultOverloadV1(hstring const& str, double val);
        hstring ContractDefaultOverloadV2(hstring const& str, double val);
        hstring ContractDefaultOverloadV2(hstring const& str, bool val);
        hstring ContractDefaultOverloadV2(hstring const& str, int32_t val);
        hstring ContractDefaultOverloadV2(hstring const& str, uint32_t val);
        hstring ContractOutParamOverloadV1(hstring const& str);
        hstring ContractOutParamOverloadV1(hstring const& str, hstring& outParam);
        hstring ContractOutParamOverloadV1(hstring const& str, hstring const& other);
        hstring ContractOutParamOverloadV1(char16_t ch);
        hstring ContractOutParamOverloadV2(hstring const& str);
        hstring ContractOutParamOverloadV2(hstring const& str, hstring& outParam);
        hstring ContractOutParamOverloadV2(hstring const& str, hstring const& other);
        hstring ContractOutParamOverloadV2(char16_t ch);

        bool BoolOutParam(bool lhs, bool rhs, bool& andResult, bool& or);
        char16_t CharOutParam(char16_t value, char16_t& next, char16_t& prev);
        int32_t NumericOutParam(int32_t value, int32_t& doubledValue, int32_t& tripledValue);
        hstring StringOutParam(hstring const& value, hstring& lower, hstring& upper);
        winrt::guid GuidOutParam(winrt::guid const& value, winrt::guid& zero, winrt::guid& allSet);
        TestEnum EnumOutParam(TestEnum const& value, TestEnum& plusOne, TestEnum& plusTwo);
        CompositeType CompositeStructOutParam(CompositeType const& input, CompositeType& first, CompositeType& second);
        Windows::Foundation::IReference<int32_t> RefOutParam(Windows::Foundation::IReference<int32_t> const& value,
            Windows::Foundation::IReference<int32_t>& doubledValue,
            Windows::Foundation::IReference<int32_t>& tripledValue);
        Windows::Foundation::Collections::IVector<int32_t> ObjectOutParam(
            Windows::Foundation::Collections::IVector<int32_t> const& values,
            Windows::Foundation::Collections::IVector<int32_t>& doubledValues,
            Windows::Foundation::Collections::IVector<int32_t>& tripledValues);

        com_array<bool> BoolArrayOutParam(array_view<bool const> values, com_array<bool>& rot1, com_array<bool>& rot2);
        com_array<char16_t> CharArrayOutParam(
            array_view<char16_t const> values, com_array<char16_t>& rot1, com_array<char16_t>& rot2);
        com_array<int32_t> NumericArrayOutParam(
            array_view<int32_t const> values, com_array<int32_t>& rot1, com_array<int32_t>& rot2);
        com_array<hstring> StringArrayOutParam(
            array_view<hstring const> values, com_array<hstring>& rot1, com_array<hstring>& rot2);
        com_array<winrt::guid> GuidArrayOutParam(
            array_view<winrt::guid const> values, com_array<winrt::guid>& rot1, com_array<winrt::guid>& rot2);
        com_array<TestEnum> EnumArrayOutParam(
            array_view<TestEnum const> values, com_array<TestEnum>& rot1, com_array<TestEnum>& rot2);
        com_array<CompositeType> CompositeStructArrayOutParam(
            array_view<CompositeType const> values, com_array<CompositeType>& rot1, com_array<CompositeType>& rot2);
        com_array<Windows::Foundation::IReference<int32_t>> RefArrayOutParam(
            array_view<Windows::Foundation::IReference<int32_t> const> values,
            com_array<Windows::Foundation::IReference<int32_t>>& rot1,
            com_array<Windows::Foundation::IReference<int32_t>>& rot2);
        com_array<Windows::Foundation::Collections::IVector<int32_t>> ObjectArrayOutParam(
            array_view<Windows::Foundation::Collections::IVector<int32_t> const> values,
            com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot1,
            com_array<Windows::Foundation::Collections::IVector<int32_t>>& rot2);

        void BoolFillParam(array_view<bool> values);
        void CharFillParam(array_view<char16_t> values);
        void NumericFillParam(array_view<int32_t> values);
        void StringFillParam(array_view<hstring> values);
        void GuidFillParam(array_view<winrt::guid> values);
        void EnumFillParam(array_view<TestEnum> values);
        void CompositeStructFillParam(array_view<CompositeType> values);
        void RefFillParam(array_view<Windows::Foundation::IReference<int32_t>> values);
        void ObjectFillParam(array_view<Windows::Foundation::Collections::IVector<int32_t>> values);

        winrt::event_token BoolEventHandler(Windows::Foundation::EventHandler<bool> const& handler);
        void BoolEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token CharEventHandler(Windows::Foundation::EventHandler<char16_t> const& handler);
        void CharEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token NumericEventHandler(Windows::Foundation::EventHandler<int32_t> const& handler);
        void NumericEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token StringEventHandler(Windows::Foundation::EventHandler<hstring> const& handler);
        void StringEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token GuidEventHandler(Windows::Foundation::EventHandler<winrt::guid> const& handler);
        void GuidEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token EnumEventHandler(Windows::Foundation::EventHandler<TestEnum> const& handler);
        void EnumEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token CompositeStructEventHandler(Windows::Foundation::EventHandler<CompositeType> const& handler);
        void CompositeStructEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token RefEventHandler(
            Windows::Foundation::EventHandler<Windows::Foundation::IReference<int32_t>> const& handler);
        void RefEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token ObjectEventHandler(
            Windows::Foundation::EventHandler<Windows::Foundation::Collections::IVector<int32_t>> const& handler);
        void ObjectEventHandler(winrt::event_token const& token) noexcept;

        void RaiseBoolEvent(bool value);
        void RaiseCharEvent(char16_t value);
        void RaiseNumericEvent(int32_t value);
        void RaiseStringEvent(hstring const& value);
        void RaiseGuidEvent(winrt::guid const& value);
        void RaiseEnumEvent(TestEnum const& value);
        void RaiseCompositeStructEvent(CompositeType const& value);
        void RaiseRefEvent(Windows::Foundation::IReference<int32_t> const& value);
        void RaiseObjectEvent(Windows::Foundation::Collections::IVector<int32_t> const& value);

    private:
        // Static properties
        inline static bool s_boolProperty = false;

        // Static event sources
        inline static event<Windows::Foundation::EventHandler<bool>> s_boolEventSource;
        inline static event<Windows::Foundation::EventHandler<char16_t>> s_charEventSource;
        inline static event<Windows::Foundation::EventHandler<int32_t>> s_numericEventSource;
        inline static event<Windows::Foundation::EventHandler<hstring>> s_stringEventSource;
        inline static event<Windows::Foundation::EventHandler<winrt::guid>> s_guidEventSource;
        inline static event<Windows::Foundation::EventHandler<TestEnum>> s_enumEventSource;
        inline static event<Windows::Foundation::EventHandler<CompositeType>> s_compositeStructEventSource;
        inline static event<Windows::Foundation::EventHandler<Windows::Foundation::IReference<int32_t>>>
            s_refEventSource;
        inline static event<Windows::Foundation::EventHandler<Windows::Foundation::Collections::IVector<int32_t>>>
            s_objectEventSource;

        int32_t m_constructorParamCount;

        // Non-static properties
        bool m_boolProperty = false;
        char16_t m_charProperty = 0;
        uint8_t m_u8Property = 0;
        uint16_t m_u16Property = 0;
        uint32_t m_u32Property = 0;
        uint64_t m_u64Property = 0;
        int16_t m_s16Property = 0;
        int32_t m_s32Property = 0;
        int64_t m_s64Property = 0;
        float m_f32Property = 0;
        double m_f64Property = 0;
        hstring m_stringProperty;
        guid m_guidProperty = {};
        TestEnum m_enumProperty = TestEnum::First;
        NumericTypes m_numericsStructProperty = {};
        StringTypes m_stringsStructProperty = {};
        BooleanTypes m_booleansStructProperty = {};
        CompositeType m_compositeStructProperty = {};
        Windows::Foundation::IReference<bool> m_refBoolProperty;
        Windows::Foundation::IReference<char16_t> m_refCharProperty;
        Windows::Foundation::IReference<int32_t> m_refNumericProperty;
        Windows::Foundation::IReference<TestEnum> m_refEnumProperty;
        Windows::Foundation::Collections::IVector<int32_t> m_objectProperty;
        std::vector<bool> m_boolArrayProperty;
        std::vector<char16_t> m_charArrayProperty;
        std::vector<int32_t> m_numericArrayProperty;
        std::vector<hstring> m_stringArrayProperty;
        std::vector<guid> m_guidArrayProperty;
        std::vector<TestEnum> m_enumArrayProperty;
        std::vector<CompositeType> m_compositeStructArrayProperty;
        std::vector<Windows::Foundation::IReference<int32_t>> m_refArrayProperty;
        std::vector<Windows::Foundation::Collections::IVector<int32_t>> m_objectArrayProperty;
        Windows::Foundation::DateTime m_dateTimeProperty;
        Windows::Foundation::TimeSpan m_timeSpanProperty;
        hresult m_hresultProperty;
        Windows::Foundation::IPropertyValue m_propertyValue;

        // Non-static event sources
        event<Windows::Foundation::EventHandler<bool>> m_boolEventSource;
        event<Windows::Foundation::EventHandler<char16_t>> m_charEventSource;
        event<Windows::Foundation::EventHandler<int32_t>> m_numericEventSource;
        event<Windows::Foundation::EventHandler<hstring>> m_stringEventSource;
        event<Windows::Foundation::EventHandler<winrt::guid>> m_guidEventSource;
        event<Windows::Foundation::EventHandler<TestEnum>> m_enumEventSource;
        event<Windows::Foundation::EventHandler<CompositeType>> m_compositeStructEventSource;
        event<Windows::Foundation::EventHandler<Windows::Foundation::IReference<int32_t>>> m_refEventSource;
        event<Windows::Foundation::EventHandler<Windows::Foundation::Collections::IVector<int32_t>>>
            m_objectEventSource;
    };
}
namespace winrt::TestComponent::factory_implementation
{
    struct Test : TestT<Test, implementation::Test>
    {
    };
}
