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
    failures = "Failed Scenarios:\r\n";

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
        this.onSingleTestCompleted(scenario);
    }

    onSingleTestCompleted(scenario) {
        ++this.completedCount;
        if (scenario.result == TestResult.Pass) {
            ++this.passCount;
        } else {
            this.failures += scenario.name + "\r\n";
        }

        this.setState({
            completedCount: this.completedCount,
            passCount: this.passCount,
        });
    }

    inProgress() {
        return this.state.completedCount != this.scenarios.length;
    }

    passed() {
        return this.state.passCount == this.scenarios.length;
    }

    resultText() {
        if (!this.inProgress() && this.passed())
        {
            TestComponent.Test.logFailures(this.failures);
        }

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
