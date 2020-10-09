#ifndef LEVELEDITOR_WINDOWCLASSES
#define LEVELEDITOR_WINDOWCLASSES

#include "LevelEditor.h"
#include "LevelEditor_Console.h"

// REMOVE EVERYTHING LATER
// HOLDS THE .h FILES FOR ALL THE OTHER CLASSES
// TESTING ONLY

void testFunc()
{
	ImGui::Text("TESTING");
}

class TestCase : public LE_WinBase_Derived<TestCase>
{
	int a = 0;
	float b = 1;
	float c = 2;
	float d = 3;
	float e = 4;
	std::string myName = "MyName";
	std::string secondName = "MyName2";
	char testing[200] = {};

	bool greyedout = false;

	std::vector<float> someData = { 1.0f,2.0f,3.0f,4.0f,3.0f,2.0f,1.0f,0.0f };
public:
	TestCase() = default;
	~TestCase() = default;

	void Init() override
	{
		ImGui::SetNextWindowBgAlpha(1.0f);
	}

	void Run() override
	{
		_levelEditor->LE_AddText("This is some useful text.");
		//_levelEditor->LE_AddChildWindow("ChildWindow", testFunc, true);
		//_levelEditor->LE_AddButton("TEST", nullptr);
		//_levelEditor->LE_AddButton<TestCase>("Button2", &TestCase::buttonTest, this);
		std::vector<std::string> items = { "a", "b", "c", "d", "e" };
		//std::vector<void(*)()> itemFuncs = { testFunc, testFunc, testFunc };
		//std::vector<void(TestCase::*)()> memberfuncs = {&TestCase::textTest, &TestCase::textTest, &TestCase::textTest };
		//_levelEditor->LE_AddMenu("ITEMS", items);

		//_levelEditor->LE_AddMenu("ITEMS2", items, itemFuncs);

		//_levelEditor->LE_AddMenu<TestCase>("ITEMS3", items, memberfuncs, this);
		//_levelEditor->LE_AddInputFloatProperty("INPUT1", a, ImGuiInputTextFlags_EnterReturnsTrue);
		//_levelEditor->LE_AddInputFloat2Property("INPUT2", a, b, ImGuiInputTextFlags_EnterReturnsTrue);
		//_levelEditor->LE_AddInputFloat3Property("INPUT3", a, b, c, ImGuiInputTextFlags_EnterReturnsTrue);

		//_levelEditor->LE_AddSliderFloatProperty("SLIDER", b, 0, 100);
		//_levelEditor->LE_AddCombo("TEST", a, items);
		//std::function<void()> func1 = std::bind(std::bind([&](LevelEditor* editor) {(editor)->LE_AddCollapsingHeader("THIS"); }, _levelEditor));
		//_levelEditor->LE_AddCollapsingHeader("Header", {func1});
		//_levelEditor->LE_AddTreeNodes("Trees", { func1 });

		// Like wtf is this shit
		//_levelEditor->LE_AddMenuOnly("First", 
		//	[&]() {_levelEditor->LE_AddMenuOnly("Second", 
		//		[&]() {_levelEditor->LE_AddMenuOnly("Third",
		//			[&]() 
		//			{
		//				_levelEditor->LE_AddMenuWithItems("Last", { "someitem" }, {}, _levelEditor->LE_BindMemberFuncsVec({ &TestCase::textTest }, this)); 
		//			}); }); });
		//_levelEditor->LE_AddMenuWithItems("Second", { "someitem" }, {}, _levelEditor->LE_BindMemberFuncsVec({ &TestCase::textTest }, this));
		
		//_levelEditor->LE_AddPlotLines("Graph", someData, true, b);
		//_levelEditor->LE_AddHistogram("Graph", someData, true, b);
		//b += rand() / 10000;
		//b -= rand() / 10000;

		//_levelEditor->LE_AddHelpMarker("This here is helping");

		//_levelEditor->LE_AddInputText("SOME NAME", myName, 100);

		//_levelEditor->LE_AddColorEdit3("Colors", b, c, d);
		//_levelEditor->LE_AddColorEdit4("Colors2", b, c, d, e);

		std::string dragdrop("TESTING");

		_levelEditor->LE_AddButton(myName.c_str());
		_levelEditor->LE_AddDragDropSource("PAYLOAD", &dragdrop);

		_levelEditor->LE_AddButton(secondName);
		std::function<void(const std::string*)> fn = [&](const std::string* t) { secondName = (*t); };
		_levelEditor->LE_AddDragDropTarget("PAYLOAD", fn);

		ImGuiIO& io = ImGui::GetIO();
		ImTextureID my_tex_id = io.Fonts->TexID;
		_levelEditor->LE_AddImage(my_tex_id, ImVec2(100, 100));
		_levelEditor->LE_AddTooltip("TIP TIP");

		/*
		if (greyedout)
		{
			_levelEditor->LE_AddStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f, 
				std::bind(&LevelEditor::LE_AddArrowButton, _levelEditor, "##ArrowButton", ImGuiDir_Right, 
					[&]()
					{
						greyedout = !greyedout;
					}));
		}
		else
		{
			_levelEditor->LE_AddArrowButton("##ArrowButton", ImGuiDir_Right, [&]()
				{
					greyedout = !greyedout;
				});
		}
		*/
		//_levelEditor->LE_AddSelectable("Selectable", &greyedout);//, [&]() { greyedout = !greyedout; });

		//ImGui::InputTextMultiline("TEST", testing, 200);

		_levelEditor->LE_AddProgressBar(-1.0f, ImVec2(100, 100));
		//_levelEditor->LE_AddButton("Button");
		_levelEditor->LE_AddContext("Context", [&]() {_levelEditor->LE_AddText("TEST"); });
		ImGui::IsItemHovered();

		//_levelEditor->LE_AccessWindowFunc("Console", &ConsoleLog::AddItem, std::string("Console Item"));

		_levelEditor->LE_AddText(std::to_string(a));

		_levelEditor->LE_AddButton("bindButton", _levelEditor->LE_BindMemFuncs(&TestCase::paramTest, this, a));
		_levelEditor->LE_BindMemFuncsVec({ &TestCase::textTest }, this);
		//_levelEditor->LE_AddPopupButton("Popup", [&]() {_levelEditor->LE_AddText("BUTTONPOPUP"); });
		/*
		_levelEditor->LE_AddModal("ModalTest", 
			// Modal Window func
			[&]() 
			{
				_levelEditor->LE_AddText("TEST");

				_levelEditor->LE_AddModal("ModalTest2",
					// Modal Window func
					[&]()
					{
						_levelEditor->LE_AddText("TEST2");
					},
					// Buttons
					{
						std::make_pair("OK2", [&]()
						{
						}),
						std::make_pair("Cancel2", [&]()
						{
						})
					}, ImVec2(0, 0), 0, ImGuiWindowFlags_AlwaysAutoResize);
			},
			// Buttons
			{
				std::make_pair("OK", [&]()
				{
				}),
				std::make_pair("Cancel", [&]()
				{
				})
			}, ImVec2(0,0), 0, ImGuiWindowFlags_AlwaysAutoResize);
			*/
	}
	void buttonTest()
	{
		_levelEditor->LE_AddText("Button2 working");
	}
	void textTest()
	{
		a++;
	}

	void paramTest(int)
	{
		textTest();
	}
};

#endif
