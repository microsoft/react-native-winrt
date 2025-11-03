#include <napi.h>
#include "../generated/rnwinrt/base.h"

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


// Placeholder addon that will later use generated projection code.

Napi::String Hello(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), "Hello from WinRT placeholder addon");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hello", Napi::Function::New(env, Hello));

    rnwinrt::initialize(env, exports);
    return exports;
}

NODE_API_MODULE(winrtaddon, Init)
