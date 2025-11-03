# Node.js Test Suite for WinRT Projections

This directory contains an adapted test suite from the React Native WinRT tests, modified to run with the Node.js addon.

## Structure

- **test/TestCommon.js** - Common test utilities, assertions, and test data values
- **test/TestRunner.js** - Test runner that executes test scenarios and reports results
- **test/BasicFunctionTests.js** - Tests for basic function calls (static, instance, overloads, out params)
- **test.js** - Main entry point for running tests

## Running Tests

```bash
# Run all tests
npm run test:basic

# Or run the full test with DLL copy
npm test
```

## Test Status

### ✅ Ported
- BasicFunctionTests.js - All basic function tests including:
  - Static functions (Or, Add, Append)
  - Static overloads (Arity, Default, OutParam)
  - Static out parameters
  - Constructors
  - Instance methods
  - Non-static overloads
  - Non-static out parameters
  - Static methods for non-activable classes

### 🔄 To Be Ported
- EnumTests.js
- PropertiesTests.js
- ArrayTests.js
- DelegateAndEventTests.js
- AsyncTests.js
- CollectionsTests.js
- InheritanceTests.js

## Adding More Tests

To port additional test files from `tests/RnWinRTTests`:

1. Copy the test file to `test/` directory
2. Adapt the imports:
   ```javascript
   const { TestScenario, assert, TestValues } = require('./TestCommon');
   ```
3. Modify the test scenario factory to accept `TestComponent` and `TestValues` as parameters:
   ```javascript
   function makeXxxTestScenarios(pThis, TestComponent, TestValues) {
       // ...
   }
   ```
4. Update test functions to pass `TestComponent` explicitly:
   ```javascript
   function runTest(pThis, TestComponent) {
       pThis.runSync(null, () => {
           // test code using TestComponent
       });
   }
   ```
5. Add the new test suite to `test.js`:
   ```javascript
   const { makeXxxTestScenarios } = require('./test/XxxTests');
   
   const testSuites = [
       // ... existing suites
       {
           name: "Xxx Tests",
           scenarios: makeXxxTestScenarios(testContext, TestComponent, TestValues),
       },
   ];
   ```

## Differences from React Native Tests

1. **No React/UI dependencies** - Pure Node.js, no EventEmitter from React Native
2. **Synchronous execution** - Tests run sequentially rather than in parallel
3. **Console output** - Results printed to console instead of UI
4. **Module system** - Uses CommonJS (`require`/`module.exports`) instead of ES6 imports
5. **Test context** - Simplified context without React component state management

## Notes

- TestComponent must be available from the addon for tests to run
- The test DLL must be copied to the current directory before running tests
- Tests use the same assertion library and test values as the React Native tests
