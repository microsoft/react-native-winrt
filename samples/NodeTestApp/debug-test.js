const addon = require('./build/Release/winrtaddon.node');

console.log('Addon keys:', Object.keys(addon));
console.log('TestComponent keys:', Object.keys(addon.TestComponent));
console.log('TestComponent type:', typeof addon.TestComponent);
console.log('TestComponent._hostObject_:', addon.TestComponent._hostObject_);

console.log('\nTrying to access Test:');
console.log('addon.TestComponent.Test:', addon.TestComponent.Test);
console.log('Type of Test:', typeof addon.TestComponent.Test);

if (addon.TestComponent.Test) {
    console.log('\nTest properties:', Object.keys(addon.TestComponent.Test));
    console.log('Test.staticOr:', addon.TestComponent.Test.staticOr);
}

console.log('\nDirect property descriptors:');
const descriptor = Object.getOwnPropertyDescriptor(addon.TestComponent, 'Test');
console.log('Test descriptor:', descriptor);
