#pragma once
#include "LevelEditor.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Components.h"

#include "../imgui/imguizmo/ImGuizmo.h"

class InspectorWindow : public LE_WinBase_Derived<InspectorWindow>
{
public:
	struct ENTITY_LAST_POS
	{
		TransformComponent* _transComp;
		glm::mat4 _newPos;
	};
private:
	int selected_index;

	ImGuizmo::OPERATION _mCurrentGizmoOperation;
	ImGuizmo::MODE _mCurrentGizmoMode;
	bool _useSnap;
	float _snap[3];

	bool _lastPos_Start;
	bool _lastEnter;
	ENTITY_LAST_POS _lastPos_ELP;

	void ComponentLayout(Entity& ent);
	bool EditTransform(const float* cameraView, float* cameraProjection, float* matrix);
	void TransformGizmo(TransformComponent* comp);
public:
	InspectorWindow()
		: selected_index{ -1 }, _mCurrentGizmoOperation{ ImGuizmo::TRANSLATE }, _mCurrentGizmoMode{ ImGuizmo::WORLD },
		_useSnap{ false }, _snap{ 1.0f, 1.0f, 1.0f }, _lastEnter{ false }, _lastPos_Start{ false }, _lastPos_ELP{ false }
	{};
	~InspectorWindow() {};

	void Start() override;
	//void Init() override;
	void Run() override;
	///void Exit() override;

	//void InitBeforeRun();
};
