#include "LevelEditor.h"

void LevelEditor::LE_AddText(const std::string& text)
{
    ImGui::Text(text.c_str());
}

void LevelEditor::LE_AddButton(const std::string& name, std::function<void()> fn, ImVec2 size)
{
    if (ImGui::Button(name.c_str(), size))
    {
        if (fn)
            fn();
    }
}

void LevelEditor::LE_AddArrowButton(const std::string& name, ImGuiDir dir, std::function<void()> fn)
{
    if (ImGui::ArrowButton(name.c_str(), dir))
    {
        if (fn)
            fn();
    }
}

void LevelEditor::LE_AddRadioButton(const std::string& name, bool active, std::function<void()> fn)
{
    if (ImGui::RadioButton(name.c_str(), active))
    {
        if (fn)
            fn();
    }
}

void LevelEditor::LE_AddSelectable(const std::string& name, bool isSelected, std::function<void()> fn, ImGuiSelectableFlags flag, ImVec2 size)
{
    if (ImGui::Selectable(name.c_str(), isSelected, flag, size))
    {
        if (fn)
            fn();
    }
}

void LevelEditor::LE_AddSelectable(const std::string& name, bool* isSelectedPtr, std::function<void()> fn, ImGuiSelectableFlags flag, ImVec2 size)
{
    if (ImGui::Selectable(name.c_str(), isSelectedPtr, flag, size))
    {
        if (fn)
            fn();
    }
}

void LevelEditor::LE_AddCheckbox(const std::string& name, bool* isSelected, std::function<void()> fn)
{
    if (ImGui::Checkbox(name.c_str(), isSelected))
    {
        if (fn)
            fn();
    }
}

void LevelEditor::LE_AddInputIntProperty(const std::string& name, int& prop, std::function<void()> fn, ImGuiInputTextFlags flag)
{
    if (ImGui::InputInt(name.c_str(), &prop, 1, 100, flag))
        if (fn)
            fn();
}

void LevelEditor::LE_AddInputInt2Property(const std::string& name, int& prop1, int& prop2, std::function<void()> fn, ImGuiInputTextFlags flag)
{
    int arr[2] = { prop1, prop2 };
    if (ImGui::InputInt2(name.c_str(), arr, flag))
        if (fn)
            fn();
    prop1 = arr[0];
    prop2 = arr[1];
}
void LevelEditor::LE_AddInputInt3Property(const std::string& name, int& prop1, int& prop2, int& prop3, std::function<void()> fn, ImGuiInputTextFlags flag)
{
    int arr[3] = { prop1, prop2, prop3 };
    if (ImGui::InputInt3(name.c_str(), arr, flag))
        if (fn)
            fn();
    prop1 = arr[0];
    prop2 = arr[1];
    prop3 = arr[2];
}

void LevelEditor::LE_AddInputFloatProperty(const std::string& name, float& prop, std::function<void()> fn, ImGuiInputTextFlags flag)
{ 
    if (ImGui::InputFloat(name.c_str(), &prop, 1, 100, 3, flag))
        if (fn)
            fn();
}

void LevelEditor::LE_AddInputFloat2Property(const std::string& name, float& prop1, float& prop2, std::function<void()> fn, ImGuiInputTextFlags flag)
{
    float arr[2] = { prop1, prop2 };
    if (ImGui::InputFloat2(name.c_str(), arr, 3, flag))
        if (fn)
            fn();
    prop1 = arr[0];
    prop2 = arr[1];
}

void LevelEditor::LE_AddInputFloat3Property(const std::string& name, float& prop1, float& prop2, float& prop3, std::function<void()> fn, ImGuiInputTextFlags flag)
{
    float arr[3] = { prop1, prop2, prop3 };
    if (ImGui::InputFloat3(name.c_str(), arr, 3, flag))
        if (fn)
            fn();
    prop1 = arr[0];
    prop2 = arr[1];
    prop3 = arr[2];
}

void LevelEditor::LE_AddSliderIntProperty(const std::string& name, int& prop, int min, int max, ImGuiInputTextFlags flag)
{
    ImGui::SliderInt(name.c_str(), &prop, min, max, "%d",flag);
}

void LevelEditor::LE_AddSliderFloatProperty(const std::string& name, float& prop, float min, float max, ImGuiInputTextFlags flag)
{
    ImGui::SliderFloat(name.c_str(), &prop, min, max, "%.3f", flag);
}

void LevelEditor::LE_AddInputText(const std::string& name, std::string& text, unsigned maxLen, 
    ImGuiInputTextFlags flag,
    std::function<void()> fn, int(*callback)(ImGuiInputTextCallbackData*), void* obj)
{
    char* toStr = new char[maxLen];

    std::copy(std::begin(text), std::end(text), toStr);
    toStr[text.size()] = '\0';

    bool runFN = false;
    if (ImGui::InputText(name.c_str(), toStr, maxLen, flag, callback, obj))
        runFN = true;

    text = toStr;

    delete[] toStr;

    if (runFN)
        if (fn)
            fn();
}

void LevelEditor::LE_AddInputMultiText(const std::string& name, std::string& text, unsigned maxLen, ImVec2 size,
    ImGuiInputTextFlags flag,
    std::function<void()> fn, int(*callback)(ImGuiInputTextCallbackData*), void* obj)
{
    char* toStr = new char[maxLen];

    std::copy(std::begin(text), std::end(text), toStr);
    toStr[text.size()] = '\0';

    bool runFN = false;
    if (ImGui::InputTextMultiline(name.c_str(), toStr, maxLen, size, flag, callback, obj))
        runFN = true;

    text = toStr;

    delete[] toStr;

    if (runFN)
        if (fn)
            fn();
}

void LevelEditor::LE_AddColorEdit3(const std::string& name, float& r, float& g, float& b)
{
    float arr[3] = { r,g,b };
    ImGui::ColorEdit3(name.c_str(), arr);
    r = arr[0]; g = arr[1]; b = arr[2];
}

void LevelEditor::LE_AddColorEdit4(const std::string& name, float& r, float& g, float& b, float& a)
{
    float arr[4] = { r,g,b,a };
    ImGui::ColorEdit4(name.c_str(), arr);
    r = arr[0]; g = arr[1]; b = arr[2]; a = arr[3];
}

void LevelEditor::LE_AddImage(const ImTextureID& id, const ImVec2& size, const ImVec2& minUV, const ImVec2& maxUV, const ImVec4& tintCol, const ImVec4& borderCol)
{
    // Add a check to see if id exists?
    ImGui::Image(id, size, minUV, maxUV, tintCol, borderCol);
}

void LevelEditor::LE_AddPlotLines(const std::string& name, std::vector<float>& graph, bool addData, const float& newData)
{
    if (addData)
    {
        graph.erase(std::begin(graph));
        graph.push_back(newData);
    }
    ImGui::PlotLines(name.c_str(), graph.data(), (int)graph.size());
}

void LevelEditor::LE_AddHistogram(const std::string& name, std::vector<float>& graph, bool addData, const float& newData, float min, float max, ImVec2 size)
{
    if (addData)
    {
        graph.erase(std::begin(graph));
        graph.push_back(newData);
    }
    ImGui::PlotHistogram(name.c_str(), graph.data(), (int)graph.size(), 0, NULL, min, max, size);
}

void LevelEditor::LE_AddProgressBar(float progress, ImVec2 size, std::string overlay)
{
    // clamps values
    if (progress < 0.0f)
        progress = 0.0f;
    else if (progress > 1.0f)
        progress = 1.0f;

    if (overlay.empty())
    {
        overlay = std::to_string((int)(progress * 100.0f)).append("%");
    }

    ImGui::ProgressBar(progress, size, overlay.c_str());
    //ImGui::ProgressBar((progress), ImVec2(0.f, 0.f), overlay.c_str());
}

void LevelEditor::LE_AddPopupButton(const std::string& name, std::function<void()> fn, ImVec2 size, ImGuiPopupFlags popupFlag, ImGuiWindowFlags winFlag)
{
    LE_AddButton(name,
        [&]()
        {
            ImGui::OpenPopup(name.c_str(), popupFlag);
        }, size);

    if (ImGui::BeginPopup(name.c_str(), winFlag))
    {
        if (fn)
            fn();
        ImGui::EndPopup();
    }
}

void LevelEditor::LE_AddContext(const std::string& name, std::function<void()> fn, ImGuiPopupFlags popupFlag)
{
    if (ImGui::BeginPopupContextItem(name.c_str(), popupFlag))
    {
        if (fn)
            fn();

        ImGui::EndPopup();
    }
}

void LevelEditor::LE_AddModal(const std::string& name, std::function<void()> fn, std::vector<std::pair<std::string, std::function<void()>>> buttonFn, ImVec2 size, ImGuiPopupFlags popupFlag, ImGuiWindowFlags winFlag)
{
    // Always center this window when appearing
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    LE_AddButton(name,
        [&]()
        {
            ImGui::OpenPopup(name.c_str(), popupFlag);
        }, size);

    if (ImGui::BeginPopupModal(name.c_str(), nullptr, winFlag))
    {
        if (fn)
            fn();

        // By default if there are no button functions to close
        // Doesn't really do anything except close the modal window
        if (buttonFn.empty())
        {
            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        }
        // If there are button inputs, run those functions in the button instead
        else
        {
            for (unsigned i = 0; i < buttonFn.size(); ++i)
            {
                if (ImGui::Button(buttonFn[i].first.c_str(), ImVec2(120,0)))
                {
                    if (buttonFn[i].second)
                        buttonFn[i].second();
                    ImGui::CloseCurrentPopup();
                }

                if (i + 1.0 < buttonFn.size())
                {
                    ImGui::SetItemDefaultFocus();
                    ImGui::SameLine();
                }
            }
        }

        ImGui::EndPopup();
    }
}

void LevelEditor::LE_AddHover(std::function<void()> fn)
{
    if (ImGui::IsItemHovered())
    {
        if (fn)
            fn();
    }
}
