const addon = require('./build/Debug/winrtaddon.node');
const TestComponent = addon.TestComponent;

console.log('Initial value:', TestComponent.Test.staticU32Property);

//console.log("object: ", TestComponent.Test);

// Test 1: Set a valid value
TestComponent.Test.staticU32Property = 42;
console.log('After setting 42:', TestComponent.Test.staticU32Property);

//console.log("object: ", TestComponent.Test);

// Test 2: Try to set Infinity (should throw)
try {
    console.log('Attempting to set Infinity...');
    TestComponent.Test.staticU32Property = Infinity;
    console.log('ERROR: Infinity was accepted! Value is now:', TestComponent.Test.staticU32Property);
} catch(e) {
    console.log('Good! Error caught:', e.message);
    console.log('Value after error:', TestComponent.Test.staticU32Property);
}

// Test 3: Try to set NaN (should throw)
try {
    console.log('Attempting to set NaN...');
    TestComponent.Test.staticU32Property = NaN;
    console.log('ERROR: NaN was accepted! Value is now:', TestComponent.Test.staticU32Property);
} catch(e) {
    console.log('Good! Error caught:', e.message);
    console.log('Value after error:', TestComponent.Test.staticU32Property);
}

console.log('Final value:', TestComponent.Test.staticU32Property);
