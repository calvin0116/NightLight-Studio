#pragma once
#include "LevelEditor.h"


class HierarchyInspector : public LE_WinBase_Derived<HierarchyInspector>
{
private:
	std::string _search;
	int selected_index;
public :
	HierarchyInspector()
		: selected_index{}
	{};
	~HierarchyInspector() {};

	void Start() override;
	void Init() override;
	void Run() override;
	void Exit() override;

	void InitBeforeRun();
};

