#include "LevelEditor.h"

void LevelEditor::LE_AddText(const std::string& text)
{
    ImGui::Text(text.c_str());
}

void LevelEditor::LE_AddSliderIntProperty(const std::string& name, int& prop, int min, int max, const ImGuiInputTextFlags& flag)
{
    ImGui::SliderInt(name.c_str(), &prop, min, max, "%d", flag);
}

void LevelEditor::LE_AddSliderFloatProperty(const std::string& name, float& prop, float min, float max, const ImGuiInputTextFlags& flag)
{
    ImGui::SliderFloat(name.c_str(), &prop, min, max, "%.3f", flag);
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

void LevelEditor::LE_AddProgressBar(float progress, const ImVec2& size, std::string overlay)
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

void LevelEditor::LE_AddModal(const std::string& name, const std::function<void()>& fn, const std::vector<std::pair<std::string, std::function<void()>>>& buttonFn,
    const ImVec2& size, const ImGuiPopupFlags& popupFlag, const ImGuiWindowFlags& winFlag)
{
    // Always center this window when appearing
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    LE_AddButton(name,
        [this, &name, &popupFlag]()
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
                if (ImGui::Button(buttonFn[i].first.c_str(), ImVec2(120, 0)))
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