//tslint:disable

declare namespace Windows.Foundation.Metadata {
    class ActivatableAttribute {
        constructor(version: number);
        constructor(version: number, type: string);
        constructor(version: number, platform: Windows.Foundation.Metadata.Platform);
        constructor(type: any, version: number);
        constructor(type: any, version: number, contractName: string);
        constructor(type: any, version: number, platform: Windows.Foundation.Metadata.Platform);
    }

    class AllowForWebAttribute {
        constructor();
    }

    class AllowMultipleAttribute {
        constructor();
    }

    class ApiContractAttribute {
        constructor();
    }

    abstract class ApiInformation {
        static isTypePresent(typeName: string): boolean;
        static isMethodPresent(typeName: string, methodName: string): boolean;
        static isMethodPresent(typeName: string, methodName: string, inputParameterCount: number): boolean;
        static isEventPresent(typeName: string, eventName: string): boolean;
        static isPropertyPresent(typeName: string, propertyName: string): boolean;
        static isReadOnlyPropertyPresent(typeName: string, propertyName: string): boolean;
        static isWriteablePropertyPresent(typeName: string, propertyName: string): boolean;
        static isEnumNamedValuePresent(enumTypeName: string, valueName: string): boolean;
        static isApiContractPresent(contractName: string, majorVersion: number): boolean;
        static isApiContractPresent(contractName: string, majorVersion: number, minorVersion: number): boolean;
    }

    enum AttributeTargets {
        all,
        delegate,
        enum,
        event,
        field,
        interface,
        method,
        parameter,
        property,
        runtimeClass,
        struct,
        interfaceImpl,
        apiContract,
    }

    class AttributeUsageAttribute {
        constructor(A_0: Windows.Foundation.Metadata.AttributeTargets);
    }

    class ComposableAttribute {
        constructor(type: any, compositionType: Windows.Foundation.Metadata.CompositionType, version: number);
        constructor(type: any, compositionType: Windows.Foundation.Metadata.CompositionType, version: number, platform: Windows.Foundation.Metadata.Platform);
        constructor(type: any, compositionType: Windows.Foundation.Metadata.CompositionType, version: number, contract: string);
    }

    enum CompositionType {
        protected,
        public,
    }

    class ContractVersionAttribute {
        constructor(version: number);
        constructor(contract: any, version: number);
        constructor(contract: string, version: number);
    }

    class CreateFromStringAttribute {
         methodName: string;
        constructor();
    }

    class DefaultAttribute {
        constructor();
    }

    class DefaultOverloadAttribute {
        constructor();
    }

    class DeprecatedAttribute {
        constructor(message: string, type: Windows.Foundation.Metadata.DeprecationType, version: number);
        constructor(message: string, type: Windows.Foundation.Metadata.DeprecationType, version: number, platform: Windows.Foundation.Metadata.Platform);
        constructor(message: string, type: Windows.Foundation.Metadata.DeprecationType, version: number, contract: string);
    }

    enum DeprecationType {
        deprecate,
        remove,
    }

    class DualApiPartitionAttribute {
         version: number;
        constructor();
    }

    class ExclusiveToAttribute {
        constructor(typeName: any);
    }

    class ExperimentalAttribute {
        constructor();
    }

    class FeatureAttribute {
        constructor(featureStage: Windows.Foundation.Metadata.FeatureStage, validInAllBranches: boolean);
    }

    enum FeatureStage {
        alwaysDisabled,
        disabledByDefault,
        enabledByDefault,
        alwaysEnabled,
    }

    enum GCPressureAmount {
        low,
        medium,
        high,
    }

    class GCPressureAttribute {
         amount: Windows.Foundation.Metadata.GCPressureAmount;
        constructor();
    }

    class GuidAttribute {
        constructor(a: number, b: number, c: number, d: number, e: number, f: number, g: number, h: number, i: number, j: number, k: number);
    }

    class HasVariantAttribute {
        constructor();
    }

    class InternalAttribute {
        constructor();
    }

    class LengthIsAttribute {
        constructor(indexLengthParameter: number);
    }

    class MarshalingBehaviorAttribute {
        constructor(behavior: Windows.Foundation.Metadata.MarshalingType);
    }

    enum MarshalingType {
        none,
        agile,
        standard,
        invalidMarshaling,
    }

    class MetadataMarshalAttribute {
        constructor();
    }

    class MuseAttribute {
         version: number;
        constructor();
    }

    class OverloadAttribute {
        constructor(method: string);
    }

    class OverridableAttribute {
        constructor();
    }

    enum Platform {
        windows,
        windowsPhone,
    }

    class PlatformAttribute {
        constructor(platform: Windows.Foundation.Metadata.Platform);
    }

    class PreviousContractVersionAttribute {
        constructor(contract: string, versionLow: number, versionHigh: number, newContract: string);
        constructor(contract: string, versionLow: number, versionHigh: number);
    }

    class ProtectedAttribute {
        constructor();
    }

    class RangeAttribute {
        constructor(minValue: number, maxValue: number);
    }

    class RemoteAsyncAttribute {
        constructor();
    }

    class StaticAttribute {
        constructor(type: any, version: number);
        constructor(type: any, version: number, platform: Windows.Foundation.Metadata.Platform);
        constructor(type: any, version: number, contractName: string);
    }

    class ThreadingAttribute {
        constructor(model: Windows.Foundation.Metadata.ThreadingModel);
    }

    enum ThreadingModel {
        sTA,
        mTA,
        both,
        invalidThreading,
    }

    class VariantAttribute {
        constructor();
    }

    class VersionAttribute {
        constructor(version: number);
        constructor(version: number, platform: Windows.Foundation.Metadata.Platform);
    }

    class WebHostHiddenAttribute {
        constructor();
    }

}