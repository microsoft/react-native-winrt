// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

class TextWriter
{
private:
    std::vector<char> m_buffer;
    uint16_t indent = 0;

    void WriteSegment(const std::string_view& value)
    {
        auto offset = value.find_first_of("^");
        if (offset == std::string_view::npos)
        {
            Write(value);
            return;
        }

        Write(value.substr(0, offset));

        FAIL_FAST_IF(offset >= value.size() - 1);
        Write(value[offset + 1]);
        WriteSegment(value.substr(offset + 2));
    }

    template <typename First, typename... Rest>
    void WriteSegment(const std::string_view& value, First const& first, Rest const&... rest)
    {
        auto offset = value.find_first_of("^%@");
        FAIL_FAST_IF(offset == std::string_view::npos);
        Write(value.substr(0, offset));

        if (value[offset] == '^')
        {
            FAIL_FAST_IF(offset >= value.size() - 1);
            Write(value[offset + 1]);
            WriteSegment(value.substr(offset + 2), first, rest...);
        }
        else
        {
            if (value[offset] == '%')
            {
                if constexpr (std::is_invocable_v<First>)
                {
                    first();
                }
                else if constexpr (std::is_convertible_v<First, std::string_view>)
                {
                    Write(first);
                }
                else if constexpr (std::is_arithmetic_v<First>)
                {
                    Write(first);
                }
                else
                {
                    FAIL_FAST();
                }
            }
            else
            {
                if constexpr (std::is_convertible_v<First, std::string_view>)
                {
                    Write(first);
                }
                else
                {
                    FAIL_FAST(); // '@' placeholders are only for text.
                }
            }

            WriteSegment(value.substr(offset + 1), rest...);
        }
    }

    uint32_t CountSegmentPlaceholders(const std::string_view& format) noexcept
    {
        uint32_t count{};
        bool escape{};

        for (auto c : format)
        {
            if (!escape)
            {
                if (c == '^')
                {
                    escape = true;
                    continue;
                }

                if (c == '%' || c == '@')
                {
                    ++count;
                }
            }
            escape = false;
        }

        return count;
    }

public:
    void Write(const std::string_view& value)
    {
        m_buffer.insert(m_buffer.end(), value.begin(), value.end());
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    void Write(T value)
    {
        // TODO: Could be more efficient w/ std::to_chars or something similar
        Write(std::to_string(value));
    }

    void Write(const char value)
    {
        m_buffer.push_back(value);
    }

    void DeleteLastCharacter()
    {
        m_buffer.pop_back();
    }

    void DeleteLast(int n)
    {
        for (int i = 0; i < n; i++)
            m_buffer.pop_back();
    }

    template <typename... Args>
    void Write(const std::string_view& format, Args const&... args)
    {
        FAIL_FAST_IF(CountSegmentPlaceholders(format) != sizeof...(Args));
        WriteSegment(format, args...);
    }

    template <typename... Args>
    void WriteIndentedLine(const std::string_view& format, Args const&... args)
    {
        FAIL_FAST_IF(CountSegmentPlaceholders(format) != sizeof...(Args));
        Write('\n');
        for (int i = 0; i < indent * 4; i++)
        {
            Write(' ');
        }
        WriteSegment(format, args...);
    }

    void WriteIndentedLine()
    {
        WriteIndentedLine(""sv);
    }

    void WriteBlankLine()
    {
        Write("\n");
    }

    void FlushToFile(const std::string_view& fileName)
    {
        FlushToFile(fileName);
    }

    void FlushToFile(const std::filesystem::path& fileName)
    {
        std::ofstream file{ fileName, std::ios::out | std::ios::binary };
        file.write(m_buffer.data(), m_buffer.size());
        m_buffer.clear();
    }

    void AddIndent()
    {
        indent++;
    }

    void ReduceIndent()
    {
        indent--;
        if (indent < 0)
        {
            FAIL_FAST();
        }
    }

    static std::string ToCamelCase(std::string value)
    {
        if (!value.empty())
        {
            value[0] = static_cast<char>(tolower(value[0]));
        }
        return value;
    }

    static std::string ToLowerAllCase(std::string value)
    {
        std::transform(
            value.begin(), value.end(), value.begin(), [](char ch) { return static_cast<char>(tolower(ch)); });
        return value;
    }
};
