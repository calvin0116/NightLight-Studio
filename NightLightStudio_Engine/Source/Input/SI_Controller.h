#ifndef SI_SYSTEM_INPUT_CONTROLLER
#define SI_SYSTEM_INPUT_CONTROLLER

#include "..\..\framework.h"

#include <Windows.h>
// XInput
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")
#include <map>
#include <vector>
#include <set>
#include <functional>

#include <string>

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

	struct ENGINE_API ControllerVec2
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
		template <typename CTYPE>
		struct CONTROLLER_BASE_EVENT
		{
			unsigned int _button;
			CTRLTrigger _trigger;

			CTYPE _event;

			CONTROLLER_BASE_EVENT(unsigned int button, CTRLTrigger trigger, CTYPE eve)
				: _button(button), _trigger(trigger), _event(eve) {}
		};

		bool _allowControllers;
		int _controllerID;
		XINPUT_STATE _controllerState;
		std::map<unsigned int, unsigned int> _buttonStates;

		std::map<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_EVENT>> _buttonEvents;
		std::map<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_TRIGGER_EVENT>> _triggerEvents;
		std::map<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_ANALOG_EVENT>> _analogEvents;


		//std::multimap<std::pair<unsigned int, unsigned int>, CONTROLLER_EVENT> _buttonEvents;

		//std::multimap<unsigned int, CONTROLLER_TRIGGER_EVENT> _triggerEvents;
		//std::multimap<unsigned int, CONTROLLER_ANALOG_EVENT> _analogEvents;

		//ONLY TO PREVENT CHECKING WHEN OUT OF WINDOW, MAY BE REMOVED/REPLACED
		const HWND _window = GetForegroundWindow();

	public:
		ENGINE_API SystemController(int _controllerID = -1, bool allowController = false);
		ENGINE_API ~SystemController() = default;

		// Resets all Controllers
		ENGINE_API void ResetController(bool allowController = false);

		// Checks directly if a button is down on the controller
		ENGINE_API bool CTRLButtonDown(unsigned int buttonDown);

		// Checks if button is presssed
		// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
		ENGINE_API bool GetIfButtonPress(unsigned int buttonPressed);
		// Checks if button is held
		// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
		ENGINE_API bool GetIfButtonHeld(unsigned int buttonPressed);
		// Checks if button is released
		// CHECKS ONLY IF REGISTERED AS PART OF AN EVENT
		ENGINE_API bool GetIfButtonRelease(unsigned int buttonPressed);
		// Enables all buttons
		ENGINE_API void ALLBUTTONS();


		// Left Trigger = 0, Right Trigger = 1, all other values default to Left
		ENGINE_API float GetTrigger(int lr = 0);
		// Left Analog Stick = 0, Right Analog Stick = 1, all other values default to Left
		ENGINE_API ControllerVec2 GetAnalog(int lr = 0);

		// Creates new Event (Member Functions)
		template <typename T, typename U>
		ENGINE_API void CreateNewEvent(std::string name, unsigned int button, CTRLTrigger trig, void(T::* func)(), U* obj)
		{
			_buttonEvents.emplace(std::pair<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_EVENT>>
				(std::make_pair(name, CONTROLLER_BASE_EVENT<CONTROLLER_EVENT>(button, trig, CtrlEvent_MemberFunc(func, obj)))));

			_buttonStates.emplace(button, 0);
		}
		// Creates new Event (Static Functions)
		ENGINE_API void CreateNewEvent(std::string name, unsigned int button, CTRLTrigger trig, CONTROLLER_EVENT func = nullptr)
		{
			_buttonEvents.emplace(std::pair<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_EVENT>>
				(std::make_pair(name, CONTROLLER_BASE_EVENT<CONTROLLER_EVENT>(button, trig, func))));

			_buttonStates.emplace(button, 0);
		}

		// Creates new Trigger Event (Member Functions)
		// lr -> Left Trigger = 0, Right Trigger = 1
		template <typename T, typename U>
		ENGINE_API void CreateNewTriggerEvent(std::string name, unsigned int lr, void(T::* func)(float), U* obj)
		{
			_triggerEvents.emplace(std::pair<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_TRIGGER_EVENT>>
				(std::make_pair(name, CONTROLLER_BASE_EVENT<CONTROLLER_TRIGGER_EVENT>
					(lr, OnCTRLDefault, CtrlEventTrigger_MemberFunc(func, obj)))));
		}
		// Creates new Event (Static Functions)
		// lr -> Left Trigger = 0, Right Trigger = 1
		ENGINE_API void CreateNewTriggerEvent(std::string name, unsigned int lr, CONTROLLER_TRIGGER_EVENT func = nullptr)
		{
			_triggerEvents.emplace(std::pair<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_TRIGGER_EVENT>>
				(std::make_pair(name, CONTROLLER_BASE_EVENT<CONTROLLER_TRIGGER_EVENT>(lr, OnCTRLDefault, func))));
		}

		// Creates new Analog Event (Member Functions)
		// lr -> Left Analog Stick = 0, Right Analog Stick = 1
		template <typename T, typename U>
		ENGINE_API void CreateNewAnalogEvent(std::string name, unsigned int lr, void(T::* func)(float, float), U* obj)
		{
			_analogEvents.emplace(std::pair<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_ANALOG_EVENT>>
				(std::make_pair(name, CONTROLLER_BASE_EVENT<CONTROLLER_ANALOG_EVENT>
					(lr, OnCTRLDefault, CtrlEventAnalog_MemberFunc(func, obj)))));
		}
		// Creates new Event (Static Functions)
		// lr -> Left Analog Stick = 0, Right Analog Stick = 1
		ENGINE_API void CreateNewAnalogEvent(std::string name, unsigned int lr, CONTROLLER_ANALOG_EVENT func = nullptr)
		{
			_analogEvents.emplace(std::pair<std::string, CONTROLLER_BASE_EVENT<CONTROLLER_ANALOG_EVENT>>
				(std::make_pair(name, CONTROLLER_BASE_EVENT<CONTROLLER_ANALOG_EVENT>(lr, OnCTRLDefault, func))));
		}

		// Changes Key that triggers the Event
		ENGINE_API void ChangeEventKey(const std::string& name, unsigned int button);

		// Removes the Event from Registered Events
		// Do remove the Event IF you are using member functions and it is about to be destroyed
		ENGINE_API void RemoveEvent(const std::string& name);

		// Removes the Event from Registered Trigger Events
		// Do remove the Event IF you are using member functions and it is about to be destroyed
		ENGINE_API void RemoveTriggerEvent(const std::string& name);

		// Removes the Event from Registered Analog Events
		// Do remove the Event IF you are using member functions and it is about to be destroyed
		ENGINE_API void RemoveAnalogEvent(const std::string& name);

		ENGINE_API bool Update(float dt = 0);
	};
}

#endif
