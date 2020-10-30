#include "LevelEditor_Inspecter.h"

#include "LevelEditor_ECHelper.h"
#include "../../Core/SceneManager.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Components.h"
#include <set>

void InspectorWindow::Run()
{
	//Check for valid Entity Id
	if (LE_ECHELPER->GetSelectedEntityID() != -1)
	{
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
		// ============================================== END OF LINE =========================

		TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
		if (trans_comp != NULL)
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				//float* rotation = &ent_selected->GetComponent<TransformComponent>().GetRotation().m[2];

				ImGui::InputFloat3("Position", glm::value_ptr(trans_comp->_position)); //,-100.f, 100.f); // Edit 3 floats representing a color
				ImGui::InputFloat3("Scale", glm::value_ptr(trans_comp->_scale)); //, 0.0f, 100.f);
				ImGui::InputFloat("Rotation Z", glm::value_ptr(trans_comp->_rotation));
			}
		}

		ComponentCollider* col_comp = ent.getComponent<ComponentCollider>();
		if (col_comp != NULL)
		{
			//~~! Need Help==//
			//1. Check collider type that it have
			//2. Get the right collider type
			//3. Insert name
			std::string name = "AABBCollider";	//e.g.
			//4. May need loop to loop through all collider
			if (ImGui::CollapsingHeader(name.c_str()))
			{

			}
		}

		ComponentLoadAudio* aud_manager = ent.getComponent<ComponentLoadAudio>();
		if (aud_manager != nullptr)
		{

			if (ImGui::CollapsingHeader("Audio Manager"))
			{
				if (ImGui::Button("Add Audio"))
				{
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
		}


		GraphicsComponent* graphics_comp = ent.getComponent<GraphicsComponent>();
		if (graphics_comp != nullptr)
		{
			if (ImGui::CollapsingHeader("Graphics component"))
			{
				_levelEditor->LE_AddInputText("##GRAPHICS_1", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
				//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			}
		}

		RigidBody* rb = ent.getComponent<RigidBody>();
		if (rb != nullptr)
		{
			if (ImGui::CollapsingHeader("Rigid Body"))
			{
				//_levelEditor->LE_AddInputText("##GRAPHICS_1", graphics_comp->_textureFileName, 500, ImGuiInputTextFlags_EnterReturnsTrue);
				//_levelEditor->LE_AddInputText("##GRAPHICS_2", graphics_comp->, 500, ImGuiInputTextFlags_EnterReturnsTrue);
			}
		}

		static int item_type = 0;
		ImGui::Combo(" ", &item_type, "Add component\0  Collider\0  RigidBody\0  Audio\0 Graphics\0");

		void* next_lol = nullptr;

		if (ImGui::Button("Add Selected Component"))
		{
			switch (item_type)
			{
				case 1:
				{
					next_lol = ent.AddComponent<ColliderComponent>();
					break;
				}
				case 2:
				{
					next_lol = ent.AddComponent<RigidBody>();
					break;
				}
				case 3:
				{
					next_lol = ent.AddComponent<AudioComponent>();
					break;
				}
				case 4:
				{
					next_lol = ent.AddComponent<AudioComponent>();
					break;
				}
				case 5:
				{
					next_lol = ent.AddComponent<GraphicsComponent>();
					break;
				}
			}

			if (next_lol == nullptr)
			{
				std::cout << "Component has already been created" << std::endl;
			}
		}



	}
}
