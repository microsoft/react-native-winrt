// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

import { TurboModuleRegistry } from 'react-native';

var winrtModule = TurboModuleRegistry.get('WinRTTurboModule');

export default winrtModule;
export const TestComponent = winrtModule.TestComponent;
export const Windows = winrtModule.Windows;
