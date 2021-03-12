/***************************************************************************
Brief Description :
			Json header files and related defines
**************************************************************************/
#include "JsonFunction.h"
#include "../../Component/Components.h"

namespace NS_SERIALISER {
    template <typename T>
    inline void CreateAndWriteComp(Value& Comp_list, Entity& entity, std::string& component_name, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
    {
        //T comp;
        //static_cast<ISerializable*>(&comp)->Read(Comp_list[component_name.c_str()]);
        //G_ECMANAGER->AttachComponent<T>(entity, comp);

        T* comp = g_ecman->AddComponent<T>(entity);
        static_cast<ISerializable*>(comp)->Read(Comp_list[component_name.c_str()]);
        
    }

    template <typename T>
    inline void ReadComp(Value& Comp_list, Entity& entity, std::string& component_name)
    {
        //T comp;
        //static_cast<ISerializable*>(&comp)->Read(Comp_list[component_name.c_str()]);
        //G_ECMANAGER->AttachComponent<T>(entity, comp);

        T* comp = entity.getComponent<T>();
        static_cast<ISerializable*>(comp)->Read(Comp_list[component_name.c_str()]);

    }

    inline void ComponentsCreation(Value& Comp_list, Entity& entity, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
    {
        for (auto itr = Comp_list.MemberBegin(); itr != Comp_list.MemberEnd(); ++itr)
        {

            std::string component_name = itr->name.GetString();
            //std::cout << "~~ Component: " << component_name << std::endl;
            if (component_name == "TransformComponent")
            {
                CreateAndWriteComp<TransformComponent>(Comp_list, entity, component_name, g_ecman);
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
                g_ecman->AttachComponent<ColliderComponent>(entity, comp);
            }
            else if (component_name == "RigidBody")
            {
                CreateAndWriteComp<RigidBody>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "AudioManager")
            {
                CreateAndWriteComp<ComponentLoadAudio>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "GraphicsComponent")
            {
                CreateAndWriteComp<GraphicsComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "CScriptComponent")
            {
                CreateAndWriteComp<CScriptComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "ScriptComponent")
            {
                CreateAndWriteComp<ScriptComponent>(Comp_list, entity, component_name, g_ecman);
            }
            /*else if (component_name == "ScriptComponent")
            {
                CreateAndWriteComp<ScriptComponent>(Comp_list, entity, component_name, g_ecman);
            }*/
            else if (component_name == "LightComponent")
            {
                CreateAndWriteComp<ComponentLight>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "PlayerStatsComponent")
            {
                CreateAndWriteComp<PlayerStatsComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "CauldronStatsComponent")
            {
                CreateAndWriteComp<CauldronStatsComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "CanvasComponent")
            {
                CreateAndWriteComp<ComponentCanvas>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "AnimationComponent")
            {
                CreateAndWriteComp<AnimationComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "ComponentVariables")
            {
                CreateAndWriteComp<VariablesComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "NavigatorComponent")
            {
                CreateAndWriteComp<NavigatorComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "WayPointMapComponent")
            {
                CreateAndWriteComp<WayPointMapComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "WayPointComponent")
            {
                CreateAndWriteComp<WayPointComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "EmitterComponent")
            {
                CreateAndWriteComp<EmitterComponent>(Comp_list, entity, component_name, g_ecman);
            }
            else if (component_name == "CameraComponent")
            {
              CreateAndWriteComp<CameraComponent>(Comp_list, entity, component_name, g_ecman);
            }
        }
    }

    inline void EntityListCreation(Value& Ent_list, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
    {
        for (auto itr = Ent_list.MemberBegin(); itr != Ent_list.MemberEnd(); ++itr)
        {
            std::string outStr = "NS_SERIALISER::EntityListCreation: Entity Name: "; outStr.append(itr->name.GetString());
            TracyMessage(outStr.c_str(), outStr.size());
            SPEEDLOG(outStr);
            //std::cout << "Entity Name: " << itr->name.GetString() << std::endl;
            Entity ent_handle = g_ecman->BuildEntity(itr->name.GetString());		//Build entity

            Value& Component_list = Ent_list[itr->name.GetString()];					//Get component list

            //std::cout << ent_handle.getId() << std::endl;
            //EntityName[ent_handle.getId()] = itr->name.GetString();
            //EntityName.emplace(std::make_pair(ent_handle.getId(), itr->name.GetString()));

            ComponentsCreation(Component_list, ent_handle);
        }
    }

    inline void EntityListInit(Value& Ent_list)
    {
        for (auto itr = Ent_list.MemberBegin(); itr != Ent_list.MemberEnd(); ++itr)
        {
            std::string outStr = "NS_SERIALISER::EntityListInit: Entity Name: "; outStr.append(itr->name.GetString());
            TracyMessage(outStr.c_str(), outStr.size());
            SPEEDLOG(outStr);
            //std::cout << "Entity Name: " << itr->name.GetString() << std::endl;
            Entity ent_handle = G_ECMANAGER->getEntityUsingEntName(itr->name.GetString());		//Build entity


            outStr = "NS_SERIALISER::EntityListInit: Entity ID: "; outStr.append(std::to_string(ent_handle.getId()));
            TracyMessage(outStr.c_str(), outStr.size());
            SPEEDLOG(outStr);
            //std::cout << "Entity ID: " << ent_handle.getId() << std::endl;

            Value& Component_list = Ent_list[itr->name.GetString()];					//Get component list

            ComponentsInit(Component_list, ent_handle);
        }
    }

    inline void ComponentsInit(Value& Comp_list, Entity& entity)
    {
        for (auto itr = Comp_list.MemberBegin(); itr != Comp_list.MemberEnd(); ++itr)
        {

            std::string component_name = itr->name.GetString();
            if (component_name == "TransformComponent")
            {
                ReadComp<TransformComponent>(Comp_list, entity, component_name);

            }
            //~~! Add your own component creation here ~~!//
            else if (component_name == "ColliderComponent")
            {
                //CreateAndWriteComp<ColliderComponent>(Comp_list, entity, component_name);
                ReadComp<ColliderComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "RigidBody")
            {
                ReadComp<RigidBody>(Comp_list, entity, component_name);
            }
            else if (component_name == "AudioManager")
            {
                ReadComp<ComponentLoadAudio>(Comp_list, entity, component_name);
            }
            else if (component_name == "GraphicsComponent")
            {
                //ReadComp<GraphicsComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "CScriptComponent")
            {
                ReadComp<CScriptComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "ScriptComponent")
            {
                ReadComp<ScriptComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "ScriptComponent")
            {
                ReadComp<ScriptComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "LightComponent")
            {
                ReadComp<ComponentLight>(Comp_list, entity, component_name);
            }
            else if (component_name == "PlayerStatsComponent")
            {
                ReadComp<PlayerStatsComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "CauldronStatsComponent")
            {
                ReadComp<CauldronStatsComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "CanvasComponent")
            {
                ReadComp<ComponentCanvas>(Comp_list, entity, component_name);
            }
            else if (component_name == "AnimationComponent")
            {
                ReadComp<ComponentAnimation>(Comp_list, entity, component_name);
            }
            else if (component_name == "ComponentVariables")
            {
                ReadComp<VariablesComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "NavigatorComponent")
            {
                ReadComp<NavigatorComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "WayPointMapComponent")
            {
                ReadComp<WayPointMapComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "WayPointComponent")
            {
                ReadComp<WayPointComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "EmitterComponent")
            {
                ReadComp<EmitterComponent>(Comp_list, entity, component_name);
            }
            else if (component_name == "CameraComponent")
            {
              ReadComp<CameraComponent>(Comp_list, entity, component_name);
            }
        }
    }
}