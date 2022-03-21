# Debugging
For the most part, the underlying causes of various issues can be resolved without any actual debugging.
It is suggested that you first follow the instructions in [the usage guide](USAGE.md#Troubleshooting-and-Known-Issues).
If none of these suggestions help solve your issue, you may need to do a bit of debugging to determine what your particular issue might be.
In general, it helps to know what may have caused the issue.
For example, the steps to debug an issue when adding React Native WinRT to a project may differ significantly from an issue when trying to use a new type in a project where React Native WinRT was successfully being used before.

## Debugging Initialization
If you're having trouble adding React Native WinRT to a project, there's likely an issue with loading/initialization of the turbo module.
To diagnose what the likely issue is, place breakpoints on the following symbols and then debug your app.
Whether or not these breakpoints get hit will help you determine what the issue may be.

|Symbol|Description|
|------|-----------|
|`WinRTTurboModule!winrt::WinRTTurboModule::`<br />`implementation::ReactPackageProvider::CreatePackage`|This function is called during App initialization to add the turbo module to the `IReactPackageBuilder`. If this function is not being called, first check whether or not `WinRTTurboModule.dll` is getting loaded. If the Dll is loaded, but the breakpoint is not hit, it's likely the case that there's something wrong with the symbols and the debugger was unable to set the breakpoint. Rebuilding may help solve this problem. If the Dll is not getting loaded, then it may be the case that autolinking was not performed or failed. Check your application's `AutolinkedNativeModules.g.cpp` for the line `packageProviders.Append(winrt::WinRTTurboModule::ReactPackageProvider());`. If this line is not present, then there was likely an issue with autolinking. You can try running `npx react-native autolink-windows`, re-build, and try again. If that fails, or if the line is there, it may help to clean your project, delete all build outputs, delete your `node_modules` folder, remove `react-native-winrt` from your `package.json`, and try again.|
|`WinRTTurboModule!WinRTTurboModule::WinRTTurboModule`|This constructor will run when the turbo module is loaded, i.e. as a side effect of the `import 'react-native-winrt'` statement. If this breakpoint is not hit, then this likely indicates that you are either missing this import, are trying to use WinRT types before the import is run, or the JavaScript engine is running in the browser where turbo modules are not supported. If the issue is the latter, please follow the instructions mentioned in the usage guide to disable the use of the web debugger. If you are confident that none of these issues are your problem, but the above breakpoint is hit, it may also be the case that something else in your JavaScript code is failing before the import.|
|`WinRTTurboModule!WinRTTurboModule::initialize`|This function is also called as a side effect of `import 'react-native-winrt'`, slightly after the turbo module is loaded. If this breakpoint is not hit, but the above function is, that indicates that some unexpected error happened when importing the turbo module. Such issues should be rare, borderline impossible, however if you find yourself in this position, this knowledge will help narrow down the underlying cause. If this breakpoint *is* hit, it may also be worth stepping through to verify that no errors occur during its execution.|

## Debugging Configuration

If you find yourself in a position where all of the above breakpoints are hit and execute without errors, but still find yourself in a position where WinRT types don't appear to be working, the cause may be related to project configuration, rather than initialization.
It may first help to verify that all desired types are projected as you'd expect.
All of the following files will be located relative to `node_modules/react-native-winrt/windows/WinRTTurboModule/Generated Files/rnwinrt/` and a description of what to look for and what to expect.

|File|Description|
|----|-----------|
|`Projections.g.cpp`|The most interesting thing in this file will be the `root_namespace_data` array. This array contains all data for the namespaces that *should* be accessible in the global namespace. E.g. if your app is expecting to use the type `Windows.Globalization.Calendar`, you should see an entry for `&rnwinrt::namespaces::Windows::data` in this array. Similarly, if your app is expecting to use the type `MyCustomComponent::Foo::Bar`, then you should see an entry for `&rnwinrt::namespaces::MyCustomComponent::data`. If such an entry is *not* present, then you may want to double check the `RnWinRTParameters` entry in your `ExperimentalFeatures.props` file to ensure that it contains entries for the expected type(s)/namespace(s).<br />This file also contains the `global_interface_map_data` array, which is an array of all interfaces whose functions/properties/events can be exposed via a WinRT object instance. If you suspect a missing interface, this is a good place to look. For example, if you create an object via `var cal = new Windows.Globalization.Calendar()` and then find `cal.dayOfWeekAsString()` is failing because `dayOfWeekAsString` returns back `undefined`, you'll want to check this array for an entry that corresponds to `&rnwinrt::interfaces::Windows::Globalization::ICalendar::data`.|
|`<namespace>.g.cpp`|Each of these files will contain all data for that particular namespace. E.g. the file `Windows.Globalization.g.cpp` corresponds to the namespace `Windows::Globalization`, etc. The first interesting thing in this file will be the `children` array. This will define what is accessible as a child property of the namespace. E.g. the entry `&rnwinrt::classes::Windows::Globalization::Calendar::data` indicates that the `Calendar` class is accessible from JavaScript via the name `Windows.Globalization.Calendar`. If you are having an issue where referencing a type/namespace by name in JavaScript code is throwing an error, this is the place that will verify whether or not the type is being included in the projection.<br/>This file will also contain all of the data for all enums, structs, classes, and interfaces that are children of the namespace. For example, the `rnwinrt::classes::Windows::Globalization::Calendar::data` object holds all static and activation information for the `Windows::Globalization::Calendar` class, and `rnwinrt::interfaces::Windows::Globalization::ICalendar::data` object holds all information about the properties, events, and functions for the `Windows::Globalization::ICalendar` interface (i.e. objects created by `new Windows.Globalization.Calendar(...)`). Setting breakpoints in these functions will allow you to verify that they are getting called and to debug their execution.|
|`ProjectedValueConverters.g.h`|For the most part, this file won't be very interesting. This file really just declares specializations that tell the projection how to convert structs to and from JavaScript/native.|
|`base.h` and `base.cpp`|If all of the above files can be thought of as the "data" for the projection, then these two files can be thought of as the "runtime" for making the projection work. The contents of these files are static and don't depend on the namespaces/types that are being projected. Therefore, their contents aren't really interesting in terms of project configuration, but can be very useful for debugging issues, which is the topic of the next section.|

## Debugging Runtime Issues
When all else fails and the above steps aren't helpful in tracking down your issue, it may help to be able to debug execution of the native code to see where things are going wrong.
The instructions below should give you good starting points for doing just that.
For the most part, these instructions are meant to help narrow down where problems are occurring.
For example, if the statement `Windows.Globalization.CalendarIdentifiers.chineseLunar` is giving issues, it may help to validate - in the debugger - that each property accessor is executing correctly: accessors for each property get called, no exceptions get thrown, and each accessor returns a valid, non-`undefined` value.
For example, if the accessor for `CalendarIdentifiers` is returning `undefined`, it would be helpful to validate that the data for the `Globalization` object is correct.
If it's not, then your project configuration is likely incorrect.
If an entry for `CalendarIdentifiers` exists in the data, you might want to check the spelling of the property name passed in to the property getter call.
On the other hand, if the accessor for `chineseLunar` is failing, you can easily debug the property getter to see where the failure is originating, which should give you a sense for why.

If you find yourself needing to debug at this level - i.e. you've ruled out configuration/initialization issues - then your issue is likely one of three things.
The first would be a spelling mistake.
I.e. the underlying machinery of RnWinRT is working correctly, but a type, function, etc. name is spelled incorrectly.
For this class of issues, you should relatively easily be able to find where `undefined` is being returned (what happens when no matching name is found) and compare the property name passed in to the list of valid property names for the namespace, class, etc.
The second class of issues would be usage issues with the WinRT type system itself.
I.e. all functions, conversions, etc. work correctly, however the underlying WinRT type(s) are throwing errors due to mis-use.
Stepping through a function's invocation should reveal if this is the issue as the error should very clearly originate at the usage of the WinRT type.
You can also set the debugger to break when `winrt::hresult_error` gets thrown.
The third class of issues is an underlying bug in either the JavaScript engine, the JSI implementation that sits on top of it, or in React Native WinRT itself.
Such issues are hopefully rare, however they do happen, particularly when the project environment is a little more abnormal and custom and hasn't been well/fully tested.
These errors may be a bit more subtle and tricky to track down/diagnose.
These tips should help you narrow down where the error is occurring which will be key in identifying what the underlying cause might be.

### Debugging Namespace Children Accessors
When you access something in a namespace from within JavaScript, this translates to a series of property getter calls.
The React Native WinRT type that corresponds to namespace objects is the `rnwinrt::projected_namespace` struct.
The only interesting function exposed off of this type is the `projected_namespace::get` function, defined in `base.cpp`, which is called to access a child of that namespace.
Each `projected_namespace` object instance holds a pointer to a `static_namespace_data` object, which holds information about that particular namespace's children.
The call to `get` will search through this array of children for the specified name and then create and return a new object if this is the first access for some particular name, or it will return a cached object that was previously created and returned during a prior call.

For example, in the statement `new Windows.Globalization.Calendar()`, the first access that will take place is to read the `Windows` property from the global object.
This is not debuggable since the object that corresponds to the `Windows` namespace was created and attached to the global object during the turbo module's `initialize` call.
The second access is to read the `Globalization` property off of the `Windows` namespace object, which *is* debuggable through a breakpoint on the `projected_namespace::get` function mentioned above.
This call will either create and return a new `rnwinrt::projected_namespace` object instance for the `Globalization` namespace, or it will return a pre-existing one that it had previously created and returned on an earlier call.
The third access will be to read the `Calendar` property off of the `Globalization` namespace object.
This is still debuggable through the same `projected_namespace::get` function, even though the `Calendar` property represents a class type and not a namespace.
The only difference is that the underlying type returned from this call will be a `function` object that represents the `Calendar` activatable class.

### Debugging Static Functions/Properties/Events
The debugging experience for static functions, properties, and events will vary slightly, depending on whether or not the type is activatable (i.e. it has a constructor).
This is due to the way that JavaScript behaves (and to an extent, some limitations with JSI) where such types need to be `function`s.
Types that are not activatable can be `object`s.
This difference is important when it comes to how properties on each are accessed.
Non-activatable types are the simplest since they can just be `object`s, and therefore behave very similarly to namespaces.
The underlying type for non-activatable types is the `rnwinrt::projected_statics_class` struct, and just like with namespaces, each property access goes through the `projected_statics_class::get` function (or `projected_statics_class::set` in the case of property setters).
I.e. a breakpoint on this function can verify that (1) you make it that far, and (2) that the property is found (i.e. it does not return `undefined`).
Similar to namespaces, the `projected_statics_class` struct holds a single pointer to a `static_class_data` object, which it uses to enumerate valid property names.
Activatable types, on the other hand, are not quite as nice.
Because JSI does not allow us to create an "object-like function" in the same way that it lets us create objects, we instead have to create a JavaScript `function` and then attach properties to it.
Therefore, there is no `projected_*` type and no `get` function that we can easily debug.
The closest alternative would be to debug execution of the `rnwinrt::static_activatable_class_data::create` function when the class is first accessed.
This creates the `function` and attaches the properties to it, so you should be able to verify that the property you expect is present.

Debugging the actual function invocation, property get/set call, or event add/remove call is a little simpler and will be more consistent between the two "types" of classes.
The definitions of these functions will be found inside of the `<namespace>.g.cpp` files, and each will appear as a lambda.
For example, the definition of the property getter for `Windows.Globalization.CalendarIdentifiers.chineseLunar` will be located inside of the `Windows.Globalization.g.cpp` file.
The property getter itself will appear inside of the `rnwinrt::classes::Windows::Globalization::CalendarIdentifiers` namespace as a part of the `property_data` array.
E.g. its definition will appear like:
```c++
{ "chineseLunar",
    [](jsi::Runtime& runtime) {
        return convert_native_to_value(runtime, winrt::Windows::Globalization::CalendarIdentifiers::ChineseLunar());
    },
    nullptr
},
```
You can see that the property name is present (and therefore easily searchable) as well as the getter (the lambda) and the setter (the `nullptr`, since there is no setter available for this particular property).
Function and event definitions will be very similar, and will be part of the `function_data` and `event_data` arrays respectively in the same `rnwinrt::classes::<namespace>::<class>` namespace.

### Debugging Object Instance Functions/Properties/Events
The implementation of functions, properties, and events for object instances is very similar - borderline identical - to the implementation of static functions/properties/events, however how they are accessed is very different.
The type that represents object instances is the `rnwinrt::projected_object_instance` struct, and just like with namespaces and non-activatable types, property get/set calls go through the `get` and `set` functions on this type.
However, unlike namespaces and classes, there is not a single backing object that describes all functions, etc. that are available from the object.
Instead, object instances are represented by a collection of interfaces that the object supports (stored in the `m_interfaces` member), and each interface is represented by its own backing `rnwinrt::static_interface_data` object.
Therefore, the `projected_object_instance::get` and `projected_object_instance::set` functions must iterate through this array of interfaces when looking for a particular name.

The implementations of each function/property/event are also found in the `<namespace>.g.cpp` file for the namespace that the interface is a part of.
E.g. the definitions for the properties of the `Windows::Globalization::ICalendar` interface are located in the `Windows.Globalization.g.cpp` file in the `property_data` array in the `rnwinrt::interfaces::Windows::Globalization::ICalendar` namespace.
Again, the definitions of these functions will look very similar to those of static functions, etc.
E.g. the definitions for the `month` property on the `ICalendar` interface looks like:
```c++
{ "month",
    [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
        return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Globalization::ICalendar>().Month());
    },
    [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
        thisValue.as<winrt::Windows::Globalization::ICalendar>().Month(convert_value_to_native<int32_t>(runtime, value));
    },
},
```
The main difference here is that there is a "this" object that is the source/target for the property get/set.

### Debugging Constructors
As mentioned before, activatable types are represented as `function` objects in JavaScript.
These `function` objects are created/returned by `rnwinrt::static_activatable_class_data::create`, however the actual target of the function invocations is again defined in a `<namespace>.g.cpp` file.
E.g. the constructor for the `Windows.Globalization.Calendar` class is defined as a function in the `Windows.Globalization.g.cpp` file in the namespace `rnwinrt::classes::Windows::Globalization::Calendar` with the name `constructor_function`.
The implementation of this function looks like:
```c++
static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
{
    if (count == 4)
    {
        auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(runtime, args[0]);
        auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
        auto arg2 = convert_value_to_native<winrt::hstring>(runtime, args[2]);
        auto arg3 = convert_value_to_native<winrt::hstring>(runtime, args[3]);
        return convert_native_to_value(runtime, winrt::Windows::Globalization::Calendar(arg0, arg1, arg2, arg3));
    }
    if (count == 1)
    {
        auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(runtime, args[0]);
        return convert_native_to_value(runtime, winrt::Windows::Globalization::Calendar(arg0));
    }
    if (count == 3)
    {
        auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(runtime, args[0]);
        auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
        auto arg2 = convert_value_to_native<winrt::hstring>(runtime, args[2]);
        return convert_native_to_value(runtime, winrt::Windows::Globalization::Calendar(arg0, arg1, arg2));
    }
    if (count == 0)
    {
        return convert_native_to_value(runtime, winrt::Windows::Globalization::Calendar());
    }
    throw_no_constructor(runtime, "Windows.Globalization"sv, "Calendar"sv, count);
}
```

### Debugging Wrap-up
Hopefully this information is enough to help you narrow down where your issue is originating from as that information will be key in determining your next steps.
If you suspect that the issue you are seeing is an issue with RnWinRT, please file an issue on GitHub with as much information as you can.
