#ifndef SI_SYSTEM_INPUT_KEYPRESS
#define SI_SYSTEM_INPUT_KEYPRESS

#include "../../framework.h"

#include <Windows.h>
#include <functional>
#include <map>
#include <string>
#include <array>

namespace SystemInput_ns
{
	/*=========== Key Codes for all buttons. Still can use regular Virtual Codes, Just not registered here. =============*/
	enum VirtualKey : int
	{
		// Mouse Input
		IMOUSE_LBUTTON = 0x01,
		IMOUSE_RBUTTON = 0x02,
		IMOUSE_MBUTTON = 0x04,

		// Keyboard Input
		// Functions
		IKEY_BACK = 0x08,
		IKEY_TAB = 0x09,
		IKEY_CLEAR = 0x0C,
		IKEY_RETURN = 0x0D,
		IKEY_SHIFT = 0x10,
		IKEY_CTRL = 0x11,
		IKEY_ALT = 0x12,

		IKEY_PAUSE = 0x13,
		IKEY_CAPS = 0x14,

		IKEY_ESCAPE = 0x1B,
		IKEY_SPACE = 0x20,

		IKEY_PGUP = 0x21,
		IKEY_PGDWN = 0x22,
		IKEY_END = 0x23,
		IKEY_HOME = 0x24,

		IKEY_LEFT = 0x25,
		IKEY_UP = 0x26,
		IKEY_RIGHT = 0x27,
		IKEY_DOWN = 0x28,

		IKEY_PRINT = 0x2A,
		IKEY_PRNTSCRN = 0x2C,
		IKEY_INSERT = 0x2D,
		IKEY_DELETE = 0x2E,

		// Numbers
		IKEY_0 = 0x30,
		IKEY_1,
		IKEY_2,
		IKEY_3,
		IKEY_4,
		IKEY_5,
		IKEY_6,
		IKEY_7,
		IKEY_8,
		IKEY_9,


		// Characters
		IKEY_A = 0x41,
		IKEY_B,
		IKEY_C,
		IKEY_D,
		IKEY_E,
		IKEY_F,
		IKEY_G,
		IKEY_H,
		IKEY_I,
		IKEY_J,
		IKEY_K,
		IKEY_L,
		IKEY_M,
		IKEY_N,
		IKEY_O,
		IKEY_P,
		IKEY_Q,
		IKEY_R,
		IKEY_S,
		IKEY_T,
		IKEY_U,
		IKEY_V,
		IKEY_W,
		IKEY_X,
		IKEY_Y,
		IKEY_Z,

		IKEY_NUMPAD_0 = 0x60,
		IKEY_NUMPAD_1,
		IKEY_NUMPAD_2,
		IKEY_NUMPAD_3,
		IKEY_NUMPAD_4,
		IKEY_NUMPAD_5,
		IKEY_NUMPAD_6,
		IKEY_NUMPAD_7,
		IKEY_NUMPAD_8,
		IKEY_NUMPAD_9,

		IKEY_F1 = 0x70,
		IKEY_F2,
		IKEY_F3,
		IKEY_F4,
		IKEY_F5,
		IKEY_F6,
		IKEY_F7,
		IKEY_F8,
		IKEY_F9,
		IKEY_F10,
		IKEY_F11,
		IKEY_F12,

		IKEY_NUMLOCK = 0x90,
		IKEY_SCROLLOCK,

		IKEY_LSHIFT = 0xA0,
		IKEY_RSHIFT,
		IKEY_LCTRL,
		IKEY_RCTRL,

		IKEY_FINAL_VALUE = 256 // DO NOT USE: Determines limit of virtual codes used.
	};

	// Internal
	namespace
	{
		typedef std::function<void()> INPUT_EVENT;

		// FOR USE WITH MEMBER FUNCTIONS
		template <typename T, typename Y>
		INPUT_EVENT InputEvent_MemberFunc(void(T::* memFunc)(), Y* myObj)
		{
			std::function<void(void)> newMemFn = std::bind(memFunc, myObj);
			return newMemFn;
		}
	}

	// Bits used to check current state of key press
	enum Trigger : unsigned int
	{
		OnDefault = 0,
		OnPress = 0x01,
		OnHold = 0x02,
		OnRelease = 0x04,
		OnDelay = 0x08
	};

	// Hexadecimal bit used to check if key is being pressed
	enum : int
	{
		KeyDown = 0x80,
		KeyToggled = 0x01
	};

	// Class that controls Key presses
	class SystemKeyPress
	{
		// Event struct used to store keypress to event
		struct InputEventStruct
		{
			unsigned int _keycode;
			std::map<std::string, std::pair<INPUT_EVENT, Trigger>> _eventFuncs;

			InputEventStruct(unsigned int k)
				: _keycode{ k } {}
			InputEventStruct(unsigned int k, std::string identifier, Trigger t, INPUT_EVENT e)
				: _keycode{ k }
			{
				std::pair<INPUT_EVENT, Trigger> pair(e, t);
				_eventFuncs.emplace(identifier, pair);
			}
			~InputEventStruct() = default;

			void AddNewEvent(std::string identifier, Trigger t, INPUT_EVENT e)
			{
				std::pair<INPUT_EVENT, Trigger> pair(e, t);
				_eventFuncs.emplace(identifier, pair);
			}


		};

		// Map of used events
		std::map<std::string, InputEventStruct> _events;
		// Array to store keyboard state obtained from Windows
		std::array<BYTE, 256> _keyboardState;
		// Array that stores current actual state of keys
		std::array<unsigned char, 256> _keyboardPressState;
		// Map that stores key codes that are being used
		std::map<unsigned int, int> _usedKeyCodes;

		//ONLY TO PREVENT CHECKING WHEN OUT OF WINDOW, MAY BE REMOVED/REPLACED
		const HWND _window = GetForegroundWindow();

		ENGINE_API void IncrementKeyUsed(unsigned int key);
		ENGINE_API void DecrementKeyUsed(unsigned int key);

	public:
		SystemKeyPress();
		~SystemKeyPress() = default;

		ENGINE_API bool Update(float dt = 0);

		// Checks only registered keys in use
		ENGINE_API bool GetKeyPress(unsigned int keycode);
		// Checks only registered keys in use
		ENGINE_API bool GetKeyHold(unsigned int keycode);
		// Checks only registered keys in use
		ENGINE_API bool GetKeyRelease(unsigned int keycode);
		// Checks if key is toggled - Usually for caps lock etc.
		ENGINE_API bool GetKeyToggle(unsigned int keycode);

		// Checks directly from hardware state
		ENGINE_API bool GetKeyStateHold(unsigned int keycode);


		// Creates new Event (Member Functions)
		template <typename T, typename U>
		ENGINE_API void CreateNewEvent(const std::string& name, unsigned int keycode, std::string identifier, Trigger val, void(T::* func)(), U* obj)
		{
			_events.emplace(std::pair<std::string, InputEventStruct>(name, InputEventStruct(keycode, identifier, val, InputEvent_MemberFunc(func, obj))));

			IncrementKeyUsed(keycode);
		}
		// Creates new Event (Static Functions)
		ENGINE_API void CreateNewEvent(const std::string& name, unsigned int keycode, std::string identifier, Trigger val, INPUT_EVENT func)
		{
			_events.emplace(std::pair<std::string, InputEventStruct>(name, InputEventStruct(keycode, identifier, val, func)));

			IncrementKeyUsed(keycode);
		}
		// Creates new Event (Empty Event)
		ENGINE_API void CreateNewEvent(const std::string& name, unsigned int keycode)
		{
			_events.emplace(std::pair<std::string, InputEventStruct>(name, InputEventStruct(keycode)));

			IncrementKeyUsed(keycode);
		}


		// Add a new function to an Event
		// Reminder to call RemoveAttachedFunction if you are attaching a member function and said object is being destroyed
		template <typename T, typename U>
		ENGINE_API void AddFunctionToEvent(const std::string& name, std::string identifier, Trigger t, void(T::* func)(), U* obj)
		{
			std::map<const std::string, InputEventStruct>::iterator temp = _events.find(name);
			if (temp != std::end(_events))
				temp->second.AddNewEvent(identifier, t, InputEvent_MemberFunc(func, obj));
		}

		// Add a new function to an Event
		// Reminder to call RemoveAttachedFunction if you are attaching a member function and said object is being destroyed
		ENGINE_API void AddFunctionToEvent(const std::string& name, std::string identifier, Trigger t, INPUT_EVENT func)
		{
			std::map<const std::string, InputEventStruct>::iterator temp = _events.find(name);
			if (temp != std::end(_events))
				temp->second.AddNewEvent(identifier, t, func);
		}

		// Changes Key that triggers the Event
		ENGINE_API void ChangeEventKey(const std::string& name, unsigned int keycode);

		// Removes the Event from Registered Events
		ENGINE_API void RemoveEvent(const std::string& name);

		// Removes a function attached to an Event
		// Reminder to use this if you attach a member function to an Event
		ENGINE_API void RemoveAttachedFunction(const std::string& name, std::string identifier);

		// Gets the Function that the Event runs
		// Returns nullptr if unable to find
		ENGINE_API INPUT_EVENT GetInputEvent(const std::string& name, std::string identifier);

		// All keys are now registered
		// Use at your own peril
		ENGINE_API void ALL_THE_KEYS();

		// All keys are now deregistered
		// Use at your own peril
		ENGINE_API void NO_KEYS();

		// Restores Event Keys so that they will be checked - In case you use NO_KEYS for some reason
		// Completely untested, but it should work
		ENGINE_API void Restore_EventKeys();
	};
}

#endif