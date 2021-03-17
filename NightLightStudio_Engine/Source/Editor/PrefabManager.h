#pragma once
#include <string>
#include "../../framework.h"
#include "../Component/ComponentManager.h"
#include "../Core/AssetsMananger.h"

namespace Prefab_Function {

	struct PrefabInstances : public AssetInfo
	{
		bool isActive = false;
		int prefab_id = -1;
		std::string filepath;
	};

	class PrefabManager : public Singleton<PrefabManager>
	{
		std::map<std::string, PrefabInstances> PrefabList;

	public:
		void Load();

		//Get component list
		NS_COMPONENT::ComponentManager::ComponentSetManager::EntityHandle::EntityComponentContainer GetPrefabComp(std::string prefab_name)
		{
			Entity prefab_ent = G_ECMANAGER_PREFABS->getEntity(PrefabList[prefab_name].prefab_id);
			if (prefab_ent.getId() == -1)
				return NS_COMPONENT::ComponentManager::ComponentSetManager::EntityHandle::EntityComponentContainer(nullptr,-1);
			return prefab_ent.getEntityComponentContainer();
		}

		ENGINE_API void WritePrefab(std::string file, Entity& prefab_ent,
			NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER);

		ENGINE_API int PrefabReadAndCreate(std::string file, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER);

		PrefabInstances* CreatePrefabInstance(std::string file);
		void SavePrefab(PrefabInstances* pi);
	};
}

//namespace PFunc = Prefab_Function;
static Prefab_Function::PrefabManager* PFunc = Prefab_Function::PrefabManager::GetInstance();