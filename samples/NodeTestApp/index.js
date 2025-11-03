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

console.log("tc:", tc);
console.log("tc.StaticOnlyTest:", tc.StaticOnlyTest);
console.log("tc.StaticOnlyTest keys:", Object.keys(tc.StaticOnlyTest));
console.log("tc.StaticOnlyTest.copyString:", tc.StaticOnlyTest.copyString);

const answer = tc.StaticOnlyTest.copyString("Hello")
console.log("tc.StaticOnlyTest.copyString(\"Hello\"):", answer);
if (answer !== "Hello") {
    console.log("ERROR: answer is not \"Hello\"");
    process.exit(1);
}


console.log("All tests passed.");
process.exit(0);