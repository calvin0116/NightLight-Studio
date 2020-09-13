#pragma once
#include "Header.h"
#include "Singleton.h"
#include "Parser.h"


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
};

class Config : public Singleton<Config> {
private:
	ConfigData config_d;
	Parser parser;
public:
	Config()
		:config_d{}
		, parser{ "config.json", assert_path }
	{
	};

	void LoadUp() 
	{
		parser.Load();

		Value& wind_val = parser["window"];

		config_d.height = wind_val["height"].GetInt();
		config_d.width = wind_val["width"].GetInt();
		config_d.toFullScreen = wind_val["fullscreen"].GetBool();

		Value& config_val = parser["config"];

		config_d.startscene = config_val["startscene"].GetString();
	};

	ConfigData& GetConfigData() { return config_d; };
};

static Config* CONFIG_DATA = Config::GetInstance();