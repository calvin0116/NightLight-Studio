#pragma once
#include "../../../framework.h"
#include "../../Core/Singleton.h"
#include "../PrefabManager.h"
#include "../../IO/Json/JsonHeader.h"

#include <string>

namespace LS
{
	class LE_ECHelper : public Singleton<LE_ECHelper>
	{
	protected:
		friend Singleton<LE_ECHelper>;

		LE_ECHelper()
			:selected_ent_amt{ 0 }
			, selected_ent_id{ -1 }
			, setFocus{ false }
			, prefabMode{ false }
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

		//==========Single/Multi select==============//
		void SelectEntity(int id, bool multi = false)
		{
			//Deselect Previous entity
			if (selected_ent_id != -1 && !multi)
			{
				for (std::map<int, bool>::iterator it = selected_ents.begin(); it != selected_ents.end(); ++it)
				{
					DeSelectEntity(it->first);
				}
			}

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
		void InsertEntityForSelection(int id, bool = false)
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

		Prefab_Function::PrefabInstances GetPrefabInst()
		{
			return prefab_inst;
		}

	};


	struct ENTITY_COMP_INFO
	{
		std::shared_ptr<Document> _rjDoc;
		ENTITY_COMP_INFO(Value&& val) : _rjDoc{ std::make_shared<Document>() }
		{
			Copy(std::move(val));
		}
		~ENTITY_COMP_INFO() {}

		void Copy(Value&& val)
		{
			if (_rjDoc->IsObject())
			{
				_rjDoc->RemoveAllMembers();
			}
			_rjDoc->CopyFrom(val, _rjDoc->GetAllocator(), true);
		}

		ENTITY_COMP_INFO(const ENTITY_COMP_INFO& ecd) : _rjDoc{ std::make_shared<Document>() }
		{
			if (_rjDoc->IsObject())
			{
				_rjDoc->RemoveAllMembers();
			}
			_rjDoc->CopyFrom(*ecd._rjDoc, _rjDoc->GetAllocator(), true);
		}

		std::string StringOut()
		{
			rapidjson::StringBuffer buffer;

			buffer.Clear();

			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			(*_rjDoc).Accept(writer);

			return std::string(buffer.GetString());
		}
	};
	typedef std::map <size_t, ENTITY_COMP_INFO> ENTITY_COMP_DATA;

	extern size_t findCaseInsensitive(std::string data, std::string toSearch, size_t pos = 0);
	extern void ReadIntoCompVec(Entity ent, ENTITY_COMP_DATA* data, bool leftTransOut = false);
}
static LS::LE_ECHelper* LE_ECHELPER = LS::LE_ECHelper::GetInstance();


