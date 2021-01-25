#pragma once

class TextWriter
{
private:
    std::vector<char> m_buffer;

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
                if (std::is_function<First>())
                {
                    first(this);
                }
                else
                {
                    Write(first);
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
    TextWriter() = default;
    virtual ~TextWriter() = default;

    void Write(const std::string_view& value)
    {
        m_buffer.insert(m_buffer.end(), value.begin(), value.end());
    }

    void Write(const char value)
    {
        m_buffer.push_back(value);
    }

    template <typename... Args>
    void Write(const std::string_view& format, Args const&... args)
    {
        FAIL_FAST_IF(CountSegmentPlaceholders(format) != sizeof...(Args));
        WriteSegment(format, args...);
    }

    void FlushToFile(const std::string_view& fileName)
    {
        std::ofstream file{ fileName, std::ios::out | std::ios::binary };
        file.write(m_buffer.data(), m_buffer.size());
        m_buffer.clear();
    }

    void FlushToFile(const std::filesystem::path& fileName)
    {
        FlushToFile(static_cast<std::string_view>(fileName.string()));
    }

};
