#include "LevelEditor.h"

bool LevelEditor::LE_AddCombo(const std::string& name, int& data, const std::vector<std::string> options)
{
    std::string toAppend;
    for (unsigned i = 0; i < options.size(); ++i)
    {
        toAppend.append(options[i]).append("\0", 1);
    }
    return ImGui::Combo(name.c_str(), &data, toAppend.c_str());
}