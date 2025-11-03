// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * Node.js Test Runner for WinRT Projection Tests
 * @format
 */

const { TestResult } = require('./TestCommon');

class TestRunner {
    constructor(testSuites, ignoredCount = 0) {
        this.testSuites = testSuites;
        this.ignoredCount = ignoredCount;
        this.completedCount = 0;
        this.passCount = 0;
        this.failures = [];
    }

    async runAll() {
        console.log('\n========================================');
        console.log('WinRT Node.js Projection Tests');
        console.log('========================================\n');

        for (const testSuite of this.testSuites) {
            await this.runTestSuite(testSuite);
        }

        this.printSummary();
        return this.passCount === this.completedCount;
    }

    async runTestSuite(testSuite) {
        console.log(`\n--- ${testSuite.name} ---`);
        let suitePassCount = 0;
        let suiteCompletedCount = 0;

        for (const scenario of testSuite.scenarios) {
            const result = await this.runScenario(scenario);
            suiteCompletedCount++;
            this.completedCount++;

            if (result === TestResult.Pass) {
                suitePassCount++;
                this.passCount++;
                process.stdout.write('.');
            } else {
                process.stdout.write('F');
                this.failures.push({
                    suite: testSuite.name,
                    scenario: scenario.name,
                    error: scenario.failureText
                });
            }
        }

        console.log(`\n${testSuite.name}: ${suitePassCount}/${suiteCompletedCount} passed`);
    }

    async runScenario(scenario) {
        try {
            await scenario.invoke(scenario);
            scenario.result = TestResult.Pass;
            return TestResult.Pass;
        } catch (e) {
            scenario.failureText = e.message || e.toString();
            scenario.result = TestResult.Fail;
            return TestResult.Fail;
        }
    }

    printSummary() {
        console.log('\n========================================');
        console.log('Test Summary');
        console.log('========================================');
        console.log(`Total: ${this.completedCount}`);
        console.log(`Passed: ${this.passCount}`);
        console.log(`Failed: ${this.completedCount - this.passCount}`);
        if (this.ignoredCount > 0) {
            console.log(`Ignored: ${this.ignoredCount}`);
        }
        console.log(`Pass Rate: ${((this.passCount / this.completedCount) * 100).toFixed(2)}%`);

        if (this.failures.length > 0) {
            console.log('\n--- Failed Tests ---');
            for (const failure of this.failures) {
                console.log(`\n${failure.suite} > ${failure.scenario}`);
                console.log(`  Error: ${failure.error}`);
            }
        }

        console.log('\n========================================');
        if (this.passCount === this.completedCount) {
            console.log('✓ All tests passed!');
        } else {
            console.log('✗ Some tests failed.');
        }
        console.log('========================================\n');
    }
}

// Helper class for synchronous test execution
class SyncTestContext {
    runSync(scenario, fn) {
        fn();
    }

    async runAsync(scenario, fn) {
        await new Promise(fn);
    }
}

module.exports = { TestRunner, SyncTestContext };
