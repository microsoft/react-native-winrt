
import * as TurboModuleRegistry from 'react-native/Libraries/TurboModule/TurboModuleRegistry';

module = TurboModuleRegistry.get('WinRTTurboModule');
if (module) {
    module.initialize();
} else {
    class Test {
        _pass = 0;
        _total = 1;

        percentage() {
            return this._pass / this._total * 100;
        }

        simple() {
            ++this._pass;
        }

        paramN(val) {
            return { returnValue: val, b: val };
        }

        paramNWithRef(val) {
            return { returnValue: val, c: val };
        }

        param1(val) { return this.paramN(val); }
        param2(val) { return this.paramN(val); }
        param3(val) { return this.paramN(val); }
        param4(val) { return this.paramN(val); }
        param5(val) { return this.paramN(val); }
        param6(val) { return this.paramN(val); }
        param7(val) { return this.paramN(val); }
        param8(val) { return this.paramN(val); }
        param9(val) { return this.paramN(val); }
        param10(val) { return this.paramN(val); }
        param11(val) { return this.paramN(val); }
        param12(val) { return this.paramN(val); }
        param13(val, valRef) { return this.paramNWithRef(val); }
        param14(val, valRef) { return this.paramNWithRef(val); }
        param15(val, valRef) { return this.paramNWithRef(val); }

        addEventListener(name, fn) {}
        removeEventListener(name, fn) {}

        event1Call(val) {}
        event2Call(val) {}
    }

    global.TestComponent = {
        TestRunner: {
            makeTests: () => {
                return new Test();
            },
        },
    };
}

export default module;
