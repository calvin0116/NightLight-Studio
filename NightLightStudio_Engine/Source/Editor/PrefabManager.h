#pragma once
#include <string>
#include "../../framework.h"
#include "../Component/ComponentManager.h"

/*
class PrefabManager : public Singleton<PrefabManager>
{



};
*/
namespace Prefab_Function {
	ENGINE_API int PrefabReadAndCreate(std::string file, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER);

	ENGINE_API void WritePrefab(std::string file, Entity& prefab_ent,
		NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER);

	struct PrefabInstances
	{
		bool isActive = false;
		int prefab_id = -1;
		std::string filepath;

		void CreatePrefabInstance(std::string file);
		void SavePrefab();
	};
}

namespace PFunc = Prefab_Function;