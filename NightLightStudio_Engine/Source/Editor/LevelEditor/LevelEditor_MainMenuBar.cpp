#include "LevelEditor.h"
#include <iostream>
#include "../../IO/Json/Parser.h"
#include "LevelEditor_AssetInsp.h"
#include "../../Graphics/GraphicsSystem.h"
#include "../../Component/Components.h"
#include "LevelEditor_Console.h"
#include "WindowsDialogBox.h"

#include "../../Core/SceneManager.h"

void LevelEditor::LE_MainMenuBar()
{
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;

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

    ImGui::Begin("DockSpace Demo", nullptr, window_flags | ImGuiWindowFlags_MenuBar);

    ImGui::PopStyleVar();

    if (ImGui::BeginMenuBar())
    {
        LE_AddMenuWithItems("File", 
            { "Open" , "Save", "Save Test"},
            { "" , "Ctrl-S", "" },
            {
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

        LE_AddMenuWithItems("Current Scene: ~" + NS_SCENE::SYS_SCENE_MANAGER->GetCurrentScene() + "~ Press 'Ctrl+1' to go next scene");

        ImVec2 size = viewport->GetWorkSize();
        ImGui::SetCursorPosX(size.x / 2.0f);
        LE_AddCheckbox("PLAY##MMBAR", &_runEngine, []()
            {
                // START/STOP ENGINE RUN HERE
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
            }

            if (LE_GetFileType(data) == "fbx")
            {
                std::string fName = LE_EraseBackSubStr(LE_GetFilename(data), std::string(".").append(LE_GetFileType(data)));
                fName = "." + fName;

                Entity ent = G_ECMANAGER->BuildEntity();
                ComponentTransform transEnt;
                transEnt._position = { 0.f, 0.f,0.f };
                ent.AttachComponent<ComponentTransform>(transEnt);

                // Causes memory leaks - pls fix calvin
                NS_GRAPHICS::SYS_GRAPHICS->LoadModel(data);
                NS_GRAPHICS::SYS_GRAPHICS->AttachModel(ent, fName);
            }
        }, ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
    ImGui::End();
}
