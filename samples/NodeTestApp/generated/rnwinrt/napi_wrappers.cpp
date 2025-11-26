#include "pch.h"
#include "napi_wrappers.h"
#include "base.h"

namespace napi_wrappers {

    // Internal helper to convert Value* + count to napi_value vector
    namespace {
        inline std::vector<napi_value> ToNapiArgs(const Value* args, size_t count) {
            std::vector<napi_value> out;
            out.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                out.push_back(args[i].m_value);
            }
            return out;
        }
    }

    // Object implementations
    Object Object::createFromHostObject(Runtime& env, std::shared_ptr<HostObject> hostObj) 
    {
        // Check if this is a projected_object_instance - if so, use WinRTObjectWrapper for better performance
        if (hostObj->isProjectedObjectInstance())
        {
            // Safe to static_cast since isProjectedObjectInstance() returned true
            auto projectedInstance = std::static_pointer_cast<rnwinrt::projected_object_instance>(hostObj);
            return Object(WinRTObjectWrapper::Create(env.env(), projectedInstance));
        }
        
        // For other HostObjects, create a simple object with property getters/setters that delegate to the HostObject
        auto obj = Napi::Object::New(env.env());
        
        // Store the HostObject in an External
        using HostObjectPtr = std::shared_ptr<HostObject>;
        auto hostObjectPtr = new HostObjectPtr(hostObj);
        auto external = Napi::External<HostObjectPtr>::New(env.env(), hostObjectPtr,
            [](Napi::Env, HostObjectPtr* ptr) { delete ptr; });
        
        // TODO: Remove this _hostObject_ prop, it's not working.
        //obj.Set("_hostObject_", external);
        
        // Get all property names from the HostObject and define getters/setters for them
        try {
            auto propNames = hostObj->getPropertyNames(env);
            for (const auto& propName : propNames) {
                auto propNameStr = propName.utf8(env);
                
                // Create getter lambda that captures hostObj and property name
                auto getter = [hostObj, propNameStr](const Napi::CallbackInfo& info) -> Napi::Value {
                    napi_wrappers::Runtime runtime(info.Env());
                    auto propId = napi_wrappers::PropNameID::forAscii(runtime, propNameStr.c_str());
                    auto result = hostObj->get(runtime, propId);
                    return result.m_value;
                };
                
                // Create setter lambda that captures hostObj and property name
                auto setter = [hostObj, propNameStr](const Napi::CallbackInfo& info) {
                    if (info.Length() > 0) {
                        napi_wrappers::Runtime runtime(info.Env());
                        auto propId = napi_wrappers::PropNameID::forAscii(runtime, propNameStr.c_str());
                        napi_wrappers::Value value(runtime, info[0]);
                        hostObj->set(runtime, propId, value);
                    }
                };

                
                // Use Napi::PropertyDescriptor::Accessor to define the property
                obj.DefineProperty(Napi::PropertyDescriptor::Accessor(
                    propNameStr,
                    getter,
                    setter
                ));
            }
        } catch (...) {
            // If we can't get property names, just return the object with _hostObject_
        }
        
        return Object(obj);
    }

    Value Object::getProperty(Runtime& env, const char* name) const 
    {
        return Value(m_value.As<Napi::Object>().Get(name));
    }

    Value Object::getProperty(Runtime& env, const std::string& name) const 
    {
        return Value(m_value.As<Napi::Object>().Get(name));
    }

    Function Object::getPropertyAsFunction(Runtime& env, const char* name) const 
    {
        return Function(m_value.As<Napi::Object>().Get(name).As<Napi::Function>());
    }

    void Object::setProperty(Runtime& env, const char* name, const Value& value) 
    {
        m_value.As<Napi::Object>().Set(name, value.m_value);
    }

    void Object::setProperty(Runtime& env, const std::string& name, const Value& value) 
    {
        m_value.As<Napi::Object>().Set(name, value.m_value);
    }

    void Object::setProperty(Runtime& env, const PropNameID& name, const Value& value) 
    {
        m_value.As<Napi::Object>().Set(name.m_value, value.m_value);
    }

    bool Object::hasProperty(Runtime& env, const char* name) const 
    {
        return m_value.As<Napi::Object>().Has(name);
    }

    bool Object::hasProperty(Runtime& env, const std::string& name) const 
    {
        return m_value.As<Napi::Object>().Has(name);
    }

    bool Object::isArray(Runtime& env) const 
    {
        return m_value.IsArray();
    }

    Array Object::getArray(Runtime& env) const 
    {
        return Array(m_value.As<Napi::Array>());
    }

    bool Object::isHostObject(Runtime& env) const 
    {
        // TODO: Implement host object detection (e.g., check for hidden symbol or internal wrapping)
        return false;
    }

    Function Object::asFunction(Runtime& env) const 
    {
        return Function(m_value.As<Napi::Function>());
    }

    Object Object::getPropertyAsObject(Runtime& env, const char* name) const 
    {
        return Object(m_value.As<Napi::Object>().Get(name).As<Napi::Object>());
    }

    bool Object::strictEquals(const Object& other) const 
    {
        return m_value.StrictEquals(other.m_value);
    }

    bool Object::strictEquals(Runtime& env, const Object& obj1, const Object& obj2) 
    {
        return obj1.m_value.StrictEquals(obj2.m_value);
    }

    // Array implementations
    Array::Array(Runtime& env, size_t length) : Object(Napi::Array::New(env.env(), length)) {}

    size_t Array::size(Runtime& env) const 
    {
        return m_value.As<Napi::Array>().Length();
    }

    Value Array::getValueAtIndex(Runtime& env, size_t index) const 
    {
        return Value(m_value.As<Napi::Array>().Get(static_cast<uint32_t>(index)));
    }

    void Array::setValueAtIndex(Runtime& env, size_t index, const Value& value)
    {
        m_value.As<Napi::Array>().Set(static_cast<uint32_t>(index), value.m_value);
    }

    // Function implementations
    Function Function::createFromHostFunction(Runtime& env, const PropNameID& name, unsigned int paramCount, HostFunctionType callback) 
    {
        // Capture the callback in a shared_ptr to ensure proper lifetime management
        auto capturedCallback = std::make_shared<HostFunctionType>(std::move(callback));
        
        // Create a NAPI function that will invoke our callback
        auto napiFunction = Napi::Function::New(env.env(), [capturedCallback](const Napi::CallbackInfo& info) -> Napi::Value {
            try {
                napi_wrappers::Runtime runtime(info.Env());
                
                // Convert JavaScript arguments to napi_wrappers::Value array
                std::vector<napi_wrappers::Value> args;
                args.reserve(info.Length());
                for (size_t i = 0; i < info.Length(); ++i) {
                    args.emplace_back(info[i]);
                }
                
                // Get 'this' value - use global object if not provided
                napi_wrappers::Value thisValue;
                if (info.This().IsObject()) {
                    thisValue = napi_wrappers::Value(info.This());
                } else {
                    thisValue = runtime.global();
                }
                
                // Call the captured callback
                auto result = (*capturedCallback)(runtime, thisValue, args.data(), args.size());
                return result.m_value; // Convert back to Napi::Value
                
            } catch (const napi_wrappers::JSError& e) {
                throw Napi::Error::New(info.Env(), e.what());
            } catch (const std::exception& e) {
                throw Napi::Error::New(info.Env(), e.what());
            } catch (const char* e) {
                throw Napi::Error::New(info.Env(), e);
            } catch (...) {
                throw Napi::Error::New(info.Env(), "Unknown error occurred");
            }
        }, name.utf8(env).c_str());
        
        return Function(napiFunction);
    }

    Value Function::call(Runtime& env, const Value& thisVal, const Value* args, size_t count) const 
    {
        auto napiArgs = ToNapiArgs(args, count);
        return Value(m_value.As<Napi::Function>().Call(thisVal.m_value, napiArgs));
    }

    Value Function::call(Runtime& env) const 
    {
        return Value(m_value.As<Napi::Function>().Call({}));
    }

    Value Function::call(Runtime& env, const Value& arg) const 
    {
        return Value(m_value.As<Napi::Function>().Call({arg.m_value}));
    }

    Value Function::call(Runtime& env, const Object& thisVal, const String& arg1, const Object& arg2) const 
    {
        return Value(m_value.As<Napi::Function>().Call(thisVal.m_value, {arg1.m_value, arg2.m_value}));
    }

    Value Function::callAsConstructor(Runtime& env, const Value* args, size_t count) const 
    {
        auto napiArgs = ToNapiArgs(args, count);
        return Value(m_value.As<Napi::Function>().New(napiArgs));
    }

    Value Function::callAsConstructor(Runtime& env, double arg) const 
    {
        return Value(m_value.As<Napi::Function>().New({Napi::Number::New(env.env(), arg)}));
    }

    Value Function::callAsConstructor(Runtime& env, const Function& arg) const 
    {
        return Value(m_value.As<Napi::Function>().New({arg.m_value}));
    }

    Value Function::call(Runtime& env, const Value& thisVal, const Value& arg1, double arg2, const Value& arg3) const 
    {
        return Value(m_value.As<Napi::Function>().Call(thisVal.m_value, {arg1.m_value, Napi::Number::New(env.env(), arg2), arg3.m_value}));
    }

    Value Function::call(Runtime& env, std::initializer_list<Value> args) const 
    {
        std::vector<napi_value> napiArgs;
        for (const auto& arg : args) {
            napiArgs.push_back(arg.m_value);
        }
        return Value(m_value.As<Napi::Function>().Call(napiArgs));
    }

    Value Function::callWithThis(Runtime& env, const Object& thisVal) const 
    {
        return Value(m_value.As<Napi::Function>().Call(thisVal.m_value, {}));
    }

    Value Function::callWithThis(Runtime& env, const Array& thisVal, const Value& arg) const 
    {
        return Value(m_value.As<Napi::Function>().Call(thisVal.m_value, {arg.m_value}));
    }

    Value Function::callWithThis(Runtime& env, const Object& thisVal, const Value& arg1, double arg2, const Value& arg3) const 
    {
        return Value(m_value.As<Napi::Function>().Call(thisVal.m_value, {arg1.m_value, Napi::Number::New(env.env(), arg2), arg3.m_value}));
    }

    // Runtime method implementations
    Object Runtime::createObject() 
    {
        return Object(Napi::Object::New(m_env));
    }

    Function Runtime::getGlobalFunction(const char* name) 
    {
        return Function(m_env.Global().Get(name).As<Napi::Function>());
    }

    String Runtime::createString(const char* str) 
    {
        return String(Napi::String::New(m_env, str));
    }

    Value Runtime::Undefined()
    {
        return Value(m_env.Undefined());
    }

    Object Runtime::global() const
    {
        return Object(m_env.Global());
    }

    Value Runtime::evaluateJavaScript(std::shared_ptr<StringBuffer> buffer, const std::string& sourceURL) const
    {
        // TODO: Implement script evaluation. Options: use node::MakeCallback with vm module, or expose a helper JS function.
        return Value(m_env.Undefined());
    }

    // Value constructors that depend on other classes
    Value::Value(Runtime& env, const Object& obj) : m_value(static_cast<const Value&>(obj).m_value) {}
    Value::Value(Runtime& env, const Function& func) : m_value(static_cast<const Value&>(func).m_value) {}
    Value::Value(Runtime& env, const String& str) : m_value(static_cast<const Value&>(str).m_value) {}

    // Value method implementations that depend on other classes
    String Value::getString(Runtime& env) const 
    { 
        return String(m_value.As<Napi::String>()); 
    }

    String Value::asString(Runtime& env) const
    {
        // Identical to toString() currently – kept for API parity; may differentiate (e.g., throws) later
        return String(m_value.ToString());
    }

    String Value::toString(Runtime& env) const
    {
        return String(m_value.ToString()); // See asString
    }

    Object Value::asObject(Runtime& env) const
    {
        return Object(m_value.As<Napi::Object>()); // TODO: Validate object type? Throw if not object?
    }

    Object Value::getObject(Runtime& env) const
    {
        return Object(m_value.As<Napi::Object>()); // TODO: Differentiate getObject vs asObject semantics
    }

    // PropNameID implementations
    PropNameID PropNameID::forAscii(Runtime& env, const char* str)
    {
        return PropNameID(Napi::String::New(env.env(), str));
    }

    PropNameID PropNameID::forUtf8(Runtime& env, const std::string& str)
    {
        return PropNameID(Napi::String::New(env.env(), str));
    }

    std::string PropNameID::utf8(Runtime& env) const
    {
        return m_value.As<Napi::String>().Utf8Value();
    }
    
    PropNameID PropNameID::forUtf8(Runtime& env, const uint8_t* str, size_t length)
    {
        return PropNameID(Napi::String::New(env.env(), reinterpret_cast<const char*>(str), length));
    }

    // String implementations
    std::string String::utf8(Runtime& env) const
    {
        return m_value.As<Napi::String>().Utf8Value();
    }

    String String::createFromUtf8(Runtime& env, const std::string& str)
    {
        return String(Napi::String::New(env.env(), str));
    }
    
    String String::createFromUtf8(Runtime& env, const uint8_t* str, size_t length)
    {
        return String(Napi::String::New(env.env(), reinterpret_cast<const char*>(str), length));
    }

    String String::createFromAscii(Runtime& env, const char* str)
    {
        return String(Napi::String::New(env.env(), str));
    }

    String String::createFromAscii(Runtime& env, const char* str, size_t length)
    {
        return String(Napi::String::New(env.env(), str, length));
    }

} // namespace napi_wrappers
