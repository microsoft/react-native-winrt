#pragma once
#include "ProjectedInterface.h"
#include "ProjectedValueConverters.h"
#include <rnwinrt/ProjectedValueConverters.g.h>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>

namespace WinRTTurboModule::Windows::Foundation::Collections
{
    template <typename T>
    ProjectedInterfaceData InitIIterableInterface()
    {
        return { {},
            {
                ProjectedFunction::Create("first"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IIterable<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IIterable<T>>::First,
                        &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IIterator<T>>)),
            },
            {} };
    }

    template <typename T, auto CTV>
    ProjectedInterfaceData InitIIteratorInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IIterator<T>>(
                         "current"sv, &winrt::Windows::Foundation::Collections::IIterator<T>::Current, &CTV),
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IIterator<T>>("hasCurrent"sv,
                         &winrt::Windows::Foundation::Collections::IIterator<T>::HasCurrent, &ConvertBooleanToValue),
                 },
            {
                ProjectedFunction::Create("getMany"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IIterator<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IIterator<T>>::GetMany,
                        &ConvertNumberToValue<uint32_t>, &ConvertValueToWriteOnlyArrayView<T, CTV>)),
                ProjectedFunction::Create("moveNext"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IIterator<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IIterator<T>>::MoveNext,
                        &ConvertBooleanToValue)),
            },
            {} };
    }

    template <typename K, typename T, auto CKV, auto CTV>
    ProjectedInterfaceData InitIKeyValuePairInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>>(
                         "key"sv, &winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>::Key, &CKV),
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>>(
                         "value"sv, &winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>::Value, &CTV),
                 },
            {}, {} };
    }

    template <typename K, auto CKV>
    ProjectedInterfaceData InitIMapChangedEventArgsInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>(
                         "collectionChange"sv,
                         &winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>::CollectionChange,
                         &ConvertEnumToValue<winrt::Windows::Foundation::Collections::CollectionChange>),
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>(
                         "key"sv, &winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>::Key, &CKV),
                 },
            {}, {} };
    }

    template <typename K, typename T, auto CVK, auto CTV>
    ProjectedInterfaceData InitIMapViewInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IMapView<K, T>::Size,
                         &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("hasKey"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMapView<K, T>>::HasKey,
                        &ConvertBooleanToValue, &CVK)),
                ProjectedFunction::Create("lookup"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMapView<K, T>>::Lookup, &CTV,
                        &CVK)),
                ProjectedFunction::Create("split"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMapView<K, T>>::Split,
                        ProjectedFunctionParamInfo(ProjectedFunctionOutParam,
                            &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapView<K, T>>, "first"),
                        ProjectedFunctionParamInfo(ProjectedFunctionOutParam,
                            &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapView<K, T>>,
                            "second"))),
            },
            {} };
    }

    template <typename K, typename T, auto CVK, auto CTV, auto CVT>
    ProjectedInterfaceData InitIMapInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IMap<K, T>::Size, &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("clear"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Clear)),
                ProjectedFunction::Create("getView"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::GetView,
                        &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapView<K, T>>)),
                ProjectedFunction::Create("hasKey"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::HasKey,
                        &ConvertBooleanToValue, &CVK)),
                ProjectedFunction::Create("insert"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Insert,
                        &ConvertBooleanToValue, &CVK, &CVT)),
                ProjectedFunction::Create("lookup"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Lookup, &CTV,
                        &CVK)),
                ProjectedFunction::Create("remove"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Remove, &CVK)),
            },
            {} };
    }

    template <typename K, typename T>
    ProjectedInterfaceData InitIObservableMapInterface()
    {
        return { {}, {},
            {
                ProjectedEvent::Create<winrt::Windows::Foundation::Collections::IObservableMap<K, T>>("mapChanged"sv,
                    &winrt::Windows::Foundation::Collections::IObservableMap<K, T>::MapChanged,
                    &winrt::Windows::Foundation::Collections::IObservableMap<K, T>::MapChanged,
                    &ConvertValueToMapChangedEventHandler<K, T>),
            } };
    }

    template <typename T, auto CTV, auto CVT>
    ProjectedInterfaceData InitIVectorViewInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IVectorView<T>::Size,
                         &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("getAt"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVectorView<T>>::GetAt, &CTV,
                        &ConvertValueToNumber<uint32_t>)),
                ProjectedFunction::Create("getMany"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVectorView<T>>::GetMany,
                        &ConvertNumberToValue<uint32_t>, &ConvertValueToNumber<uint32_t>,
                        &ConvertValueToWriteOnlyArrayView<T, CTV>)),
                ProjectedFunction::Create("indexOf"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVectorView<T>>::IndexOf,
                        &ConvertBooleanToValue, ProjectedFunctionParamInfo(ProjectedFunctionInParam, &CVT),
                        ProjectedFunctionParamInfo(
                            ProjectedFunctionOutParam, &ConvertNumberToValue<uint32_t>, "index"))),
            },
            {} };
    }

    template <typename T, auto CTV, auto CVT>
    ProjectedInterfaceData InitIVectorInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IVector<T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IVector<T>::Size, &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("append"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::Append, &CVT)),
                ProjectedFunction::Create("clear"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::Clear)),
                ProjectedFunction::Create(
                    "getAt"sv, ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                                   &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::GetAt,
                                   &CTV, &ConvertValueToNumber<uint32_t>)),
                ProjectedFunction::Create("getMany"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::GetMany,
                        &ConvertNumberToValue<uint32_t>, &ConvertValueToNumber<uint32_t>,
                        &ConvertValueToWriteOnlyArrayView<T, CTV>)),
                ProjectedFunction::Create("getView"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::GetView,
                        &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IVectorView<T>>)),
                ProjectedFunction::Create("indexOf"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::IndexOf,
                        &ConvertBooleanToValue, ProjectedFunctionParamInfo(ProjectedFunctionInParam, &CVT),
                        ProjectedFunctionParamInfo(
                            ProjectedFunctionOutParam, &ConvertNumberToValue<uint32_t>, "index"))),
                ProjectedFunction::Create("insertAt"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::InsertAt,
                        &ConvertValueToNumber<uint32_t>, &CVT)),
                ProjectedFunction::Create("replaceAll"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::ReplaceAll,
                        &ConvertValueToReadOnlyArrayView<T, CVT>)),
                ProjectedFunction::Create("removeAt"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::RemoveAt,
                        &ConvertValueToNumber<uint32_t>)),
                ProjectedFunction::Create("removeAtEnd"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::RemoveAtEnd)),
                ProjectedFunction::Create(
                    "setAt"sv, ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                                   &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::SetAt,
                                   &ConvertValueToNumber<uint32_t>, &CVT)),
            },
            {} };
    }

    template <typename T>
    ProjectedInterfaceData InitIObservableVectorInterface()
    {
        return { {}, {},
            {
                ProjectedEvent::Create<winrt::Windows::Foundation::Collections::IObservableVector<T>>("vectorChanged"sv,
                    &winrt::Windows::Foundation::Collections::IObservableVector<T>::VectorChanged,
                    &winrt::Windows::Foundation::Collections::IObservableVector<T>::VectorChanged,
                    &ConvertValueToVectorChangedEventHandler<T>),
            } };
    }
}