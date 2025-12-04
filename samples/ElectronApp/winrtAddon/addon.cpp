// Node-API + C++/WinRT integration (PCH)
#include "pch.h"
#include <WindowsAppSDK-VersionInfo.h>
#include <appmodel.h> // PACKAGE_VERSION
// NOTE: Temporarily omit generated projection headers while migrating from JSI to Napi.
// Future work: reintroduce Napi-based generated headers.

//#include "generated/rnwinrt/base_napi.h" // Napi projection metadata (namespaces, enums, classes)
#include "generated/rnwinrt/base.h"

using namespace winrt;
using namespace winrt::Microsoft::Windows::ApplicationModel::WindowsAppRuntime;

// namespace rnwinrt
// {
//     napi_wrappers::Value projected_value_traits<winrt::Windows::ApplicationModel::PackageVersion>::as_value(napi_wrappers::Runtime& runtime, const winrt::Windows::ApplicationModel::PackageVersion& value)
//     {
//         auto env = runtime.env();
//         auto result = Napi::Object::New(env);
//         result.Set("major", Napi::Number::New(env, value.Major));
//         result.Set("minor", Napi::Number::New(env, value.Minor));
//         result.Set("build", Napi::Number::New(env, value.Build));
//         result.Set("revision", Napi::Number::New(env, value.Revision));
//         return napi_wrappers::Value(result);
//     }

//     winrt::Windows::ApplicationModel::PackageVersion projected_value_traits<winrt::Windows::ApplicationModel::PackageVersion>::as_native(napi_wrappers::Runtime& runtime, const napi_wrappers::Value& value)
//     {
//         winrt::Windows::ApplicationModel::PackageVersion result{};
//         /*
//         auto env = runtime.env();
//         auto obj = value.asObject(env);
//         if (auto field = obj.getProperty(env, "major"); !field.isUndefined())
//             result.Major = convert_value_to_native<uint16_t>(env, field);
//         if (auto field = obj.getProperty(env, "minor"); !field.isUndefined())
//             result.Minor = convert_value_to_native<uint16_t>(env, field);
//         if (auto field = obj.getProperty(env, "build"); !field.isUndefined())
//             result.Build = convert_value_to_native<uint16_t>(env, field);
//         if (auto field = obj.getProperty(env, "revision"); !field.isUndefined())
//             result.Revision = convert_value_to_native<uint16_t>(env, field);
//         return result;
//         */
//        return result;
//     }
// }




namespace rnwinrt
{
    thread_local rnwinrt::runtime_context* current_thread_context = nullptr;

    // Functions exposed to JS
    void initialize(Napi::Env env, Napi::Object exports)
    {
        assert(!current_thread_context);
        current_thread_context = new rnwinrt::runtime_context(
            env, [](std::function<void()> fn) { fn(); });

        napi_wrappers::Runtime runtime(env);
        for (auto ns : rnwinrt::root_namespaces) {
            if (!ns) continue;
            exports.Set(Napi::String::New(env, std::string(ns->name)), ns->create(runtime));
        }
    }

rnwinrt::runtime_context* current_runtime_context()
    {
        auto result = current_thread_context;
        if (!result)
        {
            // WinRT module not initialized for the current thread. This is likely because of a logic error in the runtime
            // where we are trying to access data from a background thread instead of caching the context on creation
            winrt::terminate();
        }

        return result;
    }
}

// Convert DeploymentStatus enum to a readable string.
static const char* DeploymentStatusToString(DeploymentStatus status) {
    switch (status) {
        case DeploymentStatus::Unknown: return "Unknown";
        case DeploymentStatus::Ok: return "Ok";
        case DeploymentStatus::PackageInstallRequired: return "PackageInstallRequired";
        case DeploymentStatus::PackageInstallFailed: return "PackageInstallFailed";
        default: return "(Unrecognized)";
    }
}

// Wrap DeploymentResult into a JS object.
static Napi::Object DeploymentResultToObject(Napi::Env env, const DeploymentResult& result) {
    Napi::Object o = Napi::Object::New(env);
    DeploymentStatus status = result.Status();
    auto extended = static_cast<int32_t>(result.ExtendedError().value);
    o.Set("status", DeploymentStatusToString(status));
    o.Set("statusCode", static_cast<int32_t>(status));
    o.Set("extendedError", extended); // HRESULT numeric
    o.Set("ok", status == DeploymentStatus::Ok && extended == 0);
    o.Set("installRequired", status == DeploymentStatus::PackageInstallRequired);
    return o;
}

// add(a,b) example retained
Napi::Value Add(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected two arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    if (!info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Expected number arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    double a = info[0].As<Napi::Number>().DoubleValue();
    double b = info[1].As<Napi::Number>().DoubleValue();
    return Napi::Number::New(env, a + b);
}

// deploymentGetStatus(): returns current status without performing deployment.
Napi::Value DeploymentGetStatus(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    try {
        auto result = DeploymentManager::GetStatus();
        return DeploymentResultToObject(env, result);
    } catch (const winrt::hresult_error& e) {
        Napi::Object err = Napi::Object::New(env);
        err.Set("error", Napi::String::New(env, winrt::to_string(e.message())));
        err.Set("hresult", static_cast<int32_t>(e.code().value));
        return err;
    }

}

// deploymentInitialize(options?): performs deployment where needed.
// options: { forceDeployment?: boolean, onErrorShowUI?: boolean }
Napi::Value DeploymentInitialize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    bool hasOptions = info.Length() > 0 && info[0].IsObject();
    try {
        DeploymentResult result{ nullptr };
        if (hasOptions) {
            Napi::Object opts = info[0].As<Napi::Object>();
            DeploymentInitializeOptions dio;
            if (opts.Has("forceDeployment") && opts.Get("forceDeployment").IsBoolean()) {
                dio.ForceDeployment(opts.Get("forceDeployment").As<Napi::Boolean>().Value());
            }
            if (opts.Has("onErrorShowUI") && opts.Get("onErrorShowUI").IsBoolean()) {
                // Need IDeploymentInitializeOptions2 interface -> property OnErrorShowUI
                dio.OnErrorShowUI(opts.Get("onErrorShowUI").As<Napi::Boolean>().Value());
            }
            result = DeploymentManager::Initialize(dio);
        } else {
            result = DeploymentManager::Initialize();
        }
        return DeploymentResultToObject(env, result);
    } catch (const winrt::hresult_error& e) {
        Napi::Object err = Napi::Object::New(env);
        err.Set("error", Napi::String::New(env, winrt::to_string(e.message())));
        err.Set("hresult", static_cast<int32_t>(e.code().value));
        return err;
    }
}

// Export DeploymentStatus enum mapping for JS consumption.
Napi::Object CreateDeploymentStatusEnum(Napi::Env env) {
    Napi::Object en = Napi::Object::New(env);
    en.Set("Unknown", static_cast<int32_t>(DeploymentStatus::Unknown));
    en.Set("Ok", static_cast<int32_t>(DeploymentStatus::Ok));
    en.Set("PackageInstallRequired", static_cast<int32_t>(DeploymentStatus::PackageInstallRequired));
    en.Set("PackageInstallFailed", static_cast<int32_t>(DeploymentStatus::PackageInstallFailed));
    return en;
}

// Create a constructable DeploymentManager JS type. Underlying WinRT DeploymentManager
// is a static class; instances here are lightweight wrappers whose methods invoke the
// exported static DeploymentManager APIs. Using ObjectWrap provides a clearer pattern
// and makes future instance state additions simpler.
class DeploymentManagerWrapper : public Napi::ObjectWrap<DeploymentManagerWrapper> {
public:
    static Napi::FunctionReference constructor;

    static Napi::Object Init(Napi::Env env) {
        Napi::Function func = DefineClass(env, "DeploymentManager", {
            InstanceMethod<&DeploymentManagerWrapper::GetStatus>("getStatus"),
            InstanceMethod<&DeploymentManagerWrapper::Initialize>("initialize")
        });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();

        // Add static mirrors
        func.Set("getStatus", Napi::Function::New(env, DeploymentGetStatus, "getStatus"));
        func.Set("initialize", Napi::Function::New(env, DeploymentInitialize, "initialize"));
        func.Set("Status", CreateDeploymentStatusEnum(env));
        return func;
    }

    DeploymentManagerWrapper(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<DeploymentManagerWrapper>(info) {
        // No per-instance state needed yet.
    }

private:
    Napi::Value GetStatus(const Napi::CallbackInfo& info) {
        return DeploymentGetStatus(info);
    }
    Napi::Value Initialize(const Napi::CallbackInfo& info) {
        return DeploymentInitialize(info);
    }
};

Napi::FunctionReference DeploymentManagerWrapper::constructor;

// PackageVersion ObjectWrap (wrapper around Windows PACKAGE_VERSION union)
class PackageVersionWrapper : public Napi::ObjectWrap<PackageVersionWrapper> {
public:
    static Napi::FunctionReference constructor;

    static Napi::Object Init(Napi::Env env) {
        Napi::Function func = DefineClass(env, "PackageVersion", {
            InstanceMethod("major", &PackageVersionWrapper::MajorMethod),
            InstanceMethod("minor", &PackageVersionWrapper::MinorMethod),
            InstanceMethod("build", &PackageVersionWrapper::BuildMethod),
            InstanceMethod("revision", &PackageVersionWrapper::RevisionMethod),
            InstanceMethod("set", &PackageVersionWrapper::SetMethod)
        });
        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        return func;
    }

    PackageVersionWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<PackageVersionWrapper>(info) {
        size_t argc = info.Length();
        if (argc == 0) {
            pv_.Version = 0;
            return;
        }
        if (argc == 1 && info[0].IsObject()) {
            Napi::Object o = info[0].As<Napi::Object>();
            if (o.Has("Major") && o.Get("Major").IsNumber()) pv_.Major = static_cast<uint16_t>(o.Get("Major").As<Napi::Number>().Uint32Value());
            if (o.Has("Minor") && o.Get("Minor").IsNumber()) pv_.Minor = static_cast<uint16_t>(o.Get("Minor").As<Napi::Number>().Uint32Value());
            if (o.Has("Build") && o.Get("Build").IsNumber()) pv_.Build = static_cast<uint16_t>(o.Get("Build").As<Napi::Number>().Uint32Value());
            if (o.Has("Revision") && o.Get("Revision").IsNumber()) pv_.Revision = static_cast<uint16_t>(o.Get("Revision").As<Napi::Number>().Uint32Value());
            return;
        }
        if (argc >= 4) {
            pv_.Major = ArgAsU16(info,0);
            pv_.Minor = ArgAsU16(info,1);
            pv_.Build = ArgAsU16(info,2);
            pv_.Revision = ArgAsU16(info,3);
        } else {
            // Fallback: zero initialize
            pv_.Version = 0;
        }
    }

private:
    PACKAGE_VERSION pv_{};

    uint16_t ArgAsU16(const Napi::CallbackInfo& info, size_t idx) {
        if (info.Length() <= idx || !info[idx].IsNumber()) return 0;
        uint32_t v = info[idx].As<Napi::Number>().Uint32Value();
        return static_cast<uint16_t>(v & 0xFFFF);
    }

    void ParseFromObject(const Napi::Object&) {}

    Napi::Value MajorMethod(const Napi::CallbackInfo& info) { return Napi::Number::New(info.Env(), pv_.Major); }
    Napi::Value MinorMethod(const Napi::CallbackInfo& info) { return Napi::Number::New(info.Env(), pv_.Minor); }
    Napi::Value BuildMethod(const Napi::CallbackInfo& info) { return Napi::Number::New(info.Env(), pv_.Build); }
    Napi::Value RevisionMethod(const Napi::CallbackInfo& info) { return Napi::Number::New(info.Env(), pv_.Revision); }
    // Removed toJSON for minimal surface.
    Napi::Value SetMethod(const Napi::CallbackInfo& info) {
        if (info.Length() >= 4) {
            pv_.Major = ArgAsU16(info,0);
            pv_.Minor = ArgAsU16(info,1);
            pv_.Build = ArgAsU16(info,2);
            pv_.Revision = ArgAsU16(info,3);
        }
        return info.This();
    }
public:
    const PACKAGE_VERSION& Value() const { return pv_; }
};

Napi::FunctionReference PackageVersionWrapper::constructor;

// Helper to get system message for an HRESULT (best-effort)
static std::string HResultMessage(HRESULT hr) {
    char* msgBuf = nullptr;
    DWORD len = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgBuf, 0, nullptr);
    if (len == 0 || !msgBuf) return std::string();
    std::string msg(msgBuf, len);
    // Trim trailing newlines/carriage returns
    while (!msg.empty() && (msg.back()=='\r' || msg.back()=='\n')) msg.pop_back();
    LocalFree(msgBuf);
    return msg;
}

// bootstrapShutdown()
Napi::Value BootstrapShutdown(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    MddBootstrapShutdown();
    return env.Undefined();
}

template <typename T>
T CreateStructFromObject(const Napi::Object& obj);

template <>
PACKAGE_VERSION CreateStructFromObject<PACKAGE_VERSION>(const Napi::Object& obj) {
    PACKAGE_VERSION version{};
    if (obj.Has("Major") && obj.Get("Major").IsNumber()) version.Major = obj.Get("Major").As<Napi::Number>().Uint32Value();
    if (obj.Has("Minor") && obj.Get("Minor").IsNumber()) version.Minor = obj.Get("Minor").As<Napi::Number>().Uint32Value();
    if (obj.Has("Build") && obj.Get("Build").IsNumber()) version.Build = obj.Get("Build").As<Napi::Number>().Uint32Value();
    if (obj.Has("Revision") && obj.Get("Revision").IsNumber()) version.Revision = obj.Get("Revision").As<Napi::Number>().Uint32Value();
    return version;
}

Napi::Value CreateResultWithHResult(Napi::Env env, HRESULT hr) {
    Napi::Object result = Napi::Object::New(env);
    result.Set("hresult", static_cast<int32_t>(hr));
    if (FAILED(hr)) {
        std::string msg = HResultMessage(hr);
        if (!msg.empty()) result.Set("error", Napi::String::New(env, msg));
    }
    return result;
}

inline std::wstring ToWString(const std::u16string& u16) {
    return std::wstring(u16.begin(), u16.end());
}

Napi::Value MddBootstrapInitializeWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    UINT32 majorMinorVersionParam = info[0].As<Napi::Number>().Uint32Value();
    std::u16string versionTagParam = info[1].As<Napi::String>().Utf16Value();
    PACKAGE_VERSION minVersionParam = CreateStructFromObject<PACKAGE_VERSION>(info[2].As<Napi::Object>());

    const HRESULT hr = MddBootstrapInitialize(
        majorMinorVersionParam,
        ToWString(versionTagParam).c_str(),
        minVersionParam
    );

    return CreateResultWithHResult(env, hr);
}

class Example : public Napi::ObjectWrap<Example> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Example(const Napi::CallbackInfo &info);

  private:
    double _value;
    Napi::Value GetValue(const Napi::CallbackInfo &info);
    void SetValue(const Napi::CallbackInfo &info, const Napi::Value &value);
};

Napi::Object Example::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Banana", {
        // Register a class instance accessor with getter and setter functions.
        InstanceAccessor<&Example::GetValue, &Example::SetValue>("value"),
        // We can also register a readonly accessor by omitting the setter.
        //InstanceAccessor<&Example::GetValue>("type"),
        // Define a static property
        StaticValue("MyData", Napi::String::New(env, "someValue"))
    });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
    exports.Set("Banana", func);

    return exports;
}

Example::Example(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Example>(info) {
    Napi::Env env = info.Env();
    this->Value().Set("type", Napi::String::New(env, "SuperBanana"));
    
    // ...
    //Napi::Number value = info[0].As<Napi::Number>();
    //this->_value = value.DoubleValue();
}

Napi::Value Example::GetValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    //return Napi::Number::New(env, this->_value);
    return Napi::String::New(env, "Hello, world (orange)!");
}

void Example::SetValue(const Napi::CallbackInfo &info, const Napi::Value &value) {
    Napi::Env env = info.Env();
    // ...
    //Napi::Number arg = value.As<Napi::Number>();
    //this->_value = arg.DoubleValue();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Initialize WinRT apartment (MTA) once. If already initialized it's harmless with C++/WinRT's init_apartment.
    /*
    static bool apartmentInitialized = false;
    if (!apartmentInitialized) {
        winrt::init_apartment(apartment_type::multi_threaded);
        apartmentInitialized = true;
    }
        */

    exports.Set("add", Napi::Function::New(env, Add));
    exports.Set("deploymentGetStatus", Napi::Function::New(env, DeploymentGetStatus));
    exports.Set("deploymentInitialize", Napi::Function::New(env, DeploymentInitialize));
    exports.Set("DeploymentStatus", CreateDeploymentStatusEnum(env));

    exports.Set("DeploymentManager", DeploymentManagerWrapper::Init(env));
    exports.Set("PackageVersion", PackageVersionWrapper::Init(env));
    //exports.Set("bootstrapInitialize", Napi::Function::New(env, BootstrapInitialize));
    //exports.Set("bootstrapShutdown", Napi::Function::New(env, BootstrapShutdown));

    exports.Set("MddBootstrapInitialize", Napi::Function::New(env, MddBootstrapInitializeWrapper));
    
    napi_wrappers::Runtime runtime(env);
    // Expose generated WinRT projection root namespaces produced by the Napi projection layer.
    // TODO: Expose this once jsi->napi conversion is ready
    rnwinrt::initialize(runtime, exports);

    Example::Init(env, exports);

    /*
    exports.Set("Banana", Napi::Function::New(env, [](const Napi::CallbackInfo& info) {
        auto objThis = Napi::Object::New(info.Env());
        objThis.Set("type", Napi::String::New(info.Env(), "Banana"));
        objThis.Set("color", Napi::String::New(info.Env(), "Yellow"));
        return objThis;
    }));

*/
    return exports;
}

NODE_API_MODULE(winrtAddon, Init)
