#ifndef LEVEL_EDITOR
#define LEVEL_EDITOR

#include "..\imgui\imgui.h"
#include "..\imgui\imgui_impl_glfw.h"
#include "..\imgui\imgui_impl_opengl3.h"
#include "..\imgui\imgui_impl_win32.h"

#include <stdio.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <Windows.h>

class LevelEditor;

class LE_WindowBase
{
	friend class LevelEditor;
protected:
	LevelEditor* _levelEditor = nullptr;
	// Add editor pointer
	virtual void AddEditor(LevelEditor* editor) { _levelEditor = editor; }
public:
	LE_WindowBase() = default;
	virtual ~LE_WindowBase() = default;

	// Starts the Window - Runs once only
	virtual void Start() {}
	// Initializes the Window Loop. Runs before Run() and ImGui::Begin();
	virtual void Init() {}
	// Runs the Window. Main functions found here.
	virtual void Run() {}
	// Exits the Window Loop. Runs after Run() and ImGui::End();
	virtual void Exit() {}
	// Ends the Window - Runs once on destruction only.
	virtual void End() {}
	// Cloning
	virtual LE_WindowBase* Clone() const = 0;
};

template <class Derived>
class LE_WinBase_Derived : public LE_WindowBase
{
public:
	LE_WinBase_Derived() = default;
	virtual ~LE_WinBase_Derived() = default;
	virtual LE_WindowBase* Clone() const
	{
		return new Derived(static_cast<const Derived&>(*this)); // call the copy ctor.
	}
};

class LevelEditor
{
	HWND _window;

	bool _runEngine;

	struct LEWindow
	{
		std::string _name;
		bool _isOpen;
		ImGuiWindowFlags _flag;
		std::unique_ptr<LE_WindowBase> _ptr;

		LEWindow(std::string name, bool isOpen, ImGuiWindowFlags flag, LE_WindowBase* ptr)
			: _name{ name }, _isOpen{ isOpen }, _flag{ flag }, _ptr{ ptr } {}

		bool operator==(const std::string rhs)
		{
			return _name == rhs;
		}
		friend bool operator==(const LEWindow& lhs, const std::string rhs)
		{
			return lhs._name == rhs;
		}
	};

	std::vector<LEWindow> _editorWind;

	void LE_RunWindows();
	void LE_MainMenuBar();
public:
	LevelEditor();
	~LevelEditor();

	// Run Once
	void Init(HWND window); // Replace window with handle or something

	// Run Every Frame
	bool Update(float dt = 0);

	// Run Once
	void Exit();

	/******************************* Windows ***************************************/

	// Use to create a new Window to use
	// Init is used before running ImGui::Begin
	// Run is considered the "update" loop
	// Exit is used after running ImGui::End
	// This will handle ImGui's Begin and End calls automatically
	template <typename T,
		std::enable_if_t<std::is_base_of<LE_WindowBase, T>::value, int> = 0>
		void LE_CreateWindow(const std::string& name, bool isOpenByDefault = false, const ImGuiWindowFlags& flag = ImGuiWindowFlags_None);
	
	// Creates a Child Window within the current Window
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddChildWindow(const std::string& name, ImVec2 size = ImVec2(0, 0),
		TFunc fn = []() {}, bool border = false, const ImGuiWindowFlags& flag = 0);
	void LE_AddChildWindow(const std::string& name, ImVec2 size = ImVec2(0, 0), bool border = false, const ImGuiWindowFlags& flag = 0)
	{ LE_AddChildWindow(name, size, []() {}, border, flag); }

	// Runs another window's function.
	//
	template <typename T, typename... Y, typename TReturn>
	TReturn LE_AccessWindowFunc(const std::string& name, TReturn(T::* fn)(Y...), Y... arg);

	// Sets a Windows Flag. Please use in Start.
	void LE_SetWindowFlag(const std::string& name, const ImGuiWindowFlags& flag);

	/**********************************************************************/

	// std::function<void()> Mostly, use either helper functions OR lambdas

	/******************************* Helper Functions ***************************************/

	// ONLY USED FOR PERFORMANCE METRICS, DOES NOTHING OTHERWISE
	std::vector<float>* LE_GetSystemsUsage();

	/**********************************************************************/

	/******************************* Menus ***************************************/

	// Creates ONLY a Menu within the Window
	// Use this for more control over the Menu (Adding additional menus within)
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddMenuOnly(const std::string& name, TFunc fn = []() {});
	void LE_AddMenuOnly(const std::string& name) { LE_AddMenuOnly(name, []() {}); }

	// Creates a Menu with given Items and Functions to run them
	// This will run in Order, so if there is an empty item, give it an empty lambda or nullptr
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	void LE_AddMenuWithItems(const std::string& name, const std::vector<std::string>& menuItems = {},
		const std::vector<std::string>& shortcuts = {}, const std::vector<std::function<void()>>& menuItemFunc = {});

	/**********************************************************************/

	/******************************* Inputs ***************************************/
	// Adds a textbox
	void LE_AddText(const std::string& text);

	// Adds a button that runs a given function.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddButton(const std::string& name, TFunc fn = []() {}, const ImVec2& size = ImVec2(0, 0));
	void LE_AddButton(const std::string& name, const ImVec2& size = ImVec2(0, 0)) { LE_AddButton(name, []() {}, size); }

	// Adds a directional button that runs a given function.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddArrowButton(const std::string& name, const ImGuiDir& dir = ImGuiDir_Right, TFunc fn = []() {});
	void LE_AddArrowButton(const std::string& name, const ImGuiDir& dir = ImGuiDir_Right) { LE_AddArrowButton(name, dir, []() {}); }

	// Adds a radio button that runs a given function.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddRadioButton(const std::string& name, bool active = false, TFunc fn = []() {});
	void LE_AddRadioButton(const std::string& name, bool active = false) { LE_AddRadioButton(name, active, []() {}); }

	// Adds a selectable that runs a function while selected. Input Boolean is unaffected.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddSelectable(const std::string& name, bool isSelected, TFunc fn = []() {}, const ImGuiSelectableFlags& flag = 0, const ImVec2& size = ImVec2(0, 0));
	void LE_AddSelectable(const std::string& name, bool isSelected, const ImGuiSelectableFlags& flag = 0, const ImVec2& size = ImVec2(0, 0))
	{ LE_AddSelectable(name, isSelected, []() {}, flag, size); }
	// Adds a selectable that runs a function while selected. Input Boolean is affected.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddSelectable(const std::string& name, bool* isSelectedPtr, TFunc fn = []() {}, const ImGuiSelectableFlags& flag = 0, const ImVec2& size = ImVec2(0, 0));
	void LE_AddSelectable(const std::string& name, bool* isSelectedPtr, const ImGuiSelectableFlags& flag = 0, const ImVec2& size = ImVec2(0, 0))
	{ LE_AddSelectable(name, isSelectedPtr, []() {}, flag, size); }

	// Adds a Checkbox. This will automatically toggle the given pointer's value.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddCheckbox(const std::string& name, bool* isSelected, TFunc fn = []() {});
	void LE_AddCheckbox(const std::string& name, bool* isSelected)
	{ LE_AddCheckbox(name, isSelected, []() {}); }

	// Adds an Input Int Property.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddInputIntProperty(const std::string& name, int& prop, TFunc fn = []() {},
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputIntProperty(const std::string& name, int& prop,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue)
	{ LE_AddInputIntProperty(name, prop, []() {}, flag); }
	// Adds an Input with 2 Int Properties.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddInputInt2Property(const std::string& name, int& prop1, int& prop2, TFunc fn = []() {},
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputInt2Property(const std::string& name, int& prop1, int& prop2,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue)
	{ LE_AddInputInt2Property(name, prop1, prop2, []() {}, flag); }
	// Adds an Input with 3 Int Properties.
	template <typename TFunc>
	void LE_AddInputInt3Property(const std::string& name, int& prop1, int& prop2, int& prop3, TFunc fn = []() {},
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputInt3Property(const std::string& name, int& prop1, int& prop2, int& prop3,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue)
	{ LE_AddInputInt3Property(name, prop1, prop2, prop3, []() {}, flag); }

	// Adds an Input Float Property, default precision is 3 decimal places.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddInputFloatProperty(const std::string& name, float& prop, TFunc fn = []() {},
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputFloatProperty(const std::string& name, float& prop,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue)
	{ LE_AddInputFloatProperty(name, prop, []() {}, flag); }

	// Adds an Input Float Property, default precision is 3 decimal places.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddInputFloat2Property(const std::string& name, float& prop1, float& prop2, TFunc fn = []() {},
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputFloat2Property(const std::string& name, float& prop1, float& prop2,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue)
	{ LE_AddInputFloat2Property(name, prop1, prop2, []() {}, flag); }

	// Adds an Input Float Property, default precision is 3 decimal places.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddInputFloat3Property(const std::string& name, float& prop1, float& prop2, float& prop3, TFunc fn = []() {},
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputFloat3Property(const std::string& name, float& prop1, float& prop2, float& prop3,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue)
	{ LE_AddInputFloat3Property(name, prop1, prop2, prop3, []() {}, flag); }

	// Adds a Slider Int Property.
	void LE_AddSliderIntProperty(const std::string& name, int& prop, int min, int max,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	// Adds a Slider Float Property.
	void LE_AddSliderFloatProperty(const std::string& name, float& prop, float min, float max,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);

	// Adds a Textbox Input.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddInputText(const std::string& name, std::string& text, unsigned maxLen,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue,
		TFunc fn = []() {}, int(*callback)(ImGuiInputTextCallbackData*) = nullptr, void* obj = nullptr);
	void LE_AddInputText(const std::string& name, std::string& text, unsigned maxLen,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue,
		int(*callback)(ImGuiInputTextCallbackData*) = nullptr, void* obj = nullptr)
	{ LE_AddInputText(name, text, maxLen, flag, []() {}, callback, obj); }

	// Adds a multiline Textbox Input.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddInputMultiText(const std::string& name, std::string& text, unsigned maxLen, ImVec2 size = ImVec2(0, 0),
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue,
		TFunc fn = []() {}, int(*callback)(ImGuiInputTextCallbackData*) = nullptr, void* obj = nullptr);
	void LE_AddInputMultiText(const std::string& name, std::string& text, unsigned maxLen, ImVec2 size = ImVec2(0, 0),
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue,
		int(*callback)(ImGuiInputTextCallbackData*) = nullptr, void* obj = nullptr)
	{ LE_AddInputMultiText(name, text, maxLen, size, flag, []() {}, callback, obj); }


	// Color Editor. Red Green Blue
	void LE_AddColorEdit3(const std::string& name, float& r, float& g, float& b);
	// Color Editor. Red Green Blue Alpha
	void LE_AddColorEdit4(const std::string& name, float& r, float& g, float& b, float& a);

	// Adds an image.
	void LE_AddImage(const ImTextureID& id, const ImVec2& size,
		const ImVec2& minUV = ImVec2(0, 0), const ImVec2& maxUV = ImVec2(1, 1),
		const ImVec4& tintCol = ImVec4(1, 1, 1, 1), const ImVec4& borderCol = ImVec4(0, 0, 0, 0));

	/**********************************************************************/

	/******************************* Drop-Downs ***************************************/

	// Adds a drop-down list of selectable items. Data represents which item is picked.
	void LE_AddCombo(const std::string& name, int& data, const std::vector<std::string> options);

	// Adds a collapsing Header.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddCollapsingHeader(const std::string& name, TFunc fn = []() {});
	void LE_AddCollapsingHeader(const std::string& name)
	{ LE_AddCollapsingHeader(name, []() {}); }

	// Adds a tree node.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddTreeNodes(const std::string& name, TFunc fn = []() {}, ImGuiTreeNodeFlags flag = 0);
	void LE_AddTreeNodes(const std::string& name, ImGuiTreeNodeFlags flag = 0)
	{ LE_AddTreeNodes(name, []() {}, flag); }

	/**********************************************************************/

	/******************************* Hover ***************************************/
	// Adds a tooltip. Hover over object to see.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddTooltip(const std::string& tip, TFunc fn = []() {});
	void LE_AddTooltip(const std::string& tip) { LE_AddTooltip(tip, []() {}); }

	// Default Help Marker - Only shows a tooltip for help
	void LE_AddHelpMarker(const std::string& tip);

	/**********************************************************************/

	/******************************* Graphing ***************************************/

	// Shows a vector of floats as a line graph. If addData is true, automatically pushes back new data and pops the first (like a queue)
	void LE_AddPlotLines(const std::string& name, std::vector<float>& graph, bool addData = false, const float& newData = 0);

	// Shows a histogram with given graph. If addData is true, automatically pushes back new data and pops the first (like a queue)
	void LE_AddHistogram(const std::string& name, std::vector<float>& graph, bool addData = false, const float& newData = 0, float min = 0, float max = 1, ImVec2 size = ImVec2());

	// Shows progress as a bar. Progress should be a float from 0 to 1.
	void LE_AddProgressBar(float progress, const ImVec2& size = ImVec2(0, 0), std::string overlay = {});

	/**********************************************************************/

	/******************************* DragDrop ***************************************/

	// ID will be used for dragdrop target
	// data is an object that contains the information that will be passed through
	// fn is any additional actions to be taken during drag drop
	// Please indicate type in angled brackets to be safe - eg. String literals are NOT strings "xxx" =/= std::string
	// Place this after the item you want to drag from
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename T, typename TFunc>
	void LE_AddDragDropSource(const std::string& id, T* data, TFunc fn = []() {}, const ImGuiDragDropFlags& flag = 0);
	template <typename T>
	void LE_AddDragDropSource(const std::string& id, T* data, const ImGuiDragDropFlags& flag = 0)
	{ LE_AddDragDropSource(id, data, []() {}, flag); }

	// ID determines checks what payload it receives
	// Function MUST take in the payload type's parameter, payload is a reference
	// Please be very careful with Drag Drop source and Target
	// the fn determines what will be done with the data
	// Place this after the item you want to drag to
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename T, typename TFunc>
	void LE_AddDragDropTarget(const std::string& id, TFunc fn = [](const T*) {}, const ImGuiDragDropFlags& flag = 0);
	template <typename T>
	void LE_AddDragDropTarget(const std::string& id, const ImGuiDragDropFlags& flag = 0)
	{ LE_AddDragDropTarget(id, [](const T*) {}, flag); }
	/**********************************************************************/

	/******************************* Popups ***************************************/

	// Adds a button that runs a popup.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddPopupButton(const std::string& name, TFunc fn = []() {},
		const ImVec2& size = ImVec2(0, 0), const ImGuiPopupFlags& popupFlag = 0, const ImGuiWindowFlags& winFlag = 0);
	void LE_AddPopupButton(const std::string& name,
		const ImVec2& size = ImVec2(0, 0), const ImGuiPopupFlags& popupFlag = 0, const ImGuiWindowFlags& winFlag = 0)
	{ LE_AddPopupButton(name, []() {}, size, popupFlag, winFlag); }

	// Right click to open a Context Menu (Similar to a popup.)
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddContext(const std::string& name, TFunc fn = []() {}, const ImGuiPopupFlags& popupFlag = 1);
	void LE_AddContext(const std::string& name, const ImGuiPopupFlags& popupFlag = 1)
	{ LE_AddContext(name, []() {}, popupFlag); }

	// Opens a Modal Window. Default OK and Cancel options only
	// fn represents the function to run within the Modal window
	// buttonFn is a pair<string, void()> that determines the button functions at the end; Default all in the same line
	void LE_AddModal(const std::string& name, const std::function<void()>& fn = nullptr,
		const std::vector < std::pair< std::string, std::function<void()> > > & buttonFn = {},
		const ImVec2& size = ImVec2(0, 0), const ImGuiPopupFlags& popupFlag = 0, const ImGuiWindowFlags& winFlag = 0);

	// Runs if hovering over previous item.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddHover(TFunc fn = []() {});
	void LE_AddHover() { LE_AddHover([]() {}); }

	/**********************************************************************/

	/******************************* Others ***************************************/

	// Adds a style var.
	// Runs fn between this ImGui's Begin and End
	// Ignore and put the next value or an empty lambda if you do not plan to have any functions running
	template <typename TFunc>
	void LE_AddStyleVar(const ImGuiStyleVar& var, const float& val, TFunc fn = []() {});
	void LE_AddStyleVar(const ImGuiStyleVar& var, const float& val)
	{ LE_AddStyleVar(var, val, []() {}); }

	/**********************************************************************/
};

std::vector<std::string> LE_GetDirectories(const std::string& path);

std::vector<std::string> LE_GetFilesInDir(const std::string& path);

std::string LE_EraseSubStr(const std::string& str, const std::string& toErase);

std::string LE_GetFilename(const std::string& path);

std::string LE_GetFileType(const std::string& path);

#include "LevelEditor_TemplateFuncs.hpp"

#endif
