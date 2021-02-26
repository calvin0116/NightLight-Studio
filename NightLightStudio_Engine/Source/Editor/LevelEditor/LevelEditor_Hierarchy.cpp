#include "LevelEditor_Hierarchy.h"

#include "../../Component/ComponentManager.h"
#include "../../Core/SceneManager.h"
#include "LevelEditor_ECHelper.h"
#include "../../Component/Components.h"

#include "../../Input/SystemInput.h"
#include "../../Graphics/CameraSystem.h"
#include "LevelEditor_Inspector.h"
#include "../../Core/TagHandler.h"

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
			
		});

	// Delete Object
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("DELETE_OBJECT", SystemInput_ns::IKEY_CTRL, "DELETE", SystemInput_ns::OnHold,
		[this]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_DELETE))
			{
				DeleteObjects();
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

	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("HIERARCHY_MOVE_UP", SystemInput_ns::IKEY_UP, "UP", SystemInput_ns::OnPress,
		[this]()
		{
			int selectedEntity = LE_ECHELPER->GetSelectedEntityID();
			if (selectedEntity != -1)
			{
				if (selectedEntity > 1000000)
					LE_ECHELPER->SelectEntity(selectedEntity - 1, false);
			}
		});

	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("HIERARCHY_MOVE_DOWN", SystemInput_ns::IKEY_DOWN, "DOWN", SystemInput_ns::OnPress,
		[this]()
		{
			int selectedEntity = LE_ECHELPER->GetSelectedEntityID();
			if (selectedEntity != -1)
			{
				if (G_ECMANAGER->getEntity(selectedEntity + 1).getComponent<TransformComponent>())
					LE_ECHELPER->SelectEntity(selectedEntity + 1, false);
			}
		});

	_sortType = (int)_hieState;
}

void HierarchyInspector::Run()
{
	if (!_allowCopy && !_levelEditor->LE_AccessWindowFunc("Inspector", &InspectorWindow::GetIfGizmoManipulate))
	{
		_allowCopy = true;
	}

	//Add entity button
	if (ImGui::Button("Add Entity"))
	{
		//Add entity with default having a transform component
		TransformComponent tran;
		G_ECMANAGER->BuildEntity().AttachComponent(tran);

	}

	ImGui::SameLine();
	
	if (ImGui::Button("Delete Entity"))
	{
		if (!ImGui::IsAnyItemFocused())
			for (Entity ent : G_ECMANAGER->getEntityContainer())
				if (LE_ECHELPER->SelectedEntities()[ent.getId()])
				{
					ComponentLight* lightcomp = ent.getComponent<LightComponent>();

					if (lightcomp)
					{
						lightcomp->SetType(NS_GRAPHICS::Lights::INVALID_TYPE);
					}

					G_ECMANAGER->FreeEntity(ent.getId());
					break;
				}
	}

	if (_levelEditor->LE_AddCombo("##SORTBY", _sortType,
		{
			"No Sorting",
			"Sort by tag",
			"Sort by alphabetic order"
		})
		)
	{
		_hieState = (HIER_STATE)_sortType;
		switch (_hieState)
		{
			case HS_SORTBYALPHA:
			{
				ent_list_to_display.clear();
				for (Entity ent : G_ECMANAGER->getEntityContainer())
				{
					ent_list_to_display.push_back(ent);
				}

				std::sort(ent_list_to_display.begin(), ent_list_to_display.end(),
					[](Entity& ent_1, Entity& ent_2)
					{
						std::string name_1 = G_ECMANAGER->EntityName[ent_1.getId()];
						std::string name_2 = G_ECMANAGER->EntityName[ent_2.getId()];
						return name_1 < name_2;
					}
				);

				break;
			}
		}
	}
	// List box
	_levelEditor->LE_AddInputText("Search", _search, 256, 0);

	// Entity list
	static int index_selected = -1;
	int n = 1;
	
	switch (_hieState)
	{ 
		case HS_NORMAL:
		{
			for (Entity ent : G_ECMANAGER->getEntityContainer())
			{
				EntityFunction(ent, n);
				++n;
			}
			break;
		}
		case HS_SORTBYTAG:
		{
			for (const int i : TAG_HANDLER->GetTagUsed())
			{
				std::string header_with_index = "Tag " + std::to_string(i);
				if (ImGui::CollapsingHeader(header_with_index.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					for (Entity ent : G_ECMANAGER->getEntityContainer())
					{
						EntityFunction(ent, n, i);
						++n;
					}
				}
			}
			break;
		}
		case HS_SORTBYALPHA:
		{
			for (Entity& ent : ent_list_to_display)
			{
				EntityFunction(ent, n);
				++n;
			}
			break;
		}
	}
	
	if (_scrollBottom)
	{
		ImGui::SetScrollHere(1.0f);
		_scrollBottom = false;
	}
}

void HierarchyInspector::GameExit()
{
	ent_list_to_display.clear();
}

void HierarchyInspector::Exit()
{
}

void HierarchyInspector::InitBeforeRun()
{
#ifdef _EDITOR
	int n = 1;
	//EntityName
	for (auto& ent : G_ECMANAGER->EntityName)
	{
		//Set level editor id to be able to get Object id
		LE_ECHELPER->LevelEditorId_EntityIdMap()[n] = ent.first;

		//Set Selected for all to false at start
		LE_ECHELPER->InsertEntityForSelection(ent.first);
		++n;
	}
#endif // _EDITOR

}

void HierarchyInspector::EntityFunction(Entity& ent, int& index, int tag_of_ent)
{
	// For searching
	std::string ent_name = G_ECMANAGER->EntityName[ent.getId()];
	//Check if entity is related to the search string inserted
	if (_search != "" && 
		findCaseInsensitive(ent_name, _search) == std::string::npos 
		)
	{
		return;
		//++n;
		//continue;
	}

	if (tag_of_ent != -1 &&
		ent.getComponent<TransformComponent>()->_tag != tag_of_ent)
	{
		return;
	}
	
	// Print out entity
	//char buf[100];
	//sprintf_s(buf, "%i. %s", n, ent_name.c_str());

	//e.g 1. name -> for ease to read and different naming in level editor selection
	std::string name_with_index = ent_name + "##" +std::to_string(index) ; //+". "

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
		_levelEditor->LE_AddSelectable(name_with_index, LE_ECHELPER->SelectedEntities()[ent.getId()],
			[&ent, this]()
			{
				bool multi = false;
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_CTRL))
				{
					multi = true;
				}
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_SHIFT))
				{
					// Selects all objects between smallest to selected object
					multi = true;
					int smallest = 0;
					std::map<int, bool> map = LE_ECHELPER->SelectedEntities();
					for (std::map<int, bool>::iterator i = map.begin(); i != map.end(); ++i)
					{
						if (i->second)
						{
							smallest = i->first;
							break;
						}
					}
					for (int i = smallest + 1; i < ent.getId(); ++i)
					{
						LE_ECHELPER->SelectEntity(i, multi);
					}
				}

				// Select if Deselected
				if (!LE_ECHELPER->SelectedEntities()[ent.getId()])
					LE_ECHELPER->SelectEntity(ent.getId(), multi);
				else
				{
					// If Selected, Deselect if different
					if (LE_ECHELPER->GetSelectedEntityID() != ent.getId())
					{
						LE_ECHELPER->DeSelectEntity(ent.getId());
					}
					else
					{
						// Change to another object to deselect if there is another that exists
						// if no other object, ignore
						int anotherID = -1;
						std::map<int, bool> anotherMap = LE_ECHELPER->SelectedEntities();
						for (std::map<int, bool>::iterator j = anotherMap.begin(); j != anotherMap.end(); ++j)
						{
							if (j->second)
							{
								anotherID = j->first;
								break;
							}
						}
						if (anotherID != -1)
						{
							LE_ECHELPER->DeSelectEntity(ent.getId());
							LE_ECHELPER->SelectEntity(anotherID, multi);
						}

					}
				}
				std::cout << ent.getId() << ". has been selected: " << LE_ECHELPER->SelectedEntities()[ent.getId()] << std::endl;

				if (ImGui::IsMouseDoubleClicked(0))
				{
					TransformComponent* trans_comp = ent.getComponent<TransformComponent>();
					if (trans_comp != NULL)
					{
						NS_GRAPHICS::Camera& cam = NS_GRAPHICS::CameraSystem::GetInstance().GetCamera();
						glm::vec3 camFront = cam.GetFront();
						camFront *= glm::vec3{ _zoomDist, _zoomDist, _zoomDist };
						cam.SetCameraPosition(trans_comp->_position - camFront);
						NS_GRAPHICS::CameraSystem::GetInstance().ForceUpdate();
					}
				}
			}, ImGuiSelectableFlags_AllowDoubleClick);

		// Drag and Drop the entity's pointer
		_levelEditor->LE_AddDragDropSource("HIERARCHY_ENTITY_OBJECT", &ent,
			[&]()
			{
				_levelEditor->LE_AddText(ent_name);
			});
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

}

void HierarchyInspector::CopyObjects()
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
			newName = G_ECMANAGER->EntityName[ent.getId()];
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
					for (auto entsName : G_ECMANAGER->EntityName)
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
			bool multi = false;
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_CTRL))
			{
				multi = true;
			}

			LE_ECHELPER->SelectEntity(newEnt.getId(), multi);

			_scrollBottom = true;
		}
	}

	_allowCopy = aCopy;
}

void HierarchyInspector::DeleteObjects()
{
	if (!ImGui::IsAnyItemFocused())
	{
		for (Entity ent : G_ECMANAGER->getEntityContainer())
		{
			if (LE_ECHELPER->SelectedEntities()[ent.getId()])
			{
				ComponentLight* lightcomp = ent.getComponent<LightComponent>();

				if (lightcomp)
				{
					lightcomp->SetType(NS_GRAPHICS::Lights::INVALID_TYPE);
				}

				G_ECMANAGER->FreeEntity(ent.getId());
			}
		}
	}
}


