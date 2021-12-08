// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include <filesystem>
#include <string_view>

#include "MetadataTypes.h"

namespace rnwinrt
{
    struct writer
    {
        writer();

        writer(const writer&) = delete;
        writer& operator=(const writer&) = delete;

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

        template <typename Func, std::enable_if_t<std::is_invocable_v<Func, writer&>, int> = 0>
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
        bool should_write_file(const std::filesystem::path& path);

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

    struct file_writer : writer
    {
        file_writer(std::filesystem::path path) : m_path(std::move(path))
        {
        }

        ~file_writer()
        {
            flush_to_file(m_path);
        }

    private:
        std::filesystem::path m_path;
    };

    struct indent
    {
        int level;

        void operator()(writer& writer) const;
    };

    struct camel_case
    {
        std::string_view text;

        void operator()(writer& writer) const;
    };

    struct event_name
    {
        std::string_view text;

        void operator()(writer& writer) const;
    };

    struct fmt_guid
    {
        const GUID& value;

        void operator()(writer& writer) const;
    };

    struct cpp_namespace
    {
        const namespace_projection_data* data;

        void operator()(writer& writer) const;
    };

    struct cpp_typename
    {
        cpp_typename(std::string_view ns, std::string_view name) : type_namespace(ns), type_name(name)
        {
        }

        cpp_typename(winmd::reader::TypeDef typeDef) :
            type_namespace(typeDef.TypeNamespace()), type_name(typeDef.TypeName())
        {
        }

        void operator()(writer& writer) const;

        std::string_view type_namespace;
        std::string_view type_name;
    };

    void write_cppwinrt_type(writer& writer, winmd::reader::ElementType elemType);
    void write_cppwinrt_type(writer& writer, const winmd::reader::TypeDef& typeDef);
    void write_cppwinrt_type(writer& writer, system_guid_t);
    void write_cppwinrt_type(
        writer& writer, const winmd::reader::GenericTypeInstSig& sig, const generic_param_stack& genericParamStack);
    void write_cppwinrt_type(
        writer& writer, const winmd::reader::TypeSig& sig, const generic_param_stack& genericParamStack);
    void write_cppwinrt_type(writer& writer, const param_iterator& param);

    void write_cppwinrt_type_initializer(writer& writer, const winmd::reader::TypeSig& type);
}
