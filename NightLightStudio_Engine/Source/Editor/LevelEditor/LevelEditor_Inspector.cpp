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
		_levelEditor->LE_AddInputText("Name##Entity", SYS_COMPONENT->EntityName[LE_ECHELPER->GetSelectedEntityID()], 256);

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

	CanvasComp(ent);

  CScriptComp(ent);

  PlayerStatsComp(ent);

  CauldronStatsComp(ent);

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

			//ImGui::Checkbox("Is Active", &light->_isActive);
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
				[&graphics_comp, &mod]()
				{
					graphics_comp->AddModel(mod);
				});
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
					{
						//assimp doesnt deal with preceding slash
						if (data[0] == '\\')
						{
							data.erase(0, 1);
						}
						mod = data;
						graphics_comp->AddModel(mod);
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
						specular = data;
						graphics_comp->AddSpecularTexture(specular);
					}
				});

			ImGui::Separator();

			ImGui::Text("Materials");

			ImGui::ColorEdit3("Diffuse##Graphics", glm::value_ptr(graphics_comp->_materialData._diffuse));

			ImGui::ColorEdit3("Ambient##Graphics", glm::value_ptr(graphics_comp->_materialData._ambient));

			ImGui::ColorEdit3("Specular##Graphics", glm::value_ptr(graphics_comp->_materialData._specular));

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
			// use tmp variable to store active
			// this is so that we can call the function to set active state
			bool active_state = light->GetActive();

			//ImGui::Checkbox("Is Active", &light->_isActive);
			ImGui::Checkbox("Is Active", &active_state);

			light->SetActive(active_state);

			const char* lights[] = { "Directional", "Point", "Spot", "None" };
			LIGHT = (int) light->_type;
			if (ImGui::Combo("Light Type", &LIGHT, lights, IM_ARRAYSIZE(lights)))
			{
				NS_GRAPHICS::LightSystem::GetInstance().ChangeLightType(light, (NS_GRAPHICS::Lights)LIGHT);
			}

			if (ImGui::ColorEdit3("Diffuse", glm::value_ptr(light->_diffuse)))
			{
				light->SetDiffuse(light->_diffuse);
			}
			
			if (ImGui::ColorEdit3("Ambient", glm::value_ptr(light->_ambient)))
			{
				light->SetAmbient(light->_ambient);
			}

			if (ImGui::ColorEdit3("Specular", glm::value_ptr(light->_specular)))
			{
				light->SetSpecular(light->_specular);
			}

			if (light->_type != NS_GRAPHICS::Lights::DIRECTIONAL)
			{
				// This is fucked
				float intensity = light->_intensity;
				if (ImGui::DragFloat("Intensity", &intensity))
				{
					if (intensity < 0.f)
						intensity = 0.f;
					light->SetAttenuation(1.f / intensity);
					light->_intensity = intensity;
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
			ENTITY_COMP_DOC comp{ ent, ent.getComponent<GraphicsComponent>()->Write(), typeid(GraphicsComponent).hash_code() };
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
        cScript_comp->_pScript = AllScripts::MyConstruct(tex);
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
			"  CauldronStats"
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
