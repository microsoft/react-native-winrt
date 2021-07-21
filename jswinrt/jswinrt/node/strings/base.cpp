
#include "base.h"

#include <Windows.h>

// #include <combaseapi.h>
#include <winstring.h>

using namespace nodewinrt;
using namespace std::literals;

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
}

static void delete_runtime_context(void* ptr)
{
    auto ctxt = static_cast<runtime_context*>(ptr);

    // TODO: Clean up memory

    delete ctxt;
}

void nodewinrt::register_exports(v8::Local<v8::Context> context, v8::Local<v8::Object> exports)
{
    auto isolate = context->GetIsolate();

    // Set up the per-context data
    auto ctxt = new runtime_context(isolate);
    node::AddEnvironmentCleanupHook(isolate, delete_runtime_context, ctxt);

    for (auto ns : root_namespaces)
    {
        auto nsValue = ns->create(ctxt);
        // TODO: Save reference to the value (or the projected_namespace) in the root context for later cleanup
        check_maybe(exports->Set(context, make_string_checked(isolate, ns->name), nsValue));
    }
}

runtime_context::runtime_context(v8::Isolate* isolate) : isolate(isolate)
{
    // Namespace template
    auto templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    templ->SetHandler(v8::NamedPropertyHandlerConfiguration{ &projected_namespace::property_getter, nullptr,
        &projected_namespace::property_query, nullptr, &projected_namespace::property_enum });
    namespace_template.Reset(isolate, templ);

    // Enum template
    templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    templ->SetHandler(v8::NamedPropertyHandlerConfiguration{ &projected_enum::property_getter, nullptr,
        &projected_enum::property_query, nullptr, &projected_enum::property_enum });
    enum_template.Reset(isolate, templ);

    // Static class template
    templ = v8::ObjectTemplate::New(isolate);
    templ->SetInternalFieldCount(1);
    templ->SetHandler(
        v8::NamedPropertyHandlerConfiguration{ &projected_class::property_getter, &projected_class::property_setter,
            &projected_class::property_query, nullptr, &projected_class::property_enum });
    statics_class_template.Reset(isolate, templ);

    // Activatable class template
    templ = v8::ObjectTemplate::New(isolate);
    templ->SetCallAsFunctionHandler(&projected_class::construct);
    templ->SetInternalFieldCount(1);
    templ->SetHandler(
        v8::NamedPropertyHandlerConfiguration{ &projected_class::property_getter, &projected_class::property_setter,
            &projected_class::property_query, nullptr, &projected_class::property_enum });
    activatable_class_template.Reset(isolate, templ);

    // TODO: Interface template
}

v8::Local<v8::String> nodewinrt::make_string_checked(v8::Isolate* isolate, std::wstring_view str)
{
    if (str.empty())
    {
        return v8::String::NewFromUtf8Literal(isolate, "");
    }

    auto bytes = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.data(), static_cast<int32_t>(str.size()),
        nullptr /*lpMultiByteStr*/, 0 /*cbMultiByte*/, nullptr /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/);
    winrt::check_bool(bytes);

    auto buffer = std::make_unique<char[]>(bytes);
    winrt::check_bool(::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.data(), static_cast<int32_t>(str.size()),
        buffer.get(), bytes, nullptr /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/));

    return make_string_checked(isolate, std::string_view{ buffer.get(), static_cast<std::size_t>(bytes) });
}

#if 0
std::u16string nodewinrt::string_to_utf16(jsi::Runtime& runtime, const jsi::String& string)
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
#endif

[[noreturn]] __declspec(noinline) void nodewinrt::throw_no_constructor(
    v8::Isolate* isolate, std::string_view typeNamespace, std::string_view typeName, int argCount)
{
    auto msg = "No constructor overload exists for "s;
    msg.append(typeNamespace);
    msg += ".";
    msg.append(typeName);
    msg = msg + " with " + std::to_string(argCount) + " args";
    throw v8_exception::type_error(isolate, msg);
}

[[noreturn]] __declspec(noinline) void nodewinrt::throw_no_function_overload(v8::Isolate* isolate,
    std::string_view typeNamespace, std::string_view typeName, std::string_view fnName, int argCount)
{
    auto msg = "No function overload exists for "s;
    msg.append(typeNamespace);
    msg += ".";
    msg.append(typeName);
    msg += ".";
    msg.append(fnName);
    msg = msg + " with " + std::to_string(argCount) + " args";
    throw v8_exception::type_error(isolate, msg);
}

[[noreturn]] __declspec(noinline) void nodewinrt::throw_invalid_delegate_arg_count(
    v8::Isolate* isolate, std::string_view typeNamespace, std::string_view typeName)
{
    auto msg = "Invalid number of arguments to delegate "s;
    msg.append(typeNamespace);
    msg += ".";
    msg.append(typeName);
    throw v8_exception::type_error(isolate, msg);
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

#if 0
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
#endif

v8::Local<v8::Value> static_namespace_data::create(runtime_context* context) const
{
    auto isolate = context->isolate;
    auto templ = context->namespace_template.Get(isolate);
    auto result = check_maybe(templ->NewInstance(isolate->GetCurrentContext()));

    auto ptr = new projected_namespace(context, this);
    result->SetInternalField(0, v8::External::New(isolate, ptr));

    return result;
}

void projected_namespace::property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
try
{
    // TODO: Should namespace be able to respond to symbols? E.g. maybe iterator?
    if (!property->IsString())
        return;

    auto pThis = this_from_holder<projected_namespace>(info);
    auto isolate = pThis->m_context->isolate;
    assert(isolate == info.GetIsolate());
    auto propName = string_to_utf8(isolate, property.As<v8::String>());

    auto& staticChildren = pThis->m_data->children;
    if (auto itr = find_by_name(staticChildren, propName); itr != staticChildren.end())
    {
        auto& item = pThis->m_children[itr - staticChildren.begin()];
        if (item.IsEmpty())
        {
            item.Reset(isolate, (*itr)->create(pThis->m_context));
        }

        return info.GetReturnValue().Set(item.Get(isolate));
    }
}
V8_CATCH(info.GetIsolate())

void projected_namespace::property_query(
    v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info)
try
{
    if (!property->IsString())
        return;

    auto pThis = this_from_holder<projected_namespace>(info);
    auto isolate = pThis->m_context->isolate;
    assert(isolate == info.GetIsolate());
    auto propName = string_to_utf8(isolate, property.As<v8::String>());

    auto& staticChildren = pThis->m_data->children;
    if (auto itr = find_by_name(staticChildren, propName); itr != staticChildren.end())
    {
        return info.GetReturnValue().Set(v8::PropertyAttribute::ReadOnly | v8::PropertyAttribute::DontDelete);
    }
}
V8_CATCH(info.GetIsolate())

void projected_namespace::property_enum(const v8::PropertyCallbackInfo<v8::Array>& info)
try
{
    auto pThis = this_from_holder<projected_namespace>(info);
    auto isolate = pThis->m_context->isolate;
    assert(isolate == info.GetIsolate());
    auto context = isolate->GetCurrentContext();

    auto& staticChildren = pThis->m_data->children;
    auto result = v8::Array::New(isolate, static_cast<int>(staticChildren.size()));
    uint32_t index = 0;
    for (auto&& child : staticChildren)
    {
        check_maybe(result->Set(context, index++, make_string_checked(isolate, child->name)));
    }

    return info.GetReturnValue().Set(result);
}
V8_CATCH(info.GetIsolate())

v8::Local<v8::Value> static_enum_data::create(runtime_context* context) const
{
    auto isolate = context->isolate;
    auto templ = context->enum_template.Get(isolate);
    auto result = check_maybe(templ->NewInstance(isolate->GetCurrentContext()));

    auto ptr = new projected_enum(this);
    result->SetInternalField(0, v8::External::New(isolate, ptr));

    return result;
}

void projected_enum::property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
try
{
    // TODO: Should enums be able to respond to symbols? E.g. maybe iterator?
    if (!property->IsString())
        return;

    auto pThis = this_from_holder<projected_enum>(info);
    auto isolate = info.GetIsolate();
    auto propName = string_to_utf8(isolate, property.As<v8::String>());

    auto& values = pThis->m_data->values;
    if (auto itr = find_by_name(values, propName); itr != values.end())
    {
        return info.GetReturnValue().Set(itr->value);
    }
}
V8_CATCH(info.GetIsolate())

void projected_enum::property_query(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info)
try
{
    if (!property->IsString())
        return;

    auto pThis = this_from_holder<projected_enum>(info);
    auto isolate = info.GetIsolate();
    auto propName = string_to_utf8(isolate, property.As<v8::String>());

    auto& values = pThis->m_data->values;
    if (auto itr = find_by_name(values, propName); itr != values.end())
    {
        return info.GetReturnValue().Set(v8::PropertyAttribute::ReadOnly | v8::PropertyAttribute::DontDelete);
    }
}
V8_CATCH(info.GetIsolate())

void projected_enum::property_enum(const v8::PropertyCallbackInfo<v8::Array>& info)
try
{
    auto pThis = this_from_holder<projected_enum>(info);
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();

    auto& values = pThis->m_data->values;
    auto result = v8::Array::New(isolate, static_cast<int>(values.size()));
    uint32_t index = 0;
    for (auto&& value : values)
    {
        check_maybe(result->Set(context, index++, make_string_checked(isolate, value.name)));
    }

    return info.GetReturnValue().Set(result);
}
V8_CATCH(info.GetIsolate())

v8::Local<v8::Value> static_class_data::create(runtime_context* context) const
{
    auto isolate = context->isolate;
    auto templ = context->statics_class_template.Get(isolate);
    auto result = check_maybe(templ->NewInstance(isolate->GetCurrentContext()));

    auto ptr = new projected_class(context, this);
    result->SetInternalField(0, v8::External::New(isolate, ptr));

    return result;
}

v8::Local<v8::Value> static_activatable_class_data::create(runtime_context* context) const
{
    auto isolate = context->isolate;
    auto templ = context->activatable_class_template.Get(isolate);
    auto result = check_maybe(templ->NewInstance(isolate->GetCurrentContext()));

    auto ptr = new projected_class(context, this);
    result->SetInternalField(0, v8::External::New(isolate, ptr));

    return result;
}

#if 0
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
#endif

void projected_class::construct(const v8::FunctionCallbackInfo<v8::Value>& info)
try
{
    auto pThis = this_from_holder<projected_class>(info);
    auto data = static_cast<const static_activatable_class_data*>(pThis->m_data);
    return data->constructor(pThis->m_context, info);
}
V8_CATCH(info.GetIsolate())

void projected_class::property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
try
{
    // TODO: Should classes be able to respond to symbols? E.g. maybe iterator?
    if (!property->IsString())
        return;

    auto pThis = this_from_holder<projected_class>(info);
    auto isolate = pThis->m_context->isolate;
    assert(isolate == info.GetIsolate());
    auto propName = string_to_utf8(isolate, property.As<v8::String>());

    // Check to see if it is a property
    auto& props = pThis->m_data->properties;
    if (auto itr = find_by_name(props, propName); itr != props.end())
    {
        return info.GetReturnValue().Set(itr->getter(pThis->m_context));
    }

    // Check to see if it is a function
    auto& functions = pThis->m_data->functions;
    if (auto itr = find_by_name(functions, propName); itr != functions.end())
    {
        auto& item = pThis->m_functions[itr - functions.begin()];
        if (item.IsEmpty())
        {
            auto fn = check_maybe(v8::Function::New(isolate->GetCurrentContext(), itr->function,
                v8::External::New(isolate, pThis->m_context), 0, v8::ConstructorBehavior::kThrow));
            item.Reset(isolate, fn);
        }

        return info.GetReturnValue().Set(item.Get(isolate));
    }

    // Check to see if this is an event add/remove
    // TODO
}
V8_CATCH(info.GetIsolate())

void projected_class::property_setter(
    v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
try
{
    if (!property->IsString())
        return;

    auto pThis = this_from_holder<projected_class>(info);
    auto isolate = pThis->m_context->isolate;
    assert(isolate == info.GetIsolate());
    auto propName = string_to_utf8(isolate, property.As<v8::String>());

    // Can only set a property
    auto& props = pThis->m_data->properties;
    if (auto itr = find_by_name(props, propName); (itr != props.end()) && itr->setter)
    {
        itr->setter(pThis->m_context, value);
        return info.GetReturnValue().Set(value);
    }
}
V8_CATCH(info.GetIsolate())

void projected_class::property_query(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info)
try
{
    if (!property->IsString())
        return;

    auto pThis = this_from_holder<projected_class>(info);
    auto isolate = pThis->m_context->isolate;
    assert(isolate == info.GetIsolate());
    auto propName = string_to_utf8(isolate, property.As<v8::String>());

    auto& props = pThis->m_data->properties;
    if (auto itr = find_by_name(props, propName); itr != props.end())
    {
        int result = v8::PropertyAttribute::DontDelete;
        if (!itr->setter)
            result |= v8::PropertyAttribute::ReadOnly;
        return info.GetReturnValue().Set(result);
    }

    auto& functions = pThis->m_data->functions;
    if (auto itr = find_by_name(functions, propName); itr != functions.end())
    {
        return info.GetReturnValue().Set(v8::PropertyAttribute::ReadOnly | v8::PropertyAttribute::DontDelete);
    }

    if (!pThis->m_data->events.empty())
    {
        if (propName == add_event_name)
            return info.GetReturnValue().Set(v8::PropertyAttribute::ReadOnly | v8::PropertyAttribute::DontDelete);
        if (propName == remove_event_name)
            return info.GetReturnValue().Set(v8::PropertyAttribute::ReadOnly | v8::PropertyAttribute::DontDelete);
    }
}
V8_CATCH(info.GetIsolate())

void projected_class::property_enum(const v8::PropertyCallbackInfo<v8::Array>& info)
try
{
    auto pThis = this_from_holder<projected_class>(info);
    auto isolate = pThis->m_context->isolate;
    assert(isolate == info.GetIsolate());
    auto context = isolate->GetCurrentContext();

    auto& props = pThis->m_data->properties;
    auto& functions = pThis->m_data->functions;
    bool hasEvents = !pThis->m_data->events.empty();
    int propListSize = static_cast<int>(props.size() + functions.size() + (hasEvents ? 2 : 0));

    auto result = v8::Array::New(isolate, propListSize);
    uint32_t index = 0;

    for (auto&& prop : props)
    {
        check_maybe(result->Set(context, index++, make_string_checked(isolate, prop.name)));
    }

    for (auto&& fn : functions)
    {
        check_maybe(result->Set(context, index++, make_string_checked(isolate, fn.name)));
    }

    if (hasEvents)
    {
        check_maybe(result->Set(context, index++, make_string_checked(isolate, add_event_name)));
        check_maybe(result->Set(context, index++, make_string_checked(isolate, remove_event_name)));
    }

    return info.GetReturnValue().Set(result);
}
V8_CATCH(info.GetIsolate())

#if 0
jsi::Value projected_statics_class::add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
{
    return static_add_event_listener(runtime, args, count, m_data, m_events);
}

jsi::Value projected_statics_class::remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
{
    return static_remove_event_listener(runtime, args, count, m_data, m_events);
}
#endif

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

#if 0
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

namespace nodewinrt
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
#endif

v8::Local<v8::Value> projected_value_traits<char16_t>::as_value(runtime_context* context, char16_t value)
{
    char buffer[8];
    auto bytes = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, reinterpret_cast<PCWSTR>(&value), 1, buffer,
        static_cast<int>(std::size(buffer)), nullptr, nullptr);
    winrt::check_bool(bytes);
    return make_string_checked(context->isolate, std::string_view{ buffer, static_cast<std::size_t>(bytes) });
}

char16_t projected_value_traits<char16_t>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    auto str = convert_value_to_native<winrt::hstring>(context, value);
    return str.empty() ? 0 : str[0];
}

winrt::hstring projected_value_traits<winrt::hstring>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    // TODO: Should we call 'ToString' here? Or perhaps be less strict (e.g. allow numbers, null for empty, etc.)
    auto strValue = cast_string(context->isolate, value);
    auto str = string_to_utf8(context->isolate, strValue);
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

v8::Local<v8::Value> projected_value_traits<winrt::guid>::as_value(runtime_context* context, const winrt::guid& value)
{
    // NOTE: 'StringFromGUID2' formats in '{...}' form, but we don't want the curly braces
    wchar_t wideBuffer[guid_length + 1];
    winrt::check_hresult(::StringFromGUID2(reinterpret_cast<const GUID&>(value), wideBuffer, ARRAYSIZE(wideBuffer)));

    // GUIDS are always ANSI, so there's no need to call 'WideCharToMultiByte' or anything
    char buffer[uuid_length];
    std::transform(
        wideBuffer + 1, wideBuffer + 1 + uuid_length, buffer, [](wchar_t ch) { return static_cast<char>(ch); });

    return make_string_checked(context->isolate, std::string_view{ buffer, uuid_length });
}

winrt::guid projected_value_traits<winrt::guid>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    auto str = string_to_utf8(context->isolate, cast_string(context->isolate, value));
    auto strBuffer = str.data();
    if (str.size() == guid_length)
    {
        ++strBuffer; // Move past the '{'
        strBuffer[uuid_length] = 0; // UuidFromString expects null termination
    }
    else if (str.size() != uuid_length)
    {
        throw v8_exception::type_error(context->isolate, "Invalid GUID length");
    }

    winrt::guid result;
    if (::UuidFromStringA(reinterpret_cast<RPC_CSTR>(strBuffer), reinterpret_cast<UUID*>(winrt::put_abi(result))) !=
        ERROR_SUCCESS)
    {
        throw v8_exception::type_error(context->isolate, "GUID contains unexpected characters");
    }

    return result;
}

// NOTE: 'DateTime' is a 'FILETIME' value which represents the number of 100 ns "units" since 01/01/1601, whereas the
// Javascript 'Date' class represents the number of milliseconds since 01/01/1970. The delta between the two is
// 11644473600 seconds
static constexpr auto windows_to_unix_epoch_delta = 11644473600s;

v8::Local<v8::Value> projected_value_traits<winrt::DateTime>::as_value(runtime_context* context, winrt::DateTime value)
{
    auto unixTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(value.time_since_epoch() - windows_to_unix_epoch_delta);
    return check_maybe(v8::Date::New(context->isolate->GetCurrentContext(), static_cast<double>(unixTime.count())));
}

winrt::DateTime projected_value_traits<winrt::DateTime>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    double number;
    if (value->IsNumber())
    {
        number = value.As<v8::Number>()->Value();
    }
    else if (value->IsDate())
    {
        number = value.As<v8::Date>()->ValueOf();
    }
    else
    {
        throw v8_exception::type_error(context->isolate, "Expected either a Date or a number");
    }

    std::chrono::milliseconds unixTime(static_cast<int64_t>(number));
    return winrt::DateTime(unixTime + windows_to_unix_epoch_delta);
}

v8::Local<v8::Value> projected_value_traits<winrt::TimeSpan>::as_value(runtime_context* context, winrt::TimeSpan value)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(value);
    return convert_native_to_value<int64_t>(context, ms.count());
}

winrt::TimeSpan projected_value_traits<winrt::TimeSpan>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    std::chrono::milliseconds ms(convert_value_to_native<int64_t>(context, value));
    return std::chrono::duration_cast<winrt::TimeSpan>(ms);
}

v8::Local<v8::Value> projected_value_traits<winrt::float3x2>::as_value(runtime_context* context, winrt::float3x2 value)
{
    auto ctxt = context->isolate->GetCurrentContext();
    auto result = v8::Object::New(context->isolate);
    set_property(context, ctxt, result, "m11"sv, value.m11);
    set_property(context, ctxt, result, "m12"sv, value.m12);
    set_property(context, ctxt, result, "m21"sv, value.m21);
    set_property(context, ctxt, result, "m22"sv, value.m22);
    set_property(context, ctxt, result, "m31"sv, value.m31);
    set_property(context, ctxt, result, "m32"sv, value.m32);
    return result;
}

winrt::float3x2 projected_value_traits<winrt::float3x2>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    winrt::float3x2 result{};
    auto obj = cast_object(context->isolate, value);
    auto ctxt = context->isolate->GetCurrentContext();
    if (auto field = get_property(context->isolate, ctxt, obj, "m11"sv); !field->IsUndefined())
        result.m11 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m12"sv); !field->IsUndefined())
        result.m12 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m21"sv); !field->IsUndefined())
        result.m21 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m22"sv); !field->IsUndefined())
        result.m22 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m31"sv); !field->IsUndefined())
        result.m31 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m32"sv); !field->IsUndefined())
        result.m32 = convert_value_to_native<float>(context, field);
    return result;
}

v8::Local<v8::Value> projected_value_traits<winrt::float4x4>::as_value(runtime_context* context, winrt::float4x4 value)
{
    auto ctxt = context->isolate->GetCurrentContext();
    auto result = v8::Object::New(context->isolate);
    set_property(context, ctxt, result, "m11"sv, value.m11);
    set_property(context, ctxt, result, "m12"sv, value.m12);
    set_property(context, ctxt, result, "m13"sv, value.m13);
    set_property(context, ctxt, result, "m14"sv, value.m14);
    set_property(context, ctxt, result, "m21"sv, value.m21);
    set_property(context, ctxt, result, "m22"sv, value.m22);
    set_property(context, ctxt, result, "m23"sv, value.m23);
    set_property(context, ctxt, result, "m24"sv, value.m24);
    set_property(context, ctxt, result, "m31"sv, value.m31);
    set_property(context, ctxt, result, "m32"sv, value.m32);
    set_property(context, ctxt, result, "m33"sv, value.m33);
    set_property(context, ctxt, result, "m34"sv, value.m34);
    set_property(context, ctxt, result, "m41"sv, value.m41);
    set_property(context, ctxt, result, "m42"sv, value.m42);
    set_property(context, ctxt, result, "m43"sv, value.m43);
    set_property(context, ctxt, result, "m44"sv, value.m44);
    return result;
}

winrt::float4x4 projected_value_traits<winrt::float4x4>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    winrt::float4x4 result{};
    auto obj = cast_object(context->isolate, value);
    auto ctxt = context->isolate->GetCurrentContext();
    if (auto field = get_property(context->isolate, ctxt, obj, "m11"sv); !field->IsUndefined())
        result.m11 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m12"sv); !field->IsUndefined())
        result.m12 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m13"sv); !field->IsUndefined())
        result.m13 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m14"sv); !field->IsUndefined())
        result.m14 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m21"sv); !field->IsUndefined())
        result.m21 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m22"sv); !field->IsUndefined())
        result.m22 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m23"sv); !field->IsUndefined())
        result.m23 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m24"sv); !field->IsUndefined())
        result.m24 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m31"sv); !field->IsUndefined())
        result.m31 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m32"sv); !field->IsUndefined())
        result.m32 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m33"sv); !field->IsUndefined())
        result.m33 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m34"sv); !field->IsUndefined())
        result.m34 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m41"sv); !field->IsUndefined())
        result.m41 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m42"sv); !field->IsUndefined())
        result.m42 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m43"sv); !field->IsUndefined())
        result.m43 = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "m44"sv); !field->IsUndefined())
        result.m44 = convert_value_to_native<float>(context, field);
    return result;
}

v8::Local<v8::Value> projected_value_traits<winrt::plane>::as_value(runtime_context* context, winrt::plane value)
{
    auto ctxt = context->isolate->GetCurrentContext();
    auto result = v8::Object::New(context->isolate);
    set_property(context, ctxt, result, "normal"sv, value.normal);
    set_property(context, ctxt, result, "d"sv, value.d);
    return result;
}

winrt::plane projected_value_traits<winrt::plane>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    winrt::plane result{};
    auto obj = cast_object(context->isolate, value);
    auto ctxt = context->isolate->GetCurrentContext();
    if (auto field = get_property(context->isolate, ctxt, obj, "normal"sv); !field->IsUndefined())
        result.normal = convert_value_to_native<winrt::float3>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "d"sv); !field->IsUndefined())
        result.d = convert_value_to_native<float>(context, field);
    return result;
}

v8::Local<v8::Value> projected_value_traits<winrt::quaternion>::as_value(runtime_context* context, winrt::quaternion value)
{
    auto ctxt = context->isolate->GetCurrentContext();
    auto result = v8::Object::New(context->isolate);
    set_property(context, ctxt, result, "x"sv, value.x);
    set_property(context, ctxt, result, "y"sv, value.y);
    set_property(context, ctxt, result, "z"sv, value.z);
    set_property(context, ctxt, result, "w"sv, value.w);
    return result;
}

winrt::quaternion projected_value_traits<winrt::quaternion>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    winrt::quaternion result{};
    auto obj = cast_object(context->isolate, value);
    auto ctxt = context->isolate->GetCurrentContext();
    if (auto field = get_property(context->isolate, ctxt, obj, "x"sv); !field->IsUndefined())
        result.x = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "y"sv); !field->IsUndefined())
        result.y = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "z"sv); !field->IsUndefined())
        result.z = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "w"sv); !field->IsUndefined())
        result.w = convert_value_to_native<float>(context, field);
    return result;
}

v8::Local<v8::Value> projected_value_traits<winrt::float2>::as_value(runtime_context* context, winrt::float2 value)
{
    auto ctxt = context->isolate->GetCurrentContext();
    auto result = v8::Object::New(context->isolate);
    set_property(context, ctxt, result, "x"sv, value.x);
    set_property(context, ctxt, result, "y"sv, value.y);
    return result;
}

winrt::float2 projected_value_traits<winrt::float2>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    winrt::float2 result{};
    auto obj = cast_object(context->isolate, value);
    auto ctxt = context->isolate->GetCurrentContext();
    if (auto field = get_property(context->isolate, ctxt, obj, "x"sv); !field->IsUndefined())
        result.x = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "y"sv); !field->IsUndefined())
        result.y = convert_value_to_native<float>(context, field);
    return result;
}

v8::Local<v8::Value> projected_value_traits<winrt::float3>::as_value(runtime_context* context, winrt::float3 value)
{
    auto ctxt = context->isolate->GetCurrentContext();
    auto result = v8::Object::New(context->isolate);
    set_property(context, ctxt, result, "x"sv, value.x);
    set_property(context, ctxt, result, "y"sv, value.y);
    set_property(context, ctxt, result, "z"sv, value.z);
    return result;
}

winrt::float3 projected_value_traits<winrt::float3>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    winrt::float3 result{};
    auto obj = cast_object(context->isolate, value);
    auto ctxt = context->isolate->GetCurrentContext();
    if (auto field = get_property(context->isolate, ctxt, obj, "x"sv); !field->IsUndefined())
        result.x = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "y"sv); !field->IsUndefined())
        result.y = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "z"sv); !field->IsUndefined())
        result.z = convert_value_to_native<float>(context, field);
    return result;
}

v8::Local<v8::Value> projected_value_traits<winrt::float4>::as_value(runtime_context* context, winrt::float4 value)
{
    auto ctxt = context->isolate->GetCurrentContext();
    auto result = v8::Object::New(context->isolate);
    set_property(context, ctxt, result, "x"sv, value.x);
    set_property(context, ctxt, result, "y"sv, value.y);
    set_property(context, ctxt, result, "z"sv, value.z);
    set_property(context, ctxt, result, "w"sv, value.w);
    return result;
}

winrt::float4 projected_value_traits<winrt::float4>::as_native(runtime_context* context, v8::Local<v8::Value> value)
{
    winrt::float4 result{};
    auto obj = cast_object(context->isolate, value);
    auto ctxt = context->isolate->GetCurrentContext();
    if (auto field = get_property(context->isolate, ctxt, obj, "x"sv); !field->IsUndefined())
        result.x = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "y"sv); !field->IsUndefined())
        result.y = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "z"sv); !field->IsUndefined())
        result.z = convert_value_to_native<float>(context, field);
    if (auto field = get_property(context->isolate, ctxt, obj, "w"sv); !field->IsUndefined())
        result.w = convert_value_to_native<float>(context, field);
    return result;
}

v8::Local<v8::Value> nodewinrt::convert_from_property_value(
    runtime_context* context, const winrt::IPropertyValue& value)
{
    switch (value.Type())
    {
    case winrt::PropertyType::Empty:
        return v8::Undefined(context->isolate);
    case winrt::PropertyType::UInt8:
        return convert_native_to_value(context, value.GetUInt8());
    case winrt::PropertyType::Int16:
        return convert_native_to_value(context, value.GetInt16());
    case winrt::PropertyType::UInt16:
        return convert_native_to_value(context, value.GetUInt16());
    case winrt::PropertyType::Int32:
        return convert_native_to_value(context, value.GetInt32());
    case winrt::PropertyType::UInt32:
        return convert_native_to_value(context, value.GetUInt32());
    case winrt::PropertyType::Int64:
        return convert_native_to_value(context, value.GetInt64());
    case winrt::PropertyType::UInt64:
        return convert_native_to_value(context, value.GetUInt64());
    case winrt::PropertyType::Single:
        return convert_native_to_value(context, value.GetSingle());
    case winrt::PropertyType::Double:
        return convert_native_to_value(context, value.GetDouble());
    case winrt::PropertyType::Char16:
        return convert_native_to_value(context, value.GetChar16());
    case winrt::PropertyType::Boolean:
        return convert_native_to_value(context, value.GetBoolean());
    case winrt::PropertyType::String:
        return convert_native_to_value(context, value.GetString());
    case winrt::PropertyType::Inspectable:
        return v8::Undefined(context->isolate); // IInspectable is just the object itself
    case winrt::PropertyType::DateTime:
        return convert_native_to_value(context, value.GetDateTime());
    case winrt::PropertyType::TimeSpan:
        return convert_native_to_value(context, value.GetTimeSpan());
    case winrt::PropertyType::Guid:
        return convert_native_to_value(context, value.GetGuid());
    case winrt::PropertyType::Point:
        return convert_native_to_value(context, value.GetPoint());
    case winrt::PropertyType::Size:
        return convert_native_to_value(context, value.GetSize());
    case winrt::PropertyType::Rect:
        return convert_native_to_value(context, value.GetRect());
    case winrt::PropertyType::OtherType:
        return v8::Undefined(context->isolate);
    case winrt::PropertyType::UInt8Array: {
        winrt::com_array<uint8_t> result;
        value.GetUInt8Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::Int16Array: {
        winrt::com_array<int16_t> result;
        value.GetInt16Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::UInt16Array: {
        winrt::com_array<uint16_t> result;
        value.GetUInt16Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::Int32Array: {
        winrt::com_array<int32_t> result;
        value.GetInt32Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::UInt32Array: {
        winrt::com_array<uint32_t> result;
        value.GetUInt32Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::Int64Array: {
        winrt::com_array<int64_t> result;
        value.GetInt64Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::UInt64Array: {
        winrt::com_array<uint64_t> result;
        value.GetUInt64Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::SingleArray: {
        winrt::com_array<float> result;
        value.GetSingleArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::DoubleArray: {
        winrt::com_array<double> result;
        value.GetDoubleArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::Char16Array: {
        winrt::com_array<char16_t> result;
        value.GetChar16Array(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::BooleanArray: {
        winrt::com_array<bool> result;
        value.GetBooleanArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::StringArray: {
        winrt::com_array<winrt::hstring> result;
        value.GetStringArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::InspectableArray: {
        winrt::com_array<winrt::IInspectable> result;
        value.GetInspectableArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::DateTimeArray: {
        winrt::com_array<winrt::DateTime> result;
        value.GetDateTimeArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::TimeSpanArray: {
        winrt::com_array<winrt::TimeSpan> result;
        value.GetTimeSpanArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::GuidArray: {
        winrt::com_array<winrt::guid> result;
        value.GetGuidArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::PointArray: {
        winrt::com_array<winrt::Point> result;
        value.GetPointArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::SizeArray: {
        winrt::com_array<winrt::Size> result;
        value.GetSizeArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::RectArray: {
        winrt::com_array<winrt::Rect> result;
        value.GetRectArray(result);
        return convert_native_to_value(context, std::move(result));
    }
    case winrt::PropertyType::OtherTypeArray:
        return v8::Undefined(context->isolate);
    default:
        winrt::terminate(); // TODO: Just return undefined?
    }
}

winrt::IInspectable nodewinrt::convert_to_property_value(runtime_context* context, v8::Local<v8::Value> value)
{
    if (value->IsBoolean())
    {
        return winrt::PropertyValue::CreateBoolean(value.As<v8::Boolean>()->Value());
    }
    else if (value->IsNumber())
    {
        // NOTE: Due to inherent ambiguities between the loosely typed JS 'number' type and strongly typed WinRT types,
        // we can't do much better than to take a guess here and preserve the value as a double-precision floating point
        // value. It's probably best for consumers to call the 'PropertyValue' static methods directly as needed
        return winrt::PropertyValue::CreateDouble(value.As<v8::Number>()->Value());
    }
    else if (value->IsString())
    {
        return winrt::PropertyValue::CreateString(convert_value_to_native<winrt::hstring>(context, value));
    }
    else if (value->IsArray())
    {
        auto array = value.As<v8::Array>();

        // Can't determine type from empty arrays
        if (array->Length() > 0)
        {
            auto ctxt = context->isolate->GetCurrentContext();
            auto elem = get_property(ctxt, array, 0);
            if (elem->IsBoolean())
            {
                return winrt::PropertyValue::CreateBooleanArray(
                    convert_value_to_native<winrt::array_view<const bool>>(context, value));
            }
            else if (elem->IsNumber())
            {
                return winrt::PropertyValue::CreateDoubleArray(
                    convert_value_to_native<winrt::array_view<const double>>(context, value));
            }
            else if (elem->IsString())
            {
                return winrt::PropertyValue::CreateStringArray(
                    convert_value_to_native<winrt::array_view<const winrt::hstring>>(context, value));
            }
            else if (elem->IsDate())
            {
                return winrt::PropertyValue::CreateDateTimeArray(
                    convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::DateTime>>(
                        context, value));
            }
            else if (elem->IsObject())
            {
                auto elemObj = elem.As<v8::Object>();
#if 0           // TODO
                if (elemObj.isHostObject(runtime))
                {
                    return winrt::PropertyValue::CreateInspectableArray(
                        convert_value_to_native<winrt::array_view<const winrt::IInspectable>>(runtime, value));
                }
#endif

                auto isPointLike = has_property(context->isolate, ctxt, elemObj, "x"sv) &&
                                   has_property(context->isolate, ctxt, elemObj, "y");
                auto isSizeLike = has_property(context->isolate, ctxt, elemObj, "width"sv) &&
                                  has_property(context->isolate, ctxt, elemObj, "height"sv);
                if (isPointLike && isSizeLike)
                {
                    return winrt::PropertyValue::CreateRectArray(
                        convert_value_to_native<winrt::array_view<const winrt::Rect>>(context, value));
                }
                else if (isPointLike)
                {
                    return winrt::PropertyValue::CreatePointArray(
                        convert_value_to_native<winrt::array_view<const winrt::Point>>(context, value));
                }
                else if (isSizeLike)
                {
                    return winrt::PropertyValue::CreateSizeArray(
                        convert_value_to_native<winrt::array_view<const winrt::Size>>(context, value));
                }
            }
        }
    }
    else if (value->IsDate())
    {
        return winrt::PropertyValue::CreateDateTime(
            convert_value_to_native<winrt::Windows::Foundation::DateTime>(context, value));
    }
    else if (value->IsObject())
    {
        auto ctxt = context->isolate->GetCurrentContext();
        auto obj = value.As<v8::Object>();

        auto isPointLike =
            has_property(context->isolate, ctxt, obj, "x"sv) && has_property(context->isolate, ctxt, obj, "y");
        auto isSizeLike = has_property(context->isolate, ctxt, obj, "width"sv) &&
                          has_property(context->isolate, ctxt, obj, "height"sv);
        if (isPointLike && isSizeLike)
        {
            return winrt::PropertyValue::CreateRect(convert_value_to_native<winrt::Rect>(context, value));
        }
        else if (isPointLike)
        {
            return winrt::PropertyValue::CreatePoint(convert_value_to_native<winrt::Point>(context, value));
        }
        else if (isSizeLike)
        {
            return winrt::PropertyValue::CreateSize(convert_value_to_native<winrt::Size>(context, value));
        }

    }

    return nullptr;
}
