#pragma once
#include "pch.h"

#include "Writer.h"

constexpr size_t c_reservedSize = 16 * 1024;

Writer::Writer()
{
    m_first.reserve(c_reservedSize);
}

void Writer::Swap()
{
    if (m_second.empty())
    {
        m_second.reserve(c_reservedSize);
    }
    std::swap(m_first, m_second);
}

void Writer::FlushToFile(const std::string_view& fileName)
{
    if (!IsFileEqual(fileName))
    {
        std::ofstream file{ fileName, std::ios::out | std::ios::binary };
        file.write(m_first.data(), m_first.size());
        file.write(m_second.data(), m_second.size());
    }
    m_first.clear();
    m_second.clear();
}

void Writer::FlushToFile(const std::filesystem::path& fileName)
{
    FlushToFile(static_cast<std::string_view>(fileName.string()));
}

namespace
{
    std::string FileToString(const std::string_view& fileName)
    {
        std::ifstream file(fileName, std::ios::binary);
        return static_cast<std::stringstream const&>(std::stringstream() << file.rdbuf()).str();
    }
}

bool Writer::IsFileEqual(const std::string_view& fileName) const
{
    if (!std::filesystem::exists(fileName))
    {
        return false;
    }

    const auto file = FileToString(fileName);
    if (file.size() != m_first.size() + m_second.size())
    {
        return false;
    }

    if (!std::equal(m_first.begin(), m_first.end(), file.begin(), file.begin() + m_first.size()))
    {
        return false;
    }

    return std::equal(m_second.begin(), m_second.end(), file.begin() + m_first.size(), file.end());
}

bool Writer::IsFileEqual(const std::filesystem::path& fileName) const
{
    return IsFileEqual(static_cast<std::string_view>(fileName.string()));
}

void Writer::Write(const winmd::reader::Constant& value)
{
    switch (value.Type())
    {
    case winmd::reader::ConstantType::Int32:
        WritePrintF("%d", value.ValueInt32());
        break;

    case winmd::reader::ConstantType::UInt32:
        WritePrintF("%#0x", value.ValueUInt32());
        break;

    default:
        ThrowInvalidArg("Unexpected constant type: ", std::to_string(static_cast<uint32_t>(value.Type())));
    }
}

void Writer::Write(const winmd::reader::GenericParam& param)
{
    Write(param.Name());
}

void Writer::Write(const winmd::reader::TypeDef& type)
{
    const auto namespaceFullName = type.TypeNamespace();
    const auto typeName = type.TypeName();
    const auto generics = type.GenericParam();

    if (!winmd::reader::empty(generics))
    {
        Write("winrt::@::%<%>", namespaceFullName, RemoveTick(typeName), Writer::BindList(", "sv, generics));
    }
    else if (typeName == "EventRegistrationToken" && namespaceFullName == "Windows.Foundation"sv)
    {
        Write("winrt::event_token");
    }
    else if (typeName == "HResult" && namespaceFullName == "Windows.Foundation"sv)
    {
        Write("winrt::hresult");
    }
    else if (namespaceFullName == "Windows.Foundation.Numerics"sv)
    {
        // While the ABI types are also translated from the projected types, C++/WinRT replaces those with its own
        // types.
        // clang-format off
        static const std::map<std::string_view, std::string_view> s_numericsMap = {
            { "Matrix3x2", "float3x2"sv },
            { "Matrix4x4", "float4x4"sv },
            { "Plane", "plane"sv },
            { "Quaternion", "quaternion"sv },
            { "Vector2", "float2"sv },
            { "Vector3", "float3"sv },
            { "Vector4", "float4"sv }
        };
        // clang-format on

        const auto it = s_numericsMap.find(typeName);
        Write("winrt::@::%", namespaceFullName, it == s_numericsMap.end() ? typeName : it->second);
    }
    else
    {
        Write("winrt::@::%", namespaceFullName, typeName);
    }
}

void Writer::Write(const winmd::reader::TypeRef& type)
{
    if ((type.TypeNamespace() == "System"sv) && (type.TypeName() == "Guid"sv))
    {
        Write("winrt::guid");
    }
    else
    {
        Write(winmd::reader::find_required(type));
    }
}

void Writer::Write(const winmd::reader::coded_index<winmd::reader::TypeDefOrRef>& type)
{
    switch (type.type())
    {
    case winmd::reader::TypeDefOrRef::TypeDef:
        Write(type.TypeDef());
        break;
    case winmd::reader::TypeDefOrRef::TypeRef:
        Write(type.TypeRef());
        break;
    case winmd::reader::TypeDefOrRef::TypeSpec:
        Write(type.TypeSpec().Signature().GenericTypeInst());
        break;
    }
}

void Writer::Write(const winmd::reader::GenericTypeInstSig& type)
{
    const auto [namespaceFullName, typeName] = winmd::reader::get_type_namespace_and_name(type.GenericType());
    Write("winrt::@::%<%>", namespaceFullName, RemoveTick(typeName), BindList(", "sv, type.GenericArgs()));
}

void Writer::Write(const winmd::reader::TypeSig& signature)
{
    if (signature.is_szarray())
    {
        // The caller should handle this generally. Depending on the scenario, we may want winrt::array_view<const T>
        // for in-params, winrt::com_array<T> for by-ref out-params, or winrt:::array_view<> for by value out-params.
        Write("winrt::com_array<%>"sv, signature.Type());
    }
    else
    {
        Write(signature.Type());
    }
}

void Writer::Write(const winmd::reader::TypeSig::value_type& type)
{
    std::visit(
        [&](auto&& type) {
            using T = std::decay_t<decltype(type)>;
            if constexpr (std::is_same_v<T, winmd::reader::ElementType>)
            {
                switch (type)
                {
                case winmd::reader::ElementType::Boolean:
                    Write("bool"sv);
                    break;
                case winmd::reader::ElementType::Char:
                    Write("char16_t"sv);
                    break;
                case winmd::reader::ElementType::I1:
                    Write("int8_t"sv);
                    break;
                case winmd::reader::ElementType::U1:
                    Write("uint8_t"sv);
                    break;
                case winmd::reader::ElementType::I2:
                    Write("int16_t"sv);
                    break;
                case winmd::reader::ElementType::U2:
                    Write("uint16_t"sv);
                    break;
                case winmd::reader::ElementType::I4:
                    Write("int32_t"sv);
                    break;
                case winmd::reader::ElementType::U4:
                    Write("uint32_t"sv);
                    break;
                case winmd::reader::ElementType::I8:
                    Write("int64_t"sv);
                    break;
                case winmd::reader::ElementType::U8:
                    Write("uint64_t"sv);
                    break;
                case winmd::reader::ElementType::R4:
                    Write("float"sv);
                    break;
                case winmd::reader::ElementType::R8:
                    Write("double"sv);
                    break;
                case winmd::reader::ElementType::String:
                    Write("winrt::hstring"sv);
                    break;
                case winmd::reader::ElementType::Object:
                    Write("winrt::Windows::Foundation::IInspectable"sv);
                    break;
                default:
                    FAIL_FAST();
                }
            }
            else if constexpr (std::is_same_v<T, winmd::reader::GenericTypeIndex>)
            {
                // Nothing
            }
            else
            {
                Write(type);
            }
        },
        type);
}

void Writer::WriteCode(const std::string_view& value)
{
    for (auto&& c : value)
    {
        if (c == '.')
        {
            Write("::");
        }
        else if (c == '`')
        {
            return;
        }
        else
        {
            Write(c);
        }
    }
}

void Writer::WriteInclude(const std::string_view& fileSpec)
{
    Write("#include \"%\"\n", fileSpec);
}

void Writer::WriteNewLine()
{
    Write('\n');
}

std::string_view Writer::RemoveTick(const std::string_view& name)
{
    return name.substr(0, name.rfind('`'));
}

std::string Writer::ToLowerCamelCase(const std::string_view& value)
{
    std::string converted(value);
    if (!converted.empty())
    {
        converted[0] = static_cast<char>(tolower(converted[0]));
    }
    return converted;
}

std::string Writer::ToLowerCase(const std::string_view& value)
{
    std::string converted(value);
#pragma warning(push)
#pragma warning(disable : 4244) // conversion from 'int' to 'char' for tolower
    std::transform(converted.begin(), converted.end(), converted.begin(), ::tolower);
#pragma warning(pop)
    return converted;
}

void Writer::WriteSegment(const std::string_view& value)
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

uint32_t Writer::CountSegmentPlaceholders(const std::string_view& format) noexcept
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