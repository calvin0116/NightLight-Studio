#include "LevelEditor_Inspecter.h"

#include "LevelEditor_ECHelper.h"
#include "../../Core/SceneManager.h"

#include <set>


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
		if (ImGui::CollapsingHeader("Transform"))
		{
			//float* rotation = &ent_selected->GetComponent<TransformComponent>().GetRotation().m[2];

			ImGui::InputFloat3("Position", glm::value_ptr(trans_comp->_position)); //,-100.f, 100.f); // Edit 3 floats representing a color
			ImGui::InputFloat3("Scale", glm::value_ptr(trans_comp->_scale)); //, 0.0f, 100.f);
			ImGui::InputFloat3("Rotation", glm::value_ptr(trans_comp->_rotation));
		}
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
			_levelEditor->LE_AddInputText("Texture", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			_levelEditor->LE_AddInputText("Model", graphics_comp->_modelFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
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
	//AABB Colider\0  OBB Collider\0  Plane Collider\0  SphereCollider\0  CapsuleCollider\0
	ImGui::Combo(" ", &item_type, "Add component\0  RigidBody\0  Audio\0  Graphics\0  Collider");

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
		case 4:
		{
			next_lol = ent.AddComponent<ColliderComponent>();
			break;
		}
		/*
		case 5:
		{
			next_lol = ent.AddComponent<AABBCollider>();
			break;
		}

		case 6:
		{
			next_lol = ent.AddComponent<OBBCollider>();
			break;
		}
		case 7:
		{
			next_lol = ent.AddComponent<PlaneCollider>();
			break;
		}
		case 8:
		{
			next_lol = ent.AddComponent<SphereCollider>();
			break;
		}
		case 9:
		{
			next_lol = ent.AddComponent<CapsuleCollider>();
			break;
		}*/
		}
		if (next_lol == nullptr)
		{
			std::cout << "Component has already been created" << std::endl;
		}
	}
}
