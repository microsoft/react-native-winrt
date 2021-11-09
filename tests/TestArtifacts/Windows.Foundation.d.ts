//tslint:disable

declare namespace Windows.Foundation {
    type AsyncActionCompletedHandler = (asyncInfo: Windows.Foundation.WinRTPromise<void, void>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    type AsyncActionProgressHandler<TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<void, TProgress>, progressInfo: TProgress) => void;
    
    type AsyncActionWithProgressCompletedHandler<TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<void, TProgress>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    type AsyncOperationCompletedHandler<TResult> = (asyncInfo: Windows.Foundation.WinRTPromise<TResult, void>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    type AsyncOperationProgressHandler<TResult, TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<TResult, TProgress>, progressInfo: TProgress) => void;
    
    type AsyncOperationWithProgressCompletedHandler<TResult, TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<TResult, TProgress>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    enum AsyncStatus {
        canceled = 2,
        completed = 1,
        error = 3,
        started = 0,
    }

    class Deferral implements Windows.Foundation.IClosable {
        public constructor(handler: Windows.Foundation.DeferralCompletedHandler);
        public complete(): void;
        public close(): void;
    }

    type DeferralCompletedHandler = () => void;
    
    type EventHandler<T> = (sender: any, args: T) => void;
    
    interface EventRegistrationToken {
        value: number;
    }

    interface FoundationContract {
    }

    abstract class GuidHelper {
        public static readonly empty: string;
        public static createNewGuid(): string;
        public static equals(target: string, value: string): boolean;
    }

    interface IAsyncAction extends Windows.Foundation.IAsyncInfo {
        completed: Windows.Foundation.AsyncActionCompletedHandler;
        getResults(): void;
    }

    interface IAsyncActionWithProgress<TProgress> extends Windows.Foundation.IAsyncInfo {
        progress: Windows.Foundation.AsyncActionProgressHandler<TProgress>;
        completed: Windows.Foundation.AsyncActionWithProgressCompletedHandler<TProgress>;
        getResults(): void;
    }

    interface WinRTPromiseBase<TResult, TProgress> extends PromiseLike<TResult> {
        then<U, V>(success?: (value: TResult) => Promise<U>, error?: (error: any) => Promise<U>, progress?: (progress: TProgress) => void): Promise<U>;
        then<U, V>(success?: (value: TResult) => Promise<U>, error?: (error: any) => U, progress?: (progress: TProgress) => void): Promise<U>;
        then<U, V>(success?: (value: TResult) => U, error?: (error: any) => Promise<U>, progress?: (progress: TProgress) => void): Promise<U>;
        then<U, V>(success?: (value: TResult) => U, error?: (error: any) => U, progress?: (progress: TProgress) => void): Promise<U>;
        done<U, V>(success?: (value: TResult) => any, error?: (error: any) => any, progress?: (progress: TProgress) => void): void;
        cancel(): void;
        operation: IAsyncInfo;
    }

    type WinRTPromise<TResult, TProgress> = WinRTPromiseBase<TResult, TProgress> & Promise<TResult>;

    interface IAsyncInfo {
        readonly errorCode: number;
        readonly id: number;
        readonly status: Windows.Foundation.AsyncStatus;
        cancel(): void;
        close(): void;
    }

    interface IAsyncOperationWithProgress<TResult, TProgress> extends Windows.Foundation.IAsyncInfo {
        progress: Windows.Foundation.AsyncOperationProgressHandler<TResult, TProgress>;
        completed: Windows.Foundation.AsyncOperationWithProgressCompletedHandler<TResult, TProgress>;
        getResults(): TResult;
    }

    interface IAsyncOperation<TResult> extends Windows.Foundation.IAsyncInfo {
        completed: Windows.Foundation.AsyncOperationCompletedHandler<TResult>;
        getResults(): TResult;
    }

    interface IClosable {
        close(): void;
    }

    interface IMemoryBuffer extends Windows.Foundation.IClosable {
        createReference(): Windows.Foundation.IMemoryBufferReference;
    }

    interface IMemoryBufferReference extends Windows.Foundation.IClosable {
        readonly capacity: number;
        addEventListener(type: "closed", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.IMemoryBufferReference, any>): void;
        removeEventListener(type: "closed", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.IMemoryBufferReference, any>): void;
    }

    interface IPropertyValue {
        readonly isNumericScalar: boolean;
        readonly type: Windows.Foundation.PropertyType;
        getUInt8(): number;
        getInt16(): number;
        getUInt16(): number;
        getInt32(): number;
        getUInt32(): number;
        getInt64(): number;
        getUInt64(): number;
        getSingle(): number;
        getDouble(): number;
        getChar16(): string;
        getBoolean(): boolean;
        getString(): string;
        getGuid(): string;
        getDateTime(): Date;
        getTimeSpan(): number;
        getPoint(): Windows.Foundation.Point;
        getSize(): Windows.Foundation.Size;
        getRect(): Windows.Foundation.Rect;
        getUInt8Array(): number[];
        getInt16Array(): number[];
        getUInt16Array(): number[];
        getInt32Array(): number[];
        getUInt32Array(): number[];
        getInt64Array(): number[];
        getUInt64Array(): number[];
        getSingleArray(): number[];
        getDoubleArray(): number[];
        getChar16Array(): string[];
        getBooleanArray(): boolean[];
        getStringArray(): string[];
        getInspectableArray(): any;
        getGuidArray(): string[];
        getDateTimeArray(): Date[];
        getTimeSpanArray(): number[];
        getPointArray(): Windows.Foundation.Point[];
        getSizeArray(): Windows.Foundation.Size[];
        getRectArray(): Windows.Foundation.Rect[];
    }

    interface IReferenceArray<T> extends Windows.Foundation.IPropertyValue {
        readonly value: T[];
    }

    interface IReference<T> extends Windows.Foundation.IPropertyValue {
        readonly value: T;
    }

    interface IStringable {
        toString(): string;
    }

    interface IWwwFormUrlDecoderEntry {
        readonly name: string;
        readonly value: string;
    }

    class MemoryBuffer implements Windows.Foundation.IMemoryBuffer, Windows.Foundation.IClosable {
        public constructor(capacity: number);
        public createReference(): Windows.Foundation.IMemoryBufferReference;
        public close(): void;
    }

    interface Point {
        x: number;
        y: number;
    }

    enum PropertyType {
        empty = 0,
        uInt8 = 1,
        int16 = 2,
        uInt16 = 3,
        int32 = 4,
        uInt32 = 5,
        int64 = 6,
        uInt64 = 7,
        single = 8,
        double = 9,
        char16 = 10,
        boolean = 11,
        string = 12,
        inspectable = 13,
        dateTime = 14,
        timeSpan = 15,
        guid = 16,
        point = 17,
        size = 18,
        rect = 19,
        otherType = 20,
        uInt8Array = 1025,
        int16Array = 1026,
        uInt16Array = 1027,
        int32Array = 1028,
        uInt32Array = 1029,
        int64Array = 1030,
        uInt64Array = 1031,
        singleArray = 1032,
        doubleArray = 1033,
        char16Array = 1034,
        booleanArray = 1035,
        stringArray = 1036,
        inspectableArray = 1037,
        dateTimeArray = 1038,
        timeSpanArray = 1039,
        guidArray = 1040,
        pointArray = 1041,
        sizeArray = 1042,
        rectArray = 1043,
        otherTypeArray = 1044,
    }

    abstract class PropertyValue {
        public static createEmpty(): any;
        public static createUInt8(value: number): any;
        public static createInt16(value: number): any;
        public static createUInt16(value: number): any;
        public static createInt32(value: number): any;
        public static createUInt32(value: number): any;
        public static createInt64(value: number): any;
        public static createUInt64(value: number): any;
        public static createSingle(value: number): any;
        public static createDouble(value: number): any;
        public static createChar16(value: string): any;
        public static createBoolean(value: boolean): any;
        public static createString(value: string): any;
        public static createInspectable(value: any): any;
        public static createGuid(value: string): any;
        public static createDateTime(value: Date): any;
        public static createTimeSpan(value: number): any;
        public static createPoint(value: Windows.Foundation.Point): any;
        public static createSize(value: Windows.Foundation.Size): any;
        public static createRect(value: Windows.Foundation.Rect): any;
        public static createUInt8Array(value: number[]): any;
        public static createInt16Array(value: number[]): any;
        public static createUInt16Array(value: number[]): any;
        public static createInt32Array(value: number[]): any;
        public static createUInt32Array(value: number[]): any;
        public static createInt64Array(value: number[]): any;
        public static createUInt64Array(value: number[]): any;
        public static createSingleArray(value: number[]): any;
        public static createDoubleArray(value: number[]): any;
        public static createChar16Array(value: string[]): any;
        public static createBooleanArray(value: boolean[]): any;
        public static createStringArray(value: string[]): any;
        public static createInspectableArray(value: any): any;
        public static createGuidArray(value: string[]): any;
        public static createDateTimeArray(value: Date[]): any;
        public static createTimeSpanArray(value: number[]): any;
        public static createPointArray(value: Windows.Foundation.Point[]): any;
        public static createSizeArray(value: Windows.Foundation.Size[]): any;
        public static createRectArray(value: Windows.Foundation.Rect[]): any;
    }

    interface Rect {
        x: number;
        y: number;
        width: number;
        height: number;
    }

    interface Size {
        width: number;
        height: number;
    }

    type TypedEventHandler<TSender, TResult> = (sender: TSender, args: TResult) => void;
    
    interface UniversalApiContract {
    }

    class Uri implements Windows.Foundation.IStringable {
        public readonly absoluteUri: string;
        public readonly displayUri: string;
        public readonly domain: string;
        public readonly extension: string;
        public readonly fragment: string;
        public readonly host: string;
        public readonly password: string;
        public readonly path: string;
        public readonly port: number;
        public readonly query: string;
        public readonly queryParsed: Windows.Foundation.WwwFormUrlDecoder;
        public readonly rawUri: string;
        public readonly schemeName: string;
        public readonly suspicious: boolean;
        public readonly userName: string;
        public readonly absoluteCanonicalUri: string;
        public readonly displayIri: string;
        public constructor(uri: string);
        public constructor(baseUri: string, relativeUri: string);
        public equals(pUri: Windows.Foundation.Uri): boolean;
        public combineUri(relativeUri: string): Windows.Foundation.Uri;
        public toString(): string;
        public static unescapeComponent(toUnescape: string): string;
        public static escapeComponent(toEscape: string): string;
    }

    class WwwFormUrlDecoder implements Windows.Foundation.Collections.IVectorView<Windows.Foundation.IWwwFormUrlDecoderEntry>, Windows.Foundation.Collections.IIterable<Windows.Foundation.IWwwFormUrlDecoderEntry> {
        public readonly size: number;
        public constructor(query: string);
        public getFirstValueByName(name: string): string;
        public first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.IWwwFormUrlDecoderEntry>;
        public getAt(index: number): Windows.Foundation.IWwwFormUrlDecoderEntry;
        public indexOf(value: Windows.Foundation.IWwwFormUrlDecoderEntry): { index: number; returnValue: boolean };
        public getMany(startIndex: number): { items: Windows.Foundation.IWwwFormUrlDecoderEntry[]; returnValue: number };
    }

    class WwwFormUrlDecoderEntry implements Windows.Foundation.IWwwFormUrlDecoderEntry {
        public readonly name: string;
        public readonly value: string;
    }

}