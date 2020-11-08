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
		//std::cout << "===============================================" << std::endl;
		//std::cout << "Loading Scene: " << current_scene << std::endl;
		if (scene->CheckForMember("Objects"))
		{
			//std::cout << "Initialising Objects....." << std::endl;
			NS_SERIALISER::EntityListCreation((*scene)["Objects"]);
		}
		else
		{
			std::cout << "Failed to find object to initailise....." << std::endl;
		}
		//std::cout << "===============================================" << std::endl;
	}

	void SceneManager::InitScene()
	{
		NS_SERIALISER::Parser* scene = scene_list[current_scene];
		//~~!Insert data back to the objects
	}

	void SceneManager::ExitScene()
	{
		scene_list[current_scene]->CleanDoc();

		//Delete all entity
		for (Entity ent : G_ECMANAGER->getEntityContainer())
		{
			G_ECMANAGER->FreeEntity(ent.getId());
		}

		current_scene = next_scene;
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

	void SceneManager::SaveScene(std::string path)
	{
		std::cout << "Saving scene to:" << std::endl;
		//Save scene
		NS_SERIALISER::Parser* scene;
			
		if(path == "")
			scene = scene_list[current_scene];
		else
		{
			fs::path cur_path_name = path;

			scene = new NS_SERIALISER::Parser(cur_path_name.stem().string(), cur_path_name.parent_path().string());
		}
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
				Value comp_val = comp->Write();

				if(comp_val.IsObject())
					NS_SERIALISER::ChangeData(ent_val, comp->ser_name, comp_val.GetObject());
				else
				{
					const std::type_info& tinf = typeid(*comp);
					std::cout << "Wrong data given from component: " << tinf.name() << std::endl;
				}
			}

			NS_SERIALISER::ChangeData(obj_val, EntityName[ent.getId()], ent_val->GetObject());

			delete ent_val;
		}
		scene->AddData("Objects", obj_val);

		//delete obj_val;

		//scene.PrintDataList();
		scene->Save();

		std::cout << "Save scene success~!" << std::endl;
		if (path != "")
		{
			delete scene;
		}
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
		
		fs::path scene_path = scene_name;
		bool found = false;
		for (auto& scene_pair : scene_list)
		{
			if (scene_pair.first == scene_path.stem().string())
			{
				found = true;
				break;
			}
		}

		//Inside scene into list
		if(!found)
			scene_list[scene_path.stem().string()] = new NS_SERIALISER::Parser(scene_path.stem().string(), scene_path.parent_path().string());

		next_scene = scene_path.stem().string();
		std::cout << "Switching to: " << scene_name << "....." << std::endl;

		if (scene_name == EXIT_SCENCE)
			to_change_scene = SC_EXIT;
		else if (next_scene == current_scene)
			to_change_scene = SC_RESTART;
		else
			to_change_scene = SC_CHANGE;
	}
}