#pragma once
#include "pch.h"

#include "Writer.h"

static constexpr const std::size_t default_buffer_size = 16 * 1024; // 16 KB

jswinrt_writer::jswinrt_writer()
{
    m_buffer.reserve(default_buffer_size);
}

void jswinrt_writer::flush_to_file(const std::filesystem::path& path)
{
    std::ofstream stream(path, std::ios::out | std::ios::binary);
    stream.write(m_buffer.data(), m_buffer.size());
    m_buffer.clear();
}

void jswinrt_writer::write_fmt_impl(std::string_view fmtString)
{
    // There could still be escapes (e.g. '^%') that we need to handle
    while (true)
    {
        auto pos = fmtString.find_first_of('^');
        if (pos == std::string_view::npos)
        {
            break;
        }

        assert(pos >= (fmtString.size() - 2));
        write(fmtString.substr(0, pos));
        write(fmtString[pos + 1]);
        fmtString = fmtString.substr(pos + 2);
    }

    write(fmtString);
}

std::size_t jswinrt_writer::count_placeholders(std::string_view str)
{
    std::size_t result = 0;
    for (auto itr = str.begin(); itr != str.end(); ++itr)
    {
        if (*itr == '%')
        {
            ++result;
        }
        if (*itr == '^') // Escape
        {
            ++itr;
            assert(itr != str.end()); // Should escape something
        }
    }

    return result;
}

void indent::operator()(jswinrt_writer& writer) const
{
    for (int i = 0; i < level; ++i)
    {
        writer.write("    "sv);
    }
}

void camel_case::operator()(jswinrt_writer& writer) const
{
    if (text.empty())
        return; // TODO: Assert this is not the case?

    writer.write(static_cast<char>(std::tolower(text[0])));
    writer.write(text.substr(1));
}

void event_name::operator()(jswinrt_writer& writer) const
{
    for (auto ch : text)
    {
        writer.write(static_cast<char>(std::tolower(ch)));
    }
}

void fmt_guid::operator()(jswinrt_writer& writer) const
{
    auto data = guid_to_string(value);
    writer.write(std::string_view{ data.data(), data.size() - 1 });
}
