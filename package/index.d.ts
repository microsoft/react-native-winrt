// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

//tslint:disable

export * from './windows/WinRTTurboModule/Generated Files/types/WinRTModule'

// NOTE: Calling this function adds all types to the global namespace, so something like this would work:
//
//  import winrt from 'react-native-winrt';
//  winrt.initialize();
//  ...
//  var uri = new Windows.Foundation.Uri("...");
//
// HOWEVER this would break intellisense because there's no way to tell TypeScript about these side effects. Preservation
// of this function is mostly for back-compat and isn't really useful when generating code in "module mode"
export function initialize(): void;
