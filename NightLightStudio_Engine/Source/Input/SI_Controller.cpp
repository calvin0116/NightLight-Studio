#include "SI_Controller.h"

namespace SystemInput_ns
{
	//********************************************** SYSTEM CONTROLLER **********************************************//

	SystemController::SystemController(int controllerID, bool allowController) 
		: _controllerID{ controllerID }, _allowControllers{ allowController }
	{
		// Detects the first available Controller
		ResetController(allowController);
	}
	void SystemController::ResetController(bool allowController)
	{
		// Locates the first available controller
		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
		if (allowController)
		{
			for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
			{
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				if (XInputGetState(i, &state) == ERROR_SUCCESS)
				{
					_controllerID = i;
					_controllerState = state;
					_allowControllers = true;
					break;
				}
			}
		}
		else
		{
			_allowControllers = false;
		}
	}
	bool SystemController::CTRLButtonDown(unsigned int buttonDown)
	{
		return ((_controllerState.Gamepad.wButtons & buttonDown) != 0);
	}
	bool SystemController::GetIfButtonPress(unsigned int buttonPressed)
	{
		decltype(_buttonStates)::iterator it = _buttonStates.find(buttonPressed);
		if (it != std::end(_buttonStates) && it->second == OnCTRLPress)
			return true;
		else
			return false;
	}
	bool SystemController::GetIfButtonHeld(unsigned int buttonPressed)
	{
		decltype(_buttonStates)::iterator it = _buttonStates.find(buttonPressed);
		if (it != std::end(_buttonStates) && it->second == OnCTRLHold)
			return true;
		else
			return false;
	}
	bool SystemController::GetIfButtonRelease(unsigned int buttonPressed)
	{
		decltype(_buttonStates)::iterator it = _buttonStates.find(buttonPressed);
		if (it != std::end(_buttonStates) && it->second == OnCTRLRelease)
			return true;
		else
			return false;
	}

	void SystemController::ALLBUTTONS()
	{
		_buttonStates.emplace(GAMEPAD_A, 0);
		_buttonStates.emplace(GAMEPAD_B, 0);
		_buttonStates.emplace(GAMEPAD_X, 0);
		_buttonStates.emplace(GAMEPAD_Y, 0);

		_buttonStates.emplace(GAMEPAD_DPAD_UP, 0);
		_buttonStates.emplace(GAMEPAD_DPAD_DOWN, 0);
		_buttonStates.emplace(GAMEPAD_DPAD_LEFT, 0);
		_buttonStates.emplace(GAMEPAD_DPAD_RIGHT, 0);

		_buttonStates.emplace(GAMEPAD_SHOULDER_LEFT, 0);
		_buttonStates.emplace(GAMEPAD_SHOULDER_RIGHT, 0);

		_buttonStates.emplace(GAMEPAD_THUMB_LEFT, 0);
		_buttonStates.emplace(GAMEPAD_THUMB_RIGHT, 0);

		_buttonStates.emplace(GAMEPAD_BACK, 0);
		_buttonStates.emplace(GAMEPAD_START, 0);
	}

	float SystemController::GetTrigger(int lr)
	{
		if (_allowControllers)
		{
			switch (lr)
			{
			case 1:
				return (float)_controllerState.Gamepad.bRightTrigger / 255;
				break;
			default:
				return (float)_controllerState.Gamepad.bLeftTrigger / 255;
				break;
			}
		}
		return 0;
	}
	ControllerVec2 SystemController::GetAnalog(int lr)
	{
		if (_allowControllers)
		{
			float normX, normY;
			switch (lr)
			{
			case 1:
				normX = fmaxf(-1, (float)_controllerState.Gamepad.sThumbRX / 32767);
				normY = fmaxf(-1, (float)_controllerState.Gamepad.sThumbRY / 32767);
				break;
			default:
				normX = fmaxf(-1, (float)_controllerState.Gamepad.sThumbLX / 32767);
				normY = fmaxf(-1, (float)_controllerState.Gamepad.sThumbLY / 32767);
				break;
			}

			float deadzoneX = 0.05f;
			float deadzoneY = 0.02f;

			float _x = (abs(normX) < deadzoneX ? 0 : (abs(normX) - deadzoneX) * (normX / abs(normX)));
			float _y = (abs(normY) < deadzoneY ? 0 : (abs(normY) - deadzoneY) * (normY / abs(normY)));

			if (deadzoneX > 0) _x /= 1 - deadzoneX;
			if (deadzoneY > 0) _y /= 1 - deadzoneY;

			return { _x, _y };
		}
		return { 0.0f, 0.0f };
	}

	bool SystemController::Update(float)
	{
		if (!_allowControllers)
			return false;

		// Prevents input when out of focus
		if (GetForegroundWindow() != _window)
			return false;

		ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

		if (XInputGetState(_controllerID, &_controllerState) != ERROR_SUCCESS)
		{
			ResetController(true);
			return false;
		}

		for (std::pair<unsigned int, unsigned int> p : _buttonStates)
		{
			unsigned int key = p.first;
			if (CTRLButtonDown(key))
			{
				if (!_buttonStates[key])
					_buttonStates[key] = OnCTRLPress;
				else
					_buttonStates[key] = OnCTRLHold;
			}
			else
			{
				if (_buttonStates[key])
				{
					if (_buttonStates[key] != OnCTRLRelease)
						_buttonStates[key] = OnCTRLRelease;
					else
						_buttonStates[key] = 0;
				}
			}
		}

		for (decltype(_buttonEvents)::value_type but : _buttonEvents)
		{
			decltype(_buttonStates)::iterator it= _buttonStates.find(but.first.first);
			if (it != std::end(_buttonStates))
			{
				if (_buttonStates[but.first.first] == but.first.second)
				{
					if (but.second)
						but.second();
				}
			}
		}

		for (decltype(_triggerEvents)::value_type trig : _triggerEvents)
		{
			if (trig.second)
				trig.second(GetTrigger(trig.first));
		}

		for (decltype(_analogEvents)::value_type analog : _analogEvents)
		{
			if (analog.second)
			{
				ControllerVec2 vec = GetAnalog(analog.first);
				analog.second(vec._x, vec._y);
			}
		}

		return false;
	}
}