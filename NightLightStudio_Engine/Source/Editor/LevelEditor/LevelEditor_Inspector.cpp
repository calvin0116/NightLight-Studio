#include "LevelEditor_Inspector.h"

#include "LevelEditor_ECHelper.h"
#include "../../Core/SceneManager.h"
#include "../../Graphics/GraphicsSystem.h"

#include <set>
#include "LevelEditor_Console.h"
#include "../../Input/SystemInput.h"


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
		ImGuiInputTextFlags itf = 0;
		itf |= ImGuiInputTextFlags_EnterReturnsTrue;

		char buf[256];
		strcpy_s(buf, 256,
			NS_SCENE::SYS_SCENE_MANAGER->EntityName[LE_ECHELPER->GetSelectedEntityID()].c_str()
		);

		if (ImGui::InputText("Name", buf, 256, itf))
			NS_SCENE::SYS_SCENE_MANAGER->EntityName[LE_ECHELPER->GetSelectedEntityID()] = std::string(buf);

		// Print out the ID of the entity (Debug purposes)
		ImGui::SameLine(0, 10);
		ImGui::Text("ID : ");

		ImGui::SameLine(0, 10);
		ImGui::Text(std::to_string(LE_ECHELPER->GetSelectedEntityID()).c_str());

		//Componenets layout
		ComponentLayout(ent);
	}
}


void InspectorWindow::ComponentLayout(Entity& ent)
{
	TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
	if (trans_comp != NULL)
	{
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
	//Standard bool for all component to use
	bool not_remove = true;

	ComponentCollider* col_comp = ent.getComponent<ComponentCollider>();
	if (col_comp != NULL)
	{
		//~~! Need Help==//
		//1. Check collider type that it have
		//2. Get the right collider type
		//3. Insert name
		std::string name = "Collider";	//e.g.
		switch (col_comp->colliderType)
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
		if (ImGui::CollapsingHeader(name.c_str(), &not_remove))
		{

		}

		//Remove component
		if (!not_remove)
		{
			ent.RemoveComponent<ComponentCollider>();
			/*
			switch (col_comp->colliderType)
			{

				case COLLIDERS::PLANE:
				{
					ent.RemoveComponent<PlaneCollider>();
					break;
				}
				case COLLIDERS::AABB:
				{
					ent.RemoveComponent<AABBCollider>();
					break;
				}
				case COLLIDERS::SPHERE:
				{
					ent.RemoveComponent<SphereCollider>();
					break;
				}
				case COLLIDERS::OBB:
				{
					ent.RemoveComponent<OBBCollider>();
					break;
				}
				case COLLIDERS::CAPSULE:
				{
					ent.RemoveComponent<CapsuleCollider>();
					break;
				}
			}*/
			not_remove = true;
		}
	}

	ComponentLoadAudio* aud_manager = ent.getComponent<ComponentLoadAudio>();
	if (aud_manager != nullptr)
	{

		if (ImGui::CollapsingHeader("Audio Manager", &not_remove))
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

		if (!not_remove)
		{
			ent.RemoveComponent<ComponentLoadAudio>();
			not_remove = true;
		}
	}


	GraphicsComponent* graphics_comp = ent.getComponent<GraphicsComponent>();
	if (graphics_comp != nullptr)
	{
		if (ImGui::CollapsingHeader("Graphics component", &not_remove))
		{
			ImGui::Checkbox("IsActive##Grahpic", &graphics_comp->_isActive);

			std::string tex = graphics_comp->_textureFileName.toString();
			std::string mod = graphics_comp->_modelFileName.toString();
			_levelEditor->LE_AddInputText("Texture file", tex, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			_levelEditor->LE_AddInputText("Model file", mod, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			if (graphics_comp->_modelFileName.toString() != mod && !mod.empty() )
			{
				graphics_comp->_modelFileName = mod;
				NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadModel(graphics_comp->_modelFileName.toString());
				graphics_comp->_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(graphics_comp->_modelFileName.toString());
			}
			graphics_comp->_textureFileName = tex;
			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
		}

		if (!not_remove)
		{
			ent.RemoveComponent<GraphicsComponent>();
			not_remove = true;
		}
	}

	RigidBody* rb = ent.getComponent<RigidBody>();
	if (rb != nullptr)
	{
		if (ImGui::CollapsingHeader("Rigid Body", &not_remove))
		{
			//_levelEditor->LE_AddInputText("##GRAPHICS_1", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::Checkbox("IsStatic", &rb->isStatic);
			ImGui::InputFloat3("Force", rb->force.m);
			ImGui::InputFloat3("Acceleration", rb->acceleration.m);

		}

		if (!not_remove)
		{
			ent.RemoveComponent<RigidBody>();
			not_remove = true;
		}
	}

	static int item_type = 0;
	
	ImGui::Combo(" ", &item_type, "Add component\0  RigidBody\0  Audio\0  Graphics\0--Collider--\0  AABB Colider\0  OBB Collider\0  Plane Collider\0  SphereCollider\0  CapsuleCollider\0");

	void* next_lol = nullptr;

	if (ImGui::Button("Add Selected Component"))
	{
		switch (item_type)
		{
		case 1:
		{
			next_lol = ent.AddComponent<RigidBody>();
			break;
		}
		case 2:
		{
			next_lol = ent.AddComponent<ComponentLoadAudio>();
			break;
		}
		case 3:
		{
			next_lol = ent.AddComponent<GraphicsComponent>();
			break;
		}
		/*
		case 4:
		{
			next_lol = ent.AddComponent<ColliderComponent>();
			break;
		}*/
		
		case 5:
		{
			ColliderComponent aabb(COLLIDERS::AABB);
			ent.AttachComponent(aabb);
			break;
		}

		case 6:
		{
			ColliderComponent obb(COLLIDERS::OBB);
			//next_lol = ent.AddComponent<OBBCollider>();
			ent.AttachComponent(obb);
			break;
		}
		case 7:
		{
			//next_lol = ent.AddComponent<PlaneCollider>();
			ColliderComponent plane(COLLIDERS::PLANE);
			ent.AttachComponent(plane);
			break;
		}
		case 8:
		{
			ColliderComponent sphere(COLLIDERS::SPHERE);
			ent.AttachComponent(sphere);
			break;
		}
		case 9:
		{
			//next_lol = ent.AddComponent<CapsuleCollider>();
			ColliderComponent capsule(COLLIDERS::CAPSULE);
			ent.AttachComponent(capsule);
			break;
		}
		}
		if (next_lol == nullptr)
		{
			std::cout << "Component has already been created" << std::endl;
		}
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
			glm::mat4 persp = glm::perspective(glm::radians(fov), (float)windowSize.x / (float)windowSize.y, 1.0f, 1000.0f);
			cameraProjection = glm::value_ptr(persp);

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
				if (_lastPos_Start && SYS_INPUT->GetSystemKeyPress().GetKeyRelease(SystemInput_ns::IMOUSE_LBUTTON))
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
