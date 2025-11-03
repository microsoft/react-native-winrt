const addon = require('bindings')('winrtaddon');

console.log(addon.hello());

// TODO: Basic things aren't working.
// Let's get these working:


const tc = addon.TestComponent;
// This should be a valid object:
if (tc === undefined) {
    console.log("ERROR: tc is undefined");
    process.exit(1);
}

const answer = tc.StaticOnlyTest.copyString("Hello")


console.log("All tests passed.");
process.exit(0);