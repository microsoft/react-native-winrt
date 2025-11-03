// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * Node.js WinRT Projection Test Suite
 * @format
 */

const addon = require('bindings')('winrtaddon');
const { TestRunner, SyncTestContext } = require('./test/TestRunner');
const { getTestValues } = require('./test/TestCommon');
const { makeBasicFunctionTestScenarios } = require('./test/BasicFunctionTests');
const { makeEnumTestScenarios } = require('./test/EnumTests');

// Get TestComponent from the addon
const TestComponent = addon.TestComponent;

if (!TestComponent) {
    console.error('ERROR: TestComponent is not available in the addon');
    process.exit(1);
}

// Create test values with TestComponent
const TestValues = getTestValues(TestComponent);

// Create test context
const testContext = new SyncTestContext();

// Define test suites
const testSuites = [
    {
        name: "Enum Tests",
        scenarios: makeEnumTestScenarios(testContext, TestComponent, TestValues),
    },
    {
        name: "Basic Function Tests",
        scenarios: makeBasicFunctionTestScenarios(testContext, TestComponent, TestValues),
    },
    // Add more test suites here as they are ported:
    // {
    //     name: "Property Tests",
    //     scenarios: makePropertiesTestScenarios(testContext, TestComponent, TestValues),
    // },
    // {
    //     name: "Array Tests",
    //     scenarios: makeArrayTestScenarios(testContext, TestComponent, TestValues),
    // },
];

// Run tests
const runner = new TestRunner(testSuites);
runner.runAll().then(success => {
    process.exit(success ? 0 : 1);
}).catch(err => {
    console.error('Unexpected error running tests:', err);
    process.exit(1);
});
