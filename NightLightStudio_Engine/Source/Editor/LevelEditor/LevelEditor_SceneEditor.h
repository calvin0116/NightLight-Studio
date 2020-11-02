#ifndef LEVELEDITOR_SCENEEDITOR
#define LEVELEDITOR_SCENEEDITOR
#pragma once

#include "LevelEditor.h"
#include "../../Component/Components.h"

class SceneEditor : public LE_WinBase_Derived<SceneEditor>
{
public:
	struct ENTITY_LAST_POS
	{
		TransformComponent* _transComp;
		glm::mat4 _newPos;
	};
private:
	bool _lastPos_Start;
	bool _lastEnter;
	ENTITY_LAST_POS _lastPos_ELP;
public:

	SceneEditor();
	~SceneEditor();

	void Start() override;

	void Init() override;

	void Run() override;

	void Exit() override;

	bool EditTransform(const float* cameraView, float* cameraProjection, float* mat, bool editTransformDecomposition);
};



#endif
