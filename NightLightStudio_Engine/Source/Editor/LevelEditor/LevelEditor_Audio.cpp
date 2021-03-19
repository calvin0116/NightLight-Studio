#include "LevelEditor_Audio.h"
#include <unordered_map>;

void LevelEditorAudio::Init()
{
  ImGui::SetNextWindowBgAlpha(1.0f);
  ImGui::SetWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);
}

void LevelEditorAudio::Run()
{
  std::unordered_map<std::string, std::string> AudioMap; // name, path
  _levelEditor->LE_AddButton("AddAudio##AudioButton", 
    [&AudioMap]() 
    {
      static int i = 0;
      std::string s = std::to_string(i);
      AudioMap.emplace(s, "");
      ++i;
    });

  _levelEditor->LE_AddButton("Remove##AudioButton",
    [&AudioMap]()
    {
      static int i = 0;
      std::string s = std::to_string(i);
      AudioMap.emplace(s, "");
      ++i;
    });
}
