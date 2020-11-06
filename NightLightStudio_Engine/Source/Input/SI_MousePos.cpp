#include "SI_MousePos.h"
#include "..\Window\WndSystem.h"

namespace SystemInput_ns
{
	//********************************************** SYSTEM CURSOR POSITION **********************************************//
	glm::vec2 SystemMousePosition::ConvertToVec2(POINT pos)
	{
		return { (float)pos.x, (float)pos.y };
	}

	glm::vec2 SystemMousePosition::Offset(glm::vec2 pos, glm::vec2 offset)
	{
		return { pos.x + offset.x, pos.y + offset.y };
	}

	SystemMousePosition::SystemMousePosition(bool showCursor, bool clipCursor) : _mousePos{}, _prevMousePos{}, _showCursor{ showCursor }, _clipCursor{ clipCursor },  _scrollDown{}
	{
		RECT rect;
		GetClientRect(_window, &rect);
		_clientRectSize.x = rect.right;
		_clientRectSize.y = rect.bottom;

		//std::cout << _ClientRectSize.x << " " << _ClientRectSize.y;
	}

	bool SystemMousePosition::Update(float)
	{
		if (GetForegroundWindow() == _window)
		{
			_prevMousePos = _mousePos;
			GetCursorPos(&_mousePos);
			ScreenToClient(_window, &_mousePos);

			if (_mousePos.x < 0 || _mousePos.x > _clientRectSize.x || _mousePos.y < 0 || _mousePos.y > _clientRectSize.y)
			{
				_mousePos = _prevMousePos;
			}

			if (_scrollDown < -110)
				_scrollDown += 110;
			else if (_scrollDown > 110)
				_scrollDown -= 110;
			else
				_scrollDown = 0;

		}
		return 0;
	}
	glm::vec2 SystemMousePosition::operator()()
	{
		return GetMousePos();
	}
	glm::vec2 SystemMousePosition::GetMousePos()
	{
		return { _mousePos.x, _clientRectSize.y - _mousePos.y };
	}
	glm::vec2 SystemMousePosition::GetOriginalMousePos()
	{
		return { _mousePos.x, _mousePos.y };
	}
	glm::vec2 SystemMousePosition::GetMouseDragVec()
	{
		return { _mousePos.x - _prevMousePos.x,  _prevMousePos.y - _mousePos.y };
	}
	bool SystemMousePosition::ToggleCursorVisible()
	{
		_showCursor ^= 1;

		if (_showCursor)
			while (ShowCursor(true) < 0);
		else
			while (ShowCursor(false) > 0);

		return _showCursor;
	}
	void SystemMousePosition::SetCursorVisible(bool show)
	{
		_showCursor = show;

		if (_showCursor)
			while (ShowCursor(true) < 0);
		else
			while (ShowCursor(false) > 0);
	}

	void SystemMousePosition::LoadCursorImage(std::string name)
	{
		std::wstring wStr = std::wstring(std::begin(name), std::end(name));
		HCURSOR hCurs = LoadCursorFromFile((LPCSTR)wStr.data());
		SetClassLongPtr(_window, GCLP_HCURSOR, (LONG_PTR)hCurs);
	}
	POINT SystemMousePosition::GetClientRectSize()
	{
		return _clientRectSize;
	}
	RECT SystemMousePosition::GetWinRect()
	{
		RECT rect;
		HWND h = NS_WINDOW::SYS_WINDOW->GetHandlerToWindow();
		GetWindowRect(h, &rect);
		return rect;
	}
	bool SystemMousePosition::ToggleClipCursor()
	{
		_clipCursor = !_clipCursor;
		RECT rect = GetWinRect();
		rect.top += 10;
		rect.right -= 10;
		rect.bottom -= 10;
		rect.left += 10;
		ClipCursor(&rect);
		return _clipCursor;
	}
	void SystemMousePosition::SetClipCursor(bool clip)
	{
		_clipCursor = clip;
		RECT rect = GetWinRect();
		rect.top += 10;
		rect.right -= 10;
		rect.bottom -= 10;
		rect.left += 10;
		ClipCursor(&rect);
	}
	void SystemMousePosition::SetCurPos(int x, int y)
	{
		SetCursorPos(x, y);
	}
	void SystemMousePosition::SetCurPos()
	{
		RECT rect = GetWinRect();
		SetCursorPos((rect.top + rect.bottom) / 2, (rect.left + rect.right) / 2);
	}
	glm::vec2 SystemMousePosition::GetRelativeLocation()
	{
		return { (float)_mousePos.x / (float)_clientRectSize.x, 1.0f - (float)_mousePos.y / (float)_clientRectSize.y };
	}
	glm::vec2 SystemMousePosition::GetRelativeDragVec()
	{
		return { (float)(_mousePos.x - _prevMousePos.x) / (float)_clientRectSize.x,  ((_prevMousePos.y - _mousePos.y) / (float)_clientRectSize.y )};
	}
	void SystemMousePosition::SetWindow(HWND win)
	{
		_window = win;
	}
	void SystemMousePosition::SetScroll(short scroll)
	{
		if (GetForegroundWindow() == _window)
		{
			_scrollDown = scroll;
		}
	}
	bool SystemMousePosition::GetIfScrollUp()
	{
		return _scrollDown > 0;
	}
	bool SystemMousePosition::GetIfScrollDown()
	{
		return _scrollDown < 0;;
	}
}
