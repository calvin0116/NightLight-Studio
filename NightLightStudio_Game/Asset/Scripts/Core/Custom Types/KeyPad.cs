using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
  enum GamepadKey : uint
	{
		GAMEPAD_A = 0x1000,
		GAMEPAD_B = 0x2000,
		GAMEPAD_X = 0x4000,
		GAMEPAD_Y = 0x8000,

		GAMEPAD_CROSS = 0x1000,
		GAMEPAD_CIRCLE = 0x2000,
		GAMEPAD_SQUARE = 0x4000,
		GAMEPAD_TRIANGLE = 0x8000,

		GAMEPAD_DPAD_UP = 0x0001,
		GAMEPAD_DPAD_DOWN = 0x0002,
		GAMEPAD_DPAD_LEFT = 0x0004,
		GAMEPAD_DPAD_RIGHT = 0x0008,

		GAMEPAD_SHOULDER_LEFT = 0x0100,
		GAMEPAD_SHOULDER_RIGHT = 0x0200,

		GAMEPAD_THUMB_LEFT = 0x0040,
		GAMEPAD_THUMB_RIGHT = 0x0080,

		GAMEPAD_BACK = 0x0020,
		GAMEPAD_START = 0x0010,

		GAMEPAD_SHARE = 0x0020,
		GAMEPAD_OPTIONS = 0x0010,
	};

	// Bits used to check current state of key press
	enum CTRLTrigger : uint
	{
		OnCTRLDefault = 0,
		OnCTRLPress = 0x01,
		OnCTRLHold = 0x02,
		OnCTRLRelease = 0x04,
		OnCTRLDelay = 0x08
	};

}
