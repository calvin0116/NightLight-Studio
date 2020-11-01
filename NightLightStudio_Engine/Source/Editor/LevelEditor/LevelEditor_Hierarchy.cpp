#include "LevelEditor_Hierarchy.h"

#include "../../Component/ComponentManager.h"
#include "../../Core/SceneManager.h"
#include "LevelEditor_ECHelper.h"
#include "../../Component/Components.h"

#include "../../Input/SystemInput.h"


inline size_t findCaseInsensitive(std::string data, std::string toSearch, size_t pos = 0)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
	return data.find(toSearch, pos);
}

void HierarchyInspector::Init()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);

	selected_index = -1;
}

void HierarchyInspector::Run()
{
	if (!hasInit)
		InitBeforeRun();

	ImGuiWindowFlags window_flags = 0;
	ImVec4 tranform_bar = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//Delete Entity
	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_DELETE))
	{
		for (Entity ent : G_ECMANAGER->getEntityContainer())
		{
			if (LE_ECHELPER->SelectedEntities()[ent.getId()])
			{
				G_ECMANAGER->FreeEntity(ent.getId());
			}
		}
	}
	//Add entity button
	if (ImGui::Button("Add Entity"))
	{
		//Add entity with default having a transform component
		TransformComponent tran;
		G_ECMANAGER->BuildEntity().AttachComponent(tran);

	}
	// List box
	char search_buf[256];
	static std::string search = "";
	strcpy_s(search_buf, 32, search.c_str());
	if (ImGui::InputText("Search", search_buf, 256))
		search = std::string(search_buf);

	// Entity list
	static int index_selected = -1;
	int n = 1;

	for (Entity ent : G_ECMANAGER->getEntityContainer())
	{
		// For searching
		std::string& ent_name = NS_SCENE::SYS_SCENE_MANAGER->EntityName[ent.getId()];
		//Check if entity is related to the search string inserted
		if (search != "" && findCaseInsensitive(ent_name, search) == std::string::npos)
		{
			++n;
			continue;
		}

		// Print out entity
		char buf[100];
		sprintf_s(buf, "%i. %s", n, ent_name.c_str());

		//If more then one child, get the child
		if (ent.getNumChildren() > 0)
		{
			//~~!Prep for entity with children
			if (ImGui::TreeNode(buf))
			{
				//~~!!Get child to be printed out as selectable
				ImGui::TreePop();
			}
		}
		else //if (!ent.isChild) print those not a child
		{
			if (ImGui::Selectable(buf, LE_ECHELPER->SelectedEntities()[ent.getId()]))
			{
				LE_ECHELPER->SelectEntity(ent.getId());
				std::cout << ent.getId() << ". has been selected: " << LE_ECHELPER->SelectedEntities()[ent.getId()] << std::endl;
			}
			else
			{
				//std::cout << ent.first << ". not selected: " << LE_ECHELPER->SelectedEntity()[ent.first] << std::endl;
			}
		}


		++n;
	}
}

void HierarchyInspector::Exit()
{
	//Exit not called?
	//LE_ECHELPER->DestroyInstance();
}

void HierarchyInspector::InitBeforeRun()
{
#ifdef _EDITOR
	hasInit = true;

	int n = 1;
	//EntityName
	for (auto& ent : NS_SCENE::SYS_SCENE_MANAGER->EntityName)
	{
		//Set level editor id to be able to get Object id
		LE_ECHELPER->LevelEditorId_EntityIdMap()[n] = ent.first;

		//Set Selected for all to false at start
		LE_ECHELPER->InsertEntityForSelection(ent.first);
		++n;
	}
#endif // _EDITOR

}

