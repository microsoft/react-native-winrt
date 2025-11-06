// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * Miscellaneous Tests - Caching, Object Identity, etc.
 * @format
 */

import { TestScenario, assert } from './TestCommon.js';

export function makeMiscTestScenarios(pThis) {
    return [
        new TestScenario('StaticClassCaching', runStaticClassCaching.bind(pThis)),
        new TestScenario('ActivatableClassCaching', runActivatableClassCaching.bind(pThis)),
        new TestScenario('StaticPropertyPersistence', runStaticPropertyPersistence.bind(pThis)),
        new TestScenario('ConstructorCallable', runConstructorCallable.bind(pThis)),
        new TestScenario('StaticPropertiesOnConstructor', runStaticPropertiesOnConstructor.bind(pThis)),
        new TestScenario('NamespaceChildrenCached', runNamespaceChildrenCached.bind(pThis)),
        new TestScenario('SetterAndGetterOnDifferentVariables', setterAndGetterOnDifferentVariables.bind(pThis)),
    ];
}

function runStaticClassCaching(scenario) {
    this.runSync(scenario, () => {
        const t1 = TestComponent.StaticOnlyTest;
        const t2 = TestComponent.StaticOnlyTest;
        
        // Verify that we get the same object each time (caching works)
        assert.isTrue(t1 === t2, 'StaticOnlyTest should return the same cached object');
    });
}

function runActivatableClassCaching(scenario) {
    this.runSync(scenario, () => {
        const t1 = TestComponent.Test;
        const t2 = TestComponent.Test;
        
        // Verify that we get the same constructor function each time (caching works)
        assert.isTrue(t1 === t2, 'Test constructor should return the same cached function');
    });
}

function runStaticPropertyPersistence(scenario) {
    this.runSync(scenario, () => {
        // Set a value on the static property
        TestComponent.Test.staticU32Property = 12345;
        
        // Access Test again and verify the property persisted
        const value = TestComponent.Test.staticU32Property;
        assert.equal(value, 12345, 'Static property should persist across accesses');
        
        // Reset to 0 for other tests
        TestComponent.Test.staticU32Property = 0;
    });
}

function runConstructorCallable(scenario) {
    this.runSync(scenario, () => {
        // Verify that Test is a function (constructor)
        assert.equal(typeof TestComponent.Test, 'function', 'Test should be a function/constructor');
        
        // Verify we can call it as a constructor
        const instance = new TestComponent.Test();
        assert.isTrue(instance !== null && instance !== undefined, 'Should be able to create instance');
    });
}

function runStaticPropertiesOnConstructor(scenario) {
    this.runSync(scenario, () => {
        // Verify that static properties are accessible on the constructor
        const initialValue = TestComponent.Test.staticU32Property;
        assert.equal(typeof initialValue, 'number', 'Static property should be accessible on constructor');
        
        // Verify we can set it
        TestComponent.Test.staticU32Property = 999;
        assert.equal(TestComponent.Test.staticU32Property, 999, 'Should be able to set static property on constructor');
        
        // Reset
        TestComponent.Test.staticU32Property = 0;
    });
}

function runNamespaceChildrenCached(scenario) {
    this.runSync(scenario, () => {
        const tc1 = TestComponent;
        const tc2 = TestComponent;
        
        // Verify that namespace children (like Test and StaticOnlyTest) are cached
        assert.isTrue(tc1.Test === tc2.Test, 'Namespace children should be cached');
        assert.isTrue(tc1.StaticOnlyTest === tc2.StaticOnlyTest, 'Namespace children should be cached');
    });
}

function setterAndGetterOnDifferentVariables(scenario) {
    this.runSync(scenario, () => {
        const t1 = TestComponent.StaticOnlyTest; 
        const t2 = TestComponent.StaticOnlyTest;
        assert.isTrue(t1 === t2, 'StaticOnlyTest should return the same cached object');
        
        t1.u32Property = 42;
        assert.isTrue(t1.u32Property == 42, 't1.u32Property should be 42');
        assert.isTrue(t2.u32Property == 42, 't2.u32Property should be 42');
    });
}
