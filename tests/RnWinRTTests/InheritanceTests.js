// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

/**
 * @format
 */

import {
    TestScenario,
    assert,
} from './TestCommon'

export function makeInheritanceTestScenarios(pThis) {
    return [
        new TestScenario('Base class method from derived class', runBaseClassMethodFromDerivedClassTest.bind(pThis)),
        new TestScenario('Overridden base class method from derived class', runOverriddenBaseClassMethodFromDerivedClassTest.bind(pThis)),
        new TestScenario('Derived class method', runDerivedClassMethod.bind(pThis)),
        new TestScenario('Invalid derived class constructor', runInvalidDerivedClassConstructorTest.bind(pThis)),
        new TestScenario('Overloaded method test', runOverloadedMethodTest.bind(pThis)),
        new TestScenario('Static method from derived class', runStaticMethodFromDerivedClassTest.bind(pThis)),
        new TestScenario('V2 interface method', runV2InterfaceMethods.bind(pThis)),
        new TestScenario('Inaccesible method call', runExecuteNonAccessibleMethodTest.bind(pThis)),
    ];
}

function runInvalidDerivedClassConstructorTest(scenario) {
    this.runSync(scenario, () => {
        assert.throwsError(() => {
            const hierarchyDerived = new TestComponent.HierarchyDerived();
        }, "Error", "TypeError: No constructor overload exists for TestComponent.HierarchyDerived with 0 args");
    });
}

function runBaseClassMethodFromDerivedClassTest(scenario) {
    this.runSync(scenario, () => {
        const hierarchyBase = new TestComponent.HierarchyBase();
        const hierarchyDerived = new TestComponent.HierarchyDerived("");
        assert.equal(hierarchyBase.nonOverriddenHierarchyBaseMethod(), "HierarchyBase.NonOverriddenHierarchyBaseMethod");
        assert.equal(hierarchyDerived.nonOverriddenHierarchyBaseMethod(), "HierarchyBase.NonOverriddenHierarchyBaseMethod");
    });
}

function runOverriddenBaseClassMethodFromDerivedClassTest(scenario) {
    this.runSync(scenario, () => {
        const hierarchyBase = new TestComponent.HierarchyBase();
        const hierarchyDerived = new TestComponent.HierarchyDerived("");
        assert.equal(hierarchyBase.overriddenHierarchyBaseMethod(), "HierarchyBase.OverriddenHierarchyBaseMethod");
        assert.equal(hierarchyDerived.overriddenHierarchyBaseMethod(), "HierarchyDerived.OverriddenHierarchyBaseMethod");
    });
}

function runDerivedClassMethod(scenario) {
    this.runSync(scenario, () => {
        const hierarchyDerived = new TestComponent.HierarchyDerived("");
        assert.equal(hierarchyDerived.hierarchyDerivedMethod(), "HierarchyDerived.HierarchyDerivedMethod");
    });
}

function runOverloadedMethodTest(scenario) {
    this.runSync(scenario, () => {
        const hierarchyBase = new TestComponent.HierarchyBase();
        const hierarchyDerived = new TestComponent.HierarchyDerived("");
        assert.equal(hierarchyBase.overloadedHierarchyBaseMethod("hello"), "HierarchyBase.OverloadedHierarchyBaseMethod");
        assert.equal(hierarchyDerived.overloadedHierarchyBaseMethod("hello", "world"), "HierarchyDerived.OverloadedHierarchyBaseMethod");
    });
}

function runStaticMethodFromDerivedClassTest(scenario) {
    this.runSync(scenario, () => {
        assert.equal(TestComponent.HierarchyBase.staticHierarchyBaseMethod(), "HierarchyBase.StaticHierarchyBaseMethod");
        // TODO: remove this comment when this support is added
        // assert.equal(TestComponent.HierarchyDerived.staticHierarchyBaseMethod(), "HierarchyBase.StaticHierarchyBaseMethod");
    });
}

function runV2InterfaceMethods(scenario) {
    this.runSync(scenario, () => {
        const hierarchyBase = new TestComponent.HierarchyBase();
        const hierarchyDerived = new TestComponent.HierarchyDerived("");
        assert.equal(hierarchyBase.iHierarchyV2ContractMethod(), "HierarchyBase.IHierarchyV2ContractMethod");
        assert.equal(hierarchyDerived.iHierarchyV2ContractMethod(), "HierarchyBase.IHierarchyV2ContractMethod");
    });
}

function runExecuteNonAccessibleMethodTest(scenario) {    
    this.runSync(scenario, () => {
        const hierarchyBase = new TestComponent.HierarchyBase();
        const hierarchyDerived = new TestComponent.HierarchyDerived("");
        
        assert.throwsError(() => {    
            hierarchyBase.overriddenHierarchyBaseMethodOverride();
        }, "TypeError");

        assert.throwsError(() => {    
            hierarchyDerived.overriddenHierarchyBaseMethodOverride();
        }, "TypeError");
        
        assert.throwsError(() => {
            hierarchyBase.inaccessibleHierarchyBaseMethod();
        }, "TypeError");

        assert.throwsError(() => {
            hierarchyDerived.inaccessibleHierarchyBaseMethod();
        }, "TypeError");
    });
}