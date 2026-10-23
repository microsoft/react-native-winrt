#include "pch.h"

#include "base.h"

#include "Windows.Media.PlayTo.g.h"

#include <winrt/Windows.Media.PlayTo.h>

namespace rnwinrt::namespaces::Windows::Media::PlayTo
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::classes::Windows::Media::PlayTo::CurrentTimeChangeRequestedEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::MuteChangeRequestedEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToConnection::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToConnectionErrorEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToConnectionStateChangedEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToConnectionTransferredEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToManager::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToReceiver::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToSource::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceDeferral::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceRequest::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceRequestedEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceSelectedEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::PlaybackRateChangeRequestedEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::SourceChangeRequestedEventArgs::data,
        &rnwinrt::classes::Windows::Media::PlayTo::VolumeChangeRequestedEventArgs::data,
        &rnwinrt::enums::Windows::Media::PlayTo::PlayToConnectionError::data,
        &rnwinrt::enums::Windows::Media::PlayTo::PlayToConnectionState::data,
    };

    constexpr const static_namespace_data data{ "PlayTo"sv, children };
}

namespace rnwinrt::enums::Windows::Media::PlayTo::PlayToConnectionError
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "none"sv, 0, "0"sv },
        { "deviceNotResponding"sv, 1, "1"sv },
        { "deviceError"sv, 2, "2"sv },
        { "deviceLocked"sv, 3, "3"sv },
        { "protectedPlaybackFailed"sv, 4, "4"sv },
    };

    constexpr const static_enum_data data{ "PlayToConnectionError"sv, mappings };
}

namespace rnwinrt::enums::Windows::Media::PlayTo::PlayToConnectionState
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "disconnected"sv, 0, "0"sv },
        { "connected"sv, 1, "1"sv },
        { "rendering"sv, 2, "2"sv },
    };

    constexpr const static_enum_data data{ "PlayToConnectionState"sv, mappings };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::CurrentTimeChangeRequestedEventArgs
{
    constexpr const static_class_data data{ "CurrentTimeChangeRequestedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::MuteChangeRequestedEventArgs
{
    constexpr const static_class_data data{ "MuteChangeRequestedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToConnection
{
    constexpr const static_class_data data{ "PlayToConnection"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToConnectionErrorEventArgs
{
    constexpr const static_class_data data{ "PlayToConnectionErrorEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToConnectionStateChangedEventArgs
{
    constexpr const static_class_data data{ "PlayToConnectionStateChangedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToConnectionTransferredEventArgs
{
    constexpr const static_class_data data{ "PlayToConnectionTransferredEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToManager
{
    static constexpr const static_class_data::function_mapping function_data[] = {
        { "getForCurrentView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::Windows::Media::PlayTo::PlayToManager::GetForCurrentView();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Media.PlayTo"sv, "PlayToManager"sv, "getForCurrentView"sv, count);
            }
        },
        { "showPlayToUI",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    winrt::Windows::Media::PlayTo::PlayToManager::ShowPlayToUI();
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "Windows.Media.PlayTo"sv, "PlayToManager"sv, "showPlayToUI"sv, count);
            }
        },
    };

    constexpr const static_class_data data{ "PlayToManager"sv, {}, {}, function_data };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToReceiver
{
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 0)
        {
            return convert_native_to_value(runtime, winrt::Windows::Media::PlayTo::PlayToReceiver());
        }
        throw_no_constructor(runtime, "Windows.Media.PlayTo"sv, "PlayToReceiver"sv, count);
    }

    constexpr const static_activatable_class_data data{ "Windows.Media.PlayTo"sv, "PlayToReceiver"sv, constructor_function, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToSource
{
    constexpr const static_class_data data{ "PlayToSource"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceDeferral
{
    constexpr const static_class_data data{ "PlayToSourceDeferral"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceRequest
{
    constexpr const static_class_data data{ "PlayToSourceRequest"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceRequestedEventArgs
{
    constexpr const static_class_data data{ "PlayToSourceRequestedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlayToSourceSelectedEventArgs
{
    constexpr const static_class_data data{ "PlayToSourceSelectedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::PlaybackRateChangeRequestedEventArgs
{
    constexpr const static_class_data data{ "PlaybackRateChangeRequestedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::SourceChangeRequestedEventArgs
{
    constexpr const static_class_data data{ "SourceChangeRequestedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Media::PlayTo::VolumeChangeRequestedEventArgs
{
    constexpr const static_class_data data{ "VolumeChangeRequestedEventArgs"sv, {}, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::ICurrentTimeChangeRequestedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "time",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ICurrentTimeChangeRequestedEventArgs>().Time());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::ICurrentTimeChangeRequestedEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IMuteChangeRequestedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "mute",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IMuteChangeRequestedEventArgs>().Mute());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IMuteChangeRequestedEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnection
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "state",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnection>().State());
            },
            nullptr
        },
    };
    static constexpr const static_interface_data::event_mapping event_data[] = {
        { "error",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnection>().Error(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToConnection, winrt::Windows::Media::PlayTo::PlayToConnectionErrorEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnection>().Error(token);
            }
        },
        { "statechanged",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnection>().StateChanged(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToConnection, winrt::Windows::Media::PlayTo::PlayToConnectionStateChangedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnection>().StateChanged(token);
            }
        },
        { "transferred",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnection>().Transferred(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToConnection, winrt::Windows::Media::PlayTo::PlayToConnectionTransferredEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnection>().Transferred(token);
            }
        },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnection>(), property_data, event_data, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnectionErrorEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "code",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnectionErrorEventArgs>().Code());
            },
            nullptr
        },
        { "message",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnectionErrorEventArgs>().Message());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnectionErrorEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnectionStateChangedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "currentState",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnectionStateChangedEventArgs>().CurrentState());
            },
            nullptr
        },
        { "previousState",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnectionStateChangedEventArgs>().PreviousState());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnectionStateChangedEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnectionTransferredEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "currentSource",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnectionTransferredEventArgs>().CurrentSource());
            },
            nullptr
        },
        { "previousSource",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToConnectionTransferredEventArgs>().PreviousSource());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnectionTransferredEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToManager
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "defaultSourceSelection",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToManager>().DefaultSourceSelection());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToManager>().DefaultSourceSelection(convert_value_to_native<bool>(runtime, value));
            },
        },
    };
    static constexpr const static_interface_data::event_mapping event_data[] = {
        { "sourcerequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToManager>().SourceRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToManager, winrt::Windows::Media::PlayTo::PlayToSourceRequestedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToManager>().SourceRequested(token);
            }
        },
        { "sourceselected",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToManager>().SourceSelected(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToManager, winrt::Windows::Media::PlayTo::PlayToSourceSelectedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToManager>().SourceSelected(token);
            }
        },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToManager>(), property_data, event_data, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToReceiver
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "friendlyName",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().FriendlyName());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().FriendlyName(convert_value_to_native<winrt::hstring>(runtime, value));
            },
        },
        { "properties",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().Properties());
            },
            nullptr
        },
        { "supportsAudio",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SupportsAudio());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SupportsAudio(convert_value_to_native<bool>(runtime, value));
            },
        },
        { "supportsImage",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SupportsImage());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SupportsImage(convert_value_to_native<bool>(runtime, value));
            },
        },
        { "supportsVideo",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SupportsVideo());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SupportsVideo(convert_value_to_native<bool>(runtime, value));
            },
        },
    };
    static constexpr const static_interface_data::event_mapping event_data[] = {
        { "currenttimechangerequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().CurrentTimeChangeRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Media::PlayTo::CurrentTimeChangeRequestedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().CurrentTimeChangeRequested(token);
            }
        },
        { "mutechangerequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().MuteChangeRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Media::PlayTo::MuteChangeRequestedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().MuteChangeRequested(token);
            }
        },
        { "pauserequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().PauseRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Foundation::IInspectable>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().PauseRequested(token);
            }
        },
        { "playbackratechangerequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().PlaybackRateChangeRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Media::PlayTo::PlaybackRateChangeRequestedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().PlaybackRateChangeRequested(token);
            }
        },
        { "playrequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().PlayRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Foundation::IInspectable>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().PlayRequested(token);
            }
        },
        { "sourcechangerequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SourceChangeRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Media::PlayTo::SourceChangeRequestedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().SourceChangeRequested(token);
            }
        },
        { "stoprequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().StopRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Foundation::IInspectable>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().StopRequested(token);
            }
        },
        { "timeupdaterequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().TimeUpdateRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Foundation::IInspectable>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().TimeUpdateRequested(token);
            }
        },
        { "volumechangerequested",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().VolumeChangeRequested(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::PlayTo::PlayToReceiver, winrt::Windows::Media::PlayTo::VolumeChangeRequestedEventArgs>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().VolumeChangeRequested(token);
            }
        },
    };

    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "notifyDurationChange",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::TimeSpan>(runtime, args[0]);
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyDurationChange(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "notifyEnded",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyEnded();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifyError",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyError();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifyLoadedMetadata",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyLoadedMetadata();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifyPaused",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyPaused();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifyPlaying",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyPlaying();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifyRateChange",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<double>(runtime, args[0]);
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyRateChange(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "notifySeeked",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifySeeked();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifySeeking",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifySeeking();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifyStopped",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyStopped();
                    return jsi::Value::undefined();
                },
                0, false },
        { "notifyTimeUpdate",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::TimeSpan>(runtime, args[0]);
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyTimeUpdate(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "notifyVolumeChange",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<double>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<bool>(runtime, args[1]);
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().NotifyVolumeChange(arg0, arg1);
                    return jsi::Value::undefined();
                },
                2, false },
        { "startAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().StartAsync();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "stopAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Media::PlayTo::IPlayToReceiver>().StopAsync();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToReceiver>(), property_data, event_data, function_data };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSource
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "connection",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSource>().Connection());
            },
            nullptr
        },
        { "next",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSource>().Next());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSource>().Next(convert_value_to_native<winrt::Windows::Media::PlayTo::PlayToSource>(runtime, value));
            },
        },
    };
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "playNext",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSource>().PlayNext();
                    return jsi::Value::undefined();
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSource>(), property_data, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceDeferral
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "complete",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceDeferral>().Complete();
                    return jsi::Value::undefined();
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceDeferral>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceRequest
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "deadline",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceRequest>().Deadline());
            },
            nullptr
        },
    };
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "displayErrorString",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceRequest>().DisplayErrorString(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "getDeferral",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceRequest>().GetDeferral();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "setSource",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Media::PlayTo::PlayToSource>(runtime, args[0]);
                    thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceRequest>().SetSource(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceRequest>(), property_data, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceRequestedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "sourceRequest",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceRequestedEventArgs>().SourceRequest());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceRequestedEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "friendlyName",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs>().FriendlyName());
            },
            nullptr
        },
        { "icon",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs>().Icon());
            },
            nullptr
        },
        { "supportsAudio",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs>().SupportsAudio());
            },
            nullptr
        },
        { "supportsImage",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs>().SupportsImage());
            },
            nullptr
        },
        { "supportsVideo",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs>().SupportsVideo());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceWithPreferredSourceUri
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "preferredSourceUri",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceWithPreferredSourceUri>().PreferredSourceUri());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::Windows::Media::PlayTo::IPlayToSourceWithPreferredSourceUri>().PreferredSourceUri(convert_value_to_native<winrt::Windows::Foundation::Uri>(runtime, value));
            },
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceWithPreferredSourceUri>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IPlaybackRateChangeRequestedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "rate",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IPlaybackRateChangeRequestedEventArgs>().Rate());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IPlaybackRateChangeRequestedEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "album",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Album());
            },
            nullptr
        },
        { "author",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Author());
            },
            nullptr
        },
        { "date",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Date());
            },
            nullptr
        },
        { "description",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Description());
            },
            nullptr
        },
        { "genre",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Genre());
            },
            nullptr
        },
        { "properties",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Properties());
            },
            nullptr
        },
        { "rating",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Rating());
            },
            nullptr
        },
        { "stream",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Stream());
            },
            nullptr
        },
        { "thumbnail",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Thumbnail());
            },
            nullptr
        },
        { "title",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>().Title());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Media::PlayTo::IVolumeChangeRequestedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "volume",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Media::PlayTo::IVolumeChangeRequestedEventArgs>().Volume());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Media::PlayTo::IVolumeChangeRequestedEventArgs>(), property_data, {}, {} };
}
