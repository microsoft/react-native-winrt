/**
 * @format
 */

import {
    TestScenario,
    assert
} from './TestCommon'

export function makeDelegateAndEventTestScenarios(pThis) {
    return [
        // Static events
        // TODO: AV when interface does not support weak ref: https://github.com/microsoft/jswinrt/issues/13
        // new TestScenario('Test::StaticBoolEventHandler', runStaticBoolEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticCharEventHandler', runStaticCharEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticNumericEventHandler', runStaticNumericEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticStringEventHandler', runStaticStringEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticGuidEventHandler', runStaticGuidEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticEnumEventHandler', runStaticEnumEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticCompositeStructEventHandler', runStaticCompositeStructEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticRefEventHandler', runStaticRefEventHandler.bind(pThis)),
        // new TestScenario('Test::StaticObjectEventHandler', runStaticObjectEventHandler.bind(pThis)),

        // Static Delegates
        new TestScenario('Test::StaticBoolDelegate', runStaticBoolDelegate.bind(pThis)),
        new TestScenario('Test::StaticCharDelegate', runStaticCharDelegate.bind(pThis)),
        new TestScenario('Test::StaticNumericDelegate', runStaticNumericDelegate.bind(pThis)),
        new TestScenario('Test::StaticStringDelegate', runStaticStringDelegate.bind(pThis)),
        new TestScenario('Test::StaticGuidDelegate', runStaticGuidDelegate.bind(pThis)),
        new TestScenario('Test::StaticEnumDelegate', runStaticEnumDelegate.bind(pThis)),
        new TestScenario('Test::StaticCompositeStructDelegate', runStaticCompositeStructDelegate.bind(pThis)),
        new TestScenario('Test::StaticRefDelegate', runStaticRefDelegate.bind(pThis)),
        new TestScenario('Test::StaticObjectDelegate', runStaticObjectDelegate.bind(pThis)),

        // Non-static events
        new TestScenario('Test::BoolEventHandler', runBoolEventHandler.bind(pThis)),
        new TestScenario('Test::CharEventHandler', runCharEventHandler.bind(pThis)),
        new TestScenario('Test::NumericEventHandler', runNumericEventHandler.bind(pThis)),
        new TestScenario('Test::StringEventHandler', runStringEventHandler.bind(pThis)),
        new TestScenario('Test::GuidEventHandler', runGuidEventHandler.bind(pThis)),
        new TestScenario('Test::EnumEventHandler', runEnumEventHandler.bind(pThis)),
        new TestScenario('Test::CompositeStructEventHandler', runCompositeStructEventHandler.bind(pThis)),
        new TestScenario('Test::RefEventHandler', runRefEventHandler.bind(pThis)),
        new TestScenario('Test::ObjectEventHandler', runObjectEventHandler.bind(pThis)),

        // Non-static Delegates
        new TestScenario('Test::BoolDelegate', runBoolDelegate.bind(pThis)),
        new TestScenario('Test::CharDelegate', runCharDelegate.bind(pThis)),
        new TestScenario('Test::NumericDelegate', runNumericDelegate.bind(pThis)),
        new TestScenario('Test::StringDelegate', runStringDelegate.bind(pThis)),
        new TestScenario('Test::GuidDelegate', runGuidDelegate.bind(pThis)),
        new TestScenario('Test::EnumDelegate', runEnumDelegate.bind(pThis)),
        new TestScenario('Test::CompositeStructDelegate', runCompositeStructDelegate.bind(pThis)),
        new TestScenario('Test::RefDelegate', runRefDelegate.bind(pThis)),
        new TestScenario('Test::ObjectDelegate', runObjectDelegate.bind(pThis)),
    ];
}

// Static events
function testStaticEventHandler(scenario, args, name, invoke) {
    this.runSync(scenario, () => {
        var i = 0;
        var invokeCount = 0;
        var handler = (sender, arg) => {
            try {
                assert.isTrue(i < args.length);
                assert.equal(sender, null);
                assert.equal(arg, args[i]);
            } catch (e) {
                // Since the caller ignores errors
                exception = e;
            }
            ++invokeCount;
        };

        TestComponent.Test.addEventListener(name, handler);
        for (var arg of args) {
            invoke(arg);
            if (exception) {
                throw exception;
            }
            ++i;
        }
        assert.equal(invokeCount, args.length);

        TestComponent.Test.removeEventListener(name, handler);
        invoke(args[0]);
        assert.equal(invokeCount, args.length);
    });
}

function runStaticBoolEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.boolTestValues, 'staticbooleventhandler', (arg) => TestComponent.Test.raiseStaticBoolEvent(arg));
}

function runStaticCharEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.charTestValues, 'staticchareventhandler', (arg) => TestComponent.Test.raiseStaticCharEvent(arg));
}

function runStaticNumericEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.s32TestValues, 'staticnumericeventhandler', (arg) => TestComponent.Test.raiseStaticNumericEvent(arg));
}

function runStaticStringEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.stringTestValues, 'staticstringeventhandler', (arg) => TestComponent.Test.raiseStaticStringEvent(arg));
}

function runStaticGuidEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.guidTestValues, 'staticguideventhandler', (arg) => TestComponent.Test.raiseStaticGuidEvent(arg));
}

function runStaticEnumEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.enumTestValues, 'staticenumeventhandler', (arg) => TestComponent.Test.raiseStaticEnumEvent(arg));
}

function runStaticCompositeStructEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.compositeTestValues, 'staticcompositeStructeventhandler', (arg) => TestComponent.Test.raiseStaticCompositeStructEvent(arg));
}

function runStaticRefEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, this.s32TestValues, 'staticrefeventhandler', (arg) => TestComponent.Test.raiseStaticRefEvent(arg));
}

function runStaticObjectEventHandler(scenario) {
    var vals = [TestComponent.Test.makeNumericVector([]), TestComponent.Test.makeNumericVector([0]), TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4])];
    testStaticEventHandler.call(this, scenario, vals, 'staticobjecteventhandler', (arg) => TestComponent.Test.raiseStaticObjectEvent(arg));
}

// Static delegates
function runDelegateTest(scenario, values, invoke) {
    this.runSync(scenario, () => {
        for (var val of values) {
            assert.equal(val, invoke(val, (value) => value));
        }

        var threw = false;
        try {
            // TODO: Exception message not preserved through propagation; should it?
            invoke(values[0], (value) => { throw new Error() });
        } catch (e) {
            threw = true;
        }
        assert.isTrue(threw);
    });
}

function runStaticBoolDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.boolTestValues, (val, fn) => TestComponent.Test.staticInvokeBoolDelegate(val, fn));
}

function runStaticCharDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.charTestValues, (val, fn) => TestComponent.Test.staticInvokeCharDelegate(val, fn));
}

function runStaticNumericDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.s32TestValues, (val, fn) => TestComponent.Test.staticInvokeNumericDelegate(val, fn));
}

function runStaticStringDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.stringTestValues, (val, fn) => TestComponent.Test.staticInvokeStringDelegate(val, fn));
}

function runStaticGuidDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.guidTestValues, (val, fn) => TestComponent.Test.staticInvokeGuidDelegate(val, fn));
}

function runStaticEnumDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.enumTestValues, (val, fn) => TestComponent.Test.staticInvokeEnumDelegate(val, fn));
}

function runStaticCompositeStructDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.compositeTestValues, (val, fn) => TestComponent.Test.staticInvokeCompositeStructDelegate(val, fn));
}

function runStaticRefDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.s32TestValues, (val, fn) => TestComponent.Test.staticInvokeRefDelegate(val, fn));
}

function runStaticObjectDelegate(scenario) {
    var values = [];
    for (var val of this.numericArrayTestValues) {
        values.push(TestComponent.Test.makeNumericVector(val));
    }

    runDelegateTest.call(this, scenario, values, (val, fn) => TestComponent.Test.staticInvokeObjectDelegate(val, fn));
}

// Non-static events
function testEventHandler(scenario, args, name, invoke) {
    this.runSync(scenario, () => {
        var i = 0;
        var invokeCount = 0;
        var exception;
        var handler = (sender, arg) => {
            try {
                assert.isTrue(i < args.length);
                assert.isTrue(sender == this.test);
                assert.equal(arg, args[i]);
            } catch (e) {
                // Since the caller ignores errors
                exception = e;
            }
            ++invokeCount;
        };

        this.test.addEventListener(name, handler);
        for (var arg of args) {
            invoke(arg);
            if (exception) {
                throw exception;
            }
            ++i;
        }
        assert.equal(invokeCount, args.length);

        this.test.removeEventListener(name, handler);
        invoke(args[0]);
        assert.equal(invokeCount, args.length);
    });
}

function runBoolEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.boolTestValues, 'booleventhandler', (arg) => this.test.raiseBoolEvent(arg));
}

function runCharEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.charTestValues, 'chareventhandler', (arg) => this.test.raiseCharEvent(arg));
}

function runNumericEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.s32TestValues, 'numericeventhandler', (arg) => this.test.raiseNumericEvent(arg));
}

function runStringEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.stringTestValues, 'stringeventhandler', (arg) => this.test.raiseStringEvent(arg));
}

function runGuidEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.guidTestValues, 'guideventhandler', (arg) => this.test.raiseGuidEvent(arg));
}

function runEnumEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.enumTestValues, 'enumeventhandler', (arg) => this.test.raiseEnumEvent(arg));
}

function runCompositeStructEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.compositeTestValues, 'compositestructeventhandler', (arg) => this.test.raiseCompositeStructEvent(arg));
}

function runRefEventHandler(scenario) {
    testEventHandler.call(this, scenario, this.s32TestValues, 'refeventhandler', (arg) => this.test.raiseRefEvent(arg));
}

function runObjectEventHandler(scenario) {
    var vals = [TestComponent.Test.makeNumericVector([]), TestComponent.Test.makeNumericVector([0]), TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4])];
    testEventHandler.call(this, scenario, vals, 'objecteventhandler', (arg) => this.test.raiseObjectEvent(arg));
}

// Non-static delegates
function runBoolDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.boolTestValues, (val, fn) => this.test.invokeBoolDelegate(val, fn));
}

function runCharDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.charTestValues, (val, fn) => this.test.invokeCharDelegate(val, fn));
}

function runNumericDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.s32TestValues, (val, fn) => this.test.invokeNumericDelegate(val, fn));
}

function runStringDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.stringTestValues, (val, fn) => this.test.invokeStringDelegate(val, fn));
}

function runGuidDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.guidTestValues, (val, fn) => this.test.invokeGuidDelegate(val, fn));
}

function runEnumDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.enumTestValues, (val, fn) => this.test.invokeEnumDelegate(val, fn));
}

function runCompositeStructDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.compositeTestValues, (val, fn) => this.test.invokeCompositeStructDelegate(val, fn));
}

function runRefDelegate(scenario) {
    runDelegateTest.call(this, scenario, this.s32TestValues, (val, fn) => this.test.invokeRefDelegate(val, fn));
}

function runObjectDelegate(scenario) {
    var values = [];
    for (var val of this.numericArrayTestValues) {
        values.push(TestComponent.Test.makeNumericVector(val));
    }

    runDelegateTest.call(this, scenario, values, (val, fn) => this.test.invokeObjectDelegate(val, fn));
}
