#pragma once
#include "../../framework.h"

#include "MySystem.h"
#include "Singleton.h"

#include "../IO/Json/Parser.h"

using FILE_NAME = std::string;
const std::string EXIT_SCENCE = "Exit_Scene";

class SceneManager : public MySystem, public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;
	//Parser for Scene manager
	Parser scene_parser;
	
	//Scene manipulation
	std::string current_scene;
	std::string next_scene;
	bool to_change_scene;
	bool to_exit;

	//Scene List
	std::map<FILE_NAME, Parser*> scene_list;

	//Path for scenes
	std::string scenes_path;

protected:
	SceneManager()
		:scene_parser{ "SceneConfig" , json_path }
	{}

	~SceneManager();
public:
	///==================System function =================//
	//Load resources and scenes config
	void Load();
	//Save scenes config
	void Save();
	
	void Init();		//Init current scene
	bool LateUpdate();	//For checking if there is a need for change of scene
	void Exit();		//Clear current scene
	void Free();		//Destroy current system

	//Load specific scene
	void LoadScene(std::string scene_name);
	//Load is in the name but it should be done in Init
	void LoadScene();
	
	//Save current scene
	void SaveScene();


	//============== Helper function =====================//
	bool CheckIfSceneExist(std::string& scene_name);


	//============= Getting / Setter ====================//
	void SetNextScene(std::string scene_name);
	bool GetToExit();

};


static SceneManager* SYS_SCENEMANAGER = SceneManager::GetInstance();