/**
 * @format
 */

import {
    TestScenario,
    assert,
} from './TestCommon'

export function makeAsyncTestScenarios(pThis) {
    return [
        new TestScenario('Test::PauseAsync', runAsyncActionTest.bind(pThis)),
        new TestScenario('Test::CountToNumberAsync', runAsyncActionWithProgressTest.bind(pThis)),
        new TestScenario('Test::AddAsync', runAsyncOperationTest.bind(pThis)),
        new TestScenario('Test::CountDoubleAsync', runAsyncOperationWithProgressTest.bind(pThis)),
        new TestScenario('Test::ThrowAsyncException', runAsyncActionWithException.bind(pThis)),
        new TestScenario('Async await', runAsyncAwaitTest.bind(pThis)),
    ];
}

function runAsyncActionTest(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        Promise.all([
            TestComponent.Test.pauseAsync(100),
            TestComponent.Test.pauseAsync(100)
                .then(() => 42),
            TestComponent.Test.pauseAsync(100)
                .catch(err => { throw err; })
                .then(() => {}),
            TestComponent.Test.pauseAsync(100)
                .then(() => { throw 42; })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.pauseAsync(100)
                .finally(() => { throw 42; })
                .then(() => { throw new Error('Resolve handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.pauseAsync(100)
                .then(() => { throw 42; }, () => { throw new Error('Reject handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
        ]).then(values => {
            assert.equal(42, values[1]);

            // .done() does not return a Promise, so save this for last
            TestComponent.Test.pauseAsync(100).done(resolve, reject);
        }).catch(reject);
    });
}

function runAsyncActionWithProgressTest(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        Promise.all([
            TestComponent.Test.countToNumberAsync(10),
            TestComponent.Test.countToNumberAsync(10)
                .then(() => 42),
            TestComponent.Test.countToNumberAsync(10)
                .catch(err => { throw err; })
                .then(() => {}),
            TestComponent.Test.countToNumberAsync(10)
                .then(() => { throw 42; })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.countToNumberAsync(10)
                .finally(() => { throw 42; })
                .then(() => { throw new Error('Resolve handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.countToNumberAsync(10)
                .then(() => { throw 42; }, () => { throw new Error('Reject handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
        ]).then(values => {
            assert.equal(42, values[1]);

            var lastProgress = -1;
            var pass = true;
            TestComponent.Test.countToNumberAsync(10).done(
                () => {
                    try {
                        assert.isTrue(pass);
                        assert.equal(10, lastProgress); // In theory could fail, but we should have had enough time
                        resolve();
                    } catch (err) {
                        reject(err);
                    }
                }, reject, val => {
                    // NOTE: We don't get old progress values, so we might miss a couple at the beginning
                    if (lastProgress != -1) {
                        if (val != (lastProgress + 1)) pass = false;
                    }
                    lastProgress = val;
                });
        }).catch(reject);
    });
}

function runAsyncOperationTest(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        Promise.all([
            TestComponent.Test.addAsync(34, 8)
                .then(result => {
                    assert.equal(42, result);
                    return 42;
                }),
            TestComponent.Test.addAsync(34, 8)
                .catch(err => { throw err; })
                .then(result => assert.equal(42, result)),
            TestComponent.Test.addAsync(34, 8)
                .then(() => { throw 42; })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.addAsync(34, 8)
                .finally(() => { throw 42; })
                .then(() => { throw new Error('Resolve handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.addAsync(34, 8)
                .then(() => { throw 42; }, () => { throw new Error('Reject handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.immediateReturnAsync(42)
                .then(val => assert.equal(42, val)),
        ]).then(values => {
            assert.equal(42, values[0]);

            TestComponent.Test.addAsync(34, 8).done(
                result => {
                    try {
                        assert.equal(42, result);
                        resolve();
                    } catch (err) {
                        reject(err);
                    }
                }, reject);
        }).catch(reject);
    });
}

function runAsyncOperationWithProgressTest(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        Promise.all([
            TestComponent.Test.countDoubleAsync(5)
                .then(result => {
                    assert.equal(10, result);
                    return 42;
                }),
            TestComponent.Test.countDoubleAsync(5)
                .catch(err => { throw err; })
                .then(result => assert.equal(10, result)),
            TestComponent.Test.countDoubleAsync(5)
                .then(() => { throw 42; })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.countDoubleAsync(5)
                .finally(() => { throw 42; })
                .then(() => { throw new Error('Resolve handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.countDoubleAsync(5)
                .then(() => { throw 42; }, () => { throw new Error('Reject handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
        ]).then(values => {
            assert.equal(42, values[0]);

            var lastProgress = -1;
            var pass = true;
            TestComponent.Test.countDoubleAsync(5).done(
                result => {
                    try {
                        assert.equal(10, result);
                        assert.isTrue(pass);
                        assert.equal(10, lastProgress); // In theory could fail, but we should have had enough time
                        resolve();
                    } catch (err) {
                        reject(err);
                    }
                }, reject, val => {
                    // NOTE: We don't get old progress values, so we might miss a couple at the beginning
                    if (lastProgress != -1) {
                        if (val != (lastProgress + 1)) pass = false;
                    }
                    lastProgress = val;
                });
        }).catch(reject);
    });
}

function runAsyncActionWithException(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        Promise.all([
            TestComponent.Test.throwAsyncException()
                .then(() => { throw new Error('Resolve handler ran when it should not have'); }) // This will cause assert failures below
                .catch(err => {
                    assert.equal(-2147024809, err.number);
                    assert.equal("test", err.message);
                    return 42;
                }),
            TestComponent.Test.throwAsyncException()
                .catch (err => {
                    assert.equal(-2147024809, err.number);
                    assert.equal("test", err.message);
                    return true;
                })
                .then(wasCaught => assert.isTrue(wasCaught)),
            TestComponent.Test.throwAsyncException()
                .catch(() => { throw 42; })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.throwAsyncException()
                .then(() => { throw new Error('Resolve handler ran when it should not have'); }, () => { throw 42; })
                .catch(val => assert.equal(42, val)),
            TestComponent.Test.throwAsyncException()
                .finally(() => 42) // Should run, but do nothing
                .then(() => { throw new Error('Resolve handler ran when it should not have'); })
                .catch(err => {
                    assert.equal(-2147024809, err.number);
                    assert.equal("test", err.message);
                }),
            TestComponent.Test.throwAsyncException()
                .finally(() => { throw 42; }) // Throwing, on the other hand, should overwrite the failure
                .then(() => { throw new Error('Resolve handler ran when it should not have'); })
                .catch(val => assert.equal(42, val)),
        ]).then(values => {
            assert.equal(42, values[0]);

            TestComponent.Test.throwAsyncException().done(
                () => reject(new Error('Expected an exception')),
                err => {
                    try {
                        assert.equal(-2147024809, err.number);
                        assert.equal("test", err.message);
                        resolve();
                    } catch (e) {
                        reject(e);
                    }
                });
        }).catch(reject);
    });
}

function runAsyncAwaitTest(scenario) {
    this.runAsync(scenario, async (resolve, reject) => {
        try
        {
            await TestComponent.Test.pauseAsync(100);
            await TestComponent.Test.countToNumberAsync(10);
            var result = await TestComponent.Test.addAsync(34, 8);
            assert.equal(42, result);
            result = await TestComponent.Test.countDoubleAsync(5);
            assert.equal(10, result);
            assert.equal(42, await TestComponent.Test.immediateReturnAsync(42));

            try
            {
                await TestComponent.Test.throwAsyncException();
                throw new Error('Expected an exception');
            } catch (err) {
                assert.equal(-2147024809, err.number);
                assert.equal("test", err.message);
            }

            resolve();
        } catch (err) {
            reject(err);
        }
    });
}
