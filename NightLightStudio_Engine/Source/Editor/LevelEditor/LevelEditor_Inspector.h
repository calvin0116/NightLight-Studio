#pragma once
#include "LevelEditor.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Components.h"

#include "../imgui/imguizmo/ImGuizmo.h"

#include <memory>
#include <any>


struct ENTITY_COMP_READ
{
	int _entID;
	std::shared_ptr<Document> _rjDoc;

	ENTITY_COMP_READ() : _entID{ -1 }, _rjDoc{ nullptr } {}
	ENTITY_COMP_READ(Entity& ent, Value&& val) : _entID{ ent.getId() }, _rjDoc{ std::make_shared<Document>() }
	{
		Copy(std::move(val));
	}
	~ENTITY_COMP_READ() {}

	void Copy(Value&& val)
	{
		if (_rjDoc->IsObject())
		{
			_rjDoc->RemoveAllMembers();
		}
		_rjDoc->CopyFrom(val, _rjDoc->GetAllocator(), true);
	}

	ENTITY_COMP_READ(const ENTITY_COMP_READ& ecd) : _entID{ ecd._entID }, _rjDoc{ std::make_shared<Document>() }
	{
		if (_rjDoc->IsObject())
		{
			_rjDoc->RemoveAllMembers();
		}
		if (ecd._entID != -1)
		{
			_rjDoc->CopyFrom(*ecd._rjDoc, _rjDoc->GetAllocator(), true);
		}
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

	bool _isSnapX;
	bool _isSnapY;
	bool _isSnapYD;
	bool _isSnapZ;
	float _snapDist;
	float _snapGap;
	int _snapLod;

	bool _lastPos_Start;
	bool _lastEnter;
	ENTITY_LAST_POS _lastPos_ELP;
	std::vector<ENTITY_LAST_POS> _allOtherLastPos_ELP;

	glm::mat4 _transformItemActive;

	int _itemType;

	bool _notRemove;

	ENTITY_COMP_READ _origComp;

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
	void ListenerComp(Entity& ent);
  void CScriptComp(Entity& ent);
  void PlayerStatsComp(Entity& ent);
  void CauldronStatsComp(Entity& ent);
	void VariableComp(Entity& ent);
	void NavComp(Entity& ent);
	void WayPointPathComp(Entity& ent);
	void WayPointComp(Entity& ent);

	void AddSelectedComps(Entity& ent);

	void ComponentLayout(Entity& ent);
	bool EditTransform(const float* cameraView, float* cameraProjection, glm::mat4& matrix);
	void TransformGizmo(TransformComponent* comp);
	void TransformSnap(Entity& ent);
public:
	InspectorWindow()
		: selected_index{ -1 }, _mCurrentGizmoOperation{ ImGuizmo::TRANSLATE }, _mCurrentGizmoMode{ ImGuizmo::WORLD },
		_useSnap{ false }, _snapTrans{ 1.0f, 1.0f, 1.0f }, _lastEnter{ false }, _lastPos_Start{ false }, _lastPos_ELP{ false },
		_itemType{ 0 }, _notRemove{ true }, _snapRotate{ 1.0f }, _snapScale{ 1.0f }, _isSnapX{ false },  _isSnapY{ false },
		_isSnapZ{ false }, _isSnapYD{ true }, _snapDist{ 1000.0f }, _snapGap{ 1.00f }, _snapLod{ 3 }, _origComp{ }
	{};
	~InspectorWindow() {};

	void Start() override;
	//void Init() override;
	void Run() override;
	///void Exit() override;

	//void InitBeforeRun();

	bool GetIfGizmoManipulate();
};


template<class ComponentType>
inline std::any Paste_Component(std::any comp)
{
	ENTITY_COMP_READ entComp = std::any_cast<ENTITY_COMP_READ>(comp);
	Entity ent = G_ECMANAGER->getEntity(entComp._entID);
	ENTITY_COMP_READ origComp = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentType>()->Write() };
	ent.getComponent<ComponentType>()->Read(*entComp._rjDoc);

	return std::any(origComp);
}
