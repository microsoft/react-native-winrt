// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

//tslint:disable

declare namespace Windows.Foundation.Diagnostics {
    abstract class AsyncCausalityTracer {
        public static traceOperationCreation(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, operationName: string, relatedContext: number): void;
        public static traceOperationCompletion(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, status: Windows.Foundation.AsyncStatus): void;
        public static traceOperationRelation(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, relation: Windows.Foundation.Diagnostics.CausalityRelation): void;
        public static traceSynchronousWorkStart(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, work: Windows.Foundation.Diagnostics.CausalitySynchronousWork): void;
        public static traceSynchronousWorkCompletion(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, work: Windows.Foundation.Diagnostics.CausalitySynchronousWork): void;
        public static addEventListener(type: "tracingstatuschanged", listener: Windows.Foundation.EventHandler<Windows.Foundation.Diagnostics.TracingStatusChangedEventArgs>): void;
        public static removeEventListener(type: "tracingstatuschanged", listener: Windows.Foundation.EventHandler<Windows.Foundation.Diagnostics.TracingStatusChangedEventArgs>): void;
    }

    enum CausalityRelation {
        assignDelegate = 0,
        join = 1,
        choice = 2,
        cancel = 3,
        error = 4,
    }

    enum CausalitySource {
        application = 0,
        library = 1,
        system = 2,
    }

    enum CausalitySynchronousWork {
        completionNotification = 0,
        progressNotification = 1,
        execution = 2,
    }

    enum CausalityTraceLevel {
        required = 0,
        important = 1,
        verbose = 2,
    }

    class ErrorDetails {
        public readonly description: string;
        public readonly helpUri: Windows.Foundation.Uri;
        public readonly longDescription: string;
        public static createFromHResultAsync(errorCode: number): Windows.Foundation.WinRTPromise<Windows.Foundation.Diagnostics.ErrorDetails, void>;
    }

    enum ErrorOptions {
        none = 0,
        suppressExceptions = 1,
        forceExceptions = 2,
        useSetErrorInfo = 4,
        suppressSetErrorInfo = 8,
    }

    class FileLoggingSession implements Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.IClosable {
        public readonly name: string;
        public constructor(name: string);
        public addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        public addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, maxLevel: Windows.Foundation.Diagnostics.LoggingLevel): void;
        public removeLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        public closeAndSaveToFileAsync(): Windows.Foundation.WinRTPromise<any, void>;
        public close(): void;
        public addEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void;
        public removeEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void;
    }

    interface IErrorReportingSettings {
        setErrorOptions(value: Windows.Foundation.Diagnostics.ErrorOptions): void;
        getErrorOptions(): Windows.Foundation.Diagnostics.ErrorOptions;
    }

    interface IFileLoggingSession extends Windows.Foundation.IClosable {
        readonly name: string;
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, maxLevel: Windows.Foundation.Diagnostics.LoggingLevel): void;
        removeLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        closeAndSaveToFileAsync(): Windows.Foundation.WinRTPromise<any, void>;
        addEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void;
        removeEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void;
    }

    interface ILoggingChannel extends Windows.Foundation.IClosable {
        readonly enabled: boolean;
        readonly level: Windows.Foundation.Diagnostics.LoggingLevel;
        readonly name: string;
        logMessage(eventString: string): void;
        logMessage(eventString: string, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        logValuePair(value1: string, value2: number): void;
        logValuePair(value1: string, value2: number, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        addEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void;
        removeEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void;
    }

    interface ILoggingSession extends Windows.Foundation.IClosable {
        readonly name: string;
        saveToFileAsync(folder: any, fileName: string): Windows.Foundation.WinRTPromise<any, void>;
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, maxLevel: Windows.Foundation.Diagnostics.LoggingLevel): void;
        removeLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
    }

    interface ILoggingTarget {
        isEnabled(): boolean;
        isEnabled(level: Windows.Foundation.Diagnostics.LoggingLevel): boolean;
        isEnabled(level: Windows.Foundation.Diagnostics.LoggingLevel, keywords: number): boolean;
        logEvent(eventName: string): void;
        logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): void;
        logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel, options: Windows.Foundation.Diagnostics.LoggingOptions): void;
        startActivity(startEventName: string): Windows.Foundation.Diagnostics.LoggingActivity;
        startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): Windows.Foundation.Diagnostics.LoggingActivity;
        startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel): Windows.Foundation.Diagnostics.LoggingActivity;
        startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel, options: Windows.Foundation.Diagnostics.LoggingOptions): Windows.Foundation.Diagnostics.LoggingActivity;
    }

    class LogFileGeneratedEventArgs {
        public readonly file: any;
    }

    class LoggingActivity implements Windows.Foundation.IClosable, Windows.Foundation.Diagnostics.ILoggingTarget {
        public readonly id: string;
        public readonly name: string;
        public readonly channel: Windows.Foundation.Diagnostics.LoggingChannel;
        public constructor(activityName: string, loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel);
        public constructor(activityName: string, loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, level: Windows.Foundation.Diagnostics.LoggingLevel);
        public close(): void;
        public stopActivity(stopEventName: string): void;
        public stopActivity(stopEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): void;
        public stopActivity(stopEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, options: Windows.Foundation.Diagnostics.LoggingOptions): void;
        public isEnabled(): boolean;
        public isEnabled(level: Windows.Foundation.Diagnostics.LoggingLevel): boolean;
        public isEnabled(level: Windows.Foundation.Diagnostics.LoggingLevel, keywords: number): boolean;
        public logEvent(eventName: string): void;
        public logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): void;
        public logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        public logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel, options: Windows.Foundation.Diagnostics.LoggingOptions): void;
        public startActivity(startEventName: string): Windows.Foundation.Diagnostics.LoggingActivity;
        public startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): Windows.Foundation.Diagnostics.LoggingActivity;
        public startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel): Windows.Foundation.Diagnostics.LoggingActivity;
        public startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel, options: Windows.Foundation.Diagnostics.LoggingOptions): Windows.Foundation.Diagnostics.LoggingActivity;
    }

    class LoggingChannel implements Windows.Foundation.Diagnostics.ILoggingChannel, Windows.Foundation.IClosable, Windows.Foundation.Diagnostics.ILoggingTarget {
        public readonly enabled: boolean;
        public readonly level: Windows.Foundation.Diagnostics.LoggingLevel;
        public readonly name: string;
        public readonly id: string;
        public constructor(name: string, options: Windows.Foundation.Diagnostics.LoggingChannelOptions);
        public constructor(name: string, options: Windows.Foundation.Diagnostics.LoggingChannelOptions, id: string);
        public logMessage(eventString: string): void;
        public logMessage(eventString: string, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        public logValuePair(value1: string, value2: number): void;
        public logValuePair(value1: string, value2: number, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        public close(): void;
        public isEnabled(): boolean;
        public isEnabled(level: Windows.Foundation.Diagnostics.LoggingLevel): boolean;
        public isEnabled(level: Windows.Foundation.Diagnostics.LoggingLevel, keywords: number): boolean;
        public logEvent(eventName: string): void;
        public logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): void;
        public logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        public logEvent(eventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel, options: Windows.Foundation.Diagnostics.LoggingOptions): void;
        public startActivity(startEventName: string): Windows.Foundation.Diagnostics.LoggingActivity;
        public startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): Windows.Foundation.Diagnostics.LoggingActivity;
        public startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel): Windows.Foundation.Diagnostics.LoggingActivity;
        public startActivity(startEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, level: Windows.Foundation.Diagnostics.LoggingLevel, options: Windows.Foundation.Diagnostics.LoggingOptions): Windows.Foundation.Diagnostics.LoggingActivity;
        public addEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void;
        public removeEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void;
    }

    class LoggingChannelOptions {
        public group: string;
        public constructor();
        public constructor(group: string);
    }

    enum LoggingFieldFormat {
        default = 0,
        hidden = 1,
        string = 2,
        boolean = 3,
        hexadecimal = 4,
        processId = 5,
        threadId = 6,
        port = 7,
        ipv4Address = 8,
        ipv6Address = 9,
        socketAddress = 10,
        xml = 11,
        json = 12,
        win32Error = 13,
        nTStatus = 14,
        hResult = 15,
        fileTime = 16,
        signed = 17,
        unsigned = 18,
    }

    class LoggingFields {
        public constructor();
        public clear(): void;
        public beginStruct(name: string): void;
        public beginStruct(name: string, tags: number): void;
        public endStruct(): void;
        public addEmpty(name: string): void;
        public addEmpty(name: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addEmpty(name: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt8(name: string, value: number): void;
        public addUInt8(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt8(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt8Array(name: string, value: number[]): void;
        public addUInt8Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt8Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addInt16(name: string, value: number): void;
        public addInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addInt16Array(name: string, value: number[]): void;
        public addInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt16(name: string, value: number): void;
        public addUInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt16Array(name: string, value: number[]): void;
        public addUInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addInt32(name: string, value: number): void;
        public addInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addInt32Array(name: string, value: number[]): void;
        public addInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt32(name: string, value: number): void;
        public addUInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt32Array(name: string, value: number[]): void;
        public addUInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addInt64(name: string, value: number): void;
        public addInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addInt64Array(name: string, value: number[]): void;
        public addInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt64(name: string, value: number): void;
        public addUInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addUInt64Array(name: string, value: number[]): void;
        public addUInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addUInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addSingle(name: string, value: number): void;
        public addSingle(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addSingle(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addSingleArray(name: string, value: number[]): void;
        public addSingleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addSingleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addDouble(name: string, value: number): void;
        public addDouble(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addDouble(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addDoubleArray(name: string, value: number[]): void;
        public addDoubleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addDoubleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addChar16(name: string, value: string): void;
        public addChar16(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addChar16(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addChar16Array(name: string, value: string[]): void;
        public addChar16Array(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addChar16Array(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addBoolean(name: string, value: boolean): void;
        public addBoolean(name: string, value: boolean, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addBoolean(name: string, value: boolean, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addBooleanArray(name: string, value: boolean[]): void;
        public addBooleanArray(name: string, value: boolean[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addBooleanArray(name: string, value: boolean[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addString(name: string, value: string): void;
        public addString(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addString(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addStringArray(name: string, value: string[]): void;
        public addStringArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addStringArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addGuid(name: string, value: string): void;
        public addGuid(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addGuid(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addGuidArray(name: string, value: string[]): void;
        public addGuidArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addGuidArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addDateTime(name: string, value: Date): void;
        public addDateTime(name: string, value: Date, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addDateTime(name: string, value: Date, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addDateTimeArray(name: string, value: Date[]): void;
        public addDateTimeArray(name: string, value: Date[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addDateTimeArray(name: string, value: Date[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addTimeSpan(name: string, value: number): void;
        public addTimeSpan(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addTimeSpan(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addTimeSpanArray(name: string, value: number[]): void;
        public addTimeSpanArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addTimeSpanArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addPoint(name: string, value: Windows.Foundation.Point): void;
        public addPoint(name: string, value: Windows.Foundation.Point, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addPoint(name: string, value: Windows.Foundation.Point, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addPointArray(name: string, value: Windows.Foundation.Point[]): void;
        public addPointArray(name: string, value: Windows.Foundation.Point[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addPointArray(name: string, value: Windows.Foundation.Point[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addSize(name: string, value: Windows.Foundation.Size): void;
        public addSize(name: string, value: Windows.Foundation.Size, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addSize(name: string, value: Windows.Foundation.Size, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addSizeArray(name: string, value: Windows.Foundation.Size[]): void;
        public addSizeArray(name: string, value: Windows.Foundation.Size[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addSizeArray(name: string, value: Windows.Foundation.Size[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addRect(name: string, value: Windows.Foundation.Rect): void;
        public addRect(name: string, value: Windows.Foundation.Rect, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addRect(name: string, value: Windows.Foundation.Rect, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        public addRectArray(name: string, value: Windows.Foundation.Rect[]): void;
        public addRectArray(name: string, value: Windows.Foundation.Rect[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        public addRectArray(name: string, value: Windows.Foundation.Rect[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
    }

    enum LoggingLevel {
        verbose = 0,
        information = 1,
        warning = 2,
        error = 3,
        critical = 4,
    }

    enum LoggingOpcode {
        info = 0,
        start = 1,
        stop = 2,
        reply = 6,
        resume = 7,
        suspend = 8,
        send = 9,
    }

    class LoggingOptions {
        public task: number;
        public tags: number;
        public relatedActivityId: string;
        public opcode: Windows.Foundation.Diagnostics.LoggingOpcode;
        public keywords: number;
        public activityId: string;
        public constructor();
        public constructor(keywords: number);
    }

    class LoggingSession implements Windows.Foundation.Diagnostics.ILoggingSession, Windows.Foundation.IClosable {
        public readonly name: string;
        public constructor(name: string);
        public saveToFileAsync(folder: any, fileName: string): Windows.Foundation.WinRTPromise<any, void>;
        public addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        public addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, maxLevel: Windows.Foundation.Diagnostics.LoggingLevel): void;
        public removeLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        public close(): void;
    }

    class RuntimeBrokerErrorSettings implements Windows.Foundation.Diagnostics.IErrorReportingSettings {
        public constructor();
        public setErrorOptions(value: Windows.Foundation.Diagnostics.ErrorOptions): void;
        public getErrorOptions(): Windows.Foundation.Diagnostics.ErrorOptions;
    }

    class TracingStatusChangedEventArgs {
        public readonly enabled: boolean;
        public readonly traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel;
    }

}