#include "pch.h"

#include "CommandReader.h"

namespace
{
    void CheckSdkRegRead(LSTATUS result)
    {
        if (result != ERROR_SUCCESS)
        {
            throw std::invalid_argument("Could not find the Windows SDK in the registry with error: " +
                                        std::to_string(HRESULT_FROM_WIN32(result)));
        }
    }

    wil::unique_hkey OpenSdk()
    {
        wil::unique_hkey key;
        CheckSdkRegRead(RegOpenKeyExW(
            HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots", 0, KEY_READ, &key));
        return key;
    }

    std::filesystem::path GetSdkPath()
    {
        auto key = OpenSdk();

        DWORD pathSizeBytes = 0;
        CheckSdkRegRead(RegQueryValueExW(key.get(), L"KitsRoot10", nullptr, nullptr, nullptr, &pathSizeBytes));

        std::wstring root((pathSizeBytes / sizeof(wchar_t)) - 1, L'?');
        CheckSdkRegRead(RegQueryValueExW(
            key.get(), L"KitsRoot10", nullptr, nullptr, reinterpret_cast<BYTE*>(root.data()), &pathSizeBytes));

        return root;
    }

    std::string GetModulePath()
    {
        std::string path(100, '?');
        DWORD actual_size{};

        while (true)
        {
            actual_size = GetModuleFileNameA(nullptr, path.data(), 1 + static_cast<uint32_t>(path.size()));

            if (actual_size < 1 + path.size())
            {
                path.resize(actual_size);
                break;
            }
            else
            {
                path.resize(path.size() * 2, '?');
            }
        }

        return path;
    }

    std::string GetSdkVersion()
    {
        const auto sdkPath = GetSdkPath();
        const std::regex rx(R"(((\d+)\.(\d+)\.(\d+)\.(\d+)))");

        std::cmatch match;
        if (std::regex_search(GetModulePath().c_str(), match, rx))
        {
            auto path = sdkPath / "Platforms\\UAP" / match[1].str() / "Platform.xml";

            if (std::filesystem::exists(path))
            {
                return match[1].str();
            }
        }

        const auto key = OpenSdk();
        uint32_t index{};
        std::array<char, 100> subkey;
        std::array<uint32_t, 4> versionParts{};
        std::string result;

        while (!RegEnumKeyA(key.get(), index++, subkey.data(), static_cast<uint32_t>(subkey.size())))
        {
            if (!std::regex_match(subkey.data(), match, rx))
            {
                continue;
            }

            if (!std::filesystem::exists(sdkPath / "Platforms\\UAP" / match[1].str() / "Platform.xml"))
            {
                continue;
            }

            PSTR nextPart = subkey.data();
            bool forceNewer = false;

            for (size_t i = 0;; ++i)
            {
                const auto versionPart = strtoul(nextPart, &nextPart, 10);

                if ((versionPart < versionParts[i]) && !forceNewer)
                {
                    break;
                }
                else if (versionPart > versionParts[i])
                {
                    // E.g. ensure something like '2.1' is considered newer than '1.2'
                    forceNewer = true;
                }

                versionParts[i] = versionPart;

                if (i == std::size(versionParts) - 1)
                {
                    result = subkey.data();
                    break;
                }

                if (!nextPart)
                {
                    break;
                }

                ++nextPart;
            }
        }

        if (result.empty())
        {
            throw std::invalid_argument("Could not find the Windows SDK");
        }

        return result;
    }

    void CheckXmlRead(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw std::invalid_argument("Could not read the Windows SDK's Platform.xml");
        }
    };

    void AddFilesFromXml(std::set<std::string>& files, const std::string& sdkVersion,
        const std::filesystem::path& xmlPath, const std::filesystem::path& sdkPath)
    {
        wil::com_ptr<IStream> stream;
        CheckXmlRead(SHCreateStreamOnFileW(xmlPath.c_str(), STGM_READ, &stream));

        wil::com_ptr<IXmlReader> reader;
        CheckXmlRead(CreateXmlReader(IID_PPV_ARGS(&reader), nullptr));
        CheckXmlRead(reader->SetInput(stream.get()));

        XmlNodeType node_type = XmlNodeType_None;
        while (S_OK == reader->Read(&node_type))
        {
            if (node_type != XmlNodeType_Element)
            {
                continue;
            }

            PCWSTR value{ nullptr };
            CheckXmlRead(reader->GetLocalName(&value, nullptr));

            if (0 != wcscmp(value, L"ApiContract"))
            {
                continue;
            }

            auto path = sdkPath;
            path /= L"References";
            path /= sdkVersion;

            CheckXmlRead(reader->MoveToAttributeByName(L"name", nullptr));
            CheckXmlRead(reader->GetValue(&value, nullptr));
            path /= value;

            CheckXmlRead(reader->MoveToAttributeByName(L"version", nullptr));
            CheckXmlRead(reader->GetValue(&value, nullptr));
            path /= value;

            CheckXmlRead(reader->MoveToAttributeByName(L"name", nullptr));
            CheckXmlRead(reader->GetValue(&value, nullptr));
            path /= value;

            path += L".winmd";
            files.insert(path.string());
        }
    }
}

CommandReader::CommandReader(const uint32_t argc, char** argv, const Option* const options, size_t optionsCount) :
    m_options(options), m_optionsCount(optionsCount)
{
    if (argc < 2)
    {
        return;
    }

    const Option* last = nullptr;
    for (uint32_t i = 1; i < argc; ++i)
    {
        ExtractOption(argv[i], last);
    }

    for (uint32_t i = 0; i < optionsCount; ++i)
    {
        const auto& option = options[i];

        auto args = m_extractedOptions.find(option.Name);
        std::size_t const count = args == m_extractedOptions.end() ? 0 : args->second.size();

        if (option.MinArgs == Option::NoMinArgs && option.MaxArgs == Option::NoMinArgs && count > 0)
        {
            ThrowInvalidArg("Option '", option.Name, "' does not accept a value. Received: " + std::to_string(count));
        }
        else if (option.MaxArgs == option.MinArgs && count != option.MaxArgs)
        {
            ThrowInvalidArg(
                "Option '", option.Name, "' requires exactly ", std::to_string(option.MaxArgs), " value(s)");
        }
        else if (count < option.MinArgs)
        {
            ThrowInvalidArg(
                "Option '", option.Name, "' requires at least ", std::to_string(option.MinArgs), " value(s)");
        }
        else if (count > option.MaxArgs)
        {
            ThrowInvalidArg("Option '", option.Name, "' accepts at most ", std::to_string(option.MaxArgs), " value(s)");
        }
    }
}

CommandReader::operator bool() const noexcept
{
    return !m_extractedOptions.empty();
}

bool CommandReader::Exists(const std::string_view& name) const noexcept
{
    return m_extractedOptions.count(name);
}

const std::vector<std::string>& CommandReader::Values(const std::string_view& name) const noexcept
{
    const auto result = m_extractedOptions.find(name);

    if (result == m_extractedOptions.end())
    {
        static std::vector<std::string> empty{};
        return empty;
    }

    return result->second;
}

std::string CommandReader::Value(const std::string_view& name, const std::string_view& defaultValue) const
{
    const auto result = m_extractedOptions.find(name);

    if (result == m_extractedOptions.end() || result->second.empty())
    {
        return std::string(defaultValue);
    }

    return result->second.front();
}

std::set<std::string> CommandReader::Files(
    const std::string_view& name, std::function<bool(std::string_view)> directoryFilter) const
{
    std::set<std::string> files;

    auto add_directory = [&](auto&& path) {
        for (auto&& file : std::filesystem::directory_iterator(path))
        {
            if (std::filesystem::is_regular_file(file))
            {
                auto filename = file.path().string();

                if (directoryFilter && directoryFilter(filename))
                {
                    files.insert(filename);
                }
            }
        }
    };

    for (auto&& path : Values(name))
    {
        if (std::filesystem::is_directory(path))
        {
            add_directory(std::filesystem::canonical(path));
            continue;
        }

        if (std::filesystem::is_regular_file(path))
        {
            files.insert(std::filesystem::canonical(path).string());
            continue;
        }
        if (path == "local")
        {
            std::array<char, 260> local{};
#ifdef _WIN64
            ExpandEnvironmentStringsA(
                "%windir%\\System32\\WinMetadata", local.data(), static_cast<uint32_t>(local.size()));
#else
            ExpandEnvironmentStringsA(
                "%windir%\\SysNative\\WinMetadata", local.data(), static_cast<uint32_t>(local.size()));
#endif
            add_directory(local.data());
            continue;
        }

        std::string sdk_version;

        if (path == "sdk" || path == "sdk+")
        {
            sdk_version = GetSdkVersion();
        }
        else
        {
            std::regex rx(R"(((\d+)\.(\d+)\.(\d+)\.(\d+))\+?)");
            std::smatch match;

            if (std::regex_match(path, match, rx))
            {
                sdk_version = match[1].str();
            }
        }

        if (!sdk_version.empty())
        {
            auto sdk_path = GetSdkPath();
            auto xml_path = sdk_path;
            xml_path /= L"Platforms\\UAP";
            xml_path /= sdk_version;
            xml_path /= L"Platform.xml";

            AddFilesFromXml(files, sdk_version, xml_path, sdk_path);

            if (path.back() != '+')
            {
                continue;
            }

            for (auto&& item : std::filesystem::directory_iterator(sdk_path / L"Extension SDKs"))
            {
                xml_path = item.path() / sdk_version;
                xml_path /= L"SDKManifest.xml";

                AddFilesFromXml(files, sdk_version, xml_path, sdk_path);
            }

            continue;
        }

        ThrowInvalidArg("Path '", path, "' is not a file or directory");
    }

    return files;
}

const Option* CommandReader::Find(const std::string_view& name) const
{
    std::string normalized(name);

#pragma warning(push)
#pragma warning(disable : 4244) // conversion from 'int' to 'char', possible loss of data
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
#pragma warning(pop)

    for (uint32_t i = 0; i < m_optionsCount; ++i)
    {
        const auto& current = m_options[i];
        if (!current.Name.compare(0, normalized.size(), normalized))
        {
            return &current;
        }
    }
    return nullptr;
}

void CommandReader::ExtractOption(std::string_view arg, const Option*& last)
{
    if (arg[0] == '-' || arg[0] == '/')
    {
        arg.remove_prefix(1);

        last = Find(arg);
        if (!last)
        {
            ThrowInvalidArg("Option '-", arg, "' is not supported");
        }

        m_extractedOptions.try_emplace(last->Name);
    }
    else if (arg[0] == '@')
    {
        arg.remove_prefix(1);
        ExtractResponseFile(arg, last);
    }
    else if (!last)
    {
        ThrowInvalidArg("Value '", arg, "' is not supported");
    }
    else
    {
        m_extractedOptions[last->Name].push_back(std::string{ arg });
    }
}

void CommandReader::ExtractResponseFile(const std::string_view& name, const Option*& last)
{
    std::filesystem::path responsePath{ name };
    std::string extension = responsePath.extension().generic_string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
        [](auto c) { return static_cast<unsigned char>(::tolower(c)); });

    // Check if misuse of @ prefix, so if directory or metadata file instead of response file.
    if (std::filesystem::is_directory(responsePath) || extension == ".winmd")
    {
        ThrowInvalidArg("'@' is reserved for response files");
    }

    std::string line_buf;
    std::ifstream response_file(absolute(responsePath));
    while (getline(response_file, line_buf))
    {
        size_t argc = 0;
        std::vector<std::string> argv;

        ParseCommandLine(line_buf.data(), argv, &argc);

        for (size_t i = 0; i < argc; i++)
        {
            ExtractOption(argv[i], last);
        }
    }
}

void CommandReader::ParseCommandLine(PCSTR cmdstart, std::vector<std::string>& argv, size_t* argCount)
{
    std::string arg;
    bool copyCharacter;
    unsigned backslashCount;
    bool inQuotes;
    bool first_arg;

    PCSTR p = cmdstart;
    inQuotes = false;
    first_arg = true;
    *argCount = 0;

    for (;;)
    {
        if (*p)
        {
            while (*p == ' ' || *p == '\t')
                ++p;
        }

        if (!first_arg)
        {
            argv.emplace_back(arg);
            arg.clear();
            ++*argCount;
        }

        if (*p == '\0')
            break;

        for (;;)
        {
            copyCharacter = true;

            // Rules:
            // 2N     backslashes   + " ==> N backslashes and begin/end quote
            // 2N + 1 backslashes   + " ==> N backslashes + literal "
            // N      backslashes       ==> N backslashes
            backslashCount = 0;

            while (*p == '\\')
            {
                ++p;
                ++backslashCount;
            }

            if (*p == '"')
            {
                // if 2N backslashes before, start/end quote, otherwise
                // copy literally:
                if (backslashCount % 2 == 0)
                {
                    if (inQuotes && p[1] == '"')
                    {
                        p++; // Double quote inside quoted string
                    }
                    else
                    {
                        // Skip first quote char and copy second:
                        copyCharacter = false;
                        inQuotes = !inQuotes;
                    }
                }

                backslashCount /= 2;
            }

            while (backslashCount--)
            {
                arg.push_back('\\');
            }

            if (*p == '\0' || (!inQuotes && (*p == ' ' || *p == '\t')))
                break;

            if (copyCharacter)
            {
                arg.push_back(*p);
            }

            ++p;
        }

        first_arg = false;
    }
}