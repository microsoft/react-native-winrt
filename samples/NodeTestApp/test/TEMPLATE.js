// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * TEST SUITE TEMPLATE - NOT A REAL TEST FILE
 * 
 * This is a template for porting test files from tests/RnWinRTTests to NodeTestApp.
 * Do not run this file directly - it contains placeholder syntax that will cause errors.
 * 
 * [SUITE_NAME] Tests - Node.js Adapter
 * 
 * TODO: Replace [SUITE_NAME] with the actual test suite name (e.g., "Properties", "Array")
 * 
 * Porting steps:
 * 1. Copy original test file from tests/RnWinRTTests/[SUITE_NAME]Tests.js
 * 2. Save it as test/[SUITE_NAME]Tests.js
 * 3. Follow the transformation pattern shown below
 * 
 * @format
 */

/* eslint-disable */
// This file intentionally contains template placeholders that will cause lint errors

// STEP 1: Convert imports to requires
// From: import { TestScenario, assert, TestValues } from './TestCommon'
// To:   const { TestScenario, assert } = require('./TestCommon');

const { 
    TestScenario, 
    assert,
    // Add other imports as needed (zeroGuid, allSetGuid, etc.)
} = require('./TestCommon');

// STEP 2: Update function signature to accept TestComponent and TestValues
// From: export function make[SUITE_NAME]TestScenarios(pThis) {
// To:   function make[SUITE_NAME]TestScenarios(pThis, TestComponent, TestValues) {

function make[SUITE_NAME]TestScenarios(pThis, TestComponent, TestValues) {
    // STEP 3: Update TestScenario constructors to use arrow functions
    // From: new TestScenario('Test Name', runTestName.bind(pThis)),
    // To:   new TestScenario('Test Name', () => runTestName(pThis, TestComponent)),
    //       or if using TestValues: () => runTestName(pThis, TestComponent, TestValues)
    
    return [
        new TestScenario('Test 1', () => runTest1(pThis, TestComponent)),
        new TestScenario('Test 2', () => runTest2(pThis, TestComponent, TestValues)),
        // Add more scenarios...
    ];
}

// STEP 4: Update each test function signature
// From: function runTestName(scenario) {
// To:   function runTestName(pThis, TestComponent) {
//       or: function runTestName(pThis, TestComponent, TestValues) {

function runTest1(pThis, TestComponent) {
    // STEP 5: Update runSync/runAsync calls
    // From: this.runSync(scenario, () => {
    // To:   pThis.runSync(null, () => {
    
    pThis.runSync(null, () => {
        // Your test assertions here
        assert.equal(TestComponent.SomeClass.someMethod(), expectedValue);
    });
}

function runTest2(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        // Example using TestValues
        for (const val of TestValues.bools.valid) {
            assert.equal(TestComponent.SomeClass.process(val), true);
        }
    });
}

// Example async test
async function runAsyncTest(pThis, TestComponent) {
    await pThis.runAsync(null, (resolve, reject) => {
        TestComponent.SomeClass.asyncMethod()
            .then(result => {
                assert.equal(result, expectedValue);
                resolve();
            })
            .catch(reject);
    });
}

// STEP 6: Export the factory function
// From: export { make[SUITE_NAME]TestScenarios };
// To:   module.exports = { make[SUITE_NAME]TestScenarios };

module.exports = { make[SUITE_NAME]TestScenarios };

// STEP 7: Register in test.js
// Add to test.js:
//   const { make[SUITE_NAME]TestScenarios } = require('./test/[SUITE_NAME]Tests');
//
// Add to testSuites array:
//   {
//       name: "[SUITE_NAME] Tests",
//       scenarios: make[SUITE_NAME]TestScenarios(testContext, TestComponent, TestValues),
//   },

// STEP 8: Run tests to verify
// npm run test:basic
