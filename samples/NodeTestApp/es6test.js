// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * ES6 Test Suite for Node.js WinRT Projection
 * 
 * This runs the same test scenarios as the React Native test app, but in Node.js using ES6 modules
 * 
 * Usage:
 *   node es6test.js          - Run tests excluding known failures
 *   node es6test.js --all    - Run all tests including known failures
 * 
 * @format
 */

import { createRequire } from 'module';
const require = createRequire(import.meta.url);
const addon = require('bindings')('winrtaddon');

const { TestComponent } = addon;

// Parse command line arguments
const args = process.argv.slice(2);
const runAll = args.includes('--all') || args.includes('-all');

// Make TestComponent available globally BEFORE importing test modules
// This is required because TestCommon.js creates TestValues at module load time
globalThis.TestComponent = TestComponent;

// Import test scenarios from test suites (ES6 modules)
// These must be imported AFTER setting globalThis.TestComponent
import { makeBasicFunctionTestScenarios } from './es6tests/BasicFunctionTests.js';
import { makeEnumTestScenarios } from './es6tests/EnumTests.js';
import { makePropertiesTestScenarios } from './es6tests/PropertiesTests.js';
import { makeArrayTestScenarios } from './es6tests/ArrayTests.js';
import { makeCollectionsTestScenarios } from './es6tests/CollectionsTests.js';
import { makeMiscTestScenarios } from './es6tests/MiscTests.js';
import { makeDelegateAndEventTestScenarios } from './es6tests/DelegateAndEventTests.js';
import { TestResult } from './es6tests/TestCommon.js';

// Known failing tests (array_to_native_iterator not implemented, or other known issues)
const knownFailures = new Set([
    // Enum tests
    'Enum keys',
    
    // Property tests - array properties not working
    'Test::StaticBooleanArrayProperty',
    'Test::StaticCharArrayProperty',
    'Test::StaticNumericArrayProperty',
    'Test::StaticStringArrayProperty',
    'Test::StaticGuidArrayProperty',
    'Test::StaticEnumArrayProperty',
    'Test::StaticCompositeStructArrayProperty',
    'Test::StaticRefArrayProperty',
    'Test::StaticObjectArrayProperty',
    'Test::BooleanArrayProperty',
    'Test::CharArrayProperty',
    'Test::NumericArrayProperty',
    'Test::StringArrayProperty',
    'Test::GuidArrayProperty',
    'Test::EnumArrayProperty',
    'Test::CompositeStructArrayProperty',
    'Test::RefArrayProperty',
    'Test::ObjectArrayProperty',
    'Test::PropertyValue',
    
    // Function tests - array parameters not working
    'Test::StaticOrAll',
    'Test::StaticAddAll',
    'Test::StaticAppendAll',
    'Test::StaticInterwovenParams',
    'Test::OrAll',
    'Test::AddAll',
    'Test::AppendAll',
    'Test::InterwovenParams',
    
    // Array tests - array out params not working
    'Test::StaticBoolArrayOutParam',
    'Test::StaticCharArrayOutParam',
    'Test::StaticNumericArrayOutParam',
    'Test::StaticStringArrayOutParam',
    'Test::StaticGuidArrayOutParam',
    'Test::StaticEnumArrayOutParam',
    'Test::StaticCompositeStructArrayOutParam',
    'Test::StaticRefArrayOutParam',
    'Test::StaticObjectArrayOutParam',
    'Test::BoolArrayOutParam',
    'Test::CharArrayOutParam',
    'Test::NumericArrayOutParam',
    'Test::StringArrayOutParam',
    'Test::GuidArrayOutParam',
    'Test::EnumArrayOutParam',
    'Test::CompositeStructArrayOutParam',
    'Test::RefArrayOutParam',
    'Test::ObjectArrayOutParam',
    
    // Event handler tests - addEventListener not implemented
    'Test::StaticBoolEventHandler',
    'Test::StaticCharEventHandler',
    'Test::StaticNumericEventHandler',
    'Test::StaticStringEventHandler',
    'Test::StaticGuidEventHandler',
    'Test::StaticEnumEventHandler',
    'Test::StaticCompositeStructEventHandler',
    'Test::StaticRefEventHandler',
    'Test::StaticObjectEventHandler',
    'Async Event Handling',
    
    // Array delegate tests - array_to_native_iterator not implemented
    'Test::StaticInvokeBoolArrayDelegate',
    'Test::StaticInvokeCharArrayDelegate',
    'Test::StaticInvokeNumericArrayDelegate',
    'Test::StaticInvokeStringArrayDelegate',
    'Test::StaticInvokeGuidArrayDelegate',
    'Test::StaticInvokeEnumArrayDelegate',
    'Test::StaticInvokeCompositeStructArrayDelegate',
    'Test::StaticInvokeRefArrayDelegate',
    'Test::StaticInvokeObjectArrayDelegate',
    'Test::StaticInvokeInterwovenDelegate',
    
    // Object event handler test - assertion failure
    'StaticOnlyTest::ObjectEventHandler',
]);


// ES6 Class for test runner
class ES6TestRunner {
    constructor(skipKnownFailures = true) {
        this.test = new TestComponent.Test();
        this.completedCount = 0;
        this.passCount = 0;
        this.failures = [];
        this.testSuites = [];
        this.skipKnownFailures = skipKnownFailures;
        this.skippedCount = 0;
    }

    addTestSuite(name, makeScenarios) {
        const scenarios = makeScenarios(this);
        
        // Filter out known failures unless running with --all
        const filteredScenarios = this.skipKnownFailures 
            ? scenarios.filter(s => !knownFailures.has(s.name))
            : scenarios;
        
        const skipped = scenarios.length - filteredScenarios.length;
        if (skipped > 0) {
            this.skippedCount += skipped;
        }
        
        this.testSuites.push({ name, scenarios: filteredScenarios });
    }

    allScenarios() {
        return this.testSuites.reduce((acc, suite) => acc.concat(suite.scenarios), []);
    }

    runSync(scenario, fn) {
        try {
            // verbose?
            console.log(`Running test: ${scenario.name}`);
            fn();
            scenario.result = TestResult.Pass;
            this.passCount++;
        } catch (e) {
            console.log(`   Failed: ${scenario.name}`);
            scenario.result = TestResult.Fail;
            scenario.failureText = e.message;
            this.failures.push({ name: scenario.name, error: e.message });
        }
        this.completedCount++;
    }

    runAsync(scenario, fn) {
        return new Promise((resolve) => {
            new Promise(fn)
                .then(() => {
                    scenario.result = TestResult.Pass;
                    this.passCount++;
                    this.completedCount++;
                    resolve();
                })
                .catch(e => {
                    scenario.result = TestResult.Fail;
                    scenario.failureText = e.message;
                    this.failures.push({ name: scenario.name, error: e.message });
                    this.completedCount++;
                    resolve();
                });
        });
    }

    async run() {
        console.log('\n========================================');
        console.log('ES6 WinRT Test Runner');
        console.log('========================================\n');

        const allScenarios = this.allScenarios();
        
        if (this.skipKnownFailures && this.skippedCount > 0) {
            console.log(`Running with known failures skipped (${this.skippedCount} tests skipped)`);
            console.log(`Use --all to run all tests including known failures\n`);
        }
        
        console.log(`Total scenarios: ${allScenarios.length}\n`);

        // Run all scenarios
        for (const suite of this.testSuites) {
            console.log(`\nRunning ${suite.name}...`);
            const suiteStart = Date.now();
            let suitePassed = 0;
            let suiteFailed = 0;

            for (const scenario of suite.scenarios) {
                scenario.invoke(scenario);
                // For async tests, we need to wait
                if (scenario.result === TestResult.NotRun) {
                    // Give async tests a moment to complete
                    await new Promise(resolve => setTimeout(resolve, 10));
                }
                
                if (scenario.result === TestResult.Pass) {
                    suitePassed++;
                } else if (scenario.result === TestResult.Fail) {
                    suiteFailed++;
                }
            }

            const suiteTime = Date.now() - suiteStart;
            console.log(`  ✓ Passed: ${suitePassed}, ✗ Failed: ${suiteFailed}, Time: ${suiteTime}ms`);
        }

        // Print summary
        console.log('\n========================================');
        console.log('Test Summary');
        console.log('========================================');
        console.log(`Total: ${allScenarios.length}`);
        console.log(`Completed: ${this.completedCount}`);
        console.log(`Passed: ${this.passCount}`);
        console.log(`Failed: ${this.failures.length}`);
        if (this.skippedCount > 0) {
            console.log(`Skipped: ${this.skippedCount} (known failures)`);
        }
        console.log(`Pass Rate: ${((this.passCount / this.completedCount) * 100).toFixed(2)}%`);

        if (this.failures.length > 0) {
            console.log('\n========================================');
            console.log('Failed Tests');
            console.log('========================================');
            for (const failure of this.failures) {
                console.log(`✗ ${failure.name}`);
                console.log(`  Error: ${failure.error}`);
            }
        }

        console.log('========================================\n');

        return this.failures.length === 0;
    }
}

const runner = new ES6TestRunner(!runAll); // Skip known failures unless --all is specified

// Add test suites (similar to App.js testSuites array)
runner.addTestSuite('Misc Tests', makeMiscTestScenarios);
runner.addTestSuite('Enum Tests', makeEnumTestScenarios);
runner.addTestSuite('Property Tests', makePropertiesTestScenarios);
runner.addTestSuite('Function Tests', makeBasicFunctionTestScenarios);
runner.addTestSuite('Array Tests', makeArrayTestScenarios);
//crash here:
runner.addTestSuite('Collections Tests', makeCollectionsTestScenarios);

// TODO: Add more test suites as they become Node.js compatible:
runner.addTestSuite('Delegate Tests', makeDelegateAndEventTestScenarios);
// runner.addTestSuite('Async Tests', makeAsyncTestScenarios);
// runner.addTestSuite('Inheritance Tests', makeInheritanceTestScenarios);

// Run all tests
runner.run().then(success => {
    process.exit(success ? 0 : 1);
}).catch(err => {
    console.error('Unexpected error running tests:', err);
    process.exit(1);
});
