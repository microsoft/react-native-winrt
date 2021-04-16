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

export function makeCollectionsTestScenarios(pThis) {
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
        new TestScenario('IVector<TestObject>', runObjectVectorCopyTest.bind(pThis)),

        // Vectors that wrap arrays
        new TestScenario('Array as IVector<Boolean>', runBoolArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<Char>', runCharArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<Int32>', runNumericArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<String>', runStringArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<Guid>', runGuidArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<TestEnum>', runEnumArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<CompositeType>', runCompositeStructArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<IReference<Int32>>', runRefArrayAsVectorTest.bind(pThis)),
        new TestScenario('Array as IVector<TestObject>', runObjectArrayAsVectorTest.bind(pThis)),

        // Vectors behave like arrays
        new TestScenario('IVector behaves like Array', runVectorAsArrayTest.bind(pThis)),
        new TestScenario('IVectorView behaves like Array', runVectorViewAsArrayTest.bind(pThis)),
        new TestScenario('IMap with string keys behaves like JS object', runIMapAsJSObjectTest.bind(pThis)),
        new TestScenario('IMapView with string keys behaves like readonly JS object', runIMapViewAsReadonlyJSObject.bind(pThis)),
    ];
}

function verifyVectorContents(vector, expected) {
    assert.equal(expected.length, vector.size);

    for (var i = 0; i < expected.length; ++i) {
        assert.equal(expected[i], vector.getAt(i));
    }
}

function verifyVectorContentsWithIndexOperator(vector, expected) {
    assert.equal(expected.length, vector.size);

    assert.equal(undefined, vector['']);
    assert.equal(undefined, vector[-1]);
    assert.equal(undefined, vector[expected.length]);
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
    verifyVectorContentsWithIndexOperator(vector, expected);
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

    vector[3] = valuesToAdd[3];
    expected[3] = valuesToAdd[3];
    verifyVector(vector, expected);

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

function runObjectVectorCopyTest(scenario) {
    this.runSync(scenario, () => {
        var values = numericVectorContents.map(val => new TestComponent.TestObject(val));
        var valuesToAdd = numericValuesToAdd.map(val => new TestComponent.TestObject(val));
        doVectorCopyTest(TestComponent.Test.copyObjectsToVector(values), values, valuesToAdd);
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
    assert.equal(array, expected);

    vector.insertAt(2, valuesToAdd[1]);
    expected.splice(2, 0, valuesToAdd[1]);
    assert.equal(array, expected);

    // Modify values
    vector.setAt(1, valuesToAdd[2]);
    expected[1] = valuesToAdd[2];
    assert.equal(array, expected);

    vector[3] = valuesToAdd[3];
    expected[3] = valuesToAdd[3];
    assert.equal(array, expected);

    // Remove/replace values
    vector.removeAtEnd();
    expected.pop();
    assert.equal(array, expected);

    vector.removeAt(0);
    expected.splice(0, 1);
    assert.equal(array, expected);

    vector.replaceAll(expectedInitial);
    assert.equal(array, expectedInitial);

    vector.clear();
    assert.equal(array, []);
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

function runObjectArrayAsVectorTest(scenario) {
    this.runSync(scenario, () => {
        var values = numericVectorContents.map(val => new TestComponent.TestObject(val));
        var valuesToAdd = numericValuesToAdd.map(val => new TestComponent.TestObject(val));
        doArrayAsVectorTest(TestComponent.Test.returnSameObjectVector(values), values, valuesToAdd);
    });
}

// Vectors behave like arrays
function doVectorConcatTest(makeVectorFn, arr, ...values) {
    var vector = makeVectorFn(arr);
    // assert.equal(vector.concat(...values), arr.concat(...values));

    var otherNumericArray = [1, 2, 3];
    var otherNumericVector = TestComponent.Test.copyNumericsToVector(otherNumericArray);
    assert.equal(vector.concat(otherNumericVector, ...values), arr.concat(otherNumericArray, ...values));

    // TODO: This won't work until we can respond to 'Symbol.isConcatSpreadable'
    // assert.equal(arr.concat(TestComponent.Test.copyNumericsToVector([...values])), arr.concat([...values]));
}

function doVectorEntriesTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    var vectorItr = vector.entries();
    var arrItr = arr.entries();
    var len = 0;
    while (true) {
        var vectorNxt = vectorItr.next();
        var arrNxt = arrItr.next();
        assert.equal(arrNxt.done, vectorNxt.done);
        assert.equal(arrNxt.value, vectorNxt.value);
        if (arrNxt.done) break;
        ++len;
    }
    assert.equal(arr.length, len);
}

function doVectorEveryTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    var callback = (val) => (val % 2) == 0;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = (val, index, array) => (array[index] % 2) == 0;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = (val, index) => (index % 2) == 0;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => 0;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => 1;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => '';
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => 'foo';
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => null;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => undefined;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => Symbol.iterator;
    assert.equal(arr.every(callback), vector.every(callback));

    callback = () => {};
    assert.equal(arr.every(callback), vector.every(callback));

    var thisVal = { modEqual: 0 };
    callback = function(val) { return (val % 2) == this.modEqual; };
    assert.equal(arr.every(callback, thisVal), vector.every(callback, thisVal));
}

function doVectorFilterTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    var callback = (val) => (val % 2) == 0;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = (val, index, array) => (array[index] % 2) == 0;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = (val, index) => (index % 2) == 0;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => 0;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => 1;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => '';
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => 'foo';
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => null;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => undefined;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => Symbol.iterator;
    assert.equal(arr.filter(callback), vector.filter(callback));

    callback = () => {};
    assert.equal(arr.filter(callback), vector.filter(callback));

    var thisVal = { modEqual: 0 };
    callback = function(val) { return (val % 2) == this.modEqual; };
    assert.equal(arr.filter(callback, thisVal), vector.filter(callback, thisVal));
}

function doVectorFindAndFindIndexTest(makeVectorFn, arr, value) {
    var vector = makeVectorFn(arr);
    var callback = (val) => val == value;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = (val, index, array) => array[index] == value;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = (val, index) => index == value;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => 0;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => 1;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => '';
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => 'foo';
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => null;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => undefined;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => Symbol.iterator;
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    callback = () => {};
    assert.equal(arr.find(callback), vector.find(callback));
    assert.equal(arr.findIndex(callback), vector.findIndex(callback));

    var thisVal = { findValue: value };
    callback = function(val) { return val == this.findValue };
    assert.equal(arr.find(callback, thisVal), vector.find(callback, thisVal));
    assert.equal(arr.findIndex(callback, thisVal), vector.findIndex(callback, thisVal));
}

function doVectorForEachTest(makeVectorFn, values) {
    var vector = makeVectorFn(values);
    var index = 0;
    vector.forEach((val, i, arr) => {
        assert.equal(index, i);
        assert.isTrue(arr == vector);
        assert.equal(values[index], val);
        ++index;
    });
    assert.equal(values.length, index);

    var thisVal = { index: 0 };
    vector.forEach(function(val, i, arr) {
        assert.equal(this.index, i);
        assert.isTrue(arr == vector);
        assert.equal(values[this.index], val);
        ++this.index;
    }, thisVal);
    assert.equal(values.length, thisVal.index);
}

function doVectorIncludesAndLastIndexOfTest(makeVectorFn, arr, ...args) {
    var vector = makeVectorFn(arr);
    assert.equal(arr.includes(...args), vector.includes(...args));
    assert.equal(arr.lastIndexOf(...args), vector.lastIndexOf(...args));
}

function doVectorJoinTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    assert.equal(arr.join(), vector.join());
    assert.equal(arr.join(','), vector.join(','));
}

function doVectorKeysTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    var expected = 0;
    for (var key of vector.keys()) {
        assert.equal(expected++, key);
    }
    assert.equal(arr.length, expected);
}

function doVectorMapTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    var mapFn = (val, index) => {
        return { value: val, index };
    };
    assert.equal(arr.map(mapFn), vector.map(mapFn));

    var thisArg = { value: 'test' };
    mapFn = function(val, index) { return {value: val, index, thisVal: this.value }; };
    assert.equal(arr.map(mapFn, thisArg), vector.map(mapFn, thisArg));
}

function doVectorReduceTest(makeVectorFn, arr, ...args) {
    var vector = makeVectorFn(arr);
    var callback = (accum, value) => accum + value;
    assert.equal(arr.reduce(callback, ...args), vector.reduce(callback, ...args));
    assert.equal(arr.reduceRight(callback, ...args), vector.reduceRight(callback, ...args));

    var callback = (accum, value) => (accum << 1) | value;
    assert.equal(arr.reduce(callback, ...args), vector.reduce(callback, ...args));
    assert.equal(arr.reduceRight(callback, ...args), vector.reduceRight(callback, ...args));

    callback = (accum, value, index) => accum + index;
    assert.equal(arr.reduce(callback, ...args), vector.reduce(callback, ...args));
    assert.equal(arr.reduceRight(callback, ...args), vector.reduceRight(callback, ...args));

    callback = (accum, value, index, array) => accum + array[index];
    assert.equal(arr.reduce(callback, ...args), vector.reduce(callback, ...args));
    assert.equal(arr.reduceRight(callback, ...args), vector.reduceRight(callback, ...args));
}

function doVectorSliceTest(makeVectorFn, arr, ...args) {
    var vector = makeVectorFn(arr);
    assert.equal(arr.slice(...args), vector.slice(...args));
}

function doVectorSomeTest(makeVectorFn, arr, value) {
    var vector = makeVectorFn(arr);
    var callback = (val) => val == value;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = (val, index, array) => array[index] == value;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = (val, index) => index == value;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => 0;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => 1;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => '';
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => 'foo';
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => null;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => undefined;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => Symbol.iterator;
    assert.equal(arr.some(callback), vector.some(callback));

    callback = () => {};
    assert.equal(arr.some(callback), vector.some(callback));

    var thisVal = { findValue: value };
    callback = function(val) { return val == this.findValue };
    assert.equal(arr.some(callback, thisVal), vector.some(callback, thisVal));
}

function doVectorToStringTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    assert.equal(arr.toString(), vector.toString());
}

function doVectorValuesTest(makeVectorFn, arr) {
    var vector = makeVectorFn(arr);
    var vectorItr = vector.values();
    var arrItr = arr.values();
    var len = 0;
    while (true) {
        var vectorNxt = vectorItr.next();
        var arrNxt = arrItr.next();
        assert.equal(arrNxt.done, vectorNxt.done);
        assert.equal(arrNxt.value, vectorNxt.value);
        if (arrNxt.done) break;
        ++len;
    }
    assert.equal(len, arr.length);
}

function doCommonVectorAsArrayTest(makeVectorFn) {
    // Array.prototype.concat
    doVectorConcatTest(makeVectorFn, []);
    doVectorConcatTest(makeVectorFn, [], 42);
    doVectorConcatTest(makeVectorFn, [], [42]);
    doVectorConcatTest(makeVectorFn, [], 1, 2, 3);
    doVectorConcatTest(makeVectorFn, [], [1, 2, 3]);
    doVectorConcatTest(makeVectorFn, [], [1, 2], 3, 4, 5, [6], [7, 8, 9]);
    doVectorConcatTest(makeVectorFn, numericVectorContents);
    doVectorConcatTest(makeVectorFn, numericVectorContents, 42);
    doVectorConcatTest(makeVectorFn, numericVectorContents, [42]);
    doVectorConcatTest(makeVectorFn, numericVectorContents, 1, 2, 3);
    doVectorConcatTest(makeVectorFn, numericVectorContents, [1, 2, 3]);
    doVectorConcatTest(makeVectorFn, numericVectorContents, [1, 2], 3, 4, 5, [6], [7, 8, 9]);
    doVectorConcatTest(makeVectorFn, numericVectorContents, undefined, null, 1, 'one', {value: 1}, [undefined, null, 2, 'two', {value: 2}]);

    // Array.prototype.entries
    doVectorEntriesTest(makeVectorFn, []);
    doVectorEntriesTest(makeVectorFn, [1]);
    doVectorEntriesTest(makeVectorFn, [1, 2, 3, 4, 5]);

    // Array.prototype.every
    assert.throwsError(() => makeVectorFn([]).every());
    assert.throwsError(() => makeVectorFn([]).every(42));
    doVectorEveryTest(makeVectorFn, []);
    doVectorEveryTest(makeVectorFn, [1]);
    doVectorEveryTest(makeVectorFn, [2]);
    doVectorEveryTest(makeVectorFn, [2, 4, 6, 8, 10]);
    doVectorEveryTest(makeVectorFn, [2, 4, 6, 9, 10]);

    // Array.prototype.filter
    assert.throwsError(() => makeVectorFn([]).filter());
    assert.throwsError(() => makeVectorFn([]).filter(42));
    doVectorFilterTest(makeVectorFn, []);
    doVectorFilterTest(makeVectorFn, [1]);
    doVectorFilterTest(makeVectorFn, [2]);
    doVectorFilterTest(makeVectorFn, [2, 4, 6, 8, 10]);
    doVectorFilterTest(makeVectorFn, [2, 4, 6, 9, 10]);
    doVectorFilterTest(makeVectorFn, numericVectorContents);

    // Array.prototype.find / Array.prototype.findIndex
    doVectorFindAndFindIndexTest(makeVectorFn, [], 42);
    doVectorFindAndFindIndexTest(makeVectorFn, [0], 42);
    doVectorFindAndFindIndexTest(makeVectorFn, [42], 42);
    doVectorFindAndFindIndexTest(makeVectorFn, numericVectorContents, 5);

    // TODO: flat & flatMap

    // Array.prototype.forEach
    assert.throwsError(() => makeVectorFn([]).forEach());
    assert.throwsError(() => makeVectorFn([]).forEach(42));
    doVectorForEachTest(makeVectorFn, []);
    doVectorForEachTest(makeVectorFn, [42]);
    doVectorForEachTest(makeVectorFn, numericVectorContents);

    // Array.prototype.includes / Array.prototype.lastIndexOf
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [], 42);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [42]);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [0], 42);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [42], 42);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [42], 42, 1);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [42, 42]);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [42, 42], 0);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, [42, 42], -0);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, 3);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, '3');
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, 100);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, -3);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, -3);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, -100);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, null);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, undefined);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, true);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, Infinity);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, -Infinity);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, NaN);
    doVectorIncludesAndLastIndexOfTest(makeVectorFn, numericVectorContents, 3, -NaN);

    // Array.prototype.join
    doVectorJoinTest(makeVectorFn, []);
    doVectorJoinTest(makeVectorFn, [42]);
    doVectorJoinTest(makeVectorFn, numericVectorContents);

    // Array.prototype.keys
    doVectorKeysTest(makeVectorFn, []);
    doVectorKeysTest(makeVectorFn, [42]);
    doVectorKeysTest(makeVectorFn, numericVectorContents);

    // Array.prototype.map
    assert.throwsError(() => makeVectorFn([]).map());
    assert.throwsError(() => makeVectorFn([]).map(42));
    doVectorMapTest(makeVectorFn, []);
    doVectorMapTest(makeVectorFn, [42]);
    doVectorMapTest(makeVectorFn, numericVectorContents);

    // Array.prototype.reduce / Array.prototype.reduceRight
    assert.throwsError(() => makeVectorFn([]).reduce());
    assert.throwsError(() => makeVectorFn([]).reduce(42));
    assert.throwsError(() => makeVectorFn([]).reduce(() => 42));
    assert.throwsError(() => makeVectorFn([]).reduceRight());
    assert.throwsError(() => makeVectorFn([]).reduceRight(42));
    assert.throwsError(() => makeVectorFn([]).reduceRight(() => 42));
    doVectorReduceTest(makeVectorFn, [], 42);
    doVectorReduceTest(makeVectorFn, [42]);
    doVectorReduceTest(makeVectorFn, [42], 8);
    doVectorReduceTest(makeVectorFn, numericVectorContents);
    doVectorReduceTest(makeVectorFn, numericVectorContents, 42);

    // Array.prototype.slice
    doVectorSliceTest(makeVectorFn, []);
    doVectorSliceTest(makeVectorFn, [42]);
    doVectorSliceTest(makeVectorFn, [42], 1);
    doVectorSliceTest(makeVectorFn, [42], -1);
    doVectorSliceTest(makeVectorFn, [42], 0, 0);
    doVectorSliceTest(makeVectorFn, numericVectorContents);
    doVectorSliceTest(makeVectorFn, numericVectorContents, 3);
    doVectorSliceTest(makeVectorFn, numericVectorContents, 3, 6);
    doVectorSliceTest(makeVectorFn, numericVectorContents, -5);
    doVectorSliceTest(makeVectorFn, numericVectorContents, -5, -3);
    doVectorSliceTest(makeVectorFn, numericVectorContents, -100);
    doVectorSliceTest(makeVectorFn, numericVectorContents, 5, 2);
    doVectorSliceTest(makeVectorFn, numericVectorContents, -0, 2);
    doVectorSliceTest(makeVectorFn, numericVectorContents, undefined, true);
    doVectorSliceTest(makeVectorFn, numericVectorContents, null, '3');
    doVectorSliceTest(makeVectorFn, numericVectorContents, -Infinity, Infinity);
    doVectorSliceTest(makeVectorFn, numericVectorContents, NaN);

    // Array.prototype.some
    assert.throwsError(() => makeVectorFn([]).some());
    assert.throwsError(() => makeVectorFn([]).some(42));
    doVectorSomeTest(makeVectorFn, [], 42);
    doVectorSomeTest(makeVectorFn, [42], 8);
    doVectorSomeTest(makeVectorFn, [42], 42);
    doVectorSomeTest(makeVectorFn, numericVectorContents, 1);
    doVectorSomeTest(makeVectorFn, numericVectorContents, 3);
    doVectorSomeTest(makeVectorFn, numericVectorContents, 5);
    doVectorSomeTest(makeVectorFn, numericVectorContents, 0);

    // TODO: toLocaleString / toSource?

    // Array.prototype.toString
    doVectorToStringTest(makeVectorFn, []);
    doVectorToStringTest(makeVectorFn, [42]);
    doVectorToStringTest(makeVectorFn, numericVectorContents);

    // Array.prototype.values
    doVectorValuesTest(makeVectorFn, []);
    doVectorValuesTest(makeVectorFn, [42]);
    doVectorValuesTest(makeVectorFn, numericVectorContents);
}

function doVectorAppendWithAssignTest() {
    var vector = TestComponent.Test.copyNumericsToVector([]);
    var expected = [];

    vector[0] = 42;
    expected[0] = 42;
    verifyVectorContents(vector, expected);

    vector[1] = 8;
    expected[1] = 8;
    verifyVectorContents(vector, expected);
}

function doVectorSetLengthTest(newLen) {
    var vector = TestComponent.Test.copyNumericsToVector(numericVectorContents);
    var expected = [...numericVectorContents];
    var expectException = newLen > expected.length;
    try
    {
        vector.length = newLen;
        expected.length = newLen;
        verifyVectorContents(vector, expected);
        assert.isTrue(!expectException);
    } catch {
        assert.isTrue(expectException);
    }
}

function doVectorCopyWithinTest(arr, ...args) {
    var vector = TestComponent.Test.copyNumericsToVector(arr);
    var expected = [...arr];
    var result = vector.copyWithin(...args);
    assert.isTrue(result == vector);
    expected.copyWithin(...args);
    verifyVectorContents(vector, expected);
}

function doVectorFillTest(arr, ...args) {
    var vector = TestComponent.Test.copyNumericsToVector(arr);
    var expected = [...arr];
    var result = vector.fill(...args);
    assert.isTrue(result == vector);
    expected.fill(...args);
    verifyVector(vector, expected);
}

function doVectorPushPopTest() {
    var vector = TestComponent.Test.copyNumericsToVector([]);
    var expected = [];

    var vLen = vector.push(42);
    var aLen = expected.push(42);
    assert.equal(aLen, vLen);
    verifyVectorContents(vector, expected);

    vLen = vector.push(1, 2, 3, 4, 5);
    aLen = expected.push(1, 2, 3, 4, 5);
    assert.equal(aLen, vLen);
    verifyVectorContents(vector, expected);

    while (expected.length != 0) {
        assert.equal(expected.pop(), vector.pop());
        verifyVectorContents(vector, expected);
    }
    assert.equal(expected.pop(), vector.pop());
}

function doVectorReverseTest(arr) {
    var vector = TestComponent.Test.copyNumericsToVector(arr);
    var expected = [...arr].reverse();
    assert.isTrue(vector == vector.reverse());
    verifyVectorContents(vector, expected);
}

function doVectorShiftUnshiftTest(arr) {
    var vector = TestComponent.Test.copyNumericsToVector(arr);
    var expected = [...arr]
    while (expected.length != 0) {
        assert.equal(expected.shift(), vector.shift());
        verifyVectorContents(vector, expected);
    }

    assert.equal(undefined, vector.shift());

    for (var val of arr) {
        assert.equal(expected.unshift(val), vector.unshift(val));
        verifyVectorContents(vector, expected);
    }

    assert.equal(expected.unshift(), vector.unshift());
    verifyVectorContents(vector, expected);

    assert.equal(expected.unshift(8, 42), vector.unshift(8, 42));
    verifyVectorContents(vector, expected);
}

function doVectorSortTest(arr) {
    var vector = TestComponent.Test.copyNumericsToVector(arr);
    var expected = [...arr];
    assert.isTrue(vector == vector.sort());
    verifyVectorContents(vector, expected.sort());

    var compareFn = (lhs, rhs) => lhs - rhs;
    verifyVectorContents(vector.sort(compareFn), expected.sort(compareFn));
}

function doVectorSpliceTest() {
    var vector = TestComponent.Test.copyNumericsToVector(numericVectorContents);
    var expected = [...numericVectorContents];

    // No-op
    assert.equal(vector.splice(), expected.splice()); // [0, 1, 2, 3, 4]
    verifyVectorContents(vector, expected);

    // No-op again
    assert.equal(vector.splice(10), expected.splice(10)); // [0, 1, 2, 3, 4]
    verifyVectorContents(vector, expected);

    // Remove until the end
    assert.equal(vector.splice(3), expected.splice(3)); // [0, 1, 2]
    verifyVectorContents(vector, expected);

    // Remove single element
    assert.equal(vector.splice(1, 1), expected.splice(1, 1)); // [0, 2]
    verifyVectorContents(vector, expected);

    // Insert single element
    assert.equal(vector.splice(1, 0, 42), expected.splice(1, 0, 42)); // [0, 42, 2]
    verifyVectorContents(vector, expected);

    // Replace single element
    assert.equal(vector.splice(0, 1, 1), expected.splice(0, 1, 1)); // [1, 42, 2]
    verifyVectorContents(vector, expected);

    // Replace with more elements
    assert.equal(vector.splice(1, 1, 2, 3, 4), expected.splice(1, 1, 2, 3, 4)); // [1, 2, 3, 4, 2]
    verifyVectorContents(vector, expected);

    // Replace with fewer elements
    assert.equal(vector.splice(1, 3, 42, 8), expected.splice(1, 3, 42, 8)); // [1, 42, 8, 2]
    verifyVectorContents(vector, expected);

    // Remove all
    assert.equal(vector.splice(0), expected.splice(0)); // []
    verifyVectorContents(vector, expected);

    // Add many
    assert.equal(vector.splice(0, 5, 1, 2, 3, 4, 5, 6, 7), expected.splice(0, 5, 1, 2, 3, 4, 5, 6, 7)); // [1, 2, 3, 4, 5, 6, 7]
    verifyVectorContents(vector, expected);

    // Remove many
    assert.equal(vector.splice(1, 3), expected.splice(1, 3)); // [1, 5, 6, 7]
    verifyVectorContents(vector, expected);

    // Replace many
    assert.equal(vector.splice(1, 6, 8, 9, 10), expected.splice(1, 6, 8, 9, 10)); // [1, 8, 9, 10]
    verifyVectorContents(vector, expected);

    // With various arguments
    assert.equal(vector.splice(undefined, '1', 42), expected.splice(undefined, '1', 42)); // [42, 8, 9, 10]
    verifyVectorContents(vector, expected);

    assert.equal(vector.splice(null, NaN, 77), expected.splice(null, NaN, 77)); // [77, 42, 8, 9, 10]
    verifyVectorContents(vector, expected);

    assert.equal(vector.splice(-Infinity, Infinity, 88), expected.splice(-Infinity, Infinity, 88)); // [88]
    verifyVectorContents(vector, expected);
}

function runVectorAsArrayTest(scenario) {
    this.runSync(scenario, () => {
        doCommonVectorAsArrayTest(TestComponent.Test.copyNumericsToVector);

        doVectorAppendWithAssignTest();

        // Array.prototype.length (set only)
        doVectorSetLengthTest(0);
        doVectorSetLengthTest(3);
        doVectorSetLengthTest(numericVectorContents.length);
        doVectorSetLengthTest(100);

        // Array.prototype.copyWithin
        doVectorCopyWithinTest([]); // Copy nothing
        doVectorCopyWithinTest(numericVectorContents); // Copy nothing
        doVectorCopyWithinTest(numericVectorContents, undefined); // Copy nothing
        doVectorCopyWithinTest(numericVectorContents, 5); // Copy nothing
        doVectorCopyWithinTest(numericVectorContents, 0, 3, 3); // Copy nothing
        doVectorCopyWithinTest(numericVectorContents, 0, 3, 0); // Copy nothing
        doVectorCopyWithinTest(numericVectorContents, 0, 0); // Copy with full overlap
        doVectorCopyWithinTest(numericVectorContents, 0, 3, 4); // Copy the fourth to the first
        doVectorCopyWithinTest(numericVectorContents, 0, -2, -1); // Copy the fourth to the first
        doVectorCopyWithinTest(numericVectorContents, -0, 3); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, '0', '3'); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, null, 3, 5); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, 0, 3, 500); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, 0, -2); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, -6, 3); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, -Infinity, 3, Infinity); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, NaN, 3); // Copy last two to first two
        doVectorCopyWithinTest(numericVectorContents, true, 3); // Copy last two to second two
        doVectorCopyWithinTest(numericVectorContents, 0, 2); // Forward copy with overlap
        doVectorCopyWithinTest(numericVectorContents, 2, 0, 3); // Reverse copy with overlap
        doVectorCopyWithinTest(numericVectorContents, 2, 0); // Reverse copy with overlap
        doVectorCopyWithinTest(numericVectorContents, -1); // Copy first to last

        // Array.prototype.fill
        assert.throwsError(() => { TestComponent.Test.copyNumericsToVector([0]).fill('42'); });
        doVectorFillTest([], 0, 0, 100);
        doVectorFillTest(numericVectorContents, 42);
        doVectorFillTest(numericVectorContents, 42, 3);
        doVectorFillTest(numericVectorContents, 42, 3, 4);
        doVectorFillTest(numericVectorContents, 42, 3, 100);
        doVectorFillTest(numericVectorContents, 42, -2);
        doVectorFillTest(numericVectorContents, 42, -2, -1);
        doVectorFillTest(numericVectorContents, 42, -100);
        doVectorFillTest(numericVectorContents, 42, -0);
        doVectorFillTest(numericVectorContents, 42, '3');
        doVectorFillTest(numericVectorContents, 42, undefined);
        doVectorFillTest(numericVectorContents, 42, null);
        doVectorFillTest(numericVectorContents, 42, Infinity);
        doVectorFillTest(numericVectorContents, 42, -Infinity);
        doVectorFillTest(numericVectorContents, 42, NaN);
        doVectorFillTest(numericVectorContents, 42, -NaN);

        // Array.prototype.push / Array.prototype.pop
        doVectorPushPopTest();

        // Array.prototype.reverse
        doVectorReverseTest([]);
        doVectorReverseTest([42]);
        doVectorReverseTest([8, 42]);
        doVectorReverseTest(numericVectorContents);

        // Array.prototype.shift / Array.prototype.unshift
        doVectorShiftUnshiftTest([]);
        doVectorShiftUnshiftTest([42]);
        doVectorShiftUnshiftTest(numericVectorContents);

        // Array.prototype.sort
        assert.throwsError(() => TestComponent.Test.copyNumericsToVector([]).sort(42));
        doVectorSortTest([]);
        doVectorSortTest([42]);
        doVectorSortTest([1, 9, 17, 22, 1024, 42, 88, 7]);
        doVectorSortTest([...numericVectorContents].reverse());

        // Array.prototype.splice
        doVectorSpliceTest();
    });
}

function runVectorViewAsArrayTest(scenario) {
    this.runSync(scenario, () => {
        doCommonVectorAsArrayTest(TestComponent.Test.copyNumericsToVectorView);

        // TODO: Should we guarantee the presence/absence of exceptions?
        const swallowExceptions = (fn) => { try { fn(); } catch {} };

        var view = TestComponent.Test.copyNumericsToVectorView(numericVectorContents);

        // Assigning to element should fail
        swallowExceptions(() => view[0] = 42);
        swallowExceptions(() => view[100] = 42);
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.length (set should fail)
        swallowExceptions(() => view.length = 0);
        swallowExceptions(() => view.length = 100);
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.copyWithin
        swallowExceptions(() => view.copyWithin(0, 2));
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.fill
        swallowExceptions(() => view.fill(42));
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.push
        swallowExceptions(() => view.pop());
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.pop
        swallowExceptions(() => view.push(42));
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.reverse
        swallowExceptions(() => view.reverse());
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.shift
        swallowExceptions(() => view.shift());
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.unshift
        swallowExceptions(() => view.unshift(42));
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.sort
        swallowExceptions(() => view.sort());
        verifyVectorContents(view, numericVectorContents);

        // Array.prototype.splice
        swallowExceptions(() => view.splice(1, 2, 42));
        verifyVectorContents(view, numericVectorContents);
    });
}

function runIMapAsJSObjectTest(scenario) {
    this.runSync(scenario, () => {
        const map = TestComponent.Test.createStringToNumberMap();
        map["hello"] = 1;
        map.insert("world", 2);
        assert.equal(map["hello"], 1);
        assert.equal(map.lookup("hello"), 1);
        assert.equal(map["world"], 2);
        assert.equal(map.lookup("world"), 2);
        assert.equal(map.size, 2);

        map.clear();
        assert.equal(map.size, 0);
        assert.equal(typeof(map.clear), "function");
        map["clear"] = 3;
        assert.equal(typeof(map.clear), "function");
        assert.equal(map.lookup("clear"), 3);

        assert.throwsError(() => {
            map["a"] = "b";
        })
    });
}

function runIMapViewAsReadonlyJSObject(scenario) {
    this.runSync(scenario, () => {
        const map = TestComponent.Test.createStringToNumberMap();
        map["hello"] = 1;
        map.insert("world", 2);

        const mapView = TestComponent.Test.copyToMapView(map);
        assert.equal(mapView["hello"], 1);
        assert.equal(mapView["world"], 2);
        assert.equal(mapView.size, 2);
    });
}
