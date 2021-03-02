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
    ];
}

function runAsyncActionTest(scenario) {
    const vector = this.test.createIVector();
    this.runAsync(scenario, () => {
        return new Promise((resolve, reject) => {
            this.test.appendZeroToIVectorAsync(vector).then(() => {
                    assert.equal(0, vector.getAt(0));
                    assert.equal(1, vector.size);  
                    resolve();
                }).catch(reject);
        })
    });
}

function runAsyncActionWithProgressTest(scenario) {
    const vector = this.test.createIVector();
    let progressIteration = 0;
    this.runAsync(scenario, () => {
        return new Promise((resolve, reject) => {
            this.test.fillZeroesToIVectorAsync(vector).done(() => {
                assert.equal(0, vector.getAt(0));
                assert.equal(10, vector.size); 
                resolve();
            }, reject, progress => {
                progressIteration++;
                assert.equal(0.1 * progressIteration, progress);
            });
        });
    });
}

function runAsyncOperationTest(scenario) {
    this.runAsync(scenario, () => {
        return new Promise((resolve, reject) => {
            this.test.createIVectorAsync().then(vector => {
                    assert.equal(0, vector.size);  
                    resolve();
                }).catch(reject);
        })
    });
}

function runAsyncOperationWithProgressTest(scenario) {
    let progressIteration = 0;
    this.runAsync(scenario, () => {
        return new Promise((resolve, reject) => {
            this.test.createIVectorWithZeroesAsync().done(vector => {
                assert.equal(0, vector.getAt(0));
                assert.equal(10, vector.size); 
                resolve();
            }, reject, progress => {
                progressIteration++;
                assert.equal(0.1 * progressIteration, progress);
            });
        });
    });
}