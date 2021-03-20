/**
 * @format
 */

import {
    TestScenario,
    TestValues,
    assert
} from './TestCommon'

export function makePropertiesTestScenarios(pThis) {
    return [
        // Static properties
        // TODO: Figure out why static properties are not working: https://github.com/microsoft/jswinrt/issues/7
        // new TestScenario('Test::StaticBoolProperty', runStaticBoolProperty.bind(pThis)),

        // Non-static properties
        new TestScenario('Test::BoolProperty', runBoolProperty.bind(pThis)),
        new TestScenario('Test::CharProperty', runCharProperty.bind(pThis)),
        new TestScenario('Test::U8Property', runU8Property.bind(pThis)),
        new TestScenario('Test::U16Property', runU16Property.bind(pThis)),
        new TestScenario('Test::U32Property', runU32Property.bind(pThis)),
        new TestScenario('Test::U64Property', runU64Property.bind(pThis)),
        new TestScenario('Test::S16Property', runS16Property.bind(pThis)),
        new TestScenario('Test::S32Property', runS32Property.bind(pThis)),
        new TestScenario('Test::S64Property', runS64Property.bind(pThis)),
        new TestScenario('Test::F32Property', runF32Property.bind(pThis)),
        new TestScenario('Test::F64Property', runF64Property.bind(pThis)),
        new TestScenario('Test::StringProperty', runStringProperty.bind(pThis)),
        new TestScenario('Test::GuidProperty', runGuidProperty.bind(pThis)),
        new TestScenario('Test::EnumProperty', runEnumProperty.bind(pThis)),
        new TestScenario('Test::NumericsStructProperty', runNumericsStructProperty.bind(pThis)),
        new TestScenario('Test::StringsStructProperty', runStringsStructProperty.bind(pThis)),
        new TestScenario('Test::BooleansStructProperty', runBooleansStructProperty.bind(pThis)),
        new TestScenario('Test::CompositeStructProperty', runCompositeStructProperty.bind(pThis)),
        new TestScenario('Test::RefBooleanProperty', runRefBooleanProperty.bind(pThis)),
        new TestScenario('Test::RefCharProperty', runRefCharProperty.bind(pThis)),
        new TestScenario('Test::RefNumericProperty', runRefNumericProperty.bind(pThis)),
        new TestScenario('Test::RefEnumProperty', runRefEnumProperty.bind(pThis)),
        new TestScenario('Test::ObjectProperty', runObjectProperty.bind(pThis)),
        new TestScenario('Test::DateTimeProperty', runDateTimeProperty.bind(pThis)),
        new TestScenario('Test::TimeSpanProperty', runTimeSpanProperty.bind(pThis)),
        new TestScenario('Test::HResultproperty', runHResultProperty.bind(pThis)),
        new TestScenario('Test::PropertyValue', runPropertyValueProperty.bind(pThis)),

        // Non-static array properties
        new TestScenario('Test::BooleanArrayProperty', runBooleanArrayProperty.bind(pThis)),
        new TestScenario('Test::CharArrayProperty', runCharArrayProperty.bind(pThis)),
        new TestScenario('Test::NumericArrayProperty', runNumericArrayProperty.bind(pThis)),
        new TestScenario('Test::StringArrayProperty', runStringArrayProperty.bind(pThis)),
        new TestScenario('Test::GuidArrayProperty', runGuidArrayProperty.bind(pThis)),
        new TestScenario('Test::EnumArrayProperty', runEnumArrayProperty.bind(pThis)),
        new TestScenario('Test::CompositeStructArrayProperty', runCompositeStructArrayProperty.bind(pThis)),
        // TODO: Causes compilation errors in jswinrt: https://github.com/microsoft/jswinrt/issues/9
        // new TestScenario('Test::RefArrayProperty', runRefArrayProperty.bind(pThis)),
        new TestScenario('Test::ObjectArrayProperty', runObjectArrayProperty.bind(pThis)),
    ];
}

function runSyncPropertyTest(scenario, type, vals, invalidVals, get, set) {
    this.runSync(scenario, () => {
        var initial = get();
        assert.equal(typeof(initial), type);

        for (var val of vals) {
            var assignedVal;
            assignedVal = set(val);
            assert.equal(get(), val);
            assert.equal(assignedVal, val);
        }

        for (var val of invalidVals) {
            try {
                set(val);
            }
            catch {}
            assert.notEqual(get(), val);
        }
    });
}

// Static properties
function runStaticBoolProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, [], () => TestComponent.Test.staticBoolProperty, (val) => TestComponent.Test.staticBoolProperty = val);
}

// Non-static properties
function runBoolProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, () => this.test.boolProperty, (val) => this.test.boolProperty = val);
}

function runDateTimeProperty(scenario) {
    this.runSync(scenario, () => {
        for (let i = 0; i < TestValues.dates.valid.length; i++) {
            let assignedValue = TestValues.dates.valid[i];
            this.test.dateTimeProperty = assignedValue;
            assert.equal(TestValues.dates.cppValuesForValidDates[i], this.test.dateTimePropertyCppValue());
            let returnedValue = this.test.dateTimeProperty;
            assert.equal(assignedValue, returnedValue);
        }
    });
}

function runTimeSpanProperty(scenario) {
    this.runSync(scenario, () => {
        for (let i = 0; i < TestValues.timeSpans.valid.length; i++) {
            let assignedValue = TestValues.timeSpans.valid[i];
            this.test.timeSpanProperty = assignedValue;
            assert.equal(TestValues.timeSpans.cppValuesForValidTimeSpans[i], this.test.timeSpanPropertyCppValue());
            let returnedValue = this.test.timeSpanProperty;
            assert.equal(assignedValue, returnedValue);
        }
    });
}

function runPropertyValueProperty(scenario) {
    this.runSync(scenario, () => {
        for (let i = 0; i < TestValues.propertyValues.valid.length; i++) {
            let assignedValue = TestValues.propertyValues.valid[i];
            this.test.propertyValue = assignedValue;
            let returnedValue = this.test.propertyValue;
            assert.equal(assignedValue, returnedValue);
        }
    });
}

function runHResultProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.hResults.valid, TestValues.hResults.invalid, () => this.test.hResultProperty, (val) => this.test.hResultProperty = val);
}

function runCharProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.chars.valid, TestValues.chars.invalid, () => this.test.charProperty, (val) => this.test.charProperty = val);
}

function runU8Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u8.valid, TestValues.u8.invalid, () => this.test.u8Property, (val) => this.test.u8Property = val);
}

function runU16Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u16.valid, TestValues.u16.invalid, () => this.test.u16Property, (val) => this.test.u16Property = val);
}

function runU32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u32.valid, TestValues.u32.invalid, () => this.test.u32Property, (val) => this.test.u32Property = val);
}

function runU64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u64.valid, TestValues.u64.invalid, () => this.test.u64Property, (val) => this.test.u64Property = val);
}

function runS16Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s16.valid, TestValues.s16.invalid, () => this.test.s16Property, (val) => this.test.s16Property = val);
}

function runS32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s32.valid, TestValues.s32.invalid, () => this.test.s32Property, (val) => this.test.s32Property = val);
}

function runS64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s64.valid, TestValues.s64.invalid, () => this.test.s64Property, (val) => this.test.s64Property = val);
}

function runF32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.f32.valid, TestValues.f32.invalid, () => this.test.f32Property, (val) => this.test.f32Property = val);
}

function runF64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.f64.valid, [], () => this.test.f64Property, (val) => this.test.f64Property = val);
}

function runStringProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.strings.valid, [], () => this.test.stringProperty, (val) => this.test.stringProperty = val);
}

function runGuidProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.guids.valid, TestValues.guids.invalid, () => this.test.guidProperty, (val) => this.test.guidProperty = val);
}

function runEnumProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.enums.valid, TestValues.enums.invalid, () => this.test.enumProperty, (val) => this.test.enumProperty = val);
}

function runNumericsStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.numericTypes.valid, TestValues.numericTypes.invalid, () => this.test.numericsStructProperty, (val) => this.test.numericsStructProperty = val);
}

function runStringsStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.stringTypes.valid, TestValues.stringTypes.invalid, () => this.test.stringsStructProperty, (val) => this.test.stringsStructProperty = val);
}

function runBooleansStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.boolTypes.valid, TestValues.boolTypes.invalid, () => this.test.booleansStructProperty, (val) => this.test.booleansStructProperty = val);
}

function runCompositeStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.composite.valid, TestValues.composite.invalid, () => this.test.compositeStructProperty, (val) => this.test.compositeStructProperty = val);
}

function runRefBooleanProperty(scenario) {
    this.runSync(scenario, () => {
        try {
            var initial = this.test.refBooleanProperty;
            assert.equal(typeof(initial), 'object'); // Should start as null, which is an object
            assert.equal(initial, null);

            for (var val of TestValues.bools.valid) {
                var assignedVal;
                assignedVal = this.test.refBooleanProperty = val;
                assert.equal(this.test.refBooleanProperty, val);
                assert.equal(assignedVal, val);
            }
        } finally {
            this.test.refBooleanProperty = null; // In case the test gets run again
        }
    });
}

function runRefCharProperty(scenario) {
    this.runSync(scenario, () => {
        try {
            var initial = this.test.refCharProperty;
            assert.equal(typeof(initial), 'object'); // Should start as null, which is an object
            assert.equal(initial, null);

            for (var val of TestValues.chars.valid) {
                var assignedVal;
                assignedVal = this.test.refCharProperty = val;
                assert.equal(this.test.refCharProperty, val);
                assert.equal(assignedVal, val);
            }
        } finally {
            this.test.refCharProperty = null; // In case the test gets run again
        }
    });
}

function runRefNumericProperty(scenario) {
    this.runSync(scenario, () => {
        try {
            var initial = this.test.refNumericProperty;
            assert.equal(typeof(initial), 'object'); // Should start as null, which is an object
            assert.equal(initial, null);

            for (var val of TestValues.s32.valid) {
                var assignedVal;
                assignedVal = this.test.refNumericProperty = val;
                assert.equal(this.test.refNumericProperty, val);
                assert.equal(assignedVal, val);
            }
        } finally {
            this.test.refNumericProperty = null; // In case the test gets run again
        }
    });
}

function runRefEnumProperty(scenario) {
    this.runSync(scenario, () => {
        try {
            var initial = this.test.refEnumProperty;
            assert.equal(typeof(initial), 'object'); // Should start as null, which is an object
            assert.equal(initial, null);

            for (var val of TestValues.enums.valid) {
                var assignedVal;
                assignedVal = this.test.refEnumProperty = val;
                assert.equal(this.test.refEnumProperty, val);
                assert.equal(assignedVal, val);
            }
        } finally {
            this.test.refEnumProperty = null; // In case the test gets run again
        }
    });
}

function runObjectProperty(scenario) {
    this.runSync(scenario, () => {
        try {
            var prev = this.test.objectProperty;
            assert.equal(typeof(prev), 'object'); // Should start as null, which is also an object
            assert.equal(prev, null);

            for (var val of TestValues.s32.validArrays) {
                var vector = TestComponent.Test.copyNumericsToVector(val);
                var assignedVal;
                assignedVal = this.test.objectProperty = vector;
                assert.equal(typeof(this.test.objectProperty), 'object');
                assert.isTrue(this.test.objectProperty != prev);
                assert.isTrue(this.test.objectProperty == vector);
                assert.isTrue(assignedVal == vector);
                prev = vector;
            }
        } finally {
            this.test.objectProperty = null; // In case the test gets run again
        }
    });
}

// Non-static array properties
function runBooleanArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.bools.validArrays, TestValues.bools.invalidArrays, () => this.test.booleanArrayProperty, (val) => this.test.booleanArrayProperty = val);
}

function runCharArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.chars.validArrays, TestValues.chars.invalidArrays, () => this.test.charArrayProperty, (val) => this.test.charArrayProperty = val);
}

function runNumericArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.s32.validArrays, TestValues.s32.invalidArrays, () => this.test.numericArrayProperty, (val) => this.test.numericArrayProperty = val);
}

function runStringArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.strings.validArrays, TestValues.strings.invalidArrays, () => this.test.stringArrayProperty, (val) => this.test.stringArrayProperty = val);
}

function runGuidArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.guids.validArrays, TestValues.guids.invalidArrays, () => this.test.guidArrayProperty, (val) => this.test.guidArrayProperty = val);
}

function runEnumArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.enums.validArrays, TestValues.enums.invalidArrays, () => this.test.enumArrayProperty, (val) => this.test.enumArrayProperty = val);
}

function runCompositeStructArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.composite.validArrays, TestValues.composite.invalidArrays, () => this.test.compositeStructArrayProperty, (val) => this.test.compositeStructArrayProperty = val);
}

function runRefArrayPropertyArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object',
        [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], TestValues.s32.valid],
        [42, ['A'], [true], ['42'], TestValues.s32.invalid],
        () => this.test.refArrayProperty, (val) => this.test.refArrayProperty = val);
}

function runObjectArrayProperty(scenario) {
    this.runSync(scenario, () => {
        var array = [];
        for (var val of TestValues.s32.validArrays) {
            array.push(TestComponent.Test.copyNumericsToVector(val));
        }

        this.test.objectArrayProperty = array;
        var curr = this.test.objectArrayProperty;
        assert.equal(curr.length, array.length);
        for (var i = 0; i < array.length; ++i) {
            assert.isTrue(curr[i] == array[i]);
        }
    });
}
