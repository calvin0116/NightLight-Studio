#include "LevelEditor.h"
#ifndef LEVELEDITOR_INPUT_PROPERTIES_HPP
#define LEVELEDITOR_INPUT_PROPERTIES_HPP

/******************************* Windows *******************************************************************************************************/

template<typename T, std::enable_if_t<std::is_base_of<LE_WindowBase, T>::value, int>>
inline void LevelEditor::LE_CreateWindow(const std::string& name, bool isOpenByDefault, const ImGuiWindowFlags& flag)
{
    LE_WindowBase* ptr = T().Clone();
    ptr->AddEditor(this);
    _editorWind.emplace_back(LEWindow(name, isOpenByDefault, flag, ptr));
}

template<typename TFunc>
inline void LevelEditor::LE_AddChildWindow(const std::string& name, ImVec2 size, TFunc fn, bool border, const ImGuiWindowFlags& flag)
{
    ImGui::BeginChild(name.c_str(), size, border, flag);

    fn();

    ImGui::EndChild();
}

template<typename T, typename Y>
inline void LevelEditor::LE_AccessWindowFunc(const std::string& name, void(T::* fn)())
{
    T* win = dynamic_cast<T*>(std::find(std::begin(_editorWind), std::end(_editorWind), name)->_ptr.get());
    // May change to simply do something else
    assert(win);
    if (fn)
        (win->*fn)();
}

template<typename T, typename Y>
inline void LevelEditor::LE_AccessWindowFunc(const std::string& name, void(T::* fn)(const Y&), const Y& arg)
{
    T* win = dynamic_cast<T*>(std::find(std::begin(_editorWind), std::end(_editorWind), name)->_ptr.get());
    // May change to simply do something else
    assert(win);
    if (fn)
        (win->*fn)(arg);
}

/*******************************************************************************************************************************************/

/******************************* Menus ***************************************/

template<typename TFunc>
inline void LevelEditor::LE_AddMenuOnly(const std::string& name, TFunc fn)
{
    if (ImGui::BeginMenu(name.c_str()))
    {
        fn();
        ImGui::EndMenu();
    }
}

/**********************************************************************/

/******************************* Inputs ***************************************/

template<typename TFunc>
inline void LevelEditor::LE_AddButton(const std::string& name, TFunc fn, const ImVec2& size)
{
    if (ImGui::Button(name.c_str(), size))
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddArrowButton(const std::string& name, const ImGuiDir& dir, TFunc fn)
{
    if (ImGui::ArrowButton(name.c_str(), dir))
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddRadioButton(const std::string& name, bool active, TFunc fn)
{
    if (ImGui::RadioButton(name.c_str(), active))
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddSelectable(const std::string& name, bool isSelected, TFunc fn, const ImGuiSelectableFlags& flag, const ImVec2& size)
{
    if (ImGui::Selectable(name.c_str(), isSelected, flag, size))
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddSelectable(const std::string& name, bool* isSelectedPtr, TFunc fn, const ImGuiSelectableFlags& flag, const ImVec2& size)
{
    if (ImGui::Selectable(name.c_str(), isSelectedPtr, flag, size))
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddCheckbox(const std::string& name, bool* isSelected, TFunc fn)
{
    if (ImGui::Checkbox(name.c_str(), isSelected))
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputIntProperty(const std::string& name, int& prop, TFunc fn, const ImGuiInputTextFlags& flag)
{
    if (ImGui::InputInt(name.c_str(), &prop, 1, 100, flag))
        fn();
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputInt2Property(const std::string& name, int& prop1, int& prop2, TFunc fn, const ImGuiInputTextFlags& flag)
{
    int arr[2] = { prop1, prop2 };
    if (ImGui::InputInt2(name.c_str(), arr, flag))
        fn();
    prop1 = arr[0];
    prop2 = arr[1];
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputInt3Property(const std::string& name, int& prop1, int& prop2, int& prop3, TFunc fn, const ImGuiInputTextFlags& flag)
{
    int arr[3] = { prop1, prop2, prop3 };
    if (ImGui::InputInt3(name.c_str(), arr, flag))
        fn();
    prop1 = arr[0];
    prop2 = arr[1];
    prop3 = arr[2];
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputFloatProperty(const std::string& name, float& prop, TFunc fn, const ImGuiInputTextFlags& flag)
{
    if (ImGui::InputFloat(name.c_str(), &prop, 1, 100, 3, flag))
        fn();
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputFloat2Property(const std::string& name, float& prop1, float& prop2, TFunc fn, const ImGuiInputTextFlags& flag)
{
    float arr[2] = { prop1, prop2 };
    if (ImGui::InputFloat2(name.c_str(), arr, 3, flag))
        fn();
    prop1 = arr[0];
    prop2 = arr[1];
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputFloat3Property(const std::string& name, float& prop1, float& prop2, float& prop3, TFunc fn, const ImGuiInputTextFlags& flag)
{
    float arr[3] = { prop1, prop2, prop3 };
    if (ImGui::InputFloat3(name.c_str(), arr, 3, flag))
        fn();
    prop1 = arr[0];
    prop2 = arr[1];
    prop3 = arr[2];
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputText(const std::string& name, std::string& text, unsigned maxLen, const ImGuiInputTextFlags& flag, TFunc fn, int(*callback)(ImGuiInputTextCallbackData*), void* obj)
{
    char* toStr = new char[maxLen];

    std::copy(std::begin(text), std::end(text), toStr);
    toStr[text.size()] = '\0';

    bool runFN = false;
    if (ImGui::InputText(name.c_str(), toStr, maxLen, flag, callback, obj))
        runFN = true;

    text = toStr;

    delete[] toStr;

    fn();
}

template<typename TFunc>
inline void LevelEditor::LE_AddInputMultiText(const std::string& name, std::string& text, unsigned maxLen, ImVec2 size, const ImGuiInputTextFlags& flag, TFunc fn, int(*callback)(ImGuiInputTextCallbackData*), void* obj)
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
        fn();
}

/**********************************************************************/

/******************************* Drop-Downs ***************************************/

template<typename TFunc>
inline void LevelEditor::LE_AddCollapsingHeader(const std::string& name, TFunc fn)
{
    if (ImGui::CollapsingHeader(name.c_str()))
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddTreeNodes(const std::string& name, TFunc fn, ImGuiTreeNodeFlags flag)
{
    if (ImGui::TreeNodeEx(name.c_str(), flag))
    {
        fn();

        ImGui::TreePop();
        ImGui::Separator();
    }
}
/**********************************************************************/

/******************************* Hover ***************************************/

template<typename TFunc>
inline void LevelEditor::LE_AddTooltip(const std::string& tip, TFunc fn)
{
    if (ImGui::IsItemHovered())
    {
        //ImGui::BeginTooltip();

        //ImGui::Text(tip.c_str());
        ImGui::SetTooltip(tip.c_str());
        fn();

        // ImGui::EndTooltip();
    }
}

/**********************************************************************/

/******************************* DragDrop ***************************************/

template<typename T, typename TFunc>
inline void LevelEditor::LE_AddDragDropSource(const std::string& id, T* data, TFunc fn, const ImGuiDragDropFlags& flag)
{
    // Our buttons are both drag sources and drag targets here!
    if (ImGui::BeginDragDropSource(flag))
    {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload(id.c_str(), data, sizeof(T));

        fn();

        ImGui::EndDragDropSource();
    }
}

template<typename T, typename TFunc>
inline void LevelEditor::LE_AddDragDropTarget(const std::string& id, TFunc fn, const ImGuiDragDropFlags& flag)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id.c_str(), flag))
        {
            IM_ASSERT(payload->DataSize == sizeof(T));
            T payload_n = *(const T*)payload->Data;
            fn(&payload_n);
        }
        ImGui::EndDragDropTarget();
    }
}

/**********************************************************************/

template<typename TFunc>
inline void LevelEditor::LE_AddPopupButton(const std::string& name, TFunc fn, const ImVec2& size, const ImGuiPopupFlags& popupFlag, const ImGuiWindowFlags& winFlag)
{
    LE_AddButton(name,
        [&]()
        {
            ImGui::OpenPopup(name.c_str(), popupFlag);
        }, size);

    if (ImGui::BeginPopup(name.c_str(), winFlag))
    {
        fn();
        ImGui::EndPopup();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddContext(const std::string& name, TFunc fn, const ImGuiPopupFlags& popupFlag)
{
    if (ImGui::BeginPopupContextItem(name.c_str(), popupFlag))
    {
        fn();

        ImGui::EndPopup();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddHover(TFunc fn)
{
    if (ImGui::IsItemHovered())
    {
        fn();
    }
}

template<typename TFunc>
inline void LevelEditor::LE_AddStyleVar(const ImGuiStyleVar& var, const float& val, TFunc fn)
{
    ImGui::PushStyleVar(var, val);

    fn();

    ImGui::PopStyleVar();
}

#endif