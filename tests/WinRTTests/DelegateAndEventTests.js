/**
 * @format
 */

import {
    TestScenario,
    TestValues,
    assert,
    allSetGuid,
    zeroGuid
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

        new TestScenario('Test::StaticInvokeBoolArrayDelegate', runStaticBoolArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeCharArrayDelegate', runStaticCharArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeNumericArrayDelegate', runStaticNumericArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeStringArrayDelegate', runStaticStringArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeGuidArrayDelegate', runStaticGuidArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeEnumArrayDelegate', runStaticEnumArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeCompositeStructArrayDelegate', runStaticCompositeStructArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeRefArrayDelegate', runStaticRefArrayDelegate.bind(pThis)),
        new TestScenario('Test::StaticInvokeObjectArrayDelegate', runStaticObjectArrayDelegate.bind(pThis)),

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
    var vals = TestValues.s32.valid.map(val => new TestComponent.TestObject(val));
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
    var values = TestValues.s32.valid.map(val => new TestComponent.TestObject(val));
    runDelegateTest.call(this, scenario, values, (val, fn) => TestComponent.Test.staticInvokeObjectDelegate(val, fn));
}

// Static delegates with out params
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
    var values = TestValues.s32.valid.map(val => new TestComponent.TestObject(val));
    runDelegateWithOutParamTest.call(this, scenario, values, (val, fn) => TestComponent.Test.staticInvokeObjectDelegateWithOutParam(val, fn));
}

// Static array delegates
function runArrayDelegateTest(scenario, values, invoke, modify) {
    this.runSync(scenario, () => {
        assert.isTrue(invoke(values, (vals, fill) => {
            for (var i = 0; i < fill.length; ++i) {
                fill[i] = vals[i];
            }

            var out = new Array(vals.length);
            for (var i = 0; i < vals.length; ++i) {
                out[i] = vals[vals.length - i - 1];
            }

            return { returnValue: vals, outValue: out };
        }));
    });
}

function runStaticBoolArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, [true, false, true, false, true, false], (vals, fn) => TestComponent.Test.staticInvokeBoolArrayDelegate(vals, fn));
}

function runStaticCharArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, ['a', 'b', 'c', 'd', 'e', 'f'], (vals, fn) => TestComponent.Test.staticInvokeCharArrayDelegate(vals, fn));
}

function runStaticNumericArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, [1, 2, 3, 4, 5, 6], (vals, fn) => TestComponent.Test.staticInvokeNumericArrayDelegate(vals, fn));
}

function runStaticStringArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, ['apple', 'banana', 'cherry', 'durian', 'elderberry', 'fig'], (vals, fn) => TestComponent.Test.staticInvokeStringArrayDelegate(vals, fn));
}

function runStaticGuidArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario,
        [zeroGuid, allSetGuid, '11111111-1111-1111-1111-111111111111', '22222222-2222-2222-2222-222222222222', '33333333-3333-3333-3333-333333333333', '44444444-4444-4444-4444-444444444444'],
        (vals, fn) => TestComponent.Test.staticInvokeGuidArrayDelegate(vals, fn));
}

function runStaticEnumArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, [1, 2, 3, 4, 1, 2], (vals, fn) => TestComponent.Test.staticInvokeEnumArrayDelegate(vals, fn));
}

function runStaticCompositeStructArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, [...TestValues.composite.valid, ...TestValues.composite.valid], (vals, fn) => TestComponent.Test.staticInvokeCompositeStructArrayDelegate(vals, fn));
}

function runStaticRefArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, [1, 2, 3, 4, 5, 6], (vals, fn) => TestComponent.Test.staticInvokeRefArrayDelegate(vals, fn));
}

function runStaticObjectArrayDelegate(scenario) {
    runArrayDelegateTest.call(this, scenario, [
        new TestComponent.TestObject(1),
        new TestComponent.TestObject(2),
        new TestComponent.TestObject(3),
        new TestComponent.TestObject(4),
        new TestComponent.TestObject(5),
        new TestComponent.TestObject(6),
    ], (vals, fn) => TestComponent.Test.staticInvokeObjectArrayDelegate(vals, fn));
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
    var vals = TestValues.s32.valid.map(val => new TestComponent.TestObject(val));
    testEventHandler.call(this, scenario, vals, 'objecteventhandler', (arg) => this.test.raiseObjectEvent(arg));
}
