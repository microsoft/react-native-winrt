#pragma once

#include "MetadataTypes.h"
#include "Settings.h"
#include "Writer.h"

void write_files(const Settings& settings, const projection_data& data);

struct cpp_namespace
{
    const namespace_projection_data* data;

    void operator()(jswinrt_writer& writer) const;
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

    void operator()(jswinrt_writer& writer) const;

    std::string_view type_namespace;
    std::string_view type_name;
};


#if 1
void WriteFiles(const Settings& settings, const std::map<std::string_view, std::shared_ptr<Namespace>>& roots);

void WriteFiles(const Settings& settings, const std::vector<std::shared_ptr<static_projection_data>>& roots);
#endif
