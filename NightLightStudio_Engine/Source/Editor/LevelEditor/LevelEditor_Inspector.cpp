#include "LevelEditor_Inspector.h"

#include "LevelEditor_ECHelper.h"
#include "../../Core/SceneManager.h"
#include "../../Graphics/GraphicsSystem.h"

#include <set>
#include "LevelEditor_Console.h"
#include "../../Input/SystemInput.h"
// Construct script
#include "../../Logic/CScripts/AllScripts.h"
#include "../../Mono/MonoWrapper.h"

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

		return std::any(entComp);
	};

	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_ATTACH_COMP"),
		createComp, removeComp);
	_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddCommand, std::string("SCENE_EDITOR_REMOVE_COMP"),
		removeComp, createComp);

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
		}
		else if (LE_ECHELPER->GetPrefabInst().prefab_id != -1)
		{
			g_ecman = G_ECMANAGER_PREFABS;
			id = LE_ECHELPER->GetPrefabInst().prefab_id;
		}
		else
		{
			std::cout << "No set manager is allocated" << std::endl;
		}
		Entity 	ent = g_ecman->getEntity(id);
		// Entity name
		_levelEditor->LE_AddInputText("Name##Entity", g_ecman->EntityName[id], 256);
		
		ImGui::SameLine(0, 10);
		ImGui::Text("ID : ");

		ImGui::SameLine(0, 10);
		ImGui::Text(std::to_string(id).c_str());//std::to_string(LE_ECHELPER->GetSelectedEntityID()).c_str());

		//Componenets layout
		ComponentLayout(ent);
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

	ColliderComp(ent);

	AudioComp(ent);

	GraphicsComp(ent);

	LightComp(ent);

	RigidBodyComp(ent);

	ScriptComp(ent);

	CanvasComp(ent);

	AnimationComp(ent);

  CScriptComp(ent);

  PlayerStatsComp(ent);

  CauldronStatsComp(ent);

  VariableComp(ent);

  NavComp(ent);

  WayPointPathComp(ent);

  WayPointComp(ent);

	AddSelectedComps(ent);
}

void InspectorWindow::TransformComp(Entity& ent)
{
	TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
	if (trans_comp != NULL)
	{
		ImGui::NewLine();
    ImGui::InputInt("Tag", &(trans_comp->_tag));
    ImGui::NewLine();
    std::string enam = trans_comp->_entityName.toString();
		_levelEditor->LE_AddInputText("Entity Name", enam, 500, ImGuiInputTextFlags_EnterReturnsTrue,
		[&enam, &trans_comp]()
		{
				trans_comp->_entityName = enam;
		});

		TransformGizmo(trans_comp);
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
			}

			glm::vec3 glmVal = { col_comp->center };
			float* glmPtr = glm::value_ptr(glmVal);

			ImGui::Checkbox("IsCollidable##Collider", &col_comp->isCollidable);
			ImGui::InputInt("Collider Tag##COLTAG", &col_comp->colliderTag);

			if (ImGui::InputFloat3("Center##COLLIDER", glmPtr, 3))
			{
				col_comp->center = glm::make_vec3(glmPtr);
			}

			glmVal = { col_comp->extend };
			glmPtr = glm::value_ptr(glmVal);

			if (ImGui::InputFloat3("Extend##COLLIDER", glmPtr, 3))
			{
				col_comp->extend = glm::make_vec3(glmPtr);
			}

			glmVal = { col_comp->rotation };
			glmPtr = glm::value_ptr(glmVal);

			if (ImGui::InputFloat3("Rotation##COLLIDER", glmPtr, 3))
			{
				col_comp->rotation = glm::make_vec3(glmPtr);
			}
		}

		//Remove component
		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ComponentCollider>()->Write(), typeid(ComponentCollider).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		ImGui::Separator();
	}
}

void InspectorWindow::AudioComp(Entity& ent)
{
	ComponentLoadAudio* aud_manager = ent.getComponent<ComponentLoadAudio>();
	if (aud_manager != nullptr)
	{

		if (ImGui::CollapsingHeader("Audio Manager", &_notRemove))
		{
			if (ImGui::Button("Add Audio"))
			{
				static int i = 0;
				std::string s = std::to_string(i);
				ComponentLoadAudio::data d;
				strcpy_s(d.name, 128, s.c_str());
				strcpy_s(d.path, 512, "");
				aud_manager->_sounds.push_back(d);
				++i;
				
			}
      int index = 1;

			for (auto& data : aud_manager->_sounds) //[path, name]
			{
				std::string p = "##AUDIOPATH" + std::to_string(index);
				std::string n = "##AUDIONAME" + std::to_string(index);
				//ImGui::InputText(p.c_str(), data.path, 512);
				++index;
				//ImGui::InputText(n.c_str(), data.name, 256);

				std::string s_name = data.name;
				ImGui::SetNextItemWidth(50);
				_levelEditor->LE_AddInputText(n, s_name, 100, ImGuiInputTextFlags_EnterReturnsTrue,
					[&data, &s_name]()
					{
						strcpy_s(data.name,s_name.c_str());
					});

				ImGui::SameLine(0, 10);
				std::string s_path = data.path;

				_levelEditor->LE_AddInputText(p, s_path, 500, ImGuiInputTextFlags_EnterReturnsTrue,
					[&data, &s_path]()
					{
						strcpy_s(data.path, s_path.c_str());
					});

				_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
					[this, &data](std::string* str)
					{
						std::string newData = *str;
						newData.erase(newData.begin());
						std::transform(newData.begin(), newData.end(), newData.begin(),
							[](unsigned char c)
							{ return (char)std::tolower(c); });

						std::string fileType = LE_GetFileType(newData);
						if (fileType == "ogg")
						{
							memset(data.path, 0, 512);
							strcpy_s(data.path, newData.c_str());
						}
					});

			}
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
					if (fileType == "fbx" || fileType == "obj")
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
							AnimationController* animCtrl =  NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID];
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

			ImGui::Text("Materials");

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
		if (ImGui::CollapsingHeader("Rigid Body", &_notRemove))
		{
			//_levelEditor->LE_AddInputText("##GRAPHICS_1", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::Checkbox("IsStatic", &rb->isStatic);
			ImGui::Checkbox("IsGravity", &rb->isGravity);
			ImGui::InputFloat("Mass", &rb->mass);
			ImGui::InputFloat("Friction", &rb->friction);
			ImGui::InputFloat3("Force", rb->force.m);
			ImGui::InputFloat3("Acceleration", rb->acceleration.m);

		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<RigidBody>()->Write(), typeid(RigidBody).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			//ent.RemoveComponent<RigidBody>();
			_notRemove = true;
		}

		ImGui::Separator();
	}
}



void InspectorWindow::LightComp(Entity& ent)
{
	static int LIGHT = (int) NS_GRAPHICS::Lights::INVALID_TYPE;
	ComponentLight* light = ent.getComponent<ComponentLight>();
	if (light != nullptr)
	{
		if (ImGui::CollapsingHeader("Light", &_notRemove))
		{
			// use tmp variable to store active
			// this is so that we can call the function to set active state
			bool active_state = light->GetActive();

			//ImGui::Checkbox("Is Active", &light->_isActive);
			ImGui::Checkbox("Is Active", &active_state);

			light->SetActive(active_state);

			const char* lights[] = { "Directional", "Point", "Spot", "None" };

			if (light->GetActive() == false)
				LIGHT = (int)light->GetInactiveType();
			else
				LIGHT = (int) light->GetType();

			if (ImGui::Combo("Light Type", &LIGHT, lights, IM_ARRAYSIZE(lights)))
			{
				// Checks if set was successful
				if (!light->SetType((NS_GRAPHICS::Lights)LIGHT))
				{
					// Send error prompt if failed
					ImGui::OpenPopup("There can only be one sun and so many stars uwu");
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

			if ((NS_GRAPHICS::Lights)LIGHT != NS_GRAPHICS::Lights::INVALID_TYPE)
			{
				float intensity = light->GetIntensity();
				if (ImGui::DragFloat("Intensity", &intensity))
				{
					if (intensity < 0.f)
						intensity = 0.f;
					light->SetIntensity(intensity);
				}
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
			}

			if ((NS_GRAPHICS::Lights)LIGHT == NS_GRAPHICS::Lights::SPOT)
			{
				float cutoff = light->GetCutOff();
				float outercutoff = light->GetOuterCutOff();

				if(ImGui::InputFloat("Cut off", &cutoff))
					light->SetCutOff(cutoff);

				if(ImGui::InputFloat("Outer cut off", &outercutoff))
					light->SetOuterCutOff(outercutoff);
			}
		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ComponentLight>()->Write(), typeid(ComponentLight).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		ImGui::Separator();
	}
}

void InspectorWindow::ScriptComp(Entity& ent)
{
	ScriptComponent* Script_comp = ent.getComponent<ScriptComponent>();
	if (Script_comp != nullptr)
	{
		if (ImGui::CollapsingHeader("Script component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##CScript", &Script_comp->_isActive);

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
        int var_typeid = mono_type_get_type(mono_field_get_type(field));
        // To check for public fields/variables
        unsigned var_flag = mono_field_get_flags(field);
        if (var_flag == MONO_FIELD_ATTR_PUBLIC) // MONO_FIELD_ATTR_PUBLIC
        {
          bool bChanged = false; // If value changed, save it back to mono.
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
          }
          else if (var_typeid == MONO_TYPE_I4) // int
          {
            //std::cout << "Int" << std::endl;
            sName += " int";
            int typeInt = MonoWrapper::GetObjectFieldValue<int>(monoData._pInstance, var_name);
            if (ImGui::InputInt(sName.c_str(), &typeInt))
              MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeInt);
          }
          else if (var_typeid == MONO_TYPE_U4) // unsigned
          {
            //std::cout << "Unsigned" << std::endl;
            sName += " unsigned";
            int typeUnsigned = MonoWrapper::GetObjectFieldValue<unsigned>(monoData._pInstance, var_name);
            if (ImGui::InputInt(sName.c_str(), &typeUnsigned))
              MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeUnsigned);
          }
          else if (var_typeid == MONO_TYPE_R4) // float
          {
            //std::cout << "Float" << std::endl;
            sName += " float";
            float typeFloat = MonoWrapper::GetObjectFieldValue<float>(monoData._pInstance, var_name);
            if (ImGui::InputFloat(sName.c_str(), &typeFloat))
              MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeFloat);
          }
          else if (var_typeid == MONO_TYPE_R8) // double
          {
            //std::cout << "Double" << std::endl;
            sName += " double";
            double typeDouble = MonoWrapper::GetObjectFieldValue<double>(monoData._pInstance, var_name);
            if (ImGui::InputDouble(sName.c_str(), &typeDouble))
              MonoWrapper::SetObjectFieldValue(monoData._pInstance, var_name, typeDouble);
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
          }
          else
          {
            std::cout << "Unsupported type!" << std::endl;
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
		}

		if (!_notRemove)
		{
			//ent.RemoveComponent<GraphicsComponent>();
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<ScriptComponent>()->Write(), typeid(ScriptComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		ImGui::Separator();
	}
}

void InspectorWindow::CanvasComp(Entity& ent)
{
	CanvasComponent* canvas = ent.getComponent<CanvasComponent>();
	if (canvas != nullptr)
	{
		if (ImGui::CollapsingHeader("Canvas component", &_notRemove))
		{
			ImGui::Checkbox("IsActive##Canvas", &canvas->_isActive);

			size_t uiCount = canvas->_uiElements.size();
			for (size_t i = 0; i < uiCount; ++i)
			{
				ImGui::Checkbox("IsActive##UI", &canvas->_uiElements.at(i)._isActive);
				if (ImGui::Button(std::string("X##").append(std::to_string(i)).c_str()))
				{
					canvas->RemoveUI(i);
				}

				ImGui::SameLine();

				std::string tex = canvas->_uiElements.at(i)._fileName.toString();
				std::string uiName = canvas->_uiElements.at(i)._uiName.toString();
				canvas->_uiElements.at(i)._position;
				canvas->_uiElements.at(i)._size;

				ImGui::InputFloat3(std::string("UIPosition##").append(std::to_string(i)).c_str(), glm::value_ptr(canvas->_uiElements.at(i)._position), 3);
				ImGui::InputFloat2(std::string("UISize##").append(std::to_string(i)).c_str(), glm::value_ptr(canvas->_uiElements.at(i)._size), 3);

				_levelEditor->LE_AddInputText(std::string("UIName##").append(std::to_string(i)).c_str(), uiName, 500, ImGuiInputTextFlags_EnterReturnsTrue,
					[&uiName, &canvas, &i]()
					{
						canvas->_uiElements.at(i)._uiName = uiName;
					});

				_levelEditor->LE_AddInputText(std::string("Image##").append(std::to_string(i)).c_str(), tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
					[&tex, &canvas, &i]()
					{
						canvas->_uiElements.at(i).AddTexture(tex);
						canvas->_uiElements.at(i)._fileName = tex;
					});
				_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
					[this, &tex, &canvas, &i](std::string* str)
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
							canvas->_uiElements.at(i).AddTexture(tex);
							canvas->_uiElements.at(i)._fileName = tex;
						}
					});


				ImGui::ColorEdit4(std::string("Colour##Canvas").append(std::to_string(i)).c_str(), glm::value_ptr(canvas->_uiElements.at(i)._colour));

				ImGui::Separator();
			}

			if (ImGui::Button("Add UI"))
			{
				canvas->AddUI();
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

			auto it = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.begin();
			while ( it != NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_allAnims.end())
			{
				bool currAnim = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim == *it;
				if (ImGui::Selectable(it->c_str(), &currAnim))
				{
					NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID]->_currAnim = it->c_str();
					anim->StopAnimation();
				}
				++it;
			}

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
		if (ImGui::CollapsingHeader("Player Stats", &_notRemove))
		{
			//_levelEditor->LE_AddInputText("##GRAPHICS_1", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::InputFloat("player_move_mag", &psc->player_move_mag);
			ImGui::InputFloat("player_fly_mag", &psc->player_fly_mag);
			ImGui::InputInt("player_max_energy", &psc->player_max_energy);
			ImGui::InputInt("player_possess_energy_drain", &psc->player_possess_energy_drain);
			ImGui::InputInt("player_moth_energy_drain", &psc->player_moth_energy_drain);
			ImGui::InputFloat("camera_distance", &psc->camera_distance);
			ImGui::InputFloat3("camera_offset", psc->camera_offset.m);
			ImGui::InputFloat("player_max_speed", &psc->player_max_speed);

		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<PlayerStatsComponent>()->Write(), typeid(PlayerStatsComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		ImGui::Separator();
	}
}

void InspectorWindow::CauldronStatsComp(Entity& ent)
{
	CauldronStatsComponent* csc = ent.getComponent<CauldronStatsComponent>();
	if (csc != nullptr)
	{
		if (ImGui::CollapsingHeader("Cauldron Stats", &_notRemove))
		{
			std::string talis = csc->talisman.toString();
			_levelEditor->LE_AddInputText("talisman", talis, 256, ImGuiInputTextFlags_EnterReturnsTrue,
				[&talis, &csc]()
				{
					csc->talisman = talis;
				});


			std::string col = csc->collider.toString();
			_levelEditor->LE_AddInputText("collider", col, 256, ImGuiInputTextFlags_EnterReturnsTrue,
				[&col, &csc]()
				{
					csc->collider = col;
				});

			ImGui::InputFloat("magnitude", &csc->magnitude);
			ImGui::InputFloat3("direction", csc->direction.m);
		}

		if (!_notRemove)
		{
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<CauldronStatsComponent>()->Write(), typeid(CauldronStatsComponent).hash_code() };
			_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
			_notRemove = true;
		}

		ImGui::Separator();
	}
}

void InspectorWindow::VariableComp(Entity& ent)
{
	ComponentVariables* comp_var = ent.getComponent<ComponentVariables>();
	if (comp_var != nullptr)
	{

		if (ImGui::CollapsingHeader("Variable component", &_notRemove))
		{

			if (ImGui::Button("Add Float"))
			{
				float fl = 0.0f;
				comp_var->float_list.push_back(fl);
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Float"))
			{
				comp_var->float_list.pop_back();
			}

			int float_index = 1;
			for (float& f : comp_var->float_list) //[path, name]
			{
				std::string p = "Float_" + std::to_string(float_index);

				_levelEditor->LE_AddInputFloatProperty(p, f, []() {},ImGuiInputTextFlags_EnterReturnsTrue);
				float_index++;
			}

			if (ImGui::Button("Add Interger"))
			{
				int interger = 0;
				comp_var->int_list.push_back(interger);
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Interger"))
			{
				comp_var->int_list.pop_back();
			}
			int int_index = 1;

			for (int& i : comp_var->int_list) //[path, name]
			{
				std::string p = "Int_" + std::to_string(int_index);

				std::string s_name = std::to_string(int_index);
				_levelEditor->LE_AddInputIntProperty(p, i, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
				int_index++;
			}

			if (ImGui::Button("Add String"))
			{
				LocalString ls;
				comp_var->string_list.push_back(ls);
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove String"))
			{
				comp_var->string_list.pop_back();
			}

			int str_index = 1;

			for (LocalString<125>& str : comp_var->string_list) //[path, name]
			{
				std::string p = "String_" + std::to_string(str_index);
				
				std::string s_name = str;
				_levelEditor->LE_AddInputText(p, s_name, 100, ImGuiInputTextFlags_EnterReturnsTrue,
					[&str, &s_name]()
					{
						str = s_name.c_str();
					});
				str_index++;
			}

		}
	}

	if (!_notRemove)
	{
		//ent.RemoveComponent<ComponentLoadAudio>();
		ENTITY_COMP_DOC comp{ ent, ent.getComponent<ComponentVariables>()->Write(), typeid(ComponentVariables).hash_code() };
		_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
		_notRemove = true;
	}

	ImGui::Separator();
}

void InspectorWindow::NavComp(Entity& ent)
{
	NavComponent* nav_comp = ent.getComponent<NavComponent>();
	if (nav_comp != nullptr)
	{
		if (ImGui::CollapsingHeader("Navigator", &_notRemove))
		{
			_levelEditor->LE_AddInputFloatProperty("Speed", nav_comp->speed, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
			_levelEditor->LE_AddCombo("Way Point Nav Type", (int&)nav_comp->wp_nav_type,
				{
					"To and fro",
					"circular",
					"random"
				});
			//_levelEditor->LE_AddInputFloatProperty("Radius for detection", nav_comp->rad_for_detect, []() {},  ImGuiInputTextFlags_EnterReturnsTrue);
			_levelEditor->LE_AddCheckbox("Stop at each waypoint", &nav_comp->stopAtEachWayPoint);

			if(nav_comp->stopAtEachWayPoint)
				_levelEditor->LE_AddInputFloatProperty("End time", nav_comp->endTime, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
			
			//Way point path to look at
			std::string s_name;

			if (nav_comp->cur_wp_path != nullptr)
				s_name = G_ECMANAGER->EntityName[G_ECMANAGER->getEntity(nav_comp->cur_wp_path).getId()];

			_levelEditor->LE_AddInputText("WayPointPath", s_name, 100, ImGuiInputTextFlags_EnterReturnsTrue,
				[&s_name, &nav_comp]()
				{
					//str = s_name.c_str();
					nav_comp->cur_wp_path = G_ECMANAGER->getEntityUsingEntName(s_name).getComponent<WayPointMapComponent>();
				});
			_levelEditor->LE_AddDragDropTarget<Entity>("HIERARCHY_ENTITY_OBJECT",
				[this, &s_name,&nav_comp](Entity* entptr)
				{
					nav_comp->cur_wp_path = entptr->getComponent<WayPointMapComponent>();
					if (nav_comp->cur_wp_path != nullptr) {
						s_name = G_ECMANAGER->EntityName[entptr->getId()];
						nav_comp->wp_path_ent_name = s_name;
					}
				});

			if (!s_name.empty())
			{
				_levelEditor->LE_AddCombo("WayPoints to navigate", (int&)nav_comp->wp_creation_type,
				{
					"Standard (1->N)",
					"Reverse (N->1)",
					"Custom"
				});

				if (nav_comp->wp_creation_type == WPP_CUSTOM)
				{
					if (ImGui::Button("Add WayPoint Index"))
					{
						int interger = 0;
						nav_comp->path_indexes.push_back(interger);
					}
					ImGui::SameLine();
					if (ImGui::Button("Remove WayPoint Index"))
					{
						nav_comp->path_indexes.pop_back();
					}

					int wp_index = 1;
					for (int& i : nav_comp->path_indexes) //[path, name]
					{
						std::string p = "WayPoint_" + std::to_string(wp_index);
						_levelEditor->LE_AddInputIntProperty(p, i, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
						wp_index++;
					}

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
}

void InspectorWindow::WayPointPathComp(Entity& ent)
{
	WayPointMapComponent* wpm_comp = ent.getComponent<WayPointMapComponent>();
	if (wpm_comp != nullptr)
	{
		if (ImGui::CollapsingHeader("Way Point Path", &_notRemove))
		{
			if (ImGui::Button("Add WayPoint"))
			{
				LocalString ls;
				wpm_comp->way_point_list.push_back(ls);
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove WayPoint"))
			{
				wpm_comp->way_point_list.pop_back();
			}

			int str_index = 1;

			for (LocalString<125> & str : wpm_comp->way_point_list) //[path, name]
			{
				std::string p = "Waypoint_" + std::to_string(str_index);

				std::string s_name = str;
				_levelEditor->LE_AddInputText(p, s_name, 100, ImGuiInputTextFlags_EnterReturnsTrue,
					[&str, &s_name]()
					{
						str = s_name.c_str();
					});
				_levelEditor->LE_AddDragDropTarget<Entity>("HIERARCHY_ENTITY_OBJECT",
					[this, &str](Entity* entptr)
					{
						str = G_ECMANAGER->EntityName[entptr->getId()];

					});

				str_index++;
			}
		}
	}

	if (!_notRemove)
	{
		//ent.RemoveComponent<ComponentLoadAudio>();
		ENTITY_COMP_DOC comp{ ent, ent.getComponent<WayPointMapComponent>()->Write(), typeid(WayPointMapComponent).hash_code() };
		_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
		_notRemove = true;
	}
}

void InspectorWindow::WayPointComp(Entity& ent)
{
	WayPointComponent* wp_comp = ent.getComponent<WayPointComponent>();
	if (wp_comp != nullptr)
	{
		if (ImGui::CollapsingHeader("Way-Point Component", &_notRemove))
		{
			_levelEditor->LE_AddInputFloatProperty("Self-Define Cost", wp_comp->self_define_var.second, []() {}, ImGuiInputTextFlags_EnterReturnsTrue);
		}
	}

	if (!_notRemove)
	{
		//ent.RemoveComponent<ComponentLoadAudio>();
		ENTITY_COMP_DOC comp{ ent, ent.getComponent<WayPointComponent>()->Write(), typeid(WayPointComponent).hash_code() };
		_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
		_notRemove = true;
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
			"  CScript",
			"  C#Script",
			"  Canvas",
			"  PlayerStats",
			"  CauldronStats",
			"  VariablesComp",
			"  NavComp",
			"  WayPointPath",
			"  WayPointComp"
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
		}
		case 7: // C#Script
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
		case 8: // Canvas
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
		case 9: // PlayerStats
		{
			if (!ent.getComponent<PlayerStatsComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, PlayerStatsComponent().Write(), typeid(PlayerStatsComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 10: // PlayerStats
		{
			if (!ent.getComponent<CauldronStatsComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, CauldronStatsComponent().Write(), typeid(CauldronStatsComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 11: // ComponentVariable
		{
			if (!ent.getComponent<ComponentVariables>())
			{
				ENTITY_COMP_DOC comp{ ent, ComponentVariables().Write(), typeid(ComponentVariables).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 12: // ComponentNav
		{
			if (!ent.getComponent<NavComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, NavComponent().Write(), typeid(NavComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 13: // WayPointPath
		{
			if (!ent.getComponent<WayPointMapComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, WayPointMapComponent().Write(), typeid(WayPointMapComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 14: // WayPoint
		{
			if (!ent.getComponent<WayPointComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, WayPointComponent().Write(), typeid(WayPointComponent).hash_code() };
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

bool InspectorWindow::EditTransform(const float* cameraView, float* cameraProjection, float* matrix)
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
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

	if (ImGui::InputFloat3("Translation##TRANSLATION", matrixTranslation, 3, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		_lastEnter = true;
	}
	if (ImGui::InputFloat3("Rotation##ROTATION", matrixRotation, 3, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		_lastEnter = true;
	}
	if (ImGui::InputFloat3("Scale##SCALE", matrixScale, 3, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		_lastEnter = true;
	}

	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	if (_mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("World", _mCurrentGizmoMode == ImGuizmo::WORLD))
			_mCurrentGizmoMode = ImGuizmo::WORLD;
		ImGui::SameLine();
		if (ImGui::RadioButton("Local", _mCurrentGizmoMode == ImGuizmo::LOCAL))
			_mCurrentGizmoMode = ImGuizmo::LOCAL;
	}

	ImGui::Checkbox("", &_useSnap);
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
	return ImGuizmo::Manipulate(cameraView, cameraProjection, _mCurrentGizmoOperation, _mCurrentGizmoMode, matrix, NULL, _useSnap ? snapPtr : NULL, NULL, NULL);
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
			float fov = 44.5f;
			//Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 1.0f, 1000.f, cameraProjection);
			float* cameraProjection;
			if (windowSize.x && windowSize.y)
			{
				glm::mat4 persp = glm::perspective(glm::radians(fov), (float)windowSize.x / (float)windowSize.y, 1.0f, 1000.0f);
				cameraProjection = glm::value_ptr(persp);
			}
			else
			{
				glm::mat4 persp = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 1.0f, 1000.0f);
				cameraProjection = glm::value_ptr(persp);
			}

			glm::mat4 matObj = trans_comp->GetModelMatrix();
			ImGuizmo::SetID(0);
			if (EditTransform(camView, cameraProjection, glm::value_ptr(matObj)))
			{
				// Checks FIRST frame of manipulation only
				if (!_lastPos_Start)
				{
					_lastPos_Start = true;
					_lastPos_ELP = { trans_comp, trans_comp->GetModelMatrix() };
				}

				// Sets object to new position
				float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(matObj), trans, rot, scale);

				trans_comp->_position = glm::make_vec3(trans);
				trans_comp->_rotation = glm::make_vec3(rot);
				trans_comp->_scale = glm::make_vec3(scale);
			}
			else
			{
				// Checks if not manipulating and mouse is let go
				if (_lastPos_Start && !SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IMOUSE_LBUTTON))
				{
					_lastPos_Start = false;
					// New position for the object
					ENTITY_LAST_POS newObj{ trans_comp , matObj };
					std::any curPos = newObj;

					// Reset object back to original position
					float trans[3] = { 0,0,0 }, rot[3] = { 0,0,0 }, scale[3] = { 0,0,0 };
					ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(_lastPos_ELP._newPos), trans, rot, scale);
					trans_comp->_position = glm::make_vec3(trans);
					trans_comp->_rotation = glm::make_vec3(rot);
					trans_comp->_scale = glm::make_vec3(scale);

					// Runs command to move object to new position from old position
					_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_SET_ENTITY_POSITION"), curPos);
				}
				else if (_lastEnter)
				{
					_lastEnter = false;

					ENTITY_LAST_POS newObj{ trans_comp , matObj };
					std::any curPos = newObj;

					// Runs command to move object to new position from old position
					_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_SET_ENTITY_POSITION"), curPos);
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
