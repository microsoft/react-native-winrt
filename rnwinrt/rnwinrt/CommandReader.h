// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

struct Option
{
    static constexpr uint32_t NoMinArgs = 0;
    static constexpr uint32_t NoMaxArgs = UINT_MAX;

    std::string_view Name;
    uint32_t MinArgs{ NoMinArgs };
    uint32_t MaxArgs{ NoMaxArgs };
    std::string_view Arg{};
    std::string_view Description{};
};

class CommandReader final
{
public:
    template <size_t NumOptions>
    CommandReader(const uint32_t argc, char** argv, const Option (&options)[NumOptions]) :
        CommandReader(argc, argv, &options[0], NumOptions)
    {
    }

    CommandReader(const uint32_t argc, char** argv, const Option* const options, size_t optionsCount);

    explicit operator bool() const noexcept;

    bool Exists(const std::string_view& name) const noexcept;
    const std::vector<std::string>& Values(const std::string_view& name) const noexcept;
    std::string Value(const std::string_view& name, const std::string_view& default_value = {}) const;
    std::set<std::string> Files(
        const std::string_view& name, std::function<bool(std::string_view)> directoryFilter = nullptr) const;

private:
    const Option* Find(const std::string_view& name) const;
    void ExtractOption(std::string_view arg, const Option*& last);
    void ExtractResponseFile(const std::string_view& arg, const Option*& last);
    static void ParseCommandLine(PCSTR cmdstart, std::vector<std::string>& argv, size_t* argCount);

    const Option* const m_options;
    const size_t m_optionsCount;
    std::map<std::string_view, std::vector<std::string>> m_extractedOptions;
};