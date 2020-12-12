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

function guidFromString(str) {
    // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
    assertEqual(str.length, 36);
    assertEqual(str.charAt(8), '-');
    assertEqual(str.charAt(13), '-');
    assertEqual(str.charAt(18), '-');
    assertEqual(str.charAt(23), '-');
    return {
        data1: parseInt(str.substr(0, 8), 16),
        data2: parseInt(str.substr(9, 4), 16),
        data3: parseInt(str.substr(14, 4), 16),
        data4: [
            parseInt(str.substr(19, 2), 16),
            parseInt(str.substr(21, 2), 16),
            parseInt(str.substr(24, 2), 16),
            parseInt(str.substr(26, 2), 16),
            parseInt(str.substr(28, 2), 16),
            parseInt(str.substr(30, 2), 16),
            parseInt(str.substr(32, 2), 16),
            parseInt(str.substr(34, 2), 16),
        ]
    };
}

function makeGuid(data1, data2, data3, data4) {
    var result = '';
    var append = (val, len) => {
        var str = val.toString(16).toUpperCase();
        while (str.length < len) str = '0' + str;
        result += str;
    };

    append(data1, 8);
    result += '-';
    append(data2, 4);
    result += '-';
    append(data3, 4);
    result += '-';
    append(data4[0], 2);
    append(data4[1], 2);
    result += '-';
    append(data4[2], 2);
    append(data4[3], 2);
    append(data4[4], 2);
    append(data4[5], 2);
    append(data4[6], 2);
    append(data4[7], 2);

    return result;
}

function stringFromGuid(guid) {
    return makeGuid(guid.data1, guid.data2, guid.data3, guid.data4);
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
    state = { completedCount: 0, passCount: 0 };
    completedCount = 0;
    passCount = 0;

    scenarios = [
        // Static properties
        // TODO: Figure out why static properties are not working: https://github.com/microsoft/jswinrt/issues/7
        // new TestScenario('Test::StaticBoolProperty', this.runStaticBoolProperty.bind(this)),

        // Static functions
        new TestScenario('Test::StaticOr', this.runStaticOr.bind(this)),
        new TestScenario('Test::StaticOrAll', this.runStaticOrAll.bind(this)),
        new TestScenario('Test::StaticAdd', this.runStaticAdd.bind(this)),
        new TestScenario('Test::StaticAddAll', this.runStaticAddAll.bind(this)),
        new TestScenario('Test::StaticAppend', this.runStaticAppend.bind(this)),
        new TestScenario('Test::StaticAppendAll', this.runStaticAppendAll.bind(this)),

        // Static out params
        new TestScenario('Test::StaticBoolOutParam', this.runStaticBoolOutParam.bind(this)),
        new TestScenario('Test::StaticCharOutParam', this.runStaticCharOutParam.bind(this)),
        new TestScenario('Test::StaticNumericOutParam', this.runStaticNumericOutParam.bind(this)),
        new TestScenario('Test::StaticStringOutParam', this.runStaticStringOutParam.bind(this)),
        new TestScenario('Test::StaticGuidOutParam', this.runStaticGuidOutParam.bind(this)),
        new TestScenario('Test::StaticEnumOutParam', this.runStaticEnumOutParam.bind(this)),
        new TestScenario('Test::StaticCompositeStructOutParam', this.runStaticCompositeStructOutParam.bind(this)),
        new TestScenario('Test::StaticRefOutParam', this.runStaticRefOutParam.bind(this)),
        new TestScenario('Test::StaticObjectOutParam', this.runStaticObjectOutParam.bind(this)),

        // Static array out params
        new TestScenario('Test::StaticBoolArrayOutParam', this.runStaticBoolArrayOutParam.bind(this)),
        new TestScenario('Test::StaticCharArrayOutParam', this.runStaticCharArrayOutParam.bind(this)),
        new TestScenario('Test::StaticNumericArrayOutParam', this.runStaticNumericArrayOutParam.bind(this)),
        new TestScenario('Test::StaticStringArrayOutParam', this.runStaticStringArrayOutParam.bind(this)),
        new TestScenario('Test::StaticGuidArrayOutParam', this.runStaticGuidArrayOutParam.bind(this)),
        new TestScenario('Test::StaticEnumArrayOutParam', this.runStaticEnumArrayOutParam.bind(this)),
        new TestScenario('Test::StaticCompositeStructArrayOutParam', this.runStaticCompositeStructArrayOutParam.bind(this)),
        new TestScenario('Test::StaticObjectArrayOutParam', this.runStaticObjectArrayOutParam.bind(this)),

        // Static array fill params
        new TestScenario('Test::StaticBoolFillParam', this.runStaticBoolFillParam.bind(this)),
        new TestScenario('Test::StaticCharFillParam', this.runStaticCharFillParam.bind(this)),
        new TestScenario('Test::StaticNumericFillParam', this.runStaticNumericFillParam.bind(this)),
        new TestScenario('Test::StaticStringFillParam', this.runStaticStringFillParam.bind(this)),
        new TestScenario('Test::StaticGuidFillParam', this.runStaticGuidFillParam.bind(this)),
        new TestScenario('Test::StaticEnumFillParam', this.runStaticEnumFillParam.bind(this)),
        new TestScenario('Test::StaticCompositeStructFillParam', this.runStaticCompositeStructFillParam.bind(this)),
        new TestScenario('Test::StaticObjectFillParam', this.runStaticObjectFillParam.bind(this)),

        // Static events
        new TestScenario('Test::StaticBoolEventHandler', this.runStaticBoolEventHandler.bind(this)),
        new TestScenario('Test::StaticCharEventHandler', this.runStaticCharEventHandler.bind(this)),
        new TestScenario('Test::StaticNumericEventHandler', this.runStaticNumericEventHandler.bind(this)),
        new TestScenario('Test::StaticStringEventHandler', this.runStaticStringEventHandler.bind(this)),
        new TestScenario('Test::StaticGuidEventHandler', this.runStaticGuidEventHandler.bind(this)),
        new TestScenario('Test::StaticEnumEventHandler', this.runStaticEnumEventHandler.bind(this)),
        new TestScenario('Test::StaticCompositeStructEventHandler', this.runStaticCompositeStructEventHandler.bind(this)),
        new TestScenario('Test::StaticRefEventHandler', this.runStaticRefEventHandler.bind(this)),
        new TestScenario('Test::StaticObjectEventHandler', this.runStaticObjectEventHandler.bind(this)),

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
        new TestScenario('Test::OrAll', this.runOrAll.bind(this)),
        new TestScenario('Test::Add', this.runAdd.bind(this)),
        new TestScenario('Test::AddAll', this.runAddAll.bind(this)),
        new TestScenario('Test::Append', this.runAppend.bind(this)),
        new TestScenario('Test::AppendAll', this.runAppendAll.bind(this)),

        // Non-static out params
        new TestScenario('Test::BoolOutParam', this.runBoolOutParam.bind(this)),
        new TestScenario('Test::CharOutParam', this.runCharOutParam.bind(this)),
        new TestScenario('Test::NumericOutParam', this.runNumericOutParam.bind(this)),
        new TestScenario('Test::StringOutParam', this.runStringOutParam.bind(this)),
        new TestScenario('Test::GuidOutParam', this.runGuidOutParam.bind(this)),
        new TestScenario('Test::EnumOutParam', this.runEnumOutParam.bind(this)),
        new TestScenario('Test::CompositeStructOutParam', this.runCompositeStructOutParam.bind(this)),
        new TestScenario('Test::RefOutParam', this.runRefOutParam.bind(this)),
        new TestScenario('Test::ObjectOutParam', this.runObjectOutParam.bind(this)),

        // Non-static array out params
        new TestScenario('Test::BoolArrayOutParam', this.runBoolArrayOutParam.bind(this)),
        new TestScenario('Test::CharArrayOutParam', this.runCharArrayOutParam.bind(this)),
        new TestScenario('Test::NumericArrayOutParam', this.runNumericArrayOutParam.bind(this)),
        new TestScenario('Test::StringArrayOutParam', this.runStringArrayOutParam.bind(this)),
        new TestScenario('Test::GuidArrayOutParam', this.runGuidArrayOutParam.bind(this)),
        new TestScenario('Test::EnumArrayOutParam', this.runEnumArrayOutParam.bind(this)),
        new TestScenario('Test::CompositeStructArrayOutParam', this.runCompositeStructArrayOutParam.bind(this)),
        new TestScenario('Test::ObjectArrayOutParam', this.runObjectArrayOutParam.bind(this)),

        // Non-static array fill params
        new TestScenario('Test::BoolFillParam', this.runBoolFillParam.bind(this)),
        new TestScenario('Test::CharFillParam', this.runCharFillParam.bind(this)),
        new TestScenario('Test::NumericFillParam', this.runNumericFillParam.bind(this)),
        new TestScenario('Test::StringFillParam', this.runStringFillParam.bind(this)),
        new TestScenario('Test::GuidFillParam', this.runGuidFillParam.bind(this)),
        new TestScenario('Test::EnumFillParam', this.runEnumFillParam.bind(this)),
        new TestScenario('Test::CompositeStructFillParam', this.runCompositeStructFillParam.bind(this)),
        new TestScenario('Test::ObjectFillParam', this.runObjectFillParam.bind(this)),

        // Non-static events
        new TestScenario('Test::BoolEventHandler', this.runBoolEventHandler.bind(this)),
        new TestScenario('Test::CharEventHandler', this.runCharEventHandler.bind(this)),
        new TestScenario('Test::NumericEventHandler', this.runNumericEventHandler.bind(this)),
        new TestScenario('Test::StringEventHandler', this.runStringEventHandler.bind(this)),
        new TestScenario('Test::GuidEventHandler', this.runGuidEventHandler.bind(this)),
        new TestScenario('Test::EnumEventHandler', this.runEnumEventHandler.bind(this)),
        new TestScenario('Test::CompositeStructEventHandler', this.runCompositeStructEventHandler.bind(this)),
        new TestScenario('Test::RefEventHandler', this.runRefEventHandler.bind(this)),
        new TestScenario('Test::ObjectEventHandler', this.runObjectEventHandler.bind(this)),
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
    zeroGuid = '00000000-0000-0000-0000-000000000000';
    allSetGuid = 'FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF';
    guidTestValues = [ this.zeroGuid, this.allSetGuid, '01234567-89AB-CDEF-0123-456789ABCDEF' ];
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
        { ch: '\0', str: '\0', guid: this.zeroGuid },
        { ch: 'F', str: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' }
    ];
    stringTypesInvalidValues = [
        {},
        { ch: 65, str: 'Test', guid: this.zeroGuid },
        { ch: 'A', str: 'Test', guid: this.zeroGuid, extraProperty: 42 },
        { ch: 'A', str: 'Test' }
    ];

    boolTypesTestValues = [ { b: true }, { b: false } ];
    boolTypesInvalidValues = [ {}, { b: 'true' }, { b: 1 }, { b: true, extraProperty: 42 } ];

    compositeTestValues = [
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: this.zeroGuid },
            bools: { b: true }
        },
        {
            numerics: { u8: 255, u16: 65535, u32: 0xFFFFFFFF, u64: 0xFFFFFFFFFFFFF800, s16: -32768, s32: -0x80000000, s64: -0x7FFFFFFFFFFFFC00, f32: 9.80000019073486328125, f64: 6.67408e-11, e: TestComponent.TestEnum.second },
            strings: { ch: 'F', str: 'Hello, world!', guid: this.allSetGuid },
            bools: { b: false }
        },
        {
            numerics: { u8: 0, u16: 0, u32: 0, u64: 0, s16: 0, s32: 0, s64: 0, f32: 0, f64: 0, e: TestComponent.TestEnum.third },
            strings: { ch: '⚾', str: 'foo\0bar', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
            bools: { b: true }
        }
    ];
    compositeInvalidValues = [
        {}, { numerics: {}, strings: {}, bools: {} },
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: this.zeroGuid }
        },
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: this.zeroGuid },
            bools: { b: true },
            extraProperty: 'This should not be here'
        },
        {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, e: TestComponent.TestEnum.first },
            strings: { ch: '\0', str: '\0', guid: this.zeroGuid },
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

    runStaticOrAll(scenario) {
        this.runSync(scenario, () => {
            var run = (arr, expect) => {
                assertEqual(TestComponent.Test.staticOrAll(arr), expect);
            };
            run([], false);
            run([false], false);
            run([true], true);
            run([false, false, false, false], false);
            run([true, false, false, false], true);
            run([false, true, false, false], true);
            run([false, false, true, false], true);
            run([false, false, false, true ], true);
        });
    }

    runStaticAdd(scenario) {
        this.runSync(scenario, () => {
            assertEqual(TestComponent.Test.staticAdd(0, 0), 0);
            assertEqual(TestComponent.Test.staticAdd(0, 42), 42);
            assertEqual(TestComponent.Test.staticAdd(42, 0), 42);
            assertEqual(TestComponent.Test.staticAdd(42, 42), 84);
            assertEqual(TestComponent.Test.staticAdd(42, -42), 0);
        });
    }

    runStaticAddAll(scenario) {
        this.runSync(scenario, () => {
            var run = (arr, expect) => {
                assertEqual(TestComponent.Test.staticAddAll(arr), expect);
            };
            run([], 0);
            run([1], 1);
            run([1, 2, 3], 6);
            run([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 55);
        });
    }

    runStaticAppend(scenario) {
        this.runSync(scenario, () => {
            assertEqual(TestComponent.Test.staticAppend('foo', '\0', 'bar'), 'foo\0bar');
            assertEqual(TestComponent.Test.staticAppend('Hello', ' ', 'world'), 'Hello world');
        })
    }

    runStaticAppendAll(scenario) {
        this.runSync(scenario, () => {
            var run = (arr, expect) => {
                assertEqual(TestComponent.Test.staticAppendAll(arr), expect);
            };
            run([], '');
            run(['foo'], 'foo');
            run(['f\0o\0o', '\0', 'b\0a\0r'], 'f\0o\0o\0b\0a\0r');
        })
    }

    // Static out params
    runStaticBoolOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                var { returnValue, and, or } = TestComponent.Test.staticBoolOutParam(lhs, rhs);
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
                var { returnValue, next, prev } = TestComponent.Test.staticCharOutParam(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
                assertEqual(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
                assertEqual(returnValue, ch.toUpperCase());
            };

            for (var val of this.charTestValues) {
                run(val);
            }
        });
    }

    runStaticNumericOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, doubledValue, tripledValue } = TestComponent.Test.staticNumericOutParam(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
                assertEqual(returnValue, val * 4);
            };
            run(0);
            run(42);
            run(-1);
        });
    }

    runStaticStringOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, lower, upper } = TestComponent.Test.staticStringOutParam(val);
                assertEqual(lower, val.toLowerCase());
                assertEqual(upper, val.toUpperCase());
                assertEqual(returnValue, val.split('').reverse().join(''));
            };

            for (var val of this.stringTestValues) {
                run(val);
            }
        });
    }

    runStaticGuidOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, zero, allSet } = TestComponent.Test.staticGuidOutParam(val);
                assertEqual(zero, this.zeroGuid);
                assertEqual(allSet, this.allSetGuid);
                assertEqual(returnValue, val);
            };

            for (var val of this.guidTestValues) {
                run(val);
            }
        });
    }

    runStaticEnumOutParam(scenario) {
        this.runSync(scenario, () => {
            var { returnValue, plusOne, plusTwo } = TestComponent.Test.staticEnumOutParam(TestComponent.TestEnum.first);
            assertEqual(plusOne, TestComponent.TestEnum.second);
            assertEqual(plusTwo, TestComponent.TestEnum.third);
            assertEqual(returnValue, TestComponent.TestEnum.fourth);
        });
    }

    runStaticCompositeStructOutParam(scenario) {
        this.runSync(scenario, () => {
            var testVal = {
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 32.5, f64: 64.64, e: TestComponent.TestEnum.second },
                strings: { ch: 'F', str: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
                bools: { b: true }
            };

            var { returnValue, first, second } = TestComponent.Test.staticCompositeStructOutParam(testVal);
            assertEqual(returnValue, testVal);
            assertEqual(first, {
                numerics: { u8: 9, u16: 17, u32: 33, u64: 65, s16: -15, s32: -31, s64: -63, f32: 33.5, f64: 65.64, e: TestComponent.TestEnum.third },
                strings: { ch: 'G', str: 'hello, world!', guid: this.zeroGuid },
                bools: { b: false }
            });
            assertEqual(second, {
                numerics: { u8: 7, u16: 15, u32: 31, u64: 63, s16: -17, s32: -33, s64: -65, f32: 31.5, f64: 63.64, e: TestComponent.TestEnum.first },
                strings: { ch: 'E', str: 'HELLO, WORLD!', guid: this.allSetGuid },
                bools: { b: true }
            });
        });
    }

    runStaticRefOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, doubledValue, tripledValue } = TestComponent.Test.staticRefOutParam(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
                assertEqual(returnValue, val * 4);
            };
            run(0);
            run(42);
            run(-1);
        });
    }

    runStaticObjectOutParam(scenario) {
        this.runSync(scenario, () => {
            var val = TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4]);
            var { returnValue, doubledValues, tripledValues } = TestComponent.Test.staticObjectOutParam(val);
            assert(val == returnValue);

            for (var i = 0; i < val.size; ++i) {
                assertEqual(doubledValues.getAt(i), val.getAt(i) * 2);
                assertEqual(tripledValues.getAt(i), val.getAt(i) * 3);
            }
        });
    }

    validateRotatedArray(input, output, amt) {
        assertEqual(input.length, output.length);
        var i = 0, j = amt;
        for (; j < input.length; ++i, ++j) {
            assertEqual(input[i], output[j]);
        }

        j = 0;
        for (; i < input.length; ++i, ++j) {
            assertEqual(input[i], output[j]);
        }
    }

    validateReversedArray(input, output) {
        assertEqual(input.length, output.length);
        for (var i = 0, j = input.length - 1; i < input.length; ++i, --j) {
            assertEqual(input[i], output[j]);
        }
    }

    validateArrayOutParam(arr, fn) {
        var { returnValue, rot1, rot2 } = fn(arr);
        this.validateRotatedArray(arr, rot1, 1);
        this.validateRotatedArray(arr, rot2, 2);
        this.validateReversedArray(arr, returnValue);
    }

    runStaticBoolArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticBoolArrayOutParam(val);
            this.validateArrayOutParam([ false, false, true, true, false ], fn);
        });
    }

    runStaticCharArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticCharArrayOutParam(val);
            this.validateArrayOutParam([ 'A', 'B', 'C', 'D', 'E' ], fn);
        });
    }

    runStaticNumericArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticNumericArrayOutParam(val);
            this.validateArrayOutParam([ 0, 1, 2, 3, 4 ], fn);
        });
    }

    runStaticStringArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticStringArrayOutParam(val);
            this.validateArrayOutParam([ 'foo', 'bar', 'baz', 'foobar', 'foo\0bar' ], fn);
        });
    }

    runStaticGuidArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticGuidArrayOutParam(val);
            this.validateArrayOutParam(this.guidTestValues, fn);
        });
    }

    runStaticEnumArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticEnumArrayOutParam(val);
            this.validateArrayOutParam(this.enumTestValues, fn);
        });
    }

    runStaticCompositeStructArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticCompositeStructArrayOutParam(val);
            this.validateArrayOutParam(this.compositeTestValues, fn);
        });
    }

    runStaticObjectArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => TestComponent.Test.staticObjectArrayOutParam(val);
            this.validateArrayOutParam([
                TestComponent.Test.makeNumericVector([]),
                TestComponent.Test.makeNumericVector([0]),
                TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4])
            ], fn);
        });
    }

    // Static array fill params
    runStaticBoolFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticBoolFillParam(arr);

                var expect = false;
                for (var val of arr) {
                    assertEqual(val, expect);
                    expect = !expect;
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        });
    }

    runStaticCharFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticCharFillParam(arr);

                var expect = 'a'.charCodeAt(0);
                for (var val of arr) {
                    assertEqual(val.charCodeAt(0), expect++);
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runStaticNumericFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticNumericFillParam(arr);

                var expect = 0;
                for (var val of arr) {
                    assertEqual(val, expect++);
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runStaticStringFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticStringFillParam(arr);

                var expect = '';
                for (var val of arr) {
                    assertEqual(val, expect);
                    expect += 'a';
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runStaticGuidFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticGuidFillParam(arr);

                var expect = 0;
                for (var val of arr) {
                    var guid = guidFromString(val);
                    assertEqual(guid.data1, expect);
                    assertEqual(guid.data2, expect);
                    assertEqual(guid.data3, expect);
                    for (var byte of guid.data4) assertEqual(byte, expect);
                    ++expect;
                }
            };
            run(0);
            run(1);
            run(2);
            // run(100);
        })
    }

    runStaticEnumFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticEnumFillParam(arr);

                var expect = TestComponent.TestEnum.first;
                for (var val of arr) {
                    assertEqual(val, expect++);
                }
            };
            run(0);
            run(1);
            run(2);
            run(4);
        })
    }

    runStaticCompositeStructFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticCompositeStructFillParam(arr);

                var expectNumeric = 0;
                var expectString = '';
                var expectBool = false;
                for (var val of arr) {
                    assertEqual(val, {
                        numerics: { u8: expectNumeric, u16: expectNumeric, u32: expectNumeric, u64: expectNumeric, s16: expectNumeric,
                                s32: expectNumeric, s64: expectNumeric, f32: expectNumeric, f64: expectNumeric, e: expectNumeric },
                        strings: { ch: String.fromCharCode(expectNumeric), str: expectString,
                            guid: makeGuid(expectNumeric, expectNumeric, expectNumeric, [expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric]) },
                        bools: { b: expectBool }
                    });
                    ++expectNumeric;
                    expectString += 'a';
                    expectBool = !expectBool;
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runStaticObjectFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                TestComponent.Test.staticObjectFillParam(arr);

                var expect = 0;
                for (var val of arr) {
                    assertEqual(val.size, expect);
                    for (var i = 0; i < expect; ++i) {
                        assertEqual(val.getAt(i), i);
                    }
                    ++expect;
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    // Static events
    testStaticEventHandler(scenario, args, name, invoke) {
        this.runSync(scenario, () => {
            var i = 0;
            var handler = (sender, arg) => {
                assert(i < args.length);
                assertEqual(sender, null);
                assertEqual(arg, args[i]);
            };

            TestComponent.Test.addEventListener(name, handler);
            for (var arg of args) {
                invoke(arg);
                ++i;
            }

            TestComponent.Test.removeEventListener(name, handler);
            invoke(args[0]); // Should assert if handler is invoked
        });
    }

    runStaticBoolEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.boolTestValues, 'staticBoolEventHandler', (arg) => TestComponent.Test.raiseStaticBoolEvent(arg));
    }

    runStaticCharEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.charTestValues, 'staticCharEventHandler', (arg) => TestComponent.Test.raiseStaticCharEvent(arg));
    }

    runStaticNumericEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.s32TestValues, 'staticNumericEventHandler', (arg) => TestComponent.Test.raiseStaticNumericEvent(arg));
    }

    runStaticStringEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.stringTestValues, 'staticStringEventHandler', (arg) => TestComponent.Test.raiseStaticStringEvent(arg));
    }

    runStaticGuidEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.guidTestValues, 'staticGuidEventHandler', (arg) => TestComponent.Test.raiseStaticGuidEvent(arg));
    }

    runStaticEnumEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.enumTestValues, 'staticEnumEventHandler', (arg) => TestComponent.Test.raiseStaticEnumEvent(arg));
    }

    runStaticCompositeStructEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.compositeTestValues, 'staticCompositeStructEventHandler', (arg) => TestComponent.Test.raiseStaticCompositeStructEvent(arg));
    }

    runStaticRefEventHandler(scenario) {
        this.testStaticEventHandler(scenario, this.s32TestValues, 'staticRefEventHandler', (arg) => TestComponent.Test.raiseStaticRefEvent(arg));
    }

    runStaticObjectEventHandler(scenario) {
        var vals = [TestComponent.Test.makeNumericVector([]), TestComponent.Test.makeNumericVector([0]), TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4])];
        this.testStaticEventHandler(scenario, vals, 'staticObjectEventHandler', (arg) => TestComponent.Test.raiseStaticObjectEvent(arg));
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

    runOrAll(scenario) {
        this.runSync(scenario, () => {
            var run = (arr, expect) => {
                assertEqual(this.test.orAll(arr), expect);
            };
            run([], false);
            run([false], false);
            run([true], true);
            run([false, false, false, false], false);
            run([true, false, false, false], true);
            run([false, true, false, false], true);
            run([false, false, true, false], true);
            run([false, false, false, true ], true);
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

    runAddAll(scenario) {
        this.runSync(scenario, () => {
            var run = (arr, expect) => {
                assertEqual(this.test.addAll(arr), expect);
            };
            run([], 0);
            run([1], 1);
            run([1, 2, 3], 6);
            run([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 55);
        });
    }

    runAppend(scenario) {
        this.runSync(scenario, () => {
            assertEqual(this.test.append('foo', '\0', 'bar'), 'foo\0bar');
            assertEqual(this.test.append('Hello', ' ', 'world'), 'Hello world');
        })
    }

    runAppendAll(scenario) {
        this.runSync(scenario, () => {
            var run = (arr, expect) => {
                assertEqual(this.test.appendAll(arr), expect);
            };
            run([], '');
            run(['foo'], 'foo');
            run(['f\0o\0o', '\0', 'b\0a\0r'], 'f\0o\0o\0b\0a\0r');
        })
    }

    // Static out params
    runBoolOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (lhs, rhs) => {
                var { returnValue, and, or } = this.test.boolOutParam(lhs, rhs);
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
                var { returnValue, next, prev } = this.test.charOutParam(ch);
                assertEqual(next, String.fromCharCode(ch.charCodeAt(0) + 1));
                assertEqual(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
                assertEqual(returnValue, ch.toUpperCase());
            };

            for (var val of this.charTestValues) {
                run(val);
            }
        });
    }

    runNumericOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, doubledValue, tripledValue } = this.test.numericOutParam(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
                assertEqual(returnValue, val * 4);
            };
            run(0);
            run(42);
            run(-1);
        });
    }

    runStringOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, lower, upper } = this.test.stringOutParam(val);
                assertEqual(lower, val.toLowerCase());
                assertEqual(upper, val.toUpperCase());
                assertEqual(returnValue, val.split('').reverse().join(''));
            };

            for (var val of this.stringTestValues) {
                run(val);
            }
        });
    }

    runGuidOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, zero, allSet } = this.test.guidOutParam(val);
                assertEqual(zero, this.zeroGuid);
                assertEqual(allSet, this.allSetGuid);
                assertEqual(returnValue, val);
            };

            for (var val of this.guidTestValues) {
                run(val);
            }
        });
    }

    runEnumOutParam(scenario) {
        this.runSync(scenario, () => {
            var { returnValue, plusOne, plusTwo } = this.test.enumOutParam(TestComponent.TestEnum.first);
            assertEqual(plusOne, TestComponent.TestEnum.second);
            assertEqual(plusTwo, TestComponent.TestEnum.third);
            assertEqual(returnValue, TestComponent.TestEnum.fourth);
        });
    }

    runCompositeStructOutParam(scenario) {
        this.runSync(scenario, () => {
            var testVal = {
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 32.5, f64: 64.64, e: TestComponent.TestEnum.second },
                strings: { ch: 'F', str: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
                bools: { b: true }
            };

            var { returnValue, first, second } = this.test.compositeStructOutParam(testVal);
            assertEqual(returnValue, testVal);
            assertEqual(first, {
                numerics: { u8: 9, u16: 17, u32: 33, u64: 65, s16: -15, s32: -31, s64: -63, f32: 33.5, f64: 65.64, e: TestComponent.TestEnum.third },
                strings: { ch: 'G', str: 'hello, world!', guid: this.zeroGuid },
                bools: { b: false }
            });
            assertEqual(second, {
                numerics: { u8: 7, u16: 15, u32: 31, u64: 63, s16: -17, s32: -33, s64: -65, f32: 31.5, f64: 63.64, e: TestComponent.TestEnum.first },
                strings: { ch: 'E', str: 'HELLO, WORLD!', guid: this.allSetGuid },
                bools: { b: true }
            });
        });
    }

    runRefOutParam(scenario) {
        this.runSync(scenario, () => {
            var run = (val) => {
                var { returnValue, doubledValue, tripledValue } = this.test.refOutParam(val);
                assertEqual(doubledValue, val * 2);
                assertEqual(tripledValue, val * 3);
                assertEqual(returnValue, val * 4);
            };
            run(0);
            run(42);
            run(-1);
        });
    }

    runObjectOutParam(scenario) {
        this.runSync(scenario, () => {
            var val = TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4]);
            var { returnValue, doubledValues, tripledValues } = this.test.objectOutParam(val);
            assert(val == returnValue);

            for (var i = 0; i < val.size; ++i) {
                assertEqual(doubledValues.getAt(i), val.getAt(i) * 2);
                assertEqual(tripledValues.getAt(i), val.getAt(i) * 3);
            }
        });
    }

    runBoolArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.boolArrayOutParam(val);
            this.validateArrayOutParam([ false, false, true, true, false ], fn);
        });
    }

    runCharArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.charArrayOutParam(val);
            this.validateArrayOutParam([ 'A', 'B', 'C', 'D', 'E' ], fn);
        });
    }

    runNumericArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.numericArrayOutParam(val);
            this.validateArrayOutParam([ 0, 1, 2, 3, 4 ], fn);
        });
    }

    runStringArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.stringArrayOutParam(val);
            this.validateArrayOutParam([ 'foo', 'bar', 'baz', 'foobar', 'foo\0bar' ], fn);
        });
    }

    runGuidArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.guidArrayOutParam(val);
            this.validateArrayOutParam(this.guidTestValues, fn);
        });
    }

    runEnumArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.enumArrayOutParam(val);
            this.validateArrayOutParam(this.enumTestValues, fn);
        });
    }

    runCompositeStructArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.compositeStructArrayOutParam(val);
            this.validateArrayOutParam(this.compositeTestValues, fn);
        });
    }

    runObjectArrayOutParam(scenario) {
        this.runSync(scenario, () => {
            var fn = (val) => this.test.objectArrayOutParam(val);
            this.validateArrayOutParam([
                TestComponent.Test.makeNumericVector([]),
                TestComponent.Test.makeNumericVector([0]),
                TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4])
            ], fn);
        });
    }

    // Non-static array fill params
    runBoolFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.boolFillParam(arr);

                var expect = false;
                for (var val of arr) {
                    assertEqual(val, expect);
                    expect = !expect;
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        });
    }

    runCharFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.charFillParam(arr);

                var expect = 'a'.charCodeAt(0);
                for (var val of arr) {
                    assertEqual(val.charCodeAt(0), expect++);
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runNumericFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.numericFillParam(arr);

                var expect = 0;
                for (var val of arr) {
                    assertEqual(val, expect++);
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runStringFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.stringFillParam(arr);

                var expect = '';
                for (var val of arr) {
                    assertEqual(val, expect);
                    expect += 'a';
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runGuidFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.guidFillParam(arr);

                var expect = 0;
                for (var val of arr) {
                    var guid = guidFromString(val);
                    assertEqual(guid.data1, expect);
                    assertEqual(guid.data2, expect);
                    assertEqual(guid.data3, expect);
                    for (var byte of guid.data4) assertEqual(byte, expect);
                    ++expect;
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runEnumFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.enumFillParam(arr);

                var expect = TestComponent.TestEnum.first;
                for (var val of arr) {
                    assertEqual(val, expect++);
                }
            };
            run(0);
            run(1);
            run(2);
            run(4);
        })
    }

    runCompositeStructFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.compositeStructFillParam(arr);

                var expectNumeric = 0;
                var expectString = '';
                var expectBool = false;
                for (var val of arr) {
                    assertEqual(val, {
                        numerics: { u8: expectNumeric, u16: expectNumeric, u32: expectNumeric, u64: expectNumeric, s16: expectNumeric,
                                s32: expectNumeric, s64: expectNumeric, f32: expectNumeric, f64: expectNumeric, e: expectNumeric },
                        strings: { ch: String.fromCharCode(expectNumeric), str: expectString,
                            guid: makeGuid(expectNumeric, expectNumeric, expectNumeric, [expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric]) },
                        bools: { b: expectBool }
                    });
                    ++expectNumeric;
                    expectString += 'a';
                    expectBool = !expectBool;
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    runObjectFillParam(scenario) {
        this.runSync(scenario, () => {
            var run = (size) => {
                var arr = new Array(size);
                this.test.objectFillParam(arr);

                var expect = 0;
                for (var val of arr) {
                    assertEqual(val.size, expect);
                    for (var i = 0; i < expect; ++i) {
                        assertEqual(val.getAt(i), i);
                    }
                    ++expect;
                }
            };
            run(0);
            run(1);
            run(2);
            run(100);
        })
    }

    // Non-static events
    testEventHandler(scenario, args, name, invoke) {
        this.runSync(scenario, () => {
            var i = 0;
            var handler = (sender, arg) => {
                assert(i < args.length);
                assert(sender == this.test);
                assertEqual(arg, args[i]);
            };

            this.test.addEventListener(name, handler);
            for (var arg of args) {
                invoke(arg);
                ++i;
            }

            this.test.removeEventListener(name, handler);
            invoke(args[0]); // Should assert if handler is invoked
        });
    }

    runBoolEventHandler(scenario) {
        this.testEventHandler(scenario, this.boolTestValues, 'boolEventHandler', (arg) => this.test.raiseBoolEvent(arg));
    }

    runCharEventHandler(scenario) {
        this.testEventHandler(scenario, this.charTestValues, 'charEventHandler', (arg) => this.test.raiseCharEvent(arg));
    }

    runNumericEventHandler(scenario) {
        this.testEventHandler(scenario, this.s32TestValues, 'numericEventHandler', (arg) => this.test.raiseNumericEvent(arg));
    }

    runStringEventHandler(scenario) {
        this.testEventHandler(scenario, this.stringTestValues, 'stringEventHandler', (arg) => this.test.raiseStringEvent(arg));
    }

    runGuidEventHandler(scenario) {
        this.testEventHandler(scenario, this.guidTestValues, 'guidEventHandler', (arg) => this.test.raiseGuidEvent(arg));
    }

    runEnumEventHandler(scenario) {
        this.testEventHandler(scenario, this.enumTestValues, 'enumEventHandler', (arg) => this.test.raiseEnumEvent(arg));
    }

    runCompositeStructEventHandler(scenario) {
        this.testEventHandler(scenario, this.compositeTestValues, 'compositeStructEventHandler', (arg) => this.test.raiseCompositeStructEvent(arg));
    }

    runRefEventHandler(scenario) {
        this.testEventHandler(scenario, this.s32TestValues, 'refEventHandler', (arg) => this.test.raiseRefEvent(arg));
    }

    runObjectEventHandler(scenario) {
        var vals = [TestComponent.Test.makeNumericVector([]), TestComponent.Test.makeNumericVector([0]), TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4])];
        this.testEventHandler(scenario, vals, 'objectEventHandler', (arg) => this.test.raiseObjectEvent(arg));
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
