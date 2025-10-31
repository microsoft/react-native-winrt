#pragma once
#include "pch.h"
#include <functional>
#include <memory>
#include <vector>
#include <type_traits>

// Wrappers for Napi types. The idea is that code using napi_wrappers:: can use this to switch to Napi easily.
namespace napi_wrappers
{
    // Forward declarations
    class Value;
    class Object;
    class Array;
    class Function;
    class String;
    class PropNameID;
    class Runtime;
    class HostObject;
    class StringBuffer;
    
    // Runtime wrapper - defined first to avoid circular deps
    class Runtime 
    {
    public:
        Runtime(Napi::Env env) : m_env(env) {}
        operator Napi::Env&() { return m_env; }
        operator const Napi::Env&() const { return m_env; }
        Napi::Env& env() { return m_env; }
        const Napi::Env& env() const { return m_env; }
        
        Object global() const;
        Value evaluateJavaScript(std::shared_ptr<StringBuffer> buffer, const std::string& sourceURL) const;
        Object createObject();
        Function getGlobalFunction(const char* name);
        String createString(const char* str);
        Value Undefined();
        
    private:
        Napi::Env m_env;
    };
    
    // Base HostObject class
    class HostObject 
    {
    public:
        virtual ~HostObject() = default;
        
        // Virtual methods that derived classes must implement
        virtual Value get(Runtime& runtime, const PropNameID& name) = 0;
        virtual void set(Runtime& runtime, const PropNameID& name, const Value& value) = 0;
        virtual std::vector<PropNameID> getPropertyNames(Runtime& runtime) = 0;
    };
    
    // Value wrapper
    class Value 
    {
    public:
        Value() = default;
        Value(const Napi::Value& val) : m_value(val) {}
        Value(Runtime& env, const Napi::Value& val) : m_value(val) {}
        // Copy constructor from existing Value - needed by generated code
        Value(Runtime& env, const Value& val) : m_value(val.m_value) {}
        // Constructors that depend on other classes are implemented at the end
        Value(Runtime& env, const Object& obj);
        Value(Runtime& env, const Function& func);
        Value(Runtime& env, const String& str);

        explicit Value(Runtime& env, bool val) : m_value(Napi::Boolean::New(env.env(), val)) {}
        explicit Value(Runtime& env, int32_t val) : m_value(Napi::Number::New(env.env(), val)) {}
        explicit Value(Runtime& env, double val) : m_value(Napi::Number::New(env.env(), val)) {}

        //Value(bool val) : m_value() { /* Store for later conversion */ }
        //Value(int32_t val) : m_value() { /* Store for later conversion */ }
        //Value(double val) : m_value() { /* Store for later conversion */ }
        

        
        // Static constructors for numeric values
        static Value create(Runtime& env, bool val) { return Value(Napi::Boolean::New(env.env(), val)); }
        static Value create(Runtime& env, double num) { return Value(Napi::Number::New(env.env(), num)); }
        static Value create(Runtime& env, int num) { return Value(Napi::Number::New(env.env(), num)); }
        static Value create(Runtime& env, int64_t num) { return Value(Napi::Number::New(env.env(), static_cast<double>(num))); }
        static Value create(Runtime& env, uint32_t num) { return Value(Napi::Number::New(env.env(), num)); }
        

        
        operator Napi::Value() const { return m_value; }
        operator napi_value() const { return m_value; }
        
        static Value null(Runtime& env) { return Value(Napi::Value(env.env().Null())); }
        static Value undefined(Runtime& env) { return Value(Napi::Value(env.env().Undefined())); }        

        bool isBool() const { return m_value.IsBoolean(); }
        bool isNumber() const { return m_value.IsNumber(); }
        bool isString() const { return m_value.IsString(); }
        bool isObject() const { return m_value.IsObject(); }
        bool isNull() const { return m_value.IsNull(); }
        bool isUndefined() const { return m_value.IsUndefined(); }
        bool isSymbol() const { return m_value.IsSymbol(); }
        
        bool getBool() const { return m_value.As<Napi::Boolean>().Value(); }
        double getNumber() const { return m_value.As<Napi::Number>().DoubleValue(); }
        // Methods that depend on other classes are implemented at the end
        String getString(Runtime& env) const;
        // asString/toString currently identical; kept separate for API parity with JSI. Consider consolidating.
        String asString(Runtime& env) const;
        String toString(Runtime& env) const;
        Object asObject(Runtime& env) const;
        Object getObject(Runtime& env) const;
        double asNumber() const { return m_value.As<Napi::Number>().DoubleValue(); }
        bool strictEquals(const Value& other) const { return m_value.StrictEquals(other.m_value); }
        
        // Static version for compatibility
        static bool strictEquals(Runtime& env, const Value& val1, const Value& val2) { return val1.m_value.StrictEquals(val2.m_value); }
        
    public:
        Napi::Value m_value;
    };
    
    // String wrapper
    class String : public Value
    {
    public:
        String() = default;
        String(const Napi::String& str) : Value(str) {}
        
        static String createFromAscii(Runtime& env, const char* str);
        static String createFromAscii(Runtime& env, const char* str, size_t length);
        static String createFromUtf8(Runtime& env, const std::string& str);
        static String createFromUtf8(Runtime& env, const uint8_t* str, size_t length);
        
        std::string utf8(Runtime& env) const;
    };
    
    // PropNameID wrapper (essentially a string)
    class PropNameID : public String
    {
    public:
        PropNameID() = default;
        PropNameID(const Napi::String& str) : String(str) {}
        
        static PropNameID forAscii(Runtime& env, const char* str);
        static PropNameID forUtf8(Runtime& env, const std::string& str);
        static PropNameID forUtf8(Runtime& env, const uint8_t* str, size_t length);
        std::string utf8(Runtime& env) const;
    };
    
    // Object wrapper
    class Object : public Value
    {
    public:
        Object() = default;
        Object(const Napi::Object& obj) : Value(obj) {}
        Object(Runtime& env) : Value(Napi::Object::New(env.env())) {}
        
        static Object createFromHostObject(Runtime& env, std::shared_ptr<HostObject> hostObj);
        
        template<typename T>
        std::shared_ptr<T> asHostObject(Runtime& env) const;
        
        template<typename T>
        bool isHostObject(Runtime& env) const;
        
        template<typename T>
        std::shared_ptr<T> getHostObject(Runtime& env) const;
        
        Value getProperty(Runtime& env, const char* name) const;
        Value getProperty(Runtime& env, const std::string& name) const;
        Function getPropertyAsFunction(Runtime& env, const char* name) const;
        
        void setProperty(Runtime& env, const char* name, const Value& value);
        void setProperty(Runtime& env, const std::string& name, const Value& value);
        void setProperty(Runtime& env, const PropNameID& name, const Value& value);

        // Generic numeric/bool convenience template
        template<typename T,
                 std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, char> && !std::is_same_v<T, signed char> && !std::is_same_v<T, unsigned char>, int> = 0>
        void setProperty(Runtime& env, const char* name, T v)
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                setProperty(env, name, Value::create(env, v));
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                setProperty(env, name, Value::create(env, static_cast<double>(v)));
            }
            else if constexpr (std::is_signed_v<T> && sizeof(T) <= 4)
            {
                setProperty(env, name, Value::create(env, static_cast<int>(v)));
            }
            else if constexpr (!std::is_signed_v<T> && sizeof(T) <= 4)
            {
                setProperty(env, name, Value::create(env, static_cast<uint32_t>(v)));
            }
            else
            {
                // Fallback for larger integral types
                setProperty(env, name, Value::create(env, static_cast<int64_t>(v)));
            }
        }
        
        bool hasProperty(Runtime& env, const char* name) const;
        bool hasProperty(Runtime& env, const std::string& name) const;
        
        bool isArray(Runtime& env) const;
        Array getArray(Runtime& env) const;
        bool isHostObject(Runtime& env) const;
        // Cast this Object to an Array (throws if not an array)
        Array asArray(Runtime& env) const;
        
        // Additional missing methods
        Function asFunction(Runtime& env) const;
        Object getPropertyAsObject(Runtime& env, const char* name) const;
        bool strictEquals(const Object& other) const;
        
        // Static version for base.h usage
        static bool strictEquals(Runtime& env, const Object& obj1, const Object& obj2);
    };
    
    // Array wrapper
    class Array : public Object
    {
    public:
        Array() = default;
        Array(const Napi::Array& arr) : Object(arr) {}
        Array(Runtime& env, size_t length);
        
        size_t size(Runtime& env) const;
        size_t length(Runtime& env) const { return size(env); } // Alias for size
        Value getValueAtIndex(Runtime& env, size_t index) const;
        void setValueAtIndex(Runtime& env, size_t index, const Value& value);
        //using Object::setProperty; // Expose Object's overloads & template
    };
    
    // Function wrapper
    class Function : public Object
    {
    public:
        Function() = default;
        Function(const Napi::Function& func) : Object(func) {}
        
        using HostFunctionType = std::function<Value(Runtime&, const Value&, const Value*, size_t)>;
        
        static Function createFromHostFunction(Runtime& env, const PropNameID& name, unsigned int paramCount, HostFunctionType callback);
        
        Value call(Runtime& env) const;
        Value call(Runtime& env, const Value& thisVal, const Value* args, size_t count) const;
        Value call(Runtime& env, const Value& arg) const; // Single argument overload
        Value call(Runtime& env, const Object& thisVal, const String& arg1, const Object& arg2) const; // Three argument overload for base.cpp
        Value call(Runtime& env, const Value& thisVal, const Value& arg1, double arg2, const Value& arg3) const; // Five argument overload
        Value call(Runtime& env, std::initializer_list<Value> args) const; // Initializer list overload

        template <typename... Args>
        inline Value call(Runtime& runtime, Args&&... args) const {
            return call(runtime, {Value(runtime, std::forward<Args>(args))...});
        }


        Value callAsConstructor(Runtime& env, const Value* args, size_t count) const;
        Value callAsConstructor(Runtime& env, double arg) const; // Single double overload
        Value callAsConstructor(Runtime& env, const Function& arg) const; // Function overload
        Value callWithThis(Runtime& env, const Object& thisVal) const;
        Value callWithThis(Runtime& env, const Array& thisVal, const Value& arg) const; // 3-arg overload
        Value callWithThis(Runtime& env, const Object& thisVal, const Value& arg1, double arg2, const Value& arg3) const; // 5-arg overload

        // Generic variadic argument version (0+ args)
        Value callWithThis(Runtime& env, const Object& thisVal, std::initializer_list<Value> args) const {
            std::vector<napi_value> napiArgs;
            napiArgs.reserve(args.size());
            for (auto const& v : args) {
                napiArgs.push_back(v.m_value);
            }
            return m_value.As<Napi::Function>().Call(thisVal.m_value.As<Napi::Object>(), napiArgs);
        }

        // Single-argument (Value) overload for generic Object 'this' to avoid template ambiguity
        Value callWithThis(Runtime& env, const Object& thisVal, const Value& arg1) const {
            napi_value napiArgs[1] = { arg1.m_value };
            return m_value.As<Napi::Function>().Call(thisVal.m_value.As<Napi::Object>(), 1, napiArgs);
        }
        // Variadic template for 0 or 2+ arguments (exclude exactly 1 to prevent ambiguity with single-arg overload)
        template<typename... Args, std::enable_if_t<(sizeof...(Args) != 1), int> = 0>
        Value callWithThis(Runtime& runtime, const Object& thisVal, Args&&... args) const {
            std::vector<napi_value> napiArgs;
            napiArgs.reserve(sizeof...(Args));
            (napiArgs.push_back(Value(runtime, std::forward<Args>(args)).m_value), ...);
            return m_value.As<Napi::Function>().Call(thisVal.m_value.As<Napi::Object>(), napiArgs);
        }
    };
    
    // Exception type
    class JSError : public std::runtime_error
    {
    public:
        JSError(Runtime& env, const std::string& message) 
            : std::runtime_error(message), m_env(env) {}
            
        JSError(Runtime& env, std::string&& message) 
            : std::runtime_error(std::move(message)), m_env(env) {}
            
        Runtime& env() const { return m_env; }
        Value value() const { return Value(); } // Placeholder for error value
        
    private:
        Runtime& m_env;
    };
    
    // WeakObject wrapper - NAPI doesn't have direct weak references like JSI
    // This is a simplified implementation
    // TODO: Implement proper weak references
    class WeakObject
    {
    public:
        WeakObject() = default;
        WeakObject(Runtime& env, const Object& obj) 
        {
            // Simplified implementation - in real scenario would use weak references
            m_ref.Reset(obj.m_value.As<Napi::Object>(), 1);
        }
        
        Value lock(Runtime& env) 
        {
            if (m_ref.IsEmpty()) 
                return Value();
            return Value(m_ref.Value());
        }
        
    private:
        Napi::ObjectReference m_ref;
    };
    
    // StringBuffer for JavaScript evaluation
    class StringBuffer
    {
    public:
        StringBuffer(std::string str) : m_str(std::move(str)) {}
        const std::string& str() const { return m_str; }
        
    private:
        std::string m_str;
    };
    

    
    // Template methods must stay in header
    template<typename T>
    inline std::shared_ptr<T> Object::asHostObject(Runtime& env) const 
    {
        try {
            // Access the _hostObject_ property (the ProxyGetTrap will handle this specially)
            auto hostObjectValue = getProperty(env, "_hostObject_");
            
            if (hostObjectValue.m_value.IsExternal())
            {
                // The External wraps a pointer to shared_ptr<HostObject>
                using HostObjectPtr = std::shared_ptr<napi_wrappers::HostObject>;
                
                // Extract the pointer to the shared_ptr
                auto external = hostObjectValue.m_value.As<Napi::External<HostObjectPtr>>();
                auto hostObjectPtrPtr = external.Data();
                
                if (hostObjectPtrPtr) {
                    // Dereference to get the actual shared_ptr<HostObject>
                    auto hostObjectPtr = *hostObjectPtrPtr;
                    
                    // Cast to the requested type T using dynamic_pointer_cast for safety
                    return std::dynamic_pointer_cast<T>(hostObjectPtr);
                }
            }
            
            return std::shared_ptr<T>();
            
        } catch (...) {
            // If anything fails, return nullptr
            return std::shared_ptr<T>();
        }
    }
    
    template<typename T>
    inline bool Object::isHostObject(Runtime& env) const 
    {
        try {
            // Check if this object has the _hostObject_ property marker
            auto hostObjectValue = getProperty(env, "_hostObject_");
            return hostObjectValue.m_value.IsExternal();
        } catch (...) {
            return false;
        }
    }
    
    template<typename T>
    inline std::shared_ptr<T> Object::getHostObject(Runtime& env) const 
    {
        // For NAPI wrappers, getHostObject and asHostObject behave identically
        // (unlike JSI where asHostObject can throw)
        return asHostObject<T>(env);
    }

    // Inline implementation of Object::asArray
    inline Array Object::asArray(Runtime& env) const
    {
        if (!isArray(env))
        {
            throw JSError(env, "Object is not an Array"); // TODO: Attach a JS error value for richer diagnostics
        }
        return Array(m_value.As<Napi::Array>());
    }
    

    
    
}


