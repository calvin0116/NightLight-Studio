#pragma once
#include "../../../framework.h"
#include "../../Core/Singleton.h"

class LE_ECHelper: public Singleton<LE_ECHelper>
{
protected:
	friend Singleton<LE_ECHelper>;

	LE_ECHelper()
		:selected_ent_amt{0}
		, selected_ent_id{-1}
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

public:
	//==========Single select==============//
	void SelectEntity(int id)
	{
		//Deselect Previous entity
		if(selected_ent_id!= -1)
			DeSelectEntity(selected_ent_id);

		//Select current entity
		selected_ents[id] = true;
		++selected_ent_amt;
		selected_ent_id = id;
	}
	
	void DeSelectEntity(int id)
	{
		selected_ents[id] = false;
		--selected_ent_amt;
	}

	//=====================================//

	//==========Multiple Select============//
	//For future work
	//====================================//

	//	Helper
	void InsertEntityForSelection(int id, bool selected = false)
	{
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

};
#ifdef _EDITOR

static LE_ECHelper* LE_ECHELPER = LE_ECHelper::GetInstance();

#endif //  
