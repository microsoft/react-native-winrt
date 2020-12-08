
#define _CRT_SECURE_NO_WARNINGS // fopen is not insecure...

#include <cassert>
#include <cstdio>
#include <filesystem>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool write_file(FILE* file, std::string_view buffer)
{
    auto result = std::fwrite(buffer.data(), buffer.size(), 1, file) == 1;
    if (!result)
    {
        std::printf("ERROR: Failed to write to file\n");
        std::printf("ERROR: %s\n", std::generic_category().message(errno).c_str());
    }

    return result;
}

template <typename... Args>
bool print_file(FILE* file, const char* fmt, Args... args)
{
    if (std::fprintf(file, fmt, args...) < 0)
    {
        std::printf("ERROR: Failed to write to file\n");
        std::printf("ERROR: %s\n", std::generic_category().message(errno).c_str());
        return false;
    }

    return true;
}

int write_file_contents(FILE* outFile, const std::filesystem::path& inFilePath);

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::printf("ERROR: Improper usage\n");
        std::printf("ERROR:     strings.exe <inputPath> <outputPath>\n");
        return ERROR_INVALID_PARAMETER;
    }

    std::filesystem::path toPath{ argv[2] };
    if (auto toStatus = std::filesystem::status(toPath); !std::filesystem::exists(toStatus))
    {
        // Destination path does not exist. We'll assume that it's a directory unless it has a '.h' extension
        if (toPath.extension() == L".h")
        {
            auto toDir = toPath.parent_path();
            if (auto toDirStatus = std::filesystem::status(toDir); !std::filesystem::exists(toDirStatus))
            {
                std::error_code ec;
                if (!std::filesystem::create_directory(toDir, ec))
                {
                    std::printf("ERROR: Failed to create output directory '%ls'\n", toDir.c_str());
                    std::printf("ERROR: %s\n", ec.message().c_str());
                    return ec.value();
                }
            }
            else if (!std::filesystem::is_directory(toDirStatus))
            {
                std::printf(
                    "ERROR: Cannot write to file '%ls' because its parent path is not a directory\n", toPath.c_str());
                return ERROR_INVALID_PARAMETER;
            }
        }
        else
        {
            std::error_code ec;
            if (!std::filesystem::create_directory(toPath, ec))
            {
                std::printf("ERROR: Failed to create output directory '%ls'\n", toPath.c_str());
                std::printf("ERROR: %s\n", ec.message().c_str());
                return ec.value();
            }

            toPath /= L"strings.h";
        }
    }
    else if (std::filesystem::is_directory(toStatus))
    {
        toPath /= L"strings.h";
    }

    std::filesystem::path fromPath{ argv[1] };
    auto fromStatus = std::filesystem::status(fromPath);
    if (!std::filesystem::exists(fromStatus))
    {
        std::printf("ERROR: The input path '%s' does not exist\n", argv[1]);
        return ERROR_FILE_NOT_FOUND;
    }

// Far enough along to assume likely success; go ahead and create the output file
    auto outFile = _wfopen(toPath.c_str(), L"w");
    if (!outFile)
    {
        std::printf("ERROR: Failed to create file '%ls'\n", toPath.c_str());
        std::printf("ERROR: %s\n", std::generic_category().message(errno).c_str());
        return errno;
    }

    if (!write_file(outFile, R"^-^(#pragma once

#include <string_view>

using namespace std::literals;

struct file_data
{
    const char* file_name;
    std::initializer_list<std::string_view> file_contents;
};

inline const file_data file_strings[] =
{
)^-^"))
    {
        return errno;
    }

    if (std::filesystem::is_directory(fromStatus))
    {
        std::error_code ec;
        for (auto&& entry : std::filesystem::directory_iterator(fromPath, ec))
        {
            // TODO: Scope to specific extensions only?
            if (entry.is_regular_file())
            {
                if (auto code = write_file_contents(outFile, entry.path()))
                    return code;
            }
        }

        if (ec)
        {
            std::printf("ERROR: Failed to iterate directory '%ls'\n", fromPath.c_str());
            std::printf("ERROR: %s\n", ec.message().c_str());
            return ec.value();
        }
    }
    else
    {
        // Assume a single file
        if (auto code = write_file_contents(outFile, fromPath))
            return code;
    }

    if (!write_file(outFile, "};\n"))
    {
        return errno;
    }

    std::fclose(outFile);

    return 0;
}

int write_file_contents(FILE* outFile, const std::filesystem::path& inFilePath)
{
    auto inFile = _wfopen(inFilePath.c_str(), L"r");
    if (!inFile)
    {
        std::printf("ERROR: Failed to open input file '%ls'\n", inFilePath.c_str());
        std::printf("ERROR: %s\n", std::generic_category().message(errno).c_str());
        return errno;
    }

    if (!print_file(outFile, R"^-^(    {
        "%ls",
        {
)^-^", inFilePath.filename().c_str()))
    {
        std::fclose(inFile);
        return errno;
    }

    // NOTE: MSVC has a max string literal length that seems to be on the order of ~16,000 bytes without string
    // concatenation. Target something slightly less than that for some wiggle room
    static constexpr std::size_t buffer_size = 16000;
    char buffer[buffer_size];
    std::size_t bufferLen = 0;
    bool hasData = true;
    while (hasData)
    {
        auto readLen = std::fread(buffer + bufferLen, sizeof(char), buffer_size - bufferLen, inFile);
        hasData = readLen != 0; // TODO: Check for errors? Possible?
        bufferLen += readLen;

        // In case the last character ended in the middle of a 2+ byte unicode character
        auto dataLen = bufferLen;
        while (dataLen && (buffer[dataLen - 1] & 0x80))
        {
            --dataLen;
        }

        if (!print_file(outFile, "            R\"xyz(%.*s)xyz\"sv,\n", static_cast<int>(dataLen), buffer))
        {
            std::fclose(inFile);
            return errno;
        }

        bufferLen -= dataLen;
        // NOTE: May technically overlap, however the only time this copy matters is when copying very few bytes from
        // the very end of the array to the very beginning
        std::memcpy(buffer, buffer + dataLen, bufferLen);
    }

    assert(bufferLen == 0);

    if (!write_file(outFile, R"^-^(        }
    },
)^-^"))
    {
        std::fclose(inFile);
        return errno;
    }

    std::fclose(inFile);
    return 0;
}
