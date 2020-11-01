#pragma once
#include "LevelEditor.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Components.h"

class InspectorWindow : public LE_WinBase_Derived<InspectorWindow>
{
private:
	bool hasInit;

	int selected_index;

	void ComponentLayout(Entity& ent);
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
