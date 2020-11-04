//#include "..\..\..\framework.h"
#include "Parser.h"
//#include "rapidjson/rapidjson.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Components.h"

namespace NS_SERIALISER
{
    Parser::Parser(std::string name_, std::string path_)
    {
        name = name_;
        path = path_ + "/";

        alloc = &doc.GetAllocator();

        filepath = path + name + ".json";
    }

    Parser::~Parser()
    {
        //doc.ckk
    }

    void Parser::Load()
    {
        //StringBuffer s;
        //../Resource/Json/Config
        filepath = path + name + ".json";


        //Read in the data from the file
        std::ifstream in(filepath, std::ios::binary);

        if (!in)
        {
            cout << "Error: Failed to open file in: " + filepath << endl;
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
        cout << "Printing through doc:" << std::endl;
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
        return path;
    }

    std::string Parser::GetFilePath()
    {
        return filepath;
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
                        std::cout << "Member found" << std::endl;
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

        if (data_type != D_TYPE::D_ARRAY)
            std::cout << "    ";

        std::cout << itr->name.GetString() << " : ";

        switch (data_type)
        {
        case D_TYPE::D_BOOL:
            std::cout << itr->value.GetBool() << '\n';
            break;
        case D_TYPE::D_FLOAT:
            std::cout << itr->value.GetFloat() << '\n';
            break;
        case D_TYPE::D_INT:
            std::cout << itr->value.GetFloat() << '\n';
            break;
        case D_TYPE::D_STRING:
            std::cout << itr->value.GetString() << '\n';
            break;
        case D_TYPE::D_ARRAY:
        {
            std::cout << "{\n";
            for (Value::ConstMemberIterator itr2 = val[itr->name.GetString()].MemberBegin(); itr2 != val[itr->name.GetString()].MemberEnd(); ++itr2)
            {
                PrintData(itr2, val[itr->name.GetString()]);
            }
            cout << "}" << endl;
            break;
        }
        }
    }



    void Parser::Save()
    {
        std::ofstream ofs(filepath);
        OStreamWrapper osw(ofs);

        //StringBuffer buffer(doc);
        PrettyWriter<OStreamWrapper> writer(osw);
        //Writer<OStreamWrapper> writer(osw);
        doc.Accept(writer);
    }


    //!! Type T Must have ISerialisable and a Component 

    template <typename T>
    void CreateAndWriteComp(Value& Comp_list, Entity& entity, std::string& component_name)
    {
        T comp;
        static_cast<ISerializable*>(&comp)->Read(Comp_list[component_name.c_str()]);
        G_ECMANAGER->AttachComponent<T>(entity, comp);
    }


    //~~~! Helper function that is not declared in class due to Entity not declared in .h
    void ComponentsCreation(Value& Comp_list, Entity& entity)
    {
        for (auto itr = Comp_list.MemberBegin(); itr != Comp_list.MemberEnd(); ++itr)
        {

            std::string component_name = itr->name.GetString();
            std::cout << "~~ Component: " << component_name << std::endl;
            if (component_name == "TransformComponent")
            {
                CreateAndWriteComp<TransformComponent>(Comp_list, entity, component_name);
                /*
                std::cout << "~~~~ Transform: " << std::endl << trans_com;
                std::cout << "~~~~~~~~~~~~~~ " << std::endl;*/

            }
            //~~! Add your own component creation here ~~!//
            else if (component_name == "ColliderComponent")
            {
                //CreateAndWriteComp<ColliderComponent>(Comp_list, entity, component_name);
                ColliderComponent comp(itr->value["colliderType"].GetString());
                comp.Read(Comp_list[component_name.c_str()]);
                G_ECMANAGER->AttachComponent<ColliderComponent>(entity, comp);
            }
            else if (component_name == "RigidBody")
            {
                CreateAndWriteComp<RigidBody>(Comp_list, entity, component_name);
            }
            else if (component_name == "AudioManager")
            {
                CreateAndWriteComp<ComponentLoadAudio>(Comp_list, entity, component_name);
            }
            else if (component_name == "GraphicsComponent")
            {
                CreateAndWriteComp<GraphicsComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "ScriptComponent")
            {
                CreateAndWriteComp<ScriptComponent>(Comp_list, entity, component_name);
            }
        }
    }

    void EntityListCreation(Value& Ent_list)
    {
        for (auto itr = Ent_list.MemberBegin(); itr != Ent_list.MemberEnd(); ++itr)
        {
            std::cout << "Entity Name: " << itr->name.GetString() << std::endl;
            Entity ent_handle = G_ECMANAGER->BuildEntity(itr->name.GetString());		//Build entity

            Value& Component_list = Ent_list[itr->name.GetString()];					//Get component list

            //std::cout << ent_handle.getId() << std::endl;
            //EntityName[ent_handle.getId()] = itr->name.GetString();
            //EntityName.emplace(std::make_pair(ent_handle.getId(), itr->name.GetString()));

            ComponentsCreation(Component_list, ent_handle);
        }
    }


}




