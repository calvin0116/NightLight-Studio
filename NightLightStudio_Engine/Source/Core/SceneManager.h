#pragma once
#include "../../framework.h"

#include "MySystem.h"
#include "Singleton.h"

#include "../IO/Json/Parser.h"



class SceneManager : public MySystem, public Singleton<SceneManager>
{
	friend Singleton<SceneManager>;
	Parser scene_parser;
	
	std::string current_scene;
	std::map<int, Parser*> scene_list;

	SceneManager()
		:scene_parser{ "SceneConfig" , json_path }
	{}

	void EarlyLoad();

};


static SceneManager* SYS_SCENEMANAGER = SceneManager::GetInstance();