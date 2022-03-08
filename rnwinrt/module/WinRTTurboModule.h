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

    // HostObject overrides
    virtual facebook::jsi::Value get(
        facebook::jsi::Runtime& runtime, const facebook::jsi::PropNameID& propName) override;

    // Functions exposed to JS
    void initialize(facebook::jsi::Runtime& runtime);

private:
    facebook::jsi::Value& get_namespace(facebook::jsi::Runtime& runtime, std::size_t index);

    bool m_initialized = false;
    std::shared_ptr<facebook::react::CallInvoker> m_invoker;
    std::vector<facebook::jsi::Value> m_rootNamespaces;
};
