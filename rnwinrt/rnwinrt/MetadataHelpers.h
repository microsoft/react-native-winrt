#pragma once
#include "Settings.h"

struct static_projection_data : public std::enable_shared_from_this<static_projection_data>
{
    static std::vector<std::shared_ptr<static_projection_data>> ParseMetaData(const Settings& settings);

    static void SortGlobalLists();

    static_projection_data(std::string_view name, std::string_view fullName) : m_name(name), m_fullName(fullName)
    {
    }

    const std::string& Name() const noexcept;
    const std::string& FullName(bool useCppDelim = false) const;

    // virtual jsi::Value create(jsi::Runtime& runtime) const override;
    virtual std::vector<std::shared_ptr<static_projection_data>>* Children()
    {
        return {};
    }

private:
    const std::string m_name;
    const std::string m_fullName;

    mutable std::string m_fullNameCpp;
};

struct static_class_data final : static_projection_data
{
    static_class_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_enum_data final : static_projection_data
{
    static_enum_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_struct_data final : static_projection_data
{
    static_struct_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_delegate_data final : static_projection_data
{
    static_delegate_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};

struct static_interface_data final : static_projection_data
{
    static_interface_data(std::string_view name, std::string_view fullName,
        const winmd::reader::TypeDef typeDef) :
        static_projection_data(name, fullName),
        m_typeDef(typeDef)
    {
    }

private:
    winmd::reader::TypeDef m_typeDef;
};
/*
struct static_generic_interface_data final : static_projection_data
{
    static_interface_data(std::string_view name, std::string_view fullName) :
        static_projection_data(name, fullName)
    {
    }

private:

};
*/
struct static_namespace_data final : static_projection_data
{
    static_namespace_data(std::string_view name, std::string_view fullName,
        const winmd::reader::cache::namespace_members* members, const Settings& settings) :
        static_projection_data(name, fullName)
    {
        ParseChildren(members, settings);
    }

    virtual std::vector<std::shared_ptr<static_projection_data>>* Children() override
    {
        return &m_children;
    }

    std::vector<std::shared_ptr<static_projection_data>> m_children;
    std::vector<std::shared_ptr<static_interface_data>> m_interfaces;
    std::vector<std::shared_ptr<static_struct_data>> m_structs;
    std::vector<std::shared_ptr<static_delegate_data>> m_delegates;

private:
    void ParseChildren(const winmd::reader::cache::namespace_members* members, const Settings& settings);

    template <typename T, typename U>
    void ParseTypeDefs(std::vector<std::shared_ptr<T>>& items, const std::vector<winmd::reader::TypeDef>& typeDefs,
        const Settings& settings, std::vector<std::shared_ptr<static_projection_data>>* globalList);
};

extern std::vector<std::shared_ptr<static_projection_data>> _interfaces;
extern std::vector<std::shared_ptr<static_projection_data>> _structs;
extern std::vector<std::shared_ptr<static_projection_data>> _delegates;










struct Namespace : public std::enable_shared_from_this<Namespace>
{
    static std::map<std::string_view, std::shared_ptr<Namespace>> GetRoots(const Settings& settings);

    Namespace(const std::shared_ptr<Namespace>& parent, std::string&& name,
        const winmd::reader::cache::namespace_members* members = nullptr);

    const std::string& Name() const noexcept;
    const std::string& FullName(bool useCppDelim = false) const;
    const winmd::reader::cache::namespace_members& Members() const noexcept;

    bool HasProjectedTypes() const noexcept;

    std::shared_ptr<Namespace> Parent() const;
    const std::map<std::string_view, std::shared_ptr<Namespace>>& Children() const noexcept;
    uint32_t Depth() const;

    bool Enumerate(const std::function<bool(const Namespace&)>& fn, bool postorder = false) const;
    static bool EnumerateAll(const std::map<std::string_view, std::shared_ptr<Namespace>>& namespaces,
        const std::function<bool(const Namespace&)>& fn, bool postorder = false);

private:
    const std::optional<std::weak_ptr<Namespace>> m_parent;
    const std::string m_name;
    const winmd::reader::cache::namespace_members* m_members;

    std::map<std::string_view, std::shared_ptr<Namespace>> m_children;
    mutable std::optional<uint32_t> m_depth;
    mutable std::string m_fullName;
    mutable std::string m_fullNameCpp;
};

bool IsNamespaceAllowed(const Settings& settings, const std::string_view& namespaceFullName,
    const winmd::reader::cache::namespace_members& members);
bool IsMethodAllowed(const Settings& settings, const winmd::reader::MethodDef& methodDef);
bool IsTypeAllowed(const Settings& settings, const winmd::reader::TypeDef& typeDef, const bool isClass = false);

template <typename T>
bool HasAttribute(T const& row, const std::string_view& attributeNamespace, const std::string_view& attributeName)
{
    return static_cast<bool>(winmd::reader::get_attribute(row, attributeNamespace, attributeName));
}

struct ActivationFactoryInterfaceInfo
{
    winmd::reader::TypeDef TypeDef;
    bool IsActivatable{};
    bool IsStatics{};
    bool IsComposable{};
    bool IsVisible{};
};
std::vector<ActivationFactoryInterfaceInfo> GetActivationFactoryInterfaces(const winmd::reader::TypeDef& classDef);

struct PropertyInfo
{
    winmd::reader::MethodDef GetterDef;
    winmd::reader::MethodDef SetterDef;
};

struct EventInfo
{
    winmd::reader::MethodDef AddDef;
    winmd::reader::MethodDef RemoveDef;
};

struct MethodOverloadInfo
{
    MethodOverloadInfo(winmd::reader::MethodDef def);

    winmd::reader::MethodDef Def;
    uint32_t InParamCount{};
    uint32_t OutParamCount{};
    bool IsDefaultOverload{};
};

struct InterfaceInfo
{
    std::map<std::string_view, PropertyInfo> Properties;
    std::map<std::string_view, EventInfo> Events;
    std::map<std::string_view, std::map<uint32_t, std::vector<MethodOverloadInfo>>> Methods;
};

InterfaceInfo GetInterfaceInfo(const winmd::reader::TypeDef& interfaceDef);

bool IsSpecialOmittedStruct(const winmd::reader::TypeDef& type);
bool UseLowerCamelCaseCodeGenForStruct(const winmd::reader::TypeDef& type);

// TODO: Replace with C++20's built-in method.
inline bool StartsWith(const std::string_view& lhs, const std::string_view& rhs)
{
    return !lhs.compare(0, rhs.length(), rhs);
};