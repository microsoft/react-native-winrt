// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"

#include "Writer.h"

using namespace winmd::reader;

static constexpr const std::size_t default_buffer_size = 16 * 1024; // 16 KB

namespace rnwinrt
{
    writer::writer()
    {
        m_buffer.reserve(default_buffer_size);
    }

    void writer::flush_to_file(const std::filesystem::path& path)
    {
        if (!should_write_file(path))
            return;

        std::ofstream stream(path, std::ios::out | std::ios::binary);
        stream.write(m_buffer.data(), m_buffer.size());
        m_buffer.clear();
    }

    bool writer::should_write_file(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
            return true;

        wil::unique_handle handle{ ::CreateFileW(path.c_str(), GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
            nullptr) };
        if (!handle)
            return true;

        LARGE_INTEGER size;
        if (!::GetFileSizeEx(handle.get(), &size))
            return true;
        if (size.QuadPart != static_cast<LONGLONG>(m_buffer.size()))
            return true;

        // File sizes are the same. Check the full contents
        std::size_t offset = 0;
        char buffer[256];
        while (offset != m_buffer.size())
        {
            DWORD bytesRead;
            if (!::ReadFile(handle.get(), buffer, static_cast<DWORD>(std::size(buffer)), &bytesRead, nullptr) ||
                !bytesRead)
                return true;

            auto newOffset = offset + bytesRead;
            if (newOffset > m_buffer.size())
                return true; // File size concurrently changed; avoid reading past the end of the buffer

            if (std::memcmp(m_buffer.data() + offset, buffer, bytesRead) != 0)
                return true;

            offset = newOffset;
        }

        // If we've gotten this far, it means the file contents are the same. We should not write to the file
        return false;
    }

    void writer::write_fmt_impl(std::string_view fmtString)
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

    std::size_t writer::count_placeholders(std::string_view str)
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

    void indent::operator()(writer& writer) const
    {
        for (int i = 0; i < level; ++i)
        {
            writer.write("    "sv);
        }
    }

    void camel_case::operator()(writer& writer) const
    {
        assert(!text.empty());
        if (text.empty())
            return;

        writer.write(static_cast<char>(std::tolower(text[0])));
        writer.write(text.substr(1));
    }

    void event_name::operator()(writer& writer) const
    {
        for (auto ch : text)
        {
            writer.write(static_cast<char>(std::tolower(ch)));
        }
    }

    void fmt_guid::operator()(writer& writer) const
    {
        auto data = guid_to_string(value);
        writer.write(std::string_view{ data.data(), data.size() - 1 });
    }

    void write_cppwinrt_type(writer& writer, ElementType elemType)
    {
        std::string_view str;
        switch (elemType)
        {
        case ElementType::Boolean:
            str = "bool"sv;
            break;
        case ElementType::Char:
            str = "char16_t"sv;
            break;
        case ElementType::I1:
            str = "int8_t"sv;
            break;
        case ElementType::U1:
            str = "uint8_t"sv;
            break;
        case ElementType::I2:
            str = "int16_t"sv;
            break;
        case ElementType::U2:
            str = "uint16_t"sv;
            break;
        case ElementType::I4:
            str = "int32_t"sv;
            break;
        case ElementType::U4:
            str = "uint32_t"sv;
            break;
        case ElementType::I8:
            str = "int64_t"sv;
            break;
        case ElementType::U8:
            str = "uint64_t"sv;
            break;
        case ElementType::R4:
            str = "float"sv;
            break;
        case ElementType::R8:
            str = "double"sv;
            break;
        case ElementType::String:
            str = "winrt::hstring"sv;
            break;
        case ElementType::Object:
            str = "winrt::Windows::Foundation::IInspectable"sv;
            break;
        default:
            assert(false);
            throw std::runtime_error("Unexpected ElementType");
        }
        writer.write(str);
    }

    void write_cppwinrt_type(writer& writer, const TypeDef& typeDef)
    {
        assert(!is_generic(typeDef));
        writer.write_fmt("winrt::%", cpp_typename{ typeDef });
    }

    void write_cppwinrt_type(writer& writer, system_guid_t)
    {
        writer.write("winrt::guid");
    }

    void write_cppwinrt_type(
        writer& writer, const GenericTypeInstSig& sig, const generic_param_stack& genericParamStack)
    {
        auto [ns, name] = get_type_namespace_and_name(sig.GenericType());
        writer.write_fmt("winrt::%<", cpp_typename{ ns, remove_tick(name) });

        std::string_view prefix;
        for (auto&& arg : sig.GenericArgs())
        {
            writer.write(prefix);
            write_cppwinrt_type(writer, arg, genericParamStack);
            prefix = ", ";
        }

        writer.write(">");
    }

    void write_cppwinrt_type(writer& writer, const TypeSig& sig, const generic_param_stack& genericParamStack)
    {
        resolve_type(sig, genericParamStack,
            overloaded{
                [&](const GenericTypeInstSig& genericSig, const generic_param_stack& newStack, bool isArray) {
                    assert(!isArray);
                    write_cppwinrt_type(writer, genericSig, newStack);
                },
                [&](auto&& value, bool isArray) {
                    assert(!isArray);
                    write_cppwinrt_type(writer, value);
                },
            });
    }

    void write_cppwinrt_type(writer& writer, const param_iterator& param)
    {
        auto writeArrayBegin = [&](bool isArray) {
            if (isArray)
            {
                if (param.is_input())
                {
                    assert(!param.by_ref()); // What would the semantics of this even be?
                    writer.write("winrt::array_view<const ");
                }
                else if (!param.by_ref())
                {
                    // Out, but not ByRef is how fill arrays are indicated
                    writer.write("winrt::array_view<");
                }
                else
                {
                    writer.write("winrt::com_array<");
                }
            }
        };

        auto writeArrayEnd = [&](bool isArray) {
            if (isArray)
            {
                writer.write(">");
            }
        };

        // NOTE: We don't manually project generic types, so we can get away with assuming an empty generic param stack
        resolve_type(param.signature(), {},
            overloaded{
                [&](const GenericTypeInstSig& sig, const generic_param_stack& newStack, bool isArray) {
                    writeArrayBegin(isArray);
                    write_cppwinrt_type(writer, sig, newStack);
                    writeArrayEnd(isArray);
                },
                [&](auto&& value, bool isArray) {
                    writeArrayBegin(isArray);
                    write_cppwinrt_type(writer, value);
                    writeArrayEnd(isArray);
                },
            });
    }

    void write_cppwinrt_type_initializer(writer& writer, const TypeSig& type)
    {
        // Classes need to be initialized with 'nullptr' because otherwise it's a constructor invocation
        // NOTE: Similar to the above, we aren't projecting generic types, so we can get away with an empty param stack
        resolve_type(type, {},
            overloaded{
                [&](const TypeDef& type) {
                    auto c = get_category(type);
                    if (c == category::class_type)
                    {
                        writer.write("{ nullptr }");
                    }
                },
                [&](const TypeDef& type, bool isArray) {
                    if (isArray)
                        return;

                    auto c = get_category(type);
                    if (c == category::class_type)
                    {
                        writer.write("{ nullptr }");
                    }
                },
                [&](auto&&...) {},
            });
    }

    static void write_cpp_namespace(writer& writer, std::string_view ns)
    {
        while (true)
        {
            auto pos = ns.find_first_of(".");
            if (pos == std::string_view::npos)
            {
                break;
            }

            writer.write(ns.substr(0, pos));
            writer.write("::"sv);
            ns = ns.substr(pos + 1);
        }

        writer.write(ns);
    }

    void cpp_namespace::operator()(writer& writer) const
    {
        if (!data->base_namespace.empty())
        {
            write_cpp_namespace(writer, data->base_namespace);
            writer.write("::"sv);
        }

        writer.write(data->name);
    }

    void cpp_typename::operator()(writer& writer) const
    {
        if (type_namespace == foundation_namespace)
        {
            if (type_name == "EventRegistrationToken"sv)
            {
                return writer.write("event_token"sv);
            }
            else if (type_name == "HResult"sv)
            {
                return writer.write("hresult"sv);
            }
        }
        else if (type_namespace == "Windows.Foundation.Numerics"sv)
        {
            auto itr = std::find_if(std::begin(numerics_mappings), std::end(numerics_mappings),
                [&](auto& pair) { return pair.first == type_name; });
            if (itr != std::end(numerics_mappings))
            {
                return writer.write_fmt("Windows::Foundation::Numerics::%", itr->second);
            }
        }

        write_cpp_namespace(writer, type_namespace);
        writer.write("::"sv);
        writer.write(type_name);
    }
}
