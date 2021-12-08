// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * @format
 */
// TODO: JSI does not allow property setters to return a value, which causes Chakra to throw errors in strict mode
//       https://github.com/microsoft/react-native-winrt/issues/8
// 'use strict';

import React, { Component } from 'react';
import {
    SafeAreaView,
    ScrollView,
    View,
    Text,
    Button
} from 'react-native';
import { Scenario } from './Scenario';
import { styles } from './Styles';
import { TestResult } from './TestCommon'
import { makePropertiesTestScenarios } from './PropertiesTests'
import { makeBasicFunctionTestScenarios } from './BasicFunctionTests'
import { makeArrayTestScenarios } from './ArrayTests'
import { makeDelegateAndEventTestScenarios } from './DelegateAndEventTests'
import { makeAsyncTestScenarios } from './AsyncTests'
import { makeCollectionsTestScenarios } from './CollectionsTests'
import { makeInheritanceTestScenarios } from './InheritanceTests'

class App extends Component {
    test = new TestComponent.Test();
    completedCount = 0;
    passCount = 0;
    failures = "Failed Scenarios:\r\n";

    testSuites = [
        {
            name: "Property Tests",
            scenarios: makePropertiesTestScenarios(this),
        }, {
            name: "Function Tests",
            scenarios: makeBasicFunctionTestScenarios(this),
        }, {
            name: "Array Tests",
            scenarios: makeArrayTestScenarios(this),
        }, {
            name: "Delegate Tests",
            scenarios: makeDelegateAndEventTestScenarios(this),
        }, {
            name: "Async Tests",
            scenarios: makeAsyncTestScenarios(this),
        }, {
            name: "Collections Tests",
            scenarios: makeCollectionsTestScenarios(this),
        }, {
            name: "Inheritance Tests",
            scenarios: makeInheritanceTestScenarios(this),
        }
    ]

    scenarioNameToTestSuiteMap = this.testSuites.reduce((acc, testSuite) => {
        testSuite.scenarios.forEach(scenario => {
            acc[scenario.name] = testSuite.name
        })
        return acc;
    }, {});

    state = {
        completedCount: 0,
        passCount: 0,
        testSuiteCompletedCount: this.testSuites.reduce((acc, testSuite) => { return {...acc, [testSuite.name]: 0}}, {}),
        testSuitePassCount: this.testSuites.reduce((acc, testSuite) => { return {...acc, [testSuite.name]: 0}}, {}),
        testSuiteVisibility: this.testSuites.reduce((acc, testSuite) => { return {...acc, [testSuite.name]: false}}, {}),
        filterText: "",
    };



    runSync(scenario, fn) {
        let previousResult = scenario.result;
        var result = TestResult.Fail;
        try {
            fn();
            result = TestResult.Pass;
        } catch (e) {
            scenario.failureText = e.message;
        }

        scenario.result = result;
        scenario.emit('completed');
        this.onSingleTestCompleted(scenario, previousResult);
    }

    runAsync(scenario, fn) {
        let previousResult = scenario.result;
        var result = TestResult.Fail;
        new Promise(fn).then(() => {
            result = TestResult.Pass;
            scenario.result = result;
            scenario.emit('completed');
            this.onSingleTestCompleted(scenario, previousResult);
        }).catch(e => {
            scenario.failureText = e.message;
            scenario.result = result;
            scenario.emit('completed');
            this.onSingleTestCompleted(scenario, previousResult);
        })
    }

    onSingleTestCompleted(scenario, previousResult) {
        const testSuiteName = this.scenarioNameToTestSuiteMap[scenario.name];
        if (previousResult === TestResult.NotRun) ++this.completedCount;
        let resultState = {
            completedCount: this.completedCount,
            passCount: this.passCount,
            testSuiteCompletedCount: this.state.testSuiteCompletedCount,
            testSuitePassCount: this.state.testSuitePassCount,
        }
        if (previousResult === TestResult.NotRun) resultState.testSuiteCompletedCount[testSuiteName] = this.state.testSuiteCompletedCount[testSuiteName] + 1;

        if (scenario.result === TestResult.Pass && previousResult !== TestResult.Pass) {
            ++this.passCount;
            resultState.passCount = this.passCount;
            resultState.testSuitePassCount[testSuiteName] = this.state.testSuitePassCount[testSuiteName] + 1;
        } else if (scenario.result === TestResult.Fail && previousResult === TestResult.Pass){
            --this.passCount;
            resultState.passCount = this.passCount;
            resultState.testSuitePassCount[testSuiteName] = this.state.testSuitePassCount[testSuiteName] - 1;
            this.failures += scenario.name + "\r\n";
        }

        this.setState(resultState);
    }

    allScenarios() {
        return this.testSuites.reduce((acc, val) => acc.concat(val.scenarios), []);
    }

    inProgress() {
        return this.state.completedCount != this.allScenarios().length;
    }

    passed() {
        return this.state.passCount == this.allScenarios().length;
    }

    resultText() {
        if (!this.inProgress() && !this.passed()) {
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
        for (let scenario of this.allScenarios()) {
            scenario.invoke(scenario);
        }
    }

    toggleTestSuiteVisibility(testSuite) {
        this.setState({
            testSuiteVisibility: {
                ...this.state.testSuiteVisibility,
                [testSuite.name]: !this.state.testSuiteVisibility[testSuite.name]
            },
        })
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
                        this.testSuites.map((testSuite, idx) => (
                            <View key={testSuite.name}>
                                <View style={styles.listEntry}>
                                    <Text style={[styles.rowItem, {fontWeight: 'bold'}]}>{testSuite.name}</Text>
                                    <Text style={[styles.rowItem, {flex: 1}]}>{
                                        this.state.testSuitePassCount[testSuite.name]
                                        + "/"
                                        + this.state.testSuiteCompletedCount[testSuite.name]
                                        + " passed. "
                                        + (testSuite.scenarios.length - this.state.testSuiteCompletedCount[testSuite.name])
                                        + " pending."
                                    }</Text>
                                    <Button style={[styles.rowItem, {flex: 1}]}
                                        title={this.state.testSuiteVisibility[testSuite.name] ? "Hide all tests" : "Show all tests"}
                                        onPress={() => this.toggleTestSuiteVisibility(testSuite)}/>
                                </View>
                                {
                                    testSuite.scenarios.map((scenario, idx) => (
                                        this.state.testSuiteVisibility[testSuite.name] || scenario.result !== TestResult.Pass
                                            ? <Scenario key={scenario.name} scenario={scenario} />
                                            : <View key={idx} style={{marginHorizontal: 1}}/>
                                    ))
                                }
                            </View>
                        ))
                    }
                </ScrollView>
            </SafeAreaView>
        );
    }
}

export default App;
