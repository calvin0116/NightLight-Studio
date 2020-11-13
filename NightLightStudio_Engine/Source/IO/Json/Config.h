/***************************************************************************
Brief Description :
		File purely to storee config data imported in from json files
		// As of now config file is at Resources/JsonFile/
		// Look at assert_path to known the updated path
		// Thing that are affected by Config file as of now
		//			- Width and Height of the Window
**************************************************************************/
#pragma once
#include "../../framework.h"
#include "../Core/Singleton.h"
#include "Parser.h"

//Namespace io....

struct ConfigData
{
	//Window width and height
	//If there is multiple window, this needs to be changed
	int width, height;

	//For master sound volume
	//Can be here or in Sound manager
	float volume;

	//FullScreen flag
	bool toFullScreen;

	//Selected Starting Screen (for ease of changing starting screen for debuggging purpose) 
	std::string startscene;

	//Camera Variables
	float _positionSensitivity;
	float _rotationSensitivity;
	float _zoomSensitivity;
	float _lastCamPosX;
	float _lastCamPosY;
	float _lastCamPosZ;
	float _lastCamPitch;
	float _lastCamYaw;

	//Grid Variables
	float _gridSize;
	float _cellSize;
	float _gridColourRed;
	float _gridColourGreen;
	float _gridColourBlue;
	float _gridColourAlpha;

	//============= Run time variable ============//
	bool engineRunning;
	bool sceneRunning;
	bool levelEditorMode;
	bool isPlaying;
};

class Config : public MySystem ,public Singleton<Config> {
private:
	ConfigData config_d;
	NS_SERIALISER::Parser parser;
public:
	ENGINE_API Config()
		:config_d{}
		, parser{ "Config", json_path }
	{
		config_d.engineRunning = true;
		config_d.sceneRunning = true;
	};

	ENGINE_API void Load()
	{
		std::cout << "Parser::StartUp" << std::endl;
		parser.Load();
		std::cout << "Parser::Loading of data" << std::endl;
		Value& wind_val = parser["window"];

		config_d.height = wind_val["height"].GetInt();
		config_d.width = wind_val["width"].GetInt();
		config_d.toFullScreen = wind_val["fullscreen"].GetBool();

		Value& config_val = parser["settings"];
		config_d.startscene = config_val["startscene"].GetString();

		config_d._positionSensitivity = config_val["positionSensitivity"].GetFloat();
		config_d._rotationSensitivity = config_val["rotationSensitivity"].GetFloat();
		config_d._zoomSensitivity = config_val["zoomSensitivity"].GetFloat();

		//Not Working properly
		//config_d._lastCamPosX = config_val["camPosX"].GetFloat();
		//config_d._lastCamPosY = config_val["camPosY"].GetFloat();
		//config_d._lastCamPosZ = config_val["camPosZ"].GetFloat();
		//config_d._lastCamPitch = config_val["camPitch"].GetFloat();
		//config_d._lastCamYaw = config_val["camYaw"].GetFloat();

		if(config_val.FindMember("gridSize") != config_val.MemberEnd())
			config_d._gridSize = config_val["gridSize"].GetFloat();

		if (config_val.FindMember("cellSize") != config_val.MemberEnd())
			config_d._cellSize = config_val["cellSize"].GetFloat();

		if (config_val.FindMember("gridColourRed") != config_val.MemberEnd())
			config_d._gridColourRed = config_val["gridColourRed"].GetFloat();

		if (config_val.FindMember("gridColourGreen") != config_val.MemberEnd())
			config_d._gridColourGreen = config_val["gridColourGreen"].GetFloat();

		if (config_val.FindMember("gridColourBlue") != config_val.MemberEnd())
			config_d._gridColourBlue = config_val["gridColourBlue"].GetFloat();

		if (config_val.FindMember("gridColourAlpha") != config_val.MemberEnd())
			config_d._gridColourAlpha = config_val["gridColourAlpha"].GetFloat();

		config_d.levelEditorMode = config_val["start_in_level_editor"].GetBool();
		if (config_d.levelEditorMode)
			config_d.isPlaying = true;
		else
			config_d.isPlaying = false;
	}


	ENGINE_API void Init()
	{

	};

	ENGINE_API void Free() 
	{
		Value& wind_val = parser["window"];

		wind_val["height"].SetInt(config_d.height);
		wind_val["width"].SetInt(config_d.width);
		wind_val["fullscreen"].SetBool(config_d.toFullScreen);

		Value& config_val = parser["settings"];
		config_val["startscene"].SetString(rapidjson::StringRef(config_d.startscene.c_str()));
		config_val["positionSensitivity"].SetFloat(config_d._positionSensitivity);
		config_val["rotationSensitivity"].SetFloat(config_d._rotationSensitivity);
		config_val["zoomSensitivity"].SetFloat(config_d._zoomSensitivity);

		//config_val["camPosX"].SetFloat(config_d._lastCamPosX);
		//config_val["camPosY"].SetFloat(config_d._lastCamPosY);
		//config_val["camPosZ"].SetFloat(config_d._lastCamPosZ);
		//config_val["camPitch"].SetFloat(config_d._lastCamPitch);
		//config_val["camYaw"].SetFloat(config_d._lastCamYaw);

		config_val["gridSize"].SetFloat(config_d._gridSize);
		config_val["cellSize"].SetFloat(config_d._cellSize);

		config_val["gridColourRed"].SetFloat(config_d._gridColourRed);
		config_val["gridColourGreen"].SetFloat(config_d._gridColourGreen);
		config_val["gridColourBlue"].SetFloat(config_d._gridColourBlue);
		config_val["gridColourAlpha"].SetFloat(config_d._gridColourAlpha);


		config_val["start_in_level_editor"].SetBool(config_d.levelEditorMode);

		parser.Save();
	};

	ENGINE_API void Exit()
	{
		DestroyInstance();
	}

	//Get config data to change
	ConfigData& GetConfigData() { return config_d; };
};

static Config* const CONFIG_DATA = Config::GetInstance();