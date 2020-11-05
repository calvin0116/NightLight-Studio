#include "LevelEditor_Hierarchy.h"

#include "../../Component/ComponentManager.h"
#include "../../Core/SceneManager.h"
#include "LevelEditor_ECHelper.h"
#include "../../Component/Components.h"

#include "../../Input/SystemInput.h"
#include "../../Graphics/CameraSystem.h"
#include "LevelEditor_Inspector.h"


inline size_t findCaseInsensitive(std::string data, std::string toSearch, size_t pos = 0)
{
	std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) {return (unsigned char)::tolower(c); });
	std::transform(toSearch.begin(), toSearch.end(), toSearch.begin(), [](unsigned char c) {return (unsigned char)::tolower(c); });
	return data.find(toSearch, pos);
}

void HierarchyInspector::Start()
{
	InitBeforeRun();

	// Ctrl-Copy object selected
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("COPY_OBJECT", SystemInput_ns::IKEY_CTRL, "IDET", SystemInput_ns::OnHold,
		[this]()
		{
			bool copyObj = false;
			bool aCopy = _allowCopy;
			if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_C))
			{
				// If any item is focused, no copy
				if (!ImGui::IsAnyItemFocused())
					if (LE_ECHELPER->GetSelectedEntityID() != -1)
						copyObj = true;
			}
			// CTRL - MOVE GIZMO
			else
			{
				if (LE_ECHELPER->GetSelectedEntityID() != -1)
					if (_allowCopy && _levelEditor->LE_AccessWindowFunc("Inspector", &InspectorWindow::GetIfGizmoManipulate))
					{
						copyObj = true;
						aCopy = false;
					}
				
			}

			if (copyObj && _allowCopy)
			{
				Entity ent = G_ECMANAGER->getEntity(LE_ECHELPER->GetSelectedEntityID());
				TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
				if (trans_comp != NULL)
				{
					std::string newName;
					newName = NS_SCENE::SYS_SCENE_MANAGER->EntityName[ent.getId()];
					if (newName == "")
						newName = "Entity_";
					else
					{
						// Really annoying name check to determine what number to append to the name
						int num = 1;
						bool reset = false;

						std::string actualName = newName;
						int digits = 0;
						for (int i = (int)actualName.size() - 1; i >= 0; --i)
						{
							if (isdigit(actualName[i]))
								digits++;
							else
							{
								if (actualName[i] == '_')
								{
									reset = true;
									actualName = actualName.substr(0, actualName.size() - digits - 1);
									break;
								}
								else
								{
									newName.append("_1");
									break;
								}
							}
						}

						while (reset)
						{
							reset = false;
							std::string tempName = actualName;
							tempName.append("_" + std::to_string(num));
							for (auto entsName : NS_SCENE::SYS_SCENE_MANAGER->EntityName)
							{
								if (entsName.second.rfind(tempName) != std::string::npos)
								{
									num++;
									reset = true;
								}
							}
							if (!reset)
								newName = tempName;
						}
					}

					// Causes memory leaks currently (Probably Graphics Side)
					Entity newEnt = ent.Copy(G_ECMANAGER, newName);
					LE_ECHELPER->SelectEntity(newEnt.getId());

					_scrollBottom = true;
				}
			}

			_allowCopy = aCopy;
		});

	// Delete Object
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("DELETE_OBJECT", SystemInput_ns::IKEY_CTRL, "DELETE", SystemInput_ns::OnHold,
		[]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_DELETE))
				if (!ImGui::IsAnyItemFocused())
					for (Entity ent : G_ECMANAGER->getEntityContainer())
						if (LE_ECHELPER->SelectedEntities()[ent.getId()])
						{
							G_ECMANAGER->FreeEntity(ent.getId());
							break;
						}
		});

	// PRESS F TO FOCUS ON SELECTED ITEM
	// Honestly doesn't even need to be here, can be pretty much anywhere
	/*
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("FOCUS_CAMERA", SystemInput_ns::IKEY_F, "IDET", SystemInput_ns::OnPress,
		[]()
		{
			if (LE_ECHELPER->GetSelectedEntityID() != -1)
			{
				Entity ent = G_ECMANAGER->getEntity(LE_ECHELPER->GetSelectedEntityID());
				TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
				if (trans_comp != NULL)
				{
					NS_GRAPHICS::Camera& cam = NS_GRAPHICS::CameraSystem::GetInstance().GetCamera();
					glm::vec3 camFront = cam.GetFront();
					const float dist = 10.0f;
					camFront *= glm::vec3{ dist, dist, dist };
					cam.SetCameraPosition(trans_comp->_position - camFront);
					NS_GRAPHICS::CameraSystem::GetInstance().ForceUpdate();
				}
			}
		});
*/
// Object Selection
/*
SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOUSE_CLICK_SELECT_OBJ", SystemInput_ns::IMOUSE_LBUTTON, "CLICK", SystemInput_ns::OnPress,
	[this]()
	{
		if (!ImGui::IsAnyItemHovered())
		{

		}
	});
	*/
}

void HierarchyInspector::Init()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);

	selected_index = -1;
}

void HierarchyInspector::Run()
{
	if (!_allowCopy && !_levelEditor->LE_AccessWindowFunc("Inspector", &InspectorWindow::GetIfGizmoManipulate))
	{
		_allowCopy = true;
	}

	//ImGuiWindowFlags window_flags = 0;
	//ImVec4 tranform_bar = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//Add entity button
	if (ImGui::Button("Add Entity"))
	{
		//Add entity with default having a transform component
		TransformComponent tran;
		G_ECMANAGER->BuildEntity().AttachComponent(tran);

	}
	// List box
	/*
	char search_buf[256];
	static std::string search = "";
	strcpy_s(search_buf, 32, search.c_str());
	if (ImGui::InputText("Search", search_buf, 256))
		search = std::string(search_buf);
		*/

	_levelEditor->LE_AddInputText("Search", _search, 256, 0);

	// Entity list
	static int index_selected = -1;
	int n = 1;

	for (Entity ent : G_ECMANAGER->getEntityContainer())
	{
		// For searching
		std::string& ent_name = NS_SCENE::SYS_SCENE_MANAGER->EntityName[ent.getId()];
		//Check if entity is related to the search string inserted
		if (_search != "" && findCaseInsensitive(ent_name, _search) == std::string::npos)
		{
			++n;
			continue;
		}

		// Print out entity
		//char buf[100];
		//sprintf_s(buf, "%i. %s", n, ent_name.c_str());

		//If more then one child, get the child
		if (ent.getNumChildren() > 0)
		{
			//~~!Prep for entity with children
			_levelEditor->LE_AddTreeNodes(ent_name,
				[]()
				{

				});
			/*
			if (ImGui::TreeNode(buf))
			{
				//~~!!Get child to be printed out as selectable
				ImGui::TreePop();
			}
			*/
		}
		else //if (!ent.isChild) print those not a child
		{
			_levelEditor->LE_AddSelectable(ent_name, LE_ECHELPER->SelectedEntities()[ent.getId()],
				[&ent]()
				{
					LE_ECHELPER->SelectEntity(ent.getId());
					std::cout << ent.getId() << ". has been selected: " << LE_ECHELPER->SelectedEntities()[ent.getId()] << std::endl;

					if (ImGui::IsMouseDoubleClicked(0))
					{
						TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
						if (trans_comp != NULL)
						{
							NS_GRAPHICS::Camera& cam = NS_GRAPHICS::CameraSystem::GetInstance().GetCamera();
							glm::vec3 camFront = cam.GetFront();
							const float dist = 10.0f;
							camFront *= glm::vec3{ dist, dist, dist };
							cam.SetCameraPosition(trans_comp->_position - camFront);
							NS_GRAPHICS::CameraSystem::GetInstance().ForceUpdate();
						}
					}
				}, ImGuiSelectableFlags_AllowDoubleClick);
			/*
			if (ImGui::Selectable(buf, LE_ECHELPER->SelectedEntities()[ent.getId()]))
			{
				LE_ECHELPER->SelectEntity(ent.getId());
				std::cout << ent.getId() << ". has been selected: " << LE_ECHELPER->SelectedEntities()[ent.getId()] << std::endl;
			}
			else
			{
				//std::cout << ent.first << ". not selected: " << LE_ECHELPER->SelectedEntity()[ent.first] << std::endl;
			}
			*/
		}

		++n;
	}

	if (_scrollBottom)
	{
		ImGui::SetScrollHere(1.0f);
		_scrollBottom = false;
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


