//tslint:disable

declare namespace Windows.Foundation.Diagnostics {
    abstract class AsyncCausalityTracer {
        static traceOperationCreation(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, operationName: string, relatedContext: number): void;
        static traceOperationCompletion(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, status: Windows.Foundation.AsyncStatus): void;
        static traceOperationRelation(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, relation: Windows.Foundation.Diagnostics.CausalityRelation): void;
        static traceSynchronousWorkStart(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, platformId: string, operationId: number, work: Windows.Foundation.Diagnostics.CausalitySynchronousWork): void;
        static traceSynchronousWorkCompletion(traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel, source: Windows.Foundation.Diagnostics.CausalitySource, work: Windows.Foundation.Diagnostics.CausalitySynchronousWork): void;
        static addEventListener(type: "tracingstatuschanged", listener: Windows.Foundation.EventHandler<Windows.Foundation.Diagnostics.TracingStatusChangedEventArgs>): void
        static removeEventListener(type: "tracingstatuschanged", listener: Windows.Foundation.EventHandler<Windows.Foundation.Diagnostics.TracingStatusChangedEventArgs>): void
    }

    enum CausalityRelation {
        assignDelegate,
        join,
        choice,
        cancel,
        error,
    }

    enum CausalitySource {
        application,
        library,
        system,
    }

    enum CausalitySynchronousWork {
        completionNotification,
        progressNotification,
        execution,
    }

    enum CausalityTraceLevel {
        required,
        important,
        verbose,
    }

    class ErrorDetails {
        readonly description: string;
        readonly helpUri: Windows.Foundation.Uri;
        readonly longDescription: string;
        static createFromHResultAsync(errorCode: number): Windows.Foundation.WinRTPromise<Windows.Foundation.Diagnostics.ErrorDetails, void>;
    }

    enum ErrorOptions {
        none,
        suppressExceptions,
        forceExceptions,
        useSetErrorInfo,
        suppressSetErrorInfo,
    }

    class FileLoggingSession implements Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.IClosable {
        readonly name: string;
        constructor(name: string);
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, maxLevel: Windows.Foundation.Diagnostics.LoggingLevel): void;
        removeLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        closeAndSaveToFileAsync(): Windows.Foundation.WinRTPromise<any, void>;
        close(): void;
        addEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void
        removeEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void
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
        addEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void
        removeEventListener(type: "logfilegenerated", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.IFileLoggingSession, Windows.Foundation.Diagnostics.LogFileGeneratedEventArgs>): void
    }

    interface ILoggingChannel extends Windows.Foundation.IClosable {
        readonly enabled: boolean;
        readonly level: Windows.Foundation.Diagnostics.LoggingLevel;
        readonly name: string;
        logMessage(eventString: string): void;
        logMessage(eventString: string, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        logValuePair(value1: string, value2: number): void;
        logValuePair(value1: string, value2: number, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        addEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void
        removeEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void
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
        readonly file: any;
    }

    class LoggingActivity implements Windows.Foundation.IClosable, Windows.Foundation.Diagnostics.ILoggingTarget {
        readonly id: string;
        readonly name: string;
        readonly channel: Windows.Foundation.Diagnostics.LoggingChannel;
        constructor(activityName: string, loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel);
        constructor(activityName: string, loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, level: Windows.Foundation.Diagnostics.LoggingLevel);
        close(): void;
        stopActivity(stopEventName: string): void;
        stopActivity(stopEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields): void;
        stopActivity(stopEventName: string, fields: Windows.Foundation.Diagnostics.LoggingFields, options: Windows.Foundation.Diagnostics.LoggingOptions): void;
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

    class LoggingChannel implements Windows.Foundation.Diagnostics.ILoggingChannel, Windows.Foundation.IClosable, Windows.Foundation.Diagnostics.ILoggingTarget {
        readonly enabled: boolean;
        readonly level: Windows.Foundation.Diagnostics.LoggingLevel;
        readonly name: string;
        readonly id: string;
        constructor(name: string, options: Windows.Foundation.Diagnostics.LoggingChannelOptions);
        constructor(name: string, options: Windows.Foundation.Diagnostics.LoggingChannelOptions, id: string);
        logMessage(eventString: string): void;
        logMessage(eventString: string, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        logValuePair(value1: string, value2: number): void;
        logValuePair(value1: string, value2: number, level: Windows.Foundation.Diagnostics.LoggingLevel): void;
        close(): void;
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
        addEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void
        removeEventListener(type: "loggingenabled", listener: Windows.Foundation.TypedEventHandler<Windows.Foundation.Diagnostics.ILoggingChannel, any>): void
    }

    class LoggingChannelOptions {
        group: string;
        constructor();
        constructor(group: string);
    }

    enum LoggingFieldFormat {
        default,
        hidden,
        string,
        boolean,
        hexadecimal,
        processId,
        threadId,
        port,
        ipv4Address,
        ipv6Address,
        socketAddress,
        xml,
        json,
        win32Error,
        nTStatus,
        hResult,
        fileTime,
        signed,
        unsigned,
    }

    class LoggingFields {
        constructor();
        clear(): void;
        beginStruct(name: string): void;
        beginStruct(name: string, tags: number): void;
        endStruct(): void;
        addEmpty(name: string): void;
        addEmpty(name: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addEmpty(name: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt8(name: string, value: number): void;
        addUInt8(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt8(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt8Array(name: string, value: number[]): void;
        addUInt8Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt8Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addInt16(name: string, value: number): void;
        addInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addInt16Array(name: string, value: number[]): void;
        addInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt16(name: string, value: number): void;
        addUInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt16(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt16Array(name: string, value: number[]): void;
        addUInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt16Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addInt32(name: string, value: number): void;
        addInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addInt32Array(name: string, value: number[]): void;
        addInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt32(name: string, value: number): void;
        addUInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt32(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt32Array(name: string, value: number[]): void;
        addUInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt32Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addInt64(name: string, value: number): void;
        addInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addInt64Array(name: string, value: number[]): void;
        addInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt64(name: string, value: number): void;
        addUInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt64(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addUInt64Array(name: string, value: number[]): void;
        addUInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addUInt64Array(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addSingle(name: string, value: number): void;
        addSingle(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addSingle(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addSingleArray(name: string, value: number[]): void;
        addSingleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addSingleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addDouble(name: string, value: number): void;
        addDouble(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addDouble(name: string, value: number, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addDoubleArray(name: string, value: number[]): void;
        addDoubleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addDoubleArray(name: string, value: number[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addChar16(name: string, value: string): void;
        addChar16(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addChar16(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addChar16Array(name: string, value: string[]): void;
        addChar16Array(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addChar16Array(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addBoolean(name: string, value: boolean): void;
        addBoolean(name: string, value: boolean, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addBoolean(name: string, value: boolean, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addBooleanArray(name: string, value: boolean[]): void;
        addBooleanArray(name: string, value: boolean[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addBooleanArray(name: string, value: boolean[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addString(name: string, value: string): void;
        addString(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addString(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addStringArray(name: string, value: string[]): void;
        addStringArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addStringArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addGuid(name: string, value: string): void;
        addGuid(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addGuid(name: string, value: string, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addGuidArray(name: string, value: string[]): void;
        addGuidArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addGuidArray(name: string, value: string[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addDateTime(name: string, value: Windows.Foundation.DateTime): void;
        addDateTime(name: string, value: Windows.Foundation.DateTime, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addDateTime(name: string, value: Windows.Foundation.DateTime, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addDateTimeArray(name: string, value: Windows.Foundation.DateTime[]): void;
        addDateTimeArray(name: string, value: Windows.Foundation.DateTime[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addDateTimeArray(name: string, value: Windows.Foundation.DateTime[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addTimeSpan(name: string, value: Windows.Foundation.TimeSpan): void;
        addTimeSpan(name: string, value: Windows.Foundation.TimeSpan, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addTimeSpan(name: string, value: Windows.Foundation.TimeSpan, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addTimeSpanArray(name: string, value: Windows.Foundation.TimeSpan[]): void;
        addTimeSpanArray(name: string, value: Windows.Foundation.TimeSpan[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addTimeSpanArray(name: string, value: Windows.Foundation.TimeSpan[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addPoint(name: string, value: Windows.Foundation.Point): void;
        addPoint(name: string, value: Windows.Foundation.Point, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addPoint(name: string, value: Windows.Foundation.Point, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addPointArray(name: string, value: Windows.Foundation.Point[]): void;
        addPointArray(name: string, value: Windows.Foundation.Point[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addPointArray(name: string, value: Windows.Foundation.Point[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addSize(name: string, value: Windows.Foundation.Size): void;
        addSize(name: string, value: Windows.Foundation.Size, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addSize(name: string, value: Windows.Foundation.Size, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addSizeArray(name: string, value: Windows.Foundation.Size[]): void;
        addSizeArray(name: string, value: Windows.Foundation.Size[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addSizeArray(name: string, value: Windows.Foundation.Size[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addRect(name: string, value: Windows.Foundation.Rect): void;
        addRect(name: string, value: Windows.Foundation.Rect, format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addRect(name: string, value: Windows.Foundation.Rect, format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
        addRectArray(name: string, value: Windows.Foundation.Rect[]): void;
        addRectArray(name: string, value: Windows.Foundation.Rect[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat): void;
        addRectArray(name: string, value: Windows.Foundation.Rect[], format: Windows.Foundation.Diagnostics.LoggingFieldFormat, tags: number): void;
    }

    enum LoggingLevel {
        verbose,
        information,
        warning,
        error,
        critical,
    }

    enum LoggingOpcode {
        info,
        start,
        stop,
        reply,
        resume,
        suspend,
        send,
    }

    class LoggingOptions {
        task: number;
        tags: number;
        relatedActivityId: string;
        opcode: Windows.Foundation.Diagnostics.LoggingOpcode;
        keywords: number;
        activityId: string;
        constructor();
        constructor(keywords: number);
    }

    class LoggingSession implements Windows.Foundation.Diagnostics.ILoggingSession, Windows.Foundation.IClosable {
        readonly name: string;
        constructor(name: string);
        saveToFileAsync(folder: any, fileName: string): Windows.Foundation.WinRTPromise<any, void>;
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        addLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel, maxLevel: Windows.Foundation.Diagnostics.LoggingLevel): void;
        removeLoggingChannel(loggingChannel: Windows.Foundation.Diagnostics.ILoggingChannel): void;
        close(): void;
    }

    class RuntimeBrokerErrorSettings implements Windows.Foundation.Diagnostics.IErrorReportingSettings {
        constructor();
        setErrorOptions(value: Windows.Foundation.Diagnostics.ErrorOptions): void;
        getErrorOptions(): Windows.Foundation.Diagnostics.ErrorOptions;
    }

    class TracingStatusChangedEventArgs {
        readonly enabled: boolean;
        readonly traceLevel: Windows.Foundation.Diagnostics.CausalityTraceLevel;
    }

}