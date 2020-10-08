#include "SceneManager.h"

#include <filesystem>

namespace fs = std::filesystem;

inline void SceneManager::EarlyLoad()
{
	scene_parser.Load();
	scene_parser.PrintDataList();

	//Filepath that contains the scene
	std::cout << scene_parser.GetPath() << scene_parser["SceneFolder"].GetString() << std::endl;
	std::string path = scene_parser.GetPath() + scene_parser["SceneFolder"].GetString();

	//Get Scene Path
	for (const auto& entry : fs::directory_iterator(path))
		std::cout << entry.path() << std::endl;



}
