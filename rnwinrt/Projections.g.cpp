#include "pch.h"

#include "base.h"

#include "Windows.g.h"
#include "Windows.Media.PlayTo.g.h"
#include <winrt/Windows.Media.PlayTo.h>

namespace rnwinrt
{
    static constexpr const static_namespace_data* const root_namespace_data[] = {
        &rnwinrt::namespaces::Windows::data,
    };

    constexpr const span<const static_namespace_data* const> root_namespaces{ root_namespace_data };

    static constexpr const std::pair<winrt::guid, const static_interface_data*> global_interface_map_data[] = {
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToManager>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToManager::data }, // f56a206e-1b77-42ef-8f0d-b949f8d9b260
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnectionErrorEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnectionErrorEventArgs::data }, // bf5eada6-88e6-445f-9d40-d9b9f8939896
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceRequest>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceRequest::data }, // f8584665-64f4-44a0-ac0d-468d2b8fda83
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnectionTransferredEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnectionTransferredEventArgs::data }, // fae3193a-0683-47d9-8df0-18cbb48984d8
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnectionStateChangedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnectionStateChangedEventArgs::data }, // 68c4b50f-0c20-4980-8602-58c62238d423
        { winrt::guid_of<winrt::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::ISourceChangeRequestedEventArgs::data }, // fb3f3a96-7aa6-4a8b-86e7-54f6c6d34f64
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToReceiver>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToReceiver::data }, // ac15cf47-a162-4aa6-af1b-3aa35f3b9069
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSource>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSource::data }, // 7f138a08-fbb7-4b09-8356-aa5f4e335c31
        { winrt::guid_of<winrt::Windows::Media::PlayTo::ICurrentTimeChangeRequestedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::ICurrentTimeChangeRequestedEventArgs::data }, // 99711324-edc7-4bf5-91f6-3c8627db59e5
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IVolumeChangeRequestedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IVolumeChangeRequestedEventArgs::data }, // 6f026d5c-cf75-4c2b-913e-6d7c6c329179
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlaybackRateChangeRequestedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlaybackRateChangeRequestedEventArgs::data }, // 0f5661ae-2c88-4cca-8540-d586095d13a5
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceWithPreferredSourceUri>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceWithPreferredSourceUri::data }, // aab253eb-3301-4dc4-afba-b2f2ed9635a0
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceSelectedEventArgs::data }, // 0c9d8511-5202-4dcb-8c67-abda12bb3c12
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceRequestedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceRequestedEventArgs::data }, // c5cdc330-29df-4ec6-9da9-9fbdfcfc1b3e
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IMuteChangeRequestedEventArgs>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IMuteChangeRequestedEventArgs::data }, // e4b4f5f6-af1f-4f1e-b437-7da32400e1d4
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToSourceDeferral>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToSourceDeferral::data }, // 4100891d-278e-4f29-859b-a9e501053e7d
        { winrt::guid_of<winrt::Windows::Media::PlayTo::IPlayToConnection>(), &rnwinrt::interfaces::Windows::Media::PlayTo::IPlayToConnection::data }, // 112fbfc8-f235-4fde-8d41-9bf27c9e9a40
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>> }, // fe2f3d47-5d47-5499-8374-430c7cda0204
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMapView::data<winrt::hstring, winrt::Windows::Foundation::IInspectable> }, // bb78502a-f79d-54fa-92c9-90c5039fdf7e
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>> }, // 5db5fa32-707c-5849-a06b-91c8eb9d10e8
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IKeyValuePair::data<winrt::hstring, winrt::Windows::Foundation::IInspectable> }, // 09335560-6c6b-5a26-9348-97b781132b20
    };

    constexpr const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map(global_interface_map_data);
}
