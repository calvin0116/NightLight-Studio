#pragma once
#include "LevelEditor.h"

enum HIER_STATE
{
	HS_NORMAL = 0,
	HS_SORTBYTAG,
	HS_SORTBYALPHA
};

class HierarchyInspector : public LE_WinBase_Derived<HierarchyInspector>
{
private:
	std::string _search;
	int selected_index;
	bool _allowCopy;
	bool _scrollBottom;

	HIER_STATE _hieState;
	int _sortType;
	const float _zoomDist = 100.0f;

	//Used for displaying list in alphabetical order
	std::vector<Entity> ent_list_to_display;

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

public :
	HierarchyInspector()
		: selected_index{}, _allowCopy{ true }, _scrollBottom{ false }, _hieState{ HS_NORMAL }
	{};
	~HierarchyInspector() {};

	void Start() override;
	void Init() override;
	void Run() override;
	void GameExit() override;
	void Exit() override;

	void InitBeforeRun();

	void NormalHie();
	void SortByTag();

	void EntityFunction(Entity& ent, int& index, int tag_of_ent = -1);

	void CopyObjects();
	void DeleteObjects();
};

