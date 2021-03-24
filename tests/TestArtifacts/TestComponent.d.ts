//tslint:disable

declare namespace TestComponent {
    type BoolDelegate = (inputValue: boolean) => boolean;
    
    type BoolDelegateWithOutParam = (inputValue: boolean) => boolean;
    
    interface BooleanTypes {
         b: boolean;
    }

    type CharDelegate = (inputValue: string) => string;
    
    type CharDelegateWithOutParam = (inputValue: string) => string;
    
    type CompositeStructDelegate = (inputValue: TestComponent.CompositeType) => TestComponent.CompositeType;
    
    type CompositeStructDelegateWithOutParam = (inputValue: TestComponent.CompositeType) => TestComponent.CompositeType;
    
    interface CompositeType {
         numerics: TestComponent.NumericTypes;
         strings: TestComponent.StringTypes;
         bools: TestComponent.BooleanTypes;
    }

    type EnumDelegate = (inputValue: TestComponent.TestEnum) => TestComponent.TestEnum;
    
    type EnumDelegateWithOutParam = (inputValue: TestComponent.TestEnum) => TestComponent.TestEnum;
    
    type GuidDelegate = (inputValue: string) => string;
    
    type GuidDelegateWithOutParam = (inputValue: string) => string;
    
    interface ITest {
        boolProperty: boolean;
        booleanArrayProperty: boolean[];
        booleansStructProperty: TestComponent.BooleanTypes;
        charArrayProperty: string[];
        charProperty: string;
        compositeStructArrayProperty: TestComponent.CompositeType[];
        compositeStructProperty: TestComponent.CompositeType;
        dateTimeProperty: Windows.Foundation.DateTime;
        enumArrayProperty: TestComponent.TestEnum[];
        enumProperty: TestComponent.TestEnum;
        f32Property: number;
        f64Property: number;
        guidArrayProperty: string[];
        guidProperty: string;
        hResultProperty: number;
        numericArrayProperty: number[];
        numericsStructProperty: TestComponent.NumericTypes;
        objectArrayProperty: Windows.Foundation.Collections.IVector<number>[];
        objectProperty: Windows.Foundation.Collections.IVector<number>;
        propertyValue: Windows.Foundation.IPropertyValue;
        refBooleanProperty: boolean | null;
        refCharProperty: string | null;
        refEnumProperty: TestComponent.TestEnum | null;
        refNumericProperty: number | null;
        s16Property: number;
        s32Property: number;
        s64Property: number;
        stringArrayProperty: string[];
        stringProperty: string;
        stringsStructProperty: TestComponent.StringTypes;
        timeSpanProperty: Windows.Foundation.TimeSpan;
        u16Property: number;
        u32Property: number;
        u64Property: number;
        u8Property: number;
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
        objectArrayOutParam(values: Windows.Foundation.Collections.IVector<number>[]): { rot1: Windows.Foundation.Collections.IVector<number>[]; rot2: Windows.Foundation.Collections.IVector<number>[]; returnValue: Windows.Foundation.Collections.IVector<number>[] };
        boolFillParam(): boolean[];
        charFillParam(): string[];
        numericFillParam(): number[];
        stringFillParam(): string[];
        guidFillParam(): string[];
        enumFillParam(): TestComponent.TestEnum[];
        compositeStructFillParam(): TestComponent.CompositeType[];
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
        invokeBoolDelegate(inputValue: boolean, targetFn: TestComponent.BoolDelegate): boolean;
        invokeCharDelegate(inputValue: string, targetFn: TestComponent.CharDelegate): string;
        invokeNumericDelegate(inputValue: number, targetFn: TestComponent.NumericDelegate): number;
        invokeStringDelegate(inputValue: string, targetFn: TestComponent.StringDelegate): string;
        invokeGuidDelegate(inputValue: string, targetFn: TestComponent.GuidDelegate): string;
        invokeEnumDelegate(inputValue: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegate): TestComponent.TestEnum;
        invokeCompositeStructDelegate(inputValue: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegate): TestComponent.CompositeType;
        invokeRefDelegate(inputValue: number | null, targetFn: TestComponent.RefDelegate): number | null;
        invokeObjectDelegate(inputValue: Windows.Foundation.Collections.IVector<number>, targetFn: TestComponent.ObjectDelegate): Windows.Foundation.Collections.IVector<number>;
        addEventListener(type: "booleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        addEventListener(type: "chareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "compositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        addEventListener(type: "enumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        addEventListener(type: "guideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "numericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        addEventListener(type: "objecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        addEventListener(type: "refeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        addEventListener(type: "stringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "booleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        removeEventListener(type: "chareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "compositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        removeEventListener(type: "enumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        removeEventListener(type: "guideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "numericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        removeEventListener(type: "objecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        removeEventListener(type: "refeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        removeEventListener(type: "stringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
    }

    interface ITestStatics {
        staticBoolProperty: boolean;
        logFailures(failures: string): void;
        staticOr(lhs: boolean, rhs: boolean): boolean;
        staticOrAll(values: boolean[]): boolean;
        staticAdd(lhs: number, rhs: number): number;
        staticAddAll(values: number[]): number;
        staticAppend(a: string, b: string, c: string): string;
        staticAppendAll(values: string[]): string;
        staticBoolOutParam(lhs: boolean, rhs: boolean): { andResult: boolean; orResult: boolean; returnValue: boolean };
        staticCharOutParam(value: string): { next: string; prev: string; returnValue: string };
        staticNumericOutParam(value: number): { doubledValue: number; tripledValue: number; returnValue: number };
        staticStringOutParam(value: string): { lower: string; upper: string; returnValue: string };
        staticGuidOutParam(value: string): { zero: string; allSet: string; returnValue: string };
        staticEnumOutParam(value: TestComponent.TestEnum): { plusOne: TestComponent.TestEnum; plusTwo: TestComponent.TestEnum; returnValue: TestComponent.TestEnum };
        staticCompositeStructOutParam(input: TestComponent.CompositeType): { first: TestComponent.CompositeType; second: TestComponent.CompositeType; returnValue: TestComponent.CompositeType };
        staticRefOutParam(value: number | null): { doubledValue: number | null; tripledValue: number | null; returnValue: number | null };
        staticObjectOutParam(values: Windows.Foundation.Collections.IVector<number>): { doubledValues: Windows.Foundation.Collections.IVector<number>; tripledValues: Windows.Foundation.Collections.IVector<number>; returnValue: Windows.Foundation.Collections.IVector<number> };
        staticBoolArrayOutParam(values: boolean[]): { rot1: boolean[]; rot2: boolean[]; returnValue: boolean[] };
        staticCharArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        staticNumericArrayOutParam(values: number[]): { rot1: number[]; rot2: number[]; returnValue: number[] };
        staticStringArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        staticGuidArrayOutParam(values: string[]): { rot1: string[]; rot2: string[]; returnValue: string[] };
        staticEnumArrayOutParam(values: TestComponent.TestEnum[]): { rot1: TestComponent.TestEnum[]; rot2: TestComponent.TestEnum[]; returnValue: TestComponent.TestEnum[] };
        staticCompositeStructArrayOutParam(values: TestComponent.CompositeType[]): { rot1: TestComponent.CompositeType[]; rot2: TestComponent.CompositeType[]; returnValue: TestComponent.CompositeType[] };
        staticObjectArrayOutParam(values: Windows.Foundation.Collections.IVector<number>[]): { rot1: Windows.Foundation.Collections.IVector<number>[]; rot2: Windows.Foundation.Collections.IVector<number>[]; returnValue: Windows.Foundation.Collections.IVector<number>[] };
        staticBoolFillParam(): boolean[];
        staticCharFillParam(): string[];
        staticNumericFillParam(): number[];
        staticStringFillParam(): string[];
        staticGuidFillParam(): string[];
        staticEnumFillParam(): TestComponent.TestEnum[];
        staticCompositeStructFillParam(): TestComponent.CompositeType[];
        staticObjectFillParam(): Windows.Foundation.Collections.IVector<number>[];
        raiseStaticBoolEvent(value: boolean): void;
        raiseStaticCharEvent(value: string): void;
        raiseStaticNumericEvent(value: number): void;
        raiseStaticStringEvent(value: string): void;
        raiseStaticGuidEvent(value: string): void;
        raiseStaticEnumEvent(value: TestComponent.TestEnum): void;
        raiseStaticCompositeStructEvent(value: TestComponent.CompositeType): void;
        raiseStaticRefEvent(value: number | null): void;
        raiseStaticObjectEvent(value: Windows.Foundation.Collections.IVector<number>): void;
        staticInvokeBoolDelegate(inputValue: boolean, targetFn: TestComponent.BoolDelegate): boolean;
        staticInvokeCharDelegate(inputValue: string, targetFn: TestComponent.CharDelegate): string;
        staticInvokeNumericDelegate(inputValue: number, targetFn: TestComponent.NumericDelegate): number;
        staticInvokeStringDelegate(inputValue: string, targetFn: TestComponent.StringDelegate): string;
        staticInvokeGuidDelegate(inputValue: string, targetFn: TestComponent.GuidDelegate): string;
        staticInvokeEnumDelegate(inputValue: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegate): TestComponent.TestEnum;
        staticInvokeCompositeStructDelegate(inputValue: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegate): TestComponent.CompositeType;
        staticInvokeRefDelegate(inputValue: number | null, targetFn: TestComponent.RefDelegate): number | null;
        staticInvokeObjectDelegate(inputValue: Windows.Foundation.Collections.IVector<number>, targetFn: TestComponent.ObjectDelegate): Windows.Foundation.Collections.IVector<number>;
        copyBoolsToVector(values: boolean[]): Windows.Foundation.Collections.IVector<boolean>;
        copyCharsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        copyNumericsToVector(values: number[]): Windows.Foundation.Collections.IVector<number>;
        copyStringsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        copyGuidsToVector(values: string[]): Windows.Foundation.Collections.IVector<string>;
        copyEnumValuesToVector(values: TestComponent.TestEnum[]): Windows.Foundation.Collections.IVector<TestComponent.TestEnum>;
        copyCompositeStructsToVector(values: TestComponent.CompositeType[]): Windows.Foundation.Collections.IVector<TestComponent.CompositeType>;
        copyRefsToVector(values: number[] | null): Windows.Foundation.Collections.IVector<number | null>;
        copyObjectsToVector(values: any): Windows.Foundation.Collections.IVector<any>;
        returnSameBoolVector(vector: Windows.Foundation.Collections.IVector<boolean>): Windows.Foundation.Collections.IVector<boolean>;
        returnSameCharVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        returnSameNumericVector(vector: Windows.Foundation.Collections.IVector<number>): Windows.Foundation.Collections.IVector<number>;
        returnSameStringVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        returnSameGuidVector(vector: Windows.Foundation.Collections.IVector<string>): Windows.Foundation.Collections.IVector<string>;
        returnSameEnumVector(vector: Windows.Foundation.Collections.IVector<TestComponent.TestEnum>): Windows.Foundation.Collections.IVector<TestComponent.TestEnum>;
        returnSameCompositeStructVector(vector: Windows.Foundation.Collections.IVector<TestComponent.CompositeType>): Windows.Foundation.Collections.IVector<TestComponent.CompositeType>;
        returnSameRefVector(vector: Windows.Foundation.Collections.IVector<number | null>): Windows.Foundation.Collections.IVector<number | null>;
        returnSameObjectVector(vector: Windows.Foundation.Collections.IVector<any>): Windows.Foundation.Collections.IVector<any>;
        pauseAsync(milliseconds: number): Windows.Foundation.WinRTPromise<void, void>;
        countToNumberAsync(value: number): Windows.Foundation.WinRTPromise<void, number>;
        addAsync(lhs: number, rhs: number): Windows.Foundation.WinRTPromise<number, void>;
        countDoubleAsync(value: number): Windows.Foundation.WinRTPromise<number, number>;
        throwAsyncException(): Windows.Foundation.WinRTPromise<void, void>;
        addEventListener(type: "staticbooleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        addEventListener(type: "staticchareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "staticcompositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        addEventListener(type: "staticenumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        addEventListener(type: "staticguideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        addEventListener(type: "staticnumericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        addEventListener(type: "staticobjecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        addEventListener(type: "staticrefeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        addEventListener(type: "staticstringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "staticbooleventhandler", listener: Windows.Foundation.EventHandler<boolean>): void
        removeEventListener(type: "staticchareventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "staticcompositestructeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.CompositeType>): void
        removeEventListener(type: "staticenumeventhandler", listener: Windows.Foundation.EventHandler<TestComponent.TestEnum>): void
        removeEventListener(type: "staticguideventhandler", listener: Windows.Foundation.EventHandler<string>): void
        removeEventListener(type: "staticnumericeventhandler", listener: Windows.Foundation.EventHandler<number>): void
        removeEventListener(type: "staticobjecteventhandler", listener: Windows.Foundation.EventHandler<Windows.Foundation.Collections.IVector<number>>): void
        removeEventListener(type: "staticrefeventhandler", listener: Windows.Foundation.EventHandler<number | null>): void
        removeEventListener(type: "staticstringeventhandler", listener: Windows.Foundation.EventHandler<string>): void
    }

    type NumericDelegate = (inputValue: number) => number;
    
    type NumericDelegateWithOutParam = (inputValue: number) => number;
    
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
         e: TestComponent.TestEnum;
    }

    type ObjectDelegate = (inputValue: Windows.Foundation.Collections.IVector<number>) => Windows.Foundation.Collections.IVector<number>;
    
    type ObjectDelegateWithOutParam = (inputValue: Windows.Foundation.Collections.IVector<number>) => Windows.Foundation.Collections.IVector<number>;
    
    type RefDelegate = (inputValue: number | null) => number | null;
    
    type RefDelegateWithOutParam = (inputValue: number | null) => number | null;
    
    type StringDelegate = (inputValue: string) => string;
    
    type StringDelegateWithOutParam = (inputValue: string) => string;
    
    interface StringTypes {
         ch: string;
         str: string;
         guid: string;
    }

    class Test implements TestComponent.ITest {
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
        objectArrayProperty: Windows.Foundation.Collections.IVector<number>[];
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
        propertyValue: Windows.Foundation.IPropertyValue;
        objectProperty: Windows.Foundation.Collections.IVector<number>;
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
        objectArrayOutParam(values: Windows.Foundation.Collections.IVector<number>[]): { rot1: Windows.Foundation.Collections.IVector<number>[]; rot2: Windows.Foundation.Collections.IVector<number>[]; returnValue: Windows.Foundation.Collections.IVector<number>[] };
        boolFillParam(): boolean[];
        charFillParam(): string[];
        numericFillParam(): number[];
        stringFillParam(): string[];
        guidFillParam(): string[];
        enumFillParam(): TestComponent.TestEnum[];
        compositeStructFillParam(): TestComponent.CompositeType[];
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
        invokeBoolDelegate(inputValue: boolean, targetFn: TestComponent.BoolDelegate): boolean;
        invokeCharDelegate(inputValue: string, targetFn: TestComponent.CharDelegate): string;
        invokeNumericDelegate(inputValue: number, targetFn: TestComponent.NumericDelegate): number;
        invokeStringDelegate(inputValue: string, targetFn: TestComponent.StringDelegate): string;
        invokeGuidDelegate(inputValue: string, targetFn: TestComponent.GuidDelegate): string;
        invokeEnumDelegate(inputValue: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegate): TestComponent.TestEnum;
        invokeCompositeStructDelegate(inputValue: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegate): TestComponent.CompositeType;
        invokeRefDelegate(inputValue: number | null, targetFn: TestComponent.RefDelegate): number | null;
        invokeObjectDelegate(inputValue: Windows.Foundation.Collections.IVector<number>, targetFn: TestComponent.ObjectDelegate): Windows.Foundation.Collections.IVector<number>;
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
        static staticObjectArrayOutParam(values: Windows.Foundation.Collections.IVector<number>[]): { rot1: Windows.Foundation.Collections.IVector<number>[]; rot2: Windows.Foundation.Collections.IVector<number>[]; returnValue: Windows.Foundation.Collections.IVector<number>[] };
        static staticBoolFillParam(): boolean[];
        static staticCharFillParam(): string[];
        static staticNumericFillParam(): number[];
        static staticStringFillParam(): string[];
        static staticGuidFillParam(): string[];
        static staticEnumFillParam(): TestComponent.TestEnum[];
        static staticCompositeStructFillParam(): TestComponent.CompositeType[];
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
        static staticInvokeBoolDelegate(inputValue: boolean, targetFn: TestComponent.BoolDelegate): boolean;
        static staticInvokeCharDelegate(inputValue: string, targetFn: TestComponent.CharDelegate): string;
        static staticInvokeNumericDelegate(inputValue: number, targetFn: TestComponent.NumericDelegate): number;
        static staticInvokeStringDelegate(inputValue: string, targetFn: TestComponent.StringDelegate): string;
        static staticInvokeGuidDelegate(inputValue: string, targetFn: TestComponent.GuidDelegate): string;
        static staticInvokeEnumDelegate(inputValue: TestComponent.TestEnum, targetFn: TestComponent.EnumDelegate): TestComponent.TestEnum;
        static staticInvokeCompositeStructDelegate(inputValue: TestComponent.CompositeType, targetFn: TestComponent.CompositeStructDelegate): TestComponent.CompositeType;
        static staticInvokeRefDelegate(inputValue: number | null, targetFn: TestComponent.RefDelegate): number | null;
        static staticInvokeObjectDelegate(inputValue: Windows.Foundation.Collections.IVector<number>, targetFn: TestComponent.ObjectDelegate): Windows.Foundation.Collections.IVector<number>;
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