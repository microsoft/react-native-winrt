const addon = require('bindings')('winrtaddon');

console.log('addon:', addon);
console.log('addon keys:', Object.keys(addon));
console.log('TestComponent:', addon.TestComponent);
if (addon.TestComponent) {
    console.log('TestComponent type:', typeof addon.TestComponent);
    console.log('TestComponent keys:', Object.keys(addon.TestComponent));
    
    // Try to access Test
    console.log('\nTrying to access Test...');
    try {
        const Test = addon.TestComponent.Test;
        console.log('Test:', Test);
        console.log('Test type:', typeof Test);
        if (Test) {
            console.log('Test keys:', Object.keys(Test));
            console.log('Test.staticOr:', Test.staticOr);
        }
    } catch (e) {
        console.error('Error accessing Test:', e.message);
    }
    
    // Try to enumerate all properties
    console.log('\nTrying to enumerate TestComponent properties...');
    try {
        for (const key in addon.TestComponent) {
            console.log(`  ${key}:`, addon.TestComponent[key]);
        }
    } catch (e) {
        console.error('Error enumerating:', e.message);
    }
}
