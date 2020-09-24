#ifndef SI_SYSTEM_INPUT_CONTROLLER
#define SI_SYSTEM_INPUT_CONTROLLER

#include <Windows.h>
// XInput
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")
#include <map>
#include <vector>
#include <set>
#include <functional>

namespace SystemInput_ns
{
	/*=========== Key Codes for Controller Buttons. Note that this is XINPUT (XBOX CONTROLLER) =============*/
	/*=========== Dualshock/PS Buttons included are default locations in comparison to XBOX =============*/
	/*=========== Requires some sort of Dualshock to XBOX converter for Dualshock Controllers =============*/
	enum GamepadKey : unsigned int
	{
		GAMEPAD_A = XINPUT_GAMEPAD_A,
		GAMEPAD_B = XINPUT_GAMEPAD_B,
		GAMEPAD_X = XINPUT_GAMEPAD_X,
		GAMEPAD_Y = XINPUT_GAMEPAD_Y,

		GAMEPAD_CROSS = XINPUT_GAMEPAD_A,
		GAMEPAD_CIRCLE = XINPUT_GAMEPAD_B,
		GAMEPAD_SQUARE = XINPUT_GAMEPAD_X,
		GAMEPAD_TRIANGLE = XINPUT_GAMEPAD_Y,

		GAMEPAD_DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
		GAMEPAD_DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		GAMEPAD_DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		GAMEPAD_DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,

		GAMEPAD_SHOULDER_LEFT = XINPUT_GAMEPAD_LEFT_SHOULDER,
		GAMEPAD_SHOULDER_RIGHT = XINPUT_GAMEPAD_RIGHT_SHOULDER,

		GAMEPAD_THUMB_LEFT = XINPUT_GAMEPAD_LEFT_THUMB,
		GAMEPAD_THUMB_RIGHT = XINPUT_GAMEPAD_RIGHT_THUMB,

		GAMEPAD_BACK = XINPUT_GAMEPAD_BACK,
		GAMEPAD_START = XINPUT_GAMEPAD_START,

		GAMEPAD_SHARE = XINPUT_GAMEPAD_BACK,
		GAMEPAD_OPTIONS = XINPUT_GAMEPAD_START,
	};

	// Internal
	namespace
	{
		typedef std::function<void(float, float)> CONTROLLER_ANALOG_EVENT;
		typedef std::function<void(float)> CONTROLLER_TRIGGER_EVENT;
		typedef std::function<void()> CONTROLLER_EVENT;

		// FOR USE WITH MEMBER FUNCTIONS
		template <typename T, typename Y>
		CONTROLLER_EVENT CtrlEvent_MemberFunc(void(T::* memFunc)(), Y* myObj)
		{
			std::function<void()> newMemFn = std::bind(memFunc, myObj);
			return newMemFn;
		}

		// FOR USE WITH MEMBER FUNCTIONS
		template <typename T, typename Y>
		CONTROLLER_TRIGGER_EVENT CtrlEventTrigger_MemberFunc(void(T::* memFunc)(float), Y* myObj)
		{
			std::function<void(float)> newMemFn = std::bind(memFunc, myObj, std::placeholders::_1);
			return newMemFn;
		}

		// FOR USE WITH MEMBER FUNCTIONS
		template <typename T, typename Y>
		CONTROLLER_ANALOG_EVENT CtrlEventAnalog_MemberFunc(void(T::* memFunc)(float, float), Y* myObj)
		{
			std::function<void(float, float)> newMemFn = std::bind(memFunc, myObj, std::placeholders::_1, std::placeholders::_2);
			return newMemFn;
		}
	}

	struct ControllerVec2
	{
		float _x;
		float _y;
	};

	// Bits used to check current state of key press
	enum CTRLTrigger : unsigned int
	{
		OnCTRLDefault = 0,
		OnCTRLPress = 0x01,
		OnCTRLHold = 0x02,
		OnCTRLRelease = 0x04,
		OnCTRLDelay = 0x08
	};

	class SystemController
	{
		bool _allowControllers;
		int _controllerID;
		XINPUT_STATE _controllerState;
		std::map<unsigned int, unsigned int> _buttonStates;

		std::multimap<std::pair<unsigned int, unsigned int>, CONTROLLER_EVENT> _buttonEvents;

		std::multimap<unsigned int, CONTROLLER_TRIGGER_EVENT> _triggerEvents;
		std::multimap<unsigned int, CONTROLLER_ANALOG_EVENT> _analogEvents;

		//ONLY TO PREVENT CHECKING WHEN OUT OF WINDOW, MAY BE REMOVED/REPLACED
		const HWND _window = GetForegroundWindow();

	public:
		SystemController(int _controllerID = -1, bool allowController = false);
		~SystemController() = default;

		// Resets all Controllers
		void ResetController(bool allowController = false);

		// Checks directly if a button is down on the controller
		bool CTRLButtonDown(unsigned int buttonDown);

		// Checks if button is presssed
		// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
		bool GetIfButtonPress(unsigned int buttonPressed);
		// Checks if button is held
		// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
		bool GetIfButtonHeld(unsigned int buttonPressed);
		// Checks if button is released
		// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
		bool GetIfButtonRelease(unsigned int buttonPressed);
		// Enables all buttons
		void ALLBUTTONS();


		// Left Trigger = 0, Right Trigger = 1, all other values default to Left
		float GetTrigger(int lr = 0);
		// Left Analog Stick = 0, Right Analog Stick = 1, all other values default to Left
		ControllerVec2 GetAnalog(int lr = 0);

		// Creates new Event (Member Functions)
		template <typename T, typename U>
		void CreateNewEvent(unsigned int button, CTRLTrigger trig, void(T::* func)(), U* obj)
		{
			_buttonEvents.emplace(std::pair<std::pair<unsigned int, unsigned int>, CONTROLLER_EVENT>
				(std::make_pair(button, trig), CtrlEvent_MemberFunc(func, obj)));

			_buttonStates.emplace(button, 0);
		}
		// Creates new Event (Static Functions)
		void CreateNewEvent(unsigned int button, CTRLTrigger trig, CONTROLLER_EVENT func = nullptr)
		{
			_buttonEvents.emplace(std::pair<std::pair<unsigned int, unsigned int>, CONTROLLER_EVENT>
				(std::make_pair(button, trig), func));

			_buttonStates.emplace(button, 0);
		}

		// Creates new Trigger Event (Member Functions)
		// lr -> Left Trigger = 0, Right Trigger = 1
		template <typename T, typename U>
		void CreateNewTriggerEvent(unsigned int lr, void(T::* func)(float), U* obj)
		{
			_triggerEvents.emplace(std::pair<unsigned int, CONTROLLER_TRIGGER_EVENT>
				(lr, CtrlEventTrigger_MemberFunc(func, obj)));
		}
		// Creates new Event (Static Functions)
		// lr -> Left Trigger = 0, Right Trigger = 1
		void CreateNewTriggerEvent(unsigned int lr, CONTROLLER_TRIGGER_EVENT func = nullptr)
		{
			_triggerEvents.emplace(std::pair<unsigned int, CONTROLLER_TRIGGER_EVENT>
				(lr, func));
		}

		// Creates new Analog Event (Member Functions)
		// lr -> Left Analog Stick = 0, Right Analog Stick = 1
		template <typename T, typename U>
		void CreateNewAnalogEvent(unsigned int lr, void(T::* func)(float, float), U* obj)
		{
			_analogEvents.emplace(std::pair<unsigned int, CONTROLLER_ANALOG_EVENT>
				(lr, CtrlEventAnalog_MemberFunc(func, obj)));
		}
		// Creates new Event (Static Functions)
		// lr -> Left Analog Stick = 0, Right Analog Stick = 1
		void CreateNewAnalogEvent(unsigned int lr, CONTROLLER_ANALOG_EVENT func = nullptr)
		{
			_analogEvents.emplace(std::pair<unsigned int, CONTROLLER_ANALOG_EVENT>
				(lr, func));
		}

		bool Update(float dt = 0);
	};
}

#endif
