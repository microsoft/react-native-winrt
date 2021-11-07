// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

//tslint:disable

declare namespace Windows.Foundation.Collections {
    enum CollectionChange {
        reset = 0,
        itemInserted = 1,
        itemRemoved = 2,
        itemChanged = 3,
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
        readonly [key: string]: any;
    }

    interface IMap<K, V> extends Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<K, V>> {
        readonly size: number;
        lookup(key: K): V;
        hasKey(key: K): boolean;
        getView(): Windows.Foundation.Collections.IMapView<K, V>;
        insert(key: K, value: V): boolean;
        remove(key: K): void;
        clear(): void;
        [key: string]: any;
    }

    interface IObservableMap<K, V> extends Windows.Foundation.Collections.IMap<K, V> {
        addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<K, V>): void;
        removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<K, V>): void;
    }

    interface IObservableVector<T> extends Windows.Foundation.Collections.IVector<T> {
        addEventListener(type: "vectorchanged", listener: Windows.Foundation.Collections.VectorChangedEventHandler<T>): void;
        removeEventListener(type: "vectorchanged", listener: Windows.Foundation.Collections.VectorChangedEventHandler<T>): void;
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
        length: number;
        readonly [index: number]: T;
        concat(...items: (T | ConcatArray<T>)[]): T[];
        every<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): this is S[];
        filter<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): S[];
        filter(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): T[];
        forEach(callbackfn: (value: T, index: number, array: T[]) => void, thisArg?: any): void;
        lastIndexOf(searchElement: T, fromIndex?: number): number;
        map<U>(callbackfn: (value: T, index: number, array: T[]) => U, thisArg?: any): U[];
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        slice(start?: number, end?: number): T[];
        some(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): boolean;
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
        length: number;
        [index: number]: T;
        concat(...items: (T | ConcatArray<T>)[]): T[];
        copyWithin(target: number, start: number, end?: number): this;
        every<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): this is S[];
        some(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): boolean;
        filter<S extends T>(predicate: (value: T, index: number, array: T[]) => value is S, thisArg?: any): S[];
        filter(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): T[];
        forEach(callbackfn: (value: T, index: number, array: T[]) => void, thisArg?: any): void;
        lastIndexOf(searchElement: T, fromIndex?: number): number;
        pop(): T | undefined;
        push(...items: T[]): number;
        map<U>(callbackfn: (value: T, index: number, array: T[]) => U, thisArg?: any): U[];
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduce(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T): T;
        reduceRight(callbackfn: (previousValue: T, currentValue: T, currentIndex: number, array: T[]) => T, initialValue: T): T;
        reverse(): T[];
        shift(): T | undefined;
        slice(start?: number, end?: number): T[];
        sort(compareFn?: (a: T, b: T) => number): this;
        splice(start: number, deleteCount?: number): T[];
        some(predicate: (value: T, index: number, array: T[]) => unknown, thisArg?: any): boolean;
        unshift(...items: T[]): number;
    }

    type MapChangedEventHandler<K, V> = (sender: Windows.Foundation.Collections.IObservableMap<K, V>, event: Windows.Foundation.Collections.IMapChangedEventArgs<K>) => void;
    
    class PropertySet implements Windows.Foundation.Collections.IPropertySet, Windows.Foundation.Collections.IObservableMap<string, any>, Windows.Foundation.Collections.IMap<string, any>, Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<string, any>> {
        public readonly size: number;
        public constructor();
        public lookup(key: string): any;
        public hasKey(key: string): boolean;
        public getView(): Windows.Foundation.Collections.IMapView<string, any>;
        public insert(key: string, value: any): boolean;
        public remove(key: string): void;
        public clear(): void;
        public first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.Collections.IKeyValuePair<string, any>>;
        public addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void;
        public removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void;
    }

    class StringMap implements Windows.Foundation.Collections.IMap<string, string>, Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<string, string>>, Windows.Foundation.Collections.IObservableMap<string, string> {
        public readonly size: number;
        public constructor();
        public lookup(key: string): string;
        public hasKey(key: string): boolean;
        public getView(): Windows.Foundation.Collections.IMapView<string, string>;
        public insert(key: string, value: string): boolean;
        public remove(key: string): void;
        public clear(): void;
        public first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.Collections.IKeyValuePair<string, string>>;
        public addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, string>): void;
        public removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, string>): void;
    }

    class ValueSet implements Windows.Foundation.Collections.IPropertySet, Windows.Foundation.Collections.IObservableMap<string, any>, Windows.Foundation.Collections.IMap<string, any>, Windows.Foundation.Collections.IIterable<Windows.Foundation.Collections.IKeyValuePair<string, any>> {
        public readonly size: number;
        public constructor();
        public lookup(key: string): any;
        public hasKey(key: string): boolean;
        public getView(): Windows.Foundation.Collections.IMapView<string, any>;
        public insert(key: string, value: any): boolean;
        public remove(key: string): void;
        public clear(): void;
        public first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.Collections.IKeyValuePair<string, any>>;
        public addEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void;
        public removeEventListener(type: "mapchanged", listener: Windows.Foundation.Collections.MapChangedEventHandler<string, any>): void;
    }

    type VectorChangedEventHandler<T> = (sender: Windows.Foundation.Collections.IObservableVector<T>, event: Windows.Foundation.Collections.IVectorChangedEventArgs) => void;
    
}