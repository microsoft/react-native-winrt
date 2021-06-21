/**
 * @format
 */
'use strict';

import React, { Component } from "react";
import { Button, Text, View } from "react-native";
import { TestResult } from "./TestCommon";
import { styles } from './Styles'

export class Scenario extends Component {
    state = { result: TestResult.NotRun };

    constructor(props) {
        super(props);

        this.state.result = this.props.scenario.result;
        this.props.scenario.addListener('completed', this.onCompleted.bind(this));
    }

    onCompleted() {
        this.setState({ result: this.props.scenario.result });
    }

    resultText() {
        switch (this.state.result) {
            case TestResult.Pass: return 'Pass';
            case TestResult.Fail: return 'Fail';
            case TestResult.NotRun:
            default: return 'In Progress';
        }
    }

    textStyle() {
        switch (this.state.result) {
            case TestResult.Pass: return styles.passText;
            case TestResult.Fail: return styles.failText;
            case TestResult.NotRun:
            default: return styles.progressText;
        }
    }

    rerunTest() {
        this.props.scenario.invoke(this.props.scenario);
    }

    render() {
        return (
            <View style={styles.listEntry}>
                <Text style={styles.rowItem}>{this.props.scenario.name}</Text>
                <Text style={[styles.rowItem, {flex: 1}]}>{this.props.scenario.failureText}</Text>
                <Text style={[styles.rowItem, this.textStyle()]}>{this.resultText()}</Text>
                <Button style={styles.rowItem} onPress={this.rerunTest.bind(this)} title='Rerun'/>
            </View>
        );
    }
}
