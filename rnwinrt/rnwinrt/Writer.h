#pragma once

#include <filesystem>
#include <string_view>

#include "MetadataTypes.h"

struct jswinrt_writer
{
    jswinrt_writer();

    jswinrt_writer(const jswinrt_writer&) = delete;
    jswinrt_writer& operator=(const jswinrt_writer&) = delete;

    void flush_to_file(const std::filesystem::path& path);

    // Write functions
    void write(std::string_view str)
    {
        m_buffer.insert(m_buffer.end(), str.begin(), str.end());
    }

    void write(char ch)
    {
        m_buffer.push_back(ch);
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    void write(T value)
    {
        if constexpr (std::is_same_v<T, bool>)
        {
            write(value ? "true" : "false");
        }
        else
        {
            write(std::to_string(value));
        }
    }

    template <typename Func, std::enable_if_t<std::is_invocable_v<Func, jswinrt_writer&>, int> = 0>
    void write(Func&& fn)
    {
        fn(*this);
    }

    template <typename... Args>
    void write_fmt(std::string_view fmtString, Args&&... args)
    {
        assert(count_placeholders(fmtString) == sizeof...(args));
        write_fmt_impl(fmtString, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void write_fmt_temp(std::string_view fmtString, Args&&... args)
    {
        assert(count_placeholders(fmtString) == sizeof...(args));

        auto size = m_buffer.size();
        write_fmt_impl(fmtString, std::forward<Args>(args)...);
        std::string result(m_buffer.data() + size, m_buffer.size() - size);

        m_buffer.resize(size);
        return result;
    }

private:

    void write_fmt_impl(std::string_view fmtString);

    template <typename T, typename... Args>
    void write_fmt_impl(std::string_view fmtString, T&& value, Args&&... args)
    {
        auto pos = fmtString.find_first_of("^%");
        assert(pos != std::string_view::npos);
        write(fmtString.substr(0, pos));

        if (auto ch = fmtString[pos]; ch == '^')
        {
            assert(pos <= (fmtString.size() - 2));
            write(fmtString[pos + 1]);
            write_fmt_impl(fmtString.substr(pos + 2), std::forward<T>(value), std::forward<Args>(args)...);
        }
        else if (ch == '%')
        {
            write(std::forward<T>(value));
            write_fmt_impl(fmtString.substr(pos + 1), std::forward<Args>(args)...);
        }
    }

    static std::size_t count_placeholders(std::string_view str);

    std::vector<char> m_buffer;
};

struct jswinrt_file_writer : jswinrt_writer
{
    jswinrt_file_writer(std::filesystem::path path) : m_path(std::move(path))
    {
    }

    ~jswinrt_file_writer()
    {
        flush_to_file(m_path);
    }

private:

    std::filesystem::path m_path;
};

struct indent
{
    int level;

    void operator()(jswinrt_writer& writer) const;
};

struct camel_case
{
    std::string_view text;

    void operator()(jswinrt_writer& writer) const;
};

struct event_name
{
    std::string_view text;

    void operator()(jswinrt_writer& writer) const;
};

struct fmt_guid
{
    const GUID& value;

    void operator()(jswinrt_writer& writer) const;
};







#if 1
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
#endif
