
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

        param1(val) { return this.paramN(val); }
        param2(val) { return this.paramN(val); }
        param3(val) { return this.paramN(val); }
        param4(val) { return this.paramN(val); }
        param5(val) { return this.paramN(val); }
        param6(val) { return this.paramN(val); }
        param7(val) { return this.paramN(val); }
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
