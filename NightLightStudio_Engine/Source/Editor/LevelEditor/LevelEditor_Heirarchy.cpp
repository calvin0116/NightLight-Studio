#include "LevelEditor_Heirarchy.h"
#include "../../Component/ComponentManager.h"

void HeirarchyInspector::Init()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);
}

void HeirarchyInspector::Run()
{
	ImGuiWindowFlags window_flags = 0;
	ImVec4 tranform_bar = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// List box

	char search_buf[256];
	static std::string search = "";
	strcpy_s(search_buf, 32, search.c_str());
	if (ImGui::InputText("Search", search_buf, 256))
		search = std::string(search_buf);

	static int index_selected = -1;
	int n = 1;
	//EntityName
	for (auto& ent : EntityName)
	{
		char buf[100];
		sprintf_s(buf, "%i. %s", n, ent.second.c_str());
		//Check if any the object has been selected
		
		if (ImGui::Selectable(buf, true))
		{

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
}
