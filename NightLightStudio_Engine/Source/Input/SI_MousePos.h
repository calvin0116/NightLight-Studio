#ifndef SI_SYSTEM_INPUT_MOUSEPOS
#define SI_SYSTEM_INPUT_MOUSEPOS

#include "..\..\framework.h"
#include "..\glm\vec2.hpp"

#include <Windows.h>
#include <string>

namespace SystemInput_ns
{
	class ENGINE_API SystemMousePosition
	{
		POINT _mousePos;
		POINT _prevMousePos;

		bool _showCursor;

		short _scrollDown;

		POINT _clientRectSize;

		//ONLY TO PREVENT CHECKING WHEN OUT OF WINDOW, MAY BE REMOVED/REPLACED
		HWND _window = GetForegroundWindow();

		// CURRENTLY BUILDING
		glm::vec2 ConvertToVec2(POINT pos);
		glm::vec2 Offset(glm::vec2 pos, glm::vec2 offset);

	public:
		SystemMousePosition(bool showCursor = true);
		~SystemMousePosition() = default;

		// Updates Mouse Position, must run once to refresh mouse Positions
		bool Update(float dt = 0);

		// Get Mouse Position Directly
		glm::vec2 operator()();

		// Get Mouse Position - (0,0) Begins in Bottom-Left Corner
		glm::vec2 GetMousePos();
		// Get Mouse Position - (0,0) Begins in Top-Left Corner
		glm::vec2 GetOriginalMousePos();
		// Get Mouse Movement vector - (0,0) Begins in Bottom-Left Corner
		glm::vec2 GetMouseDragVec();

		// Toggles if cursor is viewable or not
		bool ToggleCursorVisible();
		// Sets If can view cursor
		void SetCursorVisible(bool show);
		// Sets Cursor image - .cur or .ani files only
		void LoadCursorImage(std::string name);
		// Returns Client Rect Size in pixels
		POINT GetClientRectSize();

		// Gets the cursor's position in the client rect from 0.0 to 1.0 in xy coordinates, (0,0) Begins in Bottom-Left Corner
		glm::vec2 GetRelativeLocation();
		glm::vec2 GetRelativeDragVec();

		void SetWindow(HWND win);

		void SetScroll(short scroll);
		bool GetIfScrollUp();
		bool GetIfScrollDown();
	};
}

#endif
