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
		ImGui::Text( std::to_string(LE_ECHELPER->GetSelectedEntityID()).c_str() );
		// ============================================== END OF LINE =========================
		//=========================Example for component layouut in inspected
		TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
		if (trans_comp != NULL)
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				//float* rotation = &ent_selected->GetComponent<TransformComponent>().GetRotation().m[2];

				ImGui::InputFloat3("Position", glm::value_ptr(trans_comp->_position) ); //,-100.f, 100.f); // Edit 3 floats representing a color
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
          char buf[512];
          char buf2[256];
          strcpy_s(buf, 512, path.c_str());
          strcpy_s(buf2, 256, name.c_str());
          ImGui::InputText("", buf, 512);
          ImGui::SameLine(0, 10);
          ImGui::InputText("", buf2, 256);
        }
      }
    }
	}
}
