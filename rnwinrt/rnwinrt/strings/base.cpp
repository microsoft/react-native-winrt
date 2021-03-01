
#if 1

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "base.h"

#include <Windows.h>

#include <combaseapi.h>
#include <winstring.h>

using namespace jswinrt;
using namespace std::literals;

namespace winrt
{
    using namespace Windows::Foundation;
}

jsi::String make_string(jsi::Runtime& runtime, std::wstring_view str)
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
            if (auto value = std::get<0>(itr->second).lock(runtime); !value.isUndefined())
            {
                return value;
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
        itr->remove(runtime, token);
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
                auto fn = jsi::Function::createFromHostFunction(runtime, id, 0, bind_host_function(add_event_listener));
                itr = m_functions.emplace(add_event_name, jsi::Value(runtime, std::move(fn))).first;
            }
            else if (name == remove_event_name)
            {
                auto fn =
                    jsi::Function::createFromHostFunction(runtime, id, 0, bind_host_function(remove_event_listener));
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

struct projected_property final : public jsi::HostObject
{
    projected_property(static_get_property_t getter, static_set_property_t setter) : m_getter(getter), m_setter(setter)
    {
    }

    // HostObject functions
    virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& id) override
    {
        auto name = id.utf8(runtime);
        if (name == "get")
        {
            if (m_getFn.isUndefined())
            {
                m_getFn = jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "get"), 0,
                    [getter = m_getter](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value*, size_t count) {
                        if (count != 0)
                        {
                            throw jsi::JSError(runtime, "TypeError: Property getter expects 0 arguments");
                        }

                        return getter(runtime);
                    });
            }

            return jsi::Value(runtime, m_getFn);
        }
        else if (name == "set")
        {
            if (m_setFn.isUndefined() && m_setter)
            {
                m_setFn = jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "set"), 1,
                    [setter = m_setter](
                        jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                        if (count != 1)
                        {
                            throw jsi::JSError(runtime, "TypeError: Property setter expects 1 argument");
                        }

                        setter(runtime, args[0]);
                        return jsi::Value::undefined();
                    });
            }

            return jsi::Value(runtime, m_setFn);
        }

        return jsi::Value::undefined();
    }

    virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value&) override
    {
        throw jsi::JSError(runtime, std::string("TypeError: Cannot assign to property '") + name.utf8(runtime) +
                                        "' of a projected property definition");
    }

    virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override
    {
        std::vector<jsi::PropNameID> result;
        if (m_setter)
        {
            result.reserve(2);
            result.push_back(make_propid(runtime, "set"));
        }

        result.push_back(make_propid(runtime, "get"));
        return result;
    }

private:
    static_get_property_t m_getter;
    jsi::Value m_getFn;
    static_set_property_t m_setter;
    jsi::Value m_setFn;
};

jsi::Value static_activatable_class_data::create(jsi::Runtime& runtime) const
{
    // TODO: param count? Seems to not matter?
    auto result = jsi::Function::createFromHostFunction(runtime, make_propid(runtime, name), 0, constructor);

    // JSI does not allow us to create a 'Function' that is also a 'HostObject' and therefore cannot provide virtual
    // get/set functions and instead must attach them to the function object
    auto defineProperty =
        runtime.global().getPropertyAsFunction(runtime, "Object").getPropertyAsFunction(runtime, "defineProperty");
    for (auto&& prop : properties)
    {
        defineProperty.call(runtime, result, make_string(runtime, prop.name),
            jsi::Value(runtime, jsi::Object::createFromHostObject(
                                    runtime, std::make_shared<projected_property>(prop.getter, prop.setter))));
    }

    for (auto&& fn : functions)
    {
        auto name = make_propid(runtime, fn.name);
        result.setProperty(runtime, name, jsi::Function::createFromHostFunction(runtime, name, 0, fn.function));
    }

    if (!events.empty())
    {
        // NOTE: We need shared state, but there's not really a good way to store that state on the function object that
        // we return... Use a unique_ptr here and give ownership to a single lambda. Neither should outlive the object
        // we return, so this should be okay
        auto evtArray = std::make_unique<event_registration_array>();

        auto name = make_propid(runtime, add_event_name);
        result.setProperty(runtime, name,
            jsi::Function::createFromHostFunction(runtime, name, 0,
                [this, evtArray = evtArray.get()](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args,
                    size_t count) { return static_add_event_listener(runtime, args, count, this, *evtArray); }));

        name = make_propid(runtime, remove_event_name);
        result.setProperty(runtime, name,
            jsi::Function::createFromHostFunction(runtime, name, 0,
                [this, evtArray = std::move(evtArray)](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args,
                    size_t count) { return static_remove_event_listener(runtime, args, count, this, *evtArray); }));
    }

    return result;
}

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
            begin = mid + 1;
        }
        else if (cmp > 0)
        {
            end = mid;
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

namespace jswinrt
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

        // TODO: Figure out a good SSO size
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
        return m_functions.emplace(functions[0]->name, jsi::Value(runtime, std::move(fn))).first->second;
    }
    else if (!functions.empty())
    {
        // TODO: Calculate max arity? Does it matter?
        auto fn = jsi::Function::createFromHostFunction(
            runtime, id, 0, projected_overloaded_function{ std::move(functions) });
        return m_functions.emplace(functions[0]->name, jsi::Value(runtime, std::move(fn))).first->second;
    }

    if (hasEvents)
    {
        if (name == add_event_name)
        {
            auto fn = jsi::Function::createFromHostFunction(runtime, id, 2, bind_host_function(add_event_listener));
            return m_functions.emplace(add_event_name, jsi::Value(runtime, std::move(fn))).first->second;
        }
        else if (name == remove_event_name)
        {
            auto fn = jsi::Function::createFromHostFunction(runtime, id, 2, bind_host_function(remove_event_listener));
            return m_functions.emplace(remove_event_name, jsi::Value(runtime, std::move(fn))).first->second;
        }
    }

    return jsi::Value::undefined();
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
            itr->remove(runtime, m_instance, args[1], token);
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
    }
    else if (str.size() != uuid_length)
    {
        throw jsi::JSError(runtime, "TypeError: Invalid GUID length");
    }

    winrt::guid result;
    if (::UuidFromStringA(reinterpret_cast<RPC_CSTR>(strBuffer), reinterpret_cast<UUID*>(winrt::put_abi(result))) !=
        RPC_S_OK)
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
    return convert_native_to_value<int64_t>(runtime, value.count());
}

winrt::TimeSpan projected_value_traits<winrt::TimeSpan>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
{
    std::chrono::milliseconds ms(convert_value_to_native<int64_t>(runtime, value));
    return std::chrono::duration_cast<winrt::TimeSpan>(ms);
}

jsi::Value convert_from_property_value(jsi::Runtime& runtime, const winrt::IPropertyValue& value)
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

winrt::IInspectable convert_to_property_value(jsi::Runtime& runtime, const jsi::Value& value)
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
                    // TODO
                    // return PropertyValue::CreateBooleanArray(convert_value_to_native<???>(runtime, value));
                }
                else if (elem.isNumber())
                {
                    // TODO
                    // return PropertyValue::CreateDoubleArray(convert_value_to_native<???>(runtime, value));
                }
                else if (elem.isString())
                {
                    // TODO
                    // return PropertyValue::CreateStringArray(convert_value_to_native<???>(runtime, value));
                }
                else if (elem.isObject())
                {
                    auto elemObj = elem.getObject(runtime);
                    if (elemObj.isHostObject(runtime))
                    {
                        // TODO
                        // return PropertyValue::CreateInspectableArray(convert_value_to_native<???>(runtime, value));
                    }

                    auto isPointLike = elemObj.hasProperty(runtime, "x") && elemObj.hasProperty(runtime, "y");
                    auto isSizeLike = elemObj.hasProperty(runtime, "width") && elemObj.hasProperty(runtime, "height");
                    if (isPointLike && isSizeLike)
                    {
                        // TODO
                        // return PropertyValue::CreateRectArray(convert_value_to_native<???>(runtime, value));
                    }
                    else if (isPointLike)
                    {
                        // TODO
                        // return PropertyValue::CreatePointArray(convert_value_to_native<???>(runtime, value));
                    }
                    else if (isSizeLike)
                    {
                        // TODO
                        // return PropertyValue::CreateSizeArray(convert_value_to_native<???>(runtime, value));
                    }

                    // TODO: DateTimeArray?
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

            // TODO: DateTime?
        }
    }

    return nullptr;
}

#else

#include <rnwinrt/ProjectedValueConverters.g.h>
#include <rnwinrt/Projections.g.h>

namespace WinRTTurboModule
{
    jsi::Value CreateError(jsi::Runtime& runtime, std::string_view message)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "message", CreateString(runtime, message));
        return jsi::Value(runtime, result);
    }

    jsi::Value CreateError(jsi::Runtime& runtime, const std::exception& exception)
    {
        return CreateError(runtime, exception.what());
    }

    jsi::Value CreateError(jsi::Runtime& runtime, const winrt::hresult_error& error)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "message", CreateString(runtime, error.message()));
        result.setProperty(runtime, "number", jsi::Value(static_cast<int32_t>(error.code())));
        return jsi::Value(runtime, result);
    }

    jsi::Value PromiseWrapper::Create(jsi::Runtime& runtime, std::shared_ptr<PromiseWrapper>& wrapper)
    {
        wrapper = {};

        jsi::Function callback =
            jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, "fn"), 2,
                [&wrapper](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    FAIL_FAST_IF(count < 2);
                    wrapper = std::make_shared<PromiseWrapper>(
                        args[0].asObject(runtime).asFunction(runtime), args[1].asObject(runtime).asFunction(runtime));
                    return jsi::Value::undefined();
                });

        jsi::Function s_promiseConstructor = runtime.global().getPropertyAsFunction(runtime, "Promise");
        return s_promiseConstructor.callAsConstructor(runtime, callback);
    }

    PromiseWrapper::PromiseWrapper(jsi::Function resolve, jsi::Function reject) :
        m_resolve(std::move(resolve)), m_reject(std::move(reject))
    {
    }

    void PromiseWrapper::Resolve(jsi::Runtime& runtime, const jsi::Value& result)
    {
        FAIL_FAST_IF(m_completed);
        m_completed = true;
        m_resolve.call(runtime, result);
    }

    void PromiseWrapper::Reject(jsi::Runtime& runtime, const jsi::Value& result)
    {
        FAIL_FAST_IF(m_completed);
        m_completed = true;
        m_reject.call(runtime, result);
    }

    CallInvokerWrapper::CallInvokerWrapper(std::shared_ptr<react::CallInvoker> callInvokerWrapper) :
        m_callInvokerWrapper(std::move(callInvokerWrapper)), ThreadId(GetCurrentThreadId())
    {
    }

    void CallInvokerWrapper::Call(std::function<void()> func)
    {
        if (ThreadId == GetCurrentThreadId())
        {
            func();
        }
        else
        {
            m_callInvokerWrapper->invokeAsync(std::move(func));
        }
    }

    void CallInvokerWrapper::CallAsync(std::function<void()> func)
    {
        m_callInvokerWrapper->invokeAsync(std::move(func));
    }

    void CallInvokerWrapper::CallSync(std::function<void()> func)
    {
        if (ThreadId == GetCurrentThreadId())
        {
            func();
        }
        else
        {
            // TODO: Use COM mechanism when crossing thread boundaries in order to propagate errors more cleanly.
            wil::shared_event event;
            event.create();

            m_callInvokerWrapper->invokeAsync([event, func{ std::move(func) }]() {
                func();
                event.SetEvent();
            });

            event.wait();
        }
    }
}

namespace WinRTTurboModule
{
    ProjectedHostObjectProvider::ProjectedHostObjectProvider(
        std::string_view name, std::shared_ptr<jsi::HostObject> hostObject) :
        m_name(name),
        m_hostObject(std::move(hostObject))
    {
    }

    std::string_view ProjectedHostObjectProvider::Name() const
    {
        return m_name;
    }

    jsi::Value ProjectedHostObjectProvider::Value(jsi::Runtime& runtime)
    {
        if (!m_object)
        {
            m_object = jsi::Object::createFromHostObject(runtime, m_hostObject);
        }
        return jsi::Value(runtime, *m_object);
    }
}

namespace WinRTTurboModule
{
    ProjectedRuntimeClassInstanceFactory::ProjectedRuntimeClassInstanceFactory(ProjectionsContext& context) :
        m_context(context)
    {
    }

    jsi::Value ProjectedRuntimeClassInstanceFactory::Get(const winrt::Windows::Foundation::IInspectable& instance)
    {
        WI_ASSERT(m_context.Invoker->ThreadId == ::GetCurrentThreadId());

        // It is a not infrequent scenario where the same WinRT object will be accessed multiple times. To avoid
        // aliasing it in the wrapper HostObject multiple times, we should return a strong reference to the object to
        // the caller but hold a weak ref in case it accessed again for the JS garbage collector releases the Object and
        // its underlying HostObject.

        // Despite the use of a cache it is possible to have a cache miss when keying off of the void* pointer for
        // IInspectable because IInspectable could point to a different v-table entry since all WinRT interfaces inherit
        // IInspectable. It is also possible the class does something atypical like have QueryInterface return an
        // entirely different implementation depending on the interface.

        const auto key = winrt::get_abi(instance);
        if (const auto it = m_instanceCache.find(key); it != m_instanceCache.end())
        {
            if (m_weakObjectSupported)
            {
                if (auto value = std::get<jsi::WeakObject>(it->second).lock(m_context.Runtime); !value.isUndefined())
                {
                    return value;
                }
            }
            else if (auto hostObject = std::get<std::weak_ptr<jsi::HostObject>>(it->second).lock())
            {
                return jsi::Value(
                    m_context.Runtime, jsi::Object::createFromHostObject(m_context.Runtime, std::move(hostObject)));
            }
            m_instanceCache.erase(it);
        }

        auto hostObject =
            std::shared_ptr<jsi::HostObject>(new ProjectedRuntimeClassInstance(m_context.shared_from_this(), instance));
        const auto object = jsi::Object::createFromHostObject(m_context.Runtime, hostObject);

        if (m_weakObjectSupported)
        {
            try
            {
                m_instanceCache.emplace(key, jsi::WeakObject(m_context.Runtime, object));
            }
            catch (std::logic_error&)
            {
                // TODO: Looks like we are using V8 which doesn't support WeakObject per
                //       V8Runtime::createWeakObject/V8Runtime::lockWeakObject in
                //       https://github.com/microsoft/v8-jsi/blob/master/src/V8JsiRuntime.cpp so until it is fixed, we
                //       should be more careful to avoid aliasing and doing extra work (e.g. requesting a property
                //       multiple times) or come up with some sort of scheme like holding strong refs temporarily.
                m_weakObjectSupported = false;
            }
        }

        // Do not combine as an else condition as we want to cache the first time when m_weakObjectSupported -> false.
        if (!m_weakObjectSupported)
        {
            m_instanceCache.emplace(key, std::move(hostObject));
        }

        return jsi::Value(m_context.Runtime, object);
    }

    bool ProjectedRuntimeClassInstanceFactory::Remove(const winrt::Windows::Foundation::IInspectable& instance)
    {
        WI_ASSERT(m_context.Invoker->ThreadId == ::GetCurrentThreadId());
        return m_instanceCache.erase(winrt::get_abi(instance));
    }
}

namespace WinRTTurboModule
{
    ProjectedEventRegistrar::ProjectedEventRegistrar(ProjectionsContext& context) : m_context(context)
    {
    }

    std::shared_ptr<ProjectedEventInstance> ProjectedEventRegistrar::Get(
        const winrt::Windows::Foundation::IInspectable& inspectable, const std::shared_ptr<IProjectedEventBase>& event)
    {
        WI_ASSERT(m_context.Invoker->ThreadId == ::GetCurrentThreadId());

        // Do an opportunistic cleanup to prevent unbounded growth of the map with dead objects over time.
        CleanupMapIfApplicable();

        const auto emplaceResult = m_map.emplace(winrt::get_abi(inspectable), Entry{});
        auto& entry = emplaceResult.first->second;
        if (emplaceResult.second)
        {
            entry.WeakRef = inspectable;
        }

        // Most objects would have a small number of event handlers, thus to reduce memory usage and lookup times we can
        // just use plain a std::vector with O(n) lookup.
        const auto it = std::find_if(entry.EventInstances.begin(), entry.EventInstances.end(),
            [&event](const std::shared_ptr<ProjectedEventInstance>& instance) { return instance->Event() == event; });

        if (it != entry.EventInstances.end())
        {
            return *it;
        }

        auto instance = std::make_shared<ProjectedEventInstance>(event);
        entry.EventInstances.push_back(instance);
        return instance;
    }

    void ProjectedEventRegistrar::CleanupMapIfApplicable()
    {
        constexpr uint64_t c_cleanupInterval = 5 * 60 * 1000; // ticks (ms) since last cleanup attempted.
        constexpr uint32_t c_cleanupThreshold = 100; // number of calls since last cleanup attempted.

        if (++m_requestsSinceCleanup >= c_cleanupThreshold)
        {
            if (const auto ticks = GetTickCount64(); ticks - m_lastCleanUp > c_cleanupInterval)
            {
                m_requestsSinceCleanup = 0;
                m_lastCleanUp = GetTickCount64();

                auto it = m_map.begin();
                while (it != m_map.end())
                {
                    if (it->second.WeakRef)
                    {
                        ++it;
                    }
                    else
                    {
                        it = m_map.erase(it);
                    }
                }
            }
        }
    }
}

namespace WinRTTurboModule
{
    std::map<DWORD, std::weak_ptr<ProjectionsContext>> ProjectionsContext::s_instanceMap;
    wil::srwlock ProjectionsContext::s_lock;

    std::shared_ptr<ProjectionsContext> ProjectionsContext::Create(
        jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker)
    {
        auto context = std::make_shared<ProjectionsContext>(runtime, std::move(invoker));

        const auto lock = s_lock.lock_exclusive();
        s_instanceMap.emplace(::GetCurrentThreadId(), context);

        return context;
    }

    std::shared_ptr<ProjectionsContext> ProjectionsContext::TryGetForCurrentThread()
    {
        const auto lock = s_lock.lock_shared();
        if (const auto it = s_instanceMap.find(::GetCurrentThreadId()); it != s_instanceMap.end())
        {
            if (auto context = it->second.lock())
            {
                return context;
            }
        }
        return nullptr;
    }

    ProjectionsContext::ProjectionsContext(jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker) :
        Runtime(runtime), Invoker(std::make_shared<CallInvokerWrapper>(std::move(invoker))), InstanceFactory(*this),
        EventRegistrar(*this)
    {
    }

    ProjectionsContext::~ProjectionsContext()
    {
        const auto lock = s_lock.lock_exclusive();
        s_instanceMap.erase(::GetCurrentThreadId());
    }
}

namespace WinRTTurboModule
{
    std::shared_ptr<IProjectedValueProvider> ProjectedNamespace::Create(
        std::string_view name, std::shared_ptr<ProjectionsContext> context, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedHostObjectProvider>(
            name, std::make_shared<ProjectedNamespace>(name, std::move(context), initializer));
    }

    ProjectedNamespace::ProjectedNamespace(
        std::string_view name, std::shared_ptr<ProjectionsContext> context, InitializerFunction initializer) :
        m_name(name),
        m_context(std::move(context)), m_initializer(initializer)
    {
    }

    jsi::Value ProjectedNamespace::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        const auto it = m_propertyMap.find(propName.utf8(runtime));
        if (it != m_propertyMap.end())
        {
            return it->second->Value(runtime);
        }

        return jsi::Value::undefined();
    }

    void ProjectedNamespace::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&)
    {
        throw jsi::JSError(runtime,
            "TypeError: Cannot assign to property '" + propName.utf8(runtime) + "' to a projected WinRT namespace");
    }

    std::vector<jsi::PropNameID> ProjectedNamespace::getPropertyNames(jsi::Runtime& runtime)
    {
        EnsureInitialized();

        // PERF: It would be nice if these were copyable but PropNameID is only moveable. It might be possible to store
        // and reuse jsi::String, which might avoid deep copies. Could we return an empty vector like TurboModule given
        // it inherits it from HostObject?
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(m_propertyMap.size());

        for (const auto& property : m_propertyMap)
        {
            propNames.push_back(CreatePropNameID(runtime, property.first));
        }

        return propNames;
    }

    void ProjectedNamespace::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto providers = m_initializer(m_context);
            m_propertyMap.reserve(providers.size());
            for (auto&& provider : providers)
            {
                m_propertyMap[provider->Name()] = std::move(provider);
            }
            m_isInitialized = true;
        }
    }
}

namespace WinRTTurboModule
{
    std::shared_ptr<IProjectedValueProvider> ProjectedEnum::Create(
        std::string_view name, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedHostObjectProvider>(name, std::make_shared<ProjectedEnum>(name, initializer));
    }

    ProjectedEnum::ProjectedEnum(std::string_view name, InitializerFunction initializer) :
        m_name(name), m_initializer(initializer)
    {
    }

    jsi::Value ProjectedEnum::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        const auto it = m_propertyMap.find(propName.utf8(runtime));
        if (it != m_propertyMap.end())
        {
            // jsi::Value uses double for int and float constructors and there doesn't appear to be BigInt support yet.
            return jsi::Value(static_cast<double>(it->second));
        }

        return jsi::Value::undefined();
    }

    void ProjectedEnum::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&)
    {
        throw jsi::JSError(runtime, std::string("TypeError: Cannot assign to property '") + propName.utf8(runtime) +
                                        "' to a projected WinRT enum");
    }

    std::vector<jsi::PropNameID> ProjectedEnum::getPropertyNames(jsi::Runtime& runtime)
    {
        // PERF: It would be nice if these were copyable but PropNameID is only moveable. It might be possible to store
        // and reuse jsi::String, which might avoid deep copies. Could we return an empty vector like TurboModule given
        // it inherits it from HostObject?
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(m_propertyMap.size());

        for (const auto& property : m_propertyMap)
        {
            propNames.push_back(CreatePropNameID(runtime, property.first));
        }

        return propNames;
    }

    void ProjectedEnum::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            m_propertyMap = m_initializer();
            m_isInitialized = true;
        }
    }
}

namespace WinRTTurboModule
{
    // Special helper for use with Object.defineProperty
    class ProjectedPropertyDefinition final : public jsi::HostObject
    {
    public:
        ProjectedPropertyDefinition(std::shared_ptr<IProjectedInterfaceInstance> interfaceInstance,
            std::shared_ptr<IProjectedPropertyBase> property) :
            m_interfaceInstance(std::move(interfaceInstance)),
            m_property(std::move(property))
        {
        }

        virtual ~ProjectedPropertyDefinition() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override
        {
            const auto propNameUtf8 = propName.utf8(runtime);

            if (!propNameUtf8.compare("get"))
            {
                if (!m_get)
                {
                    m_get = jsi::Function::createFromHostFunction(runtime, propName, 0,
                        [interfaceInstance{ m_interfaceInstance }, property{ m_property }](jsi::Runtime& runtime,
                            const jsi::Value&, const jsi::Value*,
                            size_t) mutable { return interfaceInstance->GetProperty(runtime, property); });
                }
                return jsi::Value(runtime, m_get.value());
            }

            if (!m_property->IsReadonly() && !propNameUtf8.compare("set"))
            {
                if (!m_set)
                {
                    m_set = jsi::Function::createFromHostFunction(runtime, propName, 1,
                        [interfaceInstance{ m_interfaceInstance }, property{ m_property }](
                            jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) mutable {
                            if (count != 1)
                            {
                                throw jsi::JSError(runtime, "TypeError: Property setter expects 1 argument");
                            }
                            interfaceInstance->SetProperty(runtime, property, args[0]);
                            return jsi::Value::undefined();
                        });
                }
                return jsi::Value(runtime, m_set.value());
            }

            return jsi::Value::undefined();
        }

        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&) override
        {
            throw jsi::JSError(runtime, std::string("TypeError: Cannot assign to property '") + propName.utf8(runtime) +
                                            "' to a projected property definition");
        }

        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override
        {
            std::vector<jsi::PropNameID> propNames;
            propNames.push_back(jsi::PropNameID::forAscii(runtime, "get"));

            if (!m_property->IsReadonly())
            {
                propNames.push_back(jsi::PropNameID::forAscii(runtime, "set"));
            }

            return propNames;
        }

    private:
        const std::shared_ptr<IProjectedInterfaceInstance> m_interfaceInstance;
        const std::shared_ptr<IProjectedPropertyBase> m_property;
        std::optional<jsi::Function> m_get;
        std::optional<jsi::Function> m_set;
    };

    ProjectedActivationFactory::ProjectedActivationFactory(std::string_view name,
        std::shared_ptr<ProjectionsContext> context, GetActivationFactory getActivationFactory,
        std::set<winrt::guid> factoryInterfaceIds) :
        m_name(name),
        m_context(std::move(context)), m_getActivationFactory(getActivationFactory),
        m_factoryInterfaceIds(std::move(factoryInterfaceIds))
    {
    }

    std::string_view ProjectedActivationFactory::Name() const
    {
        return m_name;
    }

    jsi::Value ProjectedActivationFactory::Value(jsi::Runtime& runtime)
    {
        if (!m_object)
        {
            // Create it directly rather than using "ProjectedRuntimeClassInstance::Get" as the factories/statics do not
            // need to be added to the lookup map given that they are strongly referenced singletons like namespaces.
            auto projectedRuntimeClassInstance =
                std::make_shared<ProjectedRuntimeClassInstance>(m_context, m_getActivationFactory());

            if (m_factoryInterfaceIds.empty())
            {
                // PERF 1: We could theoretically defer construction of the factory until calls to
                //         ProjectedRuntimeClassInstance require it, but for simplicity we can do so now. It migjt
                //         theoretically be possible to also prepopulate IIDs to save on calling GetIIDs if we also had
                //         code generation list the statics.

                // PERF 2: We are intentionally holding a strong ref to it rather than using the caching mechanism used
                //         with instances (see Get(instance)) given that this object is a factory. Arguably weak-ref
                //         based caching would be more memory efficient as the JS GC free references to HostObjects, so
                //         we could switch if memory is more important than CPU usage.

                m_object = jsi::Object::createFromHostObject(runtime, std::move(projectedRuntimeClassInstance));
            }
            else
            {
                // It's not possible with JSI to construct as a function object from both a native function and a
                // HostObject. WinRT runtimeclasses support both constructors and static methods and we need the object
                // to be a function so that it can be called like a constructor, but the static methods need to be
                // treated as properties on the function. Since we can use IHostObject::get for this purpose, they need
                // to be added onto the JSI object itself.
                const auto& interfaceInstances = projectedRuntimeClassInstance->Interfaces();

                std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances;
                std::vector<std::shared_ptr<IProjectedInterfaceInstance>> staticInterfaceInstances;
                for (const auto& interfaceInstance : interfaceInstances)
                {
                    if (m_factoryInterfaceIds.find(interfaceInstance->Interface().InterfaceId()) ==
                        m_factoryInterfaceIds.end())
                    {
                        staticInterfaceInstances.push_back(interfaceInstance);
                    }
                    else
                    {
                        factoryInterfaceInstances.push_back(interfaceInstance);
                    }
                }

                // Create a function that multiplexes all methods on all factory interfaces as C++/WinRT generates
                // interfaces for factories using the ABI names rather than overloading "CreateInstance".
                m_object =
                    CreateCompositeConstructor(runtime, std::string(m_name), std::move(factoryInterfaceInstances));

                // Exclude "factoryInterfaceInstances" from being injected as properties as there is no point in adding
                // the constructors again using their ABI names given that we are doing all of this to support
                // constructors!
                const auto objectClass = runtime.global().getPropertyAsFunction(runtime, "Object");
                std::optional<jsi::Function> defineProperty;

                std::map<std::string_view, std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>,
                                               std::shared_ptr<ProjectedFunction>>>>
                    methodMap;
                for (auto& staticInterfaceInstance : staticInterfaceInstances)
                {
                    if (const auto& properties = staticInterfaceInstance->Interface().Properties(); !properties.empty())
                    {
                        if (!defineProperty)
                        {
                            defineProperty = objectClass.getPropertyAsFunction(runtime, "defineProperty");
                        }

                        for (auto&& property : properties)
                        {
                            defineProperty->call(runtime, *m_object, CreateString(runtime, property.first),
                                jsi::Value(runtime, jsi::Object::createFromHostObject(
                                                        runtime, std::make_shared<ProjectedPropertyDefinition>(
                                                                     staticInterfaceInstance, property.second))));
                        }
                    }

                    // It is possible that multiple interfaces implement the same method rather than just having
                    // overloads for that method on the same interface. So unlike properties, we need to multiplex the
                    // methods to ensure we call the right overload.
                    for (const auto& method : staticInterfaceInstance->Interface().Methods())
                    {
                        methodMap[method.first].emplace_back(staticInterfaceInstance, method.second);
                    }
                }

                for (auto&& method : methodMap)
                {
                    m_object->setProperty(runtime, CreateString(runtime, method.first),
                        CreateCompositeMethod(runtime, std::string(method.first), std::move(method.second)));
                }

                if (auto addEventListener =
                        projectedRuntimeClassInstance->GetEventListenerFunction(runtime, true /*isAddEventListener*/))
                {
                    m_object->setProperty(runtime, "addEventListener", std::move(*addEventListener));

                    auto removeEventListener =
                        projectedRuntimeClassInstance->GetEventListenerFunction(runtime, false /*isAddEventListener*/);
                    m_object->setProperty(runtime, "removeEventListener", std::move(*removeEventListener));
                }

                objectClass.getPropertyAsFunction(runtime, "seal").call(runtime, *m_object);
            }
        }
        return jsi::Value(runtime, *m_object);
    }
}

namespace WinRTTurboModule
{
    std::shared_ptr<ProjectedFunction> ProjectedFunction::Create(std::string_view name, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedFunction>(name, initializer);
    }

    std::shared_ptr<ProjectedFunction> ProjectedFunction::Create(
        std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation)
    {
        return std::make_shared<ProjectedFunction>(name, std::move(implementation));
    }

    ProjectedFunction::ProjectedFunction(std::string_view name, InitializerFunction initializer) :
        m_name(name), m_initializer(initializer)
    {
    }

    ProjectedFunction::ProjectedFunction(
        std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation) :
        m_name(name),
        m_initializer(nullptr), m_maxArity(implementation->Arity()), m_isInitialized(true)
    {
        m_overloads.emplace(implementation->Arity(), std::move(implementation));
    }

    std::string_view ProjectedFunction::Name() const noexcept
    {
        return m_name;
    }

    bool ProjectedFunction::CanInvoke(uint16_t arity)
    {
        EnsureInitialized();
        return m_overloads.find(arity) != m_overloads.end();
    }

    std::shared_ptr<IProjectedFunctionOverloadBase>& ProjectedFunction::GetOverload(
        jsi::Runtime& runtime, uint16_t numArgs)
    {
        EnsureInitialized();
        const auto it = m_overloads.find(numArgs);
        if (it == m_overloads.end())
        {
            throw jsi::JSError(runtime, "TypeError: No function overload for " + std::string(m_name) + " exists with " +
                                            std::to_string(numArgs) + " args");
        }
        return it->second;
    }

    void ProjectedFunction::EnsureInitialized()
    {
        if (m_overloads.empty())
        {
            // PERF: Consider optimizing the trivial case of a single declaration with no overloads.
            auto overloads = m_initializer();
            for (auto&& overload : overloads)
            {
                const auto arity = overload->Arity();
                m_maxArity = std::max(m_maxArity, arity);

                // JS only supports overloading by the number of function params so code generation should either
                // have omitted all ambiguous functions or picked one to use. If not we will take the last one.
                WI_ASSERT(m_overloads.find(arity) == m_overloads.end());
                m_overloads[arity] = std::move(overload);
            }
            m_isInitialized = true;
        }
    }
}

namespace WinRTTurboModule
{
    ProjectedEventInstance::ProjectedEventInstance(std::shared_ptr<IProjectedEventBase> event) :
        m_event(std::move(event))
    {
    }

    std::vector<std::pair<jsi::Object, winrt::event_token>>::iterator ProjectedEventInstance::FindEntry(
        jsi::Runtime& runtime, const jsi::Object& object)
    {
        return std::find_if(
            m_listeners.begin(), m_listeners.end(), [&](const std::pair<jsi::Object, winrt::event_token>& entry) {
                return jsi::Object::strictEquals(runtime, entry.first, object);
            });
    }

    bool ProjectedEventInstance::HasEntry(jsi::Runtime& runtime, const jsi::Object& object)
    {
        return FindEntry(runtime, object) != m_listeners.end();
    }

    void ProjectedEventInstance::AddEntry(jsi::Object object, winrt::event_token token)
    {
        m_listeners.emplace_back(std::move(object), token);
    }

    std::optional<std::pair<jsi::Object, winrt::event_token>> ProjectedEventInstance::RemoveEntry(
        jsi::Runtime& runtime, const jsi::Object& object)
    {
        if (const auto it = FindEntry(runtime, object); it != m_listeners.end())
        {
            auto token = std::move(*it);
            m_listeners.erase(it);
            return std::move(token);
        }
        return {};
    }
}

namespace WinRTTurboModule
{
    std::unordered_map<winrt::guid, std::shared_ptr<ProjectedInterface>> ProjectedInterface::s_interfaceMap;

    std::shared_ptr<ProjectedInterface> ProjectedInterface::Get(const winrt::guid& iid)
    {
        EnsureInterfaceMap();
        const auto it = s_interfaceMap.find(iid);
        return (it == s_interfaceMap.end()) ? nullptr : it->second;
    }

    ProjectedInterface::ProjectedInterface(
        const winrt::guid& iid, InstanceFactory factory, InitializerFunction initializer) :
        m_iid(iid),
        m_factory(factory), m_initializer(initializer)
    {
    }

    const winrt::guid& ProjectedInterface::InterfaceId() const noexcept
    {
        return m_iid;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(std::string_view name)
    {
        const auto methods = Methods();
        const auto it = methods.find(name);
        return it == methods.end() ? nullptr : it->second;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(std::string_view name, uint16_t numArgs)
    {
        auto method = FindMethod(name);
        return method->CanInvoke(numArgs) ? method : nullptr;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(uint16_t numArgs)
    {
        for (const auto& method : Methods())
        {
            if (method.second->CanInvoke(numArgs))
            {
                return method.second;
            }
        }
        return nullptr;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<ProjectedFunction>>& ProjectedInterface::Methods()
    {
        EnsureInitialized();
        return m_methodMap;
    }

    std::shared_ptr<IProjectedPropertyBase> ProjectedInterface::FindProperty(std::string_view name)
    {
        const auto properties = Properties();
        const auto it = properties.find(name);
        return it == properties.end() ? nullptr : it->second;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<IProjectedPropertyBase>>&
    ProjectedInterface::Properties()
    {
        EnsureInitialized();
        return m_propertyMap;
    }

    std::shared_ptr<IProjectedEventBase> ProjectedInterface::FindEvent(std::string_view name)
    {
        const auto events = Events();
        const auto it = events.find(name);
        return it == events.end() ? nullptr : it->second;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<IProjectedEventBase>>& ProjectedInterface::Events()
    {
        EnsureInitialized();
        return m_eventMap;
    }

    std::shared_ptr<IProjectedInterfaceInstance> ProjectedInterface::CreateInstance(
        std::shared_ptr<ProjectionsContext> context, winrt::Windows::Foundation::IInspectable instance)
    {
        return m_factory(std::move(context), std::move(instance), shared_from_this());
    }

    void ProjectedInterface::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto data = m_initializer();

            m_propertyMap.reserve(data.Properties.size());
            for (auto&& property : data.Properties)
            {
                m_propertyMap[property->Name()] = std::move(property);
            }

            m_methodMap.reserve(data.Methods.size());
            for (auto&& method : data.Methods)
            {
                m_methodMap[method->Name()] = std::move(method);
            }

            m_eventMap.reserve(data.Events.size());
            for (auto&& event : data.Events)
            {
                m_eventMap[event->Name()] = std::move(event);
            }

            m_isInitialized = true;
        }
    }

    namespace
    {
        ProjectedInterfaceData InitIActivationFactoryInterface()
        {
            return { {},
                { ProjectedFunction::Create("activateInstance"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::IActivationFactory>(
                        &winrt::Windows::Foundation::IActivationFactory::ActivateInstance<
                            winrt::Windows::Foundation::IInspectable>,
                        ConvertInterfaceToValue<winrt::Windows::Foundation::IInspectable>)) },
                {} };
        }

        // TODO: For the sake of Windows.Storage.Streams.IBufferAccess, IBufferByteAccess should be projected (unlike
        // Chakra) which would make IRandomAccessStream far more usable with JS.
    }

    void ProjectedInterface::EnsureInterfaceMap()
    {
        if (s_interfaceMap.empty())
        {
            auto projectedInterfaces = GetProjectedInterfaces();
            s_interfaceMap.reserve(projectedInterfaces.size() + 1);

            // IActivationFactory is a Classic COM interface and cannot be generated from a winmd,
            s_interfaceMap[__uuidof(::IActivationFactory)] =
                ProjectedInterface::Create<winrt::Windows::Foundation::IActivationFactory>(
                    InitIActivationFactoryInterface);
            for (auto&& projectedInterface : projectedInterfaces)
            {
                s_interfaceMap[projectedInterface->InterfaceId()] = std::move(projectedInterface);
            }
        }
    }
}

namespace WinRTTurboModule
{
    jsi::Value ProjectedRuntimeClassInstance::Get(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IInspectable& instance)
    {
        return context->InstanceFactory.Get(instance);
    }

    ProjectedRuntimeClassInstance::ProjectedRuntimeClassInstance(
        std::shared_ptr<ProjectionsContext> context, winrt::Windows::Foundation::IInspectable instance) :
        m_context(std::move(context)),
        m_instance(std::move(instance))
    {
    }

    ProjectedRuntimeClassInstance::~ProjectedRuntimeClassInstance()
    {
        m_context->InstanceFactory.Remove(m_instance);
    }

    jsi::Value ProjectedRuntimeClassInstance::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        auto propNameUtf8 = propName.utf8(runtime);

        for (uint32_t i = 0; i < m_interfaceInstances.size(); ++i)
        {
            const auto& interfaceInstance = m_interfaceInstances[i];
            auto& interfaceDef = interfaceInstance->Interface();

            if (const auto property = interfaceDef.FindProperty(propNameUtf8))
            {
                return interfaceInstance->GetProperty(runtime, property);
            }

            // PERF: Would it make sense to cache unique and composite methods after resolution?
            if (auto method = interfaceDef.FindMethod(propNameUtf8))
            {
                // At least one interface has a method with the specified name, but it is possible that an overload with
                // the same name was added in a later instance of the interface so we need to multiplex them as a single
                // function.
                std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>>
                    interfaceMethods;
                for (++i; i < m_interfaceInstances.size(); ++i)
                {
                    const auto& overloadInterfaceInstance = m_interfaceInstances[i];
                    if (auto overloadMethod = overloadInterfaceInstance->Interface().FindMethod(propNameUtf8))
                    {
                        interfaceMethods.emplace_back(overloadInterfaceInstance, std::move(overloadMethod));
                    }
                }

                if (interfaceMethods.empty())
                {
                    return interfaceInstance->GetMethod(runtime, method);
                }

                interfaceMethods.emplace_back(interfaceInstance, std::move(method));
                return jsi::Value(
                    runtime, CreateCompositeMethod(runtime, std::move(propNameUtf8), std::move(interfaceMethods)));
            }
        }

        // TODO: Is it necessary to support Chakra's treatment of events like properties in addition to
        //       add/removeEventListener. It seems like a poor pattern to carry forward given that it only allows for a
        //       single handler for an event.

        const bool isAddEventListener = (propNameUtf8 == "addEventListener"sv);
        if (isAddEventListener || (propNameUtf8 == "removeEventListener"sv))
        {
            if (auto function = GetEventListenerFunction(runtime, isAddEventListener))
            {
                return std::move(*function);
            }
        }

        return jsi::Value::undefined();
    }

    void ProjectedRuntimeClassInstance::set(
        jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value)
    {
        EnsureInitialized();

        const auto propNameUtf8 = propName.utf8(runtime);

        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            if (const auto property = interfaceInstance->Interface().FindProperty(propNameUtf8);
                property && !property->IsReadonly())
            {
                interfaceInstance->SetProperty(runtime, property, value);
                break;
            }
        }

        // If no property exists with the given name, then ignore the call rather than throwing. This is more-or-less
        // consistent with EdgeHTML WebView.
    }

    std::vector<jsi::PropNameID> ProjectedRuntimeClassInstance::getPropertyNames(jsi::Runtime& runtime)
    {
        EnsureInitialized();

        std::vector<jsi::PropNameID> propNames;
        bool hasEvents = false;
        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            auto& interfaceDef = interfaceInstance->Interface();
            const auto& methods = interfaceDef.Methods();
            const auto& properties = interfaceDef.Properties();

            propNames.reserve(propNames.size() + methods.size() + properties.size());

            // TODO: Do we care if there are duplicates for methods due to overloading interfaces?
            for (const auto& method : methods)
            {
                propNames.push_back(CreatePropNameID(runtime, method.first));
            }

            for (const auto& property : properties)
            {
                propNames.push_back(CreatePropNameID(runtime, property.first));
            }

            if (!hasEvents && !interfaceDef.Events().empty())
            {
                hasEvents = true;
            }
        }

        if (hasEvents)
        {
            propNames.reserve(propNames.size() + 2);
            propNames.push_back(CreatePropNameID(runtime, "addEventListener"sv));
            propNames.push_back(CreatePropNameID(runtime, "removeEventListener"sv));
        }

        return propNames;
    }

    std::optional<jsi::Value> ProjectedRuntimeClassInstance::GetEventListenerFunction(
        jsi::Runtime& runtime, bool isAddEventListener)
    {
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> interfaceInstancesWithEvents;
        interfaceInstancesWithEvents.reserve(m_interfaceInstances.size());
        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            if (!interfaceInstance->Interface().Events().empty())
            {
                interfaceInstancesWithEvents.push_back(interfaceInstance);
            }
        }

        if (!interfaceInstancesWithEvents.empty())
        {
            // PERF: Would it make sense to cache event listener functions?
            return jsi::Value(runtime,
                jsi::Function::createFromHostFunction(runtime,
                    CreatePropNameID(runtime, isAddEventListener ? "addEventListener"sv : "removeEventListener"sv),
                    2 /*length*/,
                    [isAddEventListener, interfaceInstances{ std::move(interfaceInstancesWithEvents) }](
                        jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t numArgs) {
                        if (numArgs < 2)
                        {
                            throw jsi::JSError(
                                runtime, "TypeError: add/removeEventListener expect (at least) 2 arguments");
                        }

                        const auto eventName = args[0].asString(runtime).utf8(runtime);

                        for (auto& interfaceInstance : interfaceInstances)
                        {
                            if (const auto event = interfaceInstance->Interface().FindEvent(eventName))
                            {
                                if (isAddEventListener)
                                {
                                    interfaceInstance->AddEventListener(runtime, event, args[1]);
                                }
                                else
                                {
                                    interfaceInstance->RemoveEventListener(runtime, event, args[1]);
                                }
                            }
                        }

                        return jsi::Value::undefined();
                    }));
        }

        return {};
    }

    void ProjectedRuntimeClassInstance::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto iids = winrt::get_interfaces(m_instance);
            m_interfaceInstances.reserve(iids.size());

            for (const auto& iid : iids)
            {
                if (const auto projectedInterface = ProjectedInterface::Get(iid))
                {
                    m_interfaceInstances.push_back(projectedInterface->CreateInstance(m_context, m_instance));
                }
            }

            m_isInitialized = true;
        }
    }

    jsi::Function CreateCompositeConstructor(jsi::Runtime& runtime, std::string className,
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances)
    {
        // TODO: Does it matter that the function name is arbitrarily set to the runtimeclass name or that the function
        //       length is set to 0. The name is probably better as the class name than the ABI factory method name
        //       which could vary, and length is possible to compute as the maximum of all "overloads" but it doesn't
        //       seem like it would be worthwhile.
        auto id = CreatePropNameID(runtime, className);
        return jsi::Function::createFromHostFunction(runtime, std::move(id), 0 /*length*/,
            [className{ std::move(className) }, factoryInterfaceInstances{ std::move(factoryInterfaceInstances) }](
                jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t numArgs) {
                // C++/WinRT generates factory interfaces so the constructor overloads actually have different names
                // rather than relying on arity to differentiate them. Conveivably, there might be multiple factory
                // interfaces and/or the special ClassicCOM IActivationFactory for the zero-param case. A poorly written
                // API might also have multiple overloads with the same arity as it didn't consider JS. Thus we use
                // TryInvokeAnyMethod rather than TryInvokeMethod.

                for (auto& factoryInterfaceInstance : factoryInterfaceInstances)
                {
                    if (const auto method =
                            factoryInterfaceInstance->Interface().FindMethod(static_cast<uint16_t>(numArgs)))
                    {
                        return factoryInterfaceInstance->InvokeMethod(
                            runtime, method, args, static_cast<uint16_t>(numArgs));
                    }
                }

                throw jsi::JSError(runtime, "TypeError: No constructor overload exists for " + className + " with " +
                                                std::to_string(numArgs) + " args");
            });
    }

    jsi::Value CreateCompositeMethod(jsi::Runtime& runtime, std::string methodName,
        std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>>
            interfaceMethods)
    {
        if (interfaceMethods.size() == 1)
        {
            const auto& interfaceMethod = interfaceMethods[0];
            return interfaceMethod.first->GetMethod(runtime, interfaceMethod.second);
        }

        // TODO: Does it matter if we don't bother determining the maximum arity for the sake of the length param on the
        // function?
        auto id = CreatePropNameID(runtime, methodName);
        return jsi::Value(
            runtime, jsi::Function::createFromHostFunction(runtime, std::move(id), 0 /*length*/,
                         [methodName{ std::move(methodName) }, interfaceMethods{ std::move(interfaceMethods) }](
                             jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t numArgs) {
                             for (auto& interfaceMethod : interfaceMethods)
                             {
                                 if (interfaceMethod.second->CanInvoke(static_cast<uint16_t>(numArgs)))
                                 {
                                     return interfaceMethod.first->InvokeMethod(
                                         runtime, interfaceMethod.second, args, static_cast<uint16_t>(numArgs));
                                 }
                             }

                             throw jsi::JSError(runtime, "TypeError: No function overload exists for " + methodName +
                                                             " with " + std::to_string(numArgs) + " args");
                         }));
    }
}

namespace WinRTTurboModule
{
    ProjectedAsyncOperationBaseMethods::ProjectedAsyncOperationBaseMethods(
        std::shared_ptr<ProjectionsContext> context) :
        m_threadId(GetCurrentThreadId()),
        m_context(std::move(context))
    {
    }

    std::optional<jsi::Value> ProjectedAsyncOperationBaseMethods::TryGetMethod(
        const jsi::PropNameID& propName, const std::string& propNameUtf8)
    {
        auto& runtime = m_context->Runtime;
        if (propNameUtf8 == "then"sv)
        {
            return jsi::Value(runtime, jsi::Function::createFromHostFunction(runtime, propName, 3,
                                           [strongThis{ shared_from_this() }](jsi::Runtime&, const jsi::Value&,
                                               const jsi::Value* args, size_t count) mutable {
                                               const auto undefined = jsi::Value::undefined();
                                               return strongThis->HandleThen(count >= 1 ? args[0] : undefined,
                                                   count >= 2 ? args[1] : undefined, count >= 3 ? args[2] : undefined);
                                           }));
        }
        else if (propNameUtf8 == "done"sv)
        {
            return jsi::Value(runtime, jsi::Function::createFromHostFunction(runtime, propName, 3,
                                           [strongThis{ shared_from_this() }](jsi::Runtime&, const jsi::Value&,
                                               const jsi::Value* args, size_t count) mutable {
                                               const auto undefined = jsi::Value::undefined();
                                               return strongThis->HandleThen(count >= 1 ? args[0] : undefined,
                                                   count >= 2 ? args[1] : undefined, count >= 3 ? args[2] : undefined,
                                                   true /*forDone*/);
                                           }));
        }
        else if (propNameUtf8 == "catch"sv)
        {
            return jsi::Value(
                runtime, jsi::Function::createFromHostFunction(runtime, propName, 1,
                             [strongThis{ shared_from_this() }](
                                 jsi::Runtime&, const jsi::Value&, const jsi::Value* args, size_t count) mutable {
                                 const auto undefined = jsi::Value::undefined();
                                 return strongThis->HandleThen(undefined, count >= 1 ? args[0] : undefined, undefined);
                             }));
        }
        else if (propNameUtf8 == "finally"sv)
        {
            return jsi::Value(runtime, jsi::Function::createFromHostFunction(runtime, propName, 2,
                                           [strongThis{ shared_from_this() }](jsi::Runtime&, const jsi::Value&,
                                               const jsi::Value* args, size_t count) mutable {
                                               // TODO: Technically "finally" callbacks should be invoked without args,
                                               // but since it is JS, passing args probably doesn't matter.
                                               const auto undefined = jsi::Value::undefined();
                                               const auto& function = count >= 1 ? args[0] : undefined;
                                               return strongThis->HandleThen(function, function, undefined);
                                           }));
        }

        return {};
    }

    jsi::Value ProjectedAsyncOperationBaseMethods::HandleThen(
        const jsi::Value& onResolved, const jsi::Value& onRejected, const jsi::Value& onProgress, bool forDone)
    {
        FAIL_FAST_IF(m_threadId != ::GetCurrentThreadId());

        auto& runtime = m_context->Runtime;
        auto continuation = std::make_shared<Continuation>();
        if (!onResolved.isUndefined())
        {
            continuation->OnResolved = onResolved.asObject(runtime).asFunction(runtime);
        }

        if (!onRejected.isUndefined())
        {
            continuation->OnRejected = onRejected.asObject(runtime).asFunction(runtime);
        }

        if (!onProgress.isUndefined())
        {
            continuation->OnProgress = onProgress.asObject(runtime).asFunction(runtime);
        }

        auto result = forDone ? jsi::Value::undefined() : PromiseWrapper::Create(runtime, continuation->Promise);

        if (m_result)
        {
            // The async operation is already complete so we can immediately call the handlers and resolve the promise.
            // It is still supposed to be async and requeue so it doesn't happen synchronously to calling then.
            m_context->Invoker->CallAsync(
                [strongThis{ shared_from_this() }, continuation{ std::move(continuation) }]() {
                    strongThis->DispatchContinuation(*continuation);
                });
        }
        else
        {
            m_continuations.push_back(std::move(continuation));
        }

        return result;
    }

    void ProjectedAsyncOperationBaseMethods::OnCompleted(jsi::Value result, bool succeeded)
    {
        FAIL_FAST_IF(m_threadId != ::GetCurrentThreadId());

        m_result = std::move(result);
        m_succeeded = succeeded;

        // move m_continuations into the local variables so they will be freed on completion.
        const auto continuations = std::move(m_continuations);
        for (const auto& continuation : continuations)
        {
            DispatchContinuation(*continuation);
        }
    }

    void ProjectedAsyncOperationBaseMethods::DispatchContinuation(const Continuation& continuation) const
    {
        FAIL_FAST_IF(!m_result);

        auto& runtime = m_context->Runtime;
        bool succeeded = false;
        std::optional<jsi::Value> localResult;

        try
        {
            if (m_succeeded)
            {
                if (continuation.OnResolved)
                {
                    localResult = continuation.OnResolved->call(runtime, *m_result);
                }
                succeeded = true;
            }
            else
            {
                if (continuation.OnRejected)
                {
                    localResult = continuation.OnRejected->call(runtime, *m_result);
                    succeeded = true;
                }
            }
        }
        catch (winrt::hresult_error& e)
        {
            localResult = CreateError(runtime, e);
        }
        catch (std::exception& e)
        {
            localResult = CreateError(runtime, e);
        }

        const auto& effectiveResult = localResult ? *localResult : *m_result;
        if (!continuation.Promise)
        {
            // In the "done" scenario, throw unhandled errors instead of swallowing them.
            if (!succeeded)
            {
                throw jsi::JSError(runtime, jsi::Value(runtime, effectiveResult));
            }
        }
        else if (succeeded)
        {
            // Either initially succeeded and there was no resolved handler; or there was a resolved handler and did not
            // throw; or initially failed, but the rejected handler did not throw. In the first case, the original
            // result should be passed to resolve. Note that if the localResult happens to be a Promise itself, that JS
            // will coalesce them on calling resolve.
            continuation.Promise->Resolve(runtime, effectiveResult);
        }
        else
        {
            // The initial error was either not handled by an OnRejected handler or whichever of OnResolved or
            // OnRejected that was called threw an error which still means that the promise should be rejected.
            continuation.Promise->Reject(runtime, effectiveResult);
        }
    }

    void ProjectedAsyncOperationBaseMethods::OnProgress(jsi::Value progress)
    {
        FAIL_FAST_IF(m_threadId != ::GetCurrentThreadId());

        auto& runtime = m_context->Runtime;
        for (const auto& continuation : m_continuations)
        {
            if (continuation->OnProgress)
            {
                continuation->OnProgress->call(runtime, jsi::Value(runtime, progress));
            }
        }
    }

    ProjectedAsyncOperationBase::ProjectedAsyncOperationBase(std::shared_ptr<ProjectionsContext> context) :
        m_methods(std::make_shared<ProjectedAsyncOperationBaseMethods>(std::move(context)))
    {
    }

    jsi::Value ProjectedAsyncOperationBase::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        const auto propNameUtf8 = propName.utf8(runtime);
        if (auto method = m_methods->TryGetMethod(propName, propNameUtf8))
        {
            return std::move(*method);
        }
        else if (propNameUtf8 == "cancel"sv)
        {
            static const auto s_asyncInfo =
                ProjectedInterface::Get(winrt::guid_of<winrt::Windows::Foundation::IAsyncInfo>());
            FAIL_FAST_IF_NULL(s_asyncInfo);

            const auto cancelMethod = s_asyncInfo->FindMethod(propNameUtf8);
            FAIL_FAST_IF_NULL(cancelMethod);

            return cancelMethod->GetFunction(
                Instance().as<winrt::Windows::Foundation::IAsyncInfo>(), m_methods->Context());
        }
        else if (propNameUtf8 == "operation"sv)
        {
            if (!m_operation)
            {
                m_operation = ProjectedRuntimeClassInstance::Get(m_methods->Context(), Instance());
            }
            return jsi::Value(runtime, *m_operation);
        }

        return jsi::Value::undefined();
    }

    void ProjectedAsyncOperationBase::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&)
    {
        throw jsi::JSError(runtime, "TypeError: Cannot assign to property '" + propName.utf8(runtime) +
                                        "' to a projected WinRT AsyncOperation");
    }

    std::vector<jsi::PropNameID> ProjectedAsyncOperationBase::getPropertyNames(jsi::Runtime& runtime)
    {
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(6);

        propNames.push_back(CreatePropNameID(runtime, "cancel"sv));
        propNames.push_back(CreatePropNameID(runtime, "catch"sv));
        propNames.push_back(CreatePropNameID(runtime, "done"sv));
        propNames.push_back(CreatePropNameID(runtime, "finally"sv));
        propNames.push_back(CreatePropNameID(runtime, "then"sv));
        propNames.push_back(CreatePropNameID(runtime, "operation"sv));

        return propNames;
    }
}

#endif
