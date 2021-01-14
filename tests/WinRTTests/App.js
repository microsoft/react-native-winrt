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
import {
    TestResult,
    assert
} from './TestCommon'
import { makePropertiesTestScenarios } from './PropertiesTests'
import { makeBasicFunctionTestScenarios } from './BasicFunctionTests'
import { makeArrayTestScenarios } from './ArrayTests'
import { makeDelegateAndEventTestScenarios } from './DelegateAndEventTests'

class App extends Component {
    test = new TestComponent.Test();
    state = { completedCount: 0, passCount: 0 };
    completedCount = 0;
    passCount = 0;

    scenarios = [
        ...makePropertiesTestScenarios(this),
        ...makeBasicFunctionTestScenarios(this),
        ...makeArrayTestScenarios(this),
        ...makeDelegateAndEventTestScenarios(this),
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
