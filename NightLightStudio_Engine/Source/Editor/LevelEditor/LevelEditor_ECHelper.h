#pragma once
#include "../../../framework.h"
#include "../../Core/Singleton.h"
#include "../PrefabManager.h"

class LE_ECHelper: public Singleton<LE_ECHelper>
{
protected:
	friend Singleton<LE_ECHelper>;

	LE_ECHelper()
		:selected_ent_amt{0}
		, selected_ent_id{-1}
		, setFocus{false}
		, prefabMode{false}
	{};
	~LE_ECHelper() {};

	//Single select
	int selected_ent_id;

	//Multi select
	int selected_ent_amt;


	//First = entity id, second = selected bool
	std::map<int, bool> selected_ents;
	//First = level editor id, second = entity id
	std::map<int, int> le_id_to_object_id;

	Prefab_Function::PrefabInstances prefab_inst;  //For prefab
	bool prefabMode;
public:


	//Window focus for inspecter
	bool setFocus;

	//==========Single select==============//
	void SelectEntity(int id)
	{
		//Deselect Previous entity
		if(selected_ent_id!= -1)
			DeSelectEntity(selected_ent_id);

		if (prefab_inst.prefab_id != -1)
		{
			prefab_inst.SavePrefab();
			prefab_inst.prefab_id = -1;		//Deselect prefab
			prefab_inst.isActive = false;
		}
		//Select current entity
		selected_ents[id] = true;
		++selected_ent_amt;
		selected_ent_id = id;

		setFocus = false;
	}
	
	void DeSelectEntity(int id)
	{
		selected_ents[id] = false;
		--selected_ent_amt;
	}

	void SelectPrefab(std::string file)
	{
		//Deselect Previous entity
		if (selected_ent_id != -1) {
			DeSelectEntity(selected_ent_id);
			selected_ent_id = -1;
		}
		prefab_inst.CreatePrefabInstance(file);
		prefab_inst.isActive = true;
	}

	//=====================================//

	//==========Multiple Select============//
	//For future work
	//====================================//

	//	Helper
	void InsertEntityForSelection(int id, bool)
	{
		selected;
		selected_ents[id] = false;
	}

	int GetSelectedEntityID()
	{
		return selected_ent_id;
	}


	
	std::map<int, bool>& SelectedEntities()
	{
		return selected_ents;
	}

	//Getter for debug purpose
	std::map<int, int>& LevelEditorId_EntityIdMap()
	{
		return le_id_to_object_id;
	}

	Prefab_Function::PrefabInstances GetPrefabInst()
	{
		return prefab_inst;
	}
};

static LE_ECHelper* LE_ECHELPER = LE_ECHelper::GetInstance();

