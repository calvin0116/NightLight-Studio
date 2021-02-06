#include "LevelEditor.h"
#include <iostream>
#include "../../IO/Json/Parser.h"
#include "LevelEditor_AssetInsp.h"
#include "../../Graphics/GraphicsSystem.h"
#include "../../Component/Components.h"
#include "LevelEditor_Console.h"
#include "WindowsDialogBox.h"
#include "../../Graphics/DebugManager.h"

#include "../../Core/SceneManager.h"
#include "../PrefabManager.h"
#include "LevelEditor_ECHelper.h"

#include "../../Window/WndSystem.h"

// For message
#include "..\..\Messaging\SystemBroadcaster.h"
#include "..\..\Messaging\Messages\MessageTogglePlay.h"

static bool winFocus = false;

void LevelEditor::LE_ConfigLoad()
{

}

void LevelEditor::LE_MainMenuBar()
{
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollbar;// | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (winFocus)
    {
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    //************************************************* Dockspace Area **************************************************//
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);

    ImGui::PopStyleVar();

    if (ImGui::BeginMenuBar())
    {
        LE_AddMenuWithItems("File", 
            { "New" , "Open" , "Save", "Save As"},
            { "" , "" , "Ctrl-S", "" },
            {
                [this]()
                {
                    NS_SCENE::SYS_SCENE_MANAGER->SetNextScene(NS_SCENE::SYS_SCENE_MANAGER->LoadScene("DefaultScene"));
                },
                // Opens a Window Dialog Box for Opening a .json file
                [this]() 
                {
                    COMDLG_FILTERSPEC rgSpec[] =
                    {
                        { L"*.json", L"*.json" }
                    };
                    // Gets the RELATIVE File Path to Open from
                    std::string fileToOpen = WindowsOpenFileBox(_window, rgSpec, 1);
                    std::cout << fileToOpen << std::endl;
                    if (fileToOpen != "")
                        NS_SCENE::SYS_SCENE_MANAGER->SetNextScene(fileToOpen);
                },

                []() 
                {
                    NS_SCENE::SYS_SCENE_MANAGER->SaveScene(); 
                },

                // Opens a Window Dialog Box for Saving a .json file
                [this]()
                {
                    COMDLG_FILTERSPEC rgSpec[] =
                    {
                        { L"*.json", L"*.json" }
                    };
                    // Gets the RELATIVE File Path to Save to
                    std::string fileToSaveTo = WindowsSaveFileBox(_window, rgSpec, 1);
                    std::cout << fileToSaveTo << std::endl;
                    if (fileToSaveTo != "")
                        NS_SCENE::SYS_SCENE_MANAGER->SaveScene(fileToSaveTo);
                }
            });
        LE_AddMenuWithItems("Edit", 
            { "Undo", "Redo" }, 
            { "Ctrl-Z", "Ctrl-Y" }, 
            { 
                [this]() { LE_AccessWindowFunc("Console", &ConsoleLog::UndoLastCommand); },
                [this]() { LE_AccessWindowFunc("Console", &ConsoleLog::RedoLastCommand); }
            });

        if (NS_WINDOW::SYS_WINDOW->GetWindowedMode())
        {
            LE_AddMenuWithItems("Options",
                { "FullScreen" },
                { "", "" },
            {
                 []() { NS_WINDOW::SYS_WINDOW->SetFullScreenMode(true); }
            });
        }
        else
        {
            LE_AddMenuWithItems("Options",
                { "Windowed" },
                { "", "" },
            {
                []() { NS_WINDOW::SYS_WINDOW->SetFullScreenMode(false); }
            });
        }



        std::vector<std::string> editorNames;
        std::vector<std::function<void()>> funcs;
        for (unsigned i = 0; i < _editorWind.size(); ++i)
        {
            editorNames.push_back(_editorWind[i]._name);
            funcs.push_back([this, i]()
                {
                    (_editorWind[i]._isOpen) = true;
                });
        }
        LE_AddMenuWithItems("Windows", editorNames, {}, funcs);
        //LE_AddMenuOnly("Windows", [&](){ LE_AddMenuWithItems("SecondWindows", editorNames, {}, funcs);});

        LE_AddText("Current Scene: ~" + NS_SCENE::SYS_SCENE_MANAGER->GetCurrentScene() + "~ 'Ctrl+1' for next scene");

        ImVec2 size = viewport->GetWorkSize();
        ImGui::SetCursorPosX(size.x / 2.0f);
		
		/*
        LE_AddCheckbox("PLAY##MMBAR", &CONFIG_DATA->GetConfigData().isPlaying, []()
            {
                // START/STOP ENGINE RUN HERE
                std::cout << "Play button: " << CONFIG_DATA->GetConfigData().isPlaying << std::endl;
*/
        LE_AddCheckbox("PLAY##MMBAR", &_runEngine, [this]()
            {
              // START/STOP ENGINE RUN HERE
                MessageTogglePlay isPlaying(_runEngine, "TogglePlay");
                if (_runEngine)
                {
                    MessageTogglePlay isPlaying_1(_runEngine, "BeforePlay");
                    NS_GRAPHICS::CameraSystem::GetInstance().SavePosition();
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying_1);
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying);

                }
                else
                {
                    MessageTogglePlay isPlaying_2(_runEngine, "AfterPlay");
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying);
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying_2);
                    NS_GRAPHICS::CameraSystem::GetInstance().MoveToSavedPosition();
                }
                DELTA_T->load();
            });

        ImGui::EndMenuBar();
    }

    // Used to accept DragDrops
    ImGui::BeginChild("DockSpace Child", ImVec2(0, 0), false, window_flags);

    // Sets dockspace for other objects
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    }

    // Grid Control
    if (ImGui::BeginMenuBar())
    {
        NS_GRAPHICS::DebugManager& dm = NS_GRAPHICS::DebugManager::GetInstance();
        //ImGui::SetCursorPosX(viewport->GetWorkSize().x / 10.0f);
        ImGui::Separator();
        LE_AddCheckbox("Run Grid", &_showGrid, [this, &dm]() { dm.ShowGrid(_showGrid); });
        ImGui::Separator();
        if (_showGrid)
        {
            if (ImGui::BeginMenu("Grid Settings##GRID"))
            {
                float gridCell[2] = { dm.GetGridLength(), dm.GetCellLength() };
                glm::vec4 gridCol = dm.GetGridRGBA();
                ImGui::Text("     Grid      :     Cell    ");
                //ImGui::Separator();
                //ImGui::SetNextItemWidth(120);
                if (ImGui::InputFloat2("##GRIDGRID", gridCell, 3, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    dm.SetGridLength(gridCell[0]); dm.SetCellLength(gridCell[1]);
                    CONFIG_DATA->GetConfigData()._gridSize = gridCell[0];
                    CONFIG_DATA->GetConfigData()._cellSize = gridCell[1];
                }
                //ImGui::Separator();
                //ImGui::SetNextItemWidth(150);
                if (ImGui::ColorEdit4("##GRIDCOLOR", glm::value_ptr(gridCol)))
                {
                    dm.SetGridColor(gridCol);
                    CONFIG_DATA->GetConfigData()._gridColourRed = gridCol.r;
                    CONFIG_DATA->GetConfigData()._gridColourGreen = gridCol.g;
                    CONFIG_DATA->GetConfigData()._gridColourBlue = gridCol.b;
                    CONFIG_DATA->GetConfigData()._gridColourAlpha = gridCol.a;
                }
                ImGui::Separator();
                ImGui::EndMenu();
            }
            ImGui::Separator();
        }

        //ImGui::SetCursorPosX(viewport->GetWorkSize().x / 10.0f * 7.0f);
        if (ImGui::BeginMenu("Camera Settings##CAMERA"))
        {
            NS_GRAPHICS::CameraSystem& cs = NS_GRAPHICS::CameraSystem::GetInstance();
            float camVals[3] = { cs.GetDragSensitivity(), cs.GetRotationSensitivity(), cs.GetZoomSensitivity() };
            ImGui::Text("  Drag:");
            ImGui::SameLine();
            if (ImGui::InputFloat("##CAMDRAG", camVals, 3, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                cs.SetDragSensitivity(camVals[0]);
                CONFIG_DATA->GetConfigData()._positionSensitivity = camVals[0];
            }

            ImGui::Text("Rotate:");
            ImGui::SameLine();
            if (ImGui::InputFloat("##CAMROTATE", camVals+1, 3, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                cs.SetRotationSensitivity(camVals[1]);
                CONFIG_DATA->GetConfigData()._rotationSensitivity = camVals[1];
            }

            ImGui::Text("  Zoom:");
            ImGui::SameLine();
            if (ImGui::InputFloat("##CAMZOOM", camVals+2, 3, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                cs.SetZoomSensitivity(camVals[2]);
                CONFIG_DATA->GetConfigData()._zoomSensitivity = camVals[2];
            }
            ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::BeginMenu("Gamma Settings##GAMMA"))
        {
            float gamma = NS_GRAPHICS::LightSystem::GetInstance().GetGamma();

            // Do your stuff here
            //float gamma = 0; //Shifted to config.h
            ImGui::SetNextItemWidth(150);
            if (ImGui::DragFloat("Gamma", &gamma, 0.1f, 0.1f, 100.f))
            {
                NS_GRAPHICS::LightSystem::GetInstance().SetGamma(gamma);
                CONFIG_DATA->GetConfigData().gamma = gamma;
            }

            // Do not remove this
            ImGui::EndMenu();
        }
        ImGui::Text("FPS: %f" , ImGui::GetIO().Framerate);

        ImGui::EndMenuBar();
    }

    ImGui::EndChild();

    ImGui::PopStyleVar(2);

    // Actual site to implement what to do with the filepath obtained from drag drop
    LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
        [this](std::string* str)
        {
            std::string data = *str;
            std::transform(data.begin(), data.end(), data.begin(),
                [](unsigned char c)
                { return (char)std::tolower(c); });

            // std::cout << data << std::endl;
            // json drap and drop
            if (LE_GetFileType(data) == "json")
            {
                /*
                // Create Entity here
                std::string name = LE_EraseBackSubStr(LE_GetFilename(*str), ".json");
                std::string filepath = ".";
                filepath.append(LE_EraseSubStr(*str, LE_GetFilename(*str)));
                Parser parser(name, filepath);
                parser.Load();
                if (parser.CheckForMember("Objects"))
                {
                    
                }
                */
                data = "..//NightLightStudio_Game" + *str;
                NS_SCENE::SYS_SCENE_MANAGER->SetNextScene(data);

            }
            if (LE_GetFileType(data) == "prefab")
            {
                data = "..//NightLightStudio_Game" + *str;          //Temp solution, please help =v
                int id = PFunc::PrefabReadAndCreate(data);      //Create Prefab
                if(id != -1)
                    LE_ECHELPER->SelectEntity(id);             //Select Entity
            }


            if (LE_GetFileType(data) == "fbx" || LE_GetFileType(data) == "model")
            {
                std::string fName = LE_EraseBackSubStr(LE_GetFilename(data), std::string(".").append(LE_GetFileType(data)));
                fName = "." + fName;

                Entity ent = G_ECMANAGER->BuildEntity();
                ComponentTransform transEnt;
                transEnt._position = { 0.f, 0.f,0.f };
                ent.AttachComponent<ComponentTransform>(transEnt);

                //assimp doesnt deal with preceding slash
                if (data[0] == '\\')
                {
                    data.erase(0, 1);
                }

                NS_GRAPHICS::SYS_GRAPHICS->LoadModel(data);

                /*if (LE_GetFileType(data) == "fbx")
                {
                    //Checks for the file name
                    std::string name;
                    size_t pos = data.rfind("\\");
                    //Get just the string after the last path
                    if (pos != std::string::npos)
                    {
                        name = data.substr(pos + 1);
                    }
                    else
                    {
                        name = data;
                    }

                    pos = name.rfind(".");
                    //Trim the extension to get the file name
                    if (pos != std::string::npos)
                    {
                        name.erase(pos);
                    }

                    std::string meshName;
                    meshName = NS_GRAPHICS::s_LocalPathName + name + NS_GRAPHICS::s_ModelFileType;
                    data = meshName;
                }*/
                NS_GRAPHICS::SYS_GRAPHICS->AttachModel(ent, data);

                ComponentGraphics* graphics_comp = ent.getComponent<GraphicsComponent>();
                if (graphics_comp->_modelID >= 0)
                {
                    if (NS_GRAPHICS::ModelManager::GetInstance()._models[graphics_comp->_modelID]->_isAnimated)
                    {
                        ent.AttachComponent<ComponentAnimation>();
                        ComponentAnimation* anim = ent.getComponent<ComponentAnimation>();
                        anim->_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();
                        AnimationController* animCtrl = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID];
                        for (auto& anims : NS_GRAPHICS::ModelManager::GetInstance()._models[graphics_comp->_modelID]->_animations)
                        {
                            animCtrl->_allAnims.insert(anims.first);
                        }
                    }
                }
            }

            if (LE_GetFileType(data) == "json")
            {
                NS_SCENE::SYS_SCENE_MANAGER->SetNextScene(data);
            }
        }, ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

    if (ImGui::IsWindowFocused())
    {
       winFocus = true;
    }

    ImGui::End();
}

void LevelEditor::LE_ConfigSave()
{

}
