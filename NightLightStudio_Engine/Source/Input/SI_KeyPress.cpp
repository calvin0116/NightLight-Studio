#include "SI_KeyPress.h"

namespace NS_INPUT
{
	//********************************************** SYSTEM KEY PRESS **********************************************//

	void SystemKeyPress::_IncrementKeyUsed(unsigned int key)
	{
		_usedKeyCodes[key]++;
	}

	void SystemKeyPress::_DecrementKeyUsed(unsigned int key)
	{
		_usedKeyCodes[key]--;
		if (!_usedKeyCodes[key])
			_usedKeyCodes.erase(key);
	}

	SystemKeyPress::SystemKeyPress() : _events{}, _keyboardState{}, _keyboardPressState{}, _usedKeyCodes{}
	{
	}

	bool SystemKeyPress::Update(float)
	{
		// Prevents input when out of focus
		if (GetForegroundWindow() != _window)
			return false;

		// Needed for Keyboard state to run for some reason?
		GetKeyState(0);
		//GetAsyncKeyState(0);
		GetKeyboardState(_keyboardState.data());

		// Checks only Keycodes that are registered to optimize
		for (std::pair<unsigned int, int> keycodes : _usedKeyCodes)
		{
			unsigned int key = keycodes.first;
			if (GetKeyStateHold(key))
			{
				if (!_keyboardPressState[key])
					_keyboardPressState[key] = OnPress;
				else
					_keyboardPressState[key] = OnHold;
			}
			else
			{
				if (_keyboardPressState[key])
				{
					if (_keyboardPressState[key] != OnRelease)
						_keyboardPressState[key] = OnRelease;
					else
						_keyboardPressState[key] = 0;
				}
			}
		}

		// Checks key and trigger to see if it needs to run event
		for (std::pair<const std::string, InputEventStruct> pair : _events)
		{

			unsigned int key = pair.second._keycode;

			for (std::pair<const std::string, std::pair<INPUT_EVENT, Trigger>> eventPair : pair.second._eventFuncs)
			{
				Trigger trig = eventPair.second.second;

				switch (trig)
				{
				case OnPress:
					if (GetKeyPress(key))
						if (eventPair.second.first)
							eventPair.second.first();
					break;
				case OnHold:
					if (GetKeyHold(key))
						if (eventPair.second.first)
							eventPair.second.first();
					break;
				case OnRelease:
					if (GetKeyRelease(key))
						if (eventPair.second.first)
							eventPair.second.first();
					break;
				default:
					break;
				}
			}

		}

		return false;
	}

	bool SystemKeyPress::GetKeyStateHold(unsigned int keycode)
	{
		return (_keyboardState[keycode] & KeyDown);
	}

	bool SystemKeyPress::GetKeyPress(unsigned int keycode)
	{
		return (_keyboardPressState[keycode] & OnPress);
	}
	bool SystemKeyPress::GetKeyHold(unsigned int keycode)
	{
		return (_keyboardPressState[keycode] & OnHold);
	}
	bool SystemKeyPress::GetKeyRelease(unsigned int keycode)
	{
		return (_keyboardPressState[keycode] & OnRelease);
	}

	bool SystemKeyPress::GetKeyToggle(unsigned int keycode)
	{
		return (_keyboardState[keycode] & KeyToggled);
	}

	void SystemKeyPress::ChangeEventKey(const std::string& name, unsigned int keycode)
	{
		std::map<const std::string, InputEventStruct>::iterator temp = _events.find(name);
		if (temp != std::end(_events))
		{
			// Removes previous key from events if necessary
			unsigned int key = temp->second._keycode;
			_DecrementKeyUsed(key);

			// Changes key and adds it to used key codes
			temp->second._keycode = keycode;
			_IncrementKeyUsed(keycode);
		}
	}


	void SystemKeyPress::RemoveEvent(const std::string& name)
	{
		std::map<const std::string, InputEventStruct>::iterator temp = _events.find(name);
		if (temp != std::end(_events))
		{
			unsigned int key = temp->second._keycode;

			_DecrementKeyUsed(key);

			_events.erase(name);
		}
	}

	void SystemKeyPress::RemoveAttachedFunction(const std::string& name, std::string identifier)
	{
		std::map<const std::string, InputEventStruct>::iterator temp = _events.find(name);
		if (temp != std::end(_events))
		{
			std::map<const std::string, std::pair<INPUT_EVENT, Trigger>>::iterator secTemp = temp->second._eventFuncs.find(identifier);
			if (secTemp != std::end(temp->second._eventFuncs))
				temp->second._eventFuncs.erase(secTemp);
		}
	}

	INPUT_EVENT SystemKeyPress::GetInputEvent(const std::string& name, std::string identifier)
	{
		std::map<const std::string, InputEventStruct>::iterator temp = _events.find(name);
		if (temp != std::end(_events))
		{
			std::map<const std::string, std::pair<INPUT_EVENT, Trigger>>::iterator secTemp = temp->second._eventFuncs.find(identifier);
			if (secTemp != std::end(temp->second._eventFuncs))
				return secTemp->second.first;
		}
		return nullptr;
	}

	void SystemKeyPress::ALL_THE_KEYS()
	{
		for (unsigned i = 0; i < IKEY_FINAL_VALUE; ++i)
		{
			_usedKeyCodes[i]++;
		}
	}

	void SystemKeyPress::NO_KEYS()
	{
		_usedKeyCodes.clear();
	}

	void SystemKeyPress::RestoreEventKeys()
	{
		std::map<const std::string, InputEventStruct>::iterator temp = _events.begin();

		for (auto& it : _events)
		{
			_usedKeyCodes[it.second._keycode];
		}
	}

	void SystemKeyPress::SetWindow(HWND win)
	{
		_window = win;
	}
}
