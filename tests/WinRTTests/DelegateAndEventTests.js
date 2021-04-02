/**
 * @format
 */

import {
    TestScenario,
    TestValues,
    assert
} from './TestCommon'

export function makeDelegateAndEventTestScenarios(pThis) {
    return [
        // Static events
        new TestScenario('Test::StaticBoolEventHandler', runStaticBoolEventHandler.bind(pThis)),
        new TestScenario('Test::StaticCharEventHandler', runStaticCharEventHandler.bind(pThis)),
        new TestScenario('Test::StaticNumericEventHandler', runStaticNumericEventHandler.bind(pThis)),
        new TestScenario('Test::StaticStringEventHandler', runStaticStringEventHandler.bind(pThis)),
        new TestScenario('Test::StaticGuidEventHandler', runStaticGuidEventHandler.bind(pThis)),
        new TestScenario('Test::StaticEnumEventHandler', runStaticEnumEventHandler.bind(pThis)),
        new TestScenario('Test::StaticCompositeStructEventHandler', runStaticCompositeStructEventHandler.bind(pThis)),
        new TestScenario('Test::StaticRefEventHandler', runStaticRefEventHandler.bind(pThis)),
        new TestScenario('Test::StaticObjectEventHandler', runStaticObjectEventHandler.bind(pThis)),

        // Static Delegates
        new TestScenario('Test::StaticInvokeBoolDelegate', runStaticBoolDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeCharDelegate', runStaticCharDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeNumericDelegate', runStaticNumericDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeStringDelegate', runStaticStringDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeGuidDelegate', runStaticGuidDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeEnumDelegate', runStaticEnumDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeCompositeStructDelegate', runStaticCompositeStructDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeRefDelegate', runStaticRefDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeObjectDelegate', runStaticObjectDelegate.bind(pThis)),

        new TestScenario('Test::StaticInvokeBoolDelegateWithOutParam', runStaticBoolDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeCharDelegateWithOutParam', runStaticCharDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeNumericDelegateWithOutParam', runStaticNumericDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeStringDelegateWithOutParam', runStaticStringDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeGuidDelegateWithOutParam', runStaticGuidDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeEnumDelegateWithOutParam', runStaticEnumDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeCompositeStructDelegateWithOutParam', runStaticCompositeStructDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeRefDelegateWithOutParam', runStaticRefDelegateWithOutParam.bind(pThis)),
        new TestScenario('Test::StaticInvokeObjectDelegateWithOutParam', runStaticObjectDelegateWithOutParam.bind(pThis)),

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
        var exception;
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
    testStaticEventHandler.call(this, scenario, TestValues.bools.valid, 'staticbooleventhandler', (arg) => TestComponent.Test.raiseStaticBoolEvent(arg));
}

function runStaticCharEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, TestValues.chars.valid, 'staticchareventhandler', (arg) => TestComponent.Test.raiseStaticCharEvent(arg));
}

function runStaticNumericEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, TestValues.s32.valid, 'staticnumericeventhandler', (arg) => TestComponent.Test.raiseStaticNumericEvent(arg));
}

function runStaticStringEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, TestValues.strings.valid, 'staticstringeventhandler', (arg) => TestComponent.Test.raiseStaticStringEvent(arg));
}

function runStaticGuidEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, TestValues.guids.valid, 'staticguideventhandler', (arg) => TestComponent.Test.raiseStaticGuidEvent(arg));
}

function runStaticEnumEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, TestValues.enums.valid, 'staticenumeventhandler', (arg) => TestComponent.Test.raiseStaticEnumEvent(arg));
}

function runStaticCompositeStructEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, TestValues.composite.valid, 'staticcompositestructeventhandler', (arg) => TestComponent.Test.raiseStaticCompositeStructEvent(arg));
}

function runStaticRefEventHandler(scenario) {
    testStaticEventHandler.call(this, scenario, TestValues.s32.valid, 'staticrefeventhandler', (arg) => TestComponent.Test.raiseStaticRefEvent(arg));
}

function runStaticObjectEventHandler(scenario) {
    var vals = [TestComponent.Test.copyNumericsToVector([]), TestComponent.Test.copyNumericsToVector([0]), TestComponent.Test.copyNumericsToVector([0, 1, 2, 3, 4])];
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
            invoke(values[0], () => { throw new Error() });
        } catch (e) {
            threw = true;
        }
        assert.isTrue(threw);
    });
}

function runStaticBoolDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.bools.valid, (val, fn) => TestComponent.Test.staticInvokeBoolDelegate(val, fn));
}

function runStaticCharDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.chars.valid, (val, fn) => TestComponent.Test.staticInvokeCharDelegate(val, fn));
}

function runStaticNumericDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.s32.valid, (val, fn) => TestComponent.Test.staticInvokeNumericDelegate(val, fn));
}

function runStaticStringDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.strings.valid, (val, fn) => TestComponent.Test.staticInvokeStringDelegate(val, fn));
}

function runStaticGuidDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.guids.valid, (val, fn) => TestComponent.Test.staticInvokeGuidDelegate(val, fn));
}

function runStaticEnumDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.enums.valid, (val, fn) => TestComponent.Test.staticInvokeEnumDelegate(val, fn));
}

function runStaticCompositeStructDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.composite.valid, (val, fn) => TestComponent.Test.staticInvokeCompositeStructDelegate(val, fn));
}

function runStaticRefDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.s32.valid, (val, fn) => TestComponent.Test.staticInvokeRefDelegate(val, fn));
}

function runStaticObjectDelegate(scenario) {
    var values = [];
    for (var val of TestValues.s32.validArrays) {
        values.push(TestComponent.Test.copyNumericsToVector(val));
    }

    runDelegateTest.call(this, scenario, values, (val, fn) => TestComponent.Test.staticInvokeObjectDelegate(val, fn));
}

function runDelegateWithOutParamTest(scenario, values, invoke) {
    this.runSync(scenario, () => {
        for (var val of values) {
            assert.equal(val, invoke(val, (value) => { return { outValue: value }; }));
        }

        var threw = false;
        try {
            // TODO: Exception message not preserved through propagation; should it?
            invoke(values[0], () => { throw new Error() });
        } catch (e) {
            threw = true;
        }
        assert.isTrue(threw);
    });
}

function runStaticBoolDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.bools.valid, (val, fn) => TestComponent.Test.staticInvokeBoolDelegateWithOutParam(val, fn));
}

function runStaticCharDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.chars.valid, (val, fn) => TestComponent.Test.staticInvokeCharDelegateWithOutParam(val, fn));
}

function runStaticNumericDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.s32.valid, (val, fn) => TestComponent.Test.staticInvokeNumericDelegateWithOutParam(val, fn));
}

function runStaticStringDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.strings.valid, (val, fn) => TestComponent.Test.staticInvokeStringDelegateWithOutParam(val, fn));
}

function runStaticGuidDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.guids.valid, (val, fn) => TestComponent.Test.staticInvokeGuidDelegateWithOutParam(val, fn));
}

function runStaticEnumDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.enums.valid, (val, fn) => TestComponent.Test.staticInvokeEnumDelegateWithOutParam(val, fn));
}

function runStaticCompositeStructDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.composite.valid, (val, fn) => TestComponent.Test.staticInvokeCompositeStructDelegateWithOutParam(val, fn));
}

function runStaticRefDelegateWithOutParam(scenario) {
    runDelegateWithOutParamTest.call(this, scenario, TestValues.s32.valid, (val, fn) => TestComponent.Test.staticInvokeRefDelegateWithOutParam(val, fn));
}

function runStaticObjectDelegateWithOutParam(scenario) {
   var values = [];
    for (var val of TestValues.s32.validArrays) {
        values.push(TestComponent.Test.copyNumericsToVector(val));
    }

    runDelegateWithOutParamTest.call(this, scenario, values, (val, fn) => TestComponent.Test.staticInvokeObjectDelegateWithOutParam(val, fn));
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
    testEventHandler.call(this, scenario, TestValues.bools.valid, 'booleventhandler', (arg) => this.test.raiseBoolEvent(arg));
}

function runCharEventHandler(scenario) {
    testEventHandler.call(this, scenario, TestValues.chars.valid, 'chareventhandler', (arg) => this.test.raiseCharEvent(arg));
}

function runNumericEventHandler(scenario) {
    testEventHandler.call(this, scenario, TestValues.s32.valid, 'numericeventhandler', (arg) => this.test.raiseNumericEvent(arg));
}

function runStringEventHandler(scenario) {
    testEventHandler.call(this, scenario, TestValues.strings.valid, 'stringeventhandler', (arg) => this.test.raiseStringEvent(arg));
}

function runGuidEventHandler(scenario) {
    testEventHandler.call(this, scenario, TestValues.guids.valid, 'guideventhandler', (arg) => this.test.raiseGuidEvent(arg));
}

function runEnumEventHandler(scenario) {
    testEventHandler.call(this, scenario, TestValues.enums.valid, 'enumeventhandler', (arg) => this.test.raiseEnumEvent(arg));
}

function runCompositeStructEventHandler(scenario) {
    testEventHandler.call(this, scenario, TestValues.composite.valid, 'compositestructeventhandler', (arg) => this.test.raiseCompositeStructEvent(arg));
}

function runRefEventHandler(scenario) {
    testEventHandler.call(this, scenario, TestValues.s32.valid, 'refeventhandler', (arg) => this.test.raiseRefEvent(arg));
}

function runObjectEventHandler(scenario) {
    var vals = [TestComponent.Test.copyNumericsToVector([]), TestComponent.Test.copyNumericsToVector([0]), TestComponent.Test.copyNumericsToVector([0, 1, 2, 3, 4])];
    testEventHandler.call(this, scenario, vals, 'objecteventhandler', (arg) => this.test.raiseObjectEvent(arg));
}

// Non-static delegates
function runBoolDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.bools.valid, (val, fn) => this.test.invokeBoolDelegate(val, fn));
}

function runCharDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.chars.valid, (val, fn) => this.test.invokeCharDelegate(val, fn));
}

function runNumericDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.s32.valid, (val, fn) => this.test.invokeNumericDelegate(val, fn));
}

function runStringDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.strings.valid, (val, fn) => this.test.invokeStringDelegate(val, fn));
}

function runGuidDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.guids.valid, (val, fn) => this.test.invokeGuidDelegate(val, fn));
}

function runEnumDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.enums.valid, (val, fn) => this.test.invokeEnumDelegate(val, fn));
}

function runCompositeStructDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.composite.valid, (val, fn) => this.test.invokeCompositeStructDelegate(val, fn));
}

function runRefDelegate(scenario) {
    runDelegateTest.call(this, scenario, TestValues.s32.valid, (val, fn) => this.test.invokeRefDelegate(val, fn));
}

function runObjectDelegate(scenario) {
    var values = [];
    for (var val of TestValues.s32.validArrays) {
        values.push(TestComponent.Test.copyNumericsToVector(val));
    }

    runDelegateTest.call(this, scenario, values, (val, fn) => this.test.invokeObjectDelegate(val, fn));
}
