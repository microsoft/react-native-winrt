const addon = require('./build/Release/winrtaddon.node');
const TestComponent = addon.TestComponent;

console.log('Testing staticOrAll...');
console.log('TestComponent.Test:', TestComponent.Test);
console.log('TestComponent.Test.staticOrAll:', TestComponent.Test.staticOrAll);

try {
    const result = TestComponent.Test.staticOrAll([]);
    console.log('Result for []:', result);
} catch (e) {
    console.error('Error:', e);
    console.error('Error message:', e.message);
    console.error('Error stack:', e.stack);
}

try {
    const result = TestComponent.Test.staticOrAll([false]);
    console.log('Result for [false]:', result);
} catch (e) {
    console.error('Error:', e);
    console.error('Error message:', e.message);
    console.error('Error stack:', e.stack);
}

try {
    const result = TestComponent.Test.staticOrAll([true]);
    console.log('Result for [true]:', result);
} catch (e) {
    console.error('Error:', e);
    console.error('Error message:', e.message);
    console.error('Error stack:', e.stack);
}
