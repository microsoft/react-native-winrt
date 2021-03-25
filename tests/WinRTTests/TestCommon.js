/**
 * @format
 */

import EventEmitter from 'react-native/Libraries/vendor/emitter/EventEmitter';

function stringify(val) {
    if (val == null) return 'null';
    else if (typeof(val) == 'object') return JSON.stringify(val);
    return val.toString();
}

function checkEquals(lhs, rhs) {
    if (typeof(lhs) != typeof(rhs)) {
        return {
            success: false,
            msg: 'Types are not the same. \'' + typeof(lhs) + '\' (' + stringify(lhs) + ') vs \'' + typeof(rhs) + '\' (' + stringify(rhs) + ')'
        };
    } else if ((lhs == null) && (rhs == null)) {
        return { success: true };
    } else if ((lhs == null) || (rhs == null)) {
        return {
            success: false,
            msg: '\'' + stringify(lhs) + '\' =/= \'' + stringify(rhs) + '\''
        };
    }

    if (typeof(lhs) === 'object') {
        for (var prop in lhs) {
            if (!rhs.hasOwnProperty(prop)) {
                return {
                    success: false,
                    msg: 'Object does not have property \'' + prop + '\''
                };
            }
        }

        for (var prop in rhs) {
            if (!lhs.hasOwnProperty(prop)) {
                return {
                    success: false,
                    msg: 'Object does not have property \'' + prop + '\''
                };
            }

            var result = checkEquals(lhs[prop], rhs[prop]);
            if (!result.success) return result;
        }
    } else {
        if (lhs != rhs) {
            return {
                success: false,
                msg: '\'' + stringify(lhs) + '\' =/= \'' + stringify(rhs) + '\''
            };
        }
    }

    return { success: true };
}

export const assert = {
    isTrue(val) {
        if (!val) {
            throw new Error('Assertion failed!');
        }
    },

    equal(lhs, rhs) {
        var result = checkEquals(lhs, rhs);
        if (!result.success) {
            throw new Error('assertEqual failed! ' + result.msg);
        }
    },

    notEqual(lhs, rhs) {
        var result = checkEquals(lhs, rhs);
        if (result.success) {
            throw new Error('assertNotEqual failed! \'' + stringify(lhs) + '\' == \'' + stringify(rhs) + '\'');
        }
    }
};

export function guidFromString(str) {
    // XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
    assert.equal(str.length, 36);
    assert.equal(str.charAt(8), '-');
    assert.equal(str.charAt(13), '-');
    assert.equal(str.charAt(18), '-');
    assert.equal(str.charAt(23), '-');
    return {
        data1: parseInt(str.substr(0, 8), 16),
        data2: parseInt(str.substr(9, 4), 16),
        data3: parseInt(str.substr(14, 4), 16),
        data4: [
            parseInt(str.substr(19, 2), 16),
            parseInt(str.substr(21, 2), 16),
            parseInt(str.substr(24, 2), 16),
            parseInt(str.substr(26, 2), 16),
            parseInt(str.substr(28, 2), 16),
            parseInt(str.substr(30, 2), 16),
            parseInt(str.substr(32, 2), 16),
            parseInt(str.substr(34, 2), 16),
        ]
    };
}

export function makeGuid(data1, data2, data3, data4) {
    var result = '';
    var append = (val, len) => {
        var str = val.toString(16).toUpperCase();
        while (str.length < len) str = '0' + str;
        result += str;
    };

    append(data1, 8);
    result += '-';
    append(data2, 4);
    result += '-';
    append(data3, 4);
    result += '-';
    append(data4[0], 2);
    append(data4[1], 2);
    result += '-';
    append(data4[2], 2);
    append(data4[3], 2);
    append(data4[4], 2);
    append(data4[5], 2);
    append(data4[6], 2);
    append(data4[7], 2);

    return result;
}

export function stringFromGuid(guid) {
    return makeGuid(guid.data1, guid.data2, guid.data3, guid.data4);
}

export const TestResult = {
    NotRun: 0,
    Pass: 1,
    Fail: 2,
}

export class TestScenario extends EventEmitter {
    name;
    invoke;
    result = TestResult.NotRun;
    failureText = '';

    constructor(name, invoke) {
        super();
        this.name = name;
        this.invoke = invoke;
    }
}

export const zeroGuid = '00000000-0000-0000-0000-000000000000';
export const allSetGuid = 'FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF';

export const TestValues = {
    bools: {
        valid: [true, false],
        invalid: ['true', 0],
        validArrays: [[], [true], [true, false, true], [true, false]],
        invalidArrays: [true, ['true'], [1], ['true', 0]],
    },
    chars: {
        valid: ['\0', 'F', '⚾'],
        invalid: ['', 65, 'ABC'],
        validArrays: [[], ['F'], ['A', 'B', 'C', 'D', 'E', 'F', 'G'], ['\0', 'F', '⚾']],
        invalidArrays: ['F', 'ABCD', [65], [true], ['ABCD'], ['', 65, 'ABC']],
    },
    u8: {
        valid: [-0, 0, 127, 128, 255],
        invalid: [NaN, Infinity, -1, 0.5, 256],
    },
    u16: {
        valid: [-0, 0, 32767, 32768, 65535],
        invalid: [NaN, Infinity, -1, 0.5, 65536],
    },
    u32: {
        valid: [-0, 0, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF],
        invalid: [NaN, Infinity, -1, 0.5, 0x100000000],
    },
    u64: {
        valid: [-0, 0, 0x1FFFFFFFFFFFFF, 0xFFFFFFFFFFFFF800], // 64-bit float has 53-bit mantissa (52 explicit)
        invalid: [NaN, Infinity, -1, 0.5, 18446744073709551616], // 1.0p64
    },
    s16: {
        valid: [-32768, -0, 0, 32767],
        invalid: [NaN, Infinity, -32769, 0.5, 32768],
    },
    s32: {
        valid: [-0x80000000, -0, 0, 0x7FFFFFFF],
        invalid: [NaN, Infinity, -0x80000001, 0.5, 0x80000000, '42', true],
        validArrays: [[], [42], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9], [-0x80000000, -0, 0, 0x7FFFFFFF]],
        invalidArrays: [42, ['A'], [true], ['42'], [NaN, Infinity, -0x80000001, 0.5, 0x80000000]],
    },
    s64: {
        valid: [-0x7FFFFFFFFFFFFC00, -0x1FFFFFFFFFFFFF, -0, 0, 0x1FFFFFFFFFFFFF, 0x7FFFFFFFFFFFFC00],
        invalid: [NaN, Infinity, -9223372036854777856, 0.5, 9223372036854775808],
    },
    f32: {
        valid: [Infinity, -3.40282346638528859811704183485e+38, -1.40129846432481707092372958329e-45, -0, 0, 1.40129846432481707092372958329e-45, 3.40282346638528859811704183485e+38],
        invalid: [-3.40282346638528897590636046442e+38, -1.40129846432481691534834763676e-45, 1.40129846432481691534834763676e-45, 3.40282346638528897590636046442e+38],
    },
    f64: {
        valid: [Infinity, -1.79769313486231570814527423732e+308, -4.94065645841246544176568792868e-324, -0, 0, 4.94065645841246544176568792868e-324, 1.79769313486231570814527423732e+308],
    },
    strings: {
        valid: ['', '\0', 'foo', 'bar', 'foo\0bar', 'foo bar'],
        validArrays: [[], ['foo'], ['', '\0', 'foo', 'bar', 'foo\0bar'], ['', '\0', 'foo', 'bar', 'foo\0bar', 'foo bar']],
        invalidArrays: ['foo', [true], [42]],
    },
    guids: {
        // NOTE: Guids seem to always come back as uppercase. This assumption makes for easier comparisons, but is probably not guaranteed
        valid: [zeroGuid, allSetGuid, '01234567-89AB-CDEF-0123-456789ABCDEF'],
        invalid: ['', '{00000000-0000-0000-0000-000000000000}', '00000000 0000 0000 0000 000000000000', '01234567-89AB-CDEF-GHIJ-KLMNOPQRSTUV'],
        validArrays: [[], [zeroGuid], [zeroGuid, allSetGuid, '01234567-89AB-CDEF-0123-456789ABCDEF']],
        invalidArrays: [zeroGuid, ['{00000000-0000-0000-0000-000000000000}'], ['', '{00000000-0000-0000-0000-000000000000}', '00000000 0000 0000 0000 000000000000', '01234567-89AB-CDEF-GHIJ-KLMNOPQRSTUV']],
    },
    enums: {
        // NOTE: Enums are just represented as numbers
        valid: [ 1, TestComponent.TestEnum.first, TestComponent.TestEnum.second, TestComponent.TestEnum.third, TestComponent.TestEnum.fourth ],
        invalid: [ true, 'First', 'first', 'TestComponent.TestEnum.First', 'TestComponent.TestEnum.first' ],
        validArrays: [[], [1], [ 1, TestComponent.TestEnum.first, TestComponent.TestEnum.second, TestComponent.TestEnum.third, TestComponent.TestEnum.fourth ]],
        invalidArrays: [1, [true], [ true, 'First', 'first', 'TestComponent.TestEnum.First', 'TestComponent.TestEnum.first' ]],
    },
    numericTypes: {
        valid: [
            { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
            { u8: 255, u16: 65535, u32: 0xFFFFFFFF, u64: 0xFFFFFFFFFFFFF800, s16: -32768, s32: -0x80000000, s64: -0x7FFFFFFFFFFFFC00, f32: 9.80000019073486328125, f64: 6.67408e-11, enum: TestComponent.TestEnum.second },
        ],
        invalid: [
            {},
            { u8: '8', u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.14159, f64: 6.02e23, enum: TestComponent.TestEnum.first },
            { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.14159, f64: 6.02e23, enum: TestComponent.TestEnum.first, extraProperty: 'This should not be here' },
            { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.14159, f64: 6.02e23 },
        ],
    },
    stringTypes: {
        valid: [
            { char: '\0', string: '\0', guid: zeroGuid },
            { char: 'F', string: 'Hello, world!', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' }
        ],
        invalid: [
            {},
            { char: 65, string: 'Test', guid: zeroGuid },
            { char: 'A', string: 'Test', guid: zeroGuid, extraProperty: 42 },
            { char: 'A', string: 'Test' }
        ],
    },
    boolTypes: {
        valid: [ { value: true }, { value: false } ],
        invalid: [ {}, { value: 'true' }, { value: 1 }, { value: true, extraProperty: 42 } ],
    },
    dates: {
        valid: [new Date(2020, 2, 3, 4, 5, 6, 7)],
        cppValuesForValidDates: ["132277107060070000"],
        invalid: [132323, "21321321"]
    },
    timeSpans: {
        valid: [1726, 0, -218761],
        cppValuesForValidTimeSpans: ["17260000", "0", "-2187610000"],
        invalid: [{duration: 98217}, new Date()]
    },
    hResults: {
        valid: [1726, 0, -218761],
        invalid: [{hresult: 98217}, {}]
    },
    propertyValues: {
        valid: [true, 12723, "hello", [21, 23, 43], ["sasa", "sa"]],
        invalid: [{}, ["hi", true]]
    },
    composite: {
        valid: [
            {
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                strings: { char: '\0', string: '\0', guid: zeroGuid },
                bools: { value: true }
            },
            {
                numerics: { u8: 255, u16: 65535, u32: 0xFFFFFFFF, u64: 0xFFFFFFFFFFFFF800, s16: -32768, s32: -0x80000000, s64: -0x7FFFFFFFFFFFFC00, f32: 9.80000019073486328125, f64: 6.67408e-11, enum: TestComponent.TestEnum.second },
                strings: { char: 'F', string: 'Hello, world!', guid: allSetGuid },
                bools: { value: false }
            },
            {
                numerics: { u8: 0, u16: 0, u32: 0, u64: 0, s16: 0, s32: 0, s64: 0, f32: 0, f64: 0, enum: TestComponent.TestEnum.third },
                strings: { char: '⚾', string: 'foo\0bar', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
                bools: { value: true }
            }
        ],
        invalid: [
            {}, { numerics: {}, strings: {}, bools: {} },
            {
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                strings: { char: '\0', string: '\0', guid: zeroGuid }
            },
            {
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                strings: { char: '\0', string: '\0', guid: zeroGuid },
                bools: { value: true },
                extraProperty: 'This should not be here'
            },
            {
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                strings: { char: '\0', string: '\0', guid: zeroGuid },
                bools: { value: true, extraProperty: 'This should not be here' }
            }
        ],
        validArrays: [
            [],
            [{
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                strings: { char: '\0', string: '\0', guid: zeroGuid },
                bools: { value: true }
            }],
            [
                {
                    numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                    strings: { char: '\0', string: '\0', guid: zeroGuid },
                    bools: { value: true }
                },
                {
                    numerics: { u8: 255, u16: 65535, u32: 0xFFFFFFFF, u64: 0xFFFFFFFFFFFFF800, s16: -32768, s32: -0x80000000, s64: -0x7FFFFFFFFFFFFC00, f32: 9.80000019073486328125, f64: 6.67408e-11, enum: TestComponent.TestEnum.second },
                    strings: { char: 'F', string: 'Hello, world!', guid: allSetGuid },
                    bools: { value: false }
                },
                {
                    numerics: { u8: 0, u16: 0, u32: 0, u64: 0, s16: 0, s32: 0, s64: 0, f32: 0, f64: 0, enum: TestComponent.TestEnum.third },
                    strings: { char: '⚾', string: 'foo\0bar', guid: '01234567-89AB-CDEF-0123-456789ABCDEF' },
                    bools: { value: true }
                }
            ]
        ],
        invalidArrays: [
            {
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                strings: { char: '\0', string: '\0', guid: zeroGuid },
                bools: { value: true }
            },
            [{
                numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                strings: { char: '\0', string: '\0', guid: zeroGuid },
                bools: { value: true },
                extraProperty: 'This should not be here'
            }],
            [
                {}, { numerics: {}, strings: {}, bools: {} },
                {
                    numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                    strings: { char: '\0', string: '\0', guid: zeroGuid }
                },
                {
                    numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                    strings: { char: '\0', string: '\0', guid: zeroGuid },
                    bools: { value: true },
                    extraProperty: 'This should not be here'
                },
                {
                    numerics: { u8: 8, u16: 16, u32: 32, u64: 64, s16: -16, s32: -32, s64: -64, f32: 3.141590118408203125, f64: 6.02e23, enum: TestComponent.TestEnum.first },
                    strings: { char: '\0', string: '\0', guid: zeroGuid },
                    bools: { value: true, extraProperty: 'This should not be here' }
                }
            ]
        ],
    }
}
