/**
 * @format
 */

import {
    TestScenario,
    TestValues,
    assert,
    guidFromString,
    makeGuid
} from './TestCommon'

export function makeArrayTestScenarios(pThis) {
    return [
        // Static array out params
        new TestScenario('Test::StaticBoolArrayOutParam', runStaticBoolArrayOutParam.bind(pThis)),
        new TestScenario('Test::StaticCharArrayOutParam', runStaticCharArrayOutParam.bind(pThis)),
        new TestScenario('Test::StaticNumericArrayOutParam', runStaticNumericArrayOutParam.bind(pThis)),
        new TestScenario('Test::StaticStringArrayOutParam', runStaticStringArrayOutParam.bind(pThis)),
        new TestScenario('Test::StaticGuidArrayOutParam', runStaticGuidArrayOutParam.bind(pThis)),
        new TestScenario('Test::StaticEnumArrayOutParam', runStaticEnumArrayOutParam.bind(pThis)),
        new TestScenario('Test::StaticCompositeStructArrayOutParam', runStaticCompositeStructArrayOutParam.bind(pThis)),
        new TestScenario('Test::StaticObjectArrayOutParam', runStaticObjectArrayOutParam.bind(pThis)),

        // Static array fill params
        new TestScenario('Test::StaticBoolFillParam', runStaticBoolFillParam.bind(pThis)),
        new TestScenario('Test::StaticCharFillParam', runStaticCharFillParam.bind(pThis)),
        new TestScenario('Test::StaticNumericFillParam', runStaticNumericFillParam.bind(pThis)),
        new TestScenario('Test::StaticStringFillParam', runStaticStringFillParam.bind(pThis)),
        new TestScenario('Test::StaticGuidFillParam', runStaticGuidFillParam.bind(pThis)),
        new TestScenario('Test::StaticEnumFillParam', runStaticEnumFillParam.bind(pThis)),
        new TestScenario('Test::StaticCompositeStructFillParam', runStaticCompositeStructFillParam.bind(pThis)),
        new TestScenario('Test::StaticObjectFillParam', runStaticObjectFillParam.bind(pThis)),

        // Non-static array out params
        new TestScenario('Test::BoolArrayOutParam', runBoolArrayOutParam.bind(pThis)),
        new TestScenario('Test::CharArrayOutParam', runCharArrayOutParam.bind(pThis)),
        new TestScenario('Test::NumericArrayOutParam', runNumericArrayOutParam.bind(pThis)),
        new TestScenario('Test::StringArrayOutParam', runStringArrayOutParam.bind(pThis)),
        new TestScenario('Test::GuidArrayOutParam', runGuidArrayOutParam.bind(pThis)),
        new TestScenario('Test::EnumArrayOutParam', runEnumArrayOutParam.bind(pThis)),
        new TestScenario('Test::CompositeStructArrayOutParam', runCompositeStructArrayOutParam.bind(pThis)),
        new TestScenario('Test::ObjectArrayOutParam', runObjectArrayOutParam.bind(pThis)),

        // Non-static array fill params
        new TestScenario('Test::BoolFillParam', runBoolFillParam.bind(pThis)),
        new TestScenario('Test::CharFillParam', runCharFillParam.bind(pThis)),
        new TestScenario('Test::NumericFillParam', runNumericFillParam.bind(pThis)),
        new TestScenario('Test::StringFillParam', runStringFillParam.bind(pThis)),
        new TestScenario('Test::GuidFillParam', runGuidFillParam.bind(pThis)),
        new TestScenario('Test::EnumFillParam', runEnumFillParam.bind(pThis)),
        new TestScenario('Test::CompositeStructFillParam', runCompositeStructFillParam.bind(pThis)),
        new TestScenario('Test::ObjectFillParam', runObjectFillParam.bind(pThis)),
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

function runStaticBoolArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticBoolArrayOutParam(val);
        validateArrayOutParam([ false, false, true, true, false ], fn);
    });
}

function runStaticCharArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticCharArrayOutParam(val);
        validateArrayOutParam([ 'A', 'B', 'C', 'D', 'E' ], fn);
    });
}

function runStaticNumericArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticNumericArrayOutParam(val);
        validateArrayOutParam([ 0, 1, 2, 3, 4 ], fn);
    });
}

function runStaticStringArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticStringArrayOutParam(val);
        validateArrayOutParam([ 'foo', 'bar', 'baz', 'foobar', 'foo\0bar' ], fn);
    });
}

function runStaticGuidArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticGuidArrayOutParam(val);
        validateArrayOutParam(TestValues.guids.valid, fn);
    });
}

function runStaticEnumArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticEnumArrayOutParam(val);
        validateArrayOutParam(TestValues.enums.valid, fn);
    });
}

function runStaticCompositeStructArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticCompositeStructArrayOutParam(val);
        validateArrayOutParam(TestValues.composite.valid, fn);
    });
}

function runStaticObjectArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => TestComponent.Test.staticObjectArrayOutParam(val);
        validateArrayOutParam([
            TestComponent.Test.copyNumericsToVector([]),
            TestComponent.Test.copyNumericsToVector([0]),
            TestComponent.Test.copyNumericsToVector([0, 1, 2, 3, 4])
        ], fn);
    });
}

// Static array fill params
function runStaticBoolFillParam(scenario) {
    this.runSync(scenario, () => {
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

function runStaticCharFillParam(scenario) {
    this.runSync(scenario, () => {
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
    })
}

function runStaticNumericFillParam(scenario) {
    this.runSync(scenario, () => {
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
    })
}

function runStaticStringFillParam(scenario) {
    this.runSync(scenario, () => {
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
    })
}

function runStaticGuidFillParam(scenario) {
    this.runSync(scenario, () => {
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
    })
}

function runStaticEnumFillParam(scenario) {
    this.runSync(scenario, () => {
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
    })
}

function runStaticCompositeStructFillParam(scenario) {
    this.runSync(scenario, () => {
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
    })
}

function runStaticObjectFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            TestComponent.Test.staticObjectFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(val.size, expect);
                for (var i = 0; i < expect; ++i) {
                    assert.equal(val.getAt(i), i);
                }
                ++expect;
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    })
}

// Non-static array out params
function runBoolArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.boolArrayOutParam(val);
        validateArrayOutParam([ false, false, true, true, false ], fn);
    });
}

function runCharArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.charArrayOutParam(val);
        validateArrayOutParam([ 'A', 'B', 'C', 'D', 'E' ], fn);
    });
}

function runNumericArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.numericArrayOutParam(val);
        validateArrayOutParam([ 0, 1, 2, 3, 4 ], fn);
    });
}

function runStringArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.stringArrayOutParam(val);
        validateArrayOutParam([ 'foo', 'bar', 'baz', 'foobar', 'foo\0bar' ], fn);
    });
}

function runGuidArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.guidArrayOutParam(val);
        validateArrayOutParam(TestValues.guids.valid, fn);
    });
}

function runEnumArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.enumArrayOutParam(val);
        validateArrayOutParam(TestValues.enums.valid, fn);
    });
}

function runCompositeStructArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.compositeStructArrayOutParam(val);
        validateArrayOutParam(TestValues.composite.valid, fn);
    });
}

function runObjectArrayOutParam(scenario) {
    this.runSync(scenario, () => {
        var fn = (val) => this.test.objectArrayOutParam(val);
        validateArrayOutParam([
            TestComponent.Test.copyNumericsToVector([]),
            TestComponent.Test.copyNumericsToVector([0]),
            TestComponent.Test.copyNumericsToVector([0, 1, 2, 3, 4])
        ], fn);
    });
}

// Non-static array fill params
function runBoolFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.boolFillParam(arr);

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

function runCharFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.charFillParam(arr);

            var expect = 'a'.charCodeAt(0);
            for (var val of arr) {
                assert.equal(val.charCodeAt(0), expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    })
}

function runNumericFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.numericFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(val, expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    })
}

function runStringFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.stringFillParam(arr);

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
    })
}

function runGuidFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.guidFillParam(arr);

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
    })
}

function runEnumFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.enumFillParam(arr);

            var expect = TestComponent.TestEnum.first;
            for (var val of arr) {
                assert.equal(val, expect++);
            }
        };
        run(0);
        run(1);
        run(2);
        run(4);
    })
}

function runCompositeStructFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.compositeStructFillParam(arr);

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
    })
}

function runObjectFillParam(scenario) {
    this.runSync(scenario, () => {
        var run = (size) => {
            var arr = new Array(size);
            this.test.objectFillParam(arr);

            var expect = 0;
            for (var val of arr) {
                assert.equal(val.size, expect);
                for (var i = 0; i < expect; ++i) {
                    assert.equal(val.getAt(i), i);
                }
                ++expect;
            }
        };
        run(0);
        run(1);
        run(2);
        run(100);
    })
}
