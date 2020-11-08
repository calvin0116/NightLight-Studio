/***************************************************************************
Brief Description :
			Json header files and related defines
**************************************************************************/
#include "JsonFunction.h"
#include "../../Component/Components.h"

namespace NS_SERIALISER {
    template <typename T>
    inline void CreateAndWriteComp(Value& Comp_list, Entity& entity, std::string& component_name)
    {
        T comp;
        static_cast<ISerializable*>(&comp)->Read(Comp_list[component_name.c_str()]);
        G_ECMANAGER->AttachComponent<T>(entity, comp);
    }

    inline void ComponentsCreation(Value& Comp_list, Entity& entity)
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
            else if (component_name == "CScriptComponent")
            {
                CreateAndWriteComp<CScriptComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "ScriptComponent")
            {
                CreateAndWriteComp<ScriptComponent>(Comp_list, entity, component_name);
            }
        }
    }


    inline void EntityListCreation(Value& Ent_list)
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