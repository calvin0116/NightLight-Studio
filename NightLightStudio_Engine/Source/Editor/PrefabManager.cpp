// SpeedLog
#include "../Log/SpeedLog.h"
#include "PrefabManager.h"

#include "../IO/Json/Parser.h"
//#include "../Component/ComponentManager.h"
#include "../../ISerializable.h"
#include "../Core/SceneManager.h"
#include <filesystem>
#include "../tracy-master/Tracy.hpp"


namespace fs = std::filesystem;

namespace Prefab_Function {
	int PrefabManager::PrefabReadAndCreate(std::string file, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
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
		if (prefab_val.HasMember("Name"))
		{
			Entity prefab_ent = g_ecman->BuildEntity(prefab_val["Name"].GetString());

			NS_SERIALISER::ComponentsCreation(prefab_val, prefab_ent, g_ecman);
			return prefab_ent.getId();
		}
		else
		{
			TracyMessageL("Prefab_Function::PrefabReadAndCreate: Prefab has no name, cannot load");
			SPEEDLOG("Prefab_Function::PrefabReadAndCreate: Prefab has no name, cannot load");
			//std::cout << "Prefab has no name, cannot load" << std::endl;
			return -1;
		}
	
	}



	inline void PrefabManager::Load()
	{
		//Add PrefabInstance to Load Assert queue
		SYS_ASSETSMANAGER->AddType<PrefabInstances>();
	}

	void PrefabManager::WritePrefab(std::string file, Entity& prefab_ent,
		NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
	{
		std::string ent_name = g_ecman->EntityName[prefab_ent.getId()];
		NS_SERIALISER::Parser prefab_parser(ent_name,file, ".prefab" );

		struct stat buffer;
		if (stat(prefab_parser.GetFilePath().c_str(), &buffer) != 0)
		{
			TracyMessageL("Prefab_Function::WritePrefab: file does not exist, creating file.....");
			SPEEDLOG("Prefab_Function::WritePrefab: file does not exist, creating file.....");
			//std::cout << "file does not exist, creating file....." << std::endl;
			//Creates file
			std::ofstream MyFile(prefab_parser.GetFilePath().c_str());

			MyFile << "{\n}";
		}

		prefab_parser.Load();
		prefab_parser.CleanDoc();

		prefab_parser.AddData("Name", rapidjson::StringRef(ent_name.c_str()));
		for (ISerializable* comp : prefab_ent.getEntityComponentContainer())
		{
			Value* comp_val = new Value;
			*comp_val = comp->Write();
			//prefab_parser.
			if (comp_val->IsObject())
			{
				prefab_parser.AddData(comp->ser_name, comp_val);
			}
			else
			{
				const std::type_info& tinf = typeid(*comp);
				TracyMessageL(std::string("Prefab_Function::WritePrefab: Wrong data given from component: ").append(tinf.name()).c_str());
				SPEEDLOG(std::string("Prefab_Function::WritePrefab: Wrong data given from component: ").append(tinf.name()));
				//std::cout << "Wrong data given from component: " << tinf.name() << std::endl;
			}

		}

		prefab_parser.Save();

	}
	
	PrefabInstances* PrefabManager::CreatePrefabInstance(std::string file)
	{
		auto pi_asset = SYS_ASSETSMANAGER->CreateAsset<PrefabInstances>(file);

		pi_asset.second.filepath = file;
		fs::path cur_path_name = file;

		std::string file_without_ext = cur_path_name.parent_path().string() +"/"+ cur_path_name.stem().string();
		TracyMessageL(std::string("PrefabInstances::CreatePrefabInstance: " + file_without_ext).c_str());
		SPEEDLOG(std::string("PrefabInstances::CreatePrefabInstance: " + file_without_ext));
		//std::cout << file_without_ext << std::endl;
		//Save and delete temp prefab
		if (pi_asset.second.isActive)
		{
			SavePrefab(&pi_asset.second);
			//Entity ent = G_ECMANAGER_PREFABS->getEntity(prefab_id);
			//WritePrefab(cur_path_name.parent_path().string(), ent, G_ECMANAGER_PREFABS);
			//save
			//G_ECMANAGER_PREFABS->FreeEntity(ent.getId());
		}

		pi_asset.second.prefab_id = PrefabReadAndCreate(file_without_ext, G_ECMANAGER_PREFABS);
	}

	void PrefabManager::SavePrefab(PrefabInstances* pi)
	{
		fs::path cur_path_name = pi->filepath;
		Entity ent = G_ECMANAGER_PREFABS->getEntity(pi->prefab_id);
		WritePrefab(cur_path_name.parent_path().string(), ent, G_ECMANAGER_PREFABS);
		//save
		G_ECMANAGER_PREFABS->FreeEntity(ent.getId());

	}
}