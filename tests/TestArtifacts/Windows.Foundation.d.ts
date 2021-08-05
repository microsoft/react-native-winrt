//tslint:disable

declare namespace Windows.Foundation {
    type AsyncActionCompletedHandler = (asyncInfo: Windows.Foundation.WinRTPromise<void, void>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    type AsyncActionProgressHandler<TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<void, TProgress>, progressInfo: TProgress) => void;
    
    type AsyncActionWithProgressCompletedHandler<TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<void, TProgress>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    type AsyncOperationCompletedHandler<TResult> = (asyncInfo: Windows.Foundation.WinRTPromise<TResult, void>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    type AsyncOperationProgressHandler<TResult, TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<TResult, TProgress>, progressInfo: TProgress) => void;
    
    type AsyncOperationWithProgressCompletedHandler<TResult, TProgress> = (asyncInfo: Windows.Foundation.WinRTPromise<TResult, TProgress>, asyncStatus: Windows.Foundation.AsyncStatus) => void;
    
    enum AsyncStatus {
        canceled,
        completed,
        error,
        started,
    }

    interface DateTime {
         universalTime: number;
    }

    class Deferral implements Windows.Foundation.IClosable {
        constructor(handler: Windows.Foundation.DeferralCompletedHandler);
        complete(): void;
        close(): void;
    }

    type DeferralCompletedHandler = () => void;
    
    type EventHandler<T> = (sender: any, args: T) => void;
    
    interface EventRegistrationToken {
         value: number;
    }

    interface FoundationContract {
    }

    abstract class GuidHelper {
        static readonly empty: string;
        static createNewGuid(): string;
        static equals(target: string, value: string): boolean;
    }

    interface HResult {
         value: number;
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
        getDateTime(): Windows.Foundation.DateTime;
        getTimeSpan(): Windows.Foundation.TimeSpan;
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
        getDateTimeArray(): Windows.Foundation.DateTime[];
        getTimeSpanArray(): Windows.Foundation.TimeSpan[];
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
        constructor(capacity: number);
        createReference(): Windows.Foundation.IMemoryBufferReference;
        close(): void;
    }

    interface Point {
         x: number;
         y: number;
    }

    enum PropertyType {
        empty,
        uInt8,
        int16,
        uInt16,
        int32,
        uInt32,
        int64,
        uInt64,
        single,
        double,
        char16,
        boolean,
        string,
        inspectable,
        dateTime,
        timeSpan,
        guid,
        point,
        size,
        rect,
        otherType,
        uInt8Array,
        int16Array,
        uInt16Array,
        int32Array,
        uInt32Array,
        int64Array,
        uInt64Array,
        singleArray,
        doubleArray,
        char16Array,
        booleanArray,
        stringArray,
        inspectableArray,
        dateTimeArray,
        timeSpanArray,
        guidArray,
        pointArray,
        sizeArray,
        rectArray,
        otherTypeArray,
    }

    abstract class PropertyValue {
        static createEmpty(): any;
        static createUInt8(value: number): any;
        static createInt16(value: number): any;
        static createUInt16(value: number): any;
        static createInt32(value: number): any;
        static createUInt32(value: number): any;
        static createInt64(value: number): any;
        static createUInt64(value: number): any;
        static createSingle(value: number): any;
        static createDouble(value: number): any;
        static createChar16(value: string): any;
        static createBoolean(value: boolean): any;
        static createString(value: string): any;
        static createInspectable(value: any): any;
        static createGuid(value: string): any;
        static createDateTime(value: Windows.Foundation.DateTime): any;
        static createTimeSpan(value: Windows.Foundation.TimeSpan): any;
        static createPoint(value: Windows.Foundation.Point): any;
        static createSize(value: Windows.Foundation.Size): any;
        static createRect(value: Windows.Foundation.Rect): any;
        static createUInt8Array(value: number[]): any;
        static createInt16Array(value: number[]): any;
        static createUInt16Array(value: number[]): any;
        static createInt32Array(value: number[]): any;
        static createUInt32Array(value: number[]): any;
        static createInt64Array(value: number[]): any;
        static createUInt64Array(value: number[]): any;
        static createSingleArray(value: number[]): any;
        static createDoubleArray(value: number[]): any;
        static createChar16Array(value: string[]): any;
        static createBooleanArray(value: boolean[]): any;
        static createStringArray(value: string[]): any;
        static createInspectableArray(value: any): any;
        static createGuidArray(value: string[]): any;
        static createDateTimeArray(value: Windows.Foundation.DateTime[]): any;
        static createTimeSpanArray(value: Windows.Foundation.TimeSpan[]): any;
        static createPointArray(value: Windows.Foundation.Point[]): any;
        static createSizeArray(value: Windows.Foundation.Size[]): any;
        static createRectArray(value: Windows.Foundation.Rect[]): any;
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

    interface TimeSpan {
         duration: number;
    }

    type TypedEventHandler<TSender, TResult> = (sender: TSender, args: TResult) => void;
    
    interface UniversalApiContract {
    }

    class Uri implements Windows.Foundation.IStringable {
        readonly absoluteUri: string;
        readonly displayUri: string;
        readonly domain: string;
        readonly extension: string;
        readonly fragment: string;
        readonly host: string;
        readonly password: string;
        readonly path: string;
        readonly port: number;
        readonly query: string;
        readonly queryParsed: Windows.Foundation.WwwFormUrlDecoder;
        readonly rawUri: string;
        readonly schemeName: string;
        readonly suspicious: boolean;
        readonly userName: string;
        readonly absoluteCanonicalUri: string;
        readonly displayIri: string;
        constructor(uri: string);
        constructor(baseUri: string, relativeUri: string);
        equals(pUri: Windows.Foundation.Uri): boolean;
        combineUri(relativeUri: string): Windows.Foundation.Uri;
        toString(): string;
        static unescapeComponent(toUnescape: string): string;
        static escapeComponent(toEscape: string): string;
    }

    class WwwFormUrlDecoder implements Windows.Foundation.Collections.IVectorView<Windows.Foundation.IWwwFormUrlDecoderEntry>, Windows.Foundation.Collections.IIterable<Windows.Foundation.IWwwFormUrlDecoderEntry> {
        readonly size: number;
        constructor(query: string);
        getFirstValueByName(name: string): string;
        first(): Windows.Foundation.Collections.IIterator<Windows.Foundation.IWwwFormUrlDecoderEntry>;
        getAt(index: number): Windows.Foundation.IWwwFormUrlDecoderEntry;
        indexOf(value: Windows.Foundation.IWwwFormUrlDecoderEntry): { index: number; returnValue: boolean };
        getMany(startIndex: number): { items: Windows.Foundation.IWwwFormUrlDecoderEntry[]; returnValue: number };
    }

    class WwwFormUrlDecoderEntry implements Windows.Foundation.IWwwFormUrlDecoderEntry {
        readonly name: string;
        readonly value: string;
    }

}