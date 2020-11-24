/**
 * @format
 */
'use strict';

import { StyleSheet } from "react-native";

export var styles = StyleSheet.create({
    headerBanner: {
      backgroundColor: 'lightgray',
      alignItems: 'center',
      paddingVertical: 10,
    },
    scrollView: {
      flex: 1,
    },
    listEntry: {
        flexDirection: 'row',
        justifyContent: 'center',
        alignItems: 'center',
    },
    rowItem: {
      marginVertical: 5,
      marginHorizontal: 10,
    },
    progressText: {
      color: 'darkgray'
    },
    passText: {
      color: 'green',
    },
    failText: {
      color: 'red',
    },
  });
