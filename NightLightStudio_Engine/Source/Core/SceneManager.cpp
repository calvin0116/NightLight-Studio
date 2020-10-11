#include "SceneManager.h"

#include <filesystem>

namespace fs = std::filesystem;

SceneManager::~SceneManager()
{
	for (const auto& pars : scene_list)
	{
		delete pars.second;
	}
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
		pars.second->Load();
		pars.second->PrintDataList();
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


void SceneManager::Free()
{
	DestroyInstance();
}


void SceneManager::LoadScene()
{
    Parser* scene = scene_list[current_scene];
    scene->Load();
	to_change_scene = SC_NOCHANGE;
	//~~!Create object using data
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
    else if(next_scene == current_scene)
        to_change_scene = SC_RESTART;
    else
        to_change_scene = SC_CHANGE;
}