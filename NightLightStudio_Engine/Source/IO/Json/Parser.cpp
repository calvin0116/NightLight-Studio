//#include "..\..\..\framework.h"
#include "Parser.h"
//#include "rapidjson/rapidjson.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Components.h"

namespace NS_SERIALISER
{
    Parser::Parser(std::string name_, std::string path_, std::string type_)
    {
        name = name_;
        folderpath = path_ + "/";

        alloc = &doc.GetAllocator();

        fullfilepath = folderpath + name + type_;
    }

    Parser::~Parser()
    {
        //doc.ckk
    }

    void Parser::Load()
    {
        //StringBuffer s;
        //../Resource/Json/Config
        //filepath = path + name + ".json";


        //Read in the data from the file
        std::ifstream in(fullfilepath, std::ios::binary);

        if (!in)
        {
            TracyMessageL(std::string("Parser::Load:: Error: Failed to open file in: " + fullfilepath).c_str());
            SPEEDLOG(std::string("Parser::Load:: Error: Failed to open file in: " + fullfilepath));
            //std::cout << "Error: Failed to open file in: " + filepath << std::endl;
        }
        // dont skip on whitespace
        std::noskipws(in);
        // Read in content
        std::istreambuf_iterator<char> head(in);
        std::istreambuf_iterator<char> tail;
        std::string data(head, tail);

        //std::cout << data << std::endl;
        //Interpret json formated string
        doc.Parse(data.c_str());
        //Loop through the objects
        //PrintDataList();

        in.close();
    }

    void Parser::PrintDataList()
    {
        TracyMessageL("Parser::PrintDataList: Printing through doc:");
        SPEEDLOG("Parser::PrintDataList: Printing through doc:");
        //std::cout << "Printing through doc:" << std::endl;
        for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
        {
            PrintData(itr, (Value&)doc);
        }
    }



    std::string Parser::GetName()
    {
        return name;
    }

    std::string Parser::GetPath()
    {
        return folderpath;
    }

    std::string Parser::GetFilePath()
    {
        return fullfilepath;
    }

    Document& Parser::GetDoc()
    {
        return doc;
    }

    bool Parser::CheckForMember(std::string mem_name)
    {
        //PrintDataList();
        //Check for first layer
        if (doc.HasMember(mem_name.c_str()))
            return true;

        for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
        {
            Value& val = doc[itr->name.GetString()];
            if (val.IsObject())
            {
                for (Value::ConstMemberIterator itr2 = val.MemberBegin(); itr2 != val.MemberEnd(); ++itr2)
                {
                    //See if variable name is the same
                    if (mem_name == itr2->name.GetString())
                    {
                        TracyMessageL("Parser::CheckForMember: Member found");
                        SPEEDLOG("Parser::CheckForMember: Member found");
                        //std::cout << "Member found" << std::endl;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    D_TYPE Parser::DetermineType(Value::ConstMemberIterator itr)
    {
        /*//Debug type
        static const char* kTypeNames[] =
        { "Null", "False", "True", "Object", "Array", "String", "Number" };
        std::cout << kTypeNames[itr->value.GetType()] << std::endl;
        */
        if (itr->value.IsBool())
        {
            return D_TYPE::D_BOOL;
        }
        else if (itr->value.IsInt())
        {
            return D_TYPE::D_INT;
        }
        else if (itr->value.IsFloat())
        {
            return D_TYPE::D_FLOAT;
        }
        else if (itr->value.IsString())
        {
            return D_TYPE::D_STRING;
        }
        else if (itr->value.IsObject())
        {
            return D_TYPE::D_ARRAY;
        }

        return D_TYPE::D_INVALID;
    }

    void Parser::PrintData(Value::ConstMemberIterator itr, Value& val)
    {
        D_TYPE data_type = DetermineType(itr);

        std::string outStr;

        if (data_type != D_TYPE::D_ARRAY)
        {
            outStr.append("    ");
            //std::cout << "    ";
        }

        outStr.append(itr->name.GetString()).append(" : ");
        //std::cout << itr->name.GetString() << " : ";

        switch (data_type)
        {
        case D_TYPE::D_BOOL:
            outStr.append(itr->value.GetBool()? "true" : "false") += '\n';
            //std::cout << itr->value.GetBool() << '\n';
            break;
        case D_TYPE::D_FLOAT:
            outStr.append(std::to_string(itr->value.GetFloat())) += '\n';
            //std::cout << itr->value.GetFloat() << '\n';
            break;
        case D_TYPE::D_INT:
            outStr.append(std::to_string(itr->value.GetFloat())) += '\n';
            //std::cout << itr->value.GetFloat() << '\n';
            break;
        case D_TYPE::D_STRING:
            outStr.append(itr->value.GetString()) += '\n';
            //std::cout << itr->value.GetString() << '\n';
            break;
        case D_TYPE::D_ARRAY:
        {
            outStr.append("{\n");
            //std::cout << "{\n";
            for (Value::ConstMemberIterator itr2 = val[itr->name.GetString()].MemberBegin(); itr2 != val[itr->name.GetString()].MemberEnd(); ++itr2)
            {
                PrintData(itr2, val[itr->name.GetString()]);
            }
            outStr.append("}");
            //std::cout << "}" << std::endl;
            break;
        }
        }

        TracyMessageL(std::string("Parser::PrintData: " + outStr).c_str());
        SPEEDLOG(std::string("Parser::PrintData: " + outStr).c_str());
    }



    void Parser::Save()
    {
        std::ofstream ofs(fullfilepath);
        OStreamWrapper osw(ofs);

        //StringBuffer buffer(doc);
        PrettyWriter<OStreamWrapper> writer(osw);
        //Writer<OStreamWrapper> writer(osw);
        doc.Accept(writer);
    }
    void Parser::LoadData(std::string path)
    {
        fullfilepath = path;
        Load();
    }
}




