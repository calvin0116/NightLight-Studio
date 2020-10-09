#include "LevelEditor.h"

void LevelEditor::LE_AddCombo(const std::string& name, int& data, const std::vector<std::string> options)
{
    std::string toAppend;
    for (unsigned i = 0; i < options.size(); ++i)
    {
        toAppend.append(options[i]).append("\0", 1);
    }
    ImGui::Combo(name.c_str(), &data, toAppend.c_str());
}

void LevelEditor::LE_AddCollapsingHeader(const std::string& name, const std::vector<std::function<void()>> fns)
{
    if (ImGui::CollapsingHeader(name.c_str()))
    {
        for (unsigned i = 0; i < fns.size(); ++i)
        {
            if (fns[i])
                fns[i]();
        }
    }
}

void LevelEditor::LE_AddTreeNodes(const std::string& name, const std::vector<std::function<void()>> fns)
{
    if (ImGui::TreeNode(name.c_str()))
    {
        for (unsigned i = 0; i < fns.size(); ++i)
        {
            if (fns[i])
                fns[i]();
        }

        ImGui::TreePop();
        ImGui::Separator();
    }
}