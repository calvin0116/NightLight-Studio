#include "LevelEditor.h"
#include <iostream>

std::string GetFileType(const std::string& path)
{
    char sep = '.';
    size_t i = path.rfind(sep, path.length());
    if (i != std::string::npos) {
        return(path.substr(i + 1, path.length() - i));
    }

    return {};
}

void LevelEditor::LE_MainMenuBar()
{
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags menuBar_winFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;
    menuBar_winFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    //************************************************* Main Menu Bar **************************************************//
    {
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(ImVec2(viewport->GetWorkSize().x, 0));

        // Main Menu Bar
        ImGui::Begin("Main Menu Bar", nullptr, menuBar_winFlags);

        if (ImGui::BeginMenuBar())
        {
            LE_AddMenuWithItems("File", { "Open" });
            LE_AddMenuWithItems("Edit", { "Undo" }, { "Ctrl-Z" });

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

            ImVec2 size = viewport->GetWorkSize();
            ImGui::SetCursorPosX(size.x / 2.0f);
            LE_AddCheckbox("PLAY##MMBAR", &_runEngine, []()
                {
                    // START/STOP ENGINE RUN HERE
                });

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
    //************************************************* Dockspace Area **************************************************//
    {
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::Begin("DockSpace Demo", nullptr, window_flags);

        // Used to accept DragDrops
        ImGui::BeginChild("DockSpace Child", ImVec2(0, 0), false, window_flags);

        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        // Sets dockspace for other objects
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        }
        ImGui::EndChild();

        // Actual site to implement what to do with the filepath obtained from drag drop
        LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
            [this](std::string* str)
            {
                std::cout << (*str) << std::endl;
                if (GetFileType(*str) == "fbx")
                {

                }
            });
        ImGui::End();
    }
}
