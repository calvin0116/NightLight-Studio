#pragma once
#include "LevelEditor.h"

class InspectorWindow : public LE_WinBase_Derived<InspectorWindow>
{
private:
	bool hasInit;

	int selected_index;
public:
	InspectorWindow()
		:hasInit{ false }
		,selected_index{-1}
	{};
	~InspectorWindow() {};

	//void Init() override;
	void Run() override;
	///void Exit() override;

	//void InitBeforeRun();
};
