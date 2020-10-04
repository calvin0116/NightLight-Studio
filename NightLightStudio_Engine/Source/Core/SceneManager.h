#pragma once
#include "../../framework.h"

#include "MySystem.h"
#include "Singleton.h"

#include "../IO/Json/Parser.h"



class SceneManager : public MySystem, public Singleton<SceneManager>
{
	friend Singleton<SceneManager>;
	Parser scene_parser;

	SceneManager()
		:scene_parser{ "SceneConfig" , json_path }
	{}

	void EarlyLoad()
	{
		scene_parser.Load();
	}

};


static SceneManager* SYS_SCENEMANAGER = SceneManager::GetInstance();