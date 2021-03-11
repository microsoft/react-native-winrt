//tslint:disable

declare namespace Windows.Foundation.Collections {
    enum CollectionChange {
        reset,
        itemInserted,
        itemRemoved,
        itemChanged,
    }

    interface IIterable<T> {
        first(): Windows.Foundation.Collections.IIterator<T>;
    }

    interface IIterator<T> {
        readonly current: T;
        readonly hasCurrent: boolean;
        moveNext(): boolean;
        getMany(): { items: T[]; returnValue: number };
    }

    interface IKeyValuePair<K, V> {
        readonly key: K;
        readonly value: V;
    }

    interface IMapChangedEventArgs<K> {
        readonly collectionChange: Windows.Foundation.Collections.CollectionChange;
        readonly key: K;
    }

    interface IMapView<K, V> extends Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<K, V>> {
        readonly size: number;
        lookup(key: K): V;
        hasKey(key: K): boolean;
        split(): { first: Windows.Foundation.Collections.IMapView<K, V>; second: Windows.Foundation.Collections.IMapView<K, V> };
    }

    interface IMap<K, V> extends Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<K, V>> {
        readonly size: number;
        lookup(key: K): V;
        hasKey(key: K): boolean;
        getView(): Windows.Foundation.Collections.IMapView<K, V>;
        insert(key: K, value: V): boolean;
        remove(key: K): void;
        clear(): void;
    }

    interface IObservableMap<K, V> extends Windows.Foundation.Collections.IMap<K, V> {
        addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<K, V>): void
        removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<K, V>): void
    }

    interface IObservableVector<T> extends Windows.Foundation.Collections.IVector<T> {
        addEventListener(type: "vectorchanged", listener: Windows.Foundation.Collections.VectorChangedEventHandler<T>): void
        removeEventListener(type: "vectorchanged", listener: Windows.Foundation.Collections.VectorChangedEventHandler<T>): void
    }

    interface IPropertySet extends Windows.Foundation.Collections.IObservableMap<string, any>, Windows.Foundation.Collections.IMap<string, any>, Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<string, any>> {
    }

    interface IVectorChangedEventArgs {
        readonly collectionChange: Windows.Foundation.Collections.CollectionChange;
        readonly index: number;
    }

    interface IVectorView<T> extends Windows.Foundation.Collections.IIterable<T> {
        readonly size: number;
        getAt(index: number): T;
        indexOf(value: T): { index: number; returnValue: boolean };
        getMany(startIndex: number): { items: T[]; returnValue: number };
    }

    interface IVector<T> extends Windows.Foundation.Collections.IIterable<T> {
        readonly size: number;
        getAt(index: number): T;
        getView(): Windows.Foundation.Collections.IVectorView<T>;
        indexOf(value: T): { index: number; returnValue: boolean };
        setAt(index: number, value: T): void;
        insertAt(index: number, value: T): void;
        removeAt(index: number): void;
        append(value: T): void;
        removeAtEnd(): void;
        clear(): void;
        getMany(startIndex: number): { items: T[]; returnValue: number };
        replaceAll(items: T[]): void;
    }

    type MapChangedEventHandler<K, V> = (sender: Windows.Foundation.Collections.IObservableMap<K, V>, event: Windows.Foundation.Collections.IMapChangedEventArgs<K>) => void;
    
    class PropertySet implements Windows.Foundation.Collections.IPropertySet, Windows.Foundation.Collections.IObservableMap<string, any>, Windows.Foundation.Collections.IMap<string, any>, Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<string, any>> {
        readonly size: number;
        constructor();
        lookup(key: string): any;
        hasKey(key: string): boolean;
        getView(): Windows.Foundation.Collections.IMapView<string, any>;
        insert(key: string, value: any): boolean;
        remove(key: string): void;
        clear(): void;
        first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.Collections.IKeyValuePair<string, any>>;
        addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void
        removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void
    }

    class StringMap implements Windows.Foundation.Collections.IMap<string, string>, Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<string, string>>, Windows.Foundation.Collections.IObservableMap<string, string> {
        readonly size: number;
        constructor();
        lookup(key: string): string;
        hasKey(key: string): boolean;
        getView(): Windows.Foundation.Collections.IMapView<string, string>;
        insert(key: string, value: string): boolean;
        remove(key: string): void;
        clear(): void;
        first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.Collections.IKeyValuePair<string, string>>;
        addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, string>): void
        removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, string>): void
    }

    class ValueSet implements Windows.Foundation.Collections.IPropertySet, Windows.Foundation.Collections.IObservableMap<string, any>, Windows.Foundation.Collections.IMap<string, any>, Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<string, any>> {
        readonly size: number;
        constructor();
        lookup(key: string): any;
        hasKey(key: string): boolean;
        getView(): Windows.Foundation.Collections.IMapView<string, any>;
        insert(key: string, value: any): boolean;
        remove(key: string): void;
        clear(): void;
        first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.Collections.IKeyValuePair<string, any>>;
        addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void
        removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void
    }

    type VectorChangedEventHandler<T> = (sender: Windows.Foundation.Collections.IObservableVector<T>, event: Windows.Foundation.Collections.IVectorChangedEventArgs) => void;
    
}