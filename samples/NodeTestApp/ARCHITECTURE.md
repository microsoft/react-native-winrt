# Test Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        test.js                               │
│                    (Main Entry Point)                        │
│                                                              │
│  1. Load addon & get TestComponent                          │
│  2. Create TestValues from TestComponent                    │
│  3. Create test context (SyncTestContext)                   │
│  4. Register test suites                                    │
│  5. Run all tests via TestRunner                            │
└──────────────────┬──────────────────────────────────────────┘
                   │
                   ├─────────────────────────────────┐
                   │                                 │
                   ▼                                 ▼
         ┌──────────────────┐            ┌──────────────────┐
         │  TestRunner.js   │            │ TestCommon.js    │
         │                  │            │                  │
         │ • runAll()       │            │ • assert.*       │
         │ • runTestSuite() │            │ • TestScenario   │
         │ • runScenario()  │            │ • TestValues     │
         │ • printSummary() │            │ • GUID utils     │
         └─────────┬────────┘            └──────────────────┘
                   │                              ▲
                   │ executes                     │ uses
                   ▼                              │
         ┌──────────────────┐                    │
         │  Test Suites     │────────────────────┘
         │                  │
         │ ┌──────────────────────────────────┐
         │ │  BasicFunctionTests.js           │
         │ │  • makeBasicFunctionTestScenarios│
         │ │  • 74 test scenarios             │
         │ └──────────────────────────────────┘
         │
         │ ┌──────────────────────────────────┐
         │ │  EnumTests.js                    │
         │ │  • makeEnumTestScenarios         │
         │ │  • 4 test scenarios              │
         │ └──────────────────────────────────┘
         │
         │ ┌──────────────────────────────────┐
         │ │  [Future Test Suites]            │
         │ │  • PropertiesTests.js            │
         │ │  • ArrayTests.js                 │
         │ │  • AsyncTests.js                 │
         │ │  • CollectionsTests.js           │
         │ │  • ...                            │
         │ └──────────────────────────────────┘
         └──────────────────┘
```

## Test Execution Flow

```
1. test.js starts
   │
   ├─► Load WinRT addon with bindings('winrtaddon')
   │
   ├─► Extract TestComponent from addon
   │
   ├─► Generate TestValues using getTestValues(TestComponent)
   │
   ├─► Create SyncTestContext for test execution
   │
   ├─► Build testSuites array:
   │   │
   │   ├─► Call makeEnumTestScenarios(context, TestComponent, TestValues)
   │   │   └─► Returns array of 4 TestScenario objects
   │   │
   │   └─► Call makeBasicFunctionTestScenarios(context, TestComponent, TestValues)
   │       └─► Returns array of 74 TestScenario objects
   │
   ├─► Create TestRunner with testSuites
   │
   └─► runner.runAll()
       │
       ├─► For each test suite:
       │   │
       │   ├─► Print suite header
       │   │
       │   ├─► For each scenario:
       │   │   │
       │   │   ├─► scenario.invoke()
       │   │   │   │
       │   │   │   ├─► Execute test function
       │   │   │   │   └─► Uses TestComponent to call WinRT APIs
       │   │   │   │
       │   │   │   ├─► If assertions pass:
       │   │   │   │   └─► Print '.' and increment passCount
       │   │   │   │
       │   │   │   └─► If assertions fail:
       │   │   │       └─► Print 'F', store failure, increment failCount
       │   │   │
       │   │   └─► Continue to next scenario
       │   │
       │   └─► Print suite summary
       │
       └─► printSummary()
           │
           ├─► Print total stats (pass/fail counts, percentage)
           ├─► List all failures with details
           └─► Exit with code 0 (success) or 1 (failure)
```

## Data Flow

```
┌─────────────┐
│   Addon     │ (C++ Native Module)
│ winrtaddon  │
└──────┬──────┘
       │ provides
       ▼
┌──────────────┐
│TestComponent │ (JavaScript proxy to WinRT types)
│              │
│ • Test       │ (WinRT class with static & instance methods)
│ • TestEnum   │ (WinRT enum)
│ • TestObject │ (WinRT class)
│ • ...        │
└──────┬───────┘
       │ used by
       ▼
┌──────────────┐
│ Test Suites  │
│              │
│ Each test    │──► Calls methods on TestComponent
│ scenario     │──► Gets return values
│              │──► Uses assert.* to verify correctness
└──────────────┘
```

## Component Responsibilities

| Component | Responsibility |
|-----------|---------------|
| **test.js** | Bootstrap, orchestration, main entry point |
| **TestRunner** | Test execution, progress tracking, reporting |
| **TestCommon** | Shared utilities, assertions, test data |
| **Test Suites** | Define scenarios, execute WinRT API calls, verify results |
| **SyncTestContext** | Provide runSync/runAsync helpers for test execution |
| **TestComponent** | WinRT API surface exposed to JavaScript |

## Test Scenario Lifecycle

```
┌─────────────────────────────────────────────────────────┐
│ new TestScenario('Name', () => runTest(...))            │
│                                                         │
│ Creates scenario with:                                  │
│ • name: "Name"                                          │
│ • invoke: () => runTest(...)                            │
│ • result: TestResult.NotRun                             │
│ • failureText: ''                                       │
└────────────────────┬────────────────────────────────────┘
                     │
                     │ TestRunner calls scenario.invoke()
                     ▼
┌─────────────────────────────────────────────────────────┐
│ Test function executes:                                 │
│ pThis.runSync(null, () => {                             │
│     assert.equal(                                       │
│         TestComponent.Test.someMethod(arg),             │
│         expectedValue                                   │
│     );                                                  │
│ });                                                     │
└────────────────────┬────────────────────────────────────┘
                     │
         ┌───────────┴───────────┐
         │                       │
         ▼                       ▼
    ┌─────────┐            ┌─────────┐
    │ Success │            │ Failure │
    │         │            │         │
    │ result  │            │ result  │
    │ = Pass  │            │ = Fail  │
    │         │            │         │
    │         │            │ failure │
    │         │            │ Text =  │
    │         │            │ error   │
    └─────────┘            └─────────┘
```
