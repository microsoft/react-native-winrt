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
        assert.equal(Object.keys(TestComponent.TestEnum).sort(), ["1", "2", "3", "4", "first", "fourth", "second", "third"]);
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
