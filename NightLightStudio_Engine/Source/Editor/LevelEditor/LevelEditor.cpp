#include "LevelEditor.h"
#include "LevelEditor_Console.h"
#include "LevelEditor_AssetInsp.h"
#include "LevelEditor_PerfMetrics.h"
#include "LevelEditor_Heirarchy.h"

//Added by Teck Wei
#include "LevelEditor_ECHelper.h"

LevelEditor::LevelEditor() : _window{ nullptr }, _runEngine{ false }
{
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::Init(HWND window)
{
    // CREATE WINDOWS HERE
    //LE_CreateWindow<TestCase>("Test", false, 0);
    LE_CreateWindow<ConsoleLog>("Console", false, 0);
    LE_CreateWindow<AssetInspector>("Asset Inspector", true);
    LE_CreateWindow<PerformanceMetrics>("Performance Metrics", true);
    LE_CreateWindow<HeirarchyInspector>("Heirarchy Inspector", true);


    _window = window;

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
}

bool LevelEditor::Update(float)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Run the MainMenubar
    LE_MainMenuBar();

    /*
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    ImGui::ShowDemoWindow(&show_demo_window);

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
    LE_RunWindows();

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

    //Added by Teck Wei for singleton destruction
    LE_ECHELPER->DestroyInstance();
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
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
