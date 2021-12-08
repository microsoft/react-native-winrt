// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#include "base.h"

#include <Windows.h>

#include <combaseapi.h>
#include <winstring.h>

using namespace rnwinrt;
using namespace std::literals;

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
}

jsi::String rnwinrt::make_string(jsi::Runtime& runtime, std::wstring_view str)
{
    if (str.empty())
    {
        return jsi::String::createFromAscii(runtime, "");
    }

    auto bytes = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.data(), static_cast<int32_t>(str.size()),
        nullptr /*lpMultiByteStr*/, 0 /*cbMultiByte*/, nullptr /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/);
    winrt::check_bool(bytes);

    auto buffer = std::make_unique<char[]>(bytes);
    winrt::check_bool(::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.data(), static_cast<int32_t>(str.size()),
        buffer.get(), bytes, nullptr /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/));

    return jsi::String::createFromUtf8(runtime, reinterpret_cast<const uint8_t*>(buffer.get()), bytes);
}

std::u16string rnwinrt::string_to_utf16(jsi::Runtime& runtime, const jsi::String& string)
{
    auto str = string.utf8(runtime);
    if (str.empty())
    {
        return {};
    }

    auto len = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int32_t>(str.size()),
        nullptr /*lpWideCharStr*/, 0 /*cchWideChar*/);
    winrt::check_bool(len);

    std::u16string result(len, 0);
    winrt::check_bool(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(),
        static_cast<int32_t>(str.size()), reinterpret_cast<LPWSTR>(result.data()), len));

    return result;
}

[[noreturn]] __declspec(noinline) void rnwinrt::throw_no_constructor(
    jsi::Runtime& runtime, std::string_view typeNamespace, std::string_view typeName, size_t argCount)
{
    auto msg = "TypeError: No constructor overload exists for "s;
    msg.append(typeNamespace);
    msg += ".";
    msg.append(typeName);
    msg = msg + " with " + std::to_string(argCount) + " args";
    throw jsi::JSError(runtime, std::move(msg));
}

[[noreturn]] __declspec(noinline) void rnwinrt::throw_no_function_overload(jsi::Runtime& runtime,
    std::string_view typeNamespace, std::string_view typeName, std::string_view fnName, size_t argCount)
{
    auto msg = "TypeError: No function overload exists for "s;
    msg.append(typeNamespace);
    msg += ".";
    msg.append(typeName);
    msg += ".";
    msg.append(fnName);
    msg = msg + " with " + std::to_string(argCount) + " args";
    throw jsi::JSError(runtime, std::move(msg));
}

[[noreturn]] __declspec(noinline) void rnwinrt::throw_invalid_delegate_arg_count(
    jsi::Runtime& runtime, std::string_view typeNamespace, std::string_view typeName)
{
    auto msg = "TypeError: Invalid number of arguments to delegate "s;
    msg.append(typeNamespace);
    msg += ".";
    msg.append(typeName);
    throw jsi::JSError(runtime, std::move(msg));
}

// NOTE: Most lists are sorted, so in theory this could be a binary search-turns to linear search. The only thing
// blocking this are enums, where their values are not currently sorted by name. Note however, that if this happens,
// there can still be duplicates in a list (e.g. function overloads), so this would have to act more like
// std::lower_bound (or std::equal_range) since callers expect the result to be the first.
template <typename ThingWithName>
static auto find_by_name(span<const ThingWithName* const> list, std::string_view name) noexcept
{
    return std::find_if(list.begin(), list.end(), [&](const ThingWithName* ptr) { return ptr->name == name; });
}

template <typename ThingWithName>
static auto find_by_name(span<const ThingWithName> list, std::string_view name) noexcept
{
    return std::find_if(list.begin(), list.end(), [&](const ThingWithName& thing) { return thing.name == name; });
}

jsi::Value object_instance_cache::get_instance(jsi::Runtime& runtime, const winrt::IInspectable& value)
{
    if ((std::chrono::steady_clock::now() - last_cleanup) >= cleanup_interval)
    {
        cleanup(runtime);
    }

    // NOTE: Each interface has its own associated v-table, so two IInspectable pointers to the same object may actually
    // be different if they were originally pointers to two different interfaces. Hence the QI here
    auto instance = value.as<winrt::IInspectable>();
    auto key = winrt::get_abi(instance);
    if (auto itr = instances.find(key); itr != instances.end())
    {
        // NOTE: It is possible for an interface to get deallocated and have its memory address reused for a new object,
        // however because we hold strong references to WinRT objects and weak references to the JS objects we create,
        // this would imply that the JS object also got GC'd and would fail to resolve below
        if (supports_weak_object)
        {
            if (auto strongValue = std::get<0>(itr->second).lock(runtime); !strongValue.isUndefined())
            {
                return strongValue;
            }
        }
        else
        {
            if (auto hostObj = std::get<1>(itr->second).lock())
            {
                return jsi::Value(runtime, jsi::Object::createFromHostObject(runtime, std::move(hostObj)));
            }
        }

        // Otherwise, the object has been GC'd. Remove it so that we can re-create the object below
        instances.erase(itr);
    }

    auto hostObj = std::make_shared<projected_object_instance>(instance);
    auto obj = jsi::Object::createFromHostObject(runtime, hostObj);
    if (supports_weak_object)
    {
        try
        {
            instances.emplace(key, jsi::WeakObject(runtime, obj));
        }
        catch (std::logic_error&)
        {
            supports_weak_object = false;
        }
    }

    if (!supports_weak_object)
    {
        instances.emplace(key, std::move(hostObj));
    }

    return jsi::Value(runtime, std::move(obj));
}

jsi::Value static_namespace_data::create(jsi::Runtime& runtime) const
{
    return jsi::Value(runtime, jsi::Object::createFromHostObject(runtime, std::make_shared<projected_namespace>(this)));
}

jsi::Value projected_namespace::get(jsi::Runtime& runtime, const jsi::PropNameID& name)
{
    if (auto itr = find_by_name(m_data->children, name.utf8(runtime)); itr != m_data->children.end())
    {
        auto& item = m_children[itr - m_data->children.begin()];
        if (item.isUndefined())
        {
            item = (*itr)->create(runtime);
        }

        return jsi::Value(runtime, item);
    }

    return jsi::Value::undefined();
}

void projected_namespace::set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value&)
{
    throw jsi::JSError(
        runtime, "TypeError: Cannot assign to property '" + name.utf8(runtime) + "' of a projected WinRT namespace");
}

std::vector<jsi::PropNameID> projected_namespace::getPropertyNames(jsi::Runtime& runtime)
{
    std::vector<jsi::PropNameID> result;
    result.reserve(m_children.size());
    for (auto ptr : m_data->children)
    {
        result.push_back(make_propid(runtime, ptr->name));
    }

    return result;
}

jsi::Value static_enum_data::create(jsi::Runtime& runtime) const
{
    return jsi::Value(runtime, jsi::Object::createFromHostObject(runtime, std::make_shared<projected_enum>(this)));
}

jsi::Value static_enum_data::get_value(std::string_view valueName) const
{
    // TODO: It would also be rather simple to ensure that the array is sorted and do a binary search, however the
    // number of enum elements is typically pretty small, so that may actually be harmful
    auto itr = std::find_if(values.begin(), values.end(), [&](auto& mapping) { return mapping.name == valueName; });
    if (itr == values.end())
    {
        return jsi::Value::undefined();
    }

    return jsi::Value(itr->value);
}

jsi::Value projected_enum::get(jsi::Runtime& runtime, const jsi::PropNameID& name)
{
    return m_data->get_value(name.utf8(runtime));
}

void projected_enum::set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value&)
{
    throw jsi::JSError(
        runtime, "TypeError: Cannot assign to property '" + name.utf8(runtime) + "' of a projected WinRT enum");
}

std::vector<jsi::PropNameID> projected_enum::getPropertyNames(jsi::Runtime& runtime)
{
    std::vector<jsi::PropNameID> result;
    result.reserve(m_data->values.size());
    for (auto& mapping : m_data->values)
    {
        result.push_back(make_propid(runtime, mapping.name));
    }

    return result;
}

jsi::Value static_class_data::create(jsi::Runtime& runtime) const
{
    return jsi::Value(
        runtime, jsi::Object::createFromHostObject(runtime, std::make_shared<projected_statics_class>(this)));
}

static jsi::Value static_add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count,
    const static_class_data* data, event_registration_array& registrations)
{
    if (count < 2)
    {
        throw jsi::JSError(runtime, "TypeError: addEventListener expects (at least) 2 arguments");
    }

    auto name = args[0].asString(runtime).utf8(runtime);
    if (auto itr = find_by_name(data->events, name); itr != data->events.end())
    {
        auto token = itr->add(runtime, args[1]);
        registrations.add(args[1].asObject(runtime), itr->name.data(), token);
    }

    return jsi::Value::undefined();
}

static jsi::Value static_remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count,
    const static_class_data* data, event_registration_array& registrations)
{
    if (count < 2)
    {
        throw jsi::JSError(runtime, "TypeError: removeEventListener expects (at least) 2 arguments");
    }

    auto name = args[0].asString(runtime).utf8(runtime);
    if (auto itr = find_by_name(data->events, name); itr != data->events.end())
    {
        auto token = registrations.remove(runtime, args[1].asObject(runtime), itr->name.data());
        itr->remove(token);
    }

    return jsi::Value::undefined();
}

jsi::Value projected_statics_class::get(jsi::Runtime& runtime, const jsi::PropNameID& id)
{
    auto name = id.utf8(runtime);

    if (auto itr = find_by_name(m_data->properties, name); itr != m_data->properties.end())
    {
        return itr->getter(runtime);
    }

    auto itr = m_functions.find(name);
    if (itr == m_functions.end())
    {
        auto dataItr = find_by_name(m_data->functions, name);
        if (dataItr != m_data->functions.end())
        {
            auto fn = jsi::Function::createFromHostFunction(runtime, id, 0, dataItr->function);
            itr = m_functions.emplace(dataItr->name, jsi::Value(runtime, std::move(fn))).first;
        }
        else if (!m_data->events.empty())
        {
            if (name == add_event_name)
            {
                auto fn = bind_host_function(runtime, id, 2, &projected_statics_class::add_event_listener);
                itr = m_functions.emplace(add_event_name, jsi::Value(runtime, std::move(fn))).first;
            }
            else if (name == remove_event_name)
            {
                auto fn = bind_host_function(runtime, id, 2, &projected_statics_class::remove_event_listener);
                itr = m_functions.emplace(remove_event_name, jsi::Value(runtime, std::move(fn))).first;
            }
        }
    }

    if (itr == m_functions.end())
    {
        return jsi::Value::undefined();
    }

    return jsi::Value(runtime, itr->second);
}

void projected_statics_class::set(jsi::Runtime& runtime, const jsi::PropNameID& id, const jsi::Value& value)
{
    auto name = id.utf8(runtime);
    if (auto itr = find_by_name(m_data->properties, name); itr != m_data->properties.end())
    {
        // Unlike getters, setters can be null
        if (itr->setter)
        {
            (*itr->setter)(runtime, value);
        }
    }

    // If no property exists with the given name, then ignore the call rather than throwing. This is more-or-less
    // consistent with EdgeHTML WebView.
}

std::vector<jsi::PropNameID> projected_statics_class::getPropertyNames(jsi::Runtime& runtime)
{
    std::vector<jsi::PropNameID> result;
    result.reserve(m_data->properties.size() + m_data->functions.size() + (m_data->events.empty() ? 0 : 2));

    for (auto&& data : m_data->properties)
    {
        result.push_back(make_propid(runtime, data.name));
    }

    for (auto&& data : m_data->functions)
    {
        result.push_back(make_propid(runtime, data.name));
    }

    if (!m_data->events.empty())
    {
        result.push_back(make_propid(runtime, add_event_name));
        result.push_back(make_propid(runtime, remove_event_name));
    }

    return result;
}

jsi::Value projected_statics_class::add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
{
    return static_add_event_listener(runtime, args, count, m_data, m_events);
}

jsi::Value projected_statics_class::remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
{
    return static_remove_event_listener(runtime, args, count, m_data, m_events);
}

jsi::Value static_activatable_class_data::create(jsi::Runtime& runtime) const
{
    auto code = "(function() { return "s;
    code.append(full_namespace);
    code.append(".");
    code.append(name);
    code.append(".ctor.apply(this, arguments); })");
    auto result = runtime.evaluateJavaScript(std::make_shared<jsi::StringBuffer>(std::move(code)), "Activatable Class")
                      .asObject(runtime);

    // TODO: param count? Seems to not matter? It would be rather simple to calculate when generating the constructor
    // function, but would also be more data...
    result.setProperty(
        runtime, "ctor", jsi::Function::createFromHostFunction(runtime, make_propid(runtime, name), 0, constructor));

    // JSI does not allow us to create a 'Function' that is also a 'HostObject' and therefore cannot provide virtual
    // get/set functions and instead must attach them to the function object
    auto defineProperty =
        runtime.global().getPropertyAsFunction(runtime, "Object").getPropertyAsFunction(runtime, "defineProperty");
    for (auto&& prop : properties)
    {
        jsi::Object propDesc(runtime);
        propDesc.setProperty(runtime, "get",
            jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "get"), 0,
                [getter = prop.getter](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value*, size_t count) {
                    if (count != 0)
                    {
                        throw jsi::JSError(runtime, "TypeError: Property getter expects 0 arguments");
                    }

                    return getter(runtime);
                }));
        if (prop.setter)
        {
            propDesc.setProperty(runtime, "set",
                jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "set"), 1,
                    [setter = prop.setter](
                        jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                        if (count != 1)
                        {
                            throw jsi::JSError(runtime, "TypeError: Property setter expects 1 argument");
                        }

                        setter(runtime, args[0]);
                        return jsi::Value::undefined();
                    }));
        }

        defineProperty.call(runtime, result, make_string(runtime, prop.name), std::move(propDesc));
    }

    for (auto&& fn : functions)
    {
        auto propId = make_propid(runtime, fn.name);
        result.setProperty(runtime, propId, jsi::Function::createFromHostFunction(runtime, propId, 0, fn.function));
    }

    if (!events.empty())
    {
        // NOTE: We need shared state, but there's not really a good way to store that state on the function object that
        // we return... Use a unique_ptr here and give ownership to a single lambda. Neither should outlive the object
        // we return, so this should be okay
        auto evtArray = std::make_unique<event_registration_array>();

        auto propId = make_propid(runtime, add_event_name);
        result.setProperty(runtime, propId,
            jsi::Function::createFromHostFunction(runtime, propId, 0,
                [this, evtArray = evtArray.get()](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args,
                    size_t count) { return static_add_event_listener(runtime, args, count, this, *evtArray); }));

        propId = make_propid(runtime, remove_event_name);
        result.setProperty(runtime, propId,
            jsi::Function::createFromHostFunction(runtime, propId, 0,
                move_only_lambda([this, evtArray = std::move(evtArray)](
                                     jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    return static_remove_event_listener(runtime, args, count, this, *evtArray);
                })));
    }

    return result;
}

#ifndef _WIN32
static_assert(false, "Compilation requires a little-endian target");
#endif

// NOTE: Must be kept in-sync with the copy in MetadataTypes.h
static int compare_guid(const winrt::guid& lhs, const winrt::guid& rhs)
{
    // NOTE: This method of comparison needs to remain consistant with how we sort the static array
    auto lhsPtr = reinterpret_cast<const std::int64_t*>(&lhs);
    auto rhsPtr = reinterpret_cast<const std::int64_t*>(&rhs);
    if (auto diff = lhsPtr[0] - rhsPtr[0])
    {
        return (diff < 0) ? -1 : 1;
    }

    auto diff = lhsPtr[1] - rhsPtr[1];
    return (diff == 0) ? 0 : (diff < 0) ? -1 : 1;
}

static const static_interface_data* find_interface(const winrt::guid& guid)
{
    auto begin = global_interface_map.begin();
    auto end = global_interface_map.end();
    static constexpr std::ptrdiff_t linear_search_size = 16; // TODO: Find a good value

    while ((end - begin) > linear_search_size)
    {
        auto mid = begin + (end - begin) / 2;
        auto cmp = compare_guid(guid, mid->first);
        if (cmp < 0)
        {
            end = mid;
        }
        else if (cmp > 0)
        {
            begin = mid + 1;
        }
        else
        {
            return mid->second;
        }
    }

    for (; begin != end; ++begin)
    {
        if (begin->first == guid)
        {
            return begin->second;
        }
    }

    return nullptr;
}

projected_object_instance::projected_object_instance(const winrt::IInspectable& instance) : m_instance(instance)
{
    auto iids = winrt::get_interfaces(m_instance);
    for (auto&& iid : iids)
    {
        if (auto iface = find_interface(iid))
        {
            m_interfaces.push_back(iface);
        }
    }
}

namespace rnwinrt
{
    struct projected_function
    {
        jsi::Value operator()(
            jsi::Runtime& runtime, const jsi::Value& thisVal, const jsi::Value* args, size_t count) const
        {
            if (count != data->arity)
            {
                throw jsi::JSError(runtime, "TypeError: Non-overloaded function " + std::string(data->name) +
                                                " expects " + std::to_string(data->arity) + " arguments, but " +
                                                std::to_string(count) + " provided");
            }

            auto obj = thisVal.asObject(runtime).asHostObject<projected_object_instance>(runtime);
            return data->function(runtime, obj->m_instance, args);
        }

        const static_interface_data::function_mapping* data;
    };

    struct projected_overloaded_function
    {
        jsi::Value operator()(
            jsi::Runtime& runtime, const jsi::Value& thisVal, const jsi::Value* args, size_t count) const
        {
            for (auto func : data)
            {
                if (func->arity == count)
                {
                    auto obj = thisVal.asObject(runtime).asHostObject<projected_object_instance>(runtime);
                    return func->function(runtime, obj->m_instance, args);
                }
            }

            throw jsi::JSError(runtime, "TypeError: Overloaded function " + std::string(data[0]->name) +
                                            " does not have an overload that expects " + std::to_string(count) +
                                            " arguments");
        }

        // TODO: Figure out a good SSO size (4 might be larger than we need most of the time. Perhaps 2?)
        sso_vector<const static_interface_data::function_mapping*, 4> data;
    };
}

jsi::Value projected_object_instance::get(jsi::Runtime& runtime, const jsi::PropNameID& id)
{
    auto name = id.utf8(runtime);
    if (auto itr = m_functions.find(name); itr != m_functions.end())
    {
        return jsi::Value(runtime, itr->second);
    }

    sso_vector<const static_interface_data::function_mapping*> functions;
    bool hasEvents = false;
    for (auto iface : m_interfaces)
    {
        if (auto itr = find_by_name(iface->properties, name); (itr != iface->properties.end()) && itr->getter)
        {
            return itr->getter(runtime, m_instance);
        }

        if (auto dataItr = find_by_name(iface->functions, name); dataItr != iface->functions.end())
        {
            functions.push_back(&*dataItr);

            // NOTE: Functions are sorted, so this should be the first of N consecutive functions with the same name
            for (++dataItr; (dataItr != iface->functions.end()) && (dataItr->name == name); ++dataItr)
            {
                functions.push_back(&*dataItr);
            }
        }

        hasEvents = hasEvents || !iface->events.empty();
    }

    if (functions.size() > 1)
    {
        // Make sure there are no conflicts in arity. While this isn't technically 100% necessary, we still at least
        // need to validate that default overloads are at the front of the list
        // NOTE: The number of overloads should be pretty minimal, so doing this in a "selection sort-like" way should
        // be fine
        for (size_t i = 0; i < functions.size(); ++i)
        {
            // NOTE: Even if the target is the default overload, we still want to remove other ones with the same arity
            auto tgt = functions[i];
            for (size_t j = i + 1; j < functions.size();)
            {
                auto test = functions[j];
                if (tgt->arity == test->arity)
                {
                    if (!tgt->is_default_overload && test->is_default_overload)
                    {
                        // Use the other one
                        tgt = test;
                        functions[i] = test;
                    }

                    functions[j] = functions.back();
                    functions.pop_back();
                    continue;
                }

                ++j;
            }
        }
    }

    if (functions.size() == 1)
    {
        // Non-overloaded function, or at least not overloaded with different arities
        auto fn =
            jsi::Function::createFromHostFunction(runtime, id, functions[0]->arity, projected_function{ functions[0] });
        return jsi::Value(runtime, m_functions.emplace(functions[0]->name, std::move(fn)).first->second);
    }
    else if (!functions.empty())
    {
        // TODO: Calculate max arity? Does it matter?
        auto functionName = functions[0]->name;
        auto fn = jsi::Function::createFromHostFunction(
            runtime, id, 0, projected_overloaded_function{ std::move(functions) });
        return jsi::Value(runtime, m_functions.emplace(functionName, std::move(fn)).first->second);
    }

    if (hasEvents)
    {
        if (name == add_event_name)
        {
            auto fn = bind_host_function(runtime, id, 2, &projected_object_instance::add_event_listener);
            return jsi::Value(runtime, m_functions.emplace(add_event_name, std::move(fn)).first->second);
        }
        else if (name == remove_event_name)
        {
            auto fn = bind_host_function(runtime, id, 2, &projected_object_instance::remove_event_listener);
            return jsi::Value(runtime, m_functions.emplace(remove_event_name, std::move(fn)).first->second);
        }
    }

    // If we've made it this far, check to see if any interface wants to handle the call (e.g. operator[] etc.)
    jsi::Value fallbackValue;
    for (auto iface : m_interfaces)
    {
        if (!iface->runtime_get_property)
            continue;

        auto [result, fallback] = iface->runtime_get_property(runtime, m_instance, name);
        if (result)
            return std::move(*result);
        else if (fallback)
            fallbackValue = std::move(*fallback);
    }

    return std::move(fallbackValue);
}

void projected_object_instance::set(jsi::Runtime& runtime, const jsi::PropNameID& id, const jsi::Value& value)
{
    auto name = id.utf8(runtime);
    for (auto iface : m_interfaces)
    {
        if (auto itr = find_by_name(iface->properties, name); (itr != iface->properties.end()) && itr->setter)
        {
            itr->setter(runtime, m_instance, value);
            return;
        }
    }

    // If we've made it this far, check to see if any interface wants to handle the call (e.g. operator[] etc.)
    for (auto iface : m_interfaces)
    {
        if (!iface->runtime_set_property)
            continue;

        if (iface->runtime_set_property(runtime, m_instance, name, value))
            return;
    }

    // If no property exists with the given name, then ignore the call rather than throwing. This is more-or-less
    // consistent with EdgeHTML WebView.
}

std::vector<jsi::PropNameID> projected_object_instance::getPropertyNames(jsi::Runtime& runtime)
{
    // TODO: Since functions can be overloaded - and we don't collate them on interfaces like we do with classes - we
    // may end up with duplicates. Is that okay?
    std::vector<jsi::PropNameID> result;
    bool hasEvents = false;
    for (auto iface : m_interfaces)
    {
        for (auto&& prop : iface->properties)
        {
            result.push_back(make_propid(runtime, prop.name));
        }

        for (auto&& func : iface->functions)
        {
            result.push_back(make_propid(runtime, func.name));
        }

        hasEvents = hasEvents || !iface->events.empty();
    }

    if (hasEvents)
    {
        result.push_back(make_propid(runtime, add_event_name));
        result.push_back(make_propid(runtime, remove_event_name));
    }

    return result;
}

jsi::Value projected_object_instance::add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
{
    if (count < 2)
    {
        throw jsi::JSError(runtime, "TypeError: addEventListener expects (at least) 2 arguments");
    }

    auto name = args[0].asString(runtime).utf8(runtime);
    for (auto iface : m_interfaces)
    {
        if (auto itr = find_by_name(iface->events, name); itr != iface->events.end())
        {
            auto token = itr->add(runtime, m_instance, args[1]);
            current_runtime_context()->event_cache.add(m_instance, args[1].asObject(runtime), itr->name.data(), token);
            break;
        }
    }

    return jsi::Value::undefined();
}

jsi::Value projected_object_instance::remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
{
    if (count < 2)
    {
        throw jsi::JSError(runtime, "TypeError: removeEventListener expects (at least) 2 arguments");
    }

    auto name = args[0].asString(runtime).utf8(runtime);
    for (auto iface : m_interfaces)
    {
        if (auto itr = find_by_name(iface->events, name); itr != iface->events.end())
        {
            // TODO: Should we just no-op if the token can't be found?
            auto token = current_runtime_context()->event_cache.remove(
                runtime, m_instance, args[1].asObject(runtime), itr->name.data());
            itr->remove(m_instance, token);
            break;
        }
    }

    return jsi::Value::undefined();
}

bool projected_value_traits<bool>::as_native(jsi::Runtime&, const jsi::Value& value) noexcept
{
    if (value.isBool())
    {
        return value.getBool();
    }
    else if (value.isNumber())
    {
        return value.getNumber() != 0;
    }

    return !value.isNull() && !value.isUndefined();
}

jsi::Value projected_value_traits<char16_t>::as_value(jsi::Runtime& runtime, char16_t value)
{
    char buffer[8];
    auto bytes = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, reinterpret_cast<PCWSTR>(&value), 1, buffer,
        static_cast<int>(std::size(buffer)), nullptr, nullptr);
    winrt::check_bool(bytes);
    return jsi::String::createFromUtf8(runtime, reinterpret_cast<uint8_t*>(buffer), bytes);
}

char16_t projected_value_traits<char16_t>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    auto str = convert_value_to_native<winrt::hstring>(runtime, value);
    return str.empty() ? 0 : str[0];
}

winrt::hstring projected_value_traits<winrt::hstring>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    auto str = value.asString(runtime).utf8(runtime);
    if (str.empty())
    {
        return {};
    }

    // MultiByteToWideChar is requesting the size in wide characters required for 'stringUtf8' without null
    // termination as WindowsPreallocateStringBuffer will actually allocated 'outputLength + 1' characters and asign
    // the last as the null terminator automatically.

    auto len = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int32_t>(str.size()),
        nullptr /*lpWideCharStr*/, 0 /*cchWideChar*/);
    winrt::check_bool(len);

    PWSTR stringBuffer;
    HSTRING_BUFFER buffer;
    winrt::check_hresult(::WindowsPreallocateStringBuffer(static_cast<uint32_t>(len), &stringBuffer, &buffer));

    winrt::hstring result;
    try
    {
        winrt::check_bool(::MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int32_t>(str.size()), stringBuffer, len));
        winrt::check_hresult(::WindowsPromoteStringBuffer(buffer, reinterpret_cast<HSTRING*>(winrt::put_abi(result))));
    }
    catch (...)
    {
        ::WindowsDeleteStringBuffer(buffer);
        throw;
    }

    return result;
}

// Lengths are not null-terminated.
static constexpr uint32_t uuid_length = 8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12;
static constexpr uint32_t guid_length = 2 + uuid_length;

jsi::Value projected_value_traits<winrt::guid>::as_value(jsi::Runtime& runtime, const winrt::guid& value)
{
    // NOTE: 'StringFromGUID2' formats in '{...}' form, but we don't want the curly braces
    wchar_t wideBuffer[guid_length + 1];
    winrt::check_hresult(::StringFromGUID2(reinterpret_cast<const GUID&>(value), wideBuffer, ARRAYSIZE(wideBuffer)));

    // GUIDS are always ANSI, so there's no need to call 'WideCharToMultiByte' or anything
    char buffer[uuid_length];
    std::transform(
        wideBuffer + 1, wideBuffer + 1 + uuid_length, buffer, [](wchar_t ch) { return static_cast<char>(ch); });

    return jsi::Value(runtime, jsi::String::createFromAscii(runtime, buffer, uuid_length));
}

winrt::guid projected_value_traits<winrt::guid>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    auto str = value.asString(runtime).utf8(runtime);
    auto strBuffer = str.data();
    if (str.size() == guid_length)
    {
        ++strBuffer; // Move past the '{'
        strBuffer[uuid_length] = 0; // UuidFromString expects null termination
    }
    else if (str.size() != uuid_length)
    {
        throw jsi::JSError(runtime, "TypeError: Invalid GUID length");
    }

    winrt::guid result;
    if (::UuidFromStringA(reinterpret_cast<RPC_CSTR>(strBuffer), reinterpret_cast<UUID*>(winrt::put_abi(result))) !=
        ERROR_SUCCESS)
    {
        throw jsi::JSError(runtime, "TypeError: GUID contains unexpected characters");
    }

    return result;
}

// NOTE: 'DateTime' is a 'FILETIME' value which represents the number of 100 ns "units" since 01/01/1601, whereas the
// Javascript 'Date' class represents the number of milliseconds since 01/01/1970. The delta between the two is
// 11644473600 seconds
static constexpr auto windows_to_unix_epoch_delta = 11644473600s;

jsi::Value projected_value_traits<winrt::DateTime>::as_value(jsi::Runtime& runtime, winrt::DateTime value)
{
    auto unixTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(value.time_since_epoch() - windows_to_unix_epoch_delta);
    return runtime.global()
        .getPropertyAsFunction(runtime, "Date")
        .callAsConstructor(runtime, static_cast<double>(unixTime.count()));
}

winrt::DateTime projected_value_traits<winrt::DateTime>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    double number;
    if (value.isNumber())
    {
        number = value.getNumber();
    }
    else
    {
        auto object = value.asObject(runtime);
        number = object.getPropertyAsFunction(runtime, "valueOf").callWithThis(runtime, object).asNumber();
    }

    std::chrono::milliseconds unixTime(static_cast<int64_t>(number));
    return winrt::DateTime(unixTime + windows_to_unix_epoch_delta);
}

jsi::Value projected_value_traits<winrt::TimeSpan>::as_value(jsi::Runtime& runtime, winrt::TimeSpan value)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(value);
    return convert_native_to_value<int64_t>(runtime, ms.count());
}

winrt::TimeSpan projected_value_traits<winrt::TimeSpan>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    std::chrono::milliseconds ms(convert_value_to_native<int64_t>(runtime, value));
    return std::chrono::duration_cast<winrt::TimeSpan>(ms);
}

jsi::Value projected_value_traits<winrt::float3x2>::as_value(jsi::Runtime& runtime, winrt::float3x2 value)
{
    jsi::Object result(runtime);
    result.setProperty(runtime, "m11", convert_native_to_value(runtime, value.m11));
    result.setProperty(runtime, "m12", convert_native_to_value(runtime, value.m12));
    result.setProperty(runtime, "m21", convert_native_to_value(runtime, value.m21));
    result.setProperty(runtime, "m22", convert_native_to_value(runtime, value.m22));
    result.setProperty(runtime, "m31", convert_native_to_value(runtime, value.m31));
    result.setProperty(runtime, "m32", convert_native_to_value(runtime, value.m32));
    return result;
}

winrt::float3x2 projected_value_traits<winrt::float3x2>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    winrt::float3x2 result{};
    auto obj = value.asObject(runtime);
    if (auto field = obj.getProperty(runtime, "m11"); !field.isUndefined())
        result.m11 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m12"); !field.isUndefined())
        result.m12 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m21"); !field.isUndefined())
        result.m21 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m22"); !field.isUndefined())
        result.m22 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m31"); !field.isUndefined())
        result.m31 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m32"); !field.isUndefined())
        result.m32 = convert_value_to_native<float>(runtime, field);
    return result;
}

jsi::Value projected_value_traits<winrt::float4x4>::as_value(jsi::Runtime& runtime, winrt::float4x4 value)
{
    jsi::Object result(runtime);
    result.setProperty(runtime, "m11", convert_native_to_value(runtime, value.m11));
    result.setProperty(runtime, "m12", convert_native_to_value(runtime, value.m12));
    result.setProperty(runtime, "m13", convert_native_to_value(runtime, value.m13));
    result.setProperty(runtime, "m14", convert_native_to_value(runtime, value.m14));
    result.setProperty(runtime, "m21", convert_native_to_value(runtime, value.m21));
    result.setProperty(runtime, "m22", convert_native_to_value(runtime, value.m22));
    result.setProperty(runtime, "m23", convert_native_to_value(runtime, value.m23));
    result.setProperty(runtime, "m24", convert_native_to_value(runtime, value.m24));
    result.setProperty(runtime, "m31", convert_native_to_value(runtime, value.m31));
    result.setProperty(runtime, "m32", convert_native_to_value(runtime, value.m32));
    result.setProperty(runtime, "m33", convert_native_to_value(runtime, value.m33));
    result.setProperty(runtime, "m34", convert_native_to_value(runtime, value.m34));
    result.setProperty(runtime, "m41", convert_native_to_value(runtime, value.m41));
    result.setProperty(runtime, "m42", convert_native_to_value(runtime, value.m42));
    result.setProperty(runtime, "m43", convert_native_to_value(runtime, value.m43));
    result.setProperty(runtime, "m44", convert_native_to_value(runtime, value.m44));
    return result;
}

winrt::float4x4 projected_value_traits<winrt::float4x4>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    winrt::float4x4 result{};
    auto obj = value.asObject(runtime);
    if (auto field = obj.getProperty(runtime, "m11"); !field.isUndefined())
        result.m11 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m12"); !field.isUndefined())
        result.m12 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m13"); !field.isUndefined())
        result.m13 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m14"); !field.isUndefined())
        result.m14 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m21"); !field.isUndefined())
        result.m21 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m22"); !field.isUndefined())
        result.m22 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m23"); !field.isUndefined())
        result.m23 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m24"); !field.isUndefined())
        result.m24 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m31"); !field.isUndefined())
        result.m31 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m32"); !field.isUndefined())
        result.m32 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m33"); !field.isUndefined())
        result.m33 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m34"); !field.isUndefined())
        result.m34 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m41"); !field.isUndefined())
        result.m41 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m42"); !field.isUndefined())
        result.m42 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m43"); !field.isUndefined())
        result.m43 = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "m44"); !field.isUndefined())
        result.m44 = convert_value_to_native<float>(runtime, field);
    return result;
}

jsi::Value projected_value_traits<winrt::plane>::as_value(jsi::Runtime& runtime, winrt::plane value)
{
    jsi::Object result(runtime);
    result.setProperty(runtime, "normal", convert_native_to_value(runtime, value.normal));
    result.setProperty(runtime, "d", convert_native_to_value(runtime, value.d));
    return result;
}

winrt::plane projected_value_traits<winrt::plane>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    winrt::plane result{};
    auto obj = value.asObject(runtime);
    if (auto field = obj.getProperty(runtime, "normal"); !field.isUndefined())
        result.normal = convert_value_to_native<winrt::float3>(runtime, field);
    if (auto field = obj.getProperty(runtime, "d"); !field.isUndefined())
        result.d = convert_value_to_native<float>(runtime, field);
    return result;
}

jsi::Value projected_value_traits<winrt::quaternion>::as_value(jsi::Runtime& runtime, winrt::quaternion value)
{
    jsi::Object result(runtime);
    result.setProperty(runtime, "x", convert_native_to_value(runtime, value.x));
    result.setProperty(runtime, "y", convert_native_to_value(runtime, value.y));
    result.setProperty(runtime, "z", convert_native_to_value(runtime, value.z));
    result.setProperty(runtime, "w", convert_native_to_value(runtime, value.w));
    return result;
}

winrt::quaternion projected_value_traits<winrt::quaternion>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    winrt::quaternion result{};
    auto obj = value.asObject(runtime);
    if (auto field = obj.getProperty(runtime, "x"); !field.isUndefined())
        result.x = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "y"); !field.isUndefined())
        result.y = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "z"); !field.isUndefined())
        result.z = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "w"); !field.isUndefined())
        result.w = convert_value_to_native<float>(runtime, field);
    return result;
}

jsi::Value projected_value_traits<winrt::float2>::as_value(jsi::Runtime& runtime, winrt::float2 value)
{
    jsi::Object result(runtime);
    result.setProperty(runtime, "x", convert_native_to_value(runtime, value.x));
    result.setProperty(runtime, "y", convert_native_to_value(runtime, value.y));
    return result;
}

winrt::float2 projected_value_traits<winrt::float2>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    winrt::float2 result{};
    auto obj = value.asObject(runtime);
    if (auto field = obj.getProperty(runtime, "x"); !field.isUndefined())
        result.x = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "y"); !field.isUndefined())
        result.y = convert_value_to_native<float>(runtime, field);
    return result;
}

jsi::Value projected_value_traits<winrt::float3>::as_value(jsi::Runtime& runtime, winrt::float3 value)
{
    jsi::Object result(runtime);
    result.setProperty(runtime, "x", convert_native_to_value(runtime, value.x));
    result.setProperty(runtime, "y", convert_native_to_value(runtime, value.y));
    result.setProperty(runtime, "z", convert_native_to_value(runtime, value.z));
    return result;
}

winrt::float3 projected_value_traits<winrt::float3>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    winrt::float3 result{};
    auto obj = value.asObject(runtime);
    if (auto field = obj.getProperty(runtime, "x"); !field.isUndefined())
        result.x = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "y"); !field.isUndefined())
        result.y = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "z"); !field.isUndefined())
        result.z = convert_value_to_native<float>(runtime, field);
    return result;
}

jsi::Value projected_value_traits<winrt::float4>::as_value(jsi::Runtime& runtime, winrt::float4 value)
{
    jsi::Object result(runtime);
    result.setProperty(runtime, "x", convert_native_to_value(runtime, value.x));
    result.setProperty(runtime, "y", convert_native_to_value(runtime, value.y));
    result.setProperty(runtime, "z", convert_native_to_value(runtime, value.z));
    result.setProperty(runtime, "w", convert_native_to_value(runtime, value.w));
    return result;
}

winrt::float4 projected_value_traits<winrt::float4>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    winrt::float4 result{};
    auto obj = value.asObject(runtime);
    if (auto field = obj.getProperty(runtime, "x"); !field.isUndefined())
        result.x = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "y"); !field.isUndefined())
        result.y = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "z"); !field.isUndefined())
        result.z = convert_value_to_native<float>(runtime, field);
    if (auto field = obj.getProperty(runtime, "w"); !field.isUndefined())
        result.w = convert_value_to_native<float>(runtime, field);
    return result;
}

jsi::Value rnwinrt::convert_from_property_value(jsi::Runtime& runtime, const winrt::IPropertyValue& value)
{
    switch (value.Type())
    {
    case winrt::PropertyType::Empty:
        return jsi::Value::undefined();
    case winrt::PropertyType::UInt8:
        return convert_native_to_value(runtime, value.GetUInt8());
    case winrt::PropertyType::Int16:
        return convert_native_to_value(runtime, value.GetInt16());
    case winrt::PropertyType::UInt16:
        return convert_native_to_value(runtime, value.GetUInt16());
    case winrt::PropertyType::Int32:
        return convert_native_to_value(runtime, value.GetInt32());
    case winrt::PropertyType::UInt32:
        return convert_native_to_value(runtime, value.GetUInt32());
    case winrt::PropertyType::Int64:
        return convert_native_to_value(runtime, value.GetInt64());
    case winrt::PropertyType::UInt64:
        return convert_native_to_value(runtime, value.GetUInt64());
    case winrt::PropertyType::Single:
        return convert_native_to_value(runtime, value.GetSingle());
    case winrt::PropertyType::Double:
        return convert_native_to_value(runtime, value.GetDouble());
    case winrt::PropertyType::Char16:
        return convert_native_to_value(runtime, value.GetChar16());
    case winrt::PropertyType::Boolean:
        return convert_native_to_value(runtime, value.GetBoolean());
    case winrt::PropertyType::String:
        return convert_native_to_value(runtime, value.GetString());
    case winrt::PropertyType::Inspectable:
        return jsi::Value::undefined(); // IInspectable is just the object itself
    case winrt::PropertyType::DateTime:
        return convert_native_to_value(runtime, value.GetDateTime());
    case winrt::PropertyType::TimeSpan:
        return convert_native_to_value(runtime, value.GetTimeSpan());
    case winrt::PropertyType::Guid:
        return convert_native_to_value(runtime, value.GetGuid());
    case winrt::PropertyType::Point:
        return convert_native_to_value(runtime, value.GetPoint());
    case winrt::PropertyType::Size:
        return convert_native_to_value(runtime, value.GetSize());
    case winrt::PropertyType::Rect:
        return convert_native_to_value(runtime, value.GetRect());
    case winrt::PropertyType::OtherType:
        return jsi::Value::undefined();
    case winrt::PropertyType::UInt8Array: {
        winrt::com_array<uint8_t> result;
        value.GetUInt8Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::Int16Array: {
        winrt::com_array<int16_t> result;
        value.GetInt16Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::UInt16Array: {
        winrt::com_array<uint16_t> result;
        value.GetUInt16Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::Int32Array: {
        winrt::com_array<int32_t> result;
        value.GetInt32Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::UInt32Array: {
        winrt::com_array<uint32_t> result;
        value.GetUInt32Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::Int64Array: {
        winrt::com_array<int64_t> result;
        value.GetInt64Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::UInt64Array: {
        winrt::com_array<uint64_t> result;
        value.GetUInt64Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::SingleArray: {
        winrt::com_array<float> result;
        value.GetSingleArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::DoubleArray: {
        winrt::com_array<double> result;
        value.GetDoubleArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::Char16Array: {
        winrt::com_array<char16_t> result;
        value.GetChar16Array(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::BooleanArray: {
        winrt::com_array<bool> result;
        value.GetBooleanArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::StringArray: {
        winrt::com_array<winrt::hstring> result;
        value.GetStringArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::InspectableArray: {
        winrt::com_array<winrt::IInspectable> result;
        value.GetInspectableArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::DateTimeArray: {
        winrt::com_array<winrt::DateTime> result;
        value.GetDateTimeArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::TimeSpanArray: {
        winrt::com_array<winrt::TimeSpan> result;
        value.GetTimeSpanArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::GuidArray: {
        winrt::com_array<winrt::guid> result;
        value.GetGuidArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::PointArray: {
        winrt::com_array<winrt::Point> result;
        value.GetPointArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::SizeArray: {
        winrt::com_array<winrt::Size> result;
        value.GetSizeArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::RectArray: {
        winrt::com_array<winrt::Rect> result;
        value.GetRectArray(result);
        return convert_native_to_value(runtime, std::move(result));
    }
    case winrt::PropertyType::OtherTypeArray:
        return jsi::Value::undefined();
    default:
        winrt::terminate(); // TODO: Just return undefined?
    }
}

winrt::IInspectable rnwinrt::convert_to_property_value(jsi::Runtime& runtime, const jsi::Value& value)
{
    if (value.isBool())
    {
        return winrt::PropertyValue::CreateBoolean(value.getBool());
    }
    else if (value.isNumber())
    {
        // NOTE: Due to inherent ambiguities between the loosely typed JS 'number' type and strongly typed WinRT types,
        // we can't do much better than to take a guess here and preserve the value as a double-precision floating point
        // value. It's probably best for consumers to call the 'PropertyValue' static methods directly as needed
        return winrt::PropertyValue::CreateDouble(value.getNumber());
    }
    else if (value.isString())
    {
        return winrt::PropertyValue::CreateString(convert_value_to_native<winrt::hstring>(runtime, value));
    }
    else if (value.isObject())
    {
        auto obj = value.getObject(runtime);
        if (obj.isArray(runtime))
        {
            auto array = obj.getArray(runtime);

            // Can't determine type from empty arrays
            if (array.size(runtime) > 0)
            {
                auto elem = array.getValueAtIndex(runtime, 0);
                if (elem.isBool())
                {
                    return winrt::PropertyValue::CreateBooleanArray(
                        convert_value_to_native<winrt::array_view<const bool>>(runtime, value));
                }
                else if (elem.isNumber())
                {
                    return winrt::PropertyValue::CreateDoubleArray(
                        convert_value_to_native<winrt::array_view<const double>>(runtime, value));
                }
                else if (elem.isString())
                {
                    return winrt::PropertyValue::CreateStringArray(
                        convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, value));
                }
                else if (elem.isObject())
                {
                    auto elemObj = elem.getObject(runtime);
                    if (elemObj.isHostObject(runtime))
                    {
                        return winrt::PropertyValue::CreateInspectableArray(
                            convert_value_to_native<winrt::array_view<const winrt::IInspectable>>(runtime, value));
                    }

                    auto isPointLike = elemObj.hasProperty(runtime, "x") && elemObj.hasProperty(runtime, "y");
                    auto isSizeLike = elemObj.hasProperty(runtime, "width") && elemObj.hasProperty(runtime, "height");
                    if (isPointLike && isSizeLike)
                    {
                        return winrt::PropertyValue::CreateRectArray(
                            convert_value_to_native<winrt::array_view<const winrt::Rect>>(runtime, value));
                    }
                    else if (isPointLike)
                    {
                        return winrt::PropertyValue::CreatePointArray(
                            convert_value_to_native<winrt::array_view<const winrt::Point>>(runtime, value));
                    }
                    else if (isSizeLike)
                    {
                        return winrt::PropertyValue::CreateSizeArray(
                            convert_value_to_native<winrt::array_view<const winrt::Size>>(runtime, value));
                    }

                    auto isDateTimeLike =
                        elemObj.hasProperty(runtime, "getDate") && elemObj.hasProperty(runtime, "setDate") &&
                        elemObj.hasProperty(runtime, "getTime") && elemObj.hasProperty(runtime, "setTime") &&
                        elemObj.hasProperty(runtime, "valueOf");
                    if (isDateTimeLike)
                    {
                        return winrt::PropertyValue::CreateDateTimeArray(
                            convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::DateTime>>(
                                runtime, value));
                    }
                }
            }
        }
        else
        {
            auto isPointLike = obj.hasProperty(runtime, "x") && obj.hasProperty(runtime, "y");
            auto isSizeLike = obj.hasProperty(runtime, "width") && obj.hasProperty(runtime, "height");
            if (isPointLike && isSizeLike)
            {
                return winrt::PropertyValue::CreateRect(convert_value_to_native<winrt::Rect>(runtime, value));
            }
            else if (isPointLike)
            {
                return winrt::PropertyValue::CreatePoint(convert_value_to_native<winrt::Point>(runtime, value));
            }
            else if (isSizeLike)
            {
                return winrt::PropertyValue::CreateSize(convert_value_to_native<winrt::Size>(runtime, value));
            }

            auto isDateTimeLike = obj.hasProperty(runtime, "getDate") && obj.hasProperty(runtime, "setDate") &&
                                  obj.hasProperty(runtime, "getTime") && obj.hasProperty(runtime, "setTime") &&
                                  obj.hasProperty(runtime, "valueOf");
            if (isDateTimeLike)
            {
                return winrt::PropertyValue::CreateDateTime(
                    convert_value_to_native<winrt::Windows::Foundation::DateTime>(runtime, value));
            }
        }
    }

    return nullptr;
}
