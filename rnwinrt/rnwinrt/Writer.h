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
