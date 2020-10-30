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
		/*
		// Entity's tag(s)
		std::set<std::string> tag_names;

		if (!ent_selected->HasEmptyTagList())
			tag_names = ent_selected->GetTagList();
		else
			tag_names.emplace("Untagged");

		if (ImGui::BeginCombo("Tags", (*tag_names.begin()).c_str()))
		{
			// Display list of existing tags
			for (auto itr = tag_names.begin(); itr != tag_names.end(); ++itr)
			{
				// If tag is selected, it is removed from the entity
				if (ImGui::Selectable((*itr).c_str()))
					ent_selected->RemoveTag(*itr);
			}

			ImGui::Separator();

			// Offer option to create a new tag
			if (ImGui::Button("Add Tag..."))
				ImGui::OpenPopup("Add a New Tag");

			// Creates a small window which handles creation of a new tag

			if (ImGui::BeginPopupModal("Add a New Tag", &open_tag))
			{
				ImGui::InputText("Tag name", LEVEL_EDITOR_VARIABLES::input_tag_buf, sizeof(LEVEL_EDITOR_VARIABLES::input_tag_buf));

				ImGui::Dummy(ImVec2(0, 5));

				if (ImGui::Button("Create Tag"))
				{
					ent_selected->AddTag(LEVEL_EDITOR_VARIABLES::input_tag_buf);

					for (auto& c : LEVEL_EDITOR_VARIABLES::input_tag_buf)
						c = '\0';

					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}

			ImGui::EndCombo();
		}
		*/
		/*
		// Entity's layer
		if (ImGui::BeginCombo("Layer", ent_selected->GetLayer().GetName().c_str()))
		{
			// Display list of existing layers
			for (auto itr = EntityManager::GetInstance()->GetEntityLayers().begin(); itr != EntityManager::GetInstance()->GetEntityLayers().end(); ++itr)
			{
				std::string selectable_text = std::to_string((*itr).GetPosition()) + ": " + (*itr).GetName();

				// If layer is selected in the drop-down menu, the entity's current layer will be set to it
				if (ImGui::Selectable(selectable_text.c_str()))
				{
					ent_selected->SetLayer(*itr);
					EntityManager::GetInstance()->UpdateLayersBasedOnEntity(ent_selected);
				}
			}

			ImGui::Separator();

			// Offer option to create a new tag
			if (ImGui::Button("Set Layer..."))
				ImGui::OpenPopup("Set a New Layer");

			// Creates a small window which handles creation of a new tag
			if (ImGui::BeginPopupModal("Set a New Layer", &open_layer))
			{
				ImGui::Text("Layer name: ");
				ImGui::InputText("Layer name", LEVEL_EDITOR_VARIABLES::input_layer_name_buf, sizeof(LEVEL_EDITOR_VARIABLES::input_layer_name_buf));

				ImGui::Dummy(ImVec2(0, 5));

				ImGui::Text("Layer position: ");
				ImGui::InputInt("Layer position", &LEVEL_EDITOR_VARIABLES::input_layer_position_buf);

				ImGui::Dummy(ImVec2(0, 5));

				if (ImGui::Button("Set Layer"))
				{
					ent_selected->SetLayer(LEVEL_EDITOR_VARIABLES::input_layer_name_buf, LEVEL_EDITOR_VARIABLES::input_layer_position_buf);
					EntityManager::GetInstance()->UpdateLayersBasedOnEntity(ent_selected);

					for (auto& c : LEVEL_EDITOR_VARIABLES::input_layer_name_buf)
						c = '\0';

					LEVEL_EDITOR_VARIABLES::input_layer_position_buf = 0;

					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();

				ImGui::EndPopup();
			}
			ImGui::EndCombo();
		}
		*/
		/*
				// Entity's pickable state
				ImGui::Checkbox("Pickable in Level Editor", &(ent_selected->GetPickableStatusRef()));
				ImGui::Separator();

				ImGui::Checkbox("SetActive", &(ent_selected->GetActiveStatusRef()));
				*/
				//=========================Example for component layouut in inspected
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
					_levelEditor->LE_AddInputText("##PATH", path, 500, ImGuiInputTextFlags_EnterReturnsTrue);
					ImGui::SameLine(0, 10);
					_levelEditor->LE_AddInputText("##NAME", name, 100, ImGuiInputTextFlags_EnterReturnsTrue);
				}
			}
		}
	}
}
