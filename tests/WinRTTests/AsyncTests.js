/**
 * @format
 */

import {
    TestScenario,
    assert,
} from './TestCommon'

export function makeAsyncTestScenarios(pThis) {
    return [
        new TestScenario('Test::AppendZeroToIVectorAsync', runAsyncActionTest.bind(pThis)),
        new TestScenario('Test::FillZeroesToIVectorAsync', runAsyncActionWithProgressTest.bind(pThis)),
        new TestScenario('Test::CreateIVectorAsync', runAsyncOperationTest.bind(pThis)),
        new TestScenario('Test::CreateIVectorWithZeroesAsync', runAsyncOperationWithProgressTest.bind(pThis)),
        new TestScenario('Test::CreateAsyncException', runAsyncActionWithException.bind(pThis)),
    ];
}

function runAsyncActionTest(scenario) {
    const vector = TestComponent.Test.copyNumericsToVector([]);
    this.runAsync(scenario, (resolve, reject) => {
        this.test.appendZeroToIVectorAsync(vector)
            .catch(reject)
            .then(() => {
                assert.equal(0, vector.getAt(0));
                assert.equal(1, vector.size);
                resolve();
            });
    });
}

function runAsyncActionWithProgressTest(scenario) {
    const vector = TestComponent.Test.copyNumericsToVector([]);
    let progressIteration = 0;
    this.runAsync(scenario, (resolve, reject) => {
        this.test.fillZeroesToIVectorAsync(vector).done(() => {
            assert.equal(0, vector.getAt(0));
            assert.equal(10, vector.size);
            assert.equal(10, progressIteration);
            resolve();
        }, reject, progress => {
            progressIteration++;
            assert.equal(0.1 * progressIteration, progress);
        });
    });
}

function runAsyncOperationTest(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        const op = this.test.createIVectorAsync();
        op.catch(reject);
        op.then(vector => {
            assert.equal(0, vector.size);
            resolve();
        });
    });
}

function runAsyncOperationWithProgressTest(scenario) {
    let progressIteration = 0;
    this.runAsync(scenario, (resolve, reject) => {
        this.test.createIVectorWithZeroesAsync().done(vector => {
            assert.equal(0, vector.getAt(0));
            assert.equal(10, vector.size);
            assert.equal(10, progressIteration);
            resolve();
        }, reject, progress => {
            progressIteration++;
            assert.equal(0.1 * progressIteration, progress);
        });
    });
}

// TODO: remove commented out code once the rejected promises are fixed to run catch after finally and not then
function runAsyncActionWithException(scenario) {
    this.runAsync(scenario, (resolve, reject) => {
        isFinallyRun = false;
        this.test.createAsyncException()
            // .finally(() => {
            //     isFinallyRun = true;
            // })
            .catch(e => {
                // if (!isFinallyRun) {
                //     reject('Finally must run before catch');
                // } else
                if (e.number == -2147024809 && e.message == "test") {
                    resolve();
                } else {
                    reject(e);
                }
            }).then(reject);
    });
}