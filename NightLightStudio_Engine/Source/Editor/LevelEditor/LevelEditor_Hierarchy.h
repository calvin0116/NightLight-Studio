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
};

