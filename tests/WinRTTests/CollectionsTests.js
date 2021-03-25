/**
 * @format
 */

import {
    TestScenario,
    assert
} from './TestCommon'

// NOTE: Should be 5 initial values and 4 values to add
const boolVectorContents = [false, true, false, true, false];
const boolValuesToAdd = [true, true, false, false];

const charVectorContents = ['a', 'b', 'c', 'd', 'e'];
const charValuesToAdd = ['f', 'g', 'h', 'i'];

const numericVectorContents = [0, 1, 2, 3, 4];
const numericValuesToAdd = [5, 6, 7, 8];

const stringVectorContents = ['foo', 'bar', 'foo bar', 'foo\0bar', '\0'];
const stringValuesToAdd = ['baz', '\0baz\0', 'foo baz', 'foo\0baz'];

const guidVectorContents = ['00000000-0000-0000-0000-000000000000', 'FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF',
    'AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA', '01234567-89AB-CDEF-0123-456789ABCDEF', '01010101-2323-4545-6767-787878787878'];
const guidValuesToAdd = ['0CC4CA1B-9575-4B75-9FB6-A5C95760337B', '235E81D7-9F21-4A70-9494-C3192D892744',
    '5AA9874D-5E70-4EAA-882A-B03E1C79A97B', 'F95FEEB9-702D-4C93-BD15-52F274E45609'];

const enumVectorContents = [1, 2, 3, 4, 1];
const enumValuesToAdd = [2, 3, 4, 1];

const compositeVectorContents = [
    {
        numerics: { u8: 0, u16: 0, u32: 0, u64: 0, s16: 0, s32: 0, s64: 0, f32: 0, f64: 0, enum: 1 },
        strings: { char: 'a', string: 'aaa', guid: guidVectorContents[0] },
        bools: { value: true }
    },
    {
        numerics: { u8: 1, u16: 1, u32: 1, u64: 1, s16: 1, s32: 1, s64: 1, f32: 1, f64: 1, enum: 2 },
        strings: { char: 'b', string: 'bbb', guid: guidVectorContents[1] },
        bools: { value: false }
    },
    {
        numerics: { u8: 2, u16: 2, u32: 2, u64: 2, s16: 2, s32: 2, s64: 2, f32: 2, f64: 2, enum: 3 },
        strings: { char: 'c', string: 'ccc', guid: guidVectorContents[2] },
        bools: { value: true }
    },
    {
        numerics: { u8: 3, u16: 3, u32: 3, u64: 3, s16: 3, s32: 3, s64: 3, f32: 3, f64: 3, enum: 4 },
        strings: { char: 'd', string: 'ddd', guid: guidVectorContents[3] },
        bools: { value: false }
    },
    {
        numerics: { u8: 4, u16: 4, u32: 4, u64: 4, s16: 4, s32: 4, s64: 4, f32: 4, f64: 4, enum: 1 },
        strings: { char: 'e', string: 'eee', guid: guidVectorContents[4] },
        bools: { value: true }
    },
];
const compositeValuesToAdd = [
    {
        numerics: { u8: 5, u16: 5, u32: 5, u64: 5, s16: 5, s32: 5, s64: 5, f32: 5, f64: 5, enum: 2 },
        strings: { char: 'f', string: 'fff', guid: guidValuesToAdd[0] },
        bools: { value: false }
    },
    {
        numerics: { u8: 6, u16: 6, u32: 6, u64: 6, s16: 6, s32: 6, s64: 6, f32: 6, f64: 6, enum: 3 },
        strings: { char: 'g', string: 'ggg', guid: guidValuesToAdd[1] },
        bools: { value: true }
    },
    {
        numerics: { u8: 6, u16: 6, u32: 6, u64: 6, s16: 6, s32: 6, s64: 6, f32: 6, f64: 6, enum: 4 },
        strings: { char: 'h', string: 'hhh', guid: guidValuesToAdd[2] },
        bools: { value: false }
    },
    {
        numerics: { u8: 7, u16: 7, u32: 7, u64: 7, s16: 7, s32: 7, s64: 7, f32: 7, f64: 7, enum: 1 },
        strings: { char: 'i', string: 'iii', guid: guidValuesToAdd[3] },
        bools: { value: true }
    },
];

const refVectorContents = [null, 0, 1, 2, 3];
const refValuesToAdd = [4, 5, null, 6];

// TODO: Objects; probably need to create on the fly?

export function makeCollectionsTests(pThis) {
    return [
        // Vectors created from copies of arrays
        new TestScenario('IVector<Boolean>', runBoolVectorCopyTest.bind(pThis)),
        new TestScenario('IVector<Char>', runCharVectorCopyTest.bind(pThis)),
        new TestScenario('IVector<Int32>', runNumericVectorCopyTest.bind(pThis)),
        new TestScenario('IVector<String>', runStringVectorCopyTest.bind(pThis)),
        new TestScenario('IVector<Guid>', runGuidVectorCopyTest.bind(pThis)),
        new TestScenario('IVector<TestEnum>', runEnumVectorCopyTest.bind(pThis)),
        new TestScenario('IVector<CompositeType>', runCompositeStructVectorCopyTest.bind(pThis)),
        new TestScenario('IVector<IReference<Int32>>', runRefVectorCopyTest.bind(pThis)),
        // TODO: new TestScenario('IVector<Object>', runObjectVectorCopyTest.bind(pThis)),

        // Vectors that wrap arrays
        new TestScenario('Array as IVector<Boolean>', runBoolArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<Char>', runCharArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<Int32>', runNumericArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<String>', runStringArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<Guid>', runGuidArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<TestEnum>', runEnumArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<CompositeType>', runCompositeStructArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<IReference<Int32>>', runRefArrayAsVectorTest.bind(pThis)),
       // TODO: new TestScenario('Array as IVector<Object>', runObjectArrayAsVectorTest.bind(pThis)),
    ];
}

function verifyArraysAreSame(arr1, arr2) {
    assert.equal(arr1.length, arr2.length);
    for (var i = 0; i < arr1.length; ++i) {
        assert.equal(arr1[i], arr2[i]);
    }
}

function verifyVectorContents(vector, expected) {
    assert.equal(expected.length, vector.size);

    for (var i = 0; i < expected.length; ++i) {
        assert.equal(expected[i], vector.getAt(i));
    }
}

function verifyVectorContentsWithIndexOperator(vector, expected) {
    assert.equal(expected.length, vector.size);

    for (var i = 0; i < expected.length; ++i) {
        assert.equal(expected[i], vector[i]);
    }
}

function verifyVectorContentsWithGetMany(vector, expected) {
    var arr = new Array(3);
    for (var count = 0; count < expected.length; ) {
        var amt = vector.getMany(count, arr);
        for (var i = 0; i < amt; ++i) {
            assert.equal(expected[count], arr[i]);
            ++count;
        }
    }
}

function verifyContentsWithIterator(itr, expected) {
    var count = 0;
    while (itr.hasCurrent) {
        assert.isTrue(count < expected.length);
        assert.equal(expected[count], itr.current);
        itr.moveNext();
        ++count;
    }

    assert.equal(expected.length, count);
}

function verifyContentsWithIteratorGetMany(itr, expected) {
    var count = 0;
    var arr = new Array(3);
    while (itr.hasCurrent) {
        var amt = itr.getMany(arr);
        for (var i = 0; i < amt; ++i) {
            assert.isTrue(count < expected.length);
            assert.equal(expected[count], arr[i]);
            ++count;
        }
    }

    assert.equal(expected.length, count);
}

function verifyVector(vector, expected) {
    verifyVectorContents(vector, expected);
    // TODO: Not yet implemented
    // verifyVectorContentsWithIndexOperator(vector, expected);
    verifyVectorContentsWithGetMany(vector, expected);
    verifyContentsWithIterator(vector.first(), expected);
    verifyContentsWithIteratorGetMany(vector.first(), expected);
}

// Vectors created from copies of arrays
function doVectorCopyTest(vector, expectedInitial, valuesToAdd) {
    // NOTE: Assumes 5 elements in the vector to begin and 4 values to add
    var expected = [...expectedInitial];
    verifyVector(vector, expected);

    // Add values
    vector.append(valuesToAdd[0]);
    expected.push(valuesToAdd[0]);
    verifyVector(vector, expected);

    vector.insertAt(2, valuesToAdd[1]);
    expected.splice(2, 0, valuesToAdd[1]);
    verifyVector(vector, expected);

    // Modify values
    vector.setAt(1, valuesToAdd[2]);
    expected[1] = valuesToAdd[2];
    verifyVector(vector, expected);

    // TODO: operator[]

    // Remove/replace values
    vector.removeAtEnd();
    expected.pop();
    verifyVector(vector, expected);

    vector.removeAt(0);
    expected.splice(0, 1);
    verifyVector(vector, expected);

    vector.replaceAll(expectedInitial);
    verifyVector(vector, expectedInitial);

    vector.clear();
    verifyVector(vector, []);
}

function runBoolVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyBoolsToVector(boolVectorContents), boolVectorContents, boolValuesToAdd);
    });
}

function runCharVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyCharsToVector(charVectorContents), charVectorContents, charValuesToAdd);
    });
}

function runNumericVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyNumericsToVector(numericVectorContents), numericVectorContents, numericValuesToAdd);
    });
}

function runStringVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyStringsToVector(stringVectorContents), stringVectorContents, stringValuesToAdd);
    });
}

function runGuidVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyGuidsToVector(guidVectorContents), guidVectorContents, guidValuesToAdd);
    });
}

function runEnumVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyEnumValuesToVector(enumVectorContents), enumVectorContents, enumValuesToAdd);
    });
}

function runCompositeStructVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyCompositeStructsToVector(compositeVectorContents), compositeVectorContents, compositeValuesToAdd);
    });
}

function runRefVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        doVectorCopyTest(TestComponent.Test.copyRefsToVector(refVectorContents), refVectorContents, refValuesToAdd);
    });
}

// Vectors that wrap arrays
function doArrayAsVectorTest(vector, array, valuesToAdd) {
    // Modifications to 'vector' should be reflected in 'array'
    var expectedInitial = [...array];
    var expected = [...array];
    verifyVector(vector, expected);

    // Add values
    vector.append(valuesToAdd[0]);
    expected.push(valuesToAdd[0]);
    verifyArraysAreSame(array, expected);

    vector.insertAt(2, valuesToAdd[1]);
    expected.splice(2, 0, valuesToAdd[1]);
    verifyArraysAreSame(array, expected);

    // Modify values
    vector.setAt(1, valuesToAdd[2]);
    expected[1] = valuesToAdd[2];
    verifyArraysAreSame(array, expected);

    // TODO: operator[]

    // Remove/replace values
    vector.removeAtEnd();
    expected.pop();
    verifyArraysAreSame(array, expected);

    vector.removeAt(0);
    expected.splice(0, 1);
    verifyArraysAreSame(array, expected);

    vector.replaceAll(expectedInitial);
    verifyArraysAreSame(array, expectedInitial);

    vector.clear();
    verifyArraysAreSame(array, []);
}

function runBoolArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...boolVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameBoolVector(array), array, boolValuesToAdd);
    });
}

function runCharArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...charVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameCharVector(array), array, charValuesToAdd);
    });
}

function runNumericArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...numericVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameNumericVector(array), array, numericValuesToAdd);
    });
}

function runStringArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...stringVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameStringVector(array), array, stringValuesToAdd);
    });
}

function runGuidArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...guidVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameGuidVector(array), array, guidValuesToAdd);
    });
}

function runEnumArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...enumVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameEnumVector(array), array, enumValuesToAdd);
    });
}

function runCompositeStructArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...compositeVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameCompositeStructVector(array), array, compositeValuesToAdd);
    });
}

function runRefArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var array = [...refVectorContents];
        doArrayAsVectorTest(TestComponent.Test.returnSameRefVector(array), array, refValuesToAdd);
    });
}
