#include "LevelEditor_Audio.h"
#include <unordered_map>;

void LevelEditorAudio::Init()
{
  ImGui::SetNextWindowBgAlpha(1.0f);
  ImGui::SetWindowSize(ImVec2(640, 320), ImGuiCond_FirstUseEver);
}

void LevelEditorAudio::Run()
{
  _levelEditor->LE_AddButton("AddAudio##AudioButton", 
    [this]()
    {
      Audios.push_back("");
    });

  size_t index = 0;
  for (std::string& path : Audios)
  {
    ImGui::SetNextItemWidth(500);
    _levelEditor->LE_AddInputText(std::to_string(index), path, 500, ImGuiInputTextFlags_EnterReturnsTrue);
    ++index;

    _levelEditor->LE_AddDragDropTarget<std::string>("ASSET_FILEPATH",
      [this, &path](std::string* str)
      {
        std::string newData = *str;
        newData.erase(newData.begin());
        std::transform(newData.begin(), newData.end(), newData.begin(),
          [](unsigned char c)
          { return (char)std::tolower(c); });

        std::string fileType = LE_GetFileType(newData);
        if (fileType == "ogg")
          path = newData;
      });
  }
}
