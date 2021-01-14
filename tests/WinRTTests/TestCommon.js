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
