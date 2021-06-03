#include "pch.h"

#include "MetadataHelpers.h"

using namespace std::literals;
using namespace winmd::reader;

coded_index<TypeDefOrRef> default_interface(const TypeDef& classType)
{
    assert(get_category(classType) == category::class_type);

    for (auto&& iface : classType.InterfaceImpl())
    {
        if (is_default_interface(iface))
        {
            return iface.Interface();
        }
    }

    return {};
}

TypeDef exclusiveto_class(TypeDef iface)
{
    auto attr = get_attribute(iface, metadata_namespace, "ExclusiveToAttribute"sv);
    if (!attr)
        return {};

    auto sig = attr.Value();
    assert(sig.FixedArgs().size() == 1);
    auto sysType = std::get<ElemSig::SystemType>(std::get<ElemSig>(sig.FixedArgs()[0].value).value);

    return iface.get_database().get_cache().find_required(sysType.name);
}

bool should_project_interface(const winmd::reader::TypeDef& iface)
{
    auto classDef = exclusiveto_class(iface);
    if (!classDef)
    {
        return true;
    }

    // Don't project static/factory interfaces
    for (auto&& attr : classDef.CustomAttribute())
    {
        auto [attrNs, attrName] = attr.TypeNamespaceAndName();
        if (attrNs != metadata_namespace)
            continue;

        if ((attrName != static_attribute) && (attrName != activatable_attribute))
            continue;

        // NOTE: Both static and activatable attribute have the type as the first parameter to the constructor. Also
        // note that types can be activatable w/ no factory interface (i.e. use IActivationFactory) and therefore the
        // first parameter may not be a type
        auto sig = attr.Value();
        auto& args = sig.FixedArgs();
        if (args.empty())
            continue;

        auto type = std::get_if<ElemSig::SystemType>(&std::get<ElemSig>(args[0].value).value);
        if (!type)
            continue;

        auto [typeNs, typeName] = separate_typename(type->name);
        if ((typeNs == iface.TypeNamespace()) && (typeName == iface.TypeName()))
        {
            return false;
        }
    }

    // Don't project protected/overridable interfaces since they appear as 'protected' on the class
    for (auto&& impl : classDef.InterfaceImpl())
    {
        auto implIface = impl.Interface();
        if ((implIface.type() != TypeDefOrRef::TypeDef) && (implIface.type() != TypeDefOrRef::TypeRef))
            continue;

        auto [implNs, implName] = get_type_namespace_and_name(implIface);
        if ((implNs != iface.TypeNamespace()) || (implName != iface.TypeName()))
            continue;

        for (auto&& attr : impl.CustomAttribute())
        {
            auto [attrNs, attrName] = attr.TypeNamespaceAndName();
            if (attrNs != metadata_namespace)
                continue;

            if ((attrName == protected_attribute) || (attrName == overridable_attribute))
                return false;
        }
    }

    return true;
}

TypeDef generic_type_def(const GenericTypeInstSig& sig)
{
    TypeDef result;
    resolve_type(sig.GenericType(), {},
        overloaded{
            [&](const TypeDef& typeDef) { result = typeDef; },
            [](auto&&...) { throw std::invalid_argument("Generic type must be a TypeDef"); },
        });
    return result;
}

MethodDef delegate_invoke_function(const TypeDef& typeDef)
{
    assert(get_category(typeDef) == category::delegate_type);
    for (auto&& method : typeDef.MethodList())
    {
        if (method.Name() == "Invoke"sv)
        {
            return method;
        }
    }

    throw std::runtime_error("Delegate without Invoke method");
}

GUID get_interface_guid(const TypeDef& typeDef)
{
    auto attr = get_attribute(typeDef, metadata_namespace, "GuidAttribute"sv);
    if (!attr)
    {
        throw std::runtime_error("Interface with no guid attribute");
    }

    auto value = attr.Value();
    auto& args = value.FixedArgs();
    return {
        std::get<uint32_t>(std::get<ElemSig>(args[0].value).value),
        std::get<uint16_t>(std::get<ElemSig>(args[1].value).value),
        std::get<uint16_t>(std::get<ElemSig>(args[2].value).value),
        {
            std::get<uint8_t>(std::get<ElemSig>(args[3].value).value),
            std::get<uint8_t>(std::get<ElemSig>(args[4].value).value),
            std::get<uint8_t>(std::get<ElemSig>(args[5].value).value),
            std::get<uint8_t>(std::get<ElemSig>(args[6].value).value),
            std::get<uint8_t>(std::get<ElemSig>(args[7].value).value),
            std::get<uint8_t>(std::get<ElemSig>(args[8].value).value),
            std::get<uint8_t>(std::get<ElemSig>(args[9].value).value),
            std::get<uint8_t>(std::get<ElemSig>(args[10].value).value),
        },
    };
}
