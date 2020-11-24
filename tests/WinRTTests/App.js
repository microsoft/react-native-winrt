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

function assertEqual(lhs, rhs) {
    if (lhs != rhs) {
        throw new Error('Assertion failed! \'' + lhs.toString() + '\' == \'' + rhs.toString() + '\'');
    }
}

function assertNotEqual(lhs, rhs) {
    if (lhs == rhs) {
        throw new Error('Assertion failed! ' + lhs.toString() + ' != ' + rhs.toString());
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
    ];

    runSync(scenario, fn) {
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
            var vals = ['a', 'b', 'c']; // Char16
            for (var val of vals) {
                var { returnValue, b } = this.tests.param11(val);
                assertEqual(returnValue, val);
                assertEqual(b, val);
            }
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
                    <View style={styles.listEntry}>
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
