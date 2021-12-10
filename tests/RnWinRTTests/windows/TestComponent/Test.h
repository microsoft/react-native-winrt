// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "Test.g.h"
#include "TestObject.g.h"

namespace winrt::TestComponent::implementation
{
    struct TestObject : TestObjectT<TestObject, ITestInterface>
    {
        TestObject(int32_t val) : m_value(val)
        {
        }

        int32_t Value()
        {
            return m_value;
        }

        int32_t MagicValue()
        {
            return 42;
        }

    private:
        int32_t m_value;
    };

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
        static char16_t StaticCharProperty();
        static void StaticCharProperty(char16_t value);
        static uint8_t StaticU8Property();
        static void StaticU8Property(uint8_t value);
        static uint16_t StaticU16Property();
        static void StaticU16Property(uint16_t value);
        static uint32_t StaticU32Property();
        static void StaticU32Property(uint32_t value);
        static uint64_t StaticU64Property();
        static void StaticU64Property(uint64_t value);
        static int16_t StaticS16Property();
        static void StaticS16Property(int16_t value);
        static int32_t StaticS32Property();
        static void StaticS32Property(int32_t value);
        static int64_t StaticS64Property();
        static void StaticS64Property(int64_t value);
        static float StaticF32Property();
        static void StaticF32Property(float value);
        static double StaticF64Property();
        static void StaticF64Property(double value);
        static hstring StaticStringProperty();
        static void StaticStringProperty(hstring const& value);
        static winrt::guid StaticGuidProperty();
        static void StaticGuidProperty(winrt::guid const& value);
        static TestEnum StaticEnumProperty();
        static void StaticEnumProperty(TestEnum const& value);
        static NumericTypes StaticNumericsStructProperty();
        static void StaticNumericsStructProperty(NumericTypes const& value);
        static StringTypes StaticStringsStructProperty();
        static void StaticStringsStructProperty(StringTypes const& value);
        static BooleanTypes StaticBooleansStructProperty();
        static void StaticBooleansStructProperty(BooleanTypes const& value);
        static CompositeType StaticCompositeStructProperty();
        static void StaticCompositeStructProperty(CompositeType const& value);
        static Windows::Foundation::IReference<bool> StaticRefBooleanProperty();
        static void StaticRefBooleanProperty(Windows::Foundation::IReference<bool> const& value);
        static Windows::Foundation::IReference<char16_t> StaticRefCharProperty();
        static void StaticRefCharProperty(Windows::Foundation::IReference<char16_t> const& value);
        static Windows::Foundation::IReference<int32_t> StaticRefNumericProperty();
        static void StaticRefNumericProperty(Windows::Foundation::IReference<int32_t> const& value);
        static Windows::Foundation::IReference<TestEnum> StaticRefEnumProperty();
        static void StaticRefEnumProperty(Windows::Foundation::IReference<TestEnum> const& value);
        static TestComponent::TestObject StaticObjectProperty();
        static void StaticObjectProperty(TestComponent::TestObject const& value);

        static com_array<bool> StaticBooleanArrayProperty();
        static void StaticBooleanArrayProperty(array_view<bool const> value);
        static com_array<char16_t> StaticCharArrayProperty();
        static void StaticCharArrayProperty(array_view<char16_t const> value);
        static com_array<int32_t> StaticNumericArrayProperty();
        static void StaticNumericArrayProperty(array_view<int32_t const> value);
        static com_array<hstring> StaticStringArrayProperty();
        static void StaticStringArrayProperty(array_view<hstring const> value);
        static com_array<winrt::guid> StaticGuidArrayProperty();
        static void StaticGuidArrayProperty(array_view<winrt::guid const> value);
        static com_array<TestEnum> StaticEnumArrayProperty();
        static void StaticEnumArrayProperty(array_view<TestEnum const> value);
        static com_array<CompositeType> StaticCompositeStructArrayProperty();
        static void StaticCompositeStructArrayProperty(array_view<CompositeType const> value);
        static com_array<Windows::Foundation::IReference<int32_t>> StaticRefArrayProperty();
        static void StaticRefArrayProperty(array_view<Windows::Foundation::IReference<int32_t> const> value);
        static com_array<TestComponent::TestObject> StaticObjectArrayProperty();
        static void StaticObjectArrayProperty(array_view<TestComponent::TestObject const> value);

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
        static TestComponent::TestObject StaticObjectOutParam(TestComponent::TestObject const& value,
            TestComponent::TestObject& doubledValue, TestComponent::TestObject& tripledValue);

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
        static com_array<TestComponent::TestObject> StaticObjectArrayOutParam(
            array_view<TestComponent::TestObject const> values, com_array<TestComponent::TestObject>& rot1,
            com_array<TestComponent::TestObject>& rot2);

        static void StaticBoolFillParam(array_view<bool> values);
        static void StaticCharFillParam(array_view<char16_t> values);
        static void StaticNumericFillParam(array_view<int32_t> values);
        static void StaticStringFillParam(array_view<hstring> values);
        static void StaticGuidFillParam(array_view<winrt::guid> values);
        static void StaticEnumFillParam(array_view<TestEnum> values);
        static void StaticCompositeStructFillParam(array_view<CompositeType> values);
        static void StaticRefFillParam(array_view<Windows::Foundation::IReference<int32_t>> values);
        static void StaticObjectFillParam(array_view<TestComponent::TestObject> values);

        static uint32_t StaticInterwovenParams(bool inBool, bool& outBool, int32_t inNumeric, int32_t& outNumeric,
            array_view<int32_t const> inArray, com_array<int32_t>& outArray, array_view<int32_t> refArray);

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
            Windows::Foundation::EventHandler<TestComponent::TestObject> const& handler);
        static void StaticObjectEventHandler(winrt::event_token const& token) noexcept;

        static void RaiseStaticBoolEvent(bool value);
        static void RaiseStaticCharEvent(char16_t value);
        static void RaiseStaticNumericEvent(int32_t value);
        static void RaiseStaticStringEvent(hstring const& value);
        static void RaiseStaticGuidEvent(winrt::guid const& value);
        static void RaiseStaticEnumEvent(TestEnum const& value);
        static void RaiseStaticCompositeStructEvent(CompositeType const& value);
        static void RaiseStaticRefEvent(Windows::Foundation::IReference<int32_t> const& value);
        static void RaiseStaticObjectEvent(TestComponent::TestObject const& value);

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
        static TestComponent::TestObject StaticInvokeObjectDelegate(
            TestComponent::TestObject const& inputValue, ObjectDelegate const& targetFn);

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
        static TestComponent::TestObject StaticInvokeObjectDelegateWithOutParam(
            TestComponent::TestObject const& inputValue, ObjectDelegateWithOutParam const& targetFn);

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
            array_view<TestComponent::TestObject const> values, ObjectArrayDelegate const& targetFn);

        static bool StaticInvokeInterwovenDelegate(
            bool inBool, int32_t inNumeric, array_view<int32_t const> inArray, InterwovenDelegate const& targetFn);

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
        static Windows::Foundation::Collections::IVector<TestComponent::TestObject> CopyObjectsToVector(
            array_view<TestComponent::TestObject const> values);

        static Windows::Foundation::Collections::IVectorView<bool> CopyBoolsToVectorView(array_view<bool const> values);
        static Windows::Foundation::Collections::IVectorView<char16_t> CopyCharsToVectorView(
            array_view<char16_t const> values);
        static Windows::Foundation::Collections::IVectorView<int32_t> CopyNumericsToVectorView(
            array_view<int32_t const> values);
        static Windows::Foundation::Collections::IVectorView<hstring> CopyStringsToVectorView(
            array_view<hstring const> values);
        static Windows::Foundation::Collections::IVectorView<winrt::guid> CopyGuidsToVectorView(
            array_view<winrt::guid const> values);
        static Windows::Foundation::Collections::IVectorView<TestEnum> CopyEnumValuesToVectorView(
            array_view<TestEnum const> values);
        static Windows::Foundation::Collections::IVectorView<CompositeType> CopyCompositeStructsToVectorView(
            array_view<CompositeType const> values);
        static Windows::Foundation::Collections::IVectorView<Windows::Foundation::IReference<int32_t>>
        CopyRefsToVectorView(array_view<Windows::Foundation::IReference<int32_t> const> values);
        static Windows::Foundation::Collections::IVectorView<Windows::Foundation::IInspectable> CopyObjectsToVectorView(
            array_view<Windows::Foundation::IInspectable const> values);

        static Windows::Foundation::Collections::IObservableVector<int32_t> MakeObservableVector();
        static Windows::Foundation::Collections::IObservableMap<hstring, int32_t> MakeObservableMap();

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
        static Windows::Foundation::Collections::IVector<TestComponent::TestObject> ReturnSameObjectVector(
            Windows::Foundation::Collections::IVector<TestComponent::TestObject> const& vector);

        static Windows::Foundation::Collections::IVectorView<bool> ReturnSameBoolVectorView(
            Windows::Foundation::Collections::IVectorView<bool> const& vector);
        static Windows::Foundation::Collections::IVectorView<char16_t> ReturnSameCharVectorView(
            Windows::Foundation::Collections::IVectorView<char16_t> const& vector);
        static Windows::Foundation::Collections::IVectorView<int32_t> ReturnSameNumericVectorView(
            Windows::Foundation::Collections::IVectorView<int32_t> const& vector);
        static Windows::Foundation::Collections::IVectorView<hstring> ReturnSameStringVectorView(
            Windows::Foundation::Collections::IVectorView<hstring> const& vector);
        static Windows::Foundation::Collections::IVectorView<winrt::guid> ReturnSameGuidVectorView(
            Windows::Foundation::Collections::IVectorView<winrt::guid> const& vector);
        static Windows::Foundation::Collections::IVectorView<TestEnum> ReturnSameEnumVectorView(
            Windows::Foundation::Collections::IVectorView<TestEnum> const& vector);
        static Windows::Foundation::Collections::IVectorView<CompositeType> ReturnSameCompositeStructVectorView(
            Windows::Foundation::Collections::IVectorView<CompositeType> const& vector);
        static Windows::Foundation::Collections::IVectorView<Windows::Foundation::IReference<int32_t>>
        ReturnSameRefVectorView(
            Windows::Foundation::Collections::IVectorView<Windows::Foundation::IReference<int32_t>> const& vector);
        static Windows::Foundation::Collections::IVectorView<TestComponent::TestObject> ReturnSameObjectVectorView(
            Windows::Foundation::Collections::IVectorView<TestComponent::TestObject> const& vector);

        static Windows::Foundation::Collections::IIterable<bool> ReturnSameBoolIterable(
            Windows::Foundation::Collections::IIterable<bool> const& iterable);
        static Windows::Foundation::Collections::IIterable<char16_t> ReturnSameCharIterable(
            Windows::Foundation::Collections::IIterable<char16_t> const& iterable);
        static Windows::Foundation::Collections::IIterable<int32_t> ReturnSameNumericIterable(
            Windows::Foundation::Collections::IIterable<int32_t> const& iterable);
        static Windows::Foundation::Collections::IIterable<hstring> ReturnSameStringIterable(
            Windows::Foundation::Collections::IIterable<hstring> const& iterable);
        static Windows::Foundation::Collections::IIterable<winrt::guid> ReturnSameGuidIterable(
            Windows::Foundation::Collections::IIterable<winrt::guid> const& iterable);
        static Windows::Foundation::Collections::IIterable<TestEnum> ReturnSameEnumIterable(
            Windows::Foundation::Collections::IIterable<TestEnum> const& iterable);
        static Windows::Foundation::Collections::IIterable<CompositeType> ReturnSameCompositeStructIterable(
            Windows::Foundation::Collections::IIterable<CompositeType> const& iterable);
        static Windows::Foundation::Collections::IIterable<Windows::Foundation::IReference<int32_t>>
        ReturnSameRefIterable(
            Windows::Foundation::Collections::IIterable<Windows::Foundation::IReference<int32_t>> const& iterable);
        static Windows::Foundation::Collections::IIterable<TestComponent::TestObject> ReturnSameObjectIterable(
            Windows::Foundation::Collections::IIterable<TestComponent::TestObject> const& iterable);

        static Windows::Foundation::IAsyncAction PauseAsync(int32_t milliseconds);
        static Windows::Foundation::IAsyncActionWithProgress<int32_t> CountToNumberAsync(int32_t value);
        static Windows::Foundation::IAsyncOperation<int32_t> AddAsync(int32_t lhs, int32_t rhs);
        static Windows::Foundation::IAsyncOperationWithProgress<int32_t, int32_t> CountDoubleAsync(int32_t value);
        static Windows::Foundation::IAsyncAction ThrowAsyncException();
        static Windows::Foundation::IAsyncOperation<int32_t> ImmediateReturnAsync(int32_t value);
        static Windows::Foundation::IAsyncOperation<
            Windows::Foundation::Collections::IVectorView<TestComponent::ITestInterface>>
        GetObjectsAsync();

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
        TestComponent::TestObject ObjectProperty();
        void ObjectProperty(TestComponent::TestObject const& value);
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
        winrt::hstring PropertyValueCppType();
        void AssignPropertyValueAsType(
            Windows::Foundation::IPropertyValue const& value, winrt::hstring const& winrtPropertyValueType);

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
        com_array<TestComponent::TestObject> ObjectArrayProperty();
        void ObjectArrayProperty(array_view<TestComponent::TestObject const> value);

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
        TestComponent::TestObject ObjectOutParam(TestComponent::TestObject const& value,
            TestComponent::TestObject& doubledValue, TestComponent::TestObject& tripledValue);

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
        com_array<TestComponent::TestObject> ObjectArrayOutParam(array_view<TestComponent::TestObject const> values,
            com_array<TestComponent::TestObject>& rot1, com_array<TestComponent::TestObject>& rot2);

        void BoolFillParam(array_view<bool> values);
        void CharFillParam(array_view<char16_t> values);
        void NumericFillParam(array_view<int32_t> values);
        void StringFillParam(array_view<hstring> values);
        void GuidFillParam(array_view<winrt::guid> values);
        void EnumFillParam(array_view<TestEnum> values);
        void CompositeStructFillParam(array_view<CompositeType> values);
        void RefFillParam(array_view<Windows::Foundation::IReference<int32_t>> values);
        void ObjectFillParam(array_view<TestComponent::TestObject> values);

        uint32_t InterwovenParams(bool inBool, bool& outBool, int32_t inNumeric, int32_t& outNumeric,
            array_view<int32_t const> inArray, com_array<int32_t>& outArray, array_view<int32_t> refArray);

        winrt::event_token BoolEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, bool> const& handler);
        void BoolEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token CharEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, char16_t> const& handler);
        void CharEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token NumericEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, int32_t> const& handler);
        void NumericEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token StringEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, hstring> const& handler);
        void StringEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token GuidEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, winrt::guid> const& handler);
        void GuidEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token EnumEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, TestEnum> const& handler);
        void EnumEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token CompositeStructEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, CompositeType> const& handler);
        void CompositeStructEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token RefEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, Windows::Foundation::IReference<int32_t>> const&
                handler);
        void RefEventHandler(winrt::event_token const& token) noexcept;
        winrt::event_token ObjectEventHandler(
            Windows::Foundation::TypedEventHandler<TestComponent::Test, TestComponent::TestObject> const& handler);
        void ObjectEventHandler(winrt::event_token const& token) noexcept;

        void RaiseBoolEvent(bool value);
        void RaiseCharEvent(char16_t value);
        void RaiseNumericEvent(int32_t value);
        void RaiseStringEvent(hstring const& value);
        void RaiseGuidEvent(winrt::guid const& value);
        void RaiseEnumEvent(TestEnum const& value);
        void RaiseCompositeStructEvent(CompositeType const& value);
        void RaiseRefEvent(Windows::Foundation::IReference<int32_t> const& value);
        void RaiseObjectEvent(TestComponent::TestObject const& value);

        // IMap
        static Windows::Foundation::Collections::IMap<winrt::hstring, int32_t> CreateStringToNumberMap();
        static Windows::Foundation::Collections::IMapView<winrt::hstring, int32_t> CopyToMapView(
            Windows::Foundation::Collections::IMap<winrt::hstring, int32_t> const& stringToNumberMap);

    private:
        // Static properties
        inline static bool s_boolProperty = false;
        inline static char16_t m_staticCharProperty = 0;
        inline static uint8_t m_staticU8Property = 0;
        inline static uint16_t m_staticU16Property = 0;
        inline static uint32_t m_staticU32Property = 0;
        inline static uint64_t m_staticU64Property = 0;
        inline static int16_t m_staticS16Property = 0;
        inline static int32_t m_staticS32Property = 0;
        inline static int64_t m_staticS64Property = 0;
        inline static float m_staticF32Property = 0;
        inline static double m_staticF64Property = 0;
        inline static hstring m_staticStringProperty;
        inline static guid m_staticGuidProperty = {};
        inline static TestEnum m_staticEnumProperty = TestEnum::First;
        inline static NumericTypes m_staticNumericsStructProperty = {};
        inline static StringTypes m_staticStringsStructProperty = {};
        inline static BooleanTypes m_staticBooleansStructProperty = {};
        inline static CompositeType m_staticCompositeStructProperty = {};
        inline static Windows::Foundation::IReference<bool> m_staticRefBoolProperty;
        inline static Windows::Foundation::IReference<char16_t> m_staticRefCharProperty;
        inline static Windows::Foundation::IReference<int32_t> m_staticRefNumericProperty;
        inline static Windows::Foundation::IReference<TestEnum> m_staticRefEnumProperty;
        inline static TestComponent::TestObject m_staticObjectProperty{ nullptr };
        inline static std::vector<bool> m_staticBoolArrayProperty;
        inline static std::vector<char16_t> m_staticCharArrayProperty;
        inline static std::vector<int32_t> m_staticNumericArrayProperty;
        inline static std::vector<hstring> m_staticStringArrayProperty;
        inline static std::vector<guid> m_staticGuidArrayProperty;
        inline static std::vector<TestEnum> m_staticEnumArrayProperty;
        inline static std::vector<CompositeType> m_staticCompositeStructArrayProperty;
        inline static std::vector<Windows::Foundation::IReference<int32_t>> m_staticRefArrayProperty;
        inline static std::vector<TestComponent::TestObject> m_staticObjectArrayProperty;

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
        inline static event<Windows::Foundation::EventHandler<TestComponent::TestObject>> s_objectEventSource;

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
        TestComponent::TestObject m_objectProperty{ nullptr };
        Windows::Foundation::DateTime m_dateTimeProperty;
        Windows::Foundation::TimeSpan m_timeSpanProperty;
        hresult m_hresultProperty;
        Windows::Foundation::IPropertyValue m_propertyValue;
        std::vector<bool> m_boolArrayProperty;
        std::vector<char16_t> m_charArrayProperty;
        std::vector<int32_t> m_numericArrayProperty;
        std::vector<hstring> m_stringArrayProperty;
        std::vector<guid> m_guidArrayProperty;
        std::vector<TestEnum> m_enumArrayProperty;
        std::vector<CompositeType> m_compositeStructArrayProperty;
        std::vector<Windows::Foundation::IReference<int32_t>> m_refArrayProperty;
        std::vector<TestComponent::TestObject> m_objectArrayProperty;

        // Non-static event sources
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, bool>> m_boolEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, char16_t>> m_charEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, int32_t>> m_numericEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, hstring>> m_stringEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, winrt::guid>> m_guidEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, TestEnum>> m_enumEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, CompositeType>> m_compositeStructEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, Windows::Foundation::IReference<int32_t>>>
            m_refEventSource;
        event<Windows::Foundation::TypedEventHandler<TestComponent::Test, TestComponent::TestObject>>
            m_objectEventSource;
    };
}
namespace winrt::TestComponent::factory_implementation
{
    struct TestObject : TestObjectT<TestObject, implementation::TestObject>
    {
    };

    struct Test : TestT<Test, implementation::Test>
    {
    };
}
