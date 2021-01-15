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

export function makeBasicFunctionTestScenarios(pThis) {
    return [
        // Static functions
        new TestScenario('Test::StaticOr', runStaticOr.bind(pThis)),
        new TestScenario('Test::StaticOrAll', runStaticOrAll.bind(pThis)),
        new TestScenario('Test::StaticAdd', runStaticAdd.bind(pThis)),
        new TestScenario('Test::StaticAddAll', runStaticAddAll.bind(pThis)),
        new TestScenario('Test::StaticAppend', runStaticAppend.bind(pThis)),
        new TestScenario('Test::StaticAppendAll', runStaticAppendAll.bind(pThis)),

        // Static out params
        new TestScenario('Test::StaticBoolOutParam', runStaticBoolOutParam.bind(pThis)),
        new TestScenario('Test::StaticCharOutParam', runStaticCharOutParam.bind(pThis)),
        new TestScenario('Test::StaticNumericOutParam', runStaticNumericOutParam.bind(pThis)),
        new TestScenario('Test::StaticStringOutParam', runStaticStringOutParam.bind(pThis)),
        new TestScenario('Test::StaticGuidOutParam', runStaticGuidOutParam.bind(pThis)),
        new TestScenario('Test::StaticEnumOutParam', runStaticEnumOutParam.bind(pThis)),
        new TestScenario('Test::StaticCompositeStructOutParam', runStaticCompositeStructOutParam.bind(pThis)),
        new TestScenario('Test::StaticRefOutParam', runStaticRefOutParam.bind(pThis)),
        new TestScenario('Test::StaticObjectOutParam', runStaticObjectOutParam.bind(pThis)),

        // Non-static functions
        new TestScenario('Test::Or', runOr.bind(pThis)),
        new TestScenario('Test::OrAll', runOrAll.bind(pThis)),
        new TestScenario('Test::Add', runAdd.bind(pThis)),
        new TestScenario('Test::AddAll', runAddAll.bind(pThis)),
        new TestScenario('Test::Append', runAppend.bind(pThis)),
        new TestScenario('Test::AppendAll', runAppendAll.bind(pThis)),

        // Non-static out params
        new TestScenario('Test::BoolOutParam', runBoolOutParam.bind(pThis)),
        new TestScenario('Test::CharOutParam', runCharOutParam.bind(pThis)),
        new TestScenario('Test::NumericOutParam', runNumericOutParam.bind(pThis)),
        new TestScenario('Test::StringOutParam', runStringOutParam.bind(pThis)),
        new TestScenario('Test::GuidOutParam', runGuidOutParam.bind(pThis)),
        new TestScenario('Test::EnumOutParam', runEnumOutParam.bind(pThis)),
        new TestScenario('Test::CompositeStructOutParam', runCompositeStructOutParam.bind(pThis)),
        new TestScenario('Test::RefOutParam', runRefOutParam.bind(pThis)),
        new TestScenario('Test::ObjectOutParam', runObjectOutParam.bind(pThis)),
    ];
}

// Static functions
function runStaticOr(scenario) {
    this.runSync(scenario, () => {
        assert.equal(TestComponent.Test.staticOr(false, false), false);
        assert.equal(TestComponent.Test.staticOr(false, true), true);
        assert.equal(TestComponent.Test.staticOr(true, false), true);
        assert.equal(TestComponent.Test.staticOr(true, true), true);
    });
}

function runStaticOrAll(scenario) {
    this.runSync(scenario, () => {
        var run = (arr, expect) => {
            assert.equal(TestComponent.Test.staticOrAll(arr), expect);
        };
        run([], false);
        run([false], false);
        run([true], true);
        run([false, false, false, false], false);
        run([true, false, false, false], true);
        run([false, true, false, false], true);
        run([false, false, true, false], true);
        run([false, false, false, true ], true);
    });
}

function runStaticAdd(scenario) {
    this.runSync(scenario, () => {
        assert.equal(TestComponent.Test.staticAdd(0, 0), 0);
        assert.equal(TestComponent.Test.staticAdd(0, 42), 42);
        assert.equal(TestComponent.Test.staticAdd(42, 0), 42);
        assert.equal(TestComponent.Test.staticAdd(42, 42), 84);
        assert.equal(TestComponent.Test.staticAdd(42, -42), 0);
    });
}

function runStaticAddAll(scenario) {
    this.runSync(scenario, () => {
        var run = (arr, expect) => {
            assert.equal(TestComponent.Test.staticAddAll(arr), expect);
        };
        run([], 0);
        run([1], 1);
        run([1, 2, 3], 6);
        run([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 55);
    });
}

function runStaticAppend(scenario) {
    this.runSync(scenario, () => {
        assert.equal(TestComponent.Test.staticAppend('foo', '\0', 'bar'), 'foo\0bar');
        assert.equal(TestComponent.Test.staticAppend('Hello', ' ', 'world'), 'Hello world');
    })
}

function runStaticAppendAll(scenario) {
    this.runSync(scenario, () => {
        var run = (arr, expect) => {
            assert.equal(TestComponent.Test.staticAppendAll(arr), expect);
        };
        run([], '');
        run(['foo'], 'foo');
        run(['f\0o\0o', '\0', 'b\0a\0r'], 'f\0o\0o\0b\0a\0r');
    })
}

// Static out params
function runStaticBoolOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (lhs, rhs) => {
            var { returnValue, and, or } = TestComponent.Test.staticBoolOutParam(lhs, rhs);
            assert.equal(and, lhs && rhs);
            assert.equal(or, lhs || rhs);
            assert.equal(returnValue, lhs ^ rhs ? true : false);
        };
        run(false, false);
        run(false, true);
        run(true, false);
        run(true, true);
    });
}

function runStaticCharOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (ch) => {
            var { returnValue, next, prev } = TestComponent.Test.staticCharOutParam(ch);
            assert.equal(next, String.fromCharCode(ch.charCodeAt(0) + 1));
            assert.equal(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
            assert.equal(returnValue, ch.toUpperCase());
        };

        for (var val of TestValues.chars.valid) {
            run(val);
        }
    });
}

function runStaticNumericOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, doubledValue, tripledValue } = TestComponent.Test.staticNumericOutParam(val);
            assert.equal(doubledValue, val * 2);
            assert.equal(tripledValue, val * 3);
            assert.equal(returnValue, val * 4);
        };
        run(0);
        run(42);
        run(-1);
    });
}

function runStaticStringOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, lower, upper } = TestComponent.Test.staticStringOutParam(val);
            assert.equal(lower, val.toLowerCase());
            assert.equal(upper, val.toUpperCase());
            assert.equal(returnValue, val.split('').reverse().join(''));
        };

        for (var val of TestValues.strings.valid) {
            run(val);
        }
    });
}

function runStaticGuidOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, zero, allSet } = TestComponent.Test.staticGuidOutParam(val);
            assert.equal(zero, zeroGuid);
            assert.equal(allSet, allSetGuid);
            assert.equal(returnValue, val);
        };

        for (var val of TestValues.guids.valid) {
            run(val);
        }
    });
}

function runStaticEnumOutParam(scenario) {
    this.runSync(scenario, () => {
        var { returnValue, plusOne, plusTwo } = TestComponent.Test.staticEnumOutParam(TestComponent.TestEnum.first);
        assert.equal(plusOne, TestComponent.TestEnum.second);
        assert.equal(plusTwo, TestComponent.TestEnum.third);
        assert.equal(returnValue, TestComponent.TestEnum.fourth);
    });
}

function runStaticCompositeStructOutParam(scenario) {
    this.runSync(scenario, () => {
        var testVal = {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 32.5, f64: 64.64, e: TestComponent.TestEnum.second },
            strings: { ch: 'F', str: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
            bools: { b: true }
        };

        var { returnValue, first, second } = TestComponent.Test.staticCompositeStructOutParam(testVal);
        assert.equal(returnValue, testVal);
        assert.equal(first, {
            numerics: { u8: 9, u16: 17, u32: 33, u64: 65, s16: -15, s32: -31, s64: -63, f32: 33.5, f64: 65.64, e: TestComponent.TestEnum.third },
            strings: { ch: 'G', str: 'hello, world!', guid: zeroGuid },
            bools: { b: false }
        });
        assert.equal(second, {
            numerics: { u8: 7, u16: 15, u32: 31, u64: 63, s16: -17, s32: -33, s64: -65, f32: 31.5, f64: 63.64, e: TestComponent.TestEnum.first },
            strings: { ch: 'E', str: 'HELLO, WORLD!', guid: allSetGuid },
            bools: { b: true }
        });
    });
}

function runStaticRefOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, doubledValue, tripledValue } = TestComponent.Test.staticRefOutParam(val);
            assert.equal(doubledValue, val * 2);
            assert.equal(tripledValue, val * 3);
            assert.equal(returnValue, val * 4);
        };
        run(0);
        run(42);
        run(-1);
    });
}

function runStaticObjectOutParam(scenario) {
    this.runSync(scenario, () => {
        var val = TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4]);
        var { returnValue, doubledValues, tripledValues } = TestComponent.Test.staticObjectOutParam(val);
        assert.isTrue(val == returnValue);

        for (var i = 0; i < val.size; ++i) {
            assert.equal(doubledValues.getAt(i), val.getAt(i) * 2);
            assert.equal(tripledValues.getAt(i), val.getAt(i) * 3);
        }
    });
}

// Non-static functions
function runOr(scenario) {
    this.runSync(scenario, () => {
        assert.equal(this.test.or(false, false), false);
        assert.equal(this.test.or(false, true), true);
        assert.equal(this.test.or(true, false), true);
        assert.equal(this.test.or(true, true), true);
    });
}

function runOrAll(scenario) {
    this.runSync(scenario, () => {
        var run = (arr, expect) => {
            assert.equal(this.test.orAll(arr), expect);
        };
        run([], false);
        run([false], false);
        run([true], true);
        run([false, false, false, false], false);
        run([true, false, false, false], true);
        run([false, true, false, false], true);
        run([false, false, true, false], true);
        run([false, false, false, true ], true);
    });
}

function runAdd(scenario) {
    this.runSync(scenario, () => {
        assert.equal(this.test.add(0, 0), 0);
        assert.equal(this.test.add(0, 42), 42);
        assert.equal(this.test.add(42, 0), 42);
        assert.equal(this.test.add(42, 42), 84);
        assert.equal(this.test.add(42, -42), 0);
    })
}

function runAddAll(scenario) {
    this.runSync(scenario, () => {
        var run = (arr, expect) => {
            assert.equal(this.test.addAll(arr), expect);
        };
        run([], 0);
        run([1], 1);
        run([1, 2, 3], 6);
        run([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 55);
    });
}

function runAppend(scenario) {
    this.runSync(scenario, () => {
        assert.equal(this.test.append('foo', '\0', 'bar'), 'foo\0bar');
        assert.equal(this.test.append('Hello', ' ', 'world'), 'Hello world');
    })
}

function runAppendAll(scenario) {
    this.runSync(scenario, () => {
        var run = (arr, expect) => {
            assert.equal(this.test.appendAll(arr), expect);
        };
        run([], '');
        run(['foo'], 'foo');
        run(['f\0o\0o', '\0', 'b\0a\0r'], 'f\0o\0o\0b\0a\0r');
    })
}

// Static out params
function runBoolOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (lhs, rhs) => {
            var { returnValue, and, or } = this.test.boolOutParam(lhs, rhs);
            assert.equal(and, lhs && rhs);
            assert.equal(or, lhs || rhs);
            assert.equal(returnValue, lhs ^ rhs ? true : false);
        };
        run(false, false);
        run(false, true);
        run(true, false);
        run(true, true);
    });
}

function runCharOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (ch) => {
            var { returnValue, next, prev } = this.test.charOutParam(ch);
            assert.equal(next, String.fromCharCode(ch.charCodeAt(0) + 1));
            assert.equal(prev, String.fromCharCode(ch.charCodeAt(0) - 1));
            assert.equal(returnValue, ch.toUpperCase());
        };

        for (var val of TestValues.chars.valid) {
            run(val);
        }
    });
}

function runNumericOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, doubledValue, tripledValue } = this.test.numericOutParam(val);
            assert.equal(doubledValue, val * 2);
            assert.equal(tripledValue, val * 3);
            assert.equal(returnValue, val * 4);
        };
        run(0);
        run(42);
        run(-1);
    });
}

function runStringOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, lower, upper } = this.test.stringOutParam(val);
            assert.equal(lower, val.toLowerCase());
            assert.equal(upper, val.toUpperCase());
            assert.equal(returnValue, val.split('').reverse().join(''));
        };

        for (var val of TestValues.strings.valid) {
            run(val);
        }
    });
}

function runGuidOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, zero, allSet } = this.test.guidOutParam(val);
            assert.equal(zero, zeroGuid);
            assert.equal(allSet, allSetGuid);
            assert.equal(returnValue, val);
        };

        for (var val of TestValues.guids.valid) {
            run(val);
        }
    });
}

function runEnumOutParam(scenario) {
    this.runSync(scenario, () => {
        var { returnValue, plusOne, plusTwo } = this.test.enumOutParam(TestComponent.TestEnum.first);
        assert.equal(plusOne, TestComponent.TestEnum.second);
        assert.equal(plusTwo, TestComponent.TestEnum.third);
        assert.equal(returnValue, TestComponent.TestEnum.fourth);
    });
}

function runCompositeStructOutParam(scenario) {
    this.runSync(scenario, () => {
        var testVal = {
            numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 32.5, f64: 64.64, e: TestComponent.TestEnum.second },
            strings: { ch: 'F', str: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
            bools: { b: true }
        };

        var { returnValue, first, second } = this.test.compositeStructOutParam(testVal);
        assert.equal(returnValue, testVal);
        assert.equal(first, {
            numerics: { u8: 9, u16: 17, u32: 33, u64: 65, s16: -15, s32: -31, s64: -63, f32: 33.5, f64: 65.64, e: TestComponent.TestEnum.third },
            strings: { ch: 'G', str: 'hello, world!', guid: zeroGuid },
            bools: { b: false }
        });
        assert.equal(second, {
            numerics: { u8: 7, u16: 15, u32: 31, u64: 63, s16: -17, s32: -33, s64: -65, f32: 31.5, f64: 63.64, e: TestComponent.TestEnum.first },
            strings: { ch: 'E', str: 'HELLO, WORLD!', guid: allSetGuid },
            bools: { b: true }
        });
    });
}

function runRefOutParam(scenario) {
    this.runSync(scenario, () => {
        var run = (val) => {
            var { returnValue, doubledValue, tripledValue } = this.test.refOutParam(val);
            assert.equal(doubledValue, val * 2);
            assert.equal(tripledValue, val * 3);
            assert.equal(returnValue, val * 4);
        };
        run(0);
        run(42);
        run(-1);
    });
}

function runObjectOutParam(scenario) {
    this.runSync(scenario, () => {
        var val = TestComponent.Test.makeNumericVector([0, 1, 2, 3, 4]);
        var { returnValue, doubledValues, tripledValues } = this.test.objectOutParam(val);
        assert.isTrue(val == returnValue);

        for (var i = 0; i < val.size; ++i) {
            assert.equal(doubledValues.getAt(i), val.getAt(i) * 2);
            assert.equal(tripledValues.getAt(i), val.getAt(i) * 3);
        }
    });
}
