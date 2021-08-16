//tslint:disable

declare namespace Windows.Foundation.Metadata {
    class ActivatableAttribute {
        public constructor(version: number);
        public constructor(version: number, type: string);
        public constructor(version: number, platform: Windows.Foundation.Metadata.Platform);
        public constructor(type: any, version: number);
        public constructor(type: any, version: number, contractName: string);
        public constructor(type: any, version: number, platform: Windows.Foundation.Metadata.Platform);
    }

    class AllowForWebAttribute {
        public constructor();
    }

    class AllowMultipleAttribute {
        public constructor();
    }

    class ApiContractAttribute {
        public constructor();
    }

    abstract class ApiInformation {
        public static isTypePresent(typeName: string): boolean;
        public static isMethodPresent(typeName: string, methodName: string): boolean;
        public static isMethodPresent(typeName: string, methodName: string, inputParameterCount: number): boolean;
        public static isEventPresent(typeName: string, eventName: string): boolean;
        public static isPropertyPresent(typeName: string, propertyName: string): boolean;
        public static isReadOnlyPropertyPresent(typeName: string, propertyName: string): boolean;
        public static isWriteablePropertyPresent(typeName: string, propertyName: string): boolean;
        public static isEnumNamedValuePresent(enumTypeName: string, valueName: string): boolean;
        public static isApiContractPresent(contractName: string, majorVersion: number): boolean;
        public static isApiContractPresent(contractName: string, majorVersion: number, minorVersion: number): boolean;
    }

    enum AttributeTargets {
        all = 4294967295,
        delegate = 1,
        enum = 2,
        event = 4,
        field = 8,
        interface = 16,
        method = 64,
        parameter = 128,
        property = 256,
        runtimeClass = 512,
        struct = 1024,
        interfaceImpl = 2048,
        apiContract = 8192,
    }

    class AttributeUsageAttribute {
        public constructor(A_0: Windows.Foundation.Metadata.AttributeTargets);
    }

    class ComposableAttribute {
        public constructor(type: any, compositionType: Windows.Foundation.Metadata.CompositionType, version: number);
        public constructor(type: any, compositionType: Windows.Foundation.Metadata.CompositionType, version: number, platform: Windows.Foundation.Metadata.Platform);
        public constructor(type: any, compositionType: Windows.Foundation.Metadata.CompositionType, version: number, contract: string);
    }

    enum CompositionType {
        protected = 1,
        public = 2,
    }

    class ContractVersionAttribute {
        public constructor(version: number);
        public constructor(contract: any, version: number);
        public constructor(contract: string, version: number);
    }

    class DefaultAttribute {
        public constructor();
    }

    class DefaultOverloadAttribute {
        public constructor();
    }

    class DeprecatedAttribute {
        public constructor(message: string, type: Windows.Foundation.Metadata.DeprecationType, version: number);
        public constructor(message: string, type: Windows.Foundation.Metadata.DeprecationType, version: number, platform: Windows.Foundation.Metadata.Platform);
        public constructor(message: string, type: Windows.Foundation.Metadata.DeprecationType, version: number, contract: string);
    }

    enum DeprecationType {
        deprecate = 0,
        remove = 1,
    }

    class DualApiPartitionAttribute {
        public version: number;
        public constructor();
    }

    class ExclusiveToAttribute {
        public constructor(typeName: any);
    }

    class ExperimentalAttribute {
        public constructor();
    }

    class FeatureAttribute {
        public constructor(featureStage: Windows.Foundation.Metadata.FeatureStage, validInAllBranches: boolean);
    }

    enum FeatureStage {
        alwaysDisabled = 0,
        disabledByDefault = 1,
        enabledByDefault = 2,
        alwaysEnabled = 3,
    }

    enum GCPressureAmount {
        low = 0,
        medium = 1,
        high = 2,
    }

    class GCPressureAttribute {
        public amount: Windows.Foundation.Metadata.GCPressureAmount;
        public constructor();
    }

    class GuidAttribute {
        public constructor(a: number, b: number, c: number, d: number, e: number, f: number, g: number, h: number, i: number, j: number, k: number);
    }

    class HasVariantAttribute {
        public constructor();
    }

    class InternalAttribute {
        public constructor();
    }

    class LengthIsAttribute {
        public constructor(indexLengthParameter: number);
    }

    class MarshalingBehaviorAttribute {
        public constructor(behavior: Windows.Foundation.Metadata.MarshalingType);
    }

    enum MarshalingType {
        none = 1,
        agile = 2,
        standard = 3,
        invalidMarshaling = 0,
    }

    class MetadataMarshalAttribute {
        public constructor();
    }

    class MuseAttribute {
        public version: number;
        public constructor();
    }

    class OverloadAttribute {
        public constructor(method: string);
    }

    class OverridableAttribute {
        public constructor();
    }

    enum Platform {
        windows = 0,
        windowsPhone = 1,
    }

    class PlatformAttribute {
        public constructor(platform: Windows.Foundation.Metadata.Platform);
    }

    class PreviousContractVersionAttribute {
        public constructor(contract: string, versionLow: number, versionHigh: number, newContract: string);
        public constructor(contract: string, versionLow: number, versionHigh: number);
    }

    class ProtectedAttribute {
        public constructor();
    }

    class RangeAttribute {
        public constructor(minValue: number, maxValue: number);
    }

    class RemoteAsyncAttribute {
        public constructor();
    }

    class StaticAttribute {
        public constructor(type: any, version: number);
        public constructor(type: any, version: number, platform: Windows.Foundation.Metadata.Platform);
        public constructor(type: any, version: number, contractName: string);
    }

    class ThreadingAttribute {
        public constructor(model: Windows.Foundation.Metadata.ThreadingModel);
    }

    enum ThreadingModel {
        sTA = 1,
        mTA = 2,
        both = 3,
        invalidThreading = 0,
    }

    class VariantAttribute {
        public constructor();
    }

    class VersionAttribute {
        public constructor(version: number);
        public constructor(version: number, platform: Windows.Foundation.Metadata.Platform);
    }

    class WebHostHiddenAttribute {
        public constructor();
    }

}