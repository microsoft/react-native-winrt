// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#pragma once

#include <ReactCommon/TurboModule.h>
#include <rnwinrt/base.h>

class WinRTTurboModule final : public facebook::react::TurboModule
{
public:
    WinRTTurboModule(std::shared_ptr<facebook::react::CallInvoker> invoker);
    ~WinRTTurboModule();

    // Functions exposed to JS
    void initialize(facebook::jsi::Runtime& runtime);

private:
    bool m_initialized = false;
    std::shared_ptr<facebook::react::CallInvoker> m_invoker;
};
