#pragma once

struct Writer
{
    Writer();
    virtual ~Writer() = default;

    Writer(Writer const&) = delete;
    Writer& operator=(Writer const&) = delete;

    void Swap();

    // File
    void FlushToFile(const std::string_view& fileName);
    void FlushToFile(const std::filesystem::path& fileName);

    bool IsFileEqual(const std::string_view& fileName) const;
    bool IsFileEqual(const std::filesystem::path& fileName) const;

    // General
    void Write(const std::string_view& value)
    {
        m_first.insert(m_first.end(), value.begin(), value.end());
    }

    void Write(const char value)
    {
        m_first.push_back(value);
    }

    template <typename T, std::enable_if_t<std::is_same_v<T, bool>, int> = 0>
    void Write(const T value)
    {
        Write(value ? "true"sv : "false"sv);
    }

    template <typename T,
        std::enable_if_t<(std::is_integral_v<T> && !std::is_same_v<T, bool>) || std::is_floating_point_v<T>, int> = 0>
    void Write(const T value)
    {
        Write(std::to_string(value));
    }

    void Write(const winmd::reader::Constant& value);
    void Write(winmd::reader::GenericParam const& param);
    void Write(const winmd::reader::TypeDef& type);
    void Write(const winmd::reader::TypeRef& type);
    void Write(const winmd::reader::coded_index<winmd::reader::TypeDefOrRef>& type);
    void Write(const winmd::reader::GenericTypeInstSig& type);
    void Write(const winmd::reader::TypeSig& signature);
    void Write(const winmd::reader::TypeSig::value_type& type);

    template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, Writer&>>>
    void Write(const F& fn)
    {
        fn(*this);
    }

    template <typename... Args>
    void Write(const std::string_view& format, Args const&... args)
    {
        FAIL_FAST_IF(CountSegmentPlaceholders(format) != sizeof...(Args));
        WriteSegment(format, args...);
    }

    template <typename... Args>
    std::string WriteTemp(const std::string_view& format, Args const&... args)
    {
        const auto size = m_first.size();

        FAIL_FAST_IF(CountSegmentPlaceholders(format) != sizeof...(Args));
        WriteSegment(format, args...);

        std::string result{ m_first.data() + size, m_first.size() - size };
        m_first.resize(size);

        return result;
    }

    template <typename... Args>
    void WritePrintF(PCSTR format, Args const&... args)
    {
#pragma warning(push)
#pragma warning(disable : 4996) // snprintf is not as secure as _snprintf_s but the latter doesn't return the number of
                                // characters that would be stored.
        char stackBuffer[256];
        size_t size = snprintf(stackBuffer, ARRAYSIZE(stackBuffer), format, args...);
        FAIL_FAST_IF(size < 0);

        if (size < ARRAYSIZE(stackBuffer))
        {
            Write(std::string_view{ stackBuffer, size });
        }
        else
        {
            const size_t heapBufferSize = size + 1;
            const auto heapBuffer = std::make_unique<char[]>(heapBufferSize);
            size = _snprintf(heapBuffer.get(), heapBufferSize, format, args...);
            FAIL_FAST_IF(size != heapBufferSize - 1);

            Write(std::string_view{ heapBuffer.get(), size });
        }
#pragma warning(pop)
    }

    void WriteCode(const std::string_view& value);
    void WriteInclude(const std::string_view& fileName);
    void WriteNewLine();

    template <typename T>
    static auto BindList(const std::string_view& delimiter, const T& list)
    {
        return [&](Writer& writer) {
            bool first{ true };

            for (const auto& item : list)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    writer.Write(delimiter);
                }

                writer.Write(item);
            }
        };
    }

    static std::string_view RemoveTick(const std::string_view& name);
    static std::string ToLowerCamelCase(const std::string_view& value);
    static std::string ToLowerCase(const std::string_view& value);

private:
    void WriteSegment(const std::string_view& value);

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
                Write(first);
            }
            else
            {
                if constexpr (std::is_convertible_v<First, std::string_view>)
                {
                    WriteCode(first);
                }
                else
                {
                    FAIL_FAST(); // '@' placeholders are only for text.
                }
            }

            WriteSegment(value.substr(offset + 1), rest...);
        }
    }

    uint32_t CountSegmentPlaceholders(const std::string_view& format) noexcept;

    std::vector<char> m_first;
    std::vector<char> m_second;
};

class FileWriter final : public Writer
{
public:
    FileWriter(const std::string_view& fileName) : m_fileName(fileName)
    {
    }

    FileWriter(const std::filesystem::path& fileName) : m_fileName(fileName.string())
    {
    }

    virtual ~FileWriter()
    {
        FlushToFile(static_cast<std::string_view>(m_fileName));
    }

private:
    const std::string m_fileName;
};