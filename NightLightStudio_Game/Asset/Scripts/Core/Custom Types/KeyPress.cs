using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
	/*=========== Key Codes for all buttons. Still can use regular Virtual Codes, Just not registered here. =============*/
	public enum VK : int
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
}
