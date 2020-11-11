#include "LevelEditor_Inspector.h"

#include "LevelEditor_ECHelper.h"
#include "../../Core/SceneManager.h"
#include "../../Graphics/GraphicsSystem.h"

#include <set>
#include "LevelEditor_Console.h"
#include "../../Input/SystemInput.h"
// Construct script
#include "../../Logic/CScripts/AllScripts.h"
//#include "../../Mono/MonoWrapper.h"

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
			NS_GRAPHICS::LightSystem::GetInstance().DetachLightComponent(entComp._ent);
			//entComp._ent.RemoveComponent<LightComponent>();
		}

		else if (t == typeid(ScriptComponent).hash_code())
		{
			entComp.Copy(entComp._ent.getComponent<ScriptComponent>()->Write());
			entComp._ent.RemoveComponent<ScriptComponent>();
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
	if (LE_ECHELPER->GetSelectedEntityID() != -1)
	{
		if (!ImGui::IsWindowAppearing() && !LE_ECHELPER->setFocus)
		{
			ImGui::SetWindowFocus();
			LE_ECHELPER->setFocus = true;
		}

		//Get entity
		Entity ent = G_ECMANAGER->getEntity(LE_ECHELPER->GetSelectedEntityID());
		// Entity name
		/*
		ImGuiInputTextFlags itf = 0;
		itf |= ImGuiInputTextFlags_EnterReturnsTrue;

		char buf[256];
		strcpy_s(buf, 256,
			NS_SCENE::SYS_SCENE_MANAGER->EntityName[LE_ECHELPER->GetSelectedEntityID()].c_str()
		);

		if (ImGui::InputText("Name", buf, 256, itf))
			NS_SCENE::SYS_SCENE_MANAGER->EntityName[LE_ECHELPER->GetSelectedEntityID()] = std::string(buf);
		*/
		_levelEditor->LE_AddInputText("Name##Entity", NS_SCENE::SYS_SCENE_MANAGER->EntityName[LE_ECHELPER->GetSelectedEntityID()], 256);

		// Print out the ID of the entity (Debug purposes)
		ImGui::SameLine(0, 10);
		ImGui::Text("ID : ");

		ImGui::SameLine(0, 10);
		ImGui::Text(std::to_string(LE_ECHELPER->GetSelectedEntityID()).c_str());

		//Componenets layout
		ComponentLayout(ent);
	}
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

  CScriptComp(ent);

  PlayerStatsComp(ent);

	AddSelectedComps(ent);
}

void InspectorWindow::TransformComp(Entity& ent)
{
	TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
	if (trans_comp != NULL)
	{
		std::string enam = trans_comp->_entityName.toString();

		_levelEditor->LE_AddInputText("Entity Name", enam, 500, ImGuiInputTextFlags_EnterReturnsTrue,
		[&enam, &trans_comp]()
		{
				trans_comp->_entityName = enam;
		});

		TransformGizmo(trans_comp);
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
      ImGui::Checkbox("IsCollidable##Collider", &col_comp->isCollidable);
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
				aud_manager->_sounds.push_back(ComponentLoadAudio::data());
			}


			for (auto& [path, name] : aud_manager->_sounds)
			{
				/*
			  char buf[512];
			  char buf2[256];
			  strcpy_s(buf, 512, path.c_str());
			  strcpy_s(buf2, 256, name.c_str());
			  ImGui::InputText("##NAME", buf, 512);
			  ImGui::SameLine(0, 10);
			  ImGui::InputText("##OTHERNAME", buf2, 256);
			  */
				ImGui::InputText("##AUDIOPATH", path, 512);
				ImGui::SameLine(0, 10);
				ImGui::InputText("##AUDIONAME", name, 256);
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
			ImGui::Checkbox("IsActive##Grahpic", &graphics_comp->_isActive);

			std::string tex = graphics_comp->_albedoFileName.toString();
			std::string mod = graphics_comp->_modelFileName.toString();
			std::string normal = graphics_comp->_normalFileName.toString();
			std::string specular = graphics_comp->_specularFileName.toString();
			std::string metallic = graphics_comp->_metallicFileName.toString();
			std::string ao = graphics_comp->_aoFileName.toString();
			std::string roughness = graphics_comp->_roughnessFileName.toString();

			_levelEditor->LE_AddInputText("Texture file", tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&tex, &graphics_comp]()
				{
				});
			_levelEditor->LE_AddInputText("Model file", mod, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			// Drag and Drop from Asset Inspector onto Model File Name
			_levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
				[this, &mod, &graphics_comp](std::string* str)
				{
					std::string data = *str;
					std::transform(data.begin(), data.end(), data.begin(),
						[](unsigned char c)
						{ return (char)std::tolower(c); });

					std::string fileType = LE_GetFileType(data);
					if (fileType == "fbx" || fileType == "obj")
						mod = data;
				});

			_levelEditor->LE_AddInputText("Specular file", specular, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&specular, &graphics_comp]()
				{
				});


			if (graphics_comp->_modelFileName.toString() != mod && !mod.empty())
			{
				graphics_comp->_modelFileName = mod;
				NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadModel(graphics_comp->_modelFileName.toString());
				graphics_comp->_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(graphics_comp->_modelFileName.toString());
			}

			if (graphics_comp->_albedoFileName.toString() != tex && !tex.empty())
			{
				graphics_comp->_albedoFileName = tex;
				graphics_comp->_albedoID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(graphics_comp->_albedoFileName.toString());
			}

			if (graphics_comp->_specularFileName.toString() != specular && !specular.empty())
			{
				graphics_comp->_specularFileName =  specular;
				graphics_comp->_specularID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(graphics_comp->_specularFileName.toString());
			}

			ImGui::Separator();

			ImGui::Text("Materials");

			ImGui::InputFloat3("Diffuse##Graphics", glm::value_ptr(graphics_comp->_materialData._diffuse));

			ImGui::InputFloat3("Ambient##Graphics", glm::value_ptr(graphics_comp->_materialData._ambient));

			ImGui::InputFloat3("Specular##Graphics", glm::value_ptr(graphics_comp->_materialData._specular));

			ImGui::InputFloat("Shininess", &graphics_comp->_materialData._shininess);

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
			ImGui::Checkbox("Is Active", &light->_isActive);

			const char* lights[] = { "Directional", "Point", "Spot", "None" };
			LIGHT = (int) light->_type;
			if (ImGui::Combo("Light Type", &LIGHT, lights, IM_ARRAYSIZE(lights)))
			{
				NS_GRAPHICS::LightSystem::GetInstance().ChangeLightType(ent, (NS_GRAPHICS::Lights)LIGHT);
			}

			if (ImGui::InputFloat3("Diffuse", glm::value_ptr(light->_diffuse)))
			{
				light->SetDiffuse(light->_diffuse);
			}
			
			if (ImGui::InputFloat3("Ambient", glm::value_ptr(light->_ambient)))
			{
				light->SetAmbient(light->_ambient);
			}

			if (ImGui::InputFloat3("Specular", glm::value_ptr(light->_specular)))
			{
				light->SetSpecular(light->_specular);
			}

			if (light->_type != NS_GRAPHICS::Lights::DIRECTIONAL)
			{
				if (ImGui::InputFloat("Attenuation", &light->_attenuation))
				{
					light->SetAttenuation(light->_attenuation);
				}
			}

			if (light->_type == NS_GRAPHICS::Lights::SPOT)
			{
				ImGui::InputFloat("Cut off", &light->_cutOff);
				ImGui::InputFloat("Outer cut off", &light->_outerCutOff);
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
			std::string tex = Script_comp->_ScriptName.toString();
      std::string old = tex;

			_levelEditor->LE_AddInputText("Script Name", tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
				[&tex, &Script_comp]()
			{
				Script_comp->_ScriptName = tex;
			});

      //// Changes occured
      //if (tex != old)
      //{
      //  std::cout << "Constructing _pScript..." << std::endl;
      //  if (Script_comp->_MonoData._GCHandle) // Already has a script
      //  {

      //  }
      //  cScript_comp->_pScript = AllScripts::Construct(tex);
      //}
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

void InspectorWindow::CScriptComp(Entity& ent)
{
  CScriptComponent* cScript_comp = ent.getComponent<CScriptComponent>();
  if (cScript_comp != nullptr)
  {
    if (ImGui::CollapsingHeader("CScript component", &_notRemove))
    {
      ImGui::Checkbox("IsActive##CScript", &cScript_comp->_isActive);
      ImGui::InputInt("Tag", &cScript_comp->_iTag);
      std::string tex = cScript_comp->_sName.toString();
      std::string old = tex;
      _levelEditor->LE_AddInputText("Script Name", tex, 500, ImGuiInputTextFlags_EnterReturnsTrue,
        [&tex, &cScript_comp]()
        {
          cScript_comp->_sName = tex;
        });
      // Changes occured
      if (tex != old)
      {
        if (cScript_comp->_pScript) // Already has a script
        {
          delete cScript_comp->_pScript;
          cScript_comp->_pScript = nullptr;
        }
        cScript_comp->_pScript = AllScripts::Construct(tex);
        if (cScript_comp->_pScript)
        {
          std::cout << "Constructed Script" << std::endl;
          cScript_comp->_pScript->SetEntity(ent);
        }
      }
    }

    if (!_notRemove)
    {
      //ent.RemoveComponent<GraphicsComponent>();
      ENTITY_COMP_DOC comp{ ent, ent.getComponent<CScriptComponent>()->Write(), typeid(CScriptComponent).hash_code() };
      _levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_REMOVE_COMP"), std::any(comp));
      _notRemove = true;
    }

    ImGui::Separator();
  }
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

void InspectorWindow::AddSelectedComps(Entity& ent)
{
	_levelEditor->LE_AddCombo("##AddComponentsCombo", _itemType,
		{
			"Add component",
			"  RigidBody",
			"  Audio",
			"  Graphics",
			"  Light   ",
			"--Collider--",
			"  AABB Colider",
			"  OBB Collider",
			"  Plane Collider",
			"  SphereCollider",
			"  CapsuleCollider",
			"------------",
			"  CScript",
			"  C#Script",
			"  PlayerStats"
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

		case 6:
		{
			if (!ent.getComponent<ColliderComponent>())
			{
				//ent.AttachComponent(aabb);
				ENTITY_COMP_DOC comp{ ent, ColliderComponent(COLLIDERS::AABB).Write(), typeid(ColliderComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}

		case 7:
		{
			if (!ent.getComponent<ColliderComponent>())
			{
				//next_lol = ent.AddComponent<OBBCollider>();
				//ent.AttachComponent(obb);
				ENTITY_COMP_DOC comp{ ent, ColliderComponent(COLLIDERS::OBB).Write(), typeid(ColliderComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 8:
		{
			if (!ent.getComponent<ColliderComponent>())
			{
				//next_lol = ent.AddComponent<PlaneCollider>();
				//ent.AttachComponent(plane);
				ENTITY_COMP_DOC comp{ ent, ColliderComponent(COLLIDERS::PLANE).Write(), typeid(ColliderComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 9:
		{
			if (!ent.getComponent<ColliderComponent>())
			{
				//ent.AttachComponent(sphere);
				ENTITY_COMP_DOC comp{ ent, ColliderComponent(COLLIDERS::SPHERE).Write(), typeid(ColliderComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
			}
			break;
		}
		case 10:
		{
			if (!ent.getComponent<ColliderComponent>())
			{
				//next_lol = ent.AddComponent<CapsuleCollider>();
				//ent.AttachComponent(capsule);
				ENTITY_COMP_DOC comp{ ent, ColliderComponent(COLLIDERS::CAPSULE).Write(), typeid(ColliderComponent).hash_code() };
				_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::RunCommand, std::string("SCENE_EDITOR_ATTACH_COMP"), std::any(comp));
				//ColliderComponent capsule(COLLIDERS::CAPSULE);
				//ent.AttachComponent(capsule);
			}
			break;
		}
		
		//case 11: -> ------
		case 12: // CScript
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
		case 13: // C#Script
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
		case 14: // PlayerStats
		{
			if (!ent.getComponent<PlayerStatsComponent>())
			{
				ENTITY_COMP_DOC comp{ ent, PlayerStatsComponent().Write(), typeid(PlayerStatsComponent).hash_code() };
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

	switch (_mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &_snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &_snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &_snap[0]);
		break;
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	return ImGuizmo::Manipulate(cameraView, cameraProjection, _mCurrentGizmoOperation, _mCurrentGizmoMode, matrix, NULL, _useSnap ? &_snap[0] : NULL, NULL, NULL);
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
