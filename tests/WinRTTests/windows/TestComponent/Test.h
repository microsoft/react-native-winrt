#pragma once

#include "Test.g.h"

namespace winrt::TestComponent::implementation
{
    struct Test : TestT<Test>
    {
        Test() = default;

        static bool StaticBoolProperty();
        static void StaticBoolProperty(bool value);

        static bool StaticOr(bool lhs, bool rhs);
        static bool StaticOrAll(array_view<bool const> values);
        static int32_t StaticAdd(int32_t lhs, int32_t rhs);
        static int32_t StaticAddAll(array_view<int32_t const> values);
        static hstring StaticAppend(hstring const& a, char16_t b, hstring const& c);
        static hstring StaticAppendAll(array_view<hstring const> values);

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
        static void StaticObjectFillParam(array_view<Windows::Foundation::Collections::IVector<int32_t>> values);

        static Windows::Foundation::Collections::IVector<int32_t> MakeNumericVector(array_view<int32_t const> values);

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

        com_array<bool> BoolArrayOutParam(
            array_view<bool const> values, com_array<bool>& rot1, com_array<bool>& rot2);
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
        void ObjectFillParam(array_view<Windows::Foundation::Collections::IVector<int32_t>> values);

    private:
        // Static properties
        inline static bool s_boolProperty = false;

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
    };
}
namespace winrt::TestComponent::factory_implementation
{
    struct Test : TestT<Test, implementation::Test>
    {
    };
}
