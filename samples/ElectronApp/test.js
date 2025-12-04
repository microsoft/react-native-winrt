"use strict";

console.log("Starting...");

const addon = require('./winrtAddon/build/Debug/winrtAddon');
const winrt = addon;
const assert = require('assert');


var package_version ={
    "Major": 1,
    "Minor": 0,
    "Build": 0,
    "Revision": 0
}

if (true)
{
    console.log('MddBootstrapInitialize');
    var bootstrapResult = addon.MddBootstrapInitialize(0x00010008, "", package_version);
    console.log('MddBootstrapInitialize done');

    // print out the result
    console.log('MddBootstrapInitialize result:', bootstrapResult);
}

addon.Microsoft;


var displayArea = addon.Microsoft.UI.Windowing.DisplayArea.getFromPoint({ x: 0, y: 0 }, 0);
console.log('DisplayArea:', displayArea);
console.log('DisplayArea type:', typeof displayArea);
console.log('  isPrimary', displayArea.isPrimary);
console.log('  outerBounds', displayArea.outerBounds);
console.log('  workArea', displayArea.workArea);
console.log('  displayId', displayArea.displayId.value);

var multipleAreas = addon.Microsoft.UI.Windowing.DisplayArea.findAll();
console.log('Multiple DisplayAreas:', multipleAreas);
console.log('Multiple DisplayAreas type:', typeof multipleAreas);
console.log('  Multiple DisplayAreas length:', multipleAreas.length);

for (var i = 0; i < multipleAreas.length; i++) {
    console.log('  Multiple DisplayAreas[' + i + '].isPrimary', multipleAreas[i].isPrimary);
    console.log('  Multiple DisplayAreas[' + i + '].outerBounds', multipleAreas[i].outerBounds);
    console.log('  Multiple DisplayAreas[' + i + '].workArea', multipleAreas[i].workArea);
    console.log('  Multiple DisplayAreas[' + i + '].displayId', multipleAreas[i].displayId.value);
}


var releaseInfo = addon.Microsoft.Windows.ApplicationModel.WindowsAppRuntime.ReleaseInfo;
console.log('Got result');
console.log('releaseInfo.major:', releaseInfo.major);
console.log('releaseInfo.minor:', releaseInfo.minor);
console.log('releaseInfo.patch:', releaseInfo.patch);
console.log('releaseInfo.versionTag:', releaseInfo.versionTag);
console.log('releaseInfo.asString:', releaseInfo.asString);

var runtimeInfo = addon.Microsoft.Windows.ApplicationModel.WindowsAppRuntime.RuntimeInfo;
console.log('runtimeInfo:', runtimeInfo);
try
{
    console.log('runtimeInfo.version:', runtimeInfo.version);
}
catch (e)
{
    console.log('runtimeInfo.version:', e.message);
}

console.log('runtimeInfo.asString:', runtimeInfo.asString);
console.log('runtimeInfo.type:', runtimeInfo.type);

// Print out all the properties of the object


console.log('ReleaseInfo:', releaseInfo);
for (var prop in releaseInfo) {
    try {
        var value = releaseInfo[prop];
        // Try to convert to string, but handle objects that can't be converted
        if (typeof value === 'object' && value !== null) {
            console.log(prop + " = [object]");
        } else {
            console.log(prop + " = " + value);
        }
    } catch (e) {
        console.log(prop + " = [error: " + e.message + "]");
    }
}


var findAllFunc = addon.Microsoft.UI.Windowing.DisplayArea.findAll;
console.log('findAllFunc:', findAllFunc);
console.log('findAllFunc type:', typeof findAllFunc);

var findAllResult = findAllFunc();
console.log('findAllResult:', findAllResult);
console.log('findAllResult type:', typeof findAllResult);
console.log('findAllResult length:', findAllResult.length);

// loop through the results
for (var i = 0; i < findAllResult.length; i++) {
    console.log('DisplayArea:', findAllResult[i]);

    // dump all properties
    for (var prop in findAllResult[i]) {
        console.log(prop + " = " + findAllResult[i][prop]);
    }
}

// Test DispatcherQueue - WinRT Proxy Wrapper Demo
console.log('\n=== WinRT Proxy Wrapper Demo ===');
var controller = addon.Microsoft.UI.Dispatching.DispatcherQueueController.createOnCurrentThread();
console.log('DispatcherQueueController:', controller);
console.log('Controller type:', typeof controller);
//console.log('Controller constructor:', controller.constructor.name);

// Test proxy property access
var dispatcher = controller.dispatcherQueue;
console.log('\nDispatcherQueue:', dispatcher);
console.log('Queue type:', typeof dispatcher);
//console.log('Queue constructor:', dispatcher.constructor.name);

// Test DispatcherQueue proxy properties
if (dispatcher) {
    console.log('\n--- Testing DispatcherQueue Proxy Properties ---');
    try {
        console.log('hasThreadAccess:', dispatcher.hasThreadAccess);
        console.log('hasThreadAccess type:', typeof dispatcher.hasThreadAccess);
    } catch (e) {
        console.log('Error accessing hasThreadAccess:', e.message);
    }
    
    // Test direct method calls on the wrapper (if available)
    try {
        if (typeof dispatcher.getTypeName === 'function') {
            console.log('Type name from wrapper:', dispatcher.getTypeName());
        }
    } catch (e) {
        console.log('getTypeName not available or error:', e.message);
    }
}

// Test controller wrapper methods
if (controller) {
    console.log('\n--- Testing DispatcherQueueController Wrapper ---');
    try {
        if (typeof controller.getTypeName === 'function') {
            console.log('Controller type name:', controller.getTypeName());
        }
    } catch (e) {
        console.log('getTypeName not available on controller:', e.message);
    }
}



var callbackFn = function (a,b) {
    console.log('AAAAAAAAAAA Callback function called');
};



function testDq()
{
    console.log('\n--- Testing DispatcherQueue ---');

    // Why is this a little "c"?
    controller.dispatcherQueue.addEventListener('shutdowncompleted', callbackFn);

    console.log('Call shutdownQueue...');
    controller.shutdownQueue();
    console.log('Shutdown queue called');

}

//testDq();

function testAppWindow()
{
    console.log('\n--- Testing AppWindow ---');

    try {

        var appWindow = addon.Microsoft.UI.Windowing.AppWindow.create();
        console.log('AppWindow created:', appWindow);
        
        appWindow.title = 'AppWindow Test';
        console.log('Title set');

        appWindow.addEventListener('changed', function (s,e) {
            console.log('AppWindow changed event');
        });
        console.log('Changed event listener added');

        appWindow.addEventListener('closing', function(s,e) {
            console.log('AppWindow closing event raised!');
            console.log('  sender:', s);
            console.log('  args:', e);
            dispatcher.enqueueEventLoopExit();
        });
        console.log('Closing event listener added');

        appWindow.moveAndResize({x: 100, y: 100, width: 600, height: 200});

        appWindow.show();
        console.log('Window shown');

        console.log('Running event loop...');
        dispatcher.runEventLoop();
        console.log('Event loop exited');
    } catch (e) {
        console.error('Error in AppWindow test:', e.message);
        console.error(e.stack);
    }
};

testAppWindow();


function testLanguageModelOptions()
{    
    console.log('\n--- Testing LanguageModelOptions ---');

    var options = new winrt.Microsoft.Windows.AI.Text.LanguageModelOptions();

    console.log('Configuring options...');
    options.Temperature = 0.5;
    options.TopP = 0.9;
    options.TopK = 40;

    console.log('options: ', options.Temperature, options.TopP, options.TopK);

    var imageContentFilterSeverity = new winrt.Microsoft.Windows.AI.ContentSafety.ImageContentFilterSeverity();
    imageContentFilterSeverity.AdultContentLevel = winrt.Microsoft.Windows.AI.ContentSafety.SeverityLevel.minimum;

    console.log('imageContentFilterSeverity: ', imageContentFilterSeverity);

    var contentFilterOptions = new winrt.Microsoft.Windows.AI.ContentSafety.ContentFilterOptions();
    contentFilterOptions.LanguageModelOptions = options;
    contentFilterOptions.ImageContentFilterSeverity = imageContentFilterSeverity;
    options.ContentFilterOptions = contentFilterOptions;

    console.log('contentFilterOptions: ', contentFilterOptions);

    console.log('got this: ', options.ContentFilterOptions.ImageContentFilterSeverity.AdultContentLevel);
}
testLanguageModelOptions();

console.log('\nAll tests passed');
