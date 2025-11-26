
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

function test_RaiseObjectEvent()
{
    const testObject = new TestComponent.TestObject(4444);

    const handler = function(sender, arg)
    {
        console.log("  !!! In event handler !!!");    
        console.log("  Sender: ", sender);
        console.log("  Arg: ", arg);
        console.log("  Value: ", arg.value);
    }

    console.log("Add event handler");
    TestComponent.StaticOnlyTest.addEventListener('objecteventhandler', handler)

    console.log("Raise event");
    TestComponent.StaticOnlyTest.raiseObjectEvent(testObject);
}


try
{
    
    //test_StaticNumericArrayProperty();
    test_RaiseObjectEvent();

    console.log('---');
    console.log("Test PASSED.")
}
catch (e)
{
    console.log('---');
    console.log("Test FAILED. Exception: ", e.message)
}
