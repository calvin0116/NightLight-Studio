#pragma once
#include "LevelEditor.h"


class HeirarchyInspector : public LE_WinBase_Derived<HeirarchyInspector>
{
public :
	HeirarchyInspector() {};
	~HeirarchyInspector() {};

	void Init() override;
	void Run() override;
	void Exit() override;
};

