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
    ];
}


function runBaseClassMethodFromDerivedClassTest(scenario) {
    this.runSync(scenario, () => {
        const hierarchyBase = new TestComponent.HierarchyBase();
        const hierarchyDerived = new TestComponent.HierarchyDerived();
        assert.equal(hierarchyBase.nonOverriddenHierarchyBaseMethod(), "HierarchyBase.NonOverriddenHierarchyBaseMethod");
        assert.equal(hierarchyDerived.nonOverriddenHierarchyBaseMethod(), "HierarchyBase.NonOverriddenHierarchyBaseMethod");
    });
}

function runOverriddenBaseClassMethodFromDerivedClassTest(scenario) {
    this.runSync(scenario, () => {
        const hierarchyBase = new TestComponent.HierarchyBase();
        const hierarchyDerived = new TestComponent.HierarchyDerived();
        assert.equal(hierarchyBase.overriddenHierarchyBaseMethod(), "HierarchyBase.OverriddenHierarchyBaseMethod");
        assert.equal(hierarchyDerived.overriddenHierarchyBaseMethod(), "HierarchyDerived.OverriddenHierarchyBaseMethod");
    });
}

function runDerivedClassMethod(scenario) {
    this.runSync(scenario, () => {
        const hierarchyDerived = new TestComponent.HierarchyDerived();
        assert.equal(hierarchyDerived.hierarchyDerivedMethod(), "HierarchyDerived.HierarchyDerivedMethod");
    });
}