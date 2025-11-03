// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * Basic Function Tests - Node.js Adapter
 * @format
 */

const {
    TestScenario,
    assert,
    allSetGuid,
    zeroGuid
} = require('./TestCommon');

function makeBasicFunctionTestScenarios(pThis, TestComponent, TestValues) {
    return [
        // Static functions
        new TestScenario('Test::StaticOr', () => runStaticOr(pThis, TestComponent)),
        new TestScenario('Test::StaticOrAll', () => runStaticOrAll(pThis, TestComponent)),
        new TestScenario('Test::StaticAdd', () => runStaticAdd(pThis, TestComponent)),
        new TestScenario('Test::StaticAddAll', () => runStaticAddAll(pThis, TestComponent)),
        new TestScenario('Test::StaticAppend', () => runStaticAppend(pThis, TestComponent)),
        new TestScenario('Test::StaticAppendAll', () => runStaticAppendAll(pThis, TestComponent)),

        // Static overloads
        new TestScenario('Test::StaticArityOverload', () => runStaticArityOverload(pThis, TestComponent)),
        new TestScenario('Test::StaticDefaultOverload', () => runStaticDefaultOverload(pThis, TestComponent)),
        new TestScenario('Test::StaticOutParamOverload', () => runStaticOutParamOverload(pThis, TestComponent)),
        new TestScenario('Test::StaticContractArityOverload', () => runStaticContractArityOverload(pThis, TestComponent)),
        new TestScenario('Test::StaticContractDefaultOverloadV1', () => runStaticContractDefaultOverloadV1(pThis, TestComponent)),
        new TestScenario('Test::StaticContractDefaultOverloadV2', () => runStaticContractDefaultOverloadV2(pThis, TestComponent)),
        new TestScenario('Test::StaticContractOutParamOverloadV1', () => runStaticContractOutParamOverloadV1(pThis, TestComponent)),
        new TestScenario('Test::StaticContractOutParamOverloadV2', () => runStaticContractOutParamOverloadV2(pThis, TestComponent)),

        // Static out params
        new TestScenario('Test::StaticBoolOutParam', () => runStaticBoolOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticCharOutParam', () => runStaticCharOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticNumericOutParam', () => runStaticNumericOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticStringOutParam', () => runStaticStringOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticGuidOutParam', () => runStaticGuidOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticEnumOutParam', () => runStaticEnumOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticCompositeStructOutParam', () => runStaticCompositeStructOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticRefOutParam', () => runStaticRefOutParam(pThis, TestComponent)),
        new TestScenario('Test::StaticObjectOutParam', () => runStaticObjectOutParam(pThis, TestComponent)),

        // Static "interwoven" params
        new TestScenario('Test::StaticInterwovenParams', () => runStaticInterwovenParams(pThis, TestComponent)),

        // Constructors
        new TestScenario('Constructor Test', () => runConstructorTest(pThis, TestComponent)),

        // Non-static functions
        new TestScenario('Test::Or', () => runOr(pThis, TestComponent)),
        new TestScenario('Test::OrAll', () => runOrAll(pThis, TestComponent)),
        new TestScenario('Test::Add', () => runAdd(pThis, TestComponent)),
        new TestScenario('Test::AddAll', () => runAddAll(pThis, TestComponent)),
        new TestScenario('Test::Append', () => runAppend(pThis, TestComponent)),
        new TestScenario('Test::AppendAll', () => runAppendAll(pThis, TestComponent)),

        // Non-static overloads
        new TestScenario('Test::ArityOverload', () => runArityOverload(pThis, TestComponent)),
        new TestScenario('Test::DefaultOverload', () => runDefaultOverload(pThis, TestComponent)),
        new TestScenario('Test::OutParamOverload', () => runOutParamOverload(pThis, TestComponent)),
        new TestScenario('Test::ContractArityOverload', () => runContractArityOverload(pThis, TestComponent)),
        new TestScenario('Test::ContractDefaultOverloadV1', () => runContractDefaultOverloadV1(pThis, TestComponent)),
        new TestScenario('Test::ContractDefaultOverloadV2', () => runContractDefaultOverloadV2(pThis, TestComponent)),
        new TestScenario('Test::ContractOutParamOverloadV1', () => runContractOutParamOverloadV1(pThis, TestComponent)),
        new TestScenario('Test::ContractOutParamOverloadV2', () => runContractOutParamOverloadV2(pThis, TestComponent)),

        // Non-static out params
        new TestScenario('Test::BoolOutParam', () => runBoolOutParam(pThis, TestComponent)),
        new TestScenario('Test::CharOutParam', () => runCharOutParam(pThis, TestComponent)),
        new TestScenario('Test::NumericOutParam', () => runNumericOutParam(pThis, TestComponent)),
        new TestScenario('Test::StringOutParam', () => runStringOutParam(pThis, TestComponent)),
        new TestScenario('Test::GuidOutParam', () => runGuidOutParam(pThis, TestComponent)),
        new TestScenario('Test::EnumOutParam', () => runEnumOutParam(pThis, TestComponent)),
        new TestScenario('Test::CompositeStructOutParam', () => runCompositeStructOutParam(pThis, TestComponent)),
        new TestScenario('Test::RefOutParam', () => runRefOutParam(pThis, TestComponent)),
        new TestScenario('Test::ObjectOutParam', () => runObjectOutParam(pThis, TestComponent)),

        // Non-static "interwoven" params
        new TestScenario('Test::InterwovenParams', () => runInterwovenParams(pThis, TestComponent)),

        // Static methods for non activable classes
        new TestScenario('StaticOnlyTest::CopyString', () => runStaticMethodForNonActivableMethod(pThis, TestComponent)),
    ];
}

// Static functions
function runStaticOr(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal(TestComponent.Test.staticOr(false, false), false);
        assert.equal(TestComponent.Test.staticOr(false, true), true);
        assert.equal(TestComponent.Test.staticOr(true, false), true);
        assert.equal(TestComponent.Test.staticOr(true, true), true);
    });
}

function runStaticOrAll(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var run = (arr, expect) => {
            assert.equal(TestComponent.Test.staticOrAll(arr), expect);
        };
        run([], false);
        run([false], false);
        run([true], true);
        run([false, false, false, false], false);
        run([true, false, false, false], true);
        run([false, true, false, false], true);
        run([false, false, true, false], true);
        run([false, false, false, true], true);
    });
}

function runStaticAdd(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal(TestComponent.Test.staticAdd(0, 0), 0);
        assert.equal(TestComponent.Test.staticAdd(0, 42), 42);
        assert.equal(TestComponent.Test.staticAdd(42, 0), 42);
        assert.equal(TestComponent.Test.staticAdd(42, 42), 84);
        assert.equal(TestComponent.Test.staticAdd(42, -42), 0);
    });
}

function runStaticAddAll(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var run = (arr, expect) => {
            assert.equal(TestComponent.Test.staticAddAll(arr), expect);
        };
        run([], 0);
        run([1], 1);
        run([1, 2, 3], 6);
        run([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 55);
    });
}

function runStaticAppend(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal(TestComponent.Test.staticAppend('foo', '\0', 'bar'), 'foo\0bar');
        assert.equal(TestComponent.Test.staticAppend('Hello', ' ', 'world'), 'Hello world');
    });
}

function runStaticAppendAll(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var run = (arr, expect) => {
            assert.equal(TestComponent.Test.staticAppendAll(arr), expect);
        };
        run([], '');
        run(['foo'], 'foo');
        run(['f\0o\0o', '\0', 'b\0a\0r'], 'f\0o\0o\0b\0a\0r');
    });
}

// Static overloads
function runStaticArityOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal('No-arg overload', TestComponent.Test.staticArityOverload());
        assert.equal('Testing', TestComponent.Test.staticArityOverload('Testing'));
        assert.equal('foobar', TestComponent.Test.staticArityOverload('foo', 'bar'));
    });
}

function runStaticDefaultOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal('foofoo', TestComponent.Test.staticDefaultOverload('foo', 2));
    });
}

function runStaticOutParamOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticOutParamOverload('foo');
        assert.equal('Success!', result.outParam);
        assert.equal('foo', result.returnValue);
    });
}

function runStaticContractArityOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal('No-arg overload', TestComponent.Test.staticContractArityOverload());
        assert.equal('Testing', TestComponent.Test.staticContractArityOverload('Testing'));
    });
}

function runStaticContractDefaultOverloadV1(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal('foofoo', TestComponent.Test.staticContractDefaultOverloadV1('foo', 2));
    });
}

function runStaticContractDefaultOverloadV2(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal('foofoo', TestComponent.Test.staticContractDefaultOverloadV2('foo', 2));
    });
}

function runStaticContractOutParamOverloadV1(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticContractOutParamOverloadV1('foo');
        assert.equal('Success!', result.outParam);
        assert.equal('foo', result.returnValue);
    });
}

function runStaticContractOutParamOverloadV2(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticContractOutParamOverloadV2('foo');
        assert.equal('Success!', result.outParam);
        assert.equal('foo', result.returnValue);
    });
}

// Static out params
function runStaticBoolOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticBoolOutParam(true);
        assert.equal(result.first, true);
        assert.equal(result.second, false);

        result = TestComponent.Test.staticBoolOutParam(false);
        assert.equal(result.first, false);
        assert.equal(result.second, true);
    });
}

function runStaticCharOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticCharOutParam('\0');
        assert.equal(result.first, '\0');
        assert.equal(result.second, 'z');

        result = TestComponent.Test.staticCharOutParam('a');
        assert.equal(result.first, 'a');
        assert.equal(result.second, 'z');
    });
}

function runStaticNumericOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticNumericOutParam(
            { u8: 0, u16: 1, u32: 2, u64: 3, s16: 4, s32: 5, s64: 6, f32: 7.0, f64: 8.0, enum: TestComponent.TestEnum.first });
        assert.equal(result.input, 
            { u8: 0, u16: 1, u32: 2, u64: 3, s16: 4, s32: 5, s64: 6, f32: 7.0, f64: 8.0, enum: TestComponent.TestEnum.first });
        assert.equal(result.output, 
            { u8: 1, u16: 2, u32: 3, u64: 4, s16: 5, s32: 6, s64: 7, f32: 8.0, f64: 9.0, enum: TestComponent.TestEnum.second });
    });
}

function runStaticStringOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticStringOutParam({ char: 'a', string: 'foo', guid: zeroGuid });
        assert.equal(result.input, { char: 'a', string: 'foo', guid: zeroGuid });
        assert.equal(result.output, { char: 'b', string: 'bar', guid: allSetGuid });
    });
}

function runStaticGuidOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticGuidOutParam(zeroGuid);
        assert.equal(result.same, zeroGuid);
        assert.equal(result.ret, allSetGuid);
    });
}

function runStaticEnumOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticEnumOutParam(TestComponent.TestEnum.first);
        assert.equal(result.out, TestComponent.TestEnum.second);
        assert.equal(result.ret, TestComponent.TestEnum.first);
    });
}

function runStaticCompositeStructOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var input = {
            numerics: { u8: 0, u16: 1, u32: 2, u64: 3, s16: 4, s32: 5, s64: 6, f32: 7.0, f64: 8.0, enum: TestComponent.TestEnum.first },
            strings: { char: 'a', string: 'foo', guid: zeroGuid },
            bools: { value: true }
        };
        var result = TestComponent.Test.staticCompositeStructOutParam(input);
        assert.equal(result.value, input);
        assert.equal(result.out, {
            numerics: { u8: 1, u16: 2, u32: 3, u64: 4, s16: 5, s32: 6, s64: 7, f32: 8.0, f64: 9.0, enum: TestComponent.TestEnum.second },
            strings: { char: 'b', string: 'bar', guid: allSetGuid },
            bools: { value: false }
        });
    });
}

function runStaticRefOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var obj = new TestComponent.Test();
        var result = TestComponent.Test.staticRefOutParam(obj);
        assert.equal(result.value, obj);
        assert.notEqual(result.out, null);
    });
}

function runStaticObjectOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var obj = new TestComponent.TestObject(42);
        var result = TestComponent.Test.staticObjectOutParam(obj);
        assert.equal(result.value.value, 42);
        assert.equal(result.out.value, 0);
    });
}

// Static "interwoven" params
function runStaticInterwovenParams(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var result = TestComponent.Test.staticInterwovenParams(0, 1, 2, 3);
        assert.equal(result.out1, 1);
        assert.equal(result.out2, 3);
        assert.equal(result.ret, 6);
    });
}

// Constructors
function runConstructorTest(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.notEqual(test, null);
        assert.notEqual(test, undefined);
    });
}

// Non-static functions
function runOr(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal(test.or(false, false), false);
        assert.equal(test.or(false, true), true);
        assert.equal(test.or(true, false), true);
        assert.equal(test.or(true, true), true);
    });
}

function runOrAll(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (arr, expect) => {
            assert.equal(test.orAll(arr), expect);
        };
        run([], false);
        run([false], false);
        run([true], true);
        run([false, false, false, false], false);
        run([true, false, false, false], true);
    });
}

function runAdd(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal(test.add(0, 0), 0);
        assert.equal(test.add(0, 42), 42);
        assert.equal(test.add(42, 0), 42);
        assert.equal(test.add(42, 42), 84);
    });
}

function runAddAll(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (arr, expect) => {
            assert.equal(test.addAll(arr), expect);
        };
        run([], 0);
        run([1], 1);
        run([1, 2, 3], 6);
    });
}

function runAppend(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal(test.append('foo', '\0', 'bar'), 'foo\0bar');
        assert.equal(test.append('Hello', ' ', 'world'), 'Hello world');
    });
}

function runAppendAll(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (arr, expect) => {
            assert.equal(test.appendAll(arr), expect);
        };
        run([], '');
        run(['foo'], 'foo');
    });
}

// Non-static overloads
function runArityOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal('No-arg overload', test.arityOverload());
        assert.equal('Testing', test.arityOverload('Testing'));
        assert.equal('foobar', test.arityOverload('foo', 'bar'));
    });
}

function runDefaultOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal('foofoo', test.defaultOverload('foo', 2));
    });
}

function runOutParamOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.outParamOverload('foo');
        assert.equal('Success!', result.outParam);
        assert.equal('foo', result.returnValue);
    });
}

function runContractArityOverload(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal('No-arg overload', test.contractArityOverload());
        assert.equal('Testing', test.contractArityOverload('Testing'));
    });
}

function runContractDefaultOverloadV1(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal('foofoo', test.contractDefaultOverloadV1('foo', 2));
    });
}

function runContractDefaultOverloadV2(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        assert.equal('foofoo', test.contractDefaultOverloadV2('foo', 2));
    });
}

function runContractOutParamOverloadV1(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.contractOutParamOverloadV1('foo');
        assert.equal('Success!', result.outParam);
        assert.equal('foo', result.returnValue);
    });
}

function runContractOutParamOverloadV2(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.contractOutParamOverloadV2('foo');
        assert.equal('Success!', result.outParam);
        assert.equal('foo', result.returnValue);
    });
}

// Non-static out params
function runBoolOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.boolOutParam(true);
        assert.equal(result.first, true);
        assert.equal(result.second, false);
    });
}

function runCharOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.charOutParam('\0');
        assert.equal(result.first, '\0');
        assert.equal(result.second, 'z');
    });
}

function runNumericOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.numericOutParam(
            { u8: 0, u16: 1, u32: 2, u64: 3, s16: 4, s32: 5, s64: 6, f32: 7.0, f64: 8.0, enum: TestComponent.TestEnum.first });
        assert.equal(result.input,
            { u8: 0, u16: 1, u32: 2, u64: 3, s16: 4, s32: 5, s64: 6, f32: 7.0, f64: 8.0, enum: TestComponent.TestEnum.first });
        assert.equal(result.output,
            { u8: 1, u16: 2, u32: 3, u64: 4, s16: 5, s32: 6, s64: 7, f32: 8.0, f64: 9.0, enum: TestComponent.TestEnum.second });
    });
}

function runStringOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.stringOutParam({ char: 'a', string: 'foo', guid: zeroGuid });
        assert.equal(result.input, { char: 'a', string: 'foo', guid: zeroGuid });
        assert.equal(result.output, { char: 'b', string: 'bar', guid: allSetGuid });
    });
}

function runGuidOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.guidOutParam(zeroGuid);
        assert.equal(result.same, zeroGuid);
        assert.equal(result.ret, allSetGuid);
    });
}

function runEnumOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.enumOutParam(TestComponent.TestEnum.first);
        assert.equal(result.out, TestComponent.TestEnum.second);
        assert.equal(result.ret, TestComponent.TestEnum.first);
    });
}

function runCompositeStructOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var input = {
            numerics: { u8: 0, u16: 1, u32: 2, u64: 3, s16: 4, s32: 5, s64: 6, f32: 7.0, f64: 8.0, enum: TestComponent.TestEnum.first },
            strings: { char: 'a', string: 'foo', guid: zeroGuid },
            bools: { value: true }
        };
        var result = test.compositeStructOutParam(input);
        assert.equal(result.value, input);
        assert.equal(result.out, {
            numerics: { u8: 1, u16: 2, u32: 3, u64: 4, s16: 5, s32: 6, s64: 7, f32: 8.0, f64: 9.0, enum: TestComponent.TestEnum.second },
            strings: { char: 'b', string: 'bar', guid: allSetGuid },
            bools: { value: false }
        });
    });
}

function runRefOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.refOutParam(test);
        assert.equal(result.value, test);
        assert.notEqual(result.out, null);
    });
}

function runObjectOutParam(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var obj = new TestComponent.TestObject(42);
        var result = test.objectOutParam(obj);
        assert.equal(result.value.value, 42);
        assert.equal(result.out.value, 0);
    });
}

// Non-static "interwoven" params
function runInterwovenParams(pThis, TestComponent) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var result = test.interwovenParams(0, 1, 2, 3);
        assert.equal(result.out1, 1);
        assert.equal(result.out2, 3);
        assert.equal(result.ret, 6);
    });
}

// Static methods for non-activable classes
function runStaticMethodForNonActivableMethod(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal(TestComponent.StaticOnlyTest.copyString("Hello"), "Hello");
    });
}

module.exports = { makeBasicFunctionTestScenarios };
