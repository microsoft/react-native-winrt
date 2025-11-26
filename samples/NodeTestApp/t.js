
import { createRequire } from 'module';
const require = createRequire(import.meta.url);
const addon = require('bindings')('winrtaddon');

const TestComponent = addon.TestComponent;
const Test = TestComponent.Test;

function test_StaticNumericArrayProperty()
{
    console.log("Test StaticNumericArrayProperty...");
    
    console.log("Call setter with [101, 102, 103]");
    Test.staticNumericArrayProperty = [101, 102, 103];

    console.log("Read property value");
    var propValue = Test.staticNumericArrayProperty;

    console.log("StaticNumericArrayProperty value: ", propValue);
}


try
{
    
    test_StaticNumericArrayProperty();

    console.log('---');
    console.log("Test PASSED.")
}
catch (e)
{
    console.log('---');
    console.log("Test FAILED. Exception: ", e.message)
}
