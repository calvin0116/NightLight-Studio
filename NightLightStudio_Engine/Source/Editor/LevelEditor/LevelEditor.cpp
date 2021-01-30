#include "LevelEditor.h"
#include "LevelEditor_Console.h"
#include "LevelEditor_AssetInsp.h"
#include "LevelEditor_PerfMetrics.h"
#include "LevelEditor_Hierarchy.h"
#include "LevelEditor_Inspector.h"
#include "LevelEditor_CSVTag.h"
#include "../imgui/imguizmo/ImGuizmo.h"

#include "../../Input/SystemInput.h"

//Added by Teck Wei
#include "LevelEditor_ECHelper.h"

// For message
#include "..\..\Messaging\SystemBroadcaster.h"
#include "..\..\Messaging\Messages\MessageTogglePlay.h"
#include "..\..\Graphics\GraphicsSystem.h"

LevelEditor::LevelEditor() : _window{ nullptr }, _showGrid{ true }
//, _runEngine{ false }
{
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::Init(HWND window)
{
    // CREATE WINDOWS HERE
    //LE_CreateWindow<TestCase>("Test", false, 0);
    LE_CreateWindow<ConsoleLog>("Console", true, 0);
    LE_CreateWindow<AssetInspector>("Asset Inspector", true);
    LE_CreateWindow<PerformanceMetrics>("Performance Metrics", false);
    LE_CreateWindow<HierarchyInspector>("Hierarchy", true);
    LE_CreateWindow<InspectorWindow>("Inspector", true);
    LE_CreateWindow<LevelEditor_CSVTag>("CSVArr", false, ImGuiWindowFlags_HorizontalScrollbar);

    _window = window;

    SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ENGINE_PLAY_BUTTON_EX", SystemInput_ns::IKEY_P, "PLAYBUTTONIDET_EX", SystemInput_ns::OnPress,
        [this]()
        {
            if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
            {
                _runEngine = !_runEngine;
                MessageTogglePlay isPlaying(_runEngine, "TogglePlay");
                //GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying);

                //MessageTogglePlay isPlaying(_runEngine);
                if (_runEngine)
                {
                    MessageTogglePlay isPlaying_1(_runEngine, "BeforePlay");
                    NS_GRAPHICS::CameraSystem::GetInstance().SavePosition();
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying_1);
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying);

                    DELTA_T->load();
                   
                }
                else
                {
                    MessageTogglePlay isPlaying_2(_runEngine, "AfterPlay");
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying);
                    GLOBAL_SYSTEM_BROADCAST.ProcessMessage(isPlaying_2);

                    NS_GRAPHICS::CameraSystem::GetInstance().MoveToSavedPosition();

                    DELTA_T->load();
                }
            }


        });

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplWin32_Init(window);
    ImGui_ImplOpenGL3_Init("#version 130");

    for (auto& i : _editorWind)
        if (i._ptr)
            i._ptr->Start();

    LE_ConfigLoad();
}

bool LevelEditor::Update(float)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // Run the MainMenubar
    LE_MainMenuBar();

    //bool show_demo_window = true;
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //ImGui::ShowDemoWindow(&show_demo_window);
    /*
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    */
    if (!_runEngine)
    {
        LE_RunWindows();

        _lePicking.LE_PickingRun();
    }
    else
    {
        //LE_RunWindows();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return false;
}

void LevelEditor::Exit()
{
    for (auto& i : _editorWind)
        if (i._ptr)
            i._ptr->End();

    LE_ConfigSave();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void LevelEditor::LE_SetWindowFlag(const std::string& name, const ImGuiWindowFlags& flag)
{
    for (auto& i : _editorWind)
        if (i._name == name)
        {
            i._flag = flag;
            break;
        }
}

std::vector<float>* LevelEditor::LE_GetSystemsUsage()
{
#ifdef LEVELEDITOR_PERFORMANCE_METRICS
    decltype(_editorWind)::iterator it = std::find(std::begin(_editorWind), std::end(_editorWind), "Performance Metrics");
    if (it != std::end(_editorWind))
    {
        PerformanceMetrics* perf = dynamic_cast<PerformanceMetrics*>(it->_ptr.get());
        if (perf)
            return perf->GetSystemsUsage();
    }
#endif
    return nullptr;
}

void LevelEditor::LE_ConsoleOut(const std::string& out)
{
    LE_AccessWindowFunc("Console", &ConsoleLog::AddLog, out);
}

bool LevelEditor::GetIfRunning()
{
    return _runEngine;
}

void LevelEditor::LE_AddMenuWithItems(const std::string& name, const std::vector<std::string>& menuItems,
    const std::vector<std::string>& shortcuts, const std::vector<std::function<void()>>& menuItemFunc)
{
    if (ImGui::BeginMenu(name.c_str()))
    {
        for (unsigned i = 0; i < menuItems.size(); ++i)
        {
            std::string sc;
            if (i < shortcuts.size())
                sc = shortcuts[i];
            if (ImGui::MenuItem(menuItems[i].c_str(), sc.c_str()))
            {
                if (i < menuItemFunc.size() && menuItemFunc[i])
                {
                    (menuItemFunc[i])();
                }
            }
        }
        ImGui::EndMenu();
    }
}

void LevelEditor::LE_AddHelpMarker(const std::string& tip)
{
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(tip.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void LevelEditor::LE_RunWindows()
{
    for (auto& i : _editorWind)
    {
        if (i._isOpen)
        {
            if (i._ptr)
            {
                i._ptr->Init();
                if (ImGui::Begin(i._name.c_str(), &i._isOpen, i._flag))
                    i._ptr->Run();
                ImGui::End();
                i._ptr->Exit();
            }
        }
    }
}

#include <filesystem>

std::vector<std::string> LE_GetDirectories(const std::string& path)
{
    if (path.empty())
        return {};
    std::vector<std::string> r;
    for (auto& p : std::filesystem::directory_iterator(path))
        if (p.is_directory())
            r.push_back(p.path().string());
    return r;
}

std::vector<std::string> LE_GetFilesInDir(const std::string& path)
{
    if (path.empty())
        return {};
    std::vector<std::string> r;
    for (const auto& p : std::filesystem::directory_iterator(path))
        if (!p.is_directory())
            r.push_back(p.path().string());
    return r;
}

std::string LE_EraseSubStr(const std::string& str, const std::string& toErase)
{
    std::string mainStr = str;
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }

    return mainStr;
}

std::string LE_EraseBackSubStr(const std::string& str, const std::string& toErase)
{
    std::string mainStr = str;
    // Search for the substring in string
    size_t pos = mainStr.rfind(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }

    return mainStr;
}

std::string LE_GetFilename(const std::string& path)
{
    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = path.rfind(sep, path.length());
    if (i != std::string::npos) {
        return(path.substr(i + 1, path.length() - i));
    }

    return {};
}

std::string LE_GetFileType(const std::string& path)
{
    char sep = '.';
    size_t i = path.rfind(sep, path.length());
    if (i != std::string::npos) {
        return(path.substr(i + 1, path.length() - i));
    }

    return {};
}