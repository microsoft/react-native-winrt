// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

/**
 * @format
 */

const {
    TestScenario,
    assert,
    guidFromString,
    makeGuid
} = require('./TestCommon');

function makeArrayTestScenarios(pThis, TestComponent, TestValues) {
    return [
        // Static array out params
        new TestScenario('Test::StaticBoolArrayOutParam', () => runStaticBoolArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCharArrayOutParam', () => runStaticCharArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticNumericArrayOutParam', () => runStaticNumericArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticStringArrayOutParam', () => runStaticStringArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticGuidArrayOutParam', () => runStaticGuidArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticEnumArrayOutParam', () => runStaticEnumArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCompositeStructArrayOutParam', () => runStaticCompositeStructArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticRefArrayOutParam', () => runStaticRefArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticObjectArrayOutParam', () => runStaticObjectArrayOutParam(pThis, TestComponent, TestValues)),

        // Static array fill params
        new TestScenario('Test::StaticBoolFillParam', () => runStaticBoolFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCharFillParam', () => runStaticCharFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticNumericFillParam', () => runStaticNumericFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticStringFillParam', () => runStaticStringFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticGuidFillParam', () => runStaticGuidFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticEnumFillParam', () => runStaticEnumFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCompositeStructFillParam', () => runStaticCompositeStructFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticRefFillParam', () => runStaticRefFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticObjectFillParam', () => runStaticObjectFillParam(pThis, TestComponent, TestValues)),

        // Non-static array out params
        new TestScenario('Test::BoolArrayOutParam', () => runBoolArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CharArrayOutParam', () => runCharArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::NumericArrayOutParam', () => runNumericArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StringArrayOutParam', () => runStringArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::GuidArrayOutParam', () => runGuidArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::EnumArrayOutParam', () => runEnumArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CompositeStructArrayOutParam', () => runCompositeStructArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::RefArrayOutParam', () => runRefArrayOutParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::ObjectArrayOutParam', () => runObjectArrayOutParam(pThis, TestComponent, TestValues)),

        // Non-static array fill params
        new TestScenario('Test::BoolFillParam', () => runBoolFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CharFillParam', () => runCharFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::NumericFillParam', () => runNumericFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StringFillParam', () => runStringFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::GuidFillParam', () => runGuidFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::EnumFillParam', () => runEnumFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CompositeStructFillParam', () => runCompositeStructFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::RefFillParam', () => runRefFillParam(pThis, TestComponent, TestValues)),
        new TestScenario('Test::ObjectFillParam', () => runObjectFillParam(pThis, TestComponent, TestValues)),
    ];
}

// Static array out params
function validateRotatedArray(input, output, amt) {
    assert.equal(input.length, output.length);
    var i = 0, j = amt;
    for (; j < input.length; ++i, ++j) {
        assert.equal(input[i], output[j]);
    }

    j = 0;
    for (; i < input.length; ++i, ++j) {
        assert.equal(input[i], output[j]);
    }
}

function validateReversedArray(input, output) {
    assert.equal(input.length, output.length);
    for (var i = 0, j = input.length - 1; i < input.length; ++i, --j) {
        assert.equal(input[i], output[j]);
    }
}

function validateArrayOutParam(arr, fn) {
    var { returnValue, rot1, rot2 } = fn(arr);
    validateRotatedArray(arr, rot1, 1);
    validateRotatedArray(arr, rot2, 2);
    validateReversedArray(arr, returnValue);
}

function runStaticBoolArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticBoolArrayOutParam(val);
        validateArrayOutParam([ false, false, true, true, false ], fn);
    });
}

function runStaticCharArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticCharArrayOutParam(val);
        validateArrayOutParam([ 'A', 'B', 'C', 'D', 'E' ], fn);
    });
}

function runStaticNumericArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticNumericArrayOutParam(val);
        validateArrayOutParam([ 0, 1, 2, 3, 4 ], fn);
    });
}

function runStaticStringArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticStringArrayOutParam(val);
        validateArrayOutParam([ 'foo', 'bar', 'baz', 'foobar', 'foo\0bar' ], fn);
    });
}

function runStaticGuidArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticGuidArrayOutParam(val);
        validateArrayOutParam(TestValues.guids.valid, fn);
    });
}

function runStaticEnumArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticEnumArrayOutParam(val);
        validateArrayOutParam(TestValues.enums.valid, fn);
    });
}

function runStaticCompositeStructArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticCompositeStructArrayOutParam(val);
        validateArrayOutParam(TestValues.composite.valid, fn);
    });
}

function runStaticRefArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticRefArrayOutParam(val);
        validateArrayOutParam(TestValues.s32.valid, fn);
    });
}

function runStaticObjectArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var fn = (val) => TestComponent.Test.staticObjectArrayOutParam(val);
        validateArrayOutParam(TestValues.s32.valid.map(val => new TestComponent.TestObject(val)), fn);
    });
}

// Static array fill params
function runStaticBoolFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticBoolFillParam(arr);

            var expect = false;
            for (var val of arr) {
                assert.equal(val, expect);
                expect = !expect;
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runStaticCharFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticCharFillParam(arr);

            var expect = 'a'.charCodeAt(0);
            for (var val of arr) {
                assert.equal(val.charCodeAt(0), expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runStaticNumericFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticNumericFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(val, expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runStaticStringFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticStringFillParam(arr);

            var expect = '';
            for (var val of arr) {
                assert.equal(val, expect);
                expect += 'a';
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runStaticGuidFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticGuidFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                var guid = guidFromString(val);
                assert.equal(guid.data1, expect);
                assert.equal(guid.data2, expect);
                assert.equal(guid.data3, expect);
                for (var byte of guid.data4) assert.equal(byte, expect);
                ++expect;
            }
        };
        run(0);
        run(1);
        run(2);
        // run(100);
    });
}

function runStaticEnumFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticEnumFillParam(arr);

            var expect = TestComponent.TestEnum.first;
            for (var val of arr) {
                assert.equal(val, expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(4);
    });
}

function runStaticCompositeStructFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticCompositeStructFillParam(arr);

            var expectNumeric = 0;
            var expectString = '';
            var expectBool = false;
            for (var val of arr) {
                assert.equal(val, {
                    numerics: { u8: expectNumeric, u16: expectNumeric, u32: expectNumeric, u64: expectNumeric, s16: expectNumeric,
                            s32: expectNumeric, s64: expectNumeric, f32: expectNumeric, f64: expectNumeric, enum: expectNumeric },
                    strings: { char: String.fromCharCode(expectNumeric), string: expectString,
                        guid: makeGuid(expectNumeric, expectNumeric, expectNumeric, [expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric]) },
                    bools: { value: expectBool }
                });
                ++expectNumeric;
                expectString += 'a';
                expectBool = !expectBool;
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runStaticRefFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticRefFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(expect++, val);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runStaticObjectFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticObjectFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(expect++, val.value);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

// Non-static array out params
function runBoolArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.boolArrayOutParam(val);
        validateArrayOutParam([ false, false, true, true, false ], fn);
    });
}

function runCharArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.charArrayOutParam(val);
        validateArrayOutParam([ 'A', 'B', 'C', 'D', 'E' ], fn);
    });
}

function runNumericArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.numericArrayOutParam(val);
        validateArrayOutParam([ 0, 1, 2, 3, 4 ], fn);
    });
}

function runStringArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.stringArrayOutParam(val);
        validateArrayOutParam([ 'foo', 'bar', 'baz', 'foobar', 'foo\0bar' ], fn);
    });
}

function runGuidArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.guidArrayOutParam(val);
        validateArrayOutParam(TestValues.guids.valid, fn);
    });
}

function runEnumArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.enumArrayOutParam(val);
        validateArrayOutParam(TestValues.enums.valid, fn);
    });
}

function runCompositeStructArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.compositeStructArrayOutParam(val);
        validateArrayOutParam(TestValues.composite.valid, fn);
    });
}

function runRefArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.refArrayOutParam(val);
        validateArrayOutParam([ 0, 1, 2, 3, 4 ], fn);
    });
}

function runObjectArrayOutParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var fn = (val) => test.objectArrayOutParam(val);
        validateArrayOutParam(TestValues.s32.valid.map(val => new TestComponent.TestObject(val)), fn);
    });
}

// Non-static array fill params
function runBoolFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.boolFillParam(arr);

            var expect = false;
            for (var val of arr) {
                assert.equal(val, expect);
                expect = !expect;
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runCharFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.charFillParam(arr);

            var expect = 'a'.charCodeAt(0);
            for (var val of arr) {
                assert.equal(val.charCodeAt(0), expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runNumericFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.numericFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(val, expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runStringFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.stringFillParam(arr);

            var expect = '';
            for (var val of arr) {
                assert.equal(val, expect);
                expect += 'a';
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runGuidFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.guidFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                var guid = guidFromString(val);
                assert.equal(guid.data1, expect);
                assert.equal(guid.data2, expect);
                assert.equal(guid.data3, expect);
                for (var byte of guid.data4) assert.equal(byte, expect);
                ++expect;
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runEnumFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.enumFillParam(arr);

            var expect = TestComponent.TestEnum.first;
            for (var val of arr) {
                assert.equal(val, expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(4);
    });
}

function runCompositeStructFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.compositeStructFillParam(arr);

            var expectNumeric = 0;
            var expectString = '';
            var expectBool = false;
            for (var val of arr) {
                assert.equal(val, {
                    numerics: { u8: expectNumeric, u16: expectNumeric, u32: expectNumeric, u64: expectNumeric, s16: expectNumeric,
                            s32: expectNumeric, s64: expectNumeric, f32: expectNumeric, f64: expectNumeric, enum: expectNumeric },
                    strings: { char: String.fromCharCode(expectNumeric), string: expectString,
                        guid: makeGuid(expectNumeric, expectNumeric, expectNumeric, [expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric, expectNumeric]) },
                    bools: { value: expectBool }
                });
                ++expectNumeric;
                expectString += 'a';
                expectBool = !expectBool;
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runRefFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.refFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(expect++, val);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

function runObjectFillParam(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var run = (size) => {
            var arr = new Array(size);
            test.objectFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(expect++, val.value);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    });
}

module.exports = { makeArrayTestScenarios };