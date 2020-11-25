/**
 * @format
 */
'use strict';

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
        throw new Error('Assertion failed! ' + result.msg);
    }
}

function assertNotEqual(lhs, rhs) {
    var result = checkEquals(lhs, rhs);
    if (result.success) {
        throw new Error('Assertion failed! \'' + stringify(lhs) + '\' == \'' + stringify(rhs) + '\'');
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
    tests = TestComponent.TestRunner.makeTests();
    state = { completedCount: 0, passCount: 0 };
    completedCount = 0;
    passCount = 0;

    scenarios = [
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
