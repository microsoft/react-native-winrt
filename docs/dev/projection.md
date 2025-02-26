
# React Native WinRT Projection
This is intended to be very rough documentation on the structure and layout of the React Native WinRT projection.
The specifics of many features can be found embedded in the code as comments.

## Generated Files

* [`base.h`](#baseh) - Declarations and definitions of all functions and types that the Turbo Module and projected headers depend on. This also includes specializations for all types that cannot easily be projected by `rnwinrt.exe` (such as fundamental types, pinterfaces, async types, etc.).
* [`base.cpp`](#basecpp) - Non-template function definitions declared in `base.h`.
* [`ProjectedValueConverters.g.h`](#projectedvalueconvertersgh) - Template specializations of `rnwinrt::projected_value_traits<T>` for all projected structs and delegates.
* [`Projections.g.cpp`](#projectionsgcpp) - Definitions of [root_namespaces](#namespace-tree) and [global_interface_map](#interface-table).
* [`Projected.Namespace.h`](#projectednamespaceh) - Declarations of types/data in that namespace.
* [`Projected.Namespace.cpp`](#projectednamespacecpp) - Definitions of types/data in that namespace.

### base.h
This file contains all class, function, and variable declarations/definitions that are essential for making the projection work.
This includes, but is not limited to:
* Helper types and functions (`span`, `sso_vector`, conversion functions, etc.).
* Essential JSI interop types & functions (threading, object cache, etc.).
* Definitions of static & dynamic projected data types (`static_*_data` and `projected_*`).
* Declarations & definitions of core value converstion types & functions (`projected_value_traits`, `convert_value_to_native`, `convert_native_to_value`, etc.).
* Specializations of `projected_value_traits` for "essential" types (numeric types, `bool`, `hstring`, `guid`, `DateTime`, `IReference`, pinterface types, etc.).
* Interop types (Async/promise, array/vector, etc.).
* Declarations of global data.
* Definitions of static data for pinterface types (`IIterable`, `IVector`, etc.).

Basically, anything that is not specific to the input metadata is either defined or declared in this file.

### base.cpp
As mentioned above, this file just contains the definitions of functions declared in `base.h`.
This is done to help improve compile times so that these function definitions don't need to be parsed for every translation unit.

### ProjectedValueConverters.g.h
This file contains template specializations of `projected_value_traits` for all struct and delegate types in the projected namespaces.
The definitions of each `as_value`/`as_native` function is written to the corresponding `Namespace.g.cpp` file for faster compilation.
The purpose of this standalone file is so that all translation units can properly convert to & from native & JavaScript types; not just the cpp file that contains the definition.

### Projections.g.cpp
This file contains the definitions of all [global data](#global-data) ([root_namespaces](#namespace-tree) and [global_interface_map](#interface-table)).
This data must be in a centralized location since it contains arrays of data from all projected namespaces.

### Projected.Namespace.h
This file contains declarations of all data defined in the corresponding cpp file.
This is partially for convenience so that namespace data can reference these variables without them needing to be defined first, and partially out of necessity so that the `global_interface_map` can reference the interface definitions.

### Projected.Namespace.cpp
This file contains all definitions for the projected data for that given namespace.
This includes classes, enums, interfaces, converters for structs and delegates, as well as the namespace itself.
Because namespaces need to be accessible through successive property getters - that is, `Bar` needs to be accessible as a property off of `Foo` if its fully qualified namespace is `Foo.Bar` - some namespaces may _only_ have namespace data.
This effectively means that if some `Foo.Bar.Baz.g.cpp` file gets generated, then `Foo.Bar.g.cpp`/`Foo.Bar.g.h` and `Foo.g.cpp`/`Foo.g.h` will all be generated as well.

## Projected Types
From the WinMD, there are 6 "types" of data we need to project: namespaces, classes, interfaces, enums, structs, and delegates.
Each of these has their own underlying representation in both the Turbo Module binary as well as at runtime.

### Namespaces
For namespaces, static data is compiled into the Turbo Module binary as `static_namespace_data` objects.
When a namespace is accessed by JavaScript code, a `projected_namespace` object gets created "on demand," with the exception of root namespace(s), which get injected into the global JavaScript namespace on Turbo Module initialization.
The definitions of these two types are as follows:

```c++
namespace rnwinrt
{
    struct static_namespace_data final : static_projection_data
    {
        constexpr static_namespace_data(std::string_view name, span<const static_projection_data* const> children);

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

        span<const static_projection_data* const> children;
    };

    struct projected_namespace final : public jsi::HostObject
    {
        projected_namespace(const static_namespace_data* data);

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        const static_namespace_data* m_data;
        std::vector<jsi::Value> m_children;
    };
}
```

The `projected_namespace::get` function first finds the index of the child with the specified name, calling its `create` function and caching the result if the corresponding entry in `m_children` is undefined.

An example of what generated namespace code looks like in the projection is as follows:

```c++
namespace rnwinrt::namespaces::Windows
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::namespaces::Windows::Foundation::data,
        &rnwinrt::namespaces::Windows::Web::data,
    };

    constexpr const static_namespace_data data{ "Windows"sv, children };
}

namespace rnwinrt::namespaces::Windows::Foundation
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::classes::Windows::Foundation::Deferral::data,
        &rnwinrt::classes::Windows::Foundation::GuidHelper::data,
        &rnwinrt::classes::Windows::Foundation::MemoryBuffer::data,
        &rnwinrt::classes::Windows::Foundation::PropertyValue::data,
        &rnwinrt::classes::Windows::Foundation::Uri::data,
        &rnwinrt::classes::Windows::Foundation::WwwFormUrlDecoder::data,
        &rnwinrt::classes::Windows::Foundation::WwwFormUrlDecoderEntry::data,
        &rnwinrt::enums::Windows::Foundation::AsyncStatus::data,
        &rnwinrt::enums::Windows::Foundation::PropertyType::data,
        &rnwinrt::namespaces::Windows::Foundation::Collections::data,
        &rnwinrt::namespaces::Windows::Foundation::Diagnostics::data,
        &rnwinrt::namespaces::Windows::Foundation::Metadata::data,
        &rnwinrt::namespaces::Windows::Foundation::Numerics::data,
    };

    constexpr const static_namespace_data data{ "Foundation"sv, children };
}
```

As you can see from the above example, both classes and enums, in addition to other namespaces, are accessible from namespaces.

### Classes
Classes are split into two categories: activatable and non-activatable classes.
An "activatable class" is a class that has at least one constructor.
I.e. you can create an object of that type via `new Type()`.
The static data for non-activatable classes is compiled into the Turbo Module binary as `static_class_data` objects, and the static data for activatable classes is compiled into the binary as `static_activatable_class_data` objects.
When a non-activatable class is accessed by JavaScript code, a `projected_statics_class` object is created "on demand."
When an activatable class is accessed by JavaScript code, a new JavaScript object is created and functions (including the constructor) as well as properties and events are attached to the object.
The reason for this differentiation is because JSI doesn't (currently) have a way for a `jsi::HostObject` to behave like a function, meaning it would not be possible to call the constructor.
The definitions of these types are as follows:

```c++
namespace rnwinrt
{
    struct static_class_data : static_projection_data
    {
        struct property_mapping
        {
            std::string_view name;
            static_get_property_t getter;
            static_set_property_t setter;
        };

        struct event_mapping
        {
            std::string_view name;
            static_add_event_t add;
            static_remove_event_t remove;
        };

        struct function_mapping
        {
            std::string_view name;
            call_function_t function;
        };

        constexpr static_class_data(std::string_view name, span<const property_mapping> properties,
            span<const event_mapping> events, span<const function_mapping> functions);

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

        span<const property_mapping> properties;
        span<const event_mapping> events;
        span<const function_mapping> functions;
    };

    struct static_activatable_class_data final : static_class_data
    {
        constexpr static_activatable_class_data(std::string_view ns, std::string_view name, call_function_t constructor,
            span<const property_mapping> properties, span<const event_mapping> events,
            span<const function_mapping> functions);

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

        std::string_view full_namespace;
        call_function_t constructor;
    };

    struct projected_statics_class final : public jsi::HostObject
    {
        projected_statics_class(const static_class_data* data);

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        jsi::Value add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
        jsi::Value remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);

        const static_class_data* m_data;
        std::unordered_map<std::string_view, jsi::Value> m_functions;
        event_registration_array m_events;
    };
}
```

A small excerpt from the tests showing this data is as follows:

```c++
namespace rnwinrt::classes::TestComponent::Test
{
    static constexpr const static_class_data::property_mapping property_data[] = {
        { "staticBoolProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticBoolProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticBoolProperty(convert_value_to_native<bool>(runtime, value));
            },
        },
        // ...
    };

    static constexpr const static_class_data::event_mapping event_data[] = {
        { "staticbooleventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticBoolEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<bool>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticBoolEventHandler(token);
            }
        },
        // ...
    };

    static constexpr const static_class_data::function_mapping function_data[] = {
        { "addAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::AddAsync(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "addAsync"sv, count);
            }
        },
        { "staticArityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::StaticArityOverload();
                    return convert_native_to_value(runtime, result);
                }
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::StaticArityOverload(arg0);
                    return convert_native_to_value(runtime, result);
                }
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticArityOverload(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticArityOverload"sv, count);
            }
        },
        { "staticBoolArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    winrt::com_array<bool> arg1;
                    winrt::com_array<bool> arg2;
                    auto result = winrt::TestComponent::Test::StaticBoolArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticBoolArrayOutParam"sv, count);
            }
        },
        // ...
    };

    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 1)
        {
            auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
            return convert_native_to_value(runtime, winrt::TestComponent::Test(arg0));
        }
        if (count == 2)
        {
            auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
            auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
            return convert_native_to_value(runtime, winrt::TestComponent::Test(arg0, arg1));
        }
        if (count == 0)
        {
            return convert_native_to_value(runtime, winrt::TestComponent::Test());
        }
        throw_no_constructor(runtime, "TestComponent"sv, "Test"sv, count);
    }

    constexpr const static_activatable_class_data data{ "TestComponent"sv, "Test"sv, constructor_function, property_data, event_data, function_data };
}
```

This short example shows quite a few features: how events, properties, and functions differ, how overloads are handled based off function arity (number of parameters), how value conversion takes place, how out parameters are returned in a struct, among other things.

### Interfaces / Objects
Unlike all other projected types, there's not a 1:1 correlation between the static data type and the runtime data type for interfaces and objects.
That is, for all other projected types, there will only ever be at most one `projected_*` object created for any given `static_*_data` object.
Similarly, all other `projected_*` objects will reference exactly one `static_*_data` object.
For interfaces, static data is compiled into the Turbo Module binary as `static_interface_data` objects.
For each unique WinRT object, a single `projected_object_instance` object is created, which holds an _array_ of `static_interface_data` pointers, one for each interface that the object supports.
When a native object is being projected into a JavaScript object, the following steps are taken:
1. The object is queried for its `IInspectable` interface. This is to ensure we get a consistent, identical pointer for every object.
2. This pointer is used to look up a `projected_object_instance` in the global object cache.
3. If such an object instance is found _and_ it is still alive, then that cached JavaScript object is returned.
4. If an instance is not found, or if it has already been destroyed (i.e. it has been garbage collected), a new `projected_object_instance` is created for the WinRT object.
5. This `projected_object_instance` will call the object's `GetIids` method to get the set of interfaces supported by the object, using each interface `GUID` to look up the corresponding `static_interface_data` pointer.
6. A new JavaScript object is created for this `projected_object_instance` and stored in the cache.

The definitions of these two types are as follows:
```c++
namespace rnwinrt
{
    struct static_interface_data
    {
        struct property_mapping
        {
            std::string_view name;
            instance_get_property_t getter;
            instance_set_property_t setter;
        };

        struct event_mapping
        {
            std::string_view name;
            instance_add_event_t add;
            instance_remove_event_t remove;
        };

        struct function_mapping
        {
            std::string_view name;
            instance_call_function_t function;
            unsigned int arity;
            bool is_default_overload;
        };

        constexpr static_interface_data(const winrt::guid& guid, span<const property_mapping> properties,
            span<const event_mapping> events, span<const function_mapping> functions,
            instance_runtime_get_property_t runtimeGetProperty = nullptr,
            instance_runtime_set_property_t runtimeSetProperty = nullptr);

        winrt::guid guid;
        span<const property_mapping> properties;
        span<const event_mapping> events;
        span<const function_mapping> functions;

        instance_runtime_get_property_t runtime_get_property;
        instance_runtime_set_property_t runtime_set_property;
    };

    struct projected_object_instance : public jsi::HostObject
    {
        friend struct projected_function;
        friend struct projected_overloaded_function;

        projected_object_instance(const winrt::Windows::Foundation::IInspectable& instance);

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

        const winrt::Windows::Foundation::IInspectable& instance();

    private:
        jsi::Value add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
        jsi::Value remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);

        winrt::Windows::Foundation::IInspectable m_instance;
        sso_vector<const static_interface_data*> m_interfaces;
        std::unordered_map<std::string_view, jsi::Value> m_functions;
    };
}
```

What's not shown above is how `projected_object_instance::get` behaves.
Since the set of supported interfaces is arbitrary, both in contents and length, the mapping of name to property/function cannot be known until runtime.
This not only makes the `get` function extremely complex; it also affects how data is represented, particularly for functions since full overload information depends on all interfaces that an object implements.
This can be seen above in the definition of `static_interface_data::function_mapping` where the `arity` and `is_default_overload` is encoded in the static data.
This also means that the generated function definitions don't have the same overload resolution logic that class functions have.
Instead, this logic is handled by the `projected_object_instance` type.

A small excerpt from the tests showing this data is as follows:

```c++
namespace rnwinrt::interfaces::TestComponent::ITest
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "boolProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().BoolProperty());
            },
            nullptr
        },
        { "booleanArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().BooleanArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().BooleanArrayProperty(convert_value_to_native<winrt::array_view<const bool>>(runtime, value));
            },
        },
        // ...
    };
    static constexpr const static_interface_data::event_mapping event_data[] = {
        { "booleventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().BoolEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, bool>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().BoolEventHandler(token);
            }
        },
        // ...
    };

    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "assignPropertyValueAsType",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IPropertyValue>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    thisValue.as<winrt::TestComponent::ITest>().AssignPropertyValueAsType(arg0, arg1);
                    return jsi::Value::undefined();
                },
                2, false },
        { "contractOutParamOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractOutParamOverloadV1(arg0, arg1);
                    return make_return_struct(runtime, result, "outParam", arg1);
                },
                1, true },
        { "contractOutParamOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractOutParamOverloadV1(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, true },
        // ...
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::ITest>(), property_data, event_data, function_data };
}
```

Here, you can see how these definitions differ from class definitions a fair amount.
All of the functions accept an additional `IInspectable` instance - the `this` object to invoke the function on - and the projected functions have the parameter count embedded in the data, not the function definitions, and assume the number of arguments in `args` is the same as this value.
Additionally, this shows how `undefined` is returned from a `void`-returning WinRT function.

### Enums
For enums, static data is compiled into the Turbo Module binary as `static_enum_data` objects, with `projected_enum` objects getting created "on demand" when JavaScript code references the enum.
The definitions of these two types are as follows:

```c++
namespace rnwinrt
{
    struct static_enum_data final : static_projection_data
    {
        struct value_mapping
        {
            std::string_view name;
            double value;
            std::string_view value_as_string;
        };

        constexpr static_enum_data(std::string_view name, span<const value_mapping> values);

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

        jsi::Value get_value(jsi::Runtime& runtime, std::string_view valueName) const;

        span<const value_mapping> values;
    };

    struct projected_enum final : public jsi::HostObject
    {
        projected_enum(const static_enum_data* data);

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        const static_enum_data* m_data;
    };
}
```

The data stored for each enum value is: (1) it's name (to map `Enum.name` from JavaScript into a numeric value), (2) its numeric value as a `double` (i.e. `number` in JavaScript), and its value as a string for reverse mapping (e.g. `Enum[5]` from JavaScript).
The `projected_enum::get` function loops through this array, looking at both the name string and value string to try and determine what to return.

An example of what generated enum code looks like in the projection is as follows:

```c++
namespace rnwinrt::enums::TestComponent::TestEnum
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "first"sv, 1, "1"sv },
        { "second"sv, 2, "2"sv },
        { "third"sv, 3, "3"sv },
        { "fourth"sv, 4, "4"sv },
    };

    constexpr const static_enum_data data{ "TestEnum"sv, mappings };
}
```

### Structs
Unlike all previous data types, structs do not have any dedicated types in the projection (no `static_*_data` or `projected_*` type).
This is because structs are projected as JavaScript objects with properties whose names match the WinRT struct's field names.
That said, the projection still needs to know how to convert to/from these types.
This is done by generating specializations of the `projected_value_traits` template for these structs.
For example:

```c++
namespace rnwinrt
{
    jsi::Value projected_value_traits<winrt::Windows::Foundation::Point>::as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::Point& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "x", convert_native_to_value(runtime, value.X));
        result.setProperty(runtime, "y", convert_native_to_value(runtime, value.Y));
        return result;
    }

    winrt::Windows::Foundation::Point projected_value_traits<winrt::Windows::Foundation::Point>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::Windows::Foundation::Point result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "x"); !field.isUndefined())
            result.X = convert_value_to_native<float>(runtime, field);
        if (auto field = obj.getProperty(runtime, "y"); !field.isUndefined())
            result.Y = convert_value_to_native<float>(runtime, field);
        return result;
    }
}
```

### Delegates
Delegates are handled virtually indentically to structs.
Delegates in JavaScript code are just function definitions, and are projected as such.
This is again accomplished with specializations of the `projected_value_traits` template.
For example:

```c++
namespace rnwinrt
{
    jsi::Value projected_value_traits<winrt::Windows::Foundation::DeferralCompletedHandler>::as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::DeferralCompletedHandler& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "DeferralCompletedHandler"), 0,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 0)
                {
                    throw_invalid_delegate_arg_count(runtime, "Windows.Foundation"sv, "DeferralCompletedHandler"sv);
                }

                value();
                return jsi::Value::undefined();
            });
    }

    winrt::Windows::Foundation::DeferralCompletedHandler projected_value_traits<winrt::Windows::Foundation::DeferralCompletedHandler>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)]() {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                fn.call(runtime);
            });
        };
    }
}
```

## Global Data
The projection contains two "categories" of global data: (1) namespaces and types that are accessible by name from JavaScript, and (2) interfaces that are accessible via object instances.

### Namespace Tree
Anything reachable from a namespace - other namespaces, enums, and classes - are all stored in static data in the Turbo Module binary as a single tree.
The root of this tree is the `root_namespaces` array, technically making this a forest and not a tree.
For example, if you project the namespaces `Windows.Foundation` and `Windows.ApplicationModel`, you will have a single entry in the `root_namespaces` array for the `Windows` namespace.
If you project an additional namespace - `Microsoft.Windows.AppNotifications` - then the `root_namespaces` would have two entries: one for `Windows` and another for `Microsoft`.
For example, this array looks like the following for the tests:
```c++
namespace rnwinrt
{
    static constexpr const static_namespace_data* const root_namespace_data[] = {
        &rnwinrt::namespaces::TestComponent::data,
        &rnwinrt::namespaces::Windows::data,
    };

    constexpr const span<const static_namespace_data* const> root_namespaces{ root_namespace_data };
}
```

### Interface Table
Unlike namespaces, interfaces are _not_ reachable from a namespace.
E.g. `Windows.Foundation.IPropertyValue` would give back `undefined` in JavaScript code.
This is because interfaces don't have any information that would be valuable in JavaScript code.
You can't instantiate them, they don't have static functions, and it's not meaningful to try and implement them.
Instead, interfaces are exposed through the functions, properties, and events available on object instances.
Object instances can be acquired by activating an object or as a function return value/output parameter.
The interfaces supported by the object are queried by calling its [`GetIids`](https://learn.microsoft.com/en-us/windows/win32/api/inspectable/nf-inspectable-iinspectable-getiids) method.
This returns back an array of interface `GUIDS`s.
Each `GUID` is then used to look up a `static_interface_data` pointer in the `global_interface_map`.
The `global_interface_map` is an array of all projected interfaces, sorted by `GUID`.
For example, here's a reduced example of what this array looks like for the tests:
```c++

namespace rnwinrt
{
    static constexpr const std::pair<winrt::guid, const static_interface_data*> global_interface_map_data[] = {
        { winrt::guid_of<winrt::Windows::Foundation::IAsyncInfo>(), &rnwinrt::interfaces::Windows::Foundation::IAsyncInfo::data }, // 00000036-0000-0000-c000-000000000046
        { winrt::guid_of<winrt::Windows::Foundation::IMemoryBufferReference>(), &rnwinrt::interfaces::Windows::Foundation::IMemoryBufferReference::data }, // fbc4dd29-245b-11e4-af98-689423260cf8
        { winrt::guid_of<winrt::Windows::Foundation::IMemoryBuffer>(), &rnwinrt::interfaces::Windows::Foundation::IMemoryBuffer::data }, // fbc4dd2a-245b-11e4-af98-689423260cf8
        { winrt::guid_of<winrt::Windows::Foundation::IClosable>(), &rnwinrt::interfaces::Windows::Foundation::IClosable::data }, // 30d5a829-7fa4-4026-83bb-d75bae4ea99e
        // ...
    };

    constexpr const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map(global_interface_map_data);
}
```
