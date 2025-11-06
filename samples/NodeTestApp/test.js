// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * Node.js WinRT Projection Test Suite
 * 
 * Usage:
 *   node test.js                    - Run all passing tests (default)
 *   node test.js --all              - Run all tests including known failures
 *   node test.js --test <name>      - Run a single test by name
 *   node test.js -t <name>          - Run a single test by name (short form)
 * 
 * Examples:
 *   node test.js --test "Enum keys"
 *   node test.js -t "Test::Or"
 * 
 * @format
 */

const addon = require('bindings')('winrtaddon');
const { TestRunner, SyncTestContext } = require('./test/TestRunner');
const { getTestValues } = require('./test/TestCommon');
const { makeBasicFunctionTestScenarios } = require('./test/BasicFunctionTests');
const { makeEnumTestScenarios } = require('./test/EnumTests');
const { makeMiscTestScenarios } = require('./test/MiscTests');
const { makeArrayTestScenarios } = require('./test/ArrayTests');

const { makePropertiesTestScenarios } = require('./test/PropertiesTests');

const TestComponent = addon.TestComponent;

function oneOff()
{
    // TODO: This only works if we cache TestComponent.Test in a variable.
    //       We should fix this in the addon.
    let t = TestComponent.Test;
    t.staticU32Property = 42;
    console.log("prop", t.staticU32Property);
    process.exit(0);
}

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
        name: "Misc Tests",
        scenarios: makeMiscTestScenarios(testContext, TestComponent),
    },
    {
        name: "Enum Tests",
        scenarios: makeEnumTestScenarios(testContext, TestComponent, TestValues),
    },
    {
        name: "Basic Function Tests",
        scenarios: makeBasicFunctionTestScenarios(testContext, TestComponent, TestValues),
    },
    {
        name: "Property Tests",
        scenarios: makePropertiesTestScenarios(testContext, TestComponent, TestValues),
    },
    {
        name: "Array Tests",
        scenarios: makeArrayTestScenarios(testContext, TestComponent, TestValues),
    },
];

// Run tests

// List of failing test scenario names (from user)
const failingScenarios = [
    'Enum keys',                            // Fails because we have _HostObject_ in the list.
    'Test::StaticOrAll',
    'Test::StaticAddAll',
    'Test::StaticAppendAll',
    'Test::StaticBoolOutParam',
    'Test::StaticCharOutParam',
    'Test::StaticNumericOutParam',
    'Test::StaticStringOutParam',
    'Test::StaticGuidOutParam',
    'Test::StaticEnumOutParam',
    'Test::StaticCompositeStructOutParam',
    'Test::StaticRefOutParam',
    'Test::StaticObjectOutParam',
    'Test::StaticInterwovenParams',
    'Test::OrAll',
    'Test::AddAll',
    'Test::AppendAll',
    'Test::BoolOutParam',
    'Test::CharOutParam',
    'Test::NumericOutParam',
    'Test::StringOutParam',
    'Test::GuidOutParam',
    'Test::EnumOutParam',
    'Test::CompositeStructOutParam',
    'Test::RefOutParam',
    'Test::ObjectOutParam',
    'Test::InterwovenParams',
    // Static array property tests - array_to_native_iterator not implemented
    'Test::StaticBooleanArrayProperty',
    'Test::StaticCharArrayProperty',
    'Test::StaticNumericArrayProperty',
    'Test::StaticStringArrayProperty',
    'Test::StaticGuidArrayProperty',
    'Test::StaticEnumArrayProperty',
    'Test::StaticCompositeStructArrayProperty',
    'Test::StaticRefArrayProperty',
    'Test::StaticObjectArrayProperty',
    // Instance array property tests - array handling issues
    'Test::BooleanArrayProperty',
    'Test::CharArrayProperty',
    'Test::NumericArrayProperty',
    'Test::StringArrayProperty',
    'Test::GuidArrayProperty',
    'Test::EnumArrayProperty',
    'Test::CompositeStructArrayProperty',
    'Test::RefArrayProperty',
    'Test::ObjectArrayProperty',
    // Array out param and fill param tests - array handling issues
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
];

// Parse command line arguments
const runAllTests = process.argv.includes('--all');
const testArgIndex = process.argv.findIndex(arg => arg === '--test' || arg === '-t');
const singleTestName = testArgIndex >= 0 && process.argv[testArgIndex + 1] 
    ? process.argv[testArgIndex + 1] 
    : null;

let filteredSuites = testSuites;
let ignoredCount = 0;

if (singleTestName) {
    // Run only the specified test
    console.log(`Running single test: ${singleTestName}\n`);
    let foundTest = false;
    filteredSuites = testSuites.map(suite => {
        const filtered = suite.scenarios.filter(s => s.name === singleTestName);
        if (filtered.length > 0) {
            foundTest = true;
        }
        ignoredCount += suite.scenarios.length - filtered.length;
        return {
            ...suite,
            scenarios: filtered,
        };
    }).filter(suite => suite.scenarios.length > 0);
    
    if (!foundTest) {
        console.error(`ERROR: Test "${singleTestName}" not found.`);
        console.error('\nAvailable tests:');
        testSuites.forEach(suite => {
            console.error(`\n${suite.name}:`);
            suite.scenarios.forEach(s => console.error(`  - ${s.name}`));
        });
        process.exit(1);
    }
} else if (!runAllTests) {
    // Filter out known failing scenarios and count them
    filteredSuites = testSuites.map(suite => {
        const filtered = suite.scenarios.filter(s => !failingScenarios.includes(s.name));
        ignoredCount += suite.scenarios.length - filtered.length;
        return {
            ...suite,
            scenarios: filtered,
        };
    });
    console.log('Running passing tests only. Use --all to run the full suite.\n');
}

const runner = new TestRunner(filteredSuites, ignoredCount);
runner.runAll().then(success => {
    process.exit(success ? 0 : 1);
}).catch(err => {
    console.error('Unexpected error running tests:', err);
    process.exit(1);
});
