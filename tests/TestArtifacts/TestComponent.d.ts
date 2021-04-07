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
        constructor();
        overriddenHierarchyBaseMethod(): string;
        nonOverriddenHierarchyBaseMethod(): string;
        overloadedHierarchyBaseMethod(param1: string): string;
        protected overriddenHierarchyBaseMethodOverride(): string;
        iHierarchyV2ContractMethod(): string;
        static staticHierarchyBaseMethod(): string;
    }

    class HierarchyDerived extends TestComponent.HierarchyBase {
        constructor(name: string);
        hierarchyDerivedMethod(): string;
        overloadedHierarchyBaseMethod(param1: string, param2: string): string;
    }

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

    type ObjectArrayDelegate = (values: Windows.Foundation.Collections.IVector<number>[]) => { subset: Windows.Foundation.Collections.IVector<number>[]; outValue: Windows.Foundation.Collections.IVector<number>[]; returnValue: Windows.Foundation.Collections.IVector<number>[] };
    
    type ObjectDelegate = (value: Windows.Foundation.Collections.IVector<number>) => Windows.Foundation.Collections.IVector<number>;
    
    type ObjectDelegateWithOutParam = (value: Windows.Foundation.Collections.IVector<number>) => Windows.Foundation.Collections.IVector<number>;
    
    type RefArrayDelegate = (values: number[] | null) => { subset: number[] | null; outValue: number[] | null; returnValue: number[] | null };
    
    type RefDelegate = (value: number | null) => number | null;
    
    type RefDelegateWithOutParam = (value: number | null) => number | null;
    
    type StringArrayDelegate = (values: string[]) => { subset: string[]; outValue: string[]; returnValue: string[] };
    
    type StringDelegate = (value: string) => string;
    
    type StringDelegateWithOutParam = (value: string) => string;
    
    interface StringTypes {
         char: string;
         string: string;
         guid: string;
    }

    class Test {
        objectArrayProperty: Windows.Foundation.Collections.IVector<number>[];
        numericsStructProperty: TestComponent.NumericTypes;
        numericArrayProperty: number[];
        hResultProperty: number;
        guidProperty: string;
        guidArrayProperty: string[];
        f64Property: number;
        f32Property: number;
        enumProperty: TestComponent.TestEnum;
        enumArrayProperty: TestComponent.TestEnum[];
        dateTimeProperty: Windows.Foundation.DateTime;
        compositeStructProperty: TestComponent.CompositeType;
        compositeStructArrayProperty: TestComponent.CompositeType[];
        charProperty: string;
        charArrayProperty: string[];
        booleansStructProperty: TestComponent.BooleanTypes;
        booleanArrayProperty: boolean[];
        u8Property: number;
        objectProperty: Windows.Foundation.Collections.IVector<number>;
        boolProperty: boolean;
        u64Property: number;
        u32Property: number;
        u16Property: number;
        timeSpanProperty: Windows.Foundation.TimeSpan;
        stringsStructProperty: TestComponent.StringTypes;
        stringProperty: string;
        stringArrayProperty: string[];
        s64Property: number;
        s32Property: number;
        s16Property: number;
        refNumericProperty: number | null;
        refEnumProperty: TestComponent.TestEnum | null;
        refCharProperty: string | null;
        refBooleanProperty: boolean | null;
        refArrayProperty: number[] | null;
        propertyValue: Windows.Foundation.IPropertyValue;
        static staticBoolProperty: boolean;
        constructor();
        dateTimePropertyCppValue(): string;
        timeSpanPropertyCppValue(): string;
        or(lhs: boolean, rhs: boolean): boolean;
        orAll(values: boolean[]): boolean;
        add(lhs: number, rhs: number): number;
        addAll(values: number[]): number;
        append(a: string, b: string, c: string): string;
        appendAll(values: string[]): string;
        boolOutParam(lhs: boolean, rhs: boolean): { andResult: boolean; orResult: boolean; returnValue: boolean };
        charOutParam(value: string): { next: string; prev: string; returnValue: string };
        numericOutParam(value: number): { doubledValue: number; tripledValue: number; returnValue: number };
        stringOutParam(value: string): { lower: string; upper: string; returnValue: string };
        guidOutParam(value: string): { zero: string; allSet: string; returnValue: string };
        enumOutParam(value: TestComponent.TestEnum): { plusOne: TestComponent.TestEnum; plusTwo: TestComponent.TestEnum; returnValue: TestComponent.TestEnum };
        compositeStructOutParam(input: TestComponent.CompositeType): { first: TestComponent.CompositeType; second: TestComponent.CompositeType; returnValue: TestComponent.CompositeType };
        refOutParam(value: number | null): { doubledValue: number | null; tripledValue: number | null; returnValue: number | null };
        objectOutParam(values: Windows.Foundation.Collections.IVector<number>): { doubledValues: Windows.Foundation.Collections.IVector<number>; tripledValues: Windows.Foundation.Collections.IVector<number>; returnValue: Windows.Foundation.Collections.IVector<number> };
        boolArrayOutParam(values: boolean[]): { rot1: boolean[]; rot2: boolean[]; returnValue: boolean[] };
        charArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        numericArrayOutParam(values: number[]): { rot1: number[]; rot2: number[]; returnValue: number[] };
        stringArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        guidArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        enumArrayOutParam(values: TestComponent.TestEnum[]): { rot1: TestComponent.TestEnum[]; rot2: TestComponent.TestEnum[]; returnValue: TestComponent.TestEnum[] };
        compositeStructArrayOutParam(values: TestComponent.CompositeType[]): { rot1: TestComponent.CompositeType[]; rot2: TestComponent.CompositeType[]; returnValue: TestComponent.CompositeType[] };
        refArrayOutParam(values: number[] | null): { rot1: number[] | null; rot2: number[] | null; returnValue: number[] | null };
        objectArrayOutParam(values: Windows.Foundation.Collections.IVector<number>[]): { rot1: Windows.Foundation.Collections.IVector<number>[]; rot2: Windows.Foundation.Collections.IVector<number>[]; returnValue: Windows.Foundation.Collections.IVector<number>[] };
        boolFillParam(): boolean[];
        charFillParam(): string[];
        numericFillParam(): number[];
        stringFillParam(): string[];
        guidFillParam(): string[];
        enumFillParam(): TestComponent.TestEnum[];
        compositeStructFillParam(): TestComponent.CompositeType[];
        refFillParam(): number[] | null;
        objectFillParam(): Windows.Foundation.Collections.IVector<number>[];
        raiseBoolEvent(value: boolean): void;
        raiseCharEvent(value: string): void;
        raiseNumericEvent(value: number): void;
        raiseStringEvent(value: string): void;
        raiseGuidEvent(value: string): void;
        raiseEnumEvent(value: TestComponent.TestEnum): void;
        raiseCompositeStructEvent(value: TestComponent.CompositeType): void;
        raiseRefEvent(value: number | null): void;
        raiseObjectEvent(value: Windows.Foundation.Collections.IVector<number>): void;
        static logFailures(failures: string): void;
        static staticOr(lhs: boolean, rhs: boolean): boolean;
        static staticOrAll(values: boolean[]): boolean;
        static staticAdd(lhs: number, rhs: number): number;
        static staticAddAll(values: number[]): number;
        static staticAppend(a: string, b: string, c: string): string;
        static staticAppendAll(values: string[]): string;
        static staticBoolOutParam(lhs: boolean, rhs: boolean): { andResult: boolean; orResult: boolean; returnValue: boolean };
        static staticCharOutParam(value: string): { next: string; prev: string; returnValue: string };
        static staticNumericOutParam(value: number): { doubledValue: number; tripledValue: number; returnValue: number };
        static staticStringOutParam(value: string): { lower: string; upper: string; returnValue: string };
        static staticGuidOutParam(value: string): { zero: string; allSet: string; returnValue: string };
        static staticEnumOutParam(value: TestComponent.TestEnum): { plusOne: TestComponent.TestEnum; plusTwo: TestComponent.TestEnum; returnValue: TestComponent.TestEnum };
        static staticCompositeStructOutParam(input: TestComponent.CompositeType): { first: TestComponent.CompositeType; second: TestComponent.CompositeType; returnValue: TestComponent.CompositeType };
        static staticRefOutParam(value: number | null): { doubledValue: number | null; tripledValue: number | null; returnValue: number | null };
        static staticObjectOutParam(values: Windows.Foundation.Collections.IVector<number>): { doubledValues: Windows.Foundation.Collections.IVector<number>; tripledValues: Windows.Foundation.Collections.IVector<number>; returnValue: Windows.Foundation.Collections.IVector<number> };
        static staticBoolArrayOutParam(values: boolean[]): { rot1: boolean[]; rot2: boolean[]; returnValue: boolean[] };
        static staticCharArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        static staticNumericArrayOutParam(values: number[]): { rot1: number[]; rot2: number[]; returnValue: number[] };
        static staticStringArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        static staticGuidArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        static staticEnumArrayOutParam(values: TestComponent.TestEnum[]): { rot1: TestComponent.TestEnum[]; rot2: TestComponent.TestEnum[]; returnValue: TestComponent.TestEnum[] };
        static staticCompositeStructArrayOutParam(values: TestComponent.CompositeType[]): { rot1: TestComponent.CompositeType[]; rot2: TestComponent.CompositeType[]; returnValue: TestComponent.CompositeType[] };
        static staticRefArrayOutParam(values: number[] | null): { rot1: number[] | null; rot2: number[] | null; returnValue: number[] | null };
        static staticObjectArrayOutParam(values: Windows.Foundation.Collections.IVector<number>[]): { rot1: Windows.Foundation.Collections.IVector<number>[]; rot2: Windows.Foundation.Collections.IVector<number>[]; returnValue: Windows.Foundation.Collections.IVector<number>[] };
        static staticBoolFillParam(): boolean[];
        static staticCharFillParam(): string[];
        static staticNumericFillParam(): number[];
        static staticStringFillParam(): string[];
        static staticGuidFillParam(): string[];
        static staticEnumFillParam(): TestComponent.TestEnum[];
        static staticCompositeStructFillParam(): TestComponent.CompositeType[];
        static staticRefFillParam(): number[] | null;
        static staticObjectFillParam(): Windows.Foundation.Collections.IVector<number>[];
        static raiseStaticBoolEvent(value: boolean): void;
        static raiseStaticCharEvent(value: string): void;
        static raiseStaticNumericEvent(value: number): void;
        static raiseStaticStringEvent(value: string): void;
        static raiseStaticGuidEvent(value: string): void;
        static raiseStaticEnumEvent(value: TestComponent.TestEnum): void;
        static raiseStaticCompositeStructEvent(value: TestComponent.CompositeType): void;
        static raiseStaticRefEvent(value: number | null): void;
        static raiseStaticObjectEvent(value: Windows.Foundation.Collections.IVector<number>): void;
        static staticInvokeBoolDelegate(value: boolean, targetFn: TestComponent.BoolDelegate): boolean;
        static staticInvokeCharDelegate(value: string, targetFn: TestComponent.CharDelegate): string;
        static staticInvokeNumericDelegate(value: number, targetFn: TestComponent.NumericDelegate): number;
        static staticInvokeStringDelegate(value: string, targetFn: TestComponent.StringDelegate): string;
        static staticInvokeGuidDelegate(value: string, targetFn: TestComponent.GuidDelegate): string;
        static staticInvokeEnumDelegate(value: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegate): TestComponent.TestEnum;
        static staticInvokeCompositeStructDelegate(value: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegate): TestComponent.CompositeType;
        static staticInvokeRefDelegate(value: number | null, targetFn: TestComponent.RefDelegate): number | null;
        static staticInvokeObjectDelegate(value: Windows.Foundation.Collections.IVector<number>, targetFn: TestComponent.ObjectDelegate): Windows.Foundation.Collections.IVector<number>;
        static staticInvokeBoolDelegateWithOutParam(value: boolean, targetFn: TestComponent.BoolDelegateWithOutParam): boolean;
        static staticInvokeCharDelegateWithOutParam(value: string, targetFn: TestComponent.CharDelegateWithOutParam): string;
        static staticInvokeNumericDelegateWithOutParam(value: number, targetFn: TestComponent.NumericDelegateWithOutParam): number;
        static staticInvokeStringDelegateWithOutParam(value: string, targetFn: TestComponent.StringDelegateWithOutParam): string;
        static staticInvokeGuidDelegateWithOutParam(value: string, targetFn: TestComponent.GuidDelegateWithOutParam): string;
        static staticInvokeEnumDelegateWithOutParam(value: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegateWithOutParam): TestComponent.TestEnum;
        static staticInvokeCompositeStructDelegateWithOutParam(value: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegateWithOutParam): TestComponent.CompositeType;
        static staticInvokeRefDelegateWithOutParam(value: number | null, targetFn: TestComponent.RefDelegateWithOutParam): number | null;
        static staticInvokeObjectDelegateWithOutParam(value: Windows.Foundation.Collections.IVector<number>, targetFn: TestComponent.ObjectDelegateWithOutParam): Windows.Foundation.Collections.IVector<number>;
        static staticInvokeBoolArrayDelegate(values: boolean[], targetFn: TestComponent.BoolArrayDelegate): boolean;
        static staticInvokeCharArrayDelegate(values: string[], targetFn: TestComponent.CharArrayDelegate): boolean;
        static staticInvokeNumericArrayDelegate(values: number[], targetFn: TestComponent.NumericArrayDelegate): boolean;
        static staticInvokeStringArrayDelegate(values: string[], targetFn: TestComponent.StringArrayDelegate): boolean;
        static staticInvokeGuidArrayDelegate(values: string[], targetFn: TestComponent.GuidArrayDelegate): boolean;
        static staticInvokeEnumArrayDelegate(values: TestComponent.TestEnum[], targetFn: TestComponent.EnumArrayDelegate): boolean;
        static staticInvokeCompositeStructArrayDelegate(values: TestComponent.CompositeType[], targetFn: TestComponent.CompositeStructArrayDelegate): boolean;
        static staticInvokeRefArrayDelegate(values: number[] | null, targetFn: TestComponent.RefArrayDelegate): boolean;
        static staticInvokeObjectArrayDelegate(values: Windows.Foundation.Collections.IVector<number>[], targetFn: TestComponent.ObjectArrayDelegate): boolean;
        static copyBoolsToVector(values: boolean[]): Windows.Foundation.Collections.IVector<boolean>;
        static copyCharsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        static copyNumericsToVector(values: number[]): Windows.Foundation.Collections.IVector<number>;
        static copyStringsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        static copyGuidsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        static copyEnumValuesToVector(values: TestComponent.TestEnum[]): Windows.Foundation.Collections.IVector<TestComponent.TestEnum>;
        static copyCompositeStructsToVector(values: TestComponent.CompositeType[]): Windows.Foundation.Collections.IVector<TestComponent.CompositeType>;
        static copyRefsToVector(values: number[] | null): Windows.Foundation.Collections.IVector<number | null>;
        static copyObjectsToVector(values: any): Windows.Foundation.Collections.IVector<any>;
        static returnSameBoolVector(vector: Windows.Foundation.Collections.IVector<boolean>): Windows.Foundation.Collections.IVector<boolean>;
        static returnSameCharVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        static returnSameNumericVector(vector: Windows.Foundation.Collections.IVector<number>): Windows.Foundation.Collections.IVector<number>;
        static returnSameStringVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        static returnSameGuidVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        static returnSameEnumVector(vector: Windows.Foundation.Collections.IVector<TestComponent.TestEnum>): Windows.Foundation.Collections.IVector<TestComponent.TestEnum>;
        static returnSameCompositeStructVector(vector: Windows.Foundation.Collections.IVector<TestComponent.CompositeType>): Windows.Foundation.Collections.IVector<TestComponent.CompositeType>;
        static returnSameRefVector(vector: Windows.Foundation.Collections.IVector<number | null>): Windows.Foundation.Collections.IVector<number | null>;
        static returnSameObjectVector(vector: Windows.Foundation.Collections.IVector<any>): Windows.Foundation.Collections.IVector<any>;
        static pauseAsync(milliseconds: number): Windows.Foundation.WinRTPromise<void, void>;
        static countToNumberAsync(value: number): Windows.Foundation.WinRTPromise<void, number>;
        static addAsync(lhs: number, rhs: number): Windows.Foundation.WinRTPromise<number, void>;
        static countDoubleAsync(value: number): Windows.Foundation.WinRTPromise<number, number>;
        static throwAsyncException(): Windows.Foundation.WinRTPromise<void, void>;
        addEventListener(type: "booleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        addEventListener(type: "chareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "compositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        addEventListener(type: "enumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        addEventListener(type: "guideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "numericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        addEventListener(type: "objecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        addEventListener(type: "refeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        addEventListener(type: "staticbooleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        addEventListener(type: "staticchareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "staticcompositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        addEventListener(type: "staticenumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        addEventListener(type: "staticguideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "staticnumericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        addEventListener(type: "staticobjecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        addEventListener(type: "staticrefeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        addEventListener(type: "staticstringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "stringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "booleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        removeEventListener(type: "chareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "compositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        removeEventListener(type: "enumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        removeEventListener(type: "guideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "numericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        removeEventListener(type: "objecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        removeEventListener(type: "refeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        removeEventListener(type: "staticbooleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        removeEventListener(type: "staticchareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "staticcompositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        removeEventListener(type: "staticenumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        removeEventListener(type: "staticguideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "staticnumericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        removeEventListener(type: "staticobjecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        removeEventListener(type: "staticrefeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        removeEventListener(type: "staticstringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "stringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
    }

    interface TestContract {
    }

    enum TestEnum {
        first,
        second,
        third,
        fourth,
    }

}