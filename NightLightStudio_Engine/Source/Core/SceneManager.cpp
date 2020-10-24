#include "SceneManager.h"

#include "../Component/ComponentManager.h"
#include "../Component/Components.h"

#include <filesystem>

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

		//Get Scene Path
		for (const auto& entry : fs::directory_iterator(scenes_path))
		{
			fs::path cur_path_name = entry.path();

			//Ways to access different info of the path
			//std::cout << cur_path_name << std::endl;
			//std::cout << cur_path_name.stem() << std::endl;
			std::cout << cur_path_name.parent_path() << std::endl;

			//Insert Scene Name with its relative path
			scene_list[cur_path_name.stem().string()] = new Parser(cur_path_name.stem().string(), cur_path_name.parent_path().string());
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

	SCENE_CHANGE SceneManager::CheckChangeScene()
	{
		return to_change_scene;
	}


	void SceneManager::GameExit()
	{
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
		Parser* scene = scene_list[current_scene];
		scene->Load();
		to_change_scene = SC_NOCHANGE;
		//~~!Create object using data
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
	}

	void SceneManager::InitScene()
	{
		Parser* scene = scene_list[current_scene];
		//~~!Insert data back to the objects
	}

	void SceneManager::ExitScene()
	{
		scene_list[current_scene]->CleanDoc();
		current_scene = next_scene;
	}

	// Helper function that is not declared in class due to Entity not declared in .h
	void ComponentsCreation(Value& Comp_list, Entity& entity)
	{
		for (auto itr = Comp_list.MemberBegin(); itr != Comp_list.MemberEnd(); ++itr)
		{
		
			std::string component_name = itr->name.GetString();
			std::cout << "~~ Component: " << component_name << std::endl;
			if (component_name == "TransformComponent")
			{
				
				TransformComponent trans_com;
				trans_com.Read(Comp_list["TransformComponent"]);

				G_ECMANAGER->AttachComponent<TransformComponent>(entity, trans_com);
				std::cout << "~~~~ Transform: " << trans_com << std::endl;
			}

		}
	}

	void SceneManager::EntityListCreation(Value& Ent_list)
	{
		for (auto itr = Ent_list.MemberBegin(); itr != Ent_list.MemberEnd(); ++itr)
		{
			std::cout << "Entity Name: " << itr->name.GetString() << std::endl;
			Entity ent_handle = G_ECMANAGER->BuildEntity();
			Value& Component_list = Ent_list[itr->name.GetString()];

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

		Parser* scene = scene_list[scene_name];
		scene->Load();
	}

	void SceneManager::SaveScene()
	{
		//Save scene
		Parser* scene = scene_list[current_scene];
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
					scene_list[cur_path_name.stem().string()] = new Parser(cur_path_name.stem().string(), cur_path_name.parent_path().string());
				}
				break;
			}
		}
		return exist;
	}


	void SceneManager::SetNextScene(std::string scene_name)
	{
		next_scene = scene_name;

		if (scene_name == EXIT_SCENCE)
			to_change_scene = SC_EXIT;
		else if (next_scene == current_scene)
			to_change_scene = SC_RESTART;
		else
			to_change_scene = SC_CHANGE;
	}
}