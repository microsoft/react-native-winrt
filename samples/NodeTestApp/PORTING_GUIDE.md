# Quick Reference: Porting Tests from RnWinRTTests to NodeTestApp

## File Header Pattern

```javascript
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * [TestName] Tests - Node.js Adapter
 * @format
 */

const { TestScenario, assert, /* other imports */ } = require('./TestCommon');
```

## Test Suite Factory Pattern

### Before (React Native)
```javascript
import { TestScenario, assert } from './TestCommon'

export function makeXxxTestScenarios(pThis) {
    return [
        new TestScenario('Test Name', runTest.bind(pThis)),
    ];
}
```

### After (Node.js)
```javascript
const { TestScenario, assert } = require('./TestCommon');

function makeXxxTestScenarios(pThis, TestComponent, TestValues) {
    return [
        new TestScenario('Test Name', () => runTest(pThis, TestComponent)),
    ];
}

module.exports = { makeXxxTestScenarios };
```

## Test Function Pattern

### Before (React Native)
```javascript
function runTest(scenario) {
    this.runSync(scenario, () => {
        assert.equal(TestComponent.Test.someMethod(), expected);
    });
}
```

### After (Node.js)
```javascript
function runTest(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal(TestComponent.Test.someMethod(), expected);
    });
}
```

## Async Test Pattern

### Before (React Native)
```javascript
function runAsyncTest(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        TestComponent.Test.asyncMethod().then(result => {
            assert.equal(result, expected);
            resolve();
        }).catch(reject);
    });
}
```

### After (Node.js)
```javascript
async function runAsyncTest(pThis, TestComponent) {
    await pThis.runAsync(null, (resolve, reject) => {
        TestComponent.Test.asyncMethod().then(result => {
            assert.equal(result, expected);
            resolve();
        }).catch(reject);
    });
}
```

## Using TestValues

### Before (React Native - Global)
```javascript
import { TestValues } from './TestCommon'

function runTest(scenario) {
    this.runSync(scenario, () => {
        for (const val of TestValues.bools.valid) {
            // test with val
        }
    });
}
```

### After (Node.js - Parameter)
```javascript
function makeXxxTestScenarios(pThis, TestComponent, TestValues) {
    return [
        new TestScenario('Test', () => runTest(pThis, TestComponent, TestValues)),
    ];
}

function runTest(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        for (const val of TestValues.bools.valid) {
            // test with val
        }
    });
}
```

## Registration in test.js

```javascript
const { makeXxxTestScenarios } = require('./test/XxxTests');

const testSuites = [
    // ... existing suites
    {
        name: "Xxx Tests",
        scenarios: makeXxxTestScenarios(testContext, TestComponent, TestValues),
    },
];
```

## Common Gotchas

❌ **Don't**: Use `import`/`export`  
✅ **Do**: Use `require`/`module.exports`

❌ **Don't**: Access `TestComponent` globally  
✅ **Do**: Pass it as a parameter

❌ **Don't**: Use `.bind(pThis)` in scenario constructor  
✅ **Do**: Use arrow function `() => runTest(pThis, TestComponent)`

❌ **Don't**: Pass `scenario` to runSync/runAsync  
✅ **Do**: Pass `null` as first parameter

❌ **Don't**: Forget to export the factory function  
✅ **Do**: Add `module.exports = { makeXxxTestScenarios };`

## Checklist

- [ ] Convert import statements to require
- [ ] Add TestComponent and TestValues parameters to factory function
- [ ] Update all test functions to accept pThis and TestComponent
- [ ] Change scenario.bind(pThis) to arrow functions
- [ ] Pass null to runSync/runAsync instead of scenario
- [ ] Add module.exports at the end
- [ ] Register test suite in test.js
- [ ] Run tests to verify: `npm run test:basic`
