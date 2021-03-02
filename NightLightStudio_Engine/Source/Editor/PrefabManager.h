#pragma once
#include <string>
#include "../../framework.h"
#include "../Component/ComponentManager.h"


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

	class PrefabManager : public Singleton<PrefabManager>
	{
		std::map<std::string, PrefabInstances> PrefabList;

	public:
		NS_COMPONENT::ComponentManager::ComponentSetManager::EntityHandle::EntityComponentContainer GetPrefabComp(std::string prefab_name)
		{
			Entity prefab_ent = G_ECMANAGER_PREFABS->getEntity(PrefabList[prefab_name].prefab_id);
			if (prefab_ent.getId() == -1)
				return NS_COMPONENT::ComponentManager::ComponentSetManager::EntityHandle::EntityComponentContainer(nullptr,-1);
			return prefab_ent.getEntityComponentContainer();
		}
	};
}

namespace PFunc = Prefab_Function;