/**
 * @format
 */
// TODO: JSI does not allow property setters to return a value, which causes Chakra to throw errors in strict mode
//       https://github.com/microsoft/jswinrt/issues/8
// 'use strict';

import React, { Component } from 'react';
import {
    SafeAreaView,
    ScrollView,
    View,
    Text,
} from 'react-native';
import { Scenario } from './Scenario';
import { styles } from './Styles';
import EventEmitter from 'react-native/Libraries/vendor/emitter/EventEmitter';

function assert(val) {
    if (!val) {
        throw new Error('Assertion failed!');
    }
}

function stringify(val) {
    if (val == null) return 'null';
    else if (typeof(val) == 'object') return JSON.stringify(val);
    return val.toString();
}

function checkEquals(lhs, rhs) {
    if (typeof(lhs) != typeof(rhs)) {
        return {
            success: false,
            msg: 'Types are not the same. \'' + typeof(lhs) + '\' (' + stringify(lhs) + ') vs \'' + typeof(rhs) + '\' (' + stringify(rhs) + ')'
        };
    } else if ((lhs == null) && (rhs == null)) {
        return { success: true };
    } else if ((lhs == null) || (rhs == null)) {
        return {
            success: false,
            msg: '\'' + stringify(lhs) + '\' =/= \'' + stringify(rhs) + '\''
        };
    }

    if (typeof(lhs) === 'object') {
        for (var prop in lhs) {
            if (!rhs.hasOwnProperty(prop)) {
                return {
                    success: false,
                    msg: 'Object does not have property \'' + prop + '\''
                };
            }
        }

        for (var prop in rhs) {
            if (!lhs.hasOwnProperty(prop)) {
                return {
                    success: false,
                    msg: 'Object does not have property \'' + prop + '\''
                };
            }

            var result = checkEquals(lhs[prop], rhs[prop]);
            if (!result.success) return result;
        }
    } else {
        if (lhs != rhs) {
            return {
                success: false,
                msg: '\'' + stringify(lhs) + '\' =/= \'' + stringify(rhs) + '\''
            };
        }
    }

    return { success: true };
}

function assertEqual(lhs, rhs) {
    var result = checkEquals(lhs, rhs);
    if (!result.success) {
        throw new Error('assertEqual failed! ' + result.msg);
    }
}

function assertNotEqual(lhs, rhs) {
    var result = checkEquals(lhs, rhs);
    if (result.success) {
        throw new Error('assertNotEqual failed! \'' + stringify(lhs) + '\' == \'' + stringify(rhs) + '\'');
    }
}

export const TestResult = {
    NotRun: 0,
    Pass: 1,
    Fail: 2,
}

class TestScenario extends EventEmitter {
    name;
    invoke;
    result = TestResult.NotRun;
    failureText = '';

    constructor(name, invoke) {
        super();
        this.name = name;
        this.invoke = invoke;
    }
}

class App extends Component {
    test = new TestComponent.Test();
    tests = TestComponent.TestRunner.makeTests();
    state = { completedCount: 0, passCount: 0 };
    completedCount = 0;
    passCount = 0;

    scenarios = [
        // Static properties
        // TODO: Figure out why static properties are not working: https://github.com/microsoft/jswinrt/issues/7
        // new TestScenario('Test::StaticBoolProperty', this.runStaticBoolProperty.bind(this)),

        // Static functions
        new TestScenario('Test::StaticOr', this.runStaticOr.bind(this)),
        new TestScenario('Test::StaticAdd', this.runStaticAdd.bind(this)),
        new TestScenario('Test::StaticAppend', this.runStaticAppend.bind(this)),

        // Static out params
        new TestScenario('Test::StaticBoolOutParam', this.runStaticBoolOutParam.bind(this)),
        new TestScenario('Test::StaticBoolOutParam2', this.runStaticBoolOutParam2.bind(this)),
        new TestScenario('Test::StaticBoolOutParamWithReturn', this.runStaticBoolOutParamWithReturn.bind(this)),
        new TestScenario('Test::StaticCharOutParam', this.runStaticCharOutParam.bind(this)),
        new TestScenario('Test::StaticCharOutParam2', this.runStaticCharOutParam2.bind(this)),
        new TestScenario('Test::StaticCharOutParamWithReturn', this.runStaticCharOutParamWithReturn.bind(this)),
        new TestScenario('Test::StaticNumericOutParam', this.runStaticNumericOutParam.bind(this)),
        new TestScenario('Test::StaticNumericOutParam2', this.runStaticNumericOutParam2.bind(this)),
        new TestScenario('Test::StaticNumericOutParamWithReturn', this.runStaticNumericOutParamWithReturn.bind(this)),
        new TestScenario('Test::StaticStringOutParam', this.runStaticStringOutParam.bind(this)),
        new TestScenario('Test::StaticStringOutParam2', this.runStaticStringOutParam2.bind(this)),
        new TestScenario('Test::StaticStringOutParamWithReturn', this.runStaticStringOutParamWithReturn.bind(this)),

        // Non-static properties
        new TestScenario('Test::BoolProperty', this.runBoolProperty.bind(this)),
        new TestScenario('Test::CharProperty', this.runCharProperty.bind(this)),
        new TestScenario('Test::U8Property', this.runU8Property.bind(this)),
        new TestScenario('Test::U16Property', this.runU16Property.bind(this)),
        new TestScenario('Test::U32Property', this.runU32Property.bind(this)),
        new TestScenario('Test::U64Property', this.runU64Property.bind(this)),
        new TestScenario('Test::S16Property', this.runS16Property.bind(this)),
        new TestScenario('Test::S32Property', this.runS32Property.bind(this)),
        new TestScenario('Test::S64Property', this.runS64Property.bind(this)),
        new TestScenario('Test::F32Property', this.runF32Property.bind(this)),
        new TestScenario('Test::F64Property', this.runF64Property.bind(this)),
        new TestScenario('Test::StringProperty', this.runStringProperty.bind(this)),
        new TestScenario('Test::GuidProperty', this.runGuidProperty.bind(this)),
        new TestScenario('Test::EnumProperty', this.runEnumProperty.bind(this)),
        new TestScenario('Test::NumericsStructProperty', this.runNumericsStructProperty.bind(this)),
        new TestScenario('Test::StringsStructProperty', this.runStringsStructProperty.bind(this)),
        new TestScenario('Test::BooleansStructProperty', this.runBooleansStructProperty.bind(this)),
        new TestScenario('Test::CompositeStructProperty', this.runCompositeStructProperty.bind(this)),
        new TestScenario('Test::RefBooleanProperty', this.runRefBooleanProperty.bind(this)),
        new TestScenario('Test::RefCharProperty', this.runRefCharProperty.bind(this)),
        new TestScenario('Test::RefNumericProperty', this.runRefNumericProperty.bind(this)),
        new TestScenario('Test::RefEnumProperty', this.runRefEnumProperty.bind(this)),
        new TestScenario('Test::ObjectProperty', this.runObjectProperty.bind(this)),

        // Non-static array properties
        new TestScenario('Test::BooleanArrayProperty', this.runBooleanArrayProperty.bind(this)),
        new TestScenario('Test::CharArrayProperty', this.runCharArrayProperty.bind(this)),
        new TestScenario('Test::NumericArrayProperty', this.runNumericArrayProperty.bind(this)),
        new TestScenario('Test::StringArrayProperty', this.runStringArrayProperty.bind(this)),
        new TestScenario('Test::GuidArrayProperty', this.runGuidArrayProperty.bind(this)),
        new TestScenario('Test::EnumArrayProperty', this.runEnumArrayProperty.bind(this)),
        new TestScenario('Test::CompositeStructArrayProperty', this.runCompositeStructArrayProperty.bind(this)),
        // TODO: Causes compilation errors in jswinrt: https://github.com/microsoft/jswinrt/issues/9
        // new TestScenario('Test::RefArrayProperty', this.runRefArrayProperty.bind(this)),
        new TestScenario('Test::ObjectArrayProperty', this.runObjectArrayProperty.bind(this)),

        // Non-static functions
        new TestScenario('Test::Or', this.runOr.bind(this)),
        new TestScenario('Test::Add', this.runAdd.bind(this)),
        new TestScenario('Test::Append', this.runAppend.bind(this)),

        // Non-static out params
        new TestScenario('Test::BoolOutParam', this.runBoolOutParam.bind(this)),
        new TestScenario('Test::BoolOutParam2', this.runBoolOutParam2.bind(this)),
        new TestScenario('Test::BoolOutParamWithReturn', this.runBoolOutParamWithReturn.bind(this)),
        new TestScenario('Test::CharOutParam', this.runCharOutParam.bind(this)),
        new TestScenario('Test::CharOutParam2', this.runCharOutParam2.bind(this)),
        new TestScenario('Test::CharOutParamWithReturn', this.runCharOutParamWithReturn.bind(this)),
        new TestScenario('Test::NumericOutParam', this.runNumericOutParam.bind(this)),
        new TestScenario('Test::NumericOutParam2', this.runNumericOutParam2.bind(this)),
        new TestScenario('Test::NumericOutParamWithReturn', this.runNumericOutParamWithReturn.bind(this)),
        new TestScenario('Test::StringOutParam', this.runStringOutParam.bind(this)),
        new TestScenario('Test::StringOutParam2', this.runStringOutParam2.bind(this)),
        new TestScenario('Test::StringOutParamWithReturn', this.runStringOutParamWithReturn.bind(this)),






        new TestScenario('ITests::Simple', this.runSimple.bind(this)),
        new TestScenario('ITests::Param1', this.runParam1.bind(this)),
        new TestScenario('ITests::Param2', this.runParam2.bind(this)),
        new TestScenario('ITests::Param3', this.runParam3.bind(this)),
        new TestScenario('ITests::Param4', this.runParam4.bind(this)),
        new TestScenario('ITests::Param5', this.runParam5.bind(this)),
        new TestScenario('ITests::Param6', this.runParam6.bind(this)),
        new TestScenario('ITests::Param7', this.runParam7.bind(this)),
        new TestScenario('ITests::Param8', this.runParam8.bind(this)),
        new TestScenario('ITests::Param9', this.runParam9.bind(this)),
        new TestScenario('ITests::Param10', this.runParam10.bind(this)),
        new TestScenario('ITests::Param11', this.runParam11.bind(this)),
        new TestScenario('ITests::Param12', this.runParam12.bind(this)),
        new TestScenario('ITests::Param13', this.runParam13.bind(this)),
        new TestScenario('ITests::Param14', this.runParam14.bind(this)),
        new TestScenario('ITests::Param15', this.runParam15.bind(this)),
        // new TestScenario('ITests::Collection1', this.runCollection1.bind(this)),
        new TestScenario('ITests::Event1', this.runEvent1.bind(this)),
        new TestScenario('ITests::Event2', this.runEvent2.bind(this)),
    ];

    runSync(scenario, fn) {
        if (scenario.result != TestResult.NotRun) {
            --this.completedCount;
            if (scenario.result == TestResult.Pass) --this.passCount;
            scenario.result = TestResult.NotRun;
        }

        var result = TestResult.Fail;
        try {
            fn();
            result = TestResult.Pass;
        } catch (e) {
            scenario.failureText = e.message;
        }

        scenario.result = result;
        scenario.emit('completed');
        this.onSingleTestCompleted(result);
    }

    onSingleTestCompleted(result) {
        ++this.completedCount;
        if (result == TestResult.Pass) ++this.passCount;
        this.setState({
            completedCount: this.completedCount,
            passCount: this.passCount,
        });
    }

    boolTestValues = [ true, false ]; // TODO: Assignment with other things that are implicitly convertible to bool?
    boolInvalidValues = [ 'true', 0 ];
    boolArrayTestValues = [[], [true], [true, false, true], this.boolTestValues];
    boolArrayInvalidValues = [true, ['true'], [1], this.boolInvalidValues];

    charTestValues = [ '\0', 'F', '⚾' ];
    charInvalidValues = [ '', 65, 'ABC' ];
    charArrayTestValues = [[], ['F'], ['A', 'B', 'C', 'D', 'E', 'F', 'G'], this.charTestValues];
    charArrayInvalidValues = ['F', 'ABCD', [65], [true], ['ABCD'], this.charInvalidValues];

    u8TestValues = [ -0, 0, 127, 128, 255 ];
    u8InvalidValues = [ NaN, Infinity, -1, 0.5, 256 ];

    u16TestValues = [ -0, 0, 32767, 32768, 65535 ];
    u16InvalidValues = [ NaN, Infinity, -1, 0.5, 65536 ];

    u32TestValues = [ -0, 0, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF ];
    u32InvalidValues = [ NaN, Infinity, -1, 0.5, 0x100000000 ];

    u64TestValues = [ -0, 0, 0x1FFFFFFFFFFFFF, 0xFFFFFFFFFFFFF800 ]; // 64-bit float has 53-bit mantissa (52 explicit)
    u64InvalidValues = [ NaN, Infinity, -1, 0.5, 18446744073709551616 ]; // 1.0p64

    s16TestValues = [ -32768, -0, 0, 32767 ];
    s16InvalidValues = [ NaN, Infinity, -32769, 0.5, 32768 ];

    s32TestValues = [ -0x80000000, -0, 0, 0x7FFFFFFF ];
    s32InvalidValues = [ NaN, Infinity, -0x80000001, 0.5, 0x80000000 ];
    numericArrayTestValues = [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], this.s32TestValues];
    numericArrayInvalidValues = [42, ['A'], [true], ['42'], this.s32InvalidValues];

    s64TestValues = [ -0x7FFFFFFFFFFFFC00, -0x1FFFFFFFFFFFFF, -0, 0, 0x1FFFFFFFFFFFFF, 0x7FFFFFFFFFFFFC00 ];
    s64InvalidValues = [ NaN, Infinity, -9223372036854777856, 0.5, 9223372036854775808 ];

    f32TestValues = [ Infinity, -3.40282346638528859811704183485e+38, -1.40129846432481707092372958329e-45, -0, 0, 1.40129846432481707092372958329e-45, 3.40282346638528859811704183485e+38 ];
    f32InvalidValues = [ -3.40282346638528897590636046442e+38, -1.40129846432481691534834763676e-45, 1.40129846432481691534834763676e-45, 3.40282346638528897590636046442e+38 ];

    f64TestValues = [ Infinity, -1.79769313486231570814527423732e+308, -4.94065645841246544176568792868e-324, -0, 0, 4.94065645841246544176568792868e-324, 1.79769313486231570814527423732e+308 ];

    stringTestValues = [ '', '\0', 'foo', 'bar', 'foo\0bar', JSON.stringify(this) ];
    stringArrayTestValues = [[], ['foo'], ['', '\0', 'foo', 'bar', 'foo\0bar'], this.stringTestValues];
    stringArrayInvalidValues = ['foo', [true], [42]];

    // NOTE: Guids seem to always come back as uppercase. This assumption makes for easier comparisons, but is probably not guaranteed
    guidTestValues = [ '00000000-0000-0000-0000-000000000000', 'FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF', '01234567-89AB-CDEF-0123-456789ABCDEF' ];
    guidInvalidValues = [ '', '{00000000-0000-0000-0000-000000000000}', '00000000 0000 0000 0000 000000000000', '01234567-89AB-CDEF-GHIJ-KLMNOPQRSTUV' ];
    guidArrayTestValues = [[], [this.guidTestValues[0]], this.guidTestValues];
    guidArrayInvalidValues = [this.guidTestValues[0], [this.guidInvalidValues[0]], this.guidInvalidValues];

    // NOTE: Enums are just represented as numbers
    enumTestValues = [ 1, TestComponent.TestEnum.first, TestComponent.TestEnum.second, TestComponent.TestEnum.third, TestComponent.TestEnum.fourth ];
    enumInvalidValues = [ true, 'First' ];
    enumArrayTestValues = [[], [this.enumTestValues[1]], this.enumTestValues];
    enumArrayInvalidValues = [this.enumTestValues[1], [this.enumInvalidValues[0]], this.enumInvalidValues];

    numericTypesTestValues = [
        { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
        { u8: 255, u16: 65535, u32: 0xFFFFFFFF, u64: 0xFFFFFFFFFFFFF800, s16: -32768, s32: -0x80000000, s64: -0x7FFFFFFFFFFFFC00, f32: 9.80000019073486328125, f64: 6.67408e-11, e: TestComponent.TestEnum.second },
    ];
    numericTypesInvalidValues = [
        {},
        { u8: '8', u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.14159, f64: 6.02e23, e: TestComponent.TestEnum.first },
        { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.14159, f64: 6.02e23, e: TestComponent.TestEnum.first, extraProperty: 'This should not be here' },
        { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.14159, f64: 6.02e23 },
    ];

    stringTypesTestValues = [
        { ch: '\0', str: '\0', guid: '00000000-0000-0000-0000-000000000000' },
        { ch: 'F', str: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' }
    ];
    stringTypesInvalidValues = [
        {},
        { ch: 65, str: 'Test', guid: '00000000-0000-0000-0000-000000000000' },
        { ch: 'A', str: 'Test', guid: '00000000-0000-0000-0000-000000000000', extraProperty: 42 },
        { ch: 'A', str: 'Test' }
    ];

    boolTypesTestValues = [ { b: true }, { b: false } ];
    boolTypesInvalidValues = [ {}, { b: 'true' }, { b: 1 }, { b: true, extraProperty: 42 } ];

    compositeTestValues = [
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: '00000000-0000-0000-0000-000000000000' },
            bools: { b: true }
        },
        {
            numerics: { u8: 255, u16: 65535, u32: 0xFFFFFFFF, u64: 0xFFFFFFFFFFFFF800, s16: -32768, s32: -0x80000000, s64: -0x7FFFFFFFFFFFFC00, f32: 9.80000019073486328125, f64: 6.67408e-11, e: TestComponent.TestEnum.second },
            strings: { ch: 'F', str: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
            bools: { b: false }
        }
    ];
    compositeInvalidValues = [
        {}, { numerics: {}, strings: {}, bools: {} },
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: '00000000-0000-0000-0000-000000000000' }
        },
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: '00000000-0000-0000-0000-000000000000' },
            bools: { b: true },
            extraProperty: 'This should not be here'
        },
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: '00000000-0000-0000-0000-000000000000' },
            bools: { b: true, extraProperty: 'This should not be here' }
        }
    ];
    compositeArrayTestValues = [[], [this.compositeTestValues[0]], this.compositeTestValues];
    compositeArrayInvalidValues = [this.compositeTestValues[0], [this.compositeInvalidValues[0]], this.compositeInvalidValues];

    runSyncPropertyTest(scenario, type, vals, invalidVals, get, set) {
        this.runSync(scenario, () => {
            var initial = get();
            assertEqual(typeof(initial), type);

            for (var val of vals) {
                var assignedVal;
                assignedVal = set(val);
                assertEqual(get(), val);
                assertEqual(assignedVal, val);
            }

            for (var val of invalidVals) {
                try {
                    set(val);
                }
                catch {}
                assertNotEqual(get(), val);
            }
        });
    }

    // Static properties
    runStaticBoolProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'boolean', this.boolTestValues, [], () => TestComponent.Test.staticBoolProperty, (val) => TestComponent.Test.staticBoolProperty = val);
    }

    // Static functions
    runStaticOr(scenario) {
        this.runSync(scenario, () => {
            assertEqual(TestComponent.Test.staticOr(false, false), false);
            assertEqual(TestComponent.Test.staticOr(false, true), true);
            assertEqual(TestComponent.Test.staticOr(true, false), true);
            assertEqual(TestComponent.Test.staticOr(true, true), true);
        });
    }

    runStaticAdd(scenario) {
        this.runSync(scenario, () => {
            assertEqual(TestComponent.Test.staticAdd(0, 0), 0);
            assertEqual(TestComponent.Test.staticAdd(0, 42), 42);
            assertEqual(TestComponent.Test.staticAdd(42, 0), 42);
            assertEqual(TestComponent.Test.staticAdd(42, 42), 84);
            assertEqual(TestComponent.Test.staticAdd(42, -42), 0);
        })
    }

    runStaticAppend(scenario) {
        this.runSync(scenario, () => {
            assertEqual(TestComponent.Test.staticAppend('foo', '\0', 'bar'), 'foo\0bar');
            assertEqual(TestComponent.Test.staticAppend('Hello', ' ', 'world'), 'Hello world');
        })
    }

    // Static out params
    runStaticBoolOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                assertEqual(TestComponent.Test.staticBoolOutParam(lhs, rhs).and, lhs && rhs);
            };
            run(false, false);
            run(false, true);
            run(true, false);
            run(true, true);
        });
    }

    runStaticBoolOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                var { and, or } = TestComponent.Test.staticBoolOutParam2(lhs, rhs);
                assertEqual(and, lhs && rhs);
                assertEqual(or, lhs || rhs);
            };
            run(false, false);
            run(false, true);
            run(true, false);
            run(true, true);
        });
    }

    runStaticBoolOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                var { returnValue, and, or } = TestComponent.Test.staticBoolOutParamWithReturn(lhs, rhs);
                assertEqual(and, lhs && rhs);
                assertEqual(or, lhs || rhs);
                assertEqual(returnValue, lhs ^ rhs ? true : false);
            };
            run(false, false);
            run(false, true);
            run(true, false);
            run(true, true);
        });
    }

    runStaticCharOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (ch) => {
                var { next } = TestComponent.Test.staticCharOutParam(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
            };
            run('b');
            run('y');
        });
    }

    runStaticCharOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (ch) => {
                var { next, prev } = TestComponent.Test.staticCharOutParam2(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
                assertEqual(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
            };
            run('b');
            run('y');
        });
    }

    runStaticCharOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (ch) => {
                var { returnValue, next, prev } = TestComponent.Test.staticCharOutParamWithReturn(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
                assertEqual(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
                assertEqual(returnValue, ch.toUpperCase());
            };
            run('b');
            run('y');
        });
    }

    runStaticNumericOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                assertEqual(TestComponent.Test.staticNumericOutParam(val).doubledValue, val * 2);
            };
            run(0);
            run(42);
        });
    }

    runStaticNumericOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { doubledValue, tripledValue } = TestComponent.Test.staticNumericOutParam2(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
            };
            run(0);
            run(42);
        });
    }

    runStaticNumericOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, doubledValue, tripledValue } = TestComponent.Test.staticNumericOutParamWithReturn(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
                assertEqual(returnValue, val * 4);
            };
            run(0);
            run(42);
        });
    }

    runStaticStringOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                assertEqual(TestComponent.Test.staticStringOutParam(val).lower, val.toLowerCase());
            };
            run('');
            run('\0');
            run('Foo');
            run('Foo\0Bar');
        });
    }

    runStaticStringOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { lower, upper } = TestComponent.Test.staticStringOutParam2(val);
                assertEqual(lower, val.toLowerCase());
                assertEqual(upper, val.toUpperCase());
            };
            run('');
            run('\0');
            run('Foo');
            run('Foo\0Bar');
        });
    }

    runStaticStringOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, lower, upper } = TestComponent.Test.staticStringOutParamWithReturn(val);
                assertEqual(lower, val.toLowerCase());
                assertEqual(upper, val.toUpperCase());
                assertEqual(returnValue, val.split('').reverse().join(''));
            };
            run('');
            run('\0');
            run('Foo');
            run('Foo\0Bar');
        });
    }

    // Non-static properties
    runBoolProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'boolean', this.boolTestValues, this.boolInvalidValues, () => this.test.boolProperty, (val) => this.test.boolProperty = val);
    }

    runCharProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'string', this.charTestValues, this.charInvalidValues, () => this.test.charProperty, (val) => this.test.charProperty = val);
    }

    runU8Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.u8TestValues, this.u8InvalidValues, () => this.test.u8Property, (val) => this.test.u8Property = val);
    }

    runU16Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.u16TestValues, this.u16InvalidValues, () => this.test.u16Property, (val) => this.test.u16Property = val);
    }

    runU32Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.u32TestValues, this.u32InvalidValues, () => this.test.u32Property, (val) => this.test.u32Property = val);
    }

    runU64Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.u64TestValues, this.u64InvalidValues, () => this.test.u64Property, (val) => this.test.u64Property = val);
    }

    runS16Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.s16TestValues, this.s16InvalidValues, () => this.test.s16Property, (val) => this.test.s16Property = val);
    }

    runS32Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.s32TestValues, this.s32InvalidValues, () => this.test.s32Property, (val) => this.test.s32Property = val);
    }

    runS64Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.s64TestValues, this.s64InvalidValues, () => this.test.s64Property, (val) => this.test.s64Property = val);
    }

    runF32Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.f32TestValues, this.f32InvalidValues, () => this.test.f32Property, (val) => this.test.f32Property = val);
    }

    runF64Property(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.f64TestValues, [], () => this.test.f64Property, (val) => this.test.f64Property = val);
    }

    runStringProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'string', this.stringTestValues, [], () => this.test.stringProperty, (val) => this.test.stringProperty = val);
    }

    runGuidProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'string', this.guidTestValues, this.guidInvalidValues, () => this.test.guidProperty, (val) => this.test.guidProperty = val);
    }

    runEnumProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'number', this.enumTestValues, this.enumInvalidValues, () => this.test.enumProperty, (val) => this.test.enumProperty = val);
    }

    runNumericsStructProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.numericTypesTestValues, this.numericTypesInvalidValues, () => this.test.numericsStructProperty, (val) => this.test.numericsStructProperty = val);
    }

    runStringsStructProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.stringTypesTestValues, this.stringTypesInvalidValues, () => this.test.stringsStructProperty, (val) => this.test.stringsStructProperty = val);
    }

    runBooleansStructProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.boolTypesTestValues, this.boolTypesInvalidValues, () => this.test.booleansStructProperty, (val) => this.test.booleansStructProperty = val);
    }

    runCompositeStructProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.compositeTestValues, this.compositeInvalidValues, () => this.test.compositeStructProperty, (val) => this.test.compositeStructProperty = val);
    }

    runRefBooleanProperty(scenario) {
        this.runSync(scenario, () => {
            try {
                var initial = this.test.refBooleanProperty;
                assertEqual(typeof(initial), 'object'); // Should start as null, which is an object
                assertEqual(initial, null);

                for (var val of this.boolTestValues) {
                    var assignedVal;
                    assignedVal = this.test.refBooleanProperty = val;
                    assertEqual(this.test.refBooleanProperty, val);
                    assertEqual(assignedVal, val);
                }
            } finally {
                this.test.refBooleanProperty = null; // In case the test gets run again
            }
        });
    }

    runRefCharProperty(scenario) {
        this.runSync(scenario, () => {
            try {
                var initial = this.test.refCharProperty;
                assertEqual(typeof(initial), 'object'); // Should start as null, which is an object
                assertEqual(initial, null);

                for (var val of this.charTestValues) {
                    var assignedVal;
                    assignedVal = this.test.refCharProperty = val;
                    assertEqual(this.test.refCharProperty, val);
                    assertEqual(assignedVal, val);
                }
            } finally {
                this.test.refCharProperty = null; // In case the test gets run again
            }
        });
    }

    runRefNumericProperty(scenario) {
        this.runSync(scenario, () => {
            try {
                var initial = this.test.refNumericProperty;
                assertEqual(typeof(initial), 'object'); // Should start as null, which is an object
                assertEqual(initial, null);

                for (var val of this.s32TestValues) {
                    var assignedVal;
                    assignedVal = this.test.refNumericProperty = val;
                    assertEqual(this.test.refNumericProperty, val);
                    assertEqual(assignedVal, val);
                }
            } finally {
                this.test.refNumericProperty = null; // In case the test gets run again
            }
        });
    }

    runRefEnumProperty(scenario) {
        this.runSync(scenario, () => {
            try {
                var initial = this.test.refEnumProperty;
                assertEqual(typeof(initial), 'object'); // Should start as null, which is an object
                assertEqual(initial, null);

                for (var val of this.enumTestValues) {
                    var assignedVal;
                    assignedVal = this.test.refEnumProperty = val;
                    assertEqual(this.test.refEnumProperty, val);
                    assertEqual(assignedVal, val);
                }
            } finally {
                this.test.refEnumProperty = null; // In case the test gets run again
            }
        });
    }

    runObjectProperty(scenario) {
        this.runSync(scenario, () => {
            try {
                var prev = this.test.objectProperty;
                assertEqual(typeof(prev), 'object'); // Should start as null, which is also an object
                assertEqual(prev, null);

                for (var val of this.numericArrayTestValues) {
                    var vector = TestComponent.Test.makeNumericVector(val);
                    var assignedVal;
                    assignedVal = this.test.objectProperty = vector;
                    assertEqual(typeof(this.test.objectProperty), 'object');
                    assert(this.test.objectProperty != prev);
                    assert(this.test.objectProperty == vector);
                    assert(assignedVal == vector);
                    prev = vector;
                }
            } finally {
                this.test.objectProperty = null; // In case the test gets run again
            }
        });
    }

    // Non-static array properties
    runBooleanArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.boolArrayTestValues, this.boolArrayInvalidValues, () => this.test.booleanArrayProperty, (val) => this.test.booleanArrayProperty = val);
    }

    runCharArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.charArrayTestValues, this.charArrayInvalidValues, () => this.test.charArrayProperty, (val) => this.test.charArrayProperty = val);
    }

    runNumericArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.numericArrayTestValues, this.numericArrayInvalidValues, () => this.test.numericArrayProperty, (val) => this.test.numericArrayProperty = val);
    }

    runStringArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.stringArrayTestValues, this.stringArrayInvalidValues, () => this.test.stringArrayProperty, (val) => this.test.stringArrayProperty = val);
    }

    runGuidArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.guidArrayTestValues, this.guidArrayInvalidValues, () => this.test.guidArrayProperty, (val) => this.test.guidArrayProperty = val);
    }

    runEnumArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.enumArrayTestValues, this.enumArrayInvalidValues, () => this.test.enumArrayProperty, (val) => this.test.enumArrayProperty = val);
    }

    runCompositeStructArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object', this.compositeArrayTestValues, this.compositeArrayInvalidValues, () => this.test.compositeStructArrayProperty, (val) => this.test.compositeStructArrayProperty = val);
    }

    runRefArrayPropertyArrayProperty(scenario) {
        this.runSyncPropertyTest(scenario, 'object',
            [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], this.s32TestValues],
            [42, ['A'], [true], ['42'], this.s32InvalidValues],
            () => this.test.refArrayProperty, (val) => this.test.refArrayProperty = val);
    }

    runObjectArrayProperty(scenario) {
        this.runSync(scenario, () => {
            var array = [];
            for (var val of this.numericArrayTestValues) {
                array.push(TestComponent.Test.makeNumericVector(val));
            }

            this.test.objectArrayProperty = array;
            var curr = this.test.objectArrayProperty;
            assertEqual(curr.length, array.length);
            for (var i = 0; i < array.length; ++i) {
                assert(curr[i] == array[i]);
            }
        });
    }

    // Non-static functions
    runOr(scenario) {
        this.runSync(scenario, () => {
            assertEqual(this.test.or(false, false), false);
            assertEqual(this.test.or(false, true), true);
            assertEqual(this.test.or(true, false), true);
            assertEqual(this.test.or(true, true), true);
        });
    }

    runAdd(scenario) {
        this.runSync(scenario, () => {
            assertEqual(this.test.add(0, 0), 0);
            assertEqual(this.test.add(0, 42), 42);
            assertEqual(this.test.add(42, 0), 42);
            assertEqual(this.test.add(42, 42), 84);
            assertEqual(this.test.add(42, -42), 0);
        })
    }

    runAppend(scenario) {
        this.runSync(scenario, () => {
            assertEqual(this.test.append('foo', '\0', 'bar'), 'foo\0bar');
            assertEqual(this.test.append('Hello', ' ', 'world'), 'Hello world');
        })
    }

    // Static out params
    runBoolOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                assertEqual(this.test.boolOutParam(lhs, rhs).and, lhs && rhs);
            };
            run(false, false);
            run(false, true);
            run(true, false);
            run(true, true);
        });
    }

    runBoolOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                var { and, or } = this.test.boolOutParam2(lhs, rhs);
                assertEqual(and, lhs && rhs);
                assertEqual(or, lhs || rhs);
            };
            run(false, false);
            run(false, true);
            run(true, false);
            run(true, true);
        });
    }

    runBoolOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                var { returnValue, and, or } = this.test.boolOutParamWithReturn(lhs, rhs);
                assertEqual(and, lhs && rhs);
                assertEqual(or, lhs || rhs);
                assertEqual(returnValue, lhs ^ rhs ? true : false);
            };
            run(false, false);
            run(false, true);
            run(true, false);
            run(true, true);
        });
    }

    runCharOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (ch) => {
                var { next } = this.test.charOutParam(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
            };
            run('b');
            run('y');
        });
    }

    runCharOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (ch) => {
                var { next, prev } = this.test.charOutParam2(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
                assertEqual(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
            };
            run('b');
            run('y');
        });
    }

    runCharOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (ch) => {
                var { returnValue, next, prev } = this.test.charOutParamWithReturn(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
                assertEqual(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
                assertEqual(returnValue, ch.toUpperCase());
            };
            run('b');
            run('y');
        });
    }

    runNumericOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                assertEqual(this.test.numericOutParam(val).doubledValue, val * 2);
            };
            run(0);
            run(42);
        });
    }

    runNumericOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { doubledValue, tripledValue } = this.test.numericOutParam2(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
            };
            run(0);
            run(42);
        });
    }

    runNumericOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, doubledValue, tripledValue } = this.test.numericOutParamWithReturn(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
                assertEqual(returnValue, val * 4);
            };
            run(0);
            run(42);
        });
    }

    runStringOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                assertEqual(this.test.stringOutParam(val).lower, val.toLowerCase());
            };
            run('');
            run('\0');
            run('Foo');
            run('Foo\0Bar');
        });
    }

    runStringOutParam2(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { lower, upper } = this.test.stringOutParam2(val);
                assertEqual(lower, val.toLowerCase());
                assertEqual(upper, val.toUpperCase());
            };
            run('');
            run('\0');
            run('Foo');
            run('Foo\0Bar');
        });
    }

    runStringOutParamWithReturn(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, lower, upper } = this.test.stringOutParamWithReturn(val);
                assertEqual(lower, val.toLowerCase());
                assertEqual(upper, val.toUpperCase());
                assertEqual(returnValue, val.split('').reverse().join(''));
            };
            run('');
            run('\0');
            run('Foo');
            run('Foo\0Bar');
        });
    }






    runSimple(scenario) {
        this.runSync(scenario, () => this.tests.simple());
    }

    runParam1(scenario) {
        this.runSync(scenario, () => {
            var { returnValue, b } = this.tests.param1(true); // Boolean
            assertEqual(returnValue, true);
            assertEqual(b, true);

            var { returnValue, b } = this.tests.param1(false);
            assertEqual(returnValue, false);
            assertEqual(b, false);
        });
    }

    runParam2(scenario) {
        this.runSync(scenario, () => {
            var vals = [ 0, 127, 128, 255 ]; // UInt8
            for (var val of vals) {
                var { returnValue, b } = this.tests.param2(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            vals = [ -1, 256 ];
            for (var val of vals) {
                var { returnValue, b } = this.tests.param2(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam3(scenario) {
        this.runSync(scenario, () => {
            var vals = [ 0, 32767, 32768, 65535 ]; // UInt16
            for (var val of vals) {
                var { returnValue, b } = this.tests.param3(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            vals = [ -1, 65536 ];
            for (var val of vals) {
                var { returnValue, b } = this.tests.param3(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam4(scenario) {
        this.runSync(scenario, () => {
            var vals = [ 0, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF ]; // UInt32
            for (var val of vals) {
                var { returnValue, b } = this.tests.param4(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            vals = [ -1, 0x100000000 ];
            for (var val of vals) {
                var { returnValue, b } = this.tests.param4(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam5(scenario) {
        this.runSync(scenario, () => {
            // NOTE: This function accepts/returns 64-bit unsigned integers, however JS only supports IEEE 64-bit
            // floating point numbers, which have a 53-bit mantissa (52 bits explicit)
            var vals = [ 0, 0x1FFFFFFFFFFFFF ];
            for (var val of vals) {
                var { returnValue, b } = this.tests.param5(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            vals = [ -1, 1.8446744073709552e19 ]; // Exponent = 64
            for (var val of vals) {
                var { returnValue, b } = this.tests.param5(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam6(scenario) {
        this.runSync(scenario, () => {
            var vals = [ -32768, 0, 32767 ]; // Int16
            for (var val of vals) {
                var { returnValue, b } = this.tests.param6(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            vals = [ -32769, 32768 ];
            for (var val of vals) {
                var { returnValue, b } = this.tests.param6(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam7(scenario) {
        this.runSync(scenario, () => {
            var vals = [ -0x80000000, 0, 0x7FFFFFFF ]; // Int32
            for (var val of vals) {
                var { returnValue, b } = this.tests.param7(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            vals = [ -0x80000001, 0x80000000 ];
            for (var val of vals) {
                var { returnValue, b } = this.tests.param7(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam8(scenario) {
        this.runSync(scenario, () => {
            var vals = [ -0x1FFFFFFFFFFFFF, 0, 0x1FFFFFFFFFFFFF ]; // Int64
            for (var val of vals) {
                var { returnValue, b } = this.tests.param8(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            vals = [ -1.8446744073709552e19, 1.8446744073709552e19 ]; // Exponent = 64
            for (var val of vals) {
                var { returnValue, b } = this.tests.param8(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam9(scenario) {
        this.runSync(scenario, () => {
            var vals = [0, -0, Infinity, -Infinity, 1.401298464324817e-45, -1.401298464324817e-45, 3.402820018375656e+38, -3.402820018375656e+38]; // Single
            for (var val of vals) {
                var { returnValue, b } = this.tests.param9(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            // Equality for NaN does not work in JS
            var { returnValue, b } = this.tests.param9(NaN);
            assert(isNaN(returnValue));
            assert(isNaN(b));

            vals = [ 5e-324, 1.7976931348623157e+308 ];
            for (var val of vals) {
                var { returnValue, b } = this.tests.param9(val);
                assertNotEqual(returnValue, val);
                assertNotEqual(b, val);
            }
        });
    }

    runParam10(scenario) {
        this.runSync(scenario, () => {
            var vals = [0, -1, Infinity, -Infinity, 1.401298464324817e-45, 3.402820018375656e+38, 5e-324, -5e-324, 1.7976931348623157e+308, -1.7976931348623157e+308]; // Double
            for (var val of vals) {
                var { returnValue, b } = this.tests.param10(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }

            // Equality for NaN does not work in JS
            var { returnValue, b } = this.tests.param10(NaN);
            assert(isNaN(returnValue));
            assert(isNaN(b));
        });
    }

    runParam11(scenario) {
        this.runSync(scenario, () => {
            var vals = ['a', 'b', 'c', '⚾']; // Char16
            for (var val of vals) {
                var { returnValue, b } = this.tests.param11(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }
        });
    }

    runParam12(scenario) {
        this.runSync(scenario, () => {
            var vals = ['', '\0', 'foobar', 'foo\0bar']; // String
            for (var val of vals) {
                var { returnValue, b } = this.tests.param12(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }
        });
    }

    runParam13(scenario) {
        this.runSync(scenario, () => {
            var blittable = {
                a: 8, // UInt8
                b: 16, // UInt16
                c: 32, // UInt32
                d: 64, // UInt64
                e: -16, // Int16
                f: -32, // Int32
                g: -64, // Int64
                h: 42.5, // Single
                i: -42.5, // Double
                j: '0A26AE10-CDFA-5690-993B-6150412DE4E9', // Guid
            };
            var { returnValue, c } = this.tests.param13(blittable, blittable);
            assertEqual(returnValue, blittable);
            assertEqual(c, blittable);

            var threw = false;
            try { this.tests.param13({}, {}) }
            catch { threw = true; }
            assert(threw);
        });
    }

    runParam14(scenario) {
        this.runSync(scenario, () => {
            var nonBlittable = {
                a: true, // Boolean
                b: 'F', // Char16
                c: 'test', // String
                d: 42, // IReference<int>
            };
            var { returnValue, c } = this.tests.param14(nonBlittable, nonBlittable);
            assertEqual(returnValue, nonBlittable);
            assertEqual(c, nonBlittable);
        });
    }

    runParam15(scenario) {
        this.runSync(scenario, () => {
            var nested = {
                blittable: {
                    a: 8, // UInt8
                    b: 16, // UInt16
                    c: 32, // UInt32
                    d: 64, // UInt64
                    e: -16, // Int16
                    f: -32, // Int32
                    g: -64, // Int64
                    h: 42.5, // Single
                    i: -42.5, // Double
                    j: '0A26AE10-CDFA-5690-993B-6150412DE4E9', // Guid
                },
                nonBlittable: {
                    a: true, // Boolean
                    b: 'F', // Char16
                    c: 'test', // String
                    d: 42, // IReference<int>
                }
            };
            var { returnValue, c } = this.tests.param15(nested, nested);
            assertEqual(returnValue, nested);
            assertEqual(c, nested);
        });
    }

    /* TODO: Collections support
    runCollection1(scenario) {
        this.runSync(scenario, () => {
            var vals = [
                [],
                ['foo'],
                ['', 'foo', 'bar', 'foobar', 'foo\0bar'],
            ];
            for (var arr of vals) {
                var { returnValue, c } = this.tests.collection1(arr);
                assertEqual(returnValue.length, arr.length);
                assertEqual(c.length, arr.length);
            }
        });
    }
    */

    runEvent1(scenario) {
        this.runSync(scenario, () => {
            var eventVal; // Int32
            var callback = (sender, val) => { eventVal = val; };
            this.tests.addEventListener('event1', callback);

            this.tests.event1Call(0);
            assertEqual(eventVal, 0);

            this.tests.event1Call(42);
            assertEqual(eventVal, 42);

            this.tests.removeEventListener('event1', callback);
            this.tests.event1Call(8);
            assertEqual(eventVal, 42);

            // The event handler should ignore the error
            callback = () => { throw new Error('Thrown from JS')};
            this.tests.addEventListener('event1', callback);
            this.tests.event1Call(8);
            this.tests.removeEventListener('event1', callback);
        });
    }

    runEvent2(scenario) {
        this.runSync(scenario, () => {
            var eventVal; // Int32
            var callback = (sender, val) => { eventVal = val; };
            this.tests.addEventListener('event2', callback);

            this.tests.event2Call(0);
            assertEqual(eventVal, 0);

            this.tests.event2Call(42);
            assertEqual(eventVal, 42);

            this.tests.removeEventListener('event2', callback);
            this.tests.event2Call(8);
            assertEqual(eventVal, 42);

            // The event handler should ignore the error
            callback = () => { throw new Error('Thrown from JS')};
            this.tests.addEventListener('event2', callback);
            this.tests.event2Call(8);
            this.tests.removeEventListener('event2', callback);
        });
    }

    inProgress() {
        return this.state.completedCount != this.scenarios.length;
    }

    passed() {
        return this.state.passCount == this.scenarios.length;
    }

    resultText() {
        return this.inProgress() ? 'In Progress' :
            this.passed() ? 'Pass' : 'Fail';
    }

    textStyle() {
        return this.inProgress() ? styles.progressText :
            this.passed() ? styles.passText : styles.failText;
    }

    componentDidMount() {
        for (var scenario of this.scenarios) {
            scenario.invoke(scenario);
        }
    }

    render() {
        return (
            <SafeAreaView style={{ flex: 1, backgroundColor: 'white' }}>
                <View style={styles.headerBanner}>
                    <Text style={{ fontSize: 22 }}>WinRT Projection Tests</Text>
                </View>
                <ScrollView style={styles.scrollView}>
                    <View style={[styles.listEntry, { alignSelf: 'center' }]}>
                        <Text style={[styles.rowItem]}>Pass Rate: {this.state.passCount * 100 / this.state.completedCount}%</Text>
                        <Text style={[styles.rowItem, this.textStyle()]}>{this.resultText()}</Text>
                    </View>
                    {
                        this.scenarios.map((scenario, idx) => (
                            <Scenario key={scenario.name} scenario={scenario} />
                        ))
                    }
                </ScrollView>
            </SafeAreaView>
        );
    }
}

export default App;
