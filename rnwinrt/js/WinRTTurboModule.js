// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

import { TurboModuleRegistry } from 'react-native';

const module = TurboModuleRegistry.get('WinRTTurboModule');
if (module) {
    module.initialize();
}

export default module;
