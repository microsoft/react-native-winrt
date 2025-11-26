
import { createRequire } from 'module';
const require = createRequire(import.meta.url);
const addon = require('bindings')('winrtaddon');

const TestComponent = addon.TestComponent;
const Test = TestComponent.Test;


var array = [5, 10, 15];
console.log("Original array: ", array);

try
{
    console.log("Call returnSameNumericVector...");
    var collection = Test.returnSameNumericVector(array);
    console.log("Returned object: ", collection);
    console.log("  collection.length: ", collection.length);
    
    // print the values
    for (let i = 0; i < collection.length; i++)
    {
        console.log(`  collection[${i}]: `, collection.getAt(i));
    }

    console.log('---');
    console.log("Test PASSED.")
}
catch (e)
{
    console.log('---');
    console.log("Test FAILED. Exception: ", e.message)
}



//var numericValuesToAdd = [7, 14, 21];
//console.log("Values to add: ", numericValuesToAdd);

//doArrayAsVectorTest(sameArray, array, numericValuesToAdd);


//console.log("Final array: ", sameArray);