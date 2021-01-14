/**
 * @format
 */

import {
    TestScenario,
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
    runSyncPropertyTest.call(this, scenario, 'boolean', this.boolTestValues, [], () => TestComponent.Test.staticBoolProperty, (val) => TestComponent.Test.staticBoolProperty = val);
}

// Non-static properties
function runBoolProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'boolean', this.boolTestValues, this.boolInvalidValues, () => this.test.boolProperty, (val) => this.test.boolProperty = val);
}

function runCharProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', this.charTestValues, this.charInvalidValues, () => this.test.charProperty, (val) => this.test.charProperty = val);
}

function runU8Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.u8TestValues, this.u8InvalidValues, () => this.test.u8Property, (val) => this.test.u8Property = val);
}

function runU16Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.u16TestValues, this.u16InvalidValues, () => this.test.u16Property, (val) => this.test.u16Property = val);
}

function runU32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.u32TestValues, this.u32InvalidValues, () => this.test.u32Property, (val) => this.test.u32Property = val);
}

function runU64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.u64TestValues, this.u64InvalidValues, () => this.test.u64Property, (val) => this.test.u64Property = val);
}

function runS16Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.s16TestValues, this.s16InvalidValues, () => this.test.s16Property, (val) => this.test.s16Property = val);
}

function runS32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.s32TestValues, this.s32InvalidValues, () => this.test.s32Property, (val) => this.test.s32Property = val);
}

function runS64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.s64TestValues, this.s64InvalidValues, () => this.test.s64Property, (val) => this.test.s64Property = val);
}

function runF32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.f32TestValues, this.f32InvalidValues, () => this.test.f32Property, (val) => this.test.f32Property = val);
}

function runF64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.f64TestValues, [], () => this.test.f64Property, (val) => this.test.f64Property = val);
}

function runStringProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', this.stringTestValues, [], () => this.test.stringProperty, (val) => this.test.stringProperty = val);
}

function runGuidProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', this.guidTestValues, this.guidInvalidValues, () => this.test.guidProperty, (val) => this.test.guidProperty = val);
}

function runEnumProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', this.enumTestValues, this.enumInvalidValues, () => this.test.enumProperty, (val) => this.test.enumProperty = val);
}

function runNumericsStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.numericTypesTestValues, this.numericTypesInvalidValues, () => this.test.numericsStructProperty, (val) => this.test.numericsStructProperty = val);
}

function runStringsStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.stringTypesTestValues, this.stringTypesInvalidValues, () => this.test.stringsStructProperty, (val) => this.test.stringsStructProperty = val);
}

function runBooleansStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.boolTypesTestValues, this.boolTypesInvalidValues, () => this.test.booleansStructProperty, (val) => this.test.booleansStructProperty = val);
}

function runCompositeStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.compositeTestValues, this.compositeInvalidValues, () => this.test.compositeStructProperty, (val) => this.test.compositeStructProperty = val);
}

function runRefBooleanProperty(scenario) {
    this.runSync(scenario, () => {
        try {
            var initial = this.test.refBooleanProperty;
            assert.equal(typeof(initial), 'object'); // Should start as null, which is an object
            assert.equal(initial, null);

            for (var val of this.boolTestValues) {
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

            for (var val of this.charTestValues) {
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

            for (var val of this.s32TestValues) {
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

            for (var val of this.enumTestValues) {
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

            for (var val of this.numericArrayTestValues) {
                var vector = TestComponent.Test.makeNumericVector(val);
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
    runSyncPropertyTest.call(this, scenario, 'object', this.boolArrayTestValues, this.boolArrayInvalidValues, () => this.test.booleanArrayProperty, (val) => this.test.booleanArrayProperty = val);
}

function runCharArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.charArrayTestValues, this.charArrayInvalidValues, () => this.test.charArrayProperty, (val) => this.test.charArrayProperty = val);
}

function runNumericArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.numericArrayTestValues, this.numericArrayInvalidValues, () => this.test.numericArrayProperty, (val) => this.test.numericArrayProperty = val);
}

function runStringArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.stringArrayTestValues, this.stringArrayInvalidValues, () => this.test.stringArrayProperty, (val) => this.test.stringArrayProperty = val);
}

function runGuidArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.guidArrayTestValues, this.guidArrayInvalidValues, () => this.test.guidArrayProperty, (val) => this.test.guidArrayProperty = val);
}

function runEnumArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.enumArrayTestValues, this.enumArrayInvalidValues, () => this.test.enumArrayProperty, (val) => this.test.enumArrayProperty = val);
}

function runCompositeStructArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', this.compositeArrayTestValues, this.compositeArrayInvalidValues, () => this.test.compositeStructArrayProperty, (val) => this.test.compositeStructArrayProperty = val);
}

function runRefArrayPropertyArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object',
        [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], this.s32TestValues],
        [42, ['A'], [true], ['42'], this.s32InvalidValues],
        () => this.test.refArrayProperty, (val) => this.test.refArrayProperty = val);
}

function runObjectArrayProperty(scenario) {
    this.runSync(scenario, () => {
        var array = [];
        for (var val of this.numericArrayTestValues) {
            array.push(TestComponent.Test.makeNumericVector(val));
        }

        this.test.objectArrayProperty = array;
        var curr = this.test.objectArrayProperty;
        assert.equal(curr.length, array.length);
        for (var i = 0; i < array.length; ++i) {
            assert.isTrue(curr[i] == array[i]);
        }
    });
}
