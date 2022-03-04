// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

/**
 * @format
 */

import {AppRegistry} from 'react-native';
import App from './App';
import {name as appName} from './app.json';
import 'react-native-winrt';
AppRegistry.registerComponent(appName, () => App);
