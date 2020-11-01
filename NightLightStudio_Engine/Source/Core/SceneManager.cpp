#include "SceneManager.h"

#include "../Component/ComponentManager.h"
#include "../Component/Components.h"

#include "../Input/SystemInput.h"
#include <filesystem>
#include <typeinfo>

#undef GetObject

namespace fs = std::filesystem;

namespace NS_SCENE
{
	SceneManager::~SceneManager()
	{
		//Safety delete
		for (const auto& pars : scene_list)
		{
			delete pars.second;
		}
		scene_list.clear();
	}

	inline void SceneManager::Load()
	{
		//Load up scene manager's parser
		scene_parser.Load();
		scene_parser.PrintDataList();
		
		current_scene = scene_parser["StartUpScene"].GetString();

		//Filepath that contains the scene
		std::cout << scene_parser.GetPath() << scene_parser["SceneFolder"].GetString() << std::endl;
		//As of now is "../Resources/JsonFile/" + "Scene"
		scenes_path = scene_parser.GetPath() + scene_parser["SceneFolder"].GetString();

		int index = 0; 
		//Get Scene Path
		for (const auto& entry : fs::directory_iterator(scenes_path))
		{
			fs::path cur_path_name = entry.path();
			//Ways to access different info of the path
			//std::cout << cur_path_name << std::endl;
			//std::cout << cur_path_name.stem() << std::endl;
			//std::cout << cur_path_name.parent_path() << std::endl;

			//Insert Scene Name with its relative path
			scene_list[cur_path_name.stem().string()] = new NS_SERIALISER::Parser(cur_path_name.stem().string(), cur_path_name.parent_path().string());
			//Individual files in Scene folder
			std::cout << cur_path_name.stem().string() << std::endl;

			//Store index with string
			scene_indexes[index] = cur_path_name.stem().string();
			++index;
		}

		/*
		//Load up each scene
		for (const auto& pars : scene_list)
		{
			//pars.second->Load();
			//pars.second->PrintDataList();
			Parser& level_parse = *(pars.second);
			if (level_parse.CheckForMember("Objects"))
			{
				level_parse["Objects"]
			}

		}*/
	}

	void SceneManager::GameLoad()
	{
		LoadScene();
	}

	void SceneManager::GameInit()
	{
		InitScene();
	}

	void SceneManager::Update()
	{
		//Exit button that uses scene
		if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ESCAPE))
		{
			SetNextScene(EXIT_SCENCE);
		}
		
		//Set next scene according to index 
		//Ctrl + 1
		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_1)
			&& SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_CTRL))
		{
			
			//SetNextScene();
			if (scene_index < scene_list.size()-1)
				++scene_index;
			else
				scene_index = 0;

			std::cout << "Going next scene........: " << scene_list.size() << std::endl;

			SetNextScene(scene_indexes[scene_index]);
		}
		
		//Ctrl + S
		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_S)
			&& SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_CTRL))
		{
			SaveScene();
		}
	}

	SCENE_CHANGE SceneManager::CheckChangeScene()
	{
		return to_change_scene;
	}


	void SceneManager::GameExit()
	{
		ExitScene();
	}

	void SceneManager::Free()
	{
		for (const auto& pars : scene_list)
		{
			delete pars.second;
		}
		scene_list.clear();
	}

	void SceneManager::Exit()
	{
		DestroyInstance();
	}


	void SceneManager::LoadScene()
	{
		NS_SERIALISER::Parser* scene = scene_list[current_scene];
		scene->Load();
		to_change_scene = SC_NOCHANGE;
		
		//~~!Create object using data
		std::cout << "===============================================" << std::endl;
		std::cout << "Loading Scene: " << current_scene << std::endl;
		if (scene->CheckForMember("Objects"))
		{
			std::cout << "Initialising Objects....." << std::endl;
			EntityListCreation((*scene)["Objects"]);
		}
		else
		{
			std::cout << "Failed to find object to initailise....." << std::endl;
		}
		std::cout << "===============================================" << std::endl;
	}

	void SceneManager::InitScene()
	{
		NS_SERIALISER::Parser* scene = scene_list[current_scene];
		//~~!Insert data back to the objects
	}

	void SceneManager::ExitScene()
	{
		scene_list[current_scene]->CleanDoc();
		//G_ECMANAGER->
		current_scene = next_scene;
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
				CreateAndWriteComp<ColliderComponent>(Comp_list, entity, component_name);
			}
			else if (component_name == "RigidBody")
			{
				CreateAndWriteComp<RigidBody>(Comp_list, entity, component_name);
			}
			  else if (component_name == "AudioManager")
			  {
				CreateAndWriteComp<ComponentLoadAudio>(Comp_list, entity, component_name);
			  }
		}
	}

	void SceneManager::EntityListCreation(Value& Ent_list)
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



	void SceneManager::LoadScene(std::string scene_name)
	{
#ifdef _DEBUG 
		if (!CheckIfSceneExist(scene_name))
		{
			std::cout << "Scene does not exist" << std::endl;
			return;
		}
#endif

		NS_SERIALISER::Parser* scene = scene_list[scene_name];
		scene->Load();
	}

	void SceneManager::SaveScene()
	{
		std::cout << "Saving scene....." << std::endl;
		//Save scene
		NS_SERIALISER::Parser* scene = scene_list[current_scene];
		//std::string output_filename = "Scene/Output"; <- For testing
		//NS_SERIALISER::Parser scene = NS_SERIALISER::Parser(output_filename, scene_parser.GetPath() );
		
		struct stat buffer;
		if (stat(scene->GetFilePath().c_str(), &buffer) != 0)
		{
			std::cout << "file does not exist, creating file....." << std::endl;
			//Creates file
			std::ofstream MyFile(scene->GetFilePath().c_str());

			MyFile << "{\n}";
		}
		scene->Load();
		scene->CleanDoc();

		//Add Objects objects
		Value* obj_val = new Value;
		obj_val->SetObject();

		//Entity loop
		//auto itr = G_ECMANAGER->begin<TransformComponent>();

		//for (auto ent : EntityName)
		for(Entity ent : G_ECMANAGER->getEntityContainer())
		{
			Value* ent_val = new Value;
			ent_val->SetObject();
			//~~!! Need to know what component the Entity have and loop through them
			//Component Loop
			
			for(ISerializable* comp : ent.getEntityComponentContainer())
			{ 
				//const std::type_info& tinf = typeid(*comp);
				//std::cout << tinf.name() << std::endl;

				NS_SERIALISER::ChangeData(ent_val, comp->ser_name, comp->Write().GetObject());
			}

			NS_SERIALISER::ChangeData(obj_val, EntityName[ent.getId()], ent_val->GetObject());

			delete ent_val;
		}
		scene->AddData("Objects", obj_val);

		//delete obj_val;

		//scene.PrintDataList();
		scene->Save();
	}

	bool SceneManager::CheckIfSceneExist(std::string& scene_name)
	{
		bool exist = false;
		for (const auto& entry : fs::directory_iterator(scenes_path))
		{
			fs::path cur_path_name = entry.path();

			if (cur_path_name.stem().string() == scene_name)
			{
				//Json file exist
				exist = true;
				//Check for existing scene
				bool found = false;
				for (const auto& pars : scene_list)
				{
					if (pars.first == scene_name)
					{
						found = true;
						break;
					}
				}
				//Create Scene parser and scene if not found
				if (!found)
				{
					scene_list[cur_path_name.stem().string()] = new NS_SERIALISER::Parser(cur_path_name.stem().string(), cur_path_name.parent_path().string());
				}
				break;
			}
		}
		return exist;
	}


	void SceneManager::SetNextScene(std::string scene_name)
	{
		next_scene = scene_name;
		std::cout << "Switching to: " << scene_name << "....." << std::endl;

		if (scene_name == EXIT_SCENCE)
			to_change_scene = SC_EXIT;
		else if (next_scene == current_scene)
			to_change_scene = SC_RESTART;
		else
			to_change_scene = SC_CHANGE;
	}
}