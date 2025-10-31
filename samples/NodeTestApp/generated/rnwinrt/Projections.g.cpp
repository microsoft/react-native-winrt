#include "pch.h"

#include "base.h"

#include "TestComponent.g.h"
#include <winrt/TestComponent.h>
#include "Windows.g.h"
#include "Windows.Foundation.g.h"
#include <winrt/Windows.Foundation.h>
#include "Windows.Foundation.Collections.g.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace rnwinrt
{
    static constexpr const static_namespace_data* const root_namespace_data[] = {
        &rnwinrt::namespaces::TestComponent::data,
        &rnwinrt::namespaces::Windows::data,
    };

    constexpr const span<const static_namespace_data* const> root_namespaces{ root_namespace_data };

    static constexpr const std::pair<winrt::guid, const static_interface_data*> global_interface_map_data[] = {
        { winrt::guid_of<winrt::Windows::Foundation::IAsyncInfo>(), &rnwinrt::interfaces::Windows::Foundation::IAsyncInfo::data }, // 00000036-0000-0000-c000-000000000046
        { winrt::guid_of<winrt::Windows::Foundation::IClosable>(), &rnwinrt::interfaces::Windows::Foundation::IClosable::data }, // 30d5a829-7fa4-4026-83bb-d75bae4ea99e
        { winrt::guid_of<winrt::Windows::Foundation::IPropertyValue>(), &rnwinrt::interfaces::Windows::Foundation::IPropertyValue::data }, // 4bd682dd-7554-40e9-9a9b-82654ede7e62
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IPropertySet>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IPropertySet::data }, // 8a43ed9f-f4e6-4421-acf9-1dab2986820c
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorChangedEventArgs::data }, // 575933df-34fe-4480-af15-07691f3d5d9b
        { winrt::guid_of<winrt::Windows::Foundation::IDeferral>(), &rnwinrt::interfaces::Windows::Foundation::IDeferral::data }, // d6269732-3b7f-46a7-b40b-4fdca2a2c693
        { winrt::guid_of<winrt::Windows::Foundation::IStringable>(), &rnwinrt::interfaces::Windows::Foundation::IStringable::data }, // 96369f54-8eb6-48f0-abce-c1b211e627c3
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Foundation::IReference<int32_t>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::Windows::Foundation::IReference<int32_t>> }, // d6305b6f-5274-5001-ab1f-f1d8957b6fb9
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMapView::data<winrt::hstring, int32_t> }, // 06c17849-dfc8-501a-bf47-16152fa21d4b
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::Windows::Foundation::IReference<int32_t>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::Windows::Foundation::IReference<int32_t>> }, // 03c10a1b-b584-5081-a76d-017a80eeebfe
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::Windows::Foundation::IInspectable> }, // 44a94f2d-04f8-5091-b336-be7892dd10be
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::hstring> }, // 8c304ebb-6615-50a4-8829-879ecd443236
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<winrt::TestComponent::TestEnum>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<winrt::TestComponent::TestEnum> }, // f597c213-4ee0-50d7-a90f-379d0bf49dc8
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::TestComponent::ITestInterface>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::TestComponent::ITestInterface> }, // 9cfc4314-a599-510b-a8d2-6f259cd62cc3
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IObservableMap<winrt::hstring, int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IObservableMap::data<winrt::hstring, int32_t> }, // 1205d34e-4288-51b1-8279-9f9a22845bae
        { winrt::guid_of<winrt::TestComponent::IHierarchyDerived>(), &rnwinrt::interfaces::TestComponent::IHierarchyDerived::data }, // 52f17109-4dfb-520c-a631-1fb270141cf6
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMap<winrt::hstring, int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMap::data<winrt::hstring, int32_t> }, // ae681871-dd82-5299-93ea-0275e4e073e7
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::guid>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::guid> }, // 9520e64b-15b2-52a6-98ed-3191fa6cf68a
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMap<winrt::hstring, winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMap::data<winrt::hstring, winrt::hstring> }, // f6d1f700-49c2-52ae-8154-826f9908773c
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::Windows::Foundation::IInspectable> }, // 092b849b-60b1-52be-a44a-6fe8e933cbe4
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::TestComponent::ITestInterface>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::TestComponent::ITestInterface> }, // 809a76cf-4911-52d0-ac8d-f32ca72ac023
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IKeyValuePair::data<winrt::hstring, winrt::hstring> }, // 60310303-49c5-52e6-abc6-a9b36eccc716
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<char16_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<char16_t> }, // 33a0f9f3-efe5-52ec-b689-942c0c699bf1
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IObservableVector<int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IObservableVector::data<int32_t> }, // 95c02345-5b4a-5310-a6bf-f17e0d682da7
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<winrt::hstring> }, // 98b9acc1-4b56-532e-ac73-03d5291cca90
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMapChangedEventArgs::data<winrt::hstring> }, // 60141efb-f2f9-5377-96fd-f8c60d9558b5
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::guid>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::guid> }, // d3d64048-82b3-53c7-9285-b0be18368482
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::TestComponent::TestEnum>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::TestComponent::TestEnum> }, // 12127da4-28ec-5407-95e0-59e441d3e59b
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IReference<int32_t>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<winrt::Windows::Foundation::IReference<int32_t>> }, // 847bd694-79c7-5471-8391-050a10ea625f
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::IReference<int32_t>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::Windows::Foundation::IReference<int32_t>> }, // 81ae1001-d393-5473-aea4-27e9d8739f21
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::TestComponent::TestObject>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::TestComponent::TestObject> }, // 17e57a2b-72a3-547c-ba51-fb7bbbc16dbb
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<int32_t> }, // b939af5b-b45d-5489-9149-61442c1905fe
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>> }, // fe2f3d47-5d47-5499-8374-430c7cda0204
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::guid>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::guid> }, // f4ca3045-5dd7-54be-982e-d88d8ca0876e
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<bool>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<bool> }, // 30160817-1d7d-54e9-99db-d7636266a476
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMapView::data<winrt::hstring, winrt::Windows::Foundation::IInspectable> }, // bb78502a-f79d-54fa-92c9-90c5039fdf7e
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::hstring>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::hstring>> }, // 05eb86f1-7140-5517-b88d-cbaebe57e6b1
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<int32_t> }, // 81a643fb-f51c-5565-83c4-f96425777b66
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::TestComponent::TestObject>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::TestComponent::TestObject> }, // 31951602-84a5-55f4-b54e-00b3a19c145b
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IObservableMap<winrt::hstring, winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IObservableMap::data<winrt::hstring, winrt::hstring> }, // 1e036276-2f60-55f6-b7f3-f86079e6900b
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<bool>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<bool> }, // 243a09cb-6f40-56af-a442-fe81431fbef5
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<winrt::TestComponent::TestObject>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<winrt::TestComponent::TestObject> }, // 6e033812-d00f-5706-8fd3-3dc676a3b5ee
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::TestComponent::CompositeType>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::TestComponent::CompositeType> }, // 0cf1b567-cac8-5726-af95-e3bbae2f8de6
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<bool>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<bool> }, // 740a0296-a535-572a-bf0b-17c18ff71fe6
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<char16_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<char16_t> }, // c61b9bdd-2933-5773-a463-e81354dc0e26
        { winrt::guid_of<winrt::TestComponent::ITest2>(), &rnwinrt::interfaces::TestComponent::ITest2::data }, // 1e5ab212-8287-578f-a769-35c446e7ac56
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>> }, // 5db5fa32-707c-5849-a06b-91c8eb9d10e8
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::TestComponent::TestObject>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::TestComponent::TestObject> }, // 4f557b6c-ed09-584c-9051-26c56378334d
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IKeyValuePair::data<winrt::hstring, int32_t> }, // 40e7e72d-cbab-588b-a227-9e60532f0121
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<char16_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<char16_t> }, // 3d54d66f-c4a8-58e8-9a68-53729a6b9095
        { winrt::guid_of<winrt::TestComponent::IHierarchyBase>(), &rnwinrt::interfaces::TestComponent::IHierarchyBase::data }, // def5253c-4226-58ee-9596-b6b2ab764564
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, int32_t>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, int32_t>> }, // 2aa69c56-c3a4-58f9-b14c-465bcaf8c7ba
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<winrt::TestComponent::CompositeType>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<winrt::TestComponent::CompositeType> }, // f0d5f13c-ef90-5909-a01e-99696a982f30
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::TestComponent::TestEnum>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::TestComponent::TestEnum> }, // 91b1ba8d-0186-59dc-8f93-891237266716
        { winrt::guid_of<winrt::TestComponent::ITestObject>(), &rnwinrt::interfaces::TestComponent::ITestObject::data }, // 8a5d0890-acba-59e5-8a5c-b4e0f8a7bc09
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::hstring> }, // e2fcc7c1-3bfc-5a0b-b2b0-72e769d1cb7e
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::TestComponent::TestEnum>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::TestComponent::TestEnum> }, // 0d437f73-2677-5a1f-b975-1bd7de511698
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IKeyValuePair::data<winrt::hstring, winrt::Windows::Foundation::IInspectable> }, // 09335560-6c6b-5a26-9348-97b781132b20
        { winrt::guid_of<winrt::TestComponent::IHierarchyV2Contract>(), &rnwinrt::interfaces::TestComponent::IHierarchyV2Contract::data }, // e7611eb1-1148-5ab8-af93-df1934bf773d
        { winrt::guid_of<winrt::TestComponent::IHierarchyDerivedFactory>(), &rnwinrt::interfaces::TestComponent::IHierarchyDerivedFactory::data }, // c4e6f35c-429a-5ade-8b53-2b0a0062da10
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMapView<winrt::hstring, winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMapView::data<winrt::hstring, winrt::hstring> }, // ac7f26f2-feb7-5b2a-8ac4-345bc62caede
        { winrt::guid_of<winrt::TestComponent::IHierarchyBaseFactory>(), &rnwinrt::interfaces::TestComponent::IHierarchyBaseFactory::data }, // cdbf5391-659a-5b4f-ac03-10c37ffcabf3
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IObservableMap<winrt::hstring, winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IObservableMap::data<winrt::hstring, winrt::Windows::Foundation::IInspectable> }, // 236aac9d-fb12-5c4d-a41c-9e445fb4d7ec
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::Windows::Foundation::IInspectable> }, // a6487363-b074-5c60-ab16-866dce4ee54d
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::hstring>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, winrt::hstring>> }, // e9bdaaf0-cbf6-5c72-be90-29cbf3a1319b
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::TestComponent::ITestInterface>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::TestComponent::ITestInterface> }, // 704aa2de-9688-5cd7-9928-598a5ffaaaa8
        { winrt::guid_of<winrt::TestComponent::ITest>(), &rnwinrt::interfaces::TestComponent::ITest::data }, // 9f5b9cf9-6bf4-5ce5-b57c-0a2b78d427c3
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<int32_t> }, // 8d720cdf-3934-5d3f-9a55-40e8063b086a
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<char16_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<char16_t> }, // 848e45c7-2fbb-5d59-a35f-0b4e88349103
        { winrt::guid_of<winrt::TestComponent::ITestInterface>(), &rnwinrt::interfaces::TestComponent::ITestInterface::data }, // aa3f808a-6e2c-5dd8-80c9-2519c970d57f
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<bool>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<bool> }, // 6180171d-2ed8-5e24-8a55-01ecb1009eb2
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVector<winrt::guid>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVector::data<winrt::guid> }, // 482e676d-b913-5ec1-afa8-5f96922e94ae
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IMap<winrt::hstring, winrt::Windows::Foundation::IInspectable>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IMap::data<winrt::hstring, winrt::Windows::Foundation::IInspectable> }, // 1b0d3570-0877-5ec2-8a2c-3b9539506aca
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<int32_t>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<int32_t> }, // bfea7f78-50c2-5f1d-a6ea-9e978d2699ff
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, int32_t>>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterator::data<winrt::Windows::Foundation::Collections::IKeyValuePair<winrt::hstring, int32_t>> }, // 96c8b304-4108-5f67-8b2f-213975f085b2
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::hstring> }, // 2f13c006-a03a-5f69-b090-75a43e33423e
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorView<winrt::TestComponent::CompositeType>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IVectorView::data<winrt::TestComponent::CompositeType> }, // 9b6f79b0-8d71-5f82-8a57-1c2af88ff519
        { winrt::guid_of<winrt::Windows::Foundation::Collections::IIterable<winrt::TestComponent::CompositeType>>(), &rnwinrt::interfaces::Windows::Foundation::Collections::IIterable::data<winrt::TestComponent::CompositeType> }, // 94575193-3186-5fc8-adbd-61ef9993f47d
    };

    constexpr const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map(global_interface_map_data);
}
