#pragma once
#include "TextWriter.h"
#include "MetadataHelpers.h"

class TypescriptWriter
{

private:
    TextWriter textWriter;

public:

    void Write(const Namespace& node)
    {
        textWriter.Write("declare namespace % {\n % \n}"sv, node.FullName());
        //textWriter.FlushToFile(settings.OutputFolder / (node.FullName() + ".d.ts"));
        // std::cout << tsWriter.Buffer() << std::endl;
        for (auto&& [name, type] : node.Members().types)
        {
            switch (get_category(type))
            {
            case winmd::reader::category::interface_type:

                continue;
            case winmd::reader::category::class_type:

                continue;
            case winmd::reader::category::enum_type:
                if (node.FullName() == "Windows.ApplicationModel.Activation")
                {
                    std::cout << type.TypeName() << std::endl;
                    for (auto field : type.FieldList())
                    {
                        std::cout << "-" << field.Name() << std::endl;
                    }
                    // auto x = type.get_enum_definition().m_typedef.FieldList();
                }
                continue;
            case winmd::reader::category::struct_type:

                continue;
            case winmd::reader::category::delegate_type:

                continue;
            }
        }
    }

};
