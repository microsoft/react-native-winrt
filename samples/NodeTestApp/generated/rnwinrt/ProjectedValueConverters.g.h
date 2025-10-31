#pragma once

#include "base.h"

namespace winrt::TestComponent
{
    struct BooleanTypes;
    struct CompositeType;
    struct NumericTypes;
    struct StringTypes;
    struct BoolArrayDelegate;
    struct BoolDelegate;
    struct BoolDelegateWithOutParam;
    struct CharArrayDelegate;
    struct CharDelegate;
    struct CharDelegateWithOutParam;
    struct CompositeStructArrayDelegate;
    struct CompositeStructDelegate;
    struct CompositeStructDelegateWithOutParam;
    struct EnumArrayDelegate;
    struct EnumDelegate;
    struct EnumDelegateWithOutParam;
    struct GuidArrayDelegate;
    struct GuidDelegate;
    struct GuidDelegateWithOutParam;
    struct InterwovenDelegate;
    struct NumericArrayDelegate;
    struct NumericDelegate;
    struct NumericDelegateWithOutParam;
    struct ObjectArrayDelegate;
    struct ObjectDelegate;
    struct ObjectDelegateWithOutParam;
    struct RefArrayDelegate;
    struct RefDelegate;
    struct RefDelegateWithOutParam;
    struct StringArrayDelegate;
    struct StringDelegate;
    struct StringDelegateWithOutParam;
}

namespace rnwinrt
{
    template <>
    struct projected_value_traits<winrt::TestComponent::BooleanTypes>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::BooleanTypes& value);
        static winrt::TestComponent::BooleanTypes as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::CompositeType>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeType& value);
        static winrt::TestComponent::CompositeType as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::NumericTypes>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericTypes& value);
        static winrt::TestComponent::NumericTypes as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::StringTypes>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringTypes& value);
        static winrt::TestComponent::StringTypes as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::BoolArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::BoolArrayDelegate& value);
        static winrt::TestComponent::BoolArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::BoolDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::BoolDelegate& value);
        static winrt::TestComponent::BoolDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::BoolDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::BoolDelegateWithOutParam& value);
        static winrt::TestComponent::BoolDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::CharArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::CharArrayDelegate& value);
        static winrt::TestComponent::CharArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::CharDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::CharDelegate& value);
        static winrt::TestComponent::CharDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::CharDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::CharDelegateWithOutParam& value);
        static winrt::TestComponent::CharDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::CompositeStructArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeStructArrayDelegate& value);
        static winrt::TestComponent::CompositeStructArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::CompositeStructDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeStructDelegate& value);
        static winrt::TestComponent::CompositeStructDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::CompositeStructDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeStructDelegateWithOutParam& value);
        static winrt::TestComponent::CompositeStructDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::EnumArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::EnumArrayDelegate& value);
        static winrt::TestComponent::EnumArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::EnumDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::EnumDelegate& value);
        static winrt::TestComponent::EnumDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::EnumDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::EnumDelegateWithOutParam& value);
        static winrt::TestComponent::EnumDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::GuidArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::GuidArrayDelegate& value);
        static winrt::TestComponent::GuidArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::GuidDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::GuidDelegate& value);
        static winrt::TestComponent::GuidDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::GuidDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::GuidDelegateWithOutParam& value);
        static winrt::TestComponent::GuidDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::InterwovenDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::InterwovenDelegate& value);
        static winrt::TestComponent::InterwovenDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::NumericArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericArrayDelegate& value);
        static winrt::TestComponent::NumericArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::NumericDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericDelegate& value);
        static winrt::TestComponent::NumericDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::NumericDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericDelegateWithOutParam& value);
        static winrt::TestComponent::NumericDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::ObjectArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::ObjectArrayDelegate& value);
        static winrt::TestComponent::ObjectArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::ObjectDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::ObjectDelegate& value);
        static winrt::TestComponent::ObjectDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::ObjectDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::ObjectDelegateWithOutParam& value);
        static winrt::TestComponent::ObjectDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::RefArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::RefArrayDelegate& value);
        static winrt::TestComponent::RefArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::RefDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::RefDelegate& value);
        static winrt::TestComponent::RefDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::RefDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::RefDelegateWithOutParam& value);
        static winrt::TestComponent::RefDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::StringArrayDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringArrayDelegate& value);
        static winrt::TestComponent::StringArrayDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::StringDelegate>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringDelegate& value);
        static winrt::TestComponent::StringDelegate as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::TestComponent::StringDelegateWithOutParam>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringDelegateWithOutParam& value);
        static winrt::TestComponent::StringDelegateWithOutParam as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };
}

namespace winrt::Windows::Foundation
{
    struct Point;
    struct Rect;
    struct Size;
    struct AsyncActionCompletedHandler;
    struct DeferralCompletedHandler;
}

namespace rnwinrt
{
    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Point>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::Point& value);
        static winrt::Windows::Foundation::Point as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Rect>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::Rect& value);
        static winrt::Windows::Foundation::Rect as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Size>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::Size& value);
        static winrt::Windows::Foundation::Size as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::AsyncActionCompletedHandler>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::AsyncActionCompletedHandler& value);
        static winrt::Windows::Foundation::AsyncActionCompletedHandler as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::DeferralCompletedHandler>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::DeferralCompletedHandler& value);
        static winrt::Windows::Foundation::DeferralCompletedHandler as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };
}

