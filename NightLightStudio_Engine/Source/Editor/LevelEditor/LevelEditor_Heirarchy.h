#pragma once
#include "LevelEditor.h"


class HeirarchyInspector : public LE_WinBase_Derived<HeirarchyInspector>
{
private:
	bool hasInit;

	int selected_index;
public :
	HeirarchyInspector() 
		:hasInit{false}
	{};
	~HeirarchyInspector() {};

	void Init() override;
	void Run() override;
	void Exit() override;

	void InitBeforeRun();
};

