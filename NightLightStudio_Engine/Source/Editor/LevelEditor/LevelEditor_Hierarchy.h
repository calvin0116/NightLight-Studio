#pragma once
#include "LevelEditor.h"


class HierarchyInspector : public LE_WinBase_Derived<HierarchyInspector>
{
private:
	bool hasInit;

	int selected_index;
public :
	HierarchyInspector()
		:hasInit{false}
	{};
	~HierarchyInspector() {};

	void Init() override;
	void Run() override;
	void Exit() override;

	void InitBeforeRun();
};

