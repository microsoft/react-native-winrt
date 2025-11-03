# Complete Before/After Example

This document shows a complete side-by-side comparison of porting a test file.

## Original File: tests/RnWinRTTests/EnumTests.js

```javascript
// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

/**
 * @format
 */

import {
    TestScenario,
    assert,
} from './TestCommon'

export function makeEnumTestScenarios(pThis) {
    return [
        new TestScenario('Enum forward mapping', runEnumForwardMapping.bind(pThis)),
        new TestScenario('Enum reverse mapping', runEnumReverseMapping.bind(pThis)),
        new TestScenario('Enum keys', runEnumKeys.bind(pThis)),
        new TestScenario('Enum non-mappings', runEnumNonMappings.bind(pThis)),
    ];
}

function runEnumForwardMapping(scenario) {
    this.runSync(scenario, () => {
        assert.equal(TestComponent.TestEnum.first, 1);
        assert.equal(TestComponent.TestEnum.second, 2);
        assert.equal(TestComponent.TestEnum.third, 3);
        assert.equal(TestComponent.TestEnum.fourth, 4);
    });
}

function runEnumReverseMapping(scenario) {
    this.runSync(scenario, () => {
        assert.equal(TestComponent.TestEnum[1], "first");
        assert.equal(TestComponent.TestEnum[2], "second");
        assert.equal(TestComponent.TestEnum[3], "third");
        assert.equal(TestComponent.TestEnum[4], "fourth");
    });
}

function runEnumKeys(scenario) {
    this.runSync(scenario, () => {
        // keys only includes enum names and values
        assert.equal(Object.keys(TestComponent.TestEnum).sort(), 
            ["1", "2", "3", "4", "first", "fourth", "second", "third"]);
    })
}

function runEnumNonMappings(scenario) {
    this.runSync(scenario, () => {
        // case must match
        assert.undefined(TestComponent.TestEnum.First);
        // only enum values match, not other numbers
        assert.undefined(TestComponent.TestEnum[0]);
        // strings are not matched as numbers
        assert.undefined(TestComponent.TestEnum["1.0"]);
    });
}
```

## Ported File: samples/NodeTestApp/test/EnumTests.js

```javascript
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * Enum Tests - Node.js Adapter
 * @format
 */

const { TestScenario, assert } = require('./TestCommon');

function makeEnumTestScenarios(pThis, TestComponent, TestValues) {
    return [
        new TestScenario('Enum forward mapping', () => runEnumForwardMapping(pThis, TestComponent)),
        new TestScenario('Enum reverse mapping', () => runEnumReverseMapping(pThis, TestComponent)),
        new TestScenario('Enum keys', () => runEnumKeys(pThis, TestComponent)),
        new TestScenario('Enum non-mappings', () => runEnumNonMappings(pThis, TestComponent)),
    ];
}

function runEnumForwardMapping(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal(TestComponent.TestEnum.first, 1);
        assert.equal(TestComponent.TestEnum.second, 2);
        assert.equal(TestComponent.TestEnum.third, 3);
        assert.equal(TestComponent.TestEnum.fourth, 4);
    });
}

function runEnumReverseMapping(pThis, TestComponent) {
    pThis.runSync(null, () => {
        assert.equal(TestComponent.TestEnum[1], "first");
        assert.equal(TestComponent.TestEnum[2], "second");
        assert.equal(TestComponent.TestEnum[3], "third");
        assert.equal(TestComponent.TestEnum[4], "fourth");
    });
}

function runEnumKeys(pThis, TestComponent) {
    pThis.runSync(null, () => {
        // keys only includes enum names and values
        assert.equal(Object.keys(TestComponent.TestEnum).sort(), 
            ["1", "2", "3", "4", "first", "fourth", "second", "third"]);
    });
}

function runEnumNonMappings(pThis, TestComponent) {
    pThis.runSync(null, () => {
        // case must match
        assert.undefined(TestComponent.TestEnum.First);
        // only enum values match, not other numbers
        assert.undefined(TestComponent.TestEnum[0]);
        // strings are not matched as numbers
        assert.undefined(TestComponent.TestEnum["1.0"]);
    });
}

module.exports = { makeEnumTestScenarios };
```

## Key Changes Highlighted

### 1. Import/Export (Lines 8-11, Last line)

**Before:**
```javascript
import {
    TestScenario,
    assert,
} from './TestCommon'

export function makeEnumTestScenarios(pThis) {
```

**After:**
```javascript
const { TestScenario, assert } = require('./TestCommon');

function makeEnumTestScenarios(pThis, TestComponent, TestValues) {
    // ...
}

module.exports = { makeEnumTestScenarios };
```

### 2. Function Signature (Line 13)

**Before:**
```javascript
export function makeEnumTestScenarios(pThis) {
```

**After:**
```javascript
function makeEnumTestScenarios(pThis, TestComponent, TestValues) {
```

### 3. TestScenario Construction (Lines 14-17)

**Before:**
```javascript
new TestScenario('Enum forward mapping', runEnumForwardMapping.bind(pThis)),
```

**After:**
```javascript
new TestScenario('Enum forward mapping', () => runEnumForwardMapping(pThis, TestComponent)),
```

### 4. Test Function Signatures (Lines 21, 30, 39, 48)

**Before:**
```javascript
function runEnumForwardMapping(scenario) {
    this.runSync(scenario, () => {
```

**After:**
```javascript
function runEnumForwardMapping(pThis, TestComponent) {
    pThis.runSync(null, () => {
```

### 5. TestComponent Access (Lines 23-26)

**Before (global access):**
```javascript
assert.equal(TestComponent.TestEnum.first, 1);
```

**After (parameter access - same syntax):**
```javascript
assert.equal(TestComponent.TestEnum.first, 1);
```

Note: The actual TestComponent usage looks identical, but it's now from the parameter instead of global scope.

## Summary of Transformations

| Change | Count in this file | Pattern |
|--------|-------------------|---------|
| import → require | 1 | Convert ES6 import to CommonJS require |
| export → function | 1 | Remove export keyword |
| Add module.exports | 1 | Add CommonJS export at end |
| Add function parameters | 5 | Add `TestComponent` (and `TestValues` if needed) |
| .bind(pThis) → arrow fn | 4 | Change to `() => runTest(pThis, TestComponent)` |
| this → pThis | 4 | In runSync/runAsync calls |
| scenario → null | 4 | First parameter to runSync/runAsync |

## Diff Statistics

- **Lines changed:** 15
- **Lines added:** 2 (documentation, module.exports)
- **Lines removed:** 0
- **Total effort:** ~5 minutes for mechanical changes

## Registration in test.js

Add these lines to `samples/NodeTestApp/test.js`:

```javascript
// At the top with other requires:
const { makeEnumTestScenarios } = require('./test/EnumTests');

// In the testSuites array:
const testSuites = [
    {
        name: "Enum Tests",
        scenarios: makeEnumTestScenarios(testContext, TestComponent, TestValues),
    },
    // ... other suites
];
```

## Result

Original React Native test:
- ✅ 4 test scenarios
- ✅ Tests enum projections

Ported Node.js test:
- ✅ 4 test scenarios (same tests)
- ✅ Tests enum projections (same validations)
- ✅ Zero external dependencies
- ✅ Runs in pure Node.js
- ✅ Clean console output
