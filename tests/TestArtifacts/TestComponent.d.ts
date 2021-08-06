//tslint:disable

declare namespace TestComponent {
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
        first,
        second,
        third,
        fourth,
    }

    class TestObject {
        public readonly value: number;
        public constructor(val: number);
    }

}