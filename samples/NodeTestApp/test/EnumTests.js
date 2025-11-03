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
        assert.equal(Object.keys(TestComponent.TestEnum).sort(), ["1", "2", "3", "4", "first", "fourth", "second", "third"]);
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
