using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
  enum GamepadKey : uint
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
}
