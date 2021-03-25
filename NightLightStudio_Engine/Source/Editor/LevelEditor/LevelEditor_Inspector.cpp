﻿#include "LevelEditor_Inspector.h"

#include "LevelEditor_ECHelper.h"
#include "../../Core/SceneManager.h"
#include "../../Graphics/GraphicsSystem.h"
#include "../../Graphics/SystemEmitter.h"

#include <set>
#include "LevelEditor_Console.h"
#include "../../Input/SystemInput.h"

#include "WindowsDialogBox.h"

// Construct script
#include "../../Logic/CScripts/AllScripts.h"
#include "../../Mono/MonoWrapper.h"

#include "../../Ai/AiManager.h"

#include "../../Graphics/CameraSystem.h"

// Matrix transform and quaternions for rotation
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

#include "../../Core/TagHandler.h"

// Snap
#include "../../Collision/SystemCollision.h"

void InspectorWindow::Start()
{
	// Set up Command to run to move objects
	COMMAND setPos =
		[](std::any pos)
	{
		ENTITY_LAST_POS obj = std::any_cast<ENTITY_LAST_POS>(pos);

		glm::mat4 newPos = obj._newPos;
		TransformComponent* trans_comp = obj._transComp;
		glm::mat4 lastPos = {};

		if (trans_comp != NULL)
		{
			lastPos = trans_comp->GetModelMatrix();

			float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(newPos), trans, rot, scale);

			trans_comp->_position = glm::make_vec3(trans);
			trans_comp->_rotation = glm::make_vec3(rot);
			trans_comp->_scale = glm::make_vec3(scale);
		}
		ENTITY_LAST_POS returnObj{ obj._transComp, lastPos };
		return returnObj;
	};

	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_SET_ENTITY_POSITION"),
		setPos,
		setPos);

	// Set up Command to run to move objects
	COMMAND setMultiplePos =
		[](std::any pos)
	{
		std::vector<ENTITY_LAST_POS> obj = std::any_cast<std::vector<ENTITY_LAST_POS>>(pos);

		std::vector<ENTITY_LAST_POS> returnObj;

		for (int i = 0; i < obj.size(); ++i)
		{
			glm::mat4 newPos = obj[i]._newPos;
			TransformComponent* trans_comp = obj[i]._transComp;
			glm::mat4 lastPos = {};

			if (trans_comp != NULL)
			{
				lastPos = trans_comp->GetModelMatrix();

				float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(newPos), trans, rot, scale);

				trans_comp->_position = glm::make_vec3(trans);
				trans_comp->_rotation = glm::make_vec3(rot);
				trans_comp->_scale = glm::make_vec3(scale);

				ENTITY_LAST_POS anotherObj{ obj[i]._transComp, lastPos };

				returnObj.push_back(anotherObj);
			}
		}

		return returnObj;
	};

	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_SET_MULTIPLE_ENTITY_POSITION"),
		setMultiplePos,
		setMultiplePos);

	COMMAND createComp =
		[](std::any comp)
	{
		ENTITY_COMP_DOC entComp = std::any_cast<ENTITY_COMP_DOC>(comp);
		const size_t& t = entComp._compType;

		// Generates the correct Component type
		if (t == typeid(ComponentLoadAudio).hash_code())
		{
			entComp._ent.AttachComponent<ComponentLoadAudio>();
			entComp._ent.getComponent<ComponentLoadAudio>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(ComponentGraphics).hash_code())
		{
			entComp._ent.AttachComponent<ComponentGraphics>();
			entComp._ent.getComponent<ComponentGraphics>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(ColliderComponent).hash_code())
		{
			entComp._ent.AttachComponent<ColliderComponent>();
			entComp._ent.getComponent<ColliderComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(RigidBody).hash_code())
		{
			entComp._ent.AttachComponent<RigidBody>();
			entComp._ent.getComponent<RigidBody>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(LightComponent).hash_code())
		{
			entComp._ent.AttachComponent<LightComponent>();
			entComp._ent.getComponent<LightComponent>()->Read(*entComp._rjDoc);
		}

		else if (t == typeid(ScriptComponent).hash_code())
		{
			entComp._ent.AttachComponent<ScriptComponent>();
			entComp._ent.getComponent<ScriptComponent>()->Read(*entComp._rjDoc);
		}

		else if (t == typeid(CanvasComponent).hash_code())
		{
			entComp._ent.AttachComponent<CanvasComponent>();
			entComp._ent.getComponent<CanvasComponent>()->Read(*entComp._rjDoc);
		}

		else if (t == typeid(CScriptComponent).hash_code())
		{
			entComp._ent.AttachComponent<CScriptComponent>();
			entComp._ent.getComponent<CScriptComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(PlayerStatsComponent).hash_code())
		{
			entComp._ent.AttachComponent<PlayerStatsComponent>();
			entComp._ent.getComponent<PlayerStatsComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(CauldronStatsComponent).hash_code())
		{
			entComp._ent.AttachComponent<CauldronStatsComponent>();
			entComp._ent.getComponent<CauldronStatsComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(VariablesComponent).hash_code())
		{
			entComp._ent.AttachComponent<VariablesComponent>();
			entComp._ent.getComponent<VariablesComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(NavComponent).hash_code())
		{
			entComp._ent.AttachComponent<NavComponent>();
			entComp._ent.getComponent<NavComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(AnimationComponent).hash_code())
		{
			entComp._ent.AttachComponent<AnimationComponent>();
			entComp._ent.getComponent<AnimationComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(WayPointMapComponent).hash_code())
		{
			entComp._ent.AttachComponent<WayPointMapComponent>();
			entComp._ent.getComponent<WayPointMapComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(WayPointComponent).hash_code())
		{
			entComp._ent.AttachComponent<WayPointComponent>();
			entComp._ent.getComponent<WayPointComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(EmitterComponent).hash_code())
		{
			entComp._ent.AttachComponent<EmitterComponent>();
			entComp._ent.getComponent<EmitterComponent>()->Read(*entComp._rjDoc);
			//entComp._ent.getComponent<EmitterComponent>()->_emitterID = NS_GRAPHICS::EmitterSystem::GetInstance().AddEmitter();
		}
		else if (t == typeid(CameraComponent).hash_code())
		{
			entComp._ent.AttachComponent<CameraComponent>();
			entComp._ent.getComponent<CameraComponent>()->Read(*entComp._rjDoc);
		}
		else if (t == typeid(ListenerComponent).hash_code())
		{
			entComp._ent.AttachComponent<ListenerComponent>();
			entComp._ent.getComponent<ListenerComponent>()->Read(*entComp._rjDoc);
		}

		return comp;
	};

	COMMAND removeComp =
		[](std::any comp)
	{
		ENTITY_COMP_DOC entComp = std::any_cast<ENTITY_COMP_DOC>(comp);
		const size_t& t = entComp._compType;

		// Generates the correct Component type
		if (t == typeid(ComponentLoadAudio).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<ComponentLoadAudio>()->Write());
			entComp._ent.RemoveComponent<ComponentLoadAudio>();
		}
		else if (t == typeid(ComponentGraphics).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<ComponentGraphics>()->Write());
			entComp._ent.RemoveComponent<ComponentGraphics>();
		}
		else if (t == typeid(ColliderComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<ColliderComponent>()->Write());
			entComp._ent.RemoveComponent<ColliderComponent>();
		}
		else if (t == typeid(RigidBody).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<RigidBody>()->Write());
			entComp._ent.RemoveComponent<RigidBody>();
		}
		else if (t == typeid(LightComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<LightComponent>()->Write());
			//NS_GRAPHICS::LightSystem::GetInstance().DetachLightComponent(entComp._ent);
			// Remove light from scene
			entComp._ent.getComponent<LightComponent>()->SetType(NS_GRAPHICS::Lights::INVALID_TYPE);
			entComp._ent.RemoveComponent<LightComponent>();
		}

		else if (t == typeid(ScriptComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<ScriptComponent>()->Write());
			entComp._ent.RemoveComponent<ScriptComponent>();
		}

		else if (t == typeid(CanvasComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<CanvasComponent>()->Write());
			entComp._ent.RemoveComponent<CanvasComponent>();
		}

		else if (t == typeid(CScriptComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<CScriptComponent>()->Write());
			entComp._ent.RemoveComponent<CScriptComponent>();
		}
		else if (t == typeid(PlayerStatsComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<PlayerStatsComponent>()->Write());
			entComp._ent.RemoveComponent<PlayerStatsComponent>();
		}
		else if (t == typeid(CauldronStatsComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<CauldronStatsComponent>()->Write());
			entComp._ent.RemoveComponent<CauldronStatsComponent>();
		}
		else if (t == typeid(VariablesComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<VariablesComponent>()->Write());
			entComp._ent.RemoveComponent<VariablesComponent>();
		}
		else if (t == typeid(NavComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<NavComponent>()->Write());
			entComp._ent.RemoveComponent<NavComponent>();
		}
		else if (t == typeid(AnimationComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<AnimationComponent>()->Write());
			entComp._ent.RemoveComponent<AnimationComponent>();
		}
		else if (t == typeid(WayPointMapComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<WayPointMapComponent>()->Write());
			entComp._ent.RemoveComponent<WayPointMapComponent>();
		}
		else if (t == typeid(WayPointComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<WayPointComponent>()->Write());
			entComp._ent.RemoveComponent<WayPointComponent>();
		}
		else if (t == typeid(EmitterComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<EmitterComponent>()->Write());
			NS_GRAPHICS::EmitterSystem::GetInstance().RemoveEmitterByID(entComp._ent.getComponent<EmitterComponent>()->_emitterID);
			entComp._ent.RemoveComponent<EmitterComponent>();
		}
		else if (t == typeid(CameraComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<CameraComponent>()->Write());
			entComp._ent.RemoveComponent<CameraComponent>();
		}
		else if (t == typeid(ListenerComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<ListenerComponent>()->Write());
			entComp._ent.RemoveComponent<ListenerComponent>();
		}

		return std::any(entComp);
	};

	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_ATTACH_COMP"),
		createComp, removeComp);
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_REMOVE_COMP"),
		removeComp, createComp);

	// PASTE COMPONENTS ==================================================================================================================
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_CAMERA"),
		std::function(Paste_Component<CameraComponent>), std::function(Paste_Component<CameraComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_COLLIDER"),
		std::function(Paste_Component<ColliderComponent>), std::function(Paste_Component<ColliderComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_AUDIO"),
		std::function(Paste_Component<ComponentLoadAudio>), std::function(Paste_Component<ComponentLoadAudio>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_GRAPHICS"),
		std::function(Paste_Component<GraphicsComponent>), std::function(Paste_Component<GraphicsComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_LIGHT"),
		std::function(Paste_Component<LightComponent>), std::function(Paste_Component<LightComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_RIGIDBODY"),
		std::function(Paste_Component<RigidBody>), std::function(Paste_Component<RigidBody>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_SCRIPT"),
		std::function(Paste_Component<ScriptComponent>), std::function(Paste_Component<ScriptComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_CANVAS"),
		std::function(Paste_Component<CanvasComponent>), std::function(Paste_Component<CanvasComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_ANIMATION"),
		std::function(Paste_Component<AnimationComponent>), std::function(Paste_Component<AnimationComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_EMITTER"),
		std::function(Paste_Component<EmitterComponent>), std::function(Paste_Component<EmitterComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_EMITTER"),
		std::function(Paste_Component<ListenerComponent>), std::function(Paste_Component<ListenerComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_PLAYER"),
		std::function(Paste_Component<PlayerStatsComponent>), std::function(Paste_Component<PlayerStatsComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_CAULDRON"),
		std::function(Paste_Component<CauldronStatsComponent>), std::function(Paste_Component<CauldronStatsComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_VARIABLE"),
		std::function(Paste_Component<VariablesComponent>), std::function(Paste_Component<VariablesComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_NAVIGATOR"),
		std::function(Paste_Component<NavigatorComponent>), std::function(Paste_Component<NavigatorComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_WAYPOINTMAP"),
		std::function(Paste_Component<WayPointMapComponent>), std::function(Paste_Component<WayPointMapComponent>));
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_PASTE_COMP_WAYPOINT"),
		std::function(Paste_Component<WayPointComponent>), std::function(Paste_Component<WayPointComponent>));


	// PASTE COMPONENTS ==================================================================================================================

	// INPUTS TO CHANGE GIZMO OPERATIONS
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("CHANGE_GIZMO_TRANSFORM_TRANSLATE", SystemInput_ns::IKEY_W, "TRANSLATE", SystemInput_ns::OnPress,
		[this]()
		{
			if (!ImGui::IsAnyItemFocused())
				_mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("CHANGE_GIZMO_TRANSFORM_ROTATE", SystemInput_ns::IKEY_E, "ROTATE", SystemInput_ns::OnPress,
		[this]()
		{
			if (!ImGui::IsAnyItemFocused())
				_mCurrentGizmoOperation = ImGuizmo::ROTATE;
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("CHANGE_GIZMO_TRANSFORM_SCALE", SystemInput_ns::IKEY_R, "SCALE", SystemInput_ns::OnPress,
		[this]()
		{
			if (!ImGui::IsAnyItemFocused())
				_mCurrentGizmoOperation = ImGuizmo::SCALE;
		});
}

void InspectorWindow::Run()
{
	//Check for valid Entity Id
	if (LE_ECHELPER->GetSelectedEntityID() != -1 || LE_ECHELPER->GetPrefabInst().prefab_id != -1)
	{
		bool is_prefab = false;

		if (!ImGui::IsWindowAppearing() && !LE_ECHELPER->setFocus)
		{
			ImGui::SetWindowFocus();
			LE_ECHELPER->setFocus = true;
		}

		NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = nullptr;
		//Get entity
		//Entity ent;
		int id = -1;
		if (LE_ECHELPER->GetSelectedEntityID() != -1)
		{
			g_ecman = G_ECMANAGER;
			id = LE_ECHELPER->GetSelectedEntityID();
			is_prefab = false;
		}
		else if (LE_ECHELPER->GetPrefabInst().prefab_id != -1)
		{
			g_ecman = G_ECMANAGER_PREFABS;
			id = LE_ECHELPER->GetPrefabInst().prefab_id;
			is_prefab = true;
		}
		else
		{
			TracyMessageL("InspectorWindow::Run: No set manager is allocated");
			SPEEDLOG("InspectorWindow::Run: No set manager is allocated");
			//std::cout << "No set manager is allocated" << std::endl;
		}
		Entity selected_ent = g_ecman->getEntity(id);
		std::string& selected_ent_name = g_ecman->EntityName[id];
		// Entity name
		_levelEditor->LE_AddInputText("Name##Entity", selected_ent_name, 256);

		ImGui::SameLine(0, 10);
		ImGui::Text("ID : ");

		ImGui::SameLine(0, 10);
		ImGui::Text(std::to_string(id).c_str());//std::to_string(LE_ECHELPER->GetSelectedEntityID()).c_str());

		if (is_prefab)
		{
			_levelEditor->LE_AddButton("Update prefabs", [selected_ent, selected_ent_name, this]()
				{
					std::vector<std::pair<std::string, LS::ENTITY_COMP_DATA>> _allCopiedObjs;
					LS::ENTITY_COMP_DATA compData;
					LS::ReadIntoCompVec(selected_ent, &compData, true);

					for (Entity ent : G_ECMANAGER->getEntityContainer())
					{
						std::string ent_name = G_ECMANAGER->EntityName[ent.getId()];
						if (LS::findCaseInsensitive(ent_name, selected_ent_name) == std::string::npos)
							continue;
						//pref_name_to_update;

						_allCopiedObjs.push_back(std::make_pair(ent_name, compData));
					}
					_levelEditor->LE_AccessWindowFunc
					("Console", &ConsoleLog::RunCommand, std::string("HIERARCHY_COPY_OBJECTS"), std::any(_allCopiedObjs));

				}
			);
		}
		//Componenets layout
		ComponentLayout(selected_ent);
	}
	// Print out the ID of the entity (Debug purposes)
}

bool InspectorWindow::GetIfGizmoManipulate()
{
	return _lastPos_Start;
}

void InspectorWindow::ComponentLayout(Entity& ent)
{
	TransformComp(ent);
	//Standard bool for all component to use
	_notRemove = true;

	CameraComp(ent);

	ColliderComp(ent);

	AudioComp(ent);

	GraphicsComp(ent);

	LightComp(ent);

	RigidBodyComp(ent);

	ScriptComp(ent);

	CanvasComp(ent);

	AnimationComp(ent);

	EmitterComp(ent);

	ListenerComp(ent);

	CScriptComp(ent);

	PlayerStatsComp(ent);

	CauldronStatsComp(ent);

	VariableComp(ent);

	NavComp(ent);

	WayPointPathComp(ent);

	WayPointComp(ent);

	AddSelectedComps(ent);
}

void InspectorWindow::SetFOV(const float& FOV)
{
	_fov = FOV;
}

void InspectorWindow::TransformComp(Entity& ent)
{
	TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
	if (trans_comp != NULL)
	{
		ImGui::NewLine();
		if (ImGui::InputInt("Tag", &(trans_comp->_tag), 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			TAG_HANDLER->InsertTagToUsed(trans_comp->_tag);
		}
		ImGui::NewLine();
		std::string enam = trans_comp->_entityName.toString();
		_levelEditor->LE_AddInputText("Entity Name", enam, 500, ImGuiInputTextFlags_EnterReturnsTrue,
			[&enam, &trans_comp]()
			{
				trans_comp->_entityName = enam;
			});
		bool toRemove = false;
		int index = 0;
		for (int& tn : trans_comp->_tagNames)
		{
			if (_levelEditor->LE_AddCombo("##ADDTAGNAME" + std::to_string(index), tn, TAGNAMES) && tn == 0 && trans_comp->_tagNames.size() > 1)
				toRemove = true;
			++index;
		}
		if (toRemove)
			trans_comp->_tagNames.pop_back();
		if (trans_comp->_tagNames.size() > 1 && trans_comp->_tagNames.back() != 0)
			trans_comp->_tagNames.push_back(0);

		TransformGizmo(trans_comp);
		// the snap window
		TransformSnap(ent);
		//ImGui::NewLine();
		//int tag = trans_comp->_tag;
		/*
		if (ImGui::CollapsingHeader("Transform"))
		{
			//float* rotation = &ent_selected->GetComponent<TransformComponent>().GetRotation().m[2];
			ImGui::InputFloat3("Position", glm::value_ptr(trans_comp->_position)); //,-100.f, 100.f); // Edit 3 floats representing a color
			ImGui::InputFloat3("Scale", glm::value_ptr(trans_comp->_scale)); //, 0.0f, 100.f);
			ImGui::InputFloat3("Rotation", glm::value_ptr(trans_comp->_rotation));
		}
		*/
	}
}

void InspectorWindow::ColliderComp(Entity& ent)
{
	ComponentCollider* col_comp = ent.getComponent<ComponentCollider>();
	if (col_comp != NULL)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
		bool editedComp = false;

		//~~! Need Help==//
		//1. Check collider type that it have
		//2. Get the right collider type
		//3. Insert name
		std::string name = "Collider";	//e.g.
		switch (col_comp->GetColliderT())
		{
		case COLLIDERS::PLANE:
		{
			name = "Plane " + name;
			break;
		}
		case COLLIDERS::AABB:
		{
			name = "AABB " + name;
			break;
		}
		case COLLIDERS::SPHERE:
		{
			name = "Sphere " + name;
			break;
		}
		case COLLIDERS::OBB:
		{
			name = "OBB " + name;
			break;
		}
		case COLLIDERS::CAPSULE:
		{
			name = "Capsule " + name;
			break;
		}
		}

		//4. May need loop to loop through all collider
		if (ImGui::CollapsingHeader(name.c_str(), &_notRemove))
		{
			int colEnum = (int)col_comp->GetColliderT();

			_levelEditor->LE_AddCombo("Collider Type##COLLIDER", colEnum,
				{
					"Plane",
					"AABB",
					"Sphere",
					"OBB",
					"Capsule",
				});

			if (colEnum != (int)col_comp->GetColliderT())
			{
				col_comp->SetColliderT((COLLIDERS)colEnum);

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			glm::vec3 glmVal = { col_comp->center };
			float* glmPtr = glm::value_ptr(glmVal);

			ImGui::Checkbox("IsCollidable##Collider", &col_comp->isCollidable);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputInt("Collider Tag##COLTAG", &col_comp->colliderTag);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (ImGui::InputFloat3("Center##COLLIDER", glmPtr, 3))
			{
				col_comp->center = glm::make_vec3(glmPtr);
			}

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			glmVal = { col_comp->extend };
			glmPtr = glm::value_ptr(glmVal);

			if (ImGui::InputFloat3("Extend##COLLIDER", glmPtr, 3))
			{
				col_comp->extend = glm::make_vec3(glmPtr);
			}

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			glmVal = { col_comp->rotation };
			glmPtr = glm::value_ptr(glmVal);

			if (ImGui::InputFloat3("Rotation##COLLIDER", glmPtr, 3))
			{
				col_comp->rotation = glm::make_vec3(glmPtr);
			}

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			// Undo-Redo for Components
			//_origComp = ImGui::IsItemActivated() ? ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() } : _origComp;
			//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
		}

		//Remove component
		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ComponentCollider>()->Write(), typeid(ComponentCollider).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<ComponentCollider>()->Write() };
				col_comp->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_COLLIDER"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}

void InspectorWindow::AudioComp(Entity& ent)
{
	ComponentLoadAudio* aud_manager = ent.getComponent<ComponentLoadAudio>();
	if (aud_manager != nullptr)
	{
		if (ImGui::CollapsingHeader("Audio Manager##AudioComp", &_notRemove))
		{
			if (ImGui::Button("Add Audio##AudioCOmp"))
			{
				aud_manager->MyAudios.push_back(ComponentLoadAudio::data());
			}
			size_t size = aud_manager->MyAudios.size();
			// Remove vector
			int remove = -1;
			if (ImGui::BeginTabBar("Audio##AudioComp", ImGuiTabBarFlags_AutoSelectNewTabs))
			{
				for (size_t i = 0; i < size; ++i)
				{
					ComponentLoadAudio::data& MyData = aud_manager->MyAudios.at(i);
					std::string sIndex = std::to_string(i);
					std::string header = sIndex + "| " + std::to_string(MyData.index);
					if (ImGui::BeginTabItem(header.c_str(), &MyData.ImGuiTab))
					{
						ImGui::InputInt(std::string("Index##AudioComp" + sIndex).c_str(), &MyData.index);
						// Variables
						ImGui::Checkbox(std::string("IsLoop##AudioComp" + sIndex).c_str(), &MyData.isLoop);
						ImGui::Checkbox(std::string("PlayOnAwake##AudioComp" + sIndex).c_str(), &MyData.playOnAwake);
						//ImGui::Checkbox(std::string("Mute##AudioComp" + sIndex).c_str(), &MyData.mute);
						ImGui::InputFloat(std::string("Volume##AudioComp" + sIndex).c_str(), &MyData.volume);
						ImGui::Checkbox(std::string("Is3D##AudioComp" + sIndex).c_str(), &MyData.is3D);
						if (MyData.is3D)
						{
							ImGui::InputFloat(std::string("Min Dist##AudioComp" + sIndex).c_str(), &MyData.minDist);
							ImGui::InputFloat(std::string("Max Dist##AudioComp" + sIndex).c_str(), &MyData.maxDist);
						}
						ImGui::EndTabItem();
					}

					if (MyData.ImGuiTab == false)
						remove = (int)i;
				}
				ImGui::EndTabBar();
			}
			if (remove != -1)
				aud_manager->MyAudios.erase(remove);
		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<ComponentLoadAudio>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ComponentLoadAudio>()->Write(), typeid(ComponentLoadAudio).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}
		ImGui::Separator();
	}
}

void InspectorWindow::GraphicsComp(Entity& ent)
{
	GraphicsComponent* graphics_comp = ent.getComponent<GraphicsComponent>();
	if (graphics_comp != nullptr)
	{
		if (ImGui::CollapsingHeader("Graphics component", &_notRemove))
		{
			bool renderTextures = !(graphics_comp->_renderType == RENDERTYPE::SOLID);

			ImGui::Checkbox("Render Textures", &renderTextures);

			if (!renderTextures)
				graphics_comp->SetRenderType(RENDERTYPE::SOLID);
			else
				graphics_comp->SetRenderType(RENDERTYPE::TEXTURED);

			ImGui::Checkbox("IsActive##Graphic", &graphics_comp->_isActive);

			ImGui::Checkbox("Emission##Graphic", &graphics_comp->_renderEmission);

			ImGui::ColorEdit3("Emission Color##Graphics", glm::value_ptr(graphics_comp->_pbrData._emissive));

			float emissive_intensity = graphics_comp->GetEmissiveIntensity() * 100.f;

			if (ImGui::DragFloat("Emissive Intensity", &emissive_intensity, 0.1f, 0.1f, 100.f))
				graphics_comp->SetEmissiveIntensity(emissive_intensity / 100.f);

			std::string mod = graphics_comp->_modelFileName.toString();
			std::string tex = graphics_comp->_albedoFileName.toString();
			std::string normal = graphics_comp->_normalFileName.toString();
			std::string metallic = graphics_comp->_metallicFileName.toString();
			std::string roughness = graphics_comp->_roughnessFileName.toString();
			std::string ao = graphics_comp->_aoFileName.toString();
			std::string specular = graphics_comp->_specularFileName.toString();

			_levelEditor->LE_AddInputText("Model file", mod, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&graphics_comp, &mod, &ent]()
				{
					graphics_comp->AddModel(mod);

					//Only if valid model
					if (graphics_comp->_modelID >= 0)
					{
						if (NS_GRAPHICS::ModelManager::GetInstance()._models[graphics_comp->_modelID]->_isAnimated)
						{
							ent.AttachComponent<ComponentAnimation>();
							ComponentAnimation* anim = ent.getComponent<ComponentAnimation>();
							anim->_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();
							AnimationController* animCtrl = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID];
							for (auto& anims : NS_GRAPHICS::ModelManager::GetInstance()._models[graphics_comp->_modelID]->_animations)
							{
								animCtrl->_allAnims.insert(anims.first);
							}
						}
					}
				});

			// Drag and Drop from Asset Inspector onto Model File Name
			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &mod, &graphics_comp, &ent](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "fbx" || fileType == "obj" || fileType == "model")
					{
						//assimp doesnt deal with preceding slash
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						mod = data;
						graphics_comp->AddModel(mod);

						if (NS_GRAPHICS::ModelManager::GetInstance()._models[graphics_comp->_modelID]->_isAnimated)
						{
							ent.AttachComponent<ComponentAnimation>();
							ComponentAnimation* anim = ent.getComponent<ComponentAnimation>();
							anim->_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();
							AnimationController* animCtrl = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID];
							for (auto& anims : NS_GRAPHICS::ModelManager::GetInstance()._models[graphics_comp->_modelID]->_animations)
							{
								animCtrl->_allAnims.insert(anims.first);
							}
						}
					}
				});

			_levelEditor->LE_AddInputText("Texture file", tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&tex, &graphics_comp]()
				{
					graphics_comp->AddAlbedoTexture(tex);
				});

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &tex, &graphics_comp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "png" || fileType == "tga" || fileType == "dds")
					{
						//SOIL doesnt deal with preceding slash
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						tex = data;
						graphics_comp->AddAlbedoTexture(tex);
					}
				});

			_levelEditor->LE_AddInputText("Normal file", normal, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&normal, &graphics_comp]()
				{
					graphics_comp->AddNormalTexture(normal);
				});

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &normal, &graphics_comp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "png" || fileType == "tga" || fileType == "dds")
					{
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						normal = data;
						graphics_comp->AddNormalTexture(normal);
					}
				});

			_levelEditor->LE_AddInputText("Metallic file", metallic, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&metallic, &graphics_comp]()
				{
					graphics_comp->AddMetallicTexture(metallic);
				});

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &metallic, &graphics_comp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "png" || fileType == "tga" || fileType == "dds")
					{
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						metallic = data;
						graphics_comp->AddMetallicTexture(metallic);
					}
				});

			_levelEditor->LE_AddInputText("Roughness file", roughness, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&roughness, &graphics_comp]()
				{
					graphics_comp->AddRoughnessTexture(roughness);
				});

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &roughness, &graphics_comp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "png" || fileType == "tga" || fileType == "dds")
					{
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						roughness = data;
						graphics_comp->AddRoughnessTexture(roughness);
					}
				});

			_levelEditor->LE_AddInputText("AO file", ao, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&ao, &graphics_comp]()
				{
					graphics_comp->AddAOTexture(ao);
				});

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &ao, &graphics_comp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "png" || fileType == "tga" || fileType == "dds")
					{
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						ao = data;
						graphics_comp->AddAOTexture(ao);
					}
				});

			_levelEditor->LE_AddInputText("Specular file", specular, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&specular, &graphics_comp]()
				{
					graphics_comp->AddSpecularTexture(specular);
				});

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &specular, &graphics_comp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "png" || fileType == "tga" || fileType == "dds")
					{
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						specular = data;
						graphics_comp->AddSpecularTexture(specular);
					}
				});

			ImGui::Separator();

			ImGui::Text("Materials   ");
			ImGui::SameLine(ImGui::GetWindowWidth() - 40);
			if (ImGui::SmallButton("V"))
			{
				ImGui::OpenPopup("SubMenuSettingsGraphics");
			}
			if (ImGui::BeginPopup("SubMenuSettingsGraphics"))
			{
				if (ImGui::Button("Save"))
				{
					// Do Stuff here
					COMDLG_FILTERSPEC rgSpec[] =
					{
						{ L"*.mater", L"*.mater" }
					};
					// Gets the RELATIVE File Path to Save to
					std::string fileToSaveTo = WindowsSaveFileBox(_levelEditor->LE_GetWindowHandle(), rgSpec, 1);

					if (fileToSaveTo.size())
					{
						//... Save Material Data
						graphics_comp->SaveMaterialDataFile(fileToSaveTo);
					}

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Load"))
				{
					COMDLG_FILTERSPEC rgSpec[] =
					{
						{ L"*.mater", L"*.mater" }
					};
					// Gets the RELATIVE File Path to Open from
					std::string fileToOpen = WindowsOpenFileBox(_levelEditor->LE_GetWindowHandle(), rgSpec, 1);

					if (fileToOpen.size())
					{
						//... Load Material Data
						graphics_comp->LoadMaterialDataFile(fileToOpen);
					}

					// Do Stuff here
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			/*ImGui::ColorEdit3("Diffuse##Graphics", glm::value_ptr(graphics_comp->_materialData._diffuse));

			ImGui::ColorEdit3("Ambient##Graphics", glm::value_ptr(graphics_comp->_materialData._ambient));

			ImGui::ColorEdit3("Specular##Graphics", glm::value_ptr(graphics_comp->_materialData._specular));

			ImGui::InputFloat("Shininess", &graphics_comp->_materialData._shininess);*/

			ImGui::ColorEdit3("Color##Graphics", glm::value_ptr(graphics_comp->_pbrData._albedo));

			ImGui::DragFloat("Metallic", &graphics_comp->_pbrData._metallic, 0.1f, 0.f, 1.f);

			ImGui::DragFloat("Roughness", &graphics_comp->_pbrData._roughness, 0.1f, 0.f, 1.f);


			float alpha = graphics_comp->GetAlpha();
			if (ImGui::DragFloat("Alpha", &alpha, 0.1f, 0.f, 1.f))
				graphics_comp->SetAlpha(alpha);

			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<GraphicsComponent>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<GraphicsComponent>()->Write(), typeid(GraphicsComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		ImGui::Separator();
	}
}

void InspectorWindow::RigidBodyComp(Entity& ent)
{
	RigidBody* rb = ent.getComponent<RigidBody>();
	if (rb != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<RigidBody>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Rigid Body", &_notRemove))
		{
			//_levelEditor->LE_AddInputText("##GRAPHICS_1", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::Checkbox("IsStatic", &rb->isStatic);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::Checkbox("IsGravity", &rb->isGravity);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("Mass", &rb->mass);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("Friction", &rb->friction);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat3("Force", rb->force.m);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat3("Acceleration", rb->acceleration.m);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<RigidBody>()->Write(), typeid(RigidBody).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			//ent.RemoveComponent<RigidBody>();
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<RigidBody>()->Write() };
				rb->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_RIGIDBODY"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}



void InspectorWindow::LightComp(Entity& ent)
{
	static int LIGHT = (int)NS_GRAPHICS::Lights::INVALID_TYPE;
	ComponentLight* light = ent.getComponent<ComponentLight>();
	if (light != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentLight>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Light", &_notRemove))
		{
			// use tmp variable to store active
			// this is so that we can call the function to set active state
			bool active_state = light->GetActive();

			//ImGui::Checkbox("Is Active", &light->_isActive);
			ImGui::Checkbox("Is Active", &active_state);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			light->SetActive(active_state);

			const char* lights[] = { "Directional", "Point", "Spot", "None" };

			if (light->GetActive() == false)
				LIGHT = (int)light->GetInactiveType();
			else
				LIGHT = (int)light->GetType();

			if (ImGui::Combo("Light Type", &LIGHT, lights, IM_ARRAYSIZE(lights)))
			{
				// Checks if set was successful
				if (!light->SetType((NS_GRAPHICS::Lights)LIGHT))
				{
					// Send error prompt if failed
					ImGui::OpenPopup("There can only be one sun and so many stars uwu");
				}
				else
				{
					// Undo-Redo for Components
					_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
					editedComp = true;
				}
			}

			if (ImGui::BeginPopupModal("There can only be one sun and so many stars uwu"))
			{
				ImGui::Text("You dun goofed A.K.A there are already enough lights of that type in the scene.");
				if (ImGui::Button("OK", ImVec2(240, 0)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::EndPopup();
			}

			glm::vec3 lightColor = light->GetColor();

			if (ImGui::ColorEdit3("Diffuse", glm::value_ptr(lightColor)))
			{
				light->SetColor(lightColor);
			}

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if ((NS_GRAPHICS::Lights)LIGHT != NS_GRAPHICS::Lights::INVALID_TYPE)
			{
				float intensity = light->GetIntensity();
				if (ImGui::DragFloat("Intensity", &intensity))
				{
					if (intensity < 0.f)
						intensity = 0.f;
					light->SetIntensity(intensity);
				}

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}


			if ((NS_GRAPHICS::Lights)LIGHT == NS_GRAPHICS::Lights::POINT)
			{
				float radius = light->GetRadius();
				if (ImGui::DragFloat("Radius", &radius))
				{
					if (radius < 0.0001f)
						radius = 0.01f;
					light->SetRadius(radius);
				}

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}

			if ((NS_GRAPHICS::Lights)LIGHT == NS_GRAPHICS::Lights::SPOT)
			{
				float cutoff = light->GetCutOff();
				float outercutoff = light->GetOuterCutOff();

				if (ImGui::InputFloat("Cut off", &cutoff))
					light->SetCutOff(cutoff);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				if (ImGui::InputFloat("Outer cut off", &outercutoff))
					light->SetOuterCutOff(outercutoff);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}
		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ComponentLight>()->Write(), typeid(ComponentLight).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<ComponentLight>()->Write() };
				light->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_LIGHT"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}

void InspectorWindow::ScriptComp(Entity& ent)
{
	ScriptComponent* Script_comp = ent.getComponent<ScriptComponent>();
	if (Script_comp != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ScriptComponent>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Script component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##CScript", &Script_comp->_isActive);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			MonoClass* klass = nullptr;
			void* iter = NULL;
			MonoClassField* field = nullptr;
			ScriptComponent::data& monoData = Script_comp->_MonoData;
			if (monoData._pInstance)
			{
				klass = MonoWrapper::GetMonoClass(monoData._pInstance);
				field = mono_class_get_fields(klass, &iter);
			}
			//ImGui::SameLine();
			//// Get new values
			//if (ImGui::Button("Refresh"))
			//{
			//  monoData._pInstance = MonoWrapper::ConstructObject(Script_comp->_ScriptName.toString());
			//  monoData._GCHandle = MonoWrapper::ConstructGCHandle(Script_comp->_MonoData._pInstance);
			//  int ID = ent.getId();
			//  MonoWrapper::SetObjectFieldValue(monoData._pInstance, "id", ID);
			//}
			//}
			// Loop through C# fields/variable
			while (field)
			{
				// Name of variables
				const char* var_name = mono_field_get_name(field);
				// Type id
				MonoType* mono_type = mono_field_get_type(field);
				int var_typeid = mono_type_get_type(mono_type);
				// To check for public fields/variables
				unsigned var_flag = mono_field_get_flags(field);
				if (var_flag == MONO_FIELD_ATTR_PUBLIC) // MONO_FIELD_ATTR_PUBLIC
				{
					bool bChanged = false; // If value changed, save it back to mono.
					bChanged;
					// Inspect values here
					// Name of variable
					std::string sName = std::string(var_name) + " : ";
					_levelEditor->LE_AddText("\t");
					ImGui::SameLine();
					// type of variable
					if (var_typeid == MONO_TYPE_BOOLEAN) // bool
					{
						//std::cout << "Bool" << std::endl;
						sName += " bool";
						bool typeBool = MonoWrapper::GetObjectFieldValue<bool>(monoData._pInstance, var_name);
						if (ImGui::Checkbox(sName.c_str(), &typeBool))
							MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeBool);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else if (var_typeid == MONO_TYPE_I4) // int
					{
						//std::cout << "Int" << std::endl;
						sName += " int";
						int typeInt = MonoWrapper::GetObjectFieldValue<int>(monoData._pInstance, var_name);
						if (ImGui::InputInt(sName.c_str(), &typeInt))
							MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeInt);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else if (var_typeid == MONO_TYPE_U4) // unsigned
					{
						//std::cout << "Unsigned" << std::endl;
						sName += " unsigned";
						int typeUnsigned = MonoWrapper::GetObjectFieldValue<unsigned>(monoData._pInstance, var_name);
						if (ImGui::InputInt(sName.c_str(), &typeUnsigned))
							MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeUnsigned);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else if (var_typeid == MONO_TYPE_R4) // float
					{
						//std::cout << "Float" << std::endl;
						sName += " float";
						float typeFloat = MonoWrapper::GetObjectFieldValue<float>(monoData._pInstance, var_name);
						if (ImGui::InputFloat(sName.c_str(), &typeFloat))
							MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeFloat);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else if (var_typeid == MONO_TYPE_R8) // double
					{
						//std::cout << "Double" << std::endl;
						sName += " double";
						double typeDouble = MonoWrapper::GetObjectFieldValue<double>(monoData._pInstance, var_name);
						if (ImGui::InputDouble(sName.c_str(), &typeDouble))
							MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeDouble);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else if (var_typeid == MONO_TYPE_STRING) // string
					{
						//std::cout << "String" << std::endl;
						sName += " string";
						MonoString* monoString = MonoWrapper::GetObjectFieldValue<MonoString*>(monoData._pInstance, var_name);
						std::string saved;
						///**** For saving/getting ID ****/
						//int i = 0;
						//for (; i < Script_comp->_savedCount; ++i)
						//{
						//  // Found ID
						//  if (var_name == Script_comp->_SavedID[i])
						//    break;
						//}
						//if (i == Script_comp->_savedCount)
						//{
						//  Script_comp->_SavedID[i] = var_name;
						//  ++Script_comp->_savedCount;
						//}
						///************************/
						if (monoString != nullptr)
							saved = MonoWrapper::ToString(monoString);
						//std::string& saved = Script_comp->_SavedStrings[i];
						//std::cout << "Saved: " << saved << std::endl;
						//std::cout << "Actual: " << Script_comp->_SavedStrings[i] << std::endl;
						_levelEditor->LE_AddInputText(sName.c_str(), saved, 500, ImGuiInputTextFlags_EnterReturnsTrue, [&saved, &monoString, &var_name, &monoData]()
							{
								monoString = MonoWrapper::ToMonoString(saved);
								MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, *monoString);
							});

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else if (var_typeid == MONO_TYPE_SZARRAY)
					{
						MonoArrayType* array_type = mono_type_get_array_type(mono_type);
						//ED_OUT("My Array Type: " + std::to_string(var_typeid));
						MonoClass* raw_class_type = array_type->eklass;
						MonoType* arr_mono_type = mono_class_get_type(raw_class_type);
						int my_type = mono_type_get_type(arr_mono_type);
						//int arr_sz = 0;

						if (ImGui::CollapsingHeader(std::string(std::string(var_name) + " : Array").c_str()))
						{
							//sName += " Array";
							Script_comp->arr_sz;
							ImGui::InputInt("Size", &Script_comp->arr_sz);
							if (Script_comp->arr_sz > 0)
							{
								switch (my_type)
								{
								case MONO_TYPE_BOOLEAN:
								{
									//bool* boolArr = new bool[Script_comp->arr_sz]();
									//for (int i = 0; i < Script_comp->arr_sz; ++i)
									//{
									//	if ((i) % 2)
									//		boolArr[i] = true;
									//}
									//MonoArray* monoArr = MonoWrapper::ToMonoArray<bool>(boolArr, raw_class_type, Script_comp->arr_sz);
									////int length = mono_array_length(monoArr);
									////for (int i = 0; i < length; ++i)
									////{
									////	bool val = (bool)mono_array_get(monoArr, bool, i);
									////	std::string output = "bool[" + std::to_string(i) + "]: " + std::to_string(val);
									////	TracyMessage(output.c_str(), output.size());
									////}

									////MonoWrapper::ToArray<bool>(monoArr, boolArr);
									////for (int i = 0; i < Script_comp->arr_sz; ++i)
									////{
									////	std::string index = "Index[" + std::to_string(i) + "]";
									////	boolArr[i] = ImGui::Checkbox(index.c_str(), boolArr + i);
									////}
									//MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, *monoArr);
									//MonoArray* myArr = MonoWrapper::GetObjectFieldValue<MonoArray*>(monoData._pInstance, var_name);
									//bool* boolArr1 = new bool[Script_comp->arr_sz]();
									//MonoWrapper::ToArray<bool>(myArr, boolArr1);
									//for (int i = 0; i < Script_comp->arr_sz; ++i)
									//{
									//	std::string output = "bool[" + std::to_string(i) + "]: " + std::to_string(boolArr1[i]);
									//	TracyMessage(output.c_str(), output.size());
									//}
									//delete[] boolArr;
									//delete[] boolArr1;
								}
								break;
								case MONO_TYPE_I4:
									break;
								case MONO_TYPE_U4:
									break;
								case MONO_TYPE_R4:
									break;
								case MONO_TYPE_R8:
									break;
								case MONO_TYPE_STRING:
									break;
								default:
									break;
								}
							}
							ImGui::Separator();
						}
					}
					else
					{
						_levelEditor->LE_AddText(var_name);
					}
				}
				// Move to next field
				field = mono_class_get_fields(klass, &iter);
				//ImGui::NewLine();
			}
			std::string tex = Script_comp->_ScriptName.toString();
			std::string old = tex;

			_levelEditor->LE_AddInputText("Script Name", tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&tex, &Script_comp]()
				{
					Script_comp->_ScriptName = tex;
				});

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				//<<<<<<< Updated upstream
				[this, &Script_comp, &tex, &activeRead, &editedComp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "cs")
					{
						std::string name = LE_GetFilename(*str);
						name.erase(name.end() - 3, name.end());
						Script_comp->_ScriptName = name;

						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;
					}
					/*
									[this, &Script_comp](std::string* str)
									{
										fs::path script_path= *str;

										if (script_path.extension() == ".cs")
										{
											Script_comp->_ScriptName = script_path.stem().string();
										}
										*/

				});
		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<GraphicsComponent>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ScriptComponent>()->Write(), typeid(ScriptComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<ScriptComponent>()->Write() };
				Script_comp->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_SCRIPT"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}

void InspectorWindow::CanvasComp(Entity& ent)
{
	CanvasComponent* canvas = ent.getComponent<CanvasComponent>();
	if (canvas != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<CanvasComponent>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Canvas component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##Canvas", &canvas->_isActive);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (_levelEditor->LE_AddCombo("Canvas Type", (int&)canvas->_canvasType,
				{
					"Screen",
					"World"
				}))
			{
				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			static std::string searchString = "";
			std::string search = searchString;
			_levelEditor->LE_AddInputText("Search##canvas", search, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&search]()
				{
					searchString = search;
				});

			size_t uiCount = canvas->_uiElements.size();
			for (size_t i = 0; i < uiCount; ++i)
			{
				UI_Element& ui = canvas->_uiElements.at(i);

				std::string tex = ui._fileName.toString();
				std::string uiName = ui._uiName.toString();

				std::string tempString = uiName;
				std::string tempSearchString = searchString;

				std::transform(tempString.begin(), tempString.end(), tempString.begin(), ::toupper);
				std::transform(tempSearchString.begin(), tempSearchString.end(), tempSearchString.begin(), ::toupper);

				if (searchString == "" || tempString.find(tempSearchString) != std::string::npos)
				{
					ImGui::Checkbox(std::string("IsActive##UI").append(std::to_string(i)).c_str(), &ui._isActive);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					if (_levelEditor->LE_AddCombo(std::string("UI Type").append(std::to_string(i)).c_str(), (int&)ui._type,
						{
							"IMAGE",
							"BUTTON"
						}))
					{
						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;
					}


					if (ImGui::Button(std::string("X##").append(std::to_string(i)).c_str()))
					{
						canvas->RemoveUI(i);

						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;
					}

					ImGui::SameLine();
					//canvas->_uiElements.at(i)._position;
					//canvas->_uiElements.at(i)._size;

					if (canvas->_canvasType == SCREEN_SPACE)
					{
						ImGui::InputFloat2(std::string("UIPosition##").append(std::to_string(i)).c_str(), glm::value_ptr(ui._position), 3);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

						ImGui::InputFloat(std::string("Layer Order##").append(std::to_string(i)).c_str(), &ui._position.z, 1);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else if (canvas->_canvasType == WORLD_SPACE)
					{
						ImGui::InputFloat3(std::string("UIPosition##").append(std::to_string(i)).c_str(), glm::value_ptr(ui._position), 3);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					}
					ImGui::InputFloat2(std::string("UISize##").append(std::to_string(i)).c_str(), glm::value_ptr(ui._size), 3);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					_levelEditor->LE_AddInputText(std::string("UIName##").append(std::to_string(i)).c_str(), uiName, 500, ImGuiInputTextFlags_EnterReturnsTrue,
						[&uiName, &ui, &i]()
						{
							ui._uiName = uiName;
						});

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					_levelEditor->LE_AddInputText(std::string("Image##").append(std::to_string(i)).c_str(), tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
						[&tex, &ui, &i]()
						{
							ui.AddTexture(tex);
							ui._fileName = tex;
						});

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
						[this, &tex, &ui, &i, &activeRead, &editedComp](std::string* str)
						{
							std::string data = *str;
							std::transform(data.begin(), data.end(), data.begin(),
								[](unsigned char c)
								{ return (char)std::tolower(c); });

							std::string fileType = LE_GetFileType(data);
							if (fileType == "png" || fileType == "tga" || fileType == "dds")
							{
								//SOIL doesnt deal with preceding slash
								if (data[0] == '\\')
								{
									data.erase(0, 1);
								}
								tex = data;
								ui.AddTexture(tex);
								ui._fileName = tex;

								// Undo-Redo for Components
								_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
								editedComp = true;
							}
						});

					ImGui::Checkbox(std::string("Animatable##").append(std::to_string(i)).c_str(), &ui._isAnimated);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					if (ui._isAnimated)
					{
						ImGui::InputScalar(std::string("Row##").append(std::to_string(i)).c_str(), ImGuiDataType_U32, &ui._row);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

						ImGui::InputScalar(std::string("Column##").append(std::to_string(i)).c_str(), ImGuiDataType_U32, &ui._column);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

						ImGui::InputScalar(std::string("Total Frame##").append(std::to_string(i)).c_str(), ImGuiDataType_U32, &ui._totalFrame);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

						if (ImGui::InputScalar(std::string("Frames per second##").append(std::to_string(i)).c_str(), ImGuiDataType_U32, &ui._framesPerSecond))
						{
							if (ui._framesPerSecond == 0)
							{
								ui._animationRate = 0.0f;
							}
							else
							{
								ui._animationRate = 1.0f / ui._framesPerSecond;
							}
						}

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

						ImGui::Checkbox(std::string("Play##").append(std::to_string(i)).c_str(), &ui._play);
						ImGui::Checkbox(std::string("Loop##").append(std::to_string(i)).c_str(), &ui._loop);
						ImGui::Checkbox(std::string("Reverse##").append(std::to_string(i)).c_str(), &ui._reverse);
						ImGui::Checkbox(std::string("Auto Play##").append(std::to_string(i)).c_str(), &ui._autoPlay);

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

						if (ImGui::Button(std::string("Test Animation##").append(std::to_string(i)).c_str()))
						{
							ui.PlayAnimation(ui._loop, ui._reverse);
						}

						//For debug
						if (ImGui::Button(std::string("Next Frame##").append(std::to_string(i)).c_str()))
						{
							ui._currentFrame++;

							if (ui._currentFrame >= ui._totalFrame)
							{
								ui._currentFrame = 0;
							}
						}
					}

					ImGui::ColorEdit4(std::string("Colour##Canvas").append(std::to_string(i)).c_str(), glm::value_ptr(ui._colour));

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					ImGui::Separator();
				}
			}

			if (ImGui::Button("Add UI"))
			{
				canvas->AddUI();

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			ImGui::SameLine();

			if (ImGui::Button("Sort"))
			{
				canvas->Sort();

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			ImGui::SameLine();

			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<GraphicsComponent>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<CanvasComponent>()->Write(), typeid(CanvasComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<CanvasComponent>()->Write() };
				canvas->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_CANVAS"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}

void InspectorWindow::AnimationComp(Entity& ent)
{
	AnimationComponent* anim = ent.getComponent<AnimationComponent>();
	if (anim != nullptr)
	{
		if (ImGui::CollapsingHeader("Animation component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##Animation", &anim->_isActive);

			ImGui::Text("Current Animation: ");
			auto it = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.begin();
			while (it != NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.end())
			{
				bool currAnim = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim == *it;
				if (ImGui::Selectable(it->c_str(), &currAnim))
				{
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim = it->c_str();
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_play = false;
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_dt = 0.0f;
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim = "";
				}
				++it;
			}

			ImGui::Separator();
			ImGui::Text("Default Animation: ");
			auto it2 = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.begin();
			while (it2 != NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.end())
			{
				bool defaultAnim = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim == *it2;
				if (ImGui::Selectable(std::string(it2->c_str()).append("##defaultAnim").c_str(), &defaultAnim))
				{
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim = it2->c_str();
				}
				++it2;
			}

			ImGui::InputFloat("Animation Speed##anim", &NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_animMultiplier);

			if (ImGui::Button("Preview Animation"))
			{
				anim->PlayAnimation(NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim,
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_loop);
			}

			if (NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_play)
			{
				if (ImGui::Button("Pause Animation"))
				{
					anim->PauseAnimation();
				}
			}
			else
			{
				if (ImGui::Button("Resume Animation"))
				{
					anim->ResumeAnimation();
				}
			}

			if (ImGui::Button("Stop Animation"))
			{
				anim->StopAnimation();
			}

			ImGui::Checkbox("Loop", &NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_loop);
		}

		ImGui::Separator();
	}
}

void InspectorWindow::EmitterComp(Entity& ent)
{
	EmitterComponent* emitter = ent.getComponent<EmitterComponent>();
	if (emitter != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<EmitterComponent>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Emitter component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##Emitter", &emitter->_isActive);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			std::string tex = emitter->_image;
			Emitter* emit = NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[emitter->_emitterID];

			const char* emitterType[TOTAL_SHAPE] = { "Sphere", "Cone" };
			const char* currentEmitter = (emit->_type >= 0 && emit->_type < TOTAL_SHAPE) ? emitterType[emit->_type] : "";
			ImGui::SliderInt("Type", (int*)&emit->_type, 0, TOTAL_SHAPE - 1, currentEmitter);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			_levelEditor->LE_AddInputText(std::string("Particle Image##"), tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&tex, &emitter]()
				{
					emitter->AddTexture(tex);
					emitter->_image = tex;
				});

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &tex, &emitter, &activeRead, &editedComp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "png" || fileType == "tga" || fileType == "dds")
					{
						//SOIL doesnt deal with preceding slash
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						tex = data;
						emitter->AddTexture(tex);
						emitter->_image = tex;

						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;
					}
				});
			ImGui::Checkbox("Black is Alpha##Emitter", &emitter->_blackIsAlpha);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("Duration Time##Emitter", &emit->_durationPerCycle);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (ImGui::InputScalar("Emission Rate##Emitter", ImGuiDataType_U32, &emit->_emissionOverTime))
			{
				emit->_emissionRate = 1.0f / emit->_emissionOverTime;
			}

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (ImGui::InputScalar("Max Particles##Emitter", ImGuiDataType_U32, &emit->_maxParticles))
			{
				emit->UpdateSize();
			}

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (emit->_type == SPHERE)
			{
				ImGui::InputFloat("Radius##Emitter", &emit->_radius);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}

			else if (emit->_type == CONE)
			{
				ImGui::InputFloat("Angle##Emitter", &emit->_spawnAngle);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ImGui::InputFloat("Radius##Emitter", &emit->_radius);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}

			if (ImGui::TreeNode("Particle Size##Emitter"))
			{
				ImGui::Checkbox("Random Size##Emitter", &emit->_randomizeSize);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				if (emit->_randomizeSize)
				{
					ImGui::InputFloat("Minimum Size##Emitter", &emit->_minParticleSize);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					ImGui::InputFloat("Maximum Size##Emitter", &emit->_maxParticleSize);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
				}
				else
				{
					ImGui::InputFloat("Size##Emitter", &emit->_maxParticleSize);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
				}
				ImGui::TreePop();
			}


			if (ImGui::TreeNode("Particle Speed##Emitter"))
			{
				ImGui::Checkbox("Random Speed##Emitter", &emit->_randomizeSpeed);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				if (emit->_randomizeSpeed)
				{
					ImGui::InputFloat("Minimum Speed##Emitter", &emit->_minParticleSpeed);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					ImGui::InputFloat("Maximum Speed##Emitter", &emit->_maxParticleSpeed);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				}
				else
				{
					ImGui::InputFloat("Speed##Emitter", &emit->_maxParticleSpeed);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Particle Lifespan##Emitter"))
			{
				ImGui::Checkbox("Random LifeSpan##Emitter", &emit->_randomizeLifespan);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				if (emit->_randomizeLifespan)
				{
					ImGui::InputFloat("Minimum LifeSpan##Emitter", &emit->_minLifespan);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					ImGui::InputFloat("Maximum LifeSpan##Emitter", &emit->_maxLifespan);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
				}
				else
				{
					ImGui::InputFloat("LifeSpan##Emitter", &emit->_maxLifespan);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
				}
				ImGui::TreePop();
			}

			if (!emit->_colourOverTime)
			{
				if (ImGui::TreeNode("Particle Colour##Emitter"))
				{
					ImGui::Checkbox("Random Colour##Emitter", &emit->_randomizeColour);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					if (emit->_randomizeColour)
					{
						float colourA[4] = { emit->_colourA.x, emit->_colourA.y, emit->_colourA.z, emit->_colourA.w };
						ImGui::ColorEdit4("Colour Range Low##Emitter", colourA);
						emit->_colourA = { colourA[0], colourA[1], colourA[2], colourA[3] };

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

						float colourB[4] = { emit->_colourB.x, emit->_colourB.y, emit->_colourB.z, emit->_colourB.w };
						ImGui::ColorEdit4("Colour Range High##Emitter", colourB);
						emit->_colourB = { colourB[0], colourB[1], colourB[2], colourB[3] };

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					else
					{
						float colour[4] = { emit->_colourB.x, emit->_colourB.y, emit->_colourB.z, emit->_colourB.w };
						ImGui::ColorEdit4("Colour##Emitter", colour);
						emit->_colourB = { colour[0], colour[1], colour[2], colour[3] };

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}
					ImGui::TreePop();
				}
			}

			ImGui::Checkbox("Animatable##Emitter", &emit->_isAnimated);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (emit->_isAnimated)
			{
				ImGui::Checkbox("Loop Animation##Emitter", &emit->_loopAnimation);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ImGui::InputScalar("Row##Emitter", ImGuiDataType_U32, &emit->_row);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ImGui::InputScalar("Column##Emitter", ImGuiDataType_U32, &emit->_column);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ImGui::InputScalar("Total Frame##Emitter", ImGuiDataType_U32, &emit->_totalFrame);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				if (ImGui::InputScalar("Frames per second##Emitter", ImGuiDataType_U32, &emit->_framesPerSecond))
				{
					if (emit->_framesPerSecond == 0)
					{
						emit->_animationRate = 0.0f;
					}
					else
					{
						emit->_animationRate = 1.0f / emit->_framesPerSecond;
					}
				}

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}

			ImGui::Checkbox("PreWarm##", &emit->_preWarm);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::Checkbox("Loop##", &emit->_loop);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::Checkbox("Burst##", &emit->_burst);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (emit->_burst)
			{
				ImGui::InputFloat("Burst Rate##Emitter", &emit->_burstRate);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ImGui::InputScalar("Burst Amount##Emitter", ImGuiDataType_U32, &emit->_burstAmount);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			}

			ImGui::Checkbox("Play##Particle", &emit->_play);

			ImGui::Checkbox("Reverse##Particle", &emit->_reverse);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (ImGui::Checkbox("Follow##Emitter", &emit->_follow))
			{
				if (emit->_play)
				{
					emitter->Stop();
					emitter->Play();
				}
			}

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::Checkbox("Fade##Emitter", &emit->_fade);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (ImGui::TreeNode("Special Behaviour##Emitter"))
			{
				ImGui::Checkbox("Velocity over time##Emitter", &emit->_velocityOverTime);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				if (emit->_velocityOverTime)
				{
					ImGui::InputFloat("X##Emitter", &emit->_velocity.x);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					ImGui::InputFloat("Y##Emitter", &emit->_velocity.y);

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				}

				ImGui::Checkbox("Size over time##Emitter", &emit->_sizeOverTime);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ImGui::Checkbox("Speed over time##Emitter", &emit->_speedOverTime);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ImGui::Checkbox("Colour over time##Emitter", &emit->_colourOverTime);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				if (emit->_colourOverTime)
				{
					float colourStart[4] = { emit->_colourStart.x, emit->_colourStart.y, emit->_colourStart.z, emit->_colourStart.w };
					ImGui::ColorEdit4("Colour Start##Emitter", colourStart);
					emit->_colourStart = { colourStart[0], colourStart[1], colourStart[2], colourStart[3] };

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

					float colourEnd[4] = { emit->_colourEnd.x, emit->_colourEnd.y, emit->_colourEnd.z, emit->_colourEnd.w };
					ImGui::ColorEdit4("Colour End##Emitter", colourEnd);
					emit->_colourEnd = { colourEnd[0], colourEnd[1], colourEnd[2], colourEnd[3] };

					// Undo-Redo for Components
					_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
					editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Preview Particle##Emitter"))
			{
				emitter->Play();
			}

			if (ImGui::Button("Stop Particle##Emitter"))
			{
				emitter->Stop();
			}

			if (ImGui::Button("Pause Particle##Emitter"))
			{
				emit->_pause = !emit->_pause;
			}

			//For debug
			if (ImGui::Button("Next Frame##"))
			{
				for (size_t index = 0; index < emit->_particles.size(); ++index)
				{
					emit->_particles[index]._currentFrame++;

					if (emit->_particles[index]._currentFrame >= emit->_totalFrame)
					{
						emit->_particles[index]._currentFrame = 0;
					}
				}
			}
		}
		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<EmitterComponent>()->Write(), typeid(EmitterComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<EmitterComponent>()->Write() };
				emitter->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_EMITTER"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}

void InspectorWindow::ListenerComp(Entity& ent)
{
	ListenerComponent* lisComp = ent.getComponent<ListenerComponent>();
	if (lisComp == nullptr)
		return;

	// Undo-Redo for Components
	ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ListenerComponent>()->Write() };
	bool editedComp = false;

	if (ImGui::CollapsingHeader("Listener component", &_notRemove))
		ImGui::Checkbox("IsActive##Listener", &lisComp->_isActive);
	if (!_notRemove)
	{
		ENTITY_COMP_DOC comp{ ent, ent.getComponent<ListenerComponent>()->Write(), typeid(ListenerComponent).hash_code() };
		_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
		_notRemove = true;
	}

	if (editedComp)
	{
		if (_origComp._entID != -1)
		{
			// Undo-Redo for Components
			//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
			//bool editedComp = false;

			// Undo-Redo for Components
			//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ENTITY_COMP_READ finalComp{ ent, ent.getComponent<ListenerComponent>()->Write() };
			lisComp->Read(*_origComp._rjDoc);
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_CAMERA"), std::any(finalComp));
			_origComp = ENTITY_COMP_READ{};
		}
	}

	ImGui::Separator();
	/*if (camComp != nullptr)
	{
		if (ImGui::CollapsingHeader("Animation component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##Animation", &anim->_isActive);

			ImGui::Text("Current Animation: ");
			auto it = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.begin();
			while (it != NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.end())
			{
				bool currAnim = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim == *it;
				if (ImGui::Selectable(it->c_str(), &currAnim))
				{
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim = it->c_str();
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_play = false;
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_dt = 0.0f;
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim = "";
				}
				++it;
			}

			ImGui::Separator();
			ImGui::Text("Default Animation: ");
			auto it2 = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.begin();
			while (it2 != NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.end())
			{
				bool defaultAnim = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim == *it2;
				if (ImGui::Selectable(std::string(it2->c_str()).append("##defaultAnim").c_str(), &defaultAnim))
				{
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim = it2->c_str();
				}
				++it2;
			}

			ImGui::InputFloat("Animation Speed##anim", &NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_animMultiplier);

			if (ImGui::Button("Preview Animation"))
			{
				anim->PlayAnimation(NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim,
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_loop);
			}

			if (NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_play)
			{
				if (ImGui::Button("Pause Animation"))
				{
					anim->PauseAnimation();
				}
			}
			else
			{
				if (ImGui::Button("Resume Animation"))
				{
					anim->ResumeAnimation();
				}
			}

			if (ImGui::Button("Stop Animation"))
			{
				anim->StopAnimation();
			}

			ImGui::Checkbox("Loop", &NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_loop);
		}

		ImGui::Separator();
	}*/
}
void InspectorWindow::CameraComp(Entity& ent)
{
	CameraComponent* camComp = ent.getComponent<CameraComponent>();
	if (camComp == nullptr)
		return;

	// Undo-Redo for Components
	ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<CameraComponent>()->Write() };
	bool editedComp = false;

	NS_GRAPHICS::Camera& cam = camComp->_data;
	if (ImGui::CollapsingHeader("Camera component", &_notRemove))
	{
		ImGui::Checkbox("IsActive##Camera", &camComp->_isActive);

		// Undo-Redo for Components
		_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
		editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

		ImGui::InputFloat("FOV", &(cam.cameraFOV));

		// Undo-Redo for Components
		_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
		editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

		if (ImGui::CollapsingHeader("Sensitivity", &_notRemove))
		{
			ImGui::InputFloat("Rotate", &(cam._rotation_sensitivity));

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("Drag", &(cam._drag_sensitivity));

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("Zoom", &(cam._zoom_sensitivity));

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::NewLine();
		}
		ImGui::InputFloat("Yaw", &(cam.cameraYaw));

		// Undo-Redo for Components
		_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
		editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

		ImGui::InputFloat("Pitch", &(cam.cameraPitch));

		// Undo-Redo for Components
		_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
		editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

		//ImGui::InputFloat3("Target", glm::value_ptr(cam.cameraTarget), 3);
		//ImGui::InputFloat3("Position", glm::value_ptr(cam.cameraPos), 3);
		//ImGui::InputFloat3("Position", glm::value_ptr(cam.cameraPos), 3);
	}
	if (!_notRemove)
	{
		ENTITY_COMP_DOC comp{ ent, ent.getComponent<CameraComponent>()->Write(), typeid(CameraComponent).hash_code() };
		_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
		_notRemove = true;
	}

	if (editedComp)
	{
		if (_origComp._entID != -1)
		{
			// Undo-Redo for Components
			//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
			//bool editedComp = false;

			// Undo-Redo for Components
			//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ENTITY_COMP_READ finalComp{ ent, ent.getComponent<CameraComponent>()->Write() };
			camComp->Read(*_origComp._rjDoc);
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_CAMERA"), std::any(finalComp));
			_origComp = ENTITY_COMP_READ{};
		}
	}

	ImGui::Separator();
	/*if (camComp != nullptr)
	{
		if (ImGui::CollapsingHeader("Animation component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##Animation", &anim->_isActive);

			ImGui::Text("Current Animation: ");
			auto it = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.begin();
			while (it != NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.end())
			{
				bool currAnim = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim == *it;
				if (ImGui::Selectable(it->c_str(), &currAnim))
				{
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim = it->c_str();
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_play = false;
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_dt = 0.0f;
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim = "";
				}
				++it;
			}

			ImGui::Separator();
			ImGui::Text("Default Animation: ");
			auto it2 = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.begin();
			while (it2 != NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.end())
			{
				bool defaultAnim = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim == *it2;
				if (ImGui::Selectable(std::string(it2->c_str()).append("##defaultAnim").c_str(), &defaultAnim))
				{
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_defaultAnim = it2->c_str();
				}
				++it2;
			}

			ImGui::InputFloat("Animation Speed##anim", &NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_animMultiplier);

			if (ImGui::Button("Preview Animation"))
			{
				anim->PlayAnimation(NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim,
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_loop);
			}

			if (NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_play)
			{
				if (ImGui::Button("Pause Animation"))
				{
					anim->PauseAnimation();
				}
			}
			else
			{
				if (ImGui::Button("Resume Animation"))
				{
					anim->ResumeAnimation();
				}
			}

			if (ImGui::Button("Stop Animation"))
			{
				anim->StopAnimation();
			}

			ImGui::Checkbox("Loop", &NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_loop);
		}

		ImGui::Separator();
	}*/
}

void InspectorWindow::CScriptComp(Entity& ent)
{
	ent;
	//CScriptComponent* cScript_comp = ent.getComponent<CScriptComponent>();
	//if (cScript_comp != nullptr)
	//{
	//  if (ImGui::CollapsingHeader("CScript component", &_notRemove))
	//  {
	//    ImGui::Checkbox("IsActive##CScript", &cScript_comp->_isActive);
	//    ImGui::InputInt("Tag", &cScript_comp->_iTag);
	//    std::string tex = cScript_comp->_sName.toString();
	//    std::string old = tex;
	//    _levelEditor->LE_AddInputText("Script Name", tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
	//      [&tex, &cScript_comp]()
	//      {
	//        cScript_comp->_sName = tex;
	//      });
	//    // Changes occured
	//    if (tex != old)
	//    {
	//      if (cScript_comp->_pScript) // Already has a script
	//      {
	//        delete cScript_comp->_pScript;
	//        cScript_comp->_pScript = nullptr;
	//      }
	//      cScript_comp->_pScript = AllScripts::MyConstruct(tex);
	//      if (cScript_comp->_pScript)
	//      {
	//        std::cout << "Constructed Script" << std::endl;
	//        cScript_comp->_pScript->SetEntity(ent);
	//      }
	//    }
	//  }

	//  if (!_notRemove)
	//  {
	//    //ent.RemoveComponent<GraphicsComponent>();
	//    ENTITY_COMP_DOC comp{ ent, ent.getComponent<CScriptComponent>()->Write(), typeid(CScriptComponent).hash_code() };
	//    _levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
	//    _notRemove = true;
	//  }

	//  ImGui::Separator();
	//}
}

void InspectorWindow::PlayerStatsComp(Entity& ent)
{
	PlayerStatsComponent* psc = ent.getComponent<PlayerStatsComponent>();
	if (psc != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<PlayerStatsComponent>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Player Stats", &_notRemove))
		{
			//_levelEditor->LE_AddInputText("##GRAPHICS_1", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::InputFloat("player_move_mag", &psc->player_move_mag);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("player_fly_mag", &psc->player_fly_mag);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputInt("player_max_energy", &psc->player_max_energy);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputInt("player_possess_energy_drain", &psc->player_possess_energy_drain);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputInt("player_moth_energy_drain", &psc->player_moth_energy_drain);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("camera_distance", &psc->camera_distance);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat3("camera_offset", psc->camera_offset.m);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("player_max_speed", &psc->player_max_speed);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<PlayerStatsComponent>()->Write(), typeid(PlayerStatsComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<PlayerStatsComponent>()->Write() };
				psc->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_PLAYER"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}

void InspectorWindow::CauldronStatsComp(Entity& ent)
{
	CauldronStatsComponent* csc = ent.getComponent<CauldronStatsComponent>();
	if (csc != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<CauldronStatsComponent>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Cauldron Stats", &_notRemove))
		{
			std::string talis = csc->talisman.toString();
			_levelEditor->LE_AddInputText("talisman", talis, 256, ImGuiInputTextFlags_EnterReturnsTrue,
				[&talis, &csc]()
				{
					csc->talisman = talis;
				});

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;


			std::string col = csc->collider.toString();
			_levelEditor->LE_AddInputText("collider", col, 256, ImGuiInputTextFlags_EnterReturnsTrue,
				[&col, &csc]()
				{
					csc->collider = col;
				});

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat("magnitude", &csc->magnitude);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			ImGui::InputFloat3("direction", csc->direction.m);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<CauldronStatsComponent>()->Write(), typeid(CauldronStatsComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<CauldronStatsComponent>()->Write() };
				csc->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_CAULDRON"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

		ImGui::Separator();
	}
}

void InspectorWindow::VariableComp(Entity& ent)
{
	ComponentVariables* comp_var = ent.getComponent<ComponentVariables>();
	if (comp_var != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentVariables>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Variable component", &_notRemove))
		{
			if (ImGui::Button("Add Float"))
			{
				float fl = 0.0f;
				comp_var->float_list.push_back(fl);

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Float"))
			{
				comp_var->float_list.pop_back();

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			int float_index = 1;
			for (float& f : comp_var->float_list) //[path, name]
			{
				std::string p = "Float_" + std::to_string(float_index);

				_levelEditor->LE_AddInputFloatProperty(p, f, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
				float_index++;

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}

			if (ImGui::Button("Add Interger"))
			{
				int interger = 0;
				comp_var->int_list.push_back(interger);

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Interger"))
			{
				comp_var->int_list.pop_back();

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}
			int int_index = 1;

			for (int& i : comp_var->int_list) //[path, name]
			{
				std::string p = "Int_" + std::to_string(int_index);

				std::string s_name = std::to_string(int_index);
				_levelEditor->LE_AddInputIntProperty(p, i, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
				int_index++;

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}

			if (ImGui::Button("Add String"))
			{
				LocalString ls;
				comp_var->string_list.push_back(ls);

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove String"))
			{
				comp_var->string_list.pop_back();

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			int str_index = 1;

			for (LocalString<DEF_STR_SIZE>& str : comp_var->string_list) //[path, name]
			{
				std::string p = "String_" + std::to_string(str_index);

				std::string s_name = str;
				_levelEditor->LE_AddInputText(p, s_name, 100, ImGuiInputTextFlags_EnterReturnsTrue,
					[&str, &s_name]()
					{
						str = s_name.c_str();
					});

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
					[this, &str, &activeRead, &editedComp](std::string* _str)
					{
						str = *_str;

						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;
					});

				_levelEditor->LE_AddDragDropTarget<Entity>("HIERARCHY_ENTITY_OBJECT",
					[this, &str, &activeRead, &editedComp](Entity* entptr)
					{
						str = G_ECMANAGER->EntityName[entptr->getId()];

						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;

					});
				str_index++;
			}

		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<ComponentLoadAudio>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ComponentVariables>()->Write(), typeid(ComponentVariables).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<ComponentVariables>()->Write() };
				comp_var->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_VARIABLE"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}
	}

	ImGui::Separator();
}

void InspectorWindow::NavComp(Entity& ent)
{
	NavComponent* nav_comp = ent.getComponent<NavComponent>();
	if (nav_comp != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<NavComponent>()->Write() };
		bool editedComp = false;


		if (ImGui::CollapsingHeader("Navigator", &_notRemove))
		{
			_levelEditor->LE_AddInputFloatProperty("Speed", nav_comp->speed, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (_levelEditor->LE_AddCombo("Way Point Nav Type", (int&)nav_comp->wp_nav_type,
				{
					"To and fro",
					"circular",
					"random"
				}))
			{
				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}
			//_levelEditor->LE_AddInputFloatProperty("Radius for detection", nav_comp->rad_for_detect, []() {},  ImGuiInputTextFlags_EnterReturnsTrue);
			_levelEditor->LE_AddCheckbox("Stop at each waypoint", &nav_comp->stopAtEachWayPoint);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (nav_comp->stopAtEachWayPoint)
			{
				_levelEditor->LE_AddInputFloatProperty("End time", nav_comp->endTime, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
			}

			_levelEditor->LE_AddCheckbox("Pause at start", &nav_comp->isPaused);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			//Way point path to look at
			std::string s_name;

			if (nav_comp->cur_wp_path != nullptr)
				s_name = G_ECMANAGER->EntityName[G_ECMANAGER->getEntity(nav_comp->cur_wp_path).getId()];
			else
				s_name = nav_comp->wp_path_ent_name;

			_levelEditor->LE_AddInputText("WayPointPath", s_name, 100, ImGuiInputTextFlags_EnterReturnsTrue,
				[&s_name, &nav_comp]()
				{
					//str = s_name.c_str();
					Entity ent = G_ECMANAGER->getEntityUsingEntName(s_name);

					if (ent.getId() != -1)
						nav_comp->cur_wp_path = ent.getComponent<WayPointMapComponent>();
					else
					{
						nav_comp->cur_wp_path = nullptr;
						nav_comp->wp_path_ent_name = "";
					}
				});

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			_levelEditor->LE_AddDragDropTarget<Entity>("HIERARCHY_ENTITY_OBJECT",
				[this, &s_name, &nav_comp, &activeRead, &editedComp](Entity* entptr)
				{
					nav_comp->cur_wp_path = entptr->getComponent<WayPointMapComponent>();
					if (nav_comp->cur_wp_path != nullptr) {
						s_name = G_ECMANAGER->EntityName[entptr->getId()];
						nav_comp->wp_path_ent_name = s_name;
					}
					else
					{
						nav_comp->cur_wp_path = nullptr;
						nav_comp->wp_path_ent_name = "";
					}

					// Undo-Redo for Components
					_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
					editedComp = true;

				});

			if (_levelEditor->LE_AddCombo("Starting Nav State", (int&)nav_comp->nav_state,
				{
					"Patrol",
					"Circling around first way point"
				}))
			{
				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			_levelEditor->LE_AddInputFloatProperty("Circling Radius", nav_comp->circuling_rad, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

			if (nav_comp->cur_wp_path != nullptr)
			{
				if (_levelEditor->LE_AddCombo("WayPoints to navigate", (int&)nav_comp->wp_creation_type,
					{
						"Standard (1->N)",
						"Reverse (N->1)",
						"Custom"
					}))
				{
					// Undo-Redo for Components
					_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
					editedComp = true;
				}

				if (nav_comp->wp_creation_type == WPP_CUSTOM)
				{
					if (ImGui::Button("Add WayPoint Index"))
					{
						nav_comp->path_indexes.push_back(std::make_pair(0, true));

						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;
					}
					ImGui::SameLine();
					if (ImGui::Button("Remove WayPoint Index"))
					{
						nav_comp->path_indexes.pop_back();

						// Undo-Redo for Components
						_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
						editedComp = true;
					}

					int wp_index = 1;
					for (auto& i : nav_comp->path_indexes) //[path, name]
					{
						std::string p = "WayPoint_" + std::to_string(wp_index);
						_levelEditor->LE_AddInputIntProperty(p, i.first, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
						wp_index++;

						// Undo-Redo for Components
						_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
						editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
					}

				}
			}
		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<ComponentLoadAudio>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<NavComponent>()->Write(), typeid(NavComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<NavComponent>()->Write() };
				nav_comp->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_NAVIGATOR"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}

	}
}

void InspectorWindow::WayPointPathComp(Entity& ent)
{
	WayPointMapComponent* wpm_comp = ent.getComponent<WayPointMapComponent>();
	if (wpm_comp != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<WayPointMapComponent>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Way Point Path", &_notRemove))
		{
			if (ImGui::Button("Add WayPoint"))
			{
				LocalString ls;
				wpm_comp->way_point_list.push_back(ls);
				wpm_comp->GetPath().push_back(nullptr);

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove WayPoint"))
			{
				wpm_comp->way_point_list.pop_back();
				wpm_comp->GetPath().pop_back();

				// Undo-Redo for Components
				_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
				editedComp = true;
			}

			if (ImGui::Button("Update WayPoint"))
			{
				NS_AI::SYS_AI->BakeEdge();
			}
			int str_index = 1;

			for (LocalString<DEF_STR_SIZE>& str : wpm_comp->way_point_list) //[path, name]
			{
				std::string p = "Waypoint_" + std::to_string(str_index);

				std::string s_name = str;
				_levelEditor->LE_AddInputText(p, s_name, 100, ImGuiInputTextFlags_EnterReturnsTrue,
					[&str, &s_name, &wpm_comp, &str_index]()
					{
						Entity ent = G_ECMANAGER->getEntityUsingEntName(s_name);
						if (ent.getId() == -1)
						{
							str = "";
							TracyMessageL("InspectorWindow::WayPointPathComp: No entity has been found");
							SPEEDLOG("InspectorWindow::WayPointPathComp: No entity has been found");
							//std::cout << "No entity has been found" << std::endl;
							return;
						}

						WayPointComponent* wpc = ent.getComponent<WayPointComponent>();
						if (wpc != nullptr) {
							wpm_comp->GetPath().at(str_index - 1) = wpc;
							str = s_name.c_str();
						}
						else
						{
							TracyMessageL("InspectorWindow::WayPointPathComp: No entity with waypoint component found");
							SPEEDLOG("InspectorWindow::WayPointPathComp: No entity with waypoint component found");
							//std::cout << "No entity with waypoint component found" << std::endl;
						}
					});

				// Undo-Redo for Components
				_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				_levelEditor->LE_AddDragDropTarget<Entity>("HIERARCHY_ENTITY_OBJECT",
					[this, &str, &wpm_comp, &str_index, &activeRead, &editedComp](Entity* entptr)
					{
						WayPointComponent* wpc = entptr->getComponent<WayPointComponent>();
						if (wpc != nullptr)
						{
							wpm_comp->GetPath().at(str_index - 1) = wpc;
							str = G_ECMANAGER->EntityName[entptr->getId()];

							// Undo-Redo for Components
							_origComp = (_origComp._entID == -1) ? activeRead : _origComp;
							editedComp = true;
						}

					});

				str_index++;
			}


		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<ComponentLoadAudio>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<WayPointMapComponent>()->Write(), typeid(WayPointMapComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<WayPointMapComponent>()->Write() };
				wpm_comp->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_WAYPOINTMAP"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}
	}
}

void InspectorWindow::WayPointComp(Entity& ent)
{
	WayPointComponent* wp_comp = ent.getComponent<WayPointComponent>();
	if (wp_comp != nullptr)
	{
		// Undo-Redo for Components
		ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<WayPointComponent>()->Write() };
		bool editedComp = false;

		if (ImGui::CollapsingHeader("Way-Point Component", &_notRemove))
		{
			_levelEditor->LE_AddInputFloatProperty("Self-Define Cost", wp_comp->self_define_var.second, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);

			// Undo-Redo for Components
			_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
			editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;
		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<ComponentLoadAudio>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<WayPointComponent>()->Write(), typeid(WayPointComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		if (editedComp)
		{
			if (_origComp._entID != -1)
			{
				// Undo-Redo for Components
				//ENTITY_COMP_READ activeRead = ENTITY_COMP_READ{ ent, ent.getComponent<ComponentCollider>()->Write() };
				//bool editedComp = false;

				// Undo-Redo for Components
				//_origComp = (ImGui::IsItemActivated() && _origComp._entID == -1) ? activeRead : _origComp;
				//editedComp = !editedComp ? ImGui::IsItemDeactivatedAfterEdit() : true;

				ENTITY_COMP_READ finalComp{ ent, ent.getComponent<WayPointComponent>()->Write() };
				wp_comp->Read(*_origComp._rjDoc);
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_PASTE_COMP_WAYPOINT"), std::any(finalComp));
				_origComp = ENTITY_COMP_READ{};
			}
		}
	}
}



void InspectorWindow::AddSelectedComps(Entity& ent)
{
	_levelEditor->LE_AddCombo("##AddComponentsCombo", _itemType,
		{
			"Add component",
			"  RigidBody",
			"  Audio",
			"  Graphics",
			"  Light   ",
			"  Collider",
			"  C#Script",
			"  Canvas",
			"  PlayerStats",
			"  CauldronStats",
			"  VariablesComp",
			"  NavComp",
			"  WayPointPath",
			"  WayPointComp",
			"  Emitter",
			"  Camera",
			"  Listener"
		});

	//ImGui::Combo(" ", &item_type, "Add component\0  RigidBody\0  Audio\0  Graphics\0--Collider--\0  AABB Colider\0  OBB Collider\0  Plane Collider\0  SphereCollider\0  CapsuleCollider\0");

	//void* next_lol = nullptr;

	if (ImGui::Button("Add Selected Component"))
	{
		switch (_itemType)
		{
		case 1:
		{
			//next_lol = ent.AddComponent<RigidBody>()
			if (!ent.getComponent<RigidBody>())
			{
				// ENTITY_COMP comp{ ent, &rg, typeid(RigidBody).hash_code() };
				ENTITY_COMP_DOC comp{ ent, RigidBody().Write(),typeid(RigidBody).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 2:
		{
			if (!ent.getComponent<ComponentLoadAudio>())
			{
				//next_lol = ent.AddComponent<ComponentLoadAudio>();
				ENTITY_COMP_DOC comp{ ent, ComponentLoadAudio().Write(), typeid(ComponentLoadAudio).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 3:
		{
			//next_lol = ent.AddComponent<GraphicsComponent>();
			if (!ent.getComponent<GraphicsComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, GraphicsComponent().Write(), typeid(GraphicsComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}

		case 4:
		{
			//next_lol = ent.AddComponent<GraphicsComponent>();
			if (!ent.getComponent<LightComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, LightComponent().Write(), typeid(LightComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		/*
		case 5:
		{
			next_lol = ent.AddComponent<ColliderComponent>();
			break;
		}*/

		case 5:
		{
			if (!ent.getComponent<ColliderComponent>())
			{
				//ent.AttachComponent(aabb);
				ENTITY_COMP_DOC comp{ ent, ColliderComponent().Write(), typeid(ColliderComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}

		//case 11: -> ------
		/*
		case 6: // CScript
		{
			if (!ent.getComponent<CScriptComponent>())
			{
				// Currently not using Run Command as it will crash when it tries to read Scripts
				ent.AddComponent<CScriptComponent>();
				ENTITY_COMP_DOC comp{ ent, CScriptComponent().Write(),typeid(CScriptComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));

			}
			break;
		}*/
		case 6: // C#Script
		{
			if (!ent.getComponent<ScriptComponent>())
			{
				// Currently not using Run Command as it will crash when it tries to read Scripts
				ent.AddComponent<ScriptComponent>();
				ENTITY_COMP_DOC comp{ ent, ScriptComponent().Write(),typeid(ScriptComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 7: // Canvas
		{
			if (!ent.getComponent<CanvasComponent>())
			{
				// Currently not using Run Command as it will crash when it tries to read Scripts
				ent.AddComponent<CanvasComponent>();
				ENTITY_COMP_DOC comp{ ent, CanvasComponent().Write(),typeid(CanvasComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 8: // PlayerStats
		{
			if (!ent.getComponent<PlayerStatsComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, PlayerStatsComponent().Write(), typeid(PlayerStatsComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 9: // PlayerStats
		{
			if (!ent.getComponent<CauldronStatsComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, CauldronStatsComponent().Write(), typeid(CauldronStatsComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 10: // ComponentVariable
		{
			if (!ent.getComponent<ComponentVariables>())
			{
				ENTITY_COMP_DOC comp{ ent, ComponentVariables().Write(), typeid(ComponentVariables).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 11: // ComponentNav
		{
			if (!ent.getComponent<NavComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, NavComponent().Write(), typeid(NavComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 12: // WayPointPath
		{
			if (!ent.getComponent<WayPointMapComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, WayPointMapComponent().Write(), typeid(WayPointMapComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 13: // WayPoint
		{
			if (!ent.getComponent<WayPointComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, WayPointComponent().Write(), typeid(WayPointComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}

		case 14: // Emitter
		{
			if (!ent.getComponent<EmitterComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, EmitterComponent().Write(), typeid(EmitterComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 15: // Camera
		{
			if (!ent.getComponent<CameraComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, CameraComponent().Write(), typeid(CameraComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 16: // Listener
		{
			if (!ent.getComponent<ListenerComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, ListenerComponent().Write(), typeid(ListenerComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}

		}
		//if (next_lol == nullptr)
		//{
		//	std::cout << "Component has already been created" << std::endl;
		//}
	}
}

bool InspectorWindow::EditTransform(const float* cameraView, float* cameraProjection, glm::mat4& matrix)
{
	if (ImGui::RadioButton("Translate", _mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		_mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", _mCurrentGizmoOperation == ImGuizmo::ROTATE))
		_mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", _mCurrentGizmoOperation == ImGuizmo::SCALE))
		_mCurrentGizmoOperation = ImGuizmo::SCALE;

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matrix), matrixTranslation, matrixRotation, matrixScale);

	bool activated = false;
	bool deactivated = false;

	if (ImGui::InputFloat3("Translation##TRANSLATION", matrixTranslation, 3))
	{
		//_lastEnter = true;
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		deactivated = true;
	}
	else if (ImGui::IsItemActivated())
	{
		activated = true;
	}

	if (ImGui::InputFloat3("Rotation##ROTATION", matrixRotation, 3))
	{
		//_lastEnter = true;
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		deactivated = true;
	}
	else if (ImGui::IsItemActivated())
	{
		activated = true;
	}

	if (ImGui::InputFloat3("Scale##SCALE", matrixScale, 3))
	{
		//_lastEnter = true;
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		deactivated = true;
	}
	else if (ImGui::IsItemActivated())
	{
		activated = true;
	}

	if (deactivated)
	{
		_lastEnter = true;
	}
	else if (activated)
	{
		_transformItemActive = matrix;
	}

	//ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	// Manually calculate matrix based on values
	glm::mat4 Translate = glm::translate(glm::mat4(1.f), glm::make_vec3(matrixTranslation));

	// Perform rotation using Quarternions
	glm::quat Quaternion(glm::radians(glm::make_vec3(matrixRotation)));
	glm::mat4 Rotate = glm::mat4_cast(Quaternion);

	glm::mat4 Scale = glm::scale(glm::mat4(1.f), glm::make_vec3(matrixScale));

	matrix = Translate * Rotate * Scale;

	if (_mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("World", _mCurrentGizmoMode == ImGuizmo::WORLD))
			_mCurrentGizmoMode = ImGuizmo::WORLD;
		ImGui::SameLine();
		if (ImGui::RadioButton("Local", _mCurrentGizmoMode == ImGuizmo::LOCAL))
			_mCurrentGizmoMode = ImGuizmo::LOCAL;
	}

	ImGui::Checkbox("##USESNAP", &_useSnap);
	ImGui::SameLine();

	float* snapPtr = nullptr;

	switch (_mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		snapPtr = _snapTrans;
		ImGui::InputFloat3("Snap", &_snapTrans[0]);
		break;
	case ImGuizmo::ROTATE:
		snapPtr = &_snapRotate;
		ImGui::InputFloat("Angle Snap", &_snapRotate);
		break;
	case ImGuizmo::SCALE:
		snapPtr = &_snapScale;
		ImGui::InputFloat("Scale Snap", &_snapScale);
		break;
	}


	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	return ImGuizmo::Manipulate(cameraView, cameraProjection, _mCurrentGizmoOperation, _mCurrentGizmoMode, glm::value_ptr(matrix), NULL, _useSnap ? snapPtr : NULL, NULL, NULL);
}

void InspectorWindow::TransformSnap(Entity& ent)
{
	_levelEditor->LE_AddChildWindow("##SnapChildWindow", ImVec2(0, 210),
		[this, &ent]()
		{
			ImGui::Checkbox("Snap X direction##SNAPX", &_isSnapX);
			ImGui::Checkbox("Snap Y direction##SNAPY", &_isSnapY);
			ImGui::Checkbox("Snap Z direction##SNAPZ", &_isSnapZ);
			ImGui::Checkbox("Snap downwards##SNAPYD", &_isSnapYD);

			if (ImGui::InputFloat("Snap Distance##SNAPDIST", &_snapDist, 3))
			{
			}
			if (ImGui::InputFloat("GAP##SNAPGAP", &_snapGap, 0.10f))
			{
			}
			if (ImGui::InputInt("LOD##SNAPLOD", &_snapLod, 1))
			{
			}

			if (ImGui::Button("Do Snap##"))
			{
				int directions = 0;

#define SNAP_X 1
#define SNAP_Y 2
#define SNAP_Z 4
#define SNAP_D 8

				if (_isSnapX)
				{
					directions = directions | SNAP_X;
				}
				if (_isSnapY)
				{
					directions = directions | SNAP_Y;
				}
				if (_isSnapZ)
				{
					directions = directions | SNAP_Z;
				}
				if (_isSnapYD)
				{
					directions = directions | SNAP_D;
				}

				NS_COLLISION::SYS_COLLISION->Snap_AABB_AABB_Do(ent, _snapDist, _snapGap, directions, _snapLod, true);
			}

		}, true);

}

void InspectorWindow::TransformGizmo(TransformComponent* trans_comp)
{
	_levelEditor->LE_AddChildWindow("##TransformChildWindow", ImVec2(0, 150),
		[this, &trans_comp]()
		{
			POINT windowSize = SYS_INPUT->GetSystemMousePos().GetClientRectSize();
			NS_GRAPHICS::CameraSystem& cm = NS_GRAPHICS::CameraSystem::GetInstance();
			glm::mat4 cmMat = cm.GetViewMatrix();
			float* camView = glm::value_ptr(cmMat);
			// Matches the most closely to the actual camera
			// If gizmos don't match, change this?
			_fov = NS_GRAPHICS::CameraSystem::GetInstance().GetFOV();

			//Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 1.0f, 1000.f, cameraProjection);
			float* cameraProjection;
			if (windowSize.x && windowSize.y)
			{
				glm::mat4 persp = glm::perspective(glm::radians(_fov), (float)windowSize.x / (float)windowSize.y, 1.0f, 1000.0f);
				cameraProjection = glm::value_ptr(persp);
			}
			else
			{
				glm::mat4 persp = glm::perspective(glm::radians(_fov), 1280.0f / 720.0f, 1.0f, 1000.0f);
				cameraProjection = glm::value_ptr(persp);
			}

			glm::mat4 matObj = trans_comp->GetModelMatrix();
			ImGuizmo::SetID(0);
			if (EditTransform(camView, cameraProjection, matObj))
			{
				// Checks FIRST frame of manipulation only
				if (!_lastPos_Start)
				{
					_lastPos_Start = true;
					_lastPos_ELP = { trans_comp, trans_comp->GetModelMatrix() };

					_allOtherLastPos_ELP.clear();

					std::map<int, bool>& SelectedEntities = LE_ECHELPER->SelectedEntities();

					for (std::map<int, bool>::iterator iter = SelectedEntities.begin(); iter != SelectedEntities.end(); ++iter)
					{
						if (iter->second && iter->first != trans_comp->objId)
						{
							TransformComponent* otherSelects = G_ECMANAGER->getEntity(iter->first).getComponent<TransformComponent>();
							ENTITY_LAST_POS otherPos = { otherSelects, otherSelects->GetModelMatrix() };
							_allOtherLastPos_ELP.push_back(otherPos);
						}
					}
				}

				// Sets object to new position
				float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matObj), trans, rot, scale);

				trans_comp->_position = glm::make_vec3(trans);
				trans_comp->_rotation = glm::make_vec3(rot);
				trans_comp->_scale = glm::make_vec3(scale);

				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_lastPos_ELP._newPos), trans, rot, scale);

				// Gets the movement vector from original
				glm::vec3 moveVector = trans_comp->_position - glm::make_vec3(trans);
				glm::vec3 rotateVector = trans_comp->_rotation - glm::make_vec3(rot);
				glm::vec3 scaleVector = trans_comp->_scale - glm::make_vec3(scale);

				// Sets all objects back to original and moves it based on difference from current to original locations
				for (int i = 0; i < _allOtherLastPos_ELP.size(); ++i)
				{
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_allOtherLastPos_ELP[i]._newPos), trans, rot, scale);

					_allOtherLastPos_ELP[i]._transComp->_position = glm::make_vec3(trans);
					_allOtherLastPos_ELP[i]._transComp->_rotation = glm::make_vec3(rot);
					_allOtherLastPos_ELP[i]._transComp->_scale = glm::make_vec3(scale);

					_allOtherLastPos_ELP[i]._transComp->_position += moveVector;
					_allOtherLastPos_ELP[i]._transComp->_rotation += rotateVector;
					_allOtherLastPos_ELP[i]._transComp->_scale += scaleVector;
				}
			}
			else
			{
				// Checks if not manipulating and mouse is let go
				if (_lastPos_Start && !SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IMOUSE_LBUTTON))
				{
					_lastPos_Start = false;
					// New position for the object
					ENTITY_LAST_POS newObj{ trans_comp , matObj };

					// Reset object back to original position
					float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_lastPos_ELP._newPos), trans, rot, scale);
					trans_comp->_position = glm::make_vec3(trans);
					trans_comp->_rotation = glm::make_vec3(rot);
					trans_comp->_scale = glm::make_vec3(scale);

					// Adds to vector
					std::vector<ENTITY_LAST_POS> multiplePosObjs;
					multiplePosObjs.push_back(newObj);

					// Checks if there are other objects to move as well
					for (int i = 0; i < _allOtherLastPos_ELP.size(); ++i)
					{
						ENTITY_LAST_POS otherObj{ _allOtherLastPos_ELP[i]._transComp , _allOtherLastPos_ELP[i]._transComp->GetModelMatrix() };

						ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_allOtherLastPos_ELP[i]._newPos), trans, rot, scale);

						_allOtherLastPos_ELP[i]._transComp->_position = glm::make_vec3(trans);
						_allOtherLastPos_ELP[i]._transComp->_rotation = glm::make_vec3(rot);
						_allOtherLastPos_ELP[i]._transComp->_scale = glm::make_vec3(scale);

						multiplePosObjs.push_back(otherObj);
						// _levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_SET_ENTITY_POSITION"), otherPos);
					}

					// If multiple objects to move, run all
					if (multiplePosObjs.size() > 1)
					{
						std::any curMultiplePos = multiplePosObjs;
						_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_SET_MULTIPLE_ENTITY_POSITION"), curMultiplePos);
					}
					// Run only single object otherwise
					else
					{
						std::any curPos = newObj;
						// Runs command to move object to new position from old position
						_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_SET_ENTITY_POSITION"), curPos);
					}

				}
				else if (_lastEnter)
				{
					_lastEnter = false;
					ENTITY_LAST_POS newObj{ trans_comp , matObj };
					std::any curPos = newObj;

					float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_transformItemActive), trans, rot, scale);
					trans_comp->_position = glm::make_vec3(trans);
					trans_comp->_rotation = glm::make_vec3(rot);
					trans_comp->_scale = glm::make_vec3(scale);

					// Runs command to move object to new position from old position
					_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_SET_ENTITY_POSITION"), curPos);

					_transformItemActive = matObj;
				}
				else
				{
					// Sets object to new position
					float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matObj), trans, rot, scale);

					trans_comp->_position = glm::make_vec3(trans);
					trans_comp->_rotation = glm::make_vec3(rot);
					trans_comp->_scale = glm::make_vec3(scale);
				}
			}
		}, true);
}
