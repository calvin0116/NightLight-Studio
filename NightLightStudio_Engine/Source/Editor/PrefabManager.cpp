#include "PrefabManager.h"

#include "../IO/Json/Parser.h"
//#include "../Component/ComponentManager.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace Prefab_Function {
	Entity PrefabReadAndCreate(std::string file)
	{
		fs::path cur_path_name = file;
		NS_SERIALISER::Parser prefab_parser(
			cur_path_name.stem().string(),
			cur_path_name.parent_path().string(),
			".prefab"
		);
		
		prefab_parser.Load();
		
		Value prefab_val = prefab_parser.GetDoc().GetObject();
		//G_ECMANAGER_PREFABS->

		Entity prefab_ent = G_ECMANAGER->BuildEntity(prefab_val["Name"].GetString());

		NS_SERIALISER::ComponentsCreation(prefab_val, prefab_ent);
		return prefab_ent;
	}

	ENGINE_API void WritePrefab(std::string file, Entity& prefab_ent)
	{
		return ENGINE_API void();
	}
}