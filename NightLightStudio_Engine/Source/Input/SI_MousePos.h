#ifndef SI_SYSTEM_INPUT_MOUSEPOS
#define SI_SYSTEM_INPUT_MOUSEPOS

#include "..\..\framework.h"

#include <Windows.h>
#include <string>

namespace SystemInput_ns
{
	struct ENGINE_API InputVec2
	{
		float _x;
		float _y;
	};

	class ENGINE_API SystemMousePosition
	{
		POINT _mousePos;
		POINT _prevMousePos;

		bool _showCursor;

		POINT _clientRectSize;

		//ONLY TO PREVENT CHECKING WHEN OUT OF WINDOW, MAY BE REMOVED/REPLACED
		const HWND _window = GetForegroundWindow();

		// CURRENTLY BUILDING
		InputVec2 ConvertToVec2(POINT pos);
		InputVec2 Offset(InputVec2 pos, InputVec2 offset);

	public:
		SystemMousePosition(bool showCursor = true);
		~SystemMousePosition() = default;

		// Updates Mouse Position, must run once to refresh mouse Positions
		bool Update(float dt = 0);

		// Get Mouse Position Directly
		POINT operator()();

		// Get Mouse Position - (0,0) Begins in Bottom-Left Corner
		POINT GetMousePos();
		// Get Mouse Position - (0,0) Begins in Top-Left Corner
		POINT GetOriginalMousePos();
		// Get Mouse Movement vector - (0,0) Begins in Bottom-Left Corner
		POINT GetMouseDragVec();

		// Toggles if cursor is viewable or not
		bool ToggleCursorVisible();
		// Sets If can view cursor
		void SetCursorVisible(bool show);
		// Sets Cursor image - .cur or .ani files only
		void LoadCursorImage(std::string name);
		// Returns Client Rect Size in pixels
		POINT GetClientRectSize();

		// Gets the cursor's position in the client rect from 0.0 to 1.0 in xy coordinates, (0,0) Begins in Bottom-Left Corner
		InputVec2 GetRelativeLocation();
	};
}

#endif
