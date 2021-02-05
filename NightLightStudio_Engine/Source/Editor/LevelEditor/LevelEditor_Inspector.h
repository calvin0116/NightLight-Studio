#pragma once
#include "LevelEditor.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Components.h"

#include "../imgui/imguizmo/ImGuizmo.h"

#include <memory>

class InspectorWindow : public LE_WinBase_Derived<InspectorWindow>
{
public:
	struct ENTITY_LAST_POS
	{
		TransformComponent* _transComp;
		glm::mat4 _newPos;
	};
	struct ENTITY_COMP_DOC
	{
		Entity _ent;
		std::shared_ptr<Document> _rjDoc;
		size_t _compType;
		ENTITY_COMP_DOC(Entity& ent, Value&& val, size_t cType) : _ent{ ent }, _rjDoc{ std::make_shared<Document>() }, _compType{ cType }
		{
			Copy(std::move(val));
		}
		~ENTITY_COMP_DOC() {}

		void Copy(Value&& val)
		{
			if (_rjDoc->IsObject())
			{
				_rjDoc->RemoveAllMembers();
			}
			_rjDoc->CopyFrom(val, _rjDoc->GetAllocator(), true);
		}

		ENTITY_COMP_DOC(const ENTITY_COMP_DOC& ecd) : _ent{ ecd._ent }, _rjDoc{ std::make_shared<Document>() }, _compType{ ecd._compType }
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

	void SetFOV(const float& FOV);

private:
	int selected_index;

	ImGuizmo::OPERATION _mCurrentGizmoOperation;
	ImGuizmo::MODE _mCurrentGizmoMode;
	bool _useSnap;
	float _snapTrans[3];
	float _snapRotate;
	float _snapScale;

	bool _lastPos_Start;
	bool _lastEnter;
	ENTITY_LAST_POS _lastPos_ELP;

	int _itemType;

	bool _notRemove;

	// Added by YY to match current camera's fov
	float _fov = 44.5f;

	void TransformComp(Entity& ent);
	void ColliderComp(Entity& ent);
	void AudioComp(Entity& ent);
	void GraphicsComp(Entity& ent);
	void RigidBodyComp(Entity& ent);
	void LightComp(Entity& ent);
	void ScriptComp(Entity& ent);
	void CanvasComp(Entity& ent);
	void AnimationComp(Entity& ent);
	void EmitterComp(Entity& ent);
	void CameraComp(Entity& ent);
  void CScriptComp(Entity& ent);
  void PlayerStatsComp(Entity& ent);
  void CauldronStatsComp(Entity& ent);
	  void VariableComp(Entity& ent);
	  void NavComp(Entity& ent);
	  void WayPointPathComp(Entity& ent);
	  void WayPointComp(Entity& ent);


	void AddSelectedComps(Entity& ent);

	void ComponentLayout(Entity& ent);
	bool EditTransform(const float* cameraView, float* cameraProjection, float* matrix);
	void TransformGizmo(TransformComponent* comp);
public:
	InspectorWindow()
		: selected_index{ -1 }, _mCurrentGizmoOperation{ ImGuizmo::TRANSLATE }, _mCurrentGizmoMode{ ImGuizmo::WORLD },
		_useSnap{ false }, _snapTrans{ 1.0f, 1.0f, 1.0f }, _lastEnter{ false }, _lastPos_Start{ false }, _lastPos_ELP{ false },
		_itemType{ 0 }, _notRemove{ true }, _snapRotate{ 1.0f }, _snapScale{ 1.0f }
	{};
	~InspectorWindow() {};

	void Start() override;
	//void Init() override;
	void Run() override;
	///void Exit() override;

	//void InitBeforeRun();

	bool GetIfGizmoManipulate();
};
