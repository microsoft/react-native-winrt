// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
// @ts-check

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
        // Static properties for activable class
        // TODO: Figure out why static properties are not working: https://github.com/microsoft/react-native-winrt/issues/7
        new TestScenario('Test::StaticBoolProperty', runStaticBoolProperty.bind(pThis)),
        new TestScenario('Test::StaticCharProperty', runStaticCharProperty.bind(pThis)),
        new TestScenario('Test::StaticU8Property', runStaticU8Property.bind(pThis)),
        new TestScenario('Test::StaticU16Property', runStaticU16Property.bind(pThis)),
        new TestScenario('Test::StaticU32Property', runStaticU32Property.bind(pThis)),
        new TestScenario('Test::StaticU64Property', runStaticU64Property.bind(pThis)),
        new TestScenario('Test::StaticS16Property', runStaticS16Property.bind(pThis)),
        new TestScenario('Test::StaticS32Property', runStaticS32Property.bind(pThis)),
        new TestScenario('Test::StaticS64Property', runStaticS64Property.bind(pThis)),
        new TestScenario('Test::StaticF32Property', runStaticF32Property.bind(pThis)),
        new TestScenario('Test::StaticF64Property', runStaticF64Property.bind(pThis)),
        new TestScenario('Test::StaticStringProperty', runStaticStringProperty.bind(pThis)),
        new TestScenario('Test::StaticGuidProperty', runStaticGuidProperty.bind(pThis)),
        new TestScenario('Test::StaticEnumProperty', runStaticEnumProperty.bind(pThis)),
        new TestScenario('Test::StaticNumericsStructProperty', runStaticNumericsStructProperty.bind(pThis)),
        new TestScenario('Test::StaticStringsStructProperty', runStaticStringsStructProperty.bind(pThis)),
        new TestScenario('Test::StaticBooleansStructProperty', runStaticBooleansStructProperty.bind(pThis)),
        new TestScenario('Test::StaticCompositeStructProperty', runStaticCompositeStructProperty.bind(pThis)),
        new TestScenario('Test::StaticRefBooleanProperty', runStaticRefBooleanProperty.bind(pThis)),
        new TestScenario('Test::StaticRefCharProperty', runStaticRefCharProperty.bind(pThis)),
        new TestScenario('Test::StaticRefNumericProperty', runStaticRefNumericProperty.bind(pThis)),
        new TestScenario('Test::StaticRefEnumProperty', runStaticRefEnumProperty.bind(pThis)),
        new TestScenario('Test::StaticObjectProperty', runStaticObjectProperty.bind(pThis)),

        // Static array properties
        new TestScenario('Test::StaticBooleanArrayProperty', runStaticBooleanArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticCharArrayProperty', runStaticCharArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticNumericArrayProperty', runStaticNumericArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticStringArrayProperty', runStaticStringArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticGuidArrayProperty', runStaticGuidArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticEnumArrayProperty', runStaticEnumArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticCompositeStructArrayProperty', runStaticCompositeStructArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticRefArrayProperty', runStaticRefArrayProperty.bind(pThis)),
        new TestScenario('Test::StaticObjectArrayProperty', runStaticObjectArrayProperty.bind(pThis)),

        // Static properties for non-activable class
        new TestScenario('Static StaticOnlyTest::BoolProperty', runStaticBoolPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::CharProperty', runStaticCharPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::U8Property', runStaticU8PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::U16Property', runStaticU16PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::U32Property', runStaticU32PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::U64Property', runStaticU64PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::S16Property', runStaticS16PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::S32Property', runStaticS32PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::S64Property', runStaticS64PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::F32Property', runStaticF32PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::F64Property', runStaticF64PropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::StringProperty', runStaticStringPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::GuidProperty', runStaticGuidPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::EnumProperty', runStaticEnumPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::NumericsStructProperty', runStaticNumericsStructPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::StringsStructProperty', runStaticStringsStructPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::BooleansStructProperty', runStaticBooleansStructPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::CompositeStructProperty', runStaticCompositeStructPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::RefBooleanProperty', runStaticRefBooleanPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::RefCharProperty', runStaticRefCharPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::RefNumericProperty', runStaticRefNumericPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::RefEnumProperty', runStaticRefEnumPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::ObjectProperty', runStaticObjectPropertyForNonActivableClass.bind(pThis)),
        new TestScenario('Static StaticOnlyTest::HResultproperty', runStaticHResultPropertyForNonActivableClass.bind(pThis)),

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
        new TestScenario('Test::RefArrayProperty', runRefArrayProperty.bind(pThis)),
        new TestScenario('Test::ObjectArrayProperty', runObjectArrayProperty.bind(pThis)),
    ];
}

function runSyncPropertyTest(scenario, type, vals, invalidVals, get, set) {
    this.runSync(scenario, () => {
        var initial = get();
        assert.equal(type, typeof(initial));

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

function runSyncRefPropertyTest(scenario, vals, get, set) {
    this.runSync(scenario, () => {
        try {
            var initial = get();
            assert.equal(typeof(initial), 'object'); // Should start as null, which is an object
            assert.equal(initial, null);

            for (var val of vals) {
                set(val);
                let assignedVal = get();
                assert.equal(get(), val);
                assert.equal(assignedVal, val);
            }
        } finally {
            set(null); // In case the test gets run again
        }
    });
}

function runSyncObjectPropertyTest(scenario, vals, get, set) {
    this.runSync(scenario, () => {
        try {
            var prev = get();
            assert.equal(typeof(prev), 'object'); // Should start as null, which is also an object
            assert.equal(prev, null);

            for (var val of vals) {
                var newVal = new TestComponent.TestObject(val);
                set(newVal);
                let assignedVal = get();
                assert.equal(typeof(get()), 'object');
                assert.isTrue(get() != prev);
                assert.isTrue(get() == newVal);
                assert.isTrue(assignedVal == newVal);
                prev = newVal;
            }
        } finally {
            set(null); // In case the test gets run again
        }
    });
}

// Static properties for activable classes
function runStaticBoolProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, () => TestComponent.Test.staticBoolProperty, (val) => TestComponent.Test.staticBoolProperty = val);
}

function runStaticCharProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.chars.valid, TestValues.chars.invalid, () => TestComponent.Test.staticCharProperty, (val) => TestComponent.Test.staticCharProperty = val);
}

function runStaticU8Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u8.valid, TestValues.u8.invalid, () => TestComponent.Test.staticU8Property, (val) => TestComponent.Test.staticU8Property = val);
}

function runStaticU16Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u16.valid, TestValues.u16.invalid, () => TestComponent.Test.staticU16Property, (val) => TestComponent.Test.staticU16Property = val);
}

function runStaticU32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u32.valid, TestValues.u32.invalid, () => TestComponent.Test.staticU32Property, (val) => TestComponent.Test.staticU32Property = val);
}

function runStaticU64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u64.valid, TestValues.u64.invalid, () => TestComponent.Test.staticU64Property, (val) => TestComponent.Test.staticU64Property = val);
}

function runStaticS16Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s16.valid, TestValues.s16.invalid, () => TestComponent.Test.staticS16Property, (val) => TestComponent.Test.staticS16Property = val);
}

function runStaticS32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s32.valid, TestValues.s32.invalid, () => TestComponent.Test.staticS32Property, (val) => TestComponent.Test.staticS32Property = val);
}

function runStaticS64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s64.valid, TestValues.s64.invalid, () => TestComponent.Test.staticS64Property, (val) => TestComponent.Test.staticS64Property = val);
}

function runStaticF32Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.f32.valid, TestValues.f32.invalid, () => TestComponent.Test.staticF32Property, (val) => TestComponent.Test.staticF32Property = val);
}

function runStaticF64Property(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.f64.valid, [], () => TestComponent.Test.staticF64Property, (val) => TestComponent.Test.staticF64Property = val);
}

function runStaticStringProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.strings.valid, [], () => TestComponent.Test.staticStringProperty, (val) => TestComponent.Test.staticStringProperty = val);
}

function runStaticGuidProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.guids.valid, TestValues.guids.invalid, () => TestComponent.Test.staticGuidProperty, (val) => TestComponent.Test.staticGuidProperty = val);
}

function runStaticEnumProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.enums.valid, TestValues.enums.invalid, () => TestComponent.Test.staticEnumProperty, (val) => TestComponent.Test.staticEnumProperty = val);
}

function runStaticNumericsStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.numericTypes.valid, TestValues.numericTypes.invalid, () => TestComponent.Test.staticNumericsStructProperty, (val) => TestComponent.Test.staticNumericsStructProperty = val);
}

function runStaticStringsStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.stringTypes.valid, TestValues.stringTypes.invalid, () => TestComponent.Test.staticStringsStructProperty, (val) => TestComponent.Test.staticStringsStructProperty = val);
}

function runStaticBooleansStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.boolTypes.valid, TestValues.boolTypes.invalid, () => TestComponent.Test.staticBooleansStructProperty, (val) => TestComponent.Test.staticBooleansStructProperty = val);
}

function runStaticCompositeStructProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.composite.valid, TestValues.composite.invalid, () => TestComponent.Test.staticCompositeStructProperty, (val) => TestComponent.Test.staticCompositeStructProperty = val);
}

function runStaticRefBooleanProperty(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.bools.valid, () => TestComponent.Test.staticRefBooleanProperty, (val) => TestComponent.Test.staticRefBooleanProperty = val);
}

function runStaticRefCharProperty(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.chars.valid, () => TestComponent.Test.staticRefCharProperty, (val) => TestComponent.Test.staticRefCharProperty = val);
}

function runStaticRefNumericProperty(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.s32.valid, () => TestComponent.Test.staticRefNumericProperty, (val) => TestComponent.Test.staticRefNumericProperty = val);
}

function runStaticRefEnumProperty(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.enums.valid, () => TestComponent.Test.staticRefEnumProperty, (val) => TestComponent.Test.staticRefEnumProperty = val);
}

function runStaticObjectProperty(scenario) {
    runSyncObjectPropertyTest.call(this, scenario, TestValues.s32.valid, () => TestComponent.Test.staticObjectProperty, (val) => TestComponent.Test.staticObjectProperty = val);
}

// Static array properties
function runStaticBooleanArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.bools.validArrays, TestValues.bools.invalidArrays, () => TestComponent.Test.staticBooleanArrayProperty, (val) => TestComponent.Test.staticBooleanArrayProperty = val);
}

function runStaticCharArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.chars.validArrays, TestValues.chars.invalidArrays, () => TestComponent.Test.staticCharArrayProperty, (val) => TestComponent.Test.staticCharArrayProperty = val);
}

function runStaticNumericArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.s32.validArrays, TestValues.s32.invalidArrays, () => TestComponent.Test.staticNumericArrayProperty, (val) => TestComponent.Test.staticNumericArrayProperty = val);
}

function runStaticStringArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.strings.validArrays, TestValues.strings.invalidArrays, () => TestComponent.Test.staticStringArrayProperty, (val) => TestComponent.Test.staticStringArrayProperty = val);
}

function runStaticGuidArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.guids.validArrays, TestValues.guids.invalidArrays, () => TestComponent.Test.staticGuidArrayProperty, (val) => TestComponent.Test.staticGuidArrayProperty = val);
}

function runStaticEnumArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.enums.validArrays, TestValues.enums.invalidArrays, () => TestComponent.Test.staticEnumArrayProperty, (val) => TestComponent.Test.staticEnumArrayProperty = val);
}

function runStaticCompositeStructArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.composite.validArrays, TestValues.composite.invalidArrays, () => TestComponent.Test.staticCompositeStructArrayProperty, (val) => TestComponent.Test.staticCompositeStructArrayProperty = val);
}

function runStaticRefArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object',
        [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], TestValues.s32.valid],
        [42, ['A'], [true], ['42'], TestValues.s32.invalid],
        () => TestComponent.Test.staticRefArrayProperty, (val) => TestComponent.Test.staticRefArrayProperty = val);
}

function runStaticObjectArrayProperty(scenario) {
    this.runSync(scenario, () => {
        var array = TestValues.s32.valid.map(val => new TestComponent.TestObject(val));
        TestComponent.Test.staticObjectArrayProperty = array;
        var curr = TestComponent.Test.staticObjectArrayProperty;
        assert.equal(curr.length, array.length);
        for (var i = 0; i < array.length; ++i) {
            assert.isTrue(curr[i] == array[i]);
        }
    });
}

// Static Properties for non activable classes
function runStaticBoolPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, () => TestComponent.StaticOnlyTest.boolProperty, (val) => TestComponent.StaticOnlyTest.boolProperty = val);
}

function runStaticHResultPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.hResults.valid, TestValues.hResults.invalid, () => TestComponent.StaticOnlyTest.hResultProperty, (val) => TestComponent.StaticOnlyTest.hResultProperty = val);
}

function runStaticCharPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.chars.valid, TestValues.chars.invalid, () => TestComponent.StaticOnlyTest.charProperty, (val) => TestComponent.StaticOnlyTest.charProperty = val);
}

function runStaticU8PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u8.valid, TestValues.u8.invalid, () => TestComponent.StaticOnlyTest.u8Property, (val) => TestComponent.StaticOnlyTest.u8Property = val);
}

function runStaticU16PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u16.valid, TestValues.u16.invalid, () => TestComponent.StaticOnlyTest.u16Property, (val) => TestComponent.StaticOnlyTest.u16Property = val);
}

function runStaticU32PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u32.valid, TestValues.u32.invalid, () => TestComponent.StaticOnlyTest.u32Property, (val) => TestComponent.StaticOnlyTest.u32Property = val);
}

function runStaticU64PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.u64.valid, TestValues.u64.invalid, () => TestComponent.StaticOnlyTest.u64Property, (val) => TestComponent.StaticOnlyTest.u64Property = val);
}

function runStaticS16PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s16.valid, TestValues.s16.invalid, () => TestComponent.StaticOnlyTest.s16Property, (val) => TestComponent.StaticOnlyTest.s16Property = val);
}

function runStaticS32PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s32.valid, TestValues.s32.invalid, () => TestComponent.StaticOnlyTest.s32Property, (val) => TestComponent.StaticOnlyTest.s32Property = val);
}

function runStaticS64PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.s64.valid, TestValues.s64.invalid, () => TestComponent.StaticOnlyTest.s64Property, (val) => TestComponent.StaticOnlyTest.s64Property = val);
}

function runStaticF32PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.f32.valid, TestValues.f32.invalid, () => TestComponent.StaticOnlyTest.f32Property, (val) => TestComponent.StaticOnlyTest.f32Property = val);
}

function runStaticF64PropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.f64.valid, [], () => TestComponent.StaticOnlyTest.f64Property, (val) => TestComponent.StaticOnlyTest.f64Property = val);
}

function runStaticStringPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.strings.valid, [], () => TestComponent.StaticOnlyTest.stringProperty, (val) => TestComponent.StaticOnlyTest.stringProperty = val);
}

function runStaticGuidPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'string', TestValues.guids.valid, TestValues.guids.invalid, () => TestComponent.StaticOnlyTest.guidProperty, (val) => TestComponent.StaticOnlyTest.guidProperty = val);
}

function runStaticEnumPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.enums.valid, TestValues.enums.invalid, () => TestComponent.StaticOnlyTest.enumProperty, (val) => TestComponent.StaticOnlyTest.enumProperty = val);
}

function runStaticNumericsStructPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.numericTypes.valid, TestValues.numericTypes.invalid, () => TestComponent.StaticOnlyTest.numericsStructProperty, (val) => TestComponent.StaticOnlyTest.numericsStructProperty = val);
}

function runStaticStringsStructPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.stringTypes.valid, TestValues.stringTypes.invalid, () => TestComponent.StaticOnlyTest.stringsStructProperty, (val) => TestComponent.StaticOnlyTest.stringsStructProperty = val);
}

function runStaticBooleansStructPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.boolTypes.valid, TestValues.boolTypes.invalid, () => TestComponent.StaticOnlyTest.booleansStructProperty, (val) => TestComponent.StaticOnlyTest.booleansStructProperty = val);
}

function runStaticCompositeStructPropertyForNonActivableClass(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object', TestValues.composite.valid, TestValues.composite.invalid, () => TestComponent.StaticOnlyTest.compositeStructProperty, (val) => TestComponent.StaticOnlyTest.compositeStructProperty = val);
}

function runStaticRefBooleanPropertyForNonActivableClass(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.bools.valid, () => TestComponent.StaticOnlyTest.refBooleanProperty, (val) => TestComponent.StaticOnlyTest.refBooleanProperty = val);
}

function runStaticRefCharPropertyForNonActivableClass(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.chars.valid, () => TestComponent.StaticOnlyTest.refCharProperty, (val) => TestComponent.StaticOnlyTest.refCharProperty = val);
}

function runStaticRefNumericPropertyForNonActivableClass(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.s32.valid, () => TestComponent.StaticOnlyTest.refNumericProperty, (val) => TestComponent.StaticOnlyTest.refNumericProperty = val);
}

function runStaticRefEnumPropertyForNonActivableClass(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.enums.valid, () => TestComponent.StaticOnlyTest.refEnumProperty, (val) => TestComponent.StaticOnlyTest.refEnumProperty = val);
}

function runStaticObjectPropertyForNonActivableClass(scenario) {
    runSyncObjectPropertyTest.call(this, scenario, TestValues.s32.valid, () => TestComponent.StaticOnlyTest.objectProperty, (val) => TestComponent.StaticOnlyTest.objectProperty = val);
}

// Non-static properties
function runBoolProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, () => this.test.boolProperty, (val) => this.test.boolProperty = val);
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
    runSyncRefPropertyTest.call(this, scenario, TestValues.bools.valid, () => this.test.refBooleanProperty, (val) => this.test.refBooleanProperty = val);
}

function runRefCharProperty(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.chars.valid, () => this.test.refCharProperty, (val) => this.test.refCharProperty = val);
}

function runRefNumericProperty(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.s32.valid, () => this.test.refNumericProperty, (val) => this.test.refNumericProperty = val);
}

function runRefEnumProperty(scenario) {
    runSyncRefPropertyTest.call(this, scenario, TestValues.enums.valid, () => this.test.refEnumProperty, (val) => this.test.refEnumProperty = val);
}

function runObjectProperty(scenario) {
    runSyncObjectPropertyTest.call(this, scenario, TestValues.s32.valid, () => this.test.objectProperty, (val) => this.test.objectProperty = val);
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
        for (let [value, type] of TestValues.propertyValues.validValueTypePairs)
        {
            this.test.assignPropertyValueAsType(value, type);
            assert.equal(this.test.propertyValue, value);
            assert.equal(this.test.propertyValueCppType, type);
        }
    });
}

function runHResultProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'number', TestValues.hResults.valid, TestValues.hResults.invalid, () => this.test.hResultProperty, (val) => this.test.hResultProperty = val);
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

function runRefArrayProperty(scenario) {
    runSyncPropertyTest.call(this, scenario, 'object',
        [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], TestValues.s32.valid],
        [42, ['A'], [true], ['42'], TestValues.s32.invalid],
        () => this.test.refArrayProperty, (val) => this.test.refArrayProperty = val);
}

function runObjectArrayProperty(scenario) {
    this.runSync(scenario, () => {
        var array = TestValues.s32.valid.map(val => new TestComponent.TestObject(val));
        this.test.objectArrayProperty = array;
        var curr = this.test.objectArrayProperty;
        assert.equal(curr.length, array.length);
        for (var i = 0; i < array.length; ++i) {
            assert.isTrue(curr[i] == array[i]);
        }
    });
}
