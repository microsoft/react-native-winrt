#include "pch.h"

#include "CommandReader.h"
#include "FileGenerator.h"
#include "MetadataTypes.h"
#include "Settings.h"

namespace
{
    constexpr Option c_commandOptions[]{
        { "input", Option::NoMinArgs, Option::NoMaxArgs, "<spec>", "Windows metadata to include in projection" },
        { "reference" }, // Has the same effect as "input" with RN/WinRT and is include here to match C++/WinRT.
        { "output", Option::NoMinArgs, 1, "<path>", "Location of generated code" },
        { "tsoutput", Option::NoMinArgs, 1, "<path>", "Location of generated typescript type files" },
        { "include", Option::NoMinArgs, Option::NoMaxArgs, "<prefix>",
            "One or more prefixes to include in input. Essential Inclusions: Windows.Foundation" },
        { "filter" }, // Legacy name for "include" to match C++/WinRT.
        { "exclude", Option::NoMinArgs, Option::NoMaxArgs, "<prefix>",
            "One or more prefixes to exclude from input. Suggested Exclusions: Windows.Phone and Windows.Web.UI" },
        { "pch", 0, 1, "<name>", "Specify name of precompiled header file (defaults to pch.h)" },
        { "help", Option::NoMinArgs, Option::NoMaxArgs, {}, "Show detailed help with examples" },
        { "?" },
        { "allowforwebexclusively", Option::NoMinArgs, 0, {},
            "Restricts factories and statics exposed to JavaScript to those with AllowForWeb." },
        { "deprecatedincluded", Option::NoMinArgs, 0, {}, "Includes types and methods that are marked deprecated." },
        { "webhosthiddenincluded", Option::NoMinArgs, 0, {},
            "Includes types annotated with WebHostHidden that are intended to be hidden from JS callers." },
        { "verbose", Option::NoMinArgs, 0, {}, "Enables verbose logging" },
    };

    void PrintUsage()
    {
        std::cout << R"(React Native WinRT )" << CPPWINRT_VERSION_STRING << std::endl;
        std::cout << R"(Copyright (c) Microsoft Corporation. All rights reserved.)" << std::endl;
        std::cout << R"(  jswinrt.exe [options...])" << std::endl << std::endl;
        std::cout << R"(Options:)" << std::endl;

        for (const auto& option : c_commandOptions)
        {
            if (option.Description.empty())
            {
                continue;
            }

            std::stringstream arg;
            arg << "-" << option.Name << " " << option.Arg;
            arg.tellg();

            std::cout << "  ";
            std::cout.fill(' ');
            std::cout.width(28);
            std::cout << std::left << arg.str();
            std::cout.width(0);
            std::cout << option.Description << std::endl;
        }

        std::cout << R"(  @<path>             Response file containing command line options)" << std::endl;
        std::cout << R"(Where <spec> is one or more of:)" << std::endl;
        std::cout << R"(  path                Path to winmd file or recursively scanned folder)" << std::endl;
        std::cout << R"(  local               Local %WinDir%\System32\WinMetadata folder)" << std::endl;
        std::cout << R"("  sdk[+]              Current version of Windows SDK [with extensions])" << std::endl;
        std::cout << R"(  10.0.12345.0[+]     Specific version of Windows SDK [with extensions])" << std::endl;
    }
}

int main(int const argc, char** argv)
{
    try
    {
        CommandReader args(argc, argv, c_commandOptions);
        if (!args || args.Exists("help") || args.Exists("?"))
        {
            PrintUsage();
            return 1;
        }

        Settings settings(args);

        projection_data data;
        parse_metadata(settings, data);
        write_files(settings, data);
    }
    catch (std::exception const& e)
    {
        std::cout << "error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
