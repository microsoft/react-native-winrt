//tslint:disable

export namespace TestComponent {
    type BoolArrayDelegate = (values: boolean[]) => { subset: boolean[]; outValue: boolean[]; returnValue: boolean[] };
    
    type BoolDelegate = (value: boolean) => boolean;
    
    type BoolDelegateWithOutParam = (value: boolean) => boolean;
    
    interface BooleanTypes {
        value: boolean;
    }

    type CharArrayDelegate = (values: string[]) => { subset: string[]; outValue: string[]; returnValue: string[] };
    
    type CharDelegate = (value: string) => string;
    
    type CharDelegateWithOutParam = (value: string) => string;
    
    type CompositeStructArrayDelegate = (values: TestComponent.CompositeType[]) => { subset: TestComponent.CompositeType[]; outValue: TestComponent.CompositeType[]; returnValue: TestComponent.CompositeType[] };
    
    type CompositeStructDelegate = (value: TestComponent.CompositeType) => TestComponent.CompositeType;
    
    type CompositeStructDelegateWithOutParam = (value: TestComponent.CompositeType) => TestComponent.CompositeType;
    
    interface CompositeType {
        numerics: TestComponent.NumericTypes;
        strings: TestComponent.StringTypes;
        bools: TestComponent.BooleanTypes;
    }

    type EnumArrayDelegate = (values: TestComponent.TestEnum[]) => { subset: TestComponent.TestEnum[]; outValue: TestComponent.TestEnum[]; returnValue: TestComponent.TestEnum[] };
    
    type EnumDelegate = (value: TestComponent.TestEnum) => TestComponent.TestEnum;
    
    type EnumDelegateWithOutParam = (value: TestComponent.TestEnum) => TestComponent.TestEnum;
    
    type GuidArrayDelegate = (values: string[]) => { subset: string[]; outValue: string[]; returnValue: string[] };
    
    type GuidDelegate = (value: string) => string;
    
    type GuidDelegateWithOutParam = (value: string) => string;
    
    class HierarchyBase {
        public constructor();
        public overriddenHierarchyBaseMethod(): string;
        public nonOverriddenHierarchyBaseMethod(): string;
        public overloadedHierarchyBaseMethod(param1: string): string;
        protected inaccessibleHierarchyBaseMethod(): void;
        protected overriddenHierarchyBaseMethodOverride(): string;
        public iHierarchyV2ContractMethod(): string;
        public static staticHierarchyBaseMethod(): string;
    }

    class HierarchyDerived extends TestComponent.HierarchyBase {
        public constructor(name: string);
        public hierarchyDerivedMethod(): string;
        public overloadedHierarchyBaseMethod(param1: string, param2: string): string;
    }

    interface ITestInterface {
        readonly magicValue: number;
    }

    type InterwovenDelegate = (inBool: boolean, inNumeric: number, inArray: number[]) => { outBool: boolean; outNumeric: number; outArray: number[]; fillArray: number[]; returnValue: number };
    
    type NumericArrayDelegate = (values: number[]) => { subset: number[]; outValue: number[]; returnValue: number[] };
    
    type NumericDelegate = (value: number) => number;
    
    type NumericDelegateWithOutParam = (value: number) => number;
    
    interface NumericTypes {
        u8: number;
        u16: number;
        u32: number;
        u64: number;
        s16: number;
        s32: number;
        s64: number;
        f32: number;
        f64: number;
        enum: TestComponent.TestEnum;
    }

    type ObjectArrayDelegate = (values: TestComponent.TestObject[]) => { subset: TestComponent.TestObject[]; outValue: TestComponent.TestObject[]; returnValue: TestComponent.TestObject[] };
    
    type ObjectDelegate = (value: TestComponent.TestObject) => TestComponent.TestObject;
    
    type ObjectDelegateWithOutParam = (value: TestComponent.TestObject) => TestComponent.TestObject;
    
    type RefArrayDelegate = (values: number[] | null) => { subset: number[] | null; outValue: number[] | null; returnValue: number[] | null };
    
    type RefDelegate = (value: number | null) => number | null;
    
    type RefDelegateWithOutParam = (value: number | null) => number | null;
    
    abstract class StaticOnlyTest {
        public static u8Property: number;
        public static u64Property: number;
        public static u32Property: number;
        public static u16Property: number;
        public static stringsStructProperty: TestComponent.StringTypes;
        public static stringProperty: string;
        public static s64Property: number;
        public static s32Property: number;
        public static s16Property: number;
        public static refNumericProperty: number | null;
        public static refEnumProperty: TestComponent.TestEnum | null;
        public static refCharProperty: string | null;
        public static refBooleanProperty: boolean | null;
        public static objectProperty: TestComponent.TestObject;
        public static numericsStructProperty: TestComponent.NumericTypes;
        public static hResultProperty: number;
        public static guidProperty: string;
        public static f64Property: number;
        public static f32Property: number;
        public static enumProperty: TestComponent.TestEnum;
        public static compositeStructProperty: TestComponent.CompositeType;
        public static charProperty: string;
        public static booleansStructProperty: TestComponent.BooleanTypes;
        public static boolProperty: boolean;
        public static copyString(value: string): string;
        public static raiseObjectEvent(value: TestComponent.TestObject): void;
        public static addEventListener(type: "objecteventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestObject>): void;
        public static removeEventListener(type: "objecteventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestObject>): void;
    }

    type StringArrayDelegate = (values: string[]) => { subset: string[]; outValue: string[]; returnValue: string[] };
    
    type StringDelegate = (value: string) => string;
    
    type StringDelegateWithOutParam = (value: string) => string;
    
    interface StringTypes {
        char: string;
        string: string;
        guid: string;
    }

    class Test {
        public numericsStructProperty: TestComponent.NumericTypes;
        public numericArrayProperty: number[];
        public hResultProperty: number;
        public guidProperty: string;
        public guidArrayProperty: string[];
        public f64Property: number;
        public f32Property: number;
        public enumProperty: TestComponent.TestEnum;
        public enumArrayProperty: TestComponent.TestEnum[];
        public dateTimeProperty: Date;
        public objectArrayProperty: TestComponent.TestObject[];
        public compositeStructProperty: TestComponent.CompositeType;
        public compositeStructArrayProperty: TestComponent.CompositeType[];
        public charProperty: string;
        public charArrayProperty: string[];
        public booleansStructProperty: TestComponent.BooleanTypes;
        public booleanArrayProperty: boolean[];
        public u8Property: number;
        public objectProperty: TestComponent.TestObject;
        public boolProperty: boolean;
        public u64Property: number;
        public u32Property: number;
        public u16Property: number;
        public timeSpanProperty: number;
        public stringsStructProperty: TestComponent.StringTypes;
        public stringProperty: string;
        public stringArrayProperty: string[];
        public s64Property: number;
        public s32Property: number;
        public s16Property: number;
        public refNumericProperty: number | null;
        public refEnumProperty: TestComponent.TestEnum | null;
        public refCharProperty: string | null;
        public refBooleanProperty: boolean | null;
        public refArrayProperty: number[] | null;
        public propertyValue: Windows.Foundation.IPropertyValue;
        public readonly constructorParamCount: number;
        public readonly propertyValueCppType: string;
        public static staticU8Property: number;
        public static staticU64Property: number;
        public static staticU32Property: number;
        public static staticU16Property: number;
        public static staticStringsStructProperty: TestComponent.StringTypes;
        public static staticStringProperty: string;
        public static staticStringArrayProperty: string[];
        public static staticS64Property: number;
        public static staticS32Property: number;
        public static staticS16Property: number;
        public static staticRefNumericProperty: number | null;
        public static staticRefEnumProperty: TestComponent.TestEnum | null;
        public static staticRefCharProperty: string | null;
        public static staticRefBooleanProperty: boolean | null;
        public static staticRefArrayProperty: number[] | null;
        public static staticObjectProperty: TestComponent.TestObject;
        public static staticObjectArrayProperty: TestComponent.TestObject[];
        public static staticNumericsStructProperty: TestComponent.NumericTypes;
        public static staticNumericArrayProperty: number[];
        public static staticGuidProperty: string;
        public static staticGuidArrayProperty: string[];
        public static staticF64Property: number;
        public static staticF32Property: number;
        public static staticEnumProperty: TestComponent.TestEnum;
        public static staticEnumArrayProperty: TestComponent.TestEnum[];
        public static staticCompositeStructProperty: TestComponent.CompositeType;
        public static staticCompositeStructArrayProperty: TestComponent.CompositeType[];
        public static staticCharProperty: string;
        public static staticCharArrayProperty: string[];
        public static staticBooleansStructProperty: TestComponent.BooleanTypes;
        public static staticBooleanArrayProperty: boolean[];
        public static staticBoolProperty: boolean;
        public constructor(val: number);
        public constructor(val: number, str: string);
        public constructor();
        public dateTimePropertyCppValue(): string;
        public timeSpanPropertyCppValue(): string;
        public assignPropertyValueAsType(value: Windows.Foundation.IPropertyValue, winrtPropertyValueType: string): void;
        public or(lhs: boolean, rhs: boolean): boolean;
        public orAll(values: boolean[]): boolean;
        public add(lhs: number, rhs: number): number;
        public addAll(values: number[]): number;
        public append(a: string, b: string, c: string): string;
        public appendAll(values: string[]): string;
        public arityOverload(): string;
        public arityOverload(str: string): string;
        public arityOverload(first: string, second: string): string;
        public defaultOverload(str: string, val: boolean): string;
        public defaultOverload(str: string, repeat: number): string;
        public defaultOverload(str: string, val: number): string;
        public outParamOverload(str: string): string;
        public outParamOverload(str: string): { outParam: string; returnValue: string };
        public outParamOverload(str: string, other: string): string;
        public contractArityOverload(): string;
        public contractDefaultOverloadV1(str: string, val: boolean): string;
        public contractDefaultOverloadV1(str: string, repeat: number): string;
        public contractDefaultOverloadV1(str: string, val: number): string;
        public contractDefaultOverloadV2(str: string, val: number): string;
        public contractOutParamOverloadV1(str: string): string;
        public contractOutParamOverloadV1(str: string): { outParam: string; returnValue: string };
        public contractOutParamOverloadV1(str: string, other: string): string;
        public contractOutParamOverloadV2(ch: string): string;
        public boolOutParam(lhs: boolean, rhs: boolean): { andResult: boolean; orResult: boolean; returnValue: boolean };
        public charOutParam(value: string): { next: string; prev: string; returnValue: string };
        public numericOutParam(value: number): { doubledValue: number; tripledValue: number; returnValue: number };
        public stringOutParam(value: string): { lower: string; upper: string; returnValue: string };
        public guidOutParam(value: string): { zero: string; allSet: string; returnValue: string };
        public enumOutParam(value: TestComponent.TestEnum): { plusOne: TestComponent.TestEnum; plusTwo: TestComponent.TestEnum; returnValue: TestComponent.TestEnum };
        public compositeStructOutParam(input: TestComponent.CompositeType): { first: TestComponent.CompositeType; second: TestComponent.CompositeType; returnValue: TestComponent.CompositeType };
        public refOutParam(value: number | null): { doubledValue: number | null; tripledValue: number | null; returnValue: number | null };
        public objectOutParam(value: TestComponent.TestObject): { doubledValue: TestComponent.TestObject; tripledValue: TestComponent.TestObject; returnValue: TestComponent.TestObject };
        public boolArrayOutParam(values: boolean[]): { rot1: boolean[]; rot2: boolean[]; returnValue: boolean[] };
        public charArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        public numericArrayOutParam(values: number[]): { rot1: number[]; rot2: number[]; returnValue: number[] };
        public stringArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        public guidArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        public enumArrayOutParam(values: TestComponent.TestEnum[]): { rot1: TestComponent.TestEnum[]; rot2: TestComponent.TestEnum[]; returnValue: TestComponent.TestEnum[] };
        public compositeStructArrayOutParam(values: TestComponent.CompositeType[]): { rot1: TestComponent.CompositeType[]; rot2: TestComponent.CompositeType[]; returnValue: TestComponent.CompositeType[] };
        public refArrayOutParam(values: number[] | null): { rot1: number[] | null; rot2: number[] | null; returnValue: number[] | null };
        public objectArrayOutParam(values: TestComponent.TestObject[]): { rot1: TestComponent.TestObject[]; rot2: TestComponent.TestObject[]; returnValue: TestComponent.TestObject[] };
        public boolFillParam(): boolean[];
        public charFillParam(): string[];
        public numericFillParam(): number[];
        public stringFillParam(): string[];
        public guidFillParam(): string[];
        public enumFillParam(): TestComponent.TestEnum[];
        public compositeStructFillParam(): TestComponent.CompositeType[];
        public refFillParam(): number[] | null;
        public objectFillParam(): TestComponent.TestObject[];
        public interwovenParams(inBool: boolean, inNumeric: number, inArray: number[]): { outBool: boolean; outNumeric: number; outArray: number[]; refArray: number[]; returnValue: number };
        public raiseBoolEvent(value: boolean): void;
        public raiseCharEvent(value: string): void;
        public raiseNumericEvent(value: number): void;
        public raiseStringEvent(value: string): void;
        public raiseGuidEvent(value: string): void;
        public raiseEnumEvent(value: TestComponent.TestEnum): void;
        public raiseCompositeStructEvent(value: TestComponent.CompositeType): void;
        public raiseRefEvent(value: number | null): void;
        public raiseObjectEvent(value: TestComponent.TestObject): void;
        public contractArityOverload(str: string): string;
        private testComponent.ITest2.ContractDefaultOverloadV1(str: string, val: number): string;
        public contractDefaultOverloadV2(str: string, val: boolean): string;
        public contractDefaultOverloadV2(str: string, val: number): string;
        public contractDefaultOverloadV2(str: string, val: number): string;
        private testComponent.ITest2.ContractOutParamOverloadV1(ch: string): string;
        public contractOutParamOverloadV2(str: string): string;
        public contractOutParamOverloadV2(str: string): { outParam: string; returnValue: string };
        public contractOutParamOverloadV2(str: string, other: string): string;
        public static staticContractArityOverload(str: string): string;
        public static staticContractDefaultOverloadV1(str: string, val: number): string;
        public static staticContractDefaultOverloadV2(str: string, val: boolean): string;
        public static staticContractDefaultOverloadV2(str: string, val: number): string;
        public static staticContractDefaultOverloadV2(str: string, val: number): string;
        public static staticContractOutParamOverloadV1(ch: string): string;
        public static staticContractOutParamOverloadV2(str: string): string;
        public static staticContractOutParamOverloadV2(str: string): { outParam: string; returnValue: string };
        public static staticContractOutParamOverloadV2(str: string, other: string): string;
        public static logFailures(failures: string): void;
        public static staticOr(lhs: boolean, rhs: boolean): boolean;
        public static staticOrAll(values: boolean[]): boolean;
        public static staticAdd(lhs: number, rhs: number): number;
        public static staticAddAll(values: number[]): number;
        public static staticAppend(a: string, b: string, c: string): string;
        public static staticAppendAll(values: string[]): string;
        public static staticArityOverload(): string;
        public static staticArityOverload(str: string): string;
        public static staticArityOverload(first: string, second: string): string;
        public static staticDefaultOverload(str: string, val: boolean): string;
        public static staticDefaultOverload(str: string, repeat: number): string;
        public static staticDefaultOverload(str: string, val: number): string;
        public static staticOutParamOverload(str: string): string;
        public static staticOutParamOverload(str: string): { outParam: string; returnValue: string };
        public static staticOutParamOverload(str: string, other: string): string;
        public static staticContractArityOverload(): string;
        public static staticContractDefaultOverloadV1(str: string, val: boolean): string;
        public static staticContractDefaultOverloadV1(str: string, repeat: number): string;
        public static staticContractDefaultOverloadV1(str: string, val: number): string;
        public static staticContractDefaultOverloadV2(str: string, val: number): string;
        public static staticContractOutParamOverloadV1(str: string): string;
        public static staticContractOutParamOverloadV1(str: string): { outParam: string; returnValue: string };
        public static staticContractOutParamOverloadV1(str: string, other: string): string;
        public static staticContractOutParamOverloadV2(ch: string): string;
        public static staticBoolOutParam(lhs: boolean, rhs: boolean): { andResult: boolean; orResult: boolean; returnValue: boolean };
        public static staticCharOutParam(value: string): { next: string; prev: string; returnValue: string };
        public static staticNumericOutParam(value: number): { doubledValue: number; tripledValue: number; returnValue: number };
        public static staticStringOutParam(value: string): { lower: string; upper: string; returnValue: string };
        public static staticGuidOutParam(value: string): { zero: string; allSet: string; returnValue: string };
        public static staticEnumOutParam(value: TestComponent.TestEnum): { plusOne: TestComponent.TestEnum; plusTwo: TestComponent.TestEnum; returnValue: TestComponent.TestEnum };
        public static staticCompositeStructOutParam(input: TestComponent.CompositeType): { first: TestComponent.CompositeType; second: TestComponent.CompositeType; returnValue: TestComponent.CompositeType };
        public static staticRefOutParam(value: number | null): { doubledValue: number | null; tripledValue: number | null; returnValue: number | null };
        public static staticObjectOutParam(value: TestComponent.TestObject): { doubledValue: TestComponent.TestObject; tripledValue: TestComponent.TestObject; returnValue: TestComponent.TestObject };
        public static staticBoolArrayOutParam(values: boolean[]): { rot1: boolean[]; rot2: boolean[]; returnValue: boolean[] };
        public static staticCharArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        public static staticNumericArrayOutParam(values: number[]): { rot1: number[]; rot2: number[]; returnValue: number[] };
        public static staticStringArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        public static staticGuidArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        public static staticEnumArrayOutParam(values: TestComponent.TestEnum[]): { rot1: TestComponent.TestEnum[]; rot2: TestComponent.TestEnum[]; returnValue: TestComponent.TestEnum[] };
        public static staticCompositeStructArrayOutParam(values: TestComponent.CompositeType[]): { rot1: TestComponent.CompositeType[]; rot2: TestComponent.CompositeType[]; returnValue: TestComponent.CompositeType[] };
        public static staticRefArrayOutParam(values: number[] | null): { rot1: number[] | null; rot2: number[] | null; returnValue: number[] | null };
        public static staticObjectArrayOutParam(values: TestComponent.TestObject[]): { rot1: TestComponent.TestObject[]; rot2: TestComponent.TestObject[]; returnValue: TestComponent.TestObject[] };
        public static staticBoolFillParam(): boolean[];
        public static staticCharFillParam(): string[];
        public static staticNumericFillParam(): number[];
        public static staticStringFillParam(): string[];
        public static staticGuidFillParam(): string[];
        public static staticEnumFillParam(): TestComponent.TestEnum[];
        public static staticCompositeStructFillParam(): TestComponent.CompositeType[];
        public static staticRefFillParam(): number[] | null;
        public static staticObjectFillParam(): TestComponent.TestObject[];
        public static staticInterwovenParams(inBool: boolean, inNumeric: number, inArray: number[]): { outBool: boolean; outNumeric: number; outArray: number[]; refArray: number[]; returnValue: number };
        public static raiseStaticBoolEvent(value: boolean): void;
        public static raiseStaticCharEvent(value: string): void;
        public static raiseStaticNumericEvent(value: number): void;
        public static raiseStaticStringEvent(value: string): void;
        public static raiseStaticGuidEvent(value: string): void;
        public static raiseStaticEnumEvent(value: TestComponent.TestEnum): void;
        public static raiseStaticCompositeStructEvent(value: TestComponent.CompositeType): void;
        public static raiseStaticRefEvent(value: number | null): void;
        public static raiseStaticObjectEvent(value: TestComponent.TestObject): void;
        public static staticInvokeBoolDelegate(value: boolean, targetFn: TestComponent.BoolDelegate): boolean;
        public static staticInvokeCharDelegate(value: string, targetFn: TestComponent.CharDelegate): string;
        public static staticInvokeNumericDelegate(value: number, targetFn: TestComponent.NumericDelegate): number;
        public static staticInvokeStringDelegate(value: string, targetFn: TestComponent.StringDelegate): string;
        public static staticInvokeGuidDelegate(value: string, targetFn: TestComponent.GuidDelegate): string;
        public static staticInvokeEnumDelegate(value: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegate): TestComponent.TestEnum;
        public static staticInvokeCompositeStructDelegate(value: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegate): TestComponent.CompositeType;
        public static staticInvokeRefDelegate(value: number | null, targetFn: TestComponent.RefDelegate): number | null;
        public static staticInvokeObjectDelegate(value: TestComponent.TestObject, targetFn: TestComponent.ObjectDelegate): TestComponent.TestObject;
        public static staticInvokeBoolDelegateWithOutParam(value: boolean, targetFn: TestComponent.BoolDelegateWithOutParam): boolean;
        public static staticInvokeCharDelegateWithOutParam(value: string, targetFn: TestComponent.CharDelegateWithOutParam): string;
        public static staticInvokeNumericDelegateWithOutParam(value: number, targetFn: TestComponent.NumericDelegateWithOutParam): number;
        public static staticInvokeStringDelegateWithOutParam(value: string, targetFn: TestComponent.StringDelegateWithOutParam): string;
        public static staticInvokeGuidDelegateWithOutParam(value: string, targetFn: TestComponent.GuidDelegateWithOutParam): string;
        public static staticInvokeEnumDelegateWithOutParam(value: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegateWithOutParam): TestComponent.TestEnum;
        public static staticInvokeCompositeStructDelegateWithOutParam(value: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegateWithOutParam): TestComponent.CompositeType;
        public static staticInvokeRefDelegateWithOutParam(value: number | null, targetFn: TestComponent.RefDelegateWithOutParam): number | null;
        public static staticInvokeObjectDelegateWithOutParam(value: TestComponent.TestObject, targetFn: TestComponent.ObjectDelegateWithOutParam): TestComponent.TestObject;
        public static staticInvokeBoolArrayDelegate(values: boolean[], targetFn: TestComponent.BoolArrayDelegate): boolean;
        public static staticInvokeCharArrayDelegate(values: string[], targetFn: TestComponent.CharArrayDelegate): boolean;
        public static staticInvokeNumericArrayDelegate(values: number[], targetFn: TestComponent.NumericArrayDelegate): boolean;
        public static staticInvokeStringArrayDelegate(values: string[], targetFn: TestComponent.StringArrayDelegate): boolean;
        public static staticInvokeGuidArrayDelegate(values: string[], targetFn: TestComponent.GuidArrayDelegate): boolean;
        public static staticInvokeEnumArrayDelegate(values: TestComponent.TestEnum[], targetFn: TestComponent.EnumArrayDelegate): boolean;
        public static staticInvokeCompositeStructArrayDelegate(values: TestComponent.CompositeType[], targetFn: TestComponent.CompositeStructArrayDelegate): boolean;
        public static staticInvokeRefArrayDelegate(values: number[] | null, targetFn: TestComponent.RefArrayDelegate): boolean;
        public static staticInvokeObjectArrayDelegate(values: TestComponent.TestObject[], targetFn: TestComponent.ObjectArrayDelegate): boolean;
        public static staticInvokeInterwovenDelegate(inBool: boolean, inNumeric: number, inArray: number[], targetFn: TestComponent.InterwovenDelegate): boolean;
        public static copyBoolsToVector(values: boolean[]): Windows.Foundation.Collections.IVector<boolean>;
        public static copyCharsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        public static copyNumericsToVector(values: number[]): Windows.Foundation.Collections.IVector<number>;
        public static copyStringsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        public static copyGuidsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        public static copyEnumValuesToVector(values: TestComponent.TestEnum[]): Windows.Foundation.Collections.IVector<TestComponent.TestEnum>;
        public static copyCompositeStructsToVector(values: TestComponent.CompositeType[]): Windows.Foundation.Collections.IVector<TestComponent.CompositeType>;
        public static copyRefsToVector(values: number[] | null): Windows.Foundation.Collections.IVector<number | null>;
        public static copyObjectsToVector(values: TestComponent.TestObject[]): Windows.Foundation.Collections.IVector<TestComponent.TestObject>;
        public static copyBoolsToVectorView(values: boolean[]): Windows.Foundation.Collections.IVectorView<boolean>;
        public static copyCharsToVectorView(values: string[]): Windows.Foundation.Collections.IVectorView<string>;
        public static copyNumericsToVectorView(values: number[]): Windows.Foundation.Collections.IVectorView<number>;
        public static copyStringsToVectorView(values: string[]): Windows.Foundation.Collections.IVectorView<string>;
        public static copyGuidsToVectorView(values: string[]): Windows.Foundation.Collections.IVectorView<string>;
        public static copyEnumValuesToVectorView(values: TestComponent.TestEnum[]): Windows.Foundation.Collections.IVectorView<TestComponent.TestEnum>;
        public static copyCompositeStructsToVectorView(values: TestComponent.CompositeType[]): Windows.Foundation.Collections.IVectorView<TestComponent.CompositeType>;
        public static copyRefsToVectorView(values: number[] | null): Windows.Foundation.Collections.IVectorView<number | null>;
        public static copyObjectsToVectorView(values: any): Windows.Foundation.Collections.IVectorView<any>;
        public static makeObservableVector(): Windows.Foundation.Collections.IObservableVector<number>;
        public static makeObservableMap(): Windows.Foundation.Collections.IObservableMap<string, number>;
        public static returnSameBoolVector(vector: Windows.Foundation.Collections.IVector<boolean>): Windows.Foundation.Collections.IVector<boolean>;
        public static returnSameCharVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        public static returnSameNumericVector(vector: Windows.Foundation.Collections.IVector<number>): Windows.Foundation.Collections.IVector<number>;
        public static returnSameStringVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        public static returnSameGuidVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        public static returnSameEnumVector(vector: Windows.Foundation.Collections.IVector<TestComponent.TestEnum>): Windows.Foundation.Collections.IVector<TestComponent.TestEnum>;
        public static returnSameCompositeStructVector(vector: Windows.Foundation.Collections.IVector<TestComponent.CompositeType>): Windows.Foundation.Collections.IVector<TestComponent.CompositeType>;
        public static returnSameRefVector(vector: Windows.Foundation.Collections.IVector<number | null>): Windows.Foundation.Collections.IVector<number | null>;
        public static returnSameObjectVector(vector: Windows.Foundation.Collections.IVector<TestComponent.TestObject>): Windows.Foundation.Collections.IVector<TestComponent.TestObject>;
        public static returnSameBoolVectorView(vector: Windows.Foundation.Collections.IVectorView<boolean>): Windows.Foundation.Collections.IVectorView<boolean>;
        public static returnSameCharVectorView(vector: Windows.Foundation.Collections.IVectorView<string>): Windows.Foundation.Collections.IVectorView<string>;
        public static returnSameNumericVectorView(vector: Windows.Foundation.Collections.IVectorView<number>): Windows.Foundation.Collections.IVectorView<number>;
        public static returnSameStringVectorView(vector: Windows.Foundation.Collections.IVectorView<string>): Windows.Foundation.Collections.IVectorView<string>;
        public static returnSameGuidVectorView(vector: Windows.Foundation.Collections.IVectorView<string>): Windows.Foundation.Collections.IVectorView<string>;
        public static returnSameEnumVectorView(vector: Windows.Foundation.Collections.IVectorView<TestComponent.TestEnum>): Windows.Foundation.Collections.IVectorView<TestComponent.TestEnum>;
        public static returnSameCompositeStructVectorView(vector: Windows.Foundation.Collections.IVectorView<TestComponent.CompositeType>): Windows.Foundation.Collections.IVectorView<TestComponent.CompositeType>;
        public static returnSameRefVectorView(vector: Windows.Foundation.Collections.IVectorView<number | null>): Windows.Foundation.Collections.IVectorView<number | null>;
        public static returnSameObjectVectorView(vector: Windows.Foundation.Collections.IVectorView<TestComponent.TestObject>): Windows.Foundation.Collections.IVectorView<TestComponent.TestObject>;
        public static returnSameBoolIterable(iterable: Windows.Foundation.Collections.IIterable<boolean>): Windows.Foundation.Collections.IIterable<boolean>;
        public static returnSameCharIterable(iterable: Windows.Foundation.Collections.IIterable<string>): Windows.Foundation.Collections.IIterable<string>;
        public static returnSameNumericIterable(iterable: Windows.Foundation.Collections.IIterable<number>): Windows.Foundation.Collections.IIterable<number>;
        public static returnSameStringIterable(iterable: Windows.Foundation.Collections.IIterable<string>): Windows.Foundation.Collections.IIterable<string>;
        public static returnSameGuidIterable(iterable: Windows.Foundation.Collections.IIterable<string>): Windows.Foundation.Collections.IIterable<string>;
        public static returnSameEnumIterable(iterable: Windows.Foundation.Collections.IIterable<TestComponent.TestEnum>): Windows.Foundation.Collections.IIterable<TestComponent.TestEnum>;
        public static returnSameCompositeStructIterable(iterable: Windows.Foundation.Collections.IIterable<TestComponent.CompositeType>): Windows.Foundation.Collections.IIterable<TestComponent.CompositeType>;
        public static returnSameRefIterable(iterable: Windows.Foundation.Collections.IIterable<number | null>): Windows.Foundation.Collections.IIterable<number | null>;
        public static returnSameObjectIterable(iterable: Windows.Foundation.Collections.IIterable<TestComponent.TestObject>): Windows.Foundation.Collections.IIterable<TestComponent.TestObject>;
        public static createStringToNumberMap(): Windows.Foundation.Collections.IMap<string, number>;
        public static copyToMapView(stringToNumberMap: Windows.Foundation.Collections.IMap<string, number>): Windows.Foundation.Collections.IMapView<string, number>;
        public static pauseAsync(milliseconds: number): Windows.Foundation.WinRTPromise<void, void>;
        public static countToNumberAsync(value: number): Windows.Foundation.WinRTPromise<void, number>;
        public static addAsync(lhs: number, rhs: number): Windows.Foundation.WinRTPromise<number, void>;
        public static countDoubleAsync(value: number): Windows.Foundation.WinRTPromise<number, number>;
        public static throwAsyncException(): Windows.Foundation.WinRTPromise<void, void>;
        public static immediateReturnAsync(value: number): Windows.Foundation.WinRTPromise<number, void>;
        public static getObjectsAsync(): Windows.Foundation.WinRTPromise<Windows.Foundation.Collections.IVectorView<TestComponent.ITestInterface>, void>;
        public addEventListener(type: "booleventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, boolean>): void;
        public removeEventListener(type: "booleventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, boolean>): void;
        public addEventListener(type: "chareventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, string>): void;
        public removeEventListener(type: "chareventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, string>): void;
        public addEventListener(type: "numericeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, number>): void;
        public removeEventListener(type: "numericeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, number>): void;
        public addEventListener(type: "stringeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, string>): void;
        public removeEventListener(type: "stringeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, string>): void;
        public addEventListener(type: "guideventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, string>): void;
        public removeEventListener(type: "guideventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, string>): void;
        public addEventListener(type: "enumeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, TestComponent.TestEnum>): void;
        public removeEventListener(type: "enumeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, TestComponent.TestEnum>): void;
        public addEventListener(type: "compositestructeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, TestComponent.CompositeType>): void;
        public removeEventListener(type: "compositestructeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, TestComponent.CompositeType>): void;
        public addEventListener(type: "refeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, number | null>): void;
        public removeEventListener(type: "refeventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, number | null>): void;
        public addEventListener(type: "objecteventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, TestComponent.TestObject>): void;
        public removeEventListener(type: "objecteventhandler", listener: Windows.Foundation.TypedEventHandler<TestComponent.Test, TestComponent.TestObject>): void;
        public static addEventListener(type: "staticbooleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void;
        public static removeEventListener(type: "staticbooleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void;
        public static addEventListener(type: "staticchareventhandler", listener: Windows.Foundation.EventHandler<string>): void;
        public static removeEventListener(type: "staticchareventhandler", listener: Windows.Foundation.EventHandler<string>): void;
        public static addEventListener(type: "staticnumericeventhandler", listener: Windows.Foundation.EventHandler<number>): void;
        public static removeEventListener(type: "staticnumericeventhandler", listener: Windows.Foundation.EventHandler<number>): void;
        public static addEventListener(type: "staticstringeventhandler", listener: Windows.Foundation.EventHandler<string>): void;
        public static removeEventListener(type: "staticstringeventhandler", listener: Windows.Foundation.EventHandler<string>): void;
        public static addEventListener(type: "staticguideventhandler", listener: Windows.Foundation.EventHandler<string>): void;
        public static removeEventListener(type: "staticguideventhandler", listener: Windows.Foundation.EventHandler<string>): void;
        public static addEventListener(type: "staticenumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void;
        public static removeEventListener(type: "staticenumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void;
        public static addEventListener(type: "staticcompositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void;
        public static removeEventListener(type: "staticcompositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void;
        public static addEventListener(type: "staticrefeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void;
        public static removeEventListener(type: "staticrefeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void;
        public static addEventListener(type: "staticobjecteventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestObject>): void;
        public static removeEventListener(type: "staticobjecteventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestObject>): void;
    }

    interface TestContract {
    }

    enum TestEnum {
        first = 1,
        second = 2,
        third = 3,
        fourth = 4,
    }

    class TestObject {
        public readonly value: number;
        public constructor(val: number);
    }

}

export namespace Windows {
    export namespace Foundation {
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

        export namespace Collections {
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
        export namespace Diagnostics {
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
        export namespace Metadata {
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
        export namespace Numerics {
            interface Matrix3x2 {
                m11: number;
                m12: number;
                m21: number;
                m22: number;
                m31: number;
                m32: number;
            }

            interface Matrix4x4 {
                m11: number;
                m12: number;
                m13: number;
                m14: number;
                m21: number;
                m22: number;
                m23: number;
                m24: number;
                m31: number;
                m32: number;
                m33: number;
                m34: number;
                m41: number;
                m42: number;
                m43: number;
                m44: number;
            }

            interface Plane {
                normal: Windows.Foundation.Numerics.Vector3;
                d: number;
            }

            interface Quaternion {
                x: number;
                y: number;
                z: number;
                w: number;
            }

            interface Rational {
                numerator: number;
                denominator: number;
            }

            interface Vector2 {
                x: number;
                y: number;
            }

            interface Vector3 {
                x: number;
                y: number;
                z: number;
            }

            interface Vector4 {
                x: number;
                y: number;
                z: number;
                w: number;
            }

        }
    }
    export namespace Web {
        export namespace Http {
            enum HttpStatusCode {
                none = 0,
                continue = 100,
                switchingProtocols = 101,
                processing = 102,
                ok = 200,
                created = 201,
                accepted = 202,
                nonAuthoritativeInformation = 203,
                noContent = 204,
                resetContent = 205,
                partialContent = 206,
                multiStatus = 207,
                alreadyReported = 208,
                iMUsed = 226,
                multipleChoices = 300,
                movedPermanently = 301,
                found = 302,
                seeOther = 303,
                notModified = 304,
                useProxy = 305,
                temporaryRedirect = 307,
                permanentRedirect = 308,
                badRequest = 400,
                unauthorized = 401,
                paymentRequired = 402,
                forbidden = 403,
                notFound = 404,
                methodNotAllowed = 405,
                notAcceptable = 406,
                proxyAuthenticationRequired = 407,
                requestTimeout = 408,
                conflict = 409,
                gone = 410,
                lengthRequired = 411,
                preconditionFailed = 412,
                requestEntityTooLarge = 413,
                requestUriTooLong = 414,
                unsupportedMediaType = 415,
                requestedRangeNotSatisfiable = 416,
                expectationFailed = 417,
                unprocessableEntity = 422,
                locked = 423,
                failedDependency = 424,
                upgradeRequired = 426,
                preconditionRequired = 428,
                tooManyRequests = 429,
                requestHeaderFieldsTooLarge = 431,
                internalServerError = 500,
                notImplemented = 501,
                badGateway = 502,
                serviceUnavailable = 503,
                gatewayTimeout = 504,
                httpVersionNotSupported = 505,
                variantAlsoNegotiates = 506,
                insufficientStorage = 507,
                loopDetected = 508,
                notExtended = 510,
                networkAuthenticationRequired = 511,
            }

        }
    }
}