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
        static int32_t StaticAdd(int32_t lhs, int32_t rhs);
        static hstring StaticAppend(hstring const& a, char16_t b, hstring const& c);

        static void StaticBoolOutParam(bool lhs, bool rhs, bool& andResult);
        static void StaticBoolOutParam2(bool lhs, bool rhs, bool& andResult, bool& or);
        static bool StaticBoolOutParamWithReturn(bool lhs, bool rhs, bool& andResult, bool& or);
        static void StaticCharOutParam(char16_t value, char16_t& next);
        static void StaticCharOutParam2(char16_t value, char16_t& next, char16_t& prev);
        static char16_t StaticCharOutParamWithReturn(char16_t value, char16_t& next, char16_t& prev);
        static void StaticNumericOutParam(int32_t value, int32_t& doubledValue);
        static void StaticNumericOutParam2(int32_t value, int32_t& doubledValue, int32_t& tripledValue);
        static int32_t StaticNumericOutParamWithReturn(int32_t value, int32_t& doubledValue, int32_t& tripledValue);
        static void StaticStringOutParam(hstring const& value, hstring& lower);
        static void StaticStringOutParam2(hstring const& value, hstring& lower, hstring& upper);
        static hstring StaticStringOutParamWithReturn(hstring const& value, hstring& lower, hstring& upper);

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
        int32_t Add(int32_t lhs, int32_t rhs);
        hstring Append(hstring const& a, char16_t b, hstring const& c);

        void BoolOutParam(bool lhs, bool rhs, bool& andResult);
        void BoolOutParam2(bool lhs, bool rhs, bool& andResult, bool& or);
        bool BoolOutParamWithReturn(bool lhs, bool rhs, bool& andResult, bool& or);
        void CharOutParam(char16_t value, char16_t& next);
        void CharOutParam2(char16_t value, char16_t& next, char16_t& prev);
        char16_t CharOutParamWithReturn(char16_t value, char16_t& next, char16_t& prev);
        void NumericOutParam(int32_t value, int32_t& doubledValue);
        void NumericOutParam2(int32_t value, int32_t& doubledValue, int32_t& tripledValue);
        int32_t NumericOutParamWithReturn(int32_t value, int32_t& doubledValue, int32_t& tripledValue);
        void StringOutParam(hstring const& value, hstring& lower);
        void StringOutParam2(hstring const& value, hstring& lower, hstring& upper);
        hstring StringOutParamWithReturn(hstring const& value, hstring& lower, hstring& upper);

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
