import * as TurboModuleRegistry from 'react-native/Libraries/TurboModule/TurboModuleRegistry';
const module = TurboModuleRegistry.get('WinRTTurboModule');
if (module) {
    module.initialize();
}
export default module;