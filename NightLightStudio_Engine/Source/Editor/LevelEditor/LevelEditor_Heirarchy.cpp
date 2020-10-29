#include "LevelEditor_Heirarchy.h"
#include "../../Component/ComponentManager.h"
#include "../../Core/SceneManager.h"
#include "LevelEditor_ECHelper.h"

#include "../../Input/SystemInput.h"

inline size_t findCaseInsensitive(std::string data, std::string toSearch, size_t pos = 0)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
	return data.find(toSearch, pos);
}

void HeirarchyInspector::Init()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);

	selected_index = -1;
}

void HeirarchyInspector::Run()
{
	if (!hasInit)
		InitBeforeRun();

	ImGuiWindowFlags window_flags = 0;
	//ImVec4 tranform_bar = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	/*
	if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_DOWN))
	{
		if(selected_index != -1)
			LE_ECHELPER->selected_ents[selected_index] = false;

		if (selected_index < NS_SCENE::SYS_SCENE_MANAGER->EntityName.size())
			++selected_index;
		else
			selected_index = 0;

		LE_ECHELPER->selected_ents[selected_index] = true;

	}*/
	// List box
	char search_buf[256];
	static std::string search = "";
	strcpy_s(search_buf, 32, search.c_str());
	if (ImGui::InputText("Search", search_buf, 256))
		search = std::string(search_buf);

	static int index_selected = -1;
	int n = 1;
	//EntityName
	for (auto& ent : NS_SCENE::SYS_SCENE_MANAGER->EntityName)
	{
		//Check if entity is related to the search string inserted
		if (search != "" && findCaseInsensitive(ent.second.c_str(), search) == std::string::npos)
		{
			++n;
			continue;
		}

		char buf[100];
		sprintf_s(buf, "%i. %s", n, ent.second.c_str());
		//Check if any the object has been selected

		if (ImGui::Selectable(buf, LE_ECHELPER->SelectedEntities()[ent.first] ) )
		{
			LE_ECHELPER->SelectEntity(ent.first);
			std::cout << ent.first << ". has been selected: " << LE_ECHELPER->SelectedEntities()[ent.first] << std::endl;
		}
		else
		{
			//std::cout << ent.first << ". not selected: " << LE_ECHELPER->SelectedEntity()[ent.first] << std::endl;
		}

		++n;
	}
	/*
	std::vector<Entity*>& ent_vec = EntityManager::GetInstance()->GetEntityList();

	static int index_selected = -1;
	int n = 0;
	for (auto& ent : ent_vec)
	{
		if (search != "" && findCaseInsensitive(ent->GetName(), search) == std::string::npos)
		{
			++n;
			continue;
		}

		char buf[100];
		sprintf_s(buf, "%i. %s", n, ent->GetName().c_str());
		//Check if any the object has been selected
		if (ImGui::Selectable(buf, checkIfIndexExist(selected_index, n)))
		{
			if (io->KeyCtrl)
			{
				if (checkIfIndexExist(selected_index, n))
				{
					selected_index.erase(std::remove(selected_index.begin(), selected_index.end(), n), selected_index.end());
					ent_vec[n]->GetComponent<TransformComponent>().SetDisplayDebugBoxStatus(false);
				}
				else
				{
					if (std::find(selected_index.begin(), selected_index.end(), n) == selected_index.end())
						selected_index.push_back(n);
					ent_vec[n]->GetComponent<TransformComponent>().SetDisplayDebugBoxStatus(true);
					ent_selected = ent_vec[n];
					_object_selected = true;
				}
			}
			else if (io->KeyShift && index_selected != -1)
			{
				if (index_selected < n)
				{
					clearDebugStatus(ent_vec, selected_index);
					selected_index.clear();
					for (int i = index_selected; i <= n; i++)
					{
						selected_index.push_back(i);
						ent_vec[i]->GetComponent<TransformComponent>().SetDisplayDebugBoxStatus(true);
					}
					ent_selected = ent_vec[n];
					_object_selected = true;
				}
				else if (index_selected >= n)
				{
					clearDebugStatus(ent_vec, selected_index);
					selected_index.clear();
					for (int i = n; i <= index_selected; i++)
					{
						selected_index.push_back(i);
						ent_vec[i]->GetComponent<TransformComponent>().SetDisplayDebugBoxStatus(true);
					}
					ent_selected = ent_vec[index_selected];
					_object_selected = true;
				}
			}
			else
			{
				index_selected = n;
				clearDebugStatus(ent_vec, selected_index);
				selected_index.clear();
				selected_index.push_back(n);
				ent_selected = ent_vec[n];
				ent_selected->GetComponent<TransformComponent>().SetDisplayDebugBoxStatus(true);
				_object_selected = true;
			}
			//Selected object

			CameraSystem::GetInstance()->GetCurrentActiveCamera()->SetTarget(ent_selected);

			selected_child = nullptr;
		}

		++n;
	}
	/*
	if (0)//(Input::GetInstance()->(127))//delete
	{
		//EntityManager::GetInstance().del
		//std::cout << "Delete is here" << std::endl;
	}*/
}

void HeirarchyInspector::Exit()
{
	//Exit not called?
	//LE_ECHELPER->DestroyInstance();
}

void HeirarchyInspector::InitBeforeRun()
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


