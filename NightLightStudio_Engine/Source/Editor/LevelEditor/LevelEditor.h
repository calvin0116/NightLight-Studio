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

	// Initializes at the start of a loop: THIS CAN BE CONSIDERED PRE-RUN; Runs functions before the window begins to run
	virtual void Init() {}
	// Runs the function internally within
	virtual void Run() {}
	// Exit function: Considerd an AFTER-RUN. Runs functions after the window has ended.
	virtual void Exit() {}
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
	template <typename T,
		std::enable_if_t<std::is_base_of<LE_WindowBase, T>::value, int> = 0>
		void LE_CreateWindow(const std::string& name, bool isOpenByDefault = false, const ImGuiWindowFlags& flag = ImGuiWindowFlags_None)
	{
		LE_WindowBase* ptr = T().Clone();
		ptr->AddEditor(this);
		_editorWind.emplace_back(LEWindow(name, isOpenByDefault, flag, ptr));
	}

	void LE_AddChildWindow(const std::string& name, ImVec2 size = ImVec2(0, 0),
		const std::vector<std::function<void()>>& fns = {}, bool border = false, const ImGuiWindowFlags& flag = 0);
	template <typename T>
	void LE_AddChildWindow(const std::string& name, ImVec2 size = ImVec2(0, 0),
		const std::vector<void(T::*)()>& fns = {}, T* obj = nullptr, bool border = false, const ImGuiWindowFlags& flag = 0)
	{
		ImGui::BeginChild(name.c_str(), size, border, flag);

		for (unsigned i = 0; i < fns.size(); ++i)
		{
			if (fns[i] && obj)
				(obj->*(fns[i]))();
		}
		ImGui::EndChild();
	}

	// Runs another window's function. No parameters.
	template <typename T, typename Y>
	void LE_AccessWindowFunc(const std::string& name, void(T::* fn)())
	{
		T* win = dynamic_cast<T*>(std::find(std::begin(_editorWind), std::end(_editorWind), name)->_ptr.get());
		// May change to simply do something else
		assert(win);
		if (fn)
			(win->*fn)();

	}
	// Runs another window's function. One parameter.
	template <typename T, typename Y>
	void LE_AccessWindowFunc(const std::string& name, void(T::* fn)(const Y&), const Y& arg)
	{
		T* win = dynamic_cast<T*>(std::find(std::begin(_editorWind), std::end(_editorWind), name)->_ptr.get());
		// May change to simply do something else
		assert(win);
		if (fn)
			(win->*fn)(arg);

	}

	/**********************************************************************/

	// std::function<void()> Mostly, use either helper functions OR lambdas

	/******************************* Helper Functions ***************************************/

	// Use to bind member functions into std::functions
	// Parameters are COPIED.
	// No return is allowed
	template <typename T, typename... TArgs>
	std::function<void()> LE_BindMemFuncs(void(T::* fn)(TArgs...), T* obj, TArgs... args)
	{
		return std::bind(fn, obj, args...);
	}

	// Use to bind a vector of member functions into std::functions
	// No parameters allowed
	template <typename T>
	std::vector<std::function<void()>> LE_BindMemFuncsVec(const std::vector<void(T::*)()>& fn, T* obj)
	{
		std::vector<std::function<void()>> retVec;
		if (obj)
		{
			for (unsigned i = 0; i < fn.size(); ++i)
			{
				retVec.push_back(std::bind(fn[i], obj));
			}
		}
		return retVec;
	}

	// ONLY USED FOR PERFORMANCE METRICS, DOES NOTHING OTHERWISE
	std::vector<float>* LE_GetSystemsUsage();

	/**********************************************************************/

	/******************************* Menus ***************************************/

	// Adds only a menu
	// Let function be menu with items to create a secondary inner menu
	// or just another menu only
	void LE_AddMenuOnly(const std::string& name, const std::function<void()>& fn = nullptr);

	// Adds a menu and a list of menu items
	void LE_AddMenuWithItems(const std::string& name, const std::vector<std::string>& menuItems = {},
		const std::vector<std::string>& shortcuts = {}, const std::vector<std::function<void()>>& menuItemFunc = {});

	/**********************************************************************/

	/******************************* Inputs ***************************************/
	// Adds a textbox
	void LE_AddText(const std::string& text);

	// Adds a button that runs a function
	void LE_AddButton(const std::string& name, const std::function<void()>& fn = nullptr, const ImVec2& size = ImVec2(0, 0));

	// Adds a directional button that runs a function
	void LE_AddArrowButton(const std::string& name, const ImGuiDir& dir = ImGuiDir_Right, const std::function<void()>& fn = nullptr);

	// Adds a radio button that runs a function
	void LE_AddRadioButton(const std::string& name, bool active = false, const std::function<void()>& fn = nullptr);

	// Adds a selectable that runs a function while selected. Input Boolean is unaffected.
	void LE_AddSelectable(const std::string& name, bool isSelected, const std::function<void()>& fn = nullptr, const ImGuiSelectableFlags& flag = 0, const ImVec2& size = ImVec2(0, 0));
	// Adds a selectable that runs a function while selected. Input Boolean is affected.
	void LE_AddSelectable(const std::string& name, bool* isSelectedPtr, const std::function<void()>& fn = nullptr, const ImGuiSelectableFlags& flag = 0, const ImVec2& size = ImVec2(0, 0));

	void LE_AddCheckbox(const std::string& name, bool* isSelected, const std::function<void()>& fn = nullptr);

	// Adds an Input Int Property
	void LE_AddInputIntProperty(const std::string& name, int& prop, const std::function<void()>& fn = nullptr,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputInt2Property(const std::string& name, int& prop1, int& prop2, const std::function<void()>& fn = nullptr,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputInt3Property(const std::string& name, int& prop1, int& prop2, int& prop3, const std::function<void()>& fn = nullptr,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);

	// Adds an Input Float Property, default precision is 3 decimal places
	void LE_AddInputFloatProperty(const std::string& name, float& prop, const std::function<void()>& fn = nullptr,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputFloat2Property(const std::string& name, float& prop1, float& prop2, const std::function<void()>& fn = nullptr,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	void LE_AddInputFloat3Property(const std::string& name, float& prop1, float& prop2, float& prop3, const std::function<void()>& fn = nullptr,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);

	// Adds a Slider Int Property.
	void LE_AddSliderIntProperty(const std::string& name, int& prop, int min, int max,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);
	// Adds a Slider Float Property.
	void LE_AddSliderFloatProperty(const std::string& name, float& prop, float min, float max,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue);

	// Adds a Textbox Input.
	void LE_AddInputText(const std::string& name, std::string& text, unsigned maxLen,
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue,
		const std::function<void()>& fn = nullptr, int(*callback)(ImGuiInputTextCallbackData*) = nullptr, void* obj = nullptr);

	// Adds a multiline Textbox Input.
	void LE_AddInputMultiText(const std::string& name, std::string& text, unsigned maxLen, ImVec2 size = ImVec2(0, 0),
		const ImGuiInputTextFlags& flag = ImGuiInputTextFlags_EnterReturnsTrue,
		const std::function<void()>& fn = nullptr, int(*callback)(ImGuiInputTextCallbackData*) = nullptr, void* obj = nullptr);


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

	void LE_AddCollapsingHeader(const std::string& name, const std::vector<std::function<void()>>& fns = {});

	void LE_AddTreeNodes(const std::string& name, const std::vector<std::function<void()>> fns = {}, ImGuiTreeNodeFlags flag = 0);

	/**********************************************************************/

	/******************************* Hover ***************************************/

	// Put after the thing you want to have a tooltip
	// Allowed to add additional stuff to run in tooltip
	void LE_AddTooltip(const std::string& tip, const std::function<void()>& fn = nullptr);
	// Default Help Marker - Only shows a tooltip for help
	void LE_AddHelpMarker(const std::string& tip);

	/**********************************************************************/

	/******************************* Graphing ***************************************/

	// Shows a vector of floats as a line graph. If addData is true, automatically pushes back new data and pops the first (like a queue)
	void LE_AddPlotLines(const std::string& name, std::vector<float>& graph, bool addData = false, const float& newData = 0);

	void LE_AddHistogram(const std::string& name, std::vector<float>& graph, bool addData = false, const float& newData = 0, float min = 0, float max = 1, ImVec2 size = ImVec2());

	void LE_AddProgressBar(float progress, const ImVec2& size = ImVec2(0, 0), std::string overlay = {});

	/**********************************************************************/

	/******************************* DragDrop ***************************************/

	// ID will be used for dragdrop target
	// data is an object that contains the information that will be passed through
	// fn is any additional actions to be taken during drag drop
	// Please indicate type in angled brackets to be safe - eg. String literals are NOT strings "xxx" =/= std::string
	// Place this after the item you want to drag from
	template <typename T>
	void LE_AddDragDropSource(const std::string& id, T* data, const std::function<void()>& fn = nullptr, const ImGuiDragDropFlags& flag = 0)
	{
		// Our buttons are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(flag))
		{
			// Set payload to carry the index of our item (could be anything)
			ImGui::SetDragDropPayload(id.c_str(), data, sizeof(T));

			if (fn)
				fn();

			ImGui::EndDragDropSource();
		}
	}

	// ID determines checks what payload it receives
	// Function MUST take in the payload type's parameter, payload is a reference
	// Please be very careful with Drag Drop source and Target
	// the fn determines what will be done with the data
	// Place this after the item you want to drag to
	template <typename T>
	void LE_AddDragDropTarget(const std::string& id, const std::function<void(const T*)>& fn = nullptr, const ImGuiDragDropFlags& flag = 0)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id.c_str(), flag))
			{
				IM_ASSERT(payload->DataSize == sizeof(T));
				T payload_n = *(const T*)payload->Data;
				if (fn)
					fn(&payload_n);
			}
			ImGui::EndDragDropTarget();
		}
	}

	/**********************************************************************/

	/******************************* Popups ***************************************/

	// Adds a button that runs a popup.
	void LE_AddPopupButton(const std::string& name, const std::function<void()>& fn = nullptr,
		const ImVec2& size = ImVec2(0, 0), const ImGuiPopupFlags& popupFlag = 0, const ImGuiWindowFlags& winFlag = 0);

	// Right click to open a Context Menu (Similar to a popup.)
	void LE_AddContext(const std::string& name, const std::function<void()>& fn = nullptr, const ImGuiPopupFlags& popupFlag = 1);

	// Opens a Modal Window. Default OK and Cancel options only
	// fn represents the function to run within the Modal window
	// buttonFn is a pair<string, void()> that determines the button functions at the end; Default all in the same line
	void LE_AddModal(const std::string& name, const std::function<void()>& fn = nullptr,
		const std::vector < std::pair<std::string, std::function<void()>>>& buttonFn = {},
		const ImVec2& size = ImVec2(0, 0), const ImGuiPopupFlags& popupFlag = 0, const ImGuiWindowFlags& winFlag = 0);

	// Runs if hovering over previous
	void LE_AddHover(const std::function<void()>& fn = nullptr);

	/**********************************************************************/

	/******************************* Others ***************************************/

	void LE_AddStyleVar(const ImGuiStyleVar& var, const float& val, const std::function<void()>& fn = nullptr);

	/**********************************************************************/
};

#endif

