
const addon = require('bindings')('winrtaddon');

const TestComponent = addon.TestComponent;
const t = new TestComponent.Test();

console.log("t", t);

console.log("prop", t.boolProperty);

t.boolProperty = true;

console.log("prop", t.boolProperty);
