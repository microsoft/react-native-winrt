// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * Properties Tests - Node.js Adapter
 * @format
 */

const {
    TestScenario,
    assert
} = require('./TestCommon');

function makePropertiesTestScenarios(pThis, TestComponent, TestValues) {
    return [
        // Static properties for activable class
        new TestScenario('Test::StaticBoolProperty', () => runStaticBoolProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCharProperty', () => runStaticCharProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticU8Property', () => runStaticU8Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticU16Property', () => runStaticU16Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticU32Property', () => runStaticU32Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticU64Property', () => runStaticU64Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticS16Property', () => runStaticS16Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticS32Property', () => runStaticS32Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticS64Property', () => runStaticS64Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticF32Property', () => runStaticF32Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticF64Property', () => runStaticF64Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticStringProperty', () => runStaticStringProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticGuidProperty', () => runStaticGuidProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticEnumProperty', () => runStaticEnumProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticNumericsStructProperty', () => runStaticNumericsStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticStringsStructProperty', () => runStaticStringsStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticBooleansStructProperty', () => runStaticBooleansStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCompositeStructProperty', () => runStaticCompositeStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticRefBooleanProperty', () => runStaticRefBooleanProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticRefCharProperty', () => runStaticRefCharProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticRefNumericProperty', () => runStaticRefNumericProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticRefEnumProperty', () => runStaticRefEnumProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticObjectProperty', () => runStaticObjectProperty(pThis, TestComponent, TestValues)),

        // Static array properties
        new TestScenario('Test::StaticBooleanArrayProperty', () => runStaticBooleanArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCharArrayProperty', () => runStaticCharArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticNumericArrayProperty', () => runStaticNumericArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticStringArrayProperty', () => runStaticStringArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticGuidArrayProperty', () => runStaticGuidArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticEnumArrayProperty', () => runStaticEnumArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticCompositeStructArrayProperty', () => runStaticCompositeStructArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticRefArrayProperty', () => runStaticRefArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StaticObjectArrayProperty', () => runStaticObjectArrayProperty(pThis, TestComponent, TestValues)),

        // Static Properties for non activable classes
        new TestScenario('Static StaticOnlyTest::BoolProperty', () => runStaticBoolPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::CharProperty', () => runStaticCharPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::U8Property', () => runStaticU8PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::U16Property', () => runStaticU16PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::U32Property', () => runStaticU32PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::U64Property', () => runStaticU64PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::S16Property', () => runStaticS16PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::S32Property', () => runStaticS32PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::S64Property', () => runStaticS64PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::F32Property', () => runStaticF32PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::F64Property', () => runStaticF64PropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::StringProperty', () => runStaticStringPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::GuidProperty', () => runStaticGuidPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::EnumProperty', () => runStaticEnumPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::NumericsStructProperty', () => runStaticNumericsStructPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::StringsStructProperty', () => runStaticStringsStructPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::BooleansStructProperty', () => runStaticBooleansStructPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::CompositeStructProperty', () => runStaticCompositeStructPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::RefBooleanProperty', () => runStaticRefBooleanPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::RefCharProperty', () => runStaticRefCharPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::RefNumericProperty', () => runStaticRefNumericPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::RefEnumProperty', () => runStaticRefEnumPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::ObjectProperty', () => runStaticObjectPropertyForNonActivableClass(pThis, TestComponent, TestValues)),
        new TestScenario('Static StaticOnlyTest::HResultproperty', () => runStaticHResultPropertyForNonActivableClass(pThis, TestComponent, TestValues)),

        // Non-static properties
        new TestScenario('Test::BoolProperty', () => runBoolProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CharProperty', () => runCharProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::U8Property', () => runU8Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::U16Property', () => runU16Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::U32Property', () => runU32Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::U64Property', () => runU64Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::S16Property', () => runS16Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::S32Property', () => runS32Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::S64Property', () => runS64Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::F32Property', () => runF32Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::F64Property', () => runF64Property(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StringProperty', () => runStringProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::GuidProperty', () => runGuidProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::EnumProperty', () => runEnumProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::NumericsStructProperty', () => runNumericsStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StringsStructProperty', () => runStringsStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::BooleansStructProperty', () => runBooleansStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CompositeStructProperty', () => runCompositeStructProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::RefBooleanProperty', () => runRefBooleanProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::RefCharProperty', () => runRefCharProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::RefNumericProperty', () => runRefNumericProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::RefEnumProperty', () => runRefEnumProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::ObjectProperty', () => runObjectProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::DateTimeProperty', () => runDateTimeProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::TimeSpanProperty', () => runTimeSpanProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::HResultproperty', () => runHResultProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::PropertyValue', () => runPropertyValueProperty(pThis, TestComponent, TestValues)),

        // Non-static array properties
        new TestScenario('Test::BooleanArrayProperty', () => runBooleanArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CharArrayProperty', () => runCharArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::NumericArrayProperty', () => runNumericArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::StringArrayProperty', () => runStringArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::GuidArrayProperty', () => runGuidArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::EnumArrayProperty', () => runEnumArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::CompositeStructArrayProperty', () => runCompositeStructArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::RefArrayProperty', () => runRefArrayProperty(pThis, TestComponent, TestValues)),
        new TestScenario('Test::ObjectArrayProperty', () => runObjectArrayProperty(pThis, TestComponent, TestValues)),
    ];
}

// Helper functions
function runSyncPropertyTest(pThis, type, vals, invalidVals, get, set) {
    pThis.runSync(null, () => {
        console.log('Getting initial value');
        var initial = get();
        assert.equal(type, typeof(initial));

        for (var val of vals) {
            console.log('Setting ' + val);
            set(val);
            assert.equal(get(), val);
        }

        for (var val of invalidVals) {
            console.log('Setting ' + val);
            try {
                set(val);
            }
            catch {}
            assert.notEqual(get(), val);
        }
    });
}

function runSyncRefPropertyTest(pThis, vals, get, set) {
    pThis.runSync(null, () => {
        try {
            var initial = get();
            assert.equal(typeof(initial), 'object'); // Should start as null, which is an object
            assert.equal(initial, null);

            for (var val of vals) {
                set(val);
                assert.equal(get(), val);
            }
        } finally {
            set(null); // In case the test gets run again
        }
    });
}

function runSyncObjectPropertyTest(pThis, TestComponent, vals, get, set) {
    pThis.runSync(null, () => {
        try {
            var prev = get();
            assert.equal(typeof(prev), 'object'); // Should start as null, which is also an object
            assert.equal(prev, null);

            for (var val of vals) {
                var newVal = new TestComponent.TestObject(val);
                set(newVal);
                assert.equal(typeof(get()), 'object');
                assert.isTrue(get() != prev);
                assert.isTrue(get() == newVal);
                prev = newVal;
            }
        } finally {
            set(null); // In case the test gets run again
        }
    });
}

// Static properties for activable classes
function runStaticBoolProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, 
        () => TestComponent.Test.staticBoolProperty, 
        (val) => TestComponent.Test.staticBoolProperty = val);
}

function runStaticCharProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'string', TestValues.chars.valid, TestValues.chars.invalid, 
        () => TestComponent.Test.staticCharProperty, 
        (val) => TestComponent.Test.staticCharProperty = val);
}

function runStaticU8Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u8.valid, TestValues.u8.invalid, 
        () => TestComponent.Test.staticU8Property, 
        (val) => TestComponent.Test.staticU8Property = val);
}

function runStaticU16Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u16.valid, TestValues.u16.invalid, 
        () => TestComponent.Test.staticU16Property, 
        (val) => TestComponent.Test.staticU16Property = val);
}

function runStaticU32Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u32.valid, TestValues.u32.invalid, 
        () => TestComponent.Test.staticU32Property, 
        (val) => TestComponent.Test.staticU32Property = val);
}

function runStaticU64Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u64.valid, TestValues.u64.invalid, 
        () => TestComponent.Test.staticU64Property, 
        (val) => TestComponent.Test.staticU64Property = val);
}

function runStaticS16Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.s16.valid, TestValues.s16.invalid, 
        () => TestComponent.Test.staticS16Property, 
        (val) => TestComponent.Test.staticS16Property = val);
}

function runStaticS32Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.s32.valid, TestValues.s32.invalid, 
        () => TestComponent.Test.staticS32Property, 
        (val) => TestComponent.Test.staticS32Property = val);
}

function runStaticS64Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.s64.valid, TestValues.s64.invalid, 
        () => TestComponent.Test.staticS64Property, 
        (val) => TestComponent.Test.staticS64Property = val);
}

function runStaticF32Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.f32.valid, TestValues.f32.invalid, 
        () => TestComponent.Test.staticF32Property, 
        (val) => TestComponent.Test.staticF32Property = val);
}

function runStaticF64Property(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.f64.valid, [], 
        () => TestComponent.Test.staticF64Property, 
        (val) => TestComponent.Test.staticF64Property = val);
}

function runStaticStringProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'string', TestValues.strings.valid, [], 
        () => TestComponent.Test.staticStringProperty, 
        (val) => TestComponent.Test.staticStringProperty = val);
}

function runStaticGuidProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'string', TestValues.guids.valid, TestValues.guids.invalid, 
        () => TestComponent.Test.staticGuidProperty, 
        (val) => TestComponent.Test.staticGuidProperty = val);
}

function runStaticEnumProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.enums.valid, TestValues.enums.invalid, 
        () => TestComponent.Test.staticEnumProperty, 
        (val) => TestComponent.Test.staticEnumProperty = val);
}

function runStaticNumericsStructProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.numericTypes.valid, TestValues.numericTypes.invalid, 
        () => TestComponent.Test.staticNumericsStructProperty, 
        (val) => TestComponent.Test.staticNumericsStructProperty = val);
}

function runStaticStringsStructProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.stringTypes.valid, TestValues.stringTypes.invalid, 
        () => TestComponent.Test.staticStringsStructProperty, 
        (val) => TestComponent.Test.staticStringsStructProperty = val);
}

function runStaticBooleansStructProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.boolTypes.valid, TestValues.boolTypes.invalid, 
        () => TestComponent.Test.staticBooleansStructProperty, 
        (val) => TestComponent.Test.staticBooleansStructProperty = val);
}

function runStaticCompositeStructProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.composite.valid, TestValues.composite.invalid, 
        () => TestComponent.Test.staticCompositeStructProperty, 
        (val) => TestComponent.Test.staticCompositeStructProperty = val);
}

function runStaticRefBooleanProperty(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.bools.valid, 
        () => TestComponent.Test.staticRefBooleanProperty, 
        (val) => TestComponent.Test.staticRefBooleanProperty = val);
}

function runStaticRefCharProperty(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.chars.valid, 
        () => TestComponent.Test.staticRefCharProperty, 
        (val) => TestComponent.Test.staticRefCharProperty = val);
}

function runStaticRefNumericProperty(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.s32.valid, 
        () => TestComponent.Test.staticRefNumericProperty, 
        (val) => TestComponent.Test.staticRefNumericProperty = val);
}

function runStaticRefEnumProperty(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.enums.valid, 
        () => TestComponent.Test.staticRefEnumProperty, 
        (val) => TestComponent.Test.staticRefEnumProperty = val);
}

function runStaticObjectProperty(pThis, TestComponent, TestValues) {
    runSyncObjectPropertyTest(pThis, TestComponent, TestValues.s32.valid, 
        () => TestComponent.Test.staticObjectProperty, 
        (val) => TestComponent.Test.staticObjectProperty = val);
}

// Static array properties
function runStaticBooleanArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.bools.validArrays, TestValues.bools.invalidArrays, 
        () => TestComponent.Test.staticBooleanArrayProperty, 
        (val) => TestComponent.Test.staticBooleanArrayProperty = val);
}

function runStaticCharArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.chars.validArrays, TestValues.chars.invalidArrays, 
        () => TestComponent.Test.staticCharArrayProperty, 
        (val) => TestComponent.Test.staticCharArrayProperty = val);
}

function runStaticNumericArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.s32.validArrays, TestValues.s32.invalidArrays, 
        () => TestComponent.Test.staticNumericArrayProperty, 
        (val) => TestComponent.Test.staticNumericArrayProperty = val);
}

function runStaticStringArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.strings.validArrays, TestValues.strings.invalidArrays, 
        () => TestComponent.Test.staticStringArrayProperty, 
        (val) => TestComponent.Test.staticStringArrayProperty = val);
}

function runStaticGuidArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.guids.validArrays, TestValues.guids.invalidArrays, 
        () => TestComponent.Test.staticGuidArrayProperty, 
        (val) => TestComponent.Test.staticGuidArrayProperty = val);
}

function runStaticEnumArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.enums.validArrays, TestValues.enums.invalidArrays, 
        () => TestComponent.Test.staticEnumArrayProperty, 
        (val) => TestComponent.Test.staticEnumArrayProperty = val);
}

function runStaticCompositeStructArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.composite.validArrays, TestValues.composite.invalidArrays, 
        () => TestComponent.Test.staticCompositeStructArrayProperty, 
        (val) => TestComponent.Test.staticCompositeStructArrayProperty = val);
}

function runStaticRefArrayProperty(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object',
        [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], TestValues.s32.valid],
        [42, ['A'], [true], ['42'], TestValues.s32.invalid],
        () => TestComponent.Test.staticRefArrayProperty, 
        (val) => TestComponent.Test.staticRefArrayProperty = val);
}

function runStaticObjectArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        try {
            var initial = TestComponent.Test.staticObjectArrayProperty;
            assert.equal(typeof(initial), 'object');
            assert.equal(initial.length, 0);

            var val = [new TestComponent.TestObject(0), new TestComponent.TestObject(1), new TestComponent.TestObject(2)];
            TestComponent.Test.staticObjectArrayProperty = val;
            assert.equal(TestComponent.Test.staticObjectArrayProperty, val);
        } finally {
            TestComponent.Test.staticObjectArrayProperty = [];
        }
    });
}

// Static Properties for non activable classes
function runStaticBoolPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, 
        () => TestComponent.StaticOnlyTest.boolProperty, 
        (val) => TestComponent.StaticOnlyTest.boolProperty = val);
}

function runStaticHResultPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.hResults.valid, TestValues.hResults.invalid, 
        () => TestComponent.StaticOnlyTest.hResultProperty, 
        (val) => TestComponent.StaticOnlyTest.hResultProperty = val);
}

function runStaticCharPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'string', TestValues.chars.valid, TestValues.chars.invalid, 
        () => TestComponent.StaticOnlyTest.charProperty, 
        (val) => TestComponent.StaticOnlyTest.charProperty = val);
}

function runStaticU8PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u8.valid, TestValues.u8.invalid, 
        () => TestComponent.StaticOnlyTest.u8Property, 
        (val) => TestComponent.StaticOnlyTest.u8Property = val);
}

function runStaticU16PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u16.valid, TestValues.u16.invalid, 
        () => TestComponent.StaticOnlyTest.u16Property, 
        (val) => TestComponent.StaticOnlyTest.u16Property = val);
}

function runStaticU32PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u32.valid, TestValues.u32.invalid, 
        () => TestComponent.StaticOnlyTest.u32Property, 
        (val) => TestComponent.StaticOnlyTest.u32Property = val);
}

function runStaticU64PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.u64.valid, TestValues.u64.invalid, 
        () => TestComponent.StaticOnlyTest.u64Property, 
        (val) => TestComponent.StaticOnlyTest.u64Property = val);
}

function runStaticS16PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.s16.valid, TestValues.s16.invalid, 
        () => TestComponent.StaticOnlyTest.s16Property, 
        (val) => TestComponent.StaticOnlyTest.s16Property = val);
}

function runStaticS32PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.s32.valid, TestValues.s32.invalid, 
        () => TestComponent.StaticOnlyTest.s32Property, 
        (val) => TestComponent.StaticOnlyTest.s32Property = val);
}

function runStaticS64PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.s64.valid, TestValues.s64.invalid, 
        () => TestComponent.StaticOnlyTest.s64Property, 
        (val) => TestComponent.StaticOnlyTest.s64Property = val);
}

function runStaticF32PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.f32.valid, TestValues.f32.invalid, 
        () => TestComponent.StaticOnlyTest.f32Property, 
        (val) => TestComponent.StaticOnlyTest.f32Property = val);
}

function runStaticF64PropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.f64.valid, [], 
        () => TestComponent.StaticOnlyTest.f64Property, 
        (val) => TestComponent.StaticOnlyTest.f64Property = val);
}

function runStaticStringPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'string', TestValues.strings.valid, [], 
        () => TestComponent.StaticOnlyTest.stringProperty, 
        (val) => TestComponent.StaticOnlyTest.stringProperty = val);
}

function runStaticGuidPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'string', TestValues.guids.valid, TestValues.guids.invalid, 
        () => TestComponent.StaticOnlyTest.guidProperty, 
        (val) => TestComponent.StaticOnlyTest.guidProperty = val);
}

function runStaticEnumPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'number', TestValues.enums.valid, TestValues.enums.invalid, 
        () => TestComponent.StaticOnlyTest.enumProperty, 
        (val) => TestComponent.StaticOnlyTest.enumProperty = val);
}

function runStaticNumericsStructPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.numericTypes.valid, TestValues.numericTypes.invalid, 
        () => TestComponent.StaticOnlyTest.numericsStructProperty, 
        (val) => TestComponent.StaticOnlyTest.numericsStructProperty = val);
}

function runStaticStringsStructPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.stringTypes.valid, TestValues.stringTypes.invalid, 
        () => TestComponent.StaticOnlyTest.stringsStructProperty, 
        (val) => TestComponent.StaticOnlyTest.stringsStructProperty = val);
}

function runStaticBooleansStructPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.boolTypes.valid, TestValues.boolTypes.invalid, 
        () => TestComponent.StaticOnlyTest.booleansStructProperty, 
        (val) => TestComponent.StaticOnlyTest.booleansStructProperty = val);
}

function runStaticCompositeStructPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncPropertyTest(pThis, 'object', TestValues.composite.valid, TestValues.composite.invalid, 
        () => TestComponent.StaticOnlyTest.compositeStructProperty, 
        (val) => TestComponent.StaticOnlyTest.compositeStructProperty = val);
}

function runStaticRefBooleanPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.bools.valid, 
        () => TestComponent.StaticOnlyTest.refBooleanProperty, 
        (val) => TestComponent.StaticOnlyTest.refBooleanProperty = val);
}

function runStaticRefCharPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.chars.valid, 
        () => TestComponent.StaticOnlyTest.refCharProperty, 
        (val) => TestComponent.StaticOnlyTest.refCharProperty = val);
}

function runStaticRefNumericPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.s32.valid, 
        () => TestComponent.StaticOnlyTest.refNumericProperty, 
        (val) => TestComponent.StaticOnlyTest.refNumericProperty = val);
}

function runStaticRefEnumPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncRefPropertyTest(pThis, TestValues.enums.valid, 
        () => TestComponent.StaticOnlyTest.refEnumProperty, 
        (val) => TestComponent.StaticOnlyTest.refEnumProperty = val);
}

function runStaticObjectPropertyForNonActivableClass(pThis, TestComponent, TestValues) {
    runSyncObjectPropertyTest(pThis, TestComponent, TestValues.s32.valid, 
        () => TestComponent.StaticOnlyTest.objectProperty, 
        (val) => TestComponent.StaticOnlyTest.objectProperty = val);
}

// Non-static properties (instance properties)
function runBoolProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'boolean', TestValues.bools.valid, TestValues.bools.invalid, 
            () => test.boolProperty, 
            (val) => test.boolProperty = val);
    });
}

function runCharProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'string', TestValues.chars.valid, TestValues.chars.invalid, 
            () => test.charProperty, 
            (val) => test.charProperty = val);
    });
}

function runU8Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.u8.valid, TestValues.u8.invalid, 
            () => test.u8Property, 
            (val) => test.u8Property = val);
    });
}

function runU16Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.u16.valid, TestValues.u16.invalid, 
            () => test.u16Property, 
            (val) => test.u16Property = val);
    });
}

function runU32Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.u32.valid, TestValues.u32.invalid, 
            () => test.u32Property, 
            (val) => test.u32Property = val);
    });
}

function runU64Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.u64.valid, TestValues.u64.invalid, 
            () => test.u64Property, 
            (val) => test.u64Property = val);
    });
}

function runS16Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.s16.valid, TestValues.s16.invalid, 
            () => test.s16Property, 
            (val) => test.s16Property = val);
    });
}

function runS32Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.s32.valid, TestValues.s32.invalid, 
            () => test.s32Property, 
            (val) => test.s32Property = val);
    });
}

function runS64Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.s64.valid, TestValues.s64.invalid, 
            () => test.s64Property, 
            (val) => test.s64Property = val);
    });
}

function runF32Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.f32.valid, TestValues.f32.invalid, 
            () => test.f32Property, 
            (val) => test.f32Property = val);
    });
}

function runF64Property(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.f64.valid, [], 
            () => test.f64Property, 
            (val) => test.f64Property = val);
    });
}

function runStringProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'string', TestValues.strings.valid, [], 
            () => test.stringProperty, 
            (val) => test.stringProperty = val);
    });
}

function runGuidProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'string', TestValues.guids.valid, TestValues.guids.invalid, 
            () => test.guidProperty, 
            (val) => test.guidProperty = val);
    });
}

function runEnumProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.enums.valid, TestValues.enums.invalid, 
            () => test.enumProperty, 
            (val) => test.enumProperty = val);
    });
}

function runNumericsStructProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.numericTypes.valid, TestValues.numericTypes.invalid, 
            () => test.numericsStructProperty, 
            (val) => test.numericsStructProperty = val);
    });
}

function runStringsStructProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.stringTypes.valid, TestValues.stringTypes.invalid, 
            () => test.stringsStructProperty, 
            (val) => test.stringsStructProperty = val);
    });
}

function runBooleansStructProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.boolTypes.valid, TestValues.boolTypes.invalid, 
            () => test.booleansStructProperty, 
            (val) => test.booleansStructProperty = val);
    });
}

function runCompositeStructProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.composite.valid, TestValues.composite.invalid, 
            () => test.compositeStructProperty, 
            (val) => test.compositeStructProperty = val);
    });
}

function runRefBooleanProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncRefPropertyTest(pThis, TestValues.bools.valid, 
            () => test.refBooleanProperty, 
            (val) => test.refBooleanProperty = val);
    });
}

function runRefCharProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncRefPropertyTest(pThis, TestValues.chars.valid, 
            () => test.refCharProperty, 
            (val) => test.refCharProperty = val);
    });
}

function runRefNumericProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncRefPropertyTest(pThis, TestValues.s32.valid, 
            () => test.refNumericProperty, 
            (val) => test.refNumericProperty = val);
    });
}

function runRefEnumProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncRefPropertyTest(pThis, TestValues.enums.valid, 
            () => test.refEnumProperty, 
            (val) => test.refEnumProperty = val);
    });
}

function runObjectProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncObjectPropertyTest(pThis, TestComponent, TestValues.s32.valid, 
            () => test.objectProperty, 
            (val) => test.objectProperty = val);
    });
}

function runDateTimeProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var initial = test.dateTimeProperty;
        assert.equal(typeof(initial), 'object');
        assert.equal(typeof(initial.getTime), 'function');

        var val = new Date();
        test.dateTimeProperty = val;
        assert.equal(test.dateTimeProperty.getTime(), val.getTime());
    });
}

function runTimeSpanProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var initial = test.timeSpanProperty;
        assert.equal(typeof(initial), 'number');

        var val = 1234567890;
        test.timeSpanProperty = val;
        assert.equal(test.timeSpanProperty, val);
    });
}

function runPropertyValueProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        var initial = test.propertyValue;
        assert.equal(initial, null);

        // Set a boolean property value
        test.assignPropertyValueAsType(true, 'Boolean');
        assert.equal(test.propertyValueCppType, 'Boolean');
        assert.equal(test.propertyValue, true);

        // Set a number property value
        test.assignPropertyValueAsType(42, 'Int32');
        assert.equal(test.propertyValueCppType, 'Int32');
        assert.equal(test.propertyValue, 42);
    });
}

function runHResultProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'number', TestValues.hResults.valid, TestValues.hResults.invalid, 
            () => test.hResultProperty, 
            (val) => test.hResultProperty = val);
    });
}

// Non-static array properties
function runBooleanArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.bools.validArrays, TestValues.bools.invalidArrays, 
            () => test.booleanArrayProperty, 
            (val) => test.booleanArrayProperty = val);
    });
}

function runCharArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.chars.validArrays, TestValues.chars.invalidArrays, 
            () => test.charArrayProperty, 
            (val) => test.charArrayProperty = val);
    });
}

function runNumericArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.s32.validArrays, TestValues.s32.invalidArrays, 
            () => test.numericArrayProperty, 
            (val) => test.numericArrayProperty = val);
    });
}

function runStringArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.strings.validArrays, TestValues.strings.invalidArrays, 
            () => test.stringArrayProperty, 
            (val) => test.stringArrayProperty = val);
    });
}

function runGuidArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.guids.validArrays, TestValues.guids.invalidArrays, 
            () => test.guidArrayProperty, 
            (val) => test.guidArrayProperty = val);
    });
}

function runEnumArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.enums.validArrays, TestValues.enums.invalidArrays, 
            () => test.enumArrayProperty, 
            (val) => test.enumArrayProperty = val);
    });
}

function runCompositeStructArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object', TestValues.composite.validArrays, TestValues.composite.invalidArrays, 
            () => test.compositeStructArrayProperty, 
            (val) => test.compositeStructArrayProperty = val);
    });
}

function runRefArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        runSyncPropertyTest(pThis, 'object',
            [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], TestValues.s32.valid],
            [42, ['A'], [true], ['42'], TestValues.s32.invalid],
            () => test.refArrayProperty, 
            (val) => test.refArrayProperty = val);
    });
}

function runObjectArrayProperty(pThis, TestComponent, TestValues) {
    pThis.runSync(null, () => {
        var test = new TestComponent.Test();
        try {
            var initial = test.objectArrayProperty;
            assert.equal(typeof(initial), 'object');
            assert.equal(initial.length, 0);

            var val = [new TestComponent.TestObject(0), new TestComponent.TestObject(1), new TestComponent.TestObject(2)];
            test.objectArrayProperty = val;
            assert.equal(test.objectArrayProperty, val);
        } finally {
            test.objectArrayProperty = [];
        }
    });
}

module.exports = { makePropertiesTestScenarios };
