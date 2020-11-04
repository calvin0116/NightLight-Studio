#pragma once
#include "../../framework.h"

#include "MySystem.h"
#include "Singleton.h"

#include "../IO/Json/Parser.h"

namespace NS_SCENE
{
	using FILE_NAME = std::string;

	const std::string EXIT_SCENCE = "Exit_Scene";	//Change to this scene to exit through scene manaager

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
		NS_SERIALISER::Parser scene_parser;

		//Scene manipulation
		FILE_NAME current_scene;
		FILE_NAME next_scene;
		SCENE_CHANGE to_change_scene;
		//bool to_exit;

		int scene_index;		//For change scene test / change scene by index 
		std::map<int, FILE_NAME> scene_indexes;
		//Scene List
		std::map<FILE_NAME, NS_SERIALISER::Parser*> scene_list;

		//Path for scenes
		std::string scenes_path;

	protected:
		SceneManager()
			:scene_parser{ "SceneConfig" , json_path },
			current_scene{},
			next_scene{},
			to_change_scene{ SC_NOCHANGE },
			scene_list{},
			scenes_path{},
			scene_index{0}
		{}

		~SceneManager();

		//Scene base naming
		void LoadScene();
		void InitScene();
		void ExitScene();


		//Component creation for entitiy using Json data
		//void ComponentsCreation(Value& Comp_list, Entity& entity);
	public:
		//Storing of entity name according to object id
		std::map<int, std::string> EntityName;

		//==================System function =================//
		void Load() override;	//Load resources and scenes config

		void GameLoad() override;
		void GameInit() override;		//Init current scene

		void Update() override;		//For testing without scipting

		void GameExit() override;
		void Free() override;	//Destroy current system
		void Exit() override;

		//================Scene Function====================//
		SCENE_CHANGE CheckChangeScene();	//For checking if there is a need for change of scene
		void LoadScene(std::string scene_name);
		//Save current scene
		void SaveScene(std::string path = "");

		//============== Helper function =====================//
		bool CheckIfSceneExist(std::string& scene_name);


		//============= Getting / Setter ====================//
		ENGINE_API void SetNextScene(std::string scene_name);
		//bool GetToExit();
		//SCENE_CHANGE GetToChangeScene();
		std::string GetCurrentScene() { return current_scene; };
	};


	static SceneManager* SYS_SCENE_MANAGER = SceneManager::GetInstance();
}