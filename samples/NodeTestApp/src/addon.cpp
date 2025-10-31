#include <napi.h>

// Placeholder addon that will later use generated projection code.

Napi::String Hello(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), "Hello from WinRT placeholder addon");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("hello", Napi::Function::New(env, Hello));
    return exports;
}

NODE_API_MODULE(winrtaddon, Init)
