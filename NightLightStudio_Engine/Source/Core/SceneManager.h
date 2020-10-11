#pragma once
#include "../../framework.h"

#include "MySystem.h"
#include "Singleton.h"

#include "../IO/Json/Parser.h"

using FILE_NAME = std::string;
const std::string EXIT_SCENCE = "Exit_Scene";

enum SCENE_CHANGE
{
    SC_NOCHANGE = 0,
    SC_CHANGE,
    SC_RESTART,
    SC_EXIT
};

class SceneManager : public MySystem, public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;
	//Parser for Scene manager
	Parser scene_parser;
	
	//Scene manipulation
	std::string current_scene;
	std::string next_scene;
    SCENE_CHANGE to_change_scene;
	//bool to_exit;

	//Scene List
	std::map<FILE_NAME, Parser*> scene_list;

	//Path for scenes
	std::string scenes_path;

protected:
	SceneManager()
		:scene_parser{ "SceneConfig" , json_path },
        current_scene{},
        next_scene{},
        to_change_scene{SC_NOCHANGE},
        scene_list{},
        scenes_path{}
	{}

	~SceneManager();

	//Scene base naming
	void LoadScene();
	void InitScene();
	void ExitScene();
public:
	//==================System function =================//
	//Load resources and scenes config
	void Load() override;

	void GameLoad();
	void GameInit();		//Init current scene
	
	void Free();		//Destroy current system

	//================Scene Function====================//
    SCENE_CHANGE CheckChangeScene();	//For checking if there is a need for change of scene
	void LoadScene(std::string scene_name);
	//Save current scene
	void SaveScene();

	//============== Helper function =====================//
	bool CheckIfSceneExist(std::string& scene_name);


	//============= Getting / Setter ====================//
	ENGINE_API void SetNextScene(std::string scene_name);
	//bool GetToExit();
    //SCENE_CHANGE GetToChangeScene();
};


static SceneManager* SYS_SCENE_MANAGER = SceneManager::GetInstance();