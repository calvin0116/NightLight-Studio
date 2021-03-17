#include "SI_MousePos.h"
#include "..\Window\WndSystem.h"

namespace SystemInput_ns
{
	//********************************************** SYSTEM CURSOR POSITION **********************************************//
	SystemMousePosition::SystemMousePosition(bool showCursor, bool clipCursor) : _mousePos{}, _prevMousePos{}, _showCursor{ showCursor }, _clipCursor{ clipCursor },  _scrollDown{}
	{
		ResetWinSize();
		//std::cout << _ClientRectSize.x << " " << _ClientRectSize.y;
	}

	bool SystemMousePosition::Update(float)
	{
		if (GetForegroundWindow() == _window)
		{

			if (_toCenter)
			{
				// do the thing
				GetCursorPos(&_mousePos);
				ScreenToClient(_window, &_mousePos);

				SetClipCursor(true);
				POINT point = SetCurPos();
				ScreenToClient(_window, &point);
				_prevMousePos = point;
			}
			else
			{
				SetClipCursor(false);

				_prevMousePos = _mousePos;

				GetCursorPos(&_mousePos);
				ScreenToClient(_window, &_mousePos);

				if (_mousePos.x < 0 || _mousePos.x > _clientRectSize.x || _mousePos.y < 0 || _mousePos.y > _clientRectSize.y)
				{
					_mousePos = _prevMousePos;
				}
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

		/*if (_showCursor)
			while (ShowCursor(true) < 0);
		else
			while (ShowCursor(false) > 0);*/

		if (_showCursor)
			NS_WINDOW::SYS_WINDOW->SetCursorVisibility(true);
		else
			NS_WINDOW::SYS_WINDOW->SetCursorVisibility(false);

		return _showCursor;
	}
	void SystemMousePosition::SetCursorVisible(bool show)
	{
		_showCursor = show;

		if (_showCursor)
			while (ShowCursor(true) < 0);
		else
			while (ShowCursor(false) > 0);

		//if (_showCursor)
		//	NS_WINDOW::SYS_WINDOW->SetCursorVisibility(true);
		//else
		//	NS_WINDOW::SYS_WINDOW->SetCursorVisibility(false);
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
	RECT SystemMousePosition::GetWinClientRect()
	{
		RECT rect;
		//GetWindowRect(_window, &rect);
		GetClientRect(_window, &rect);
		return rect;
	}
	bool SystemMousePosition::ToggleClipCursor()
	{
		_clipCursor = !_clipCursor;
		if (_clipCursor)
		{
			RECT rect = GetWinClientRect();
			rect.top += 10;
			rect.right -= 10;
			rect.bottom -= 10;
			rect.left += 10;
			ClipCursor(&rect);
		}
		return _clipCursor;
	}
	void SystemMousePosition::SetClipCursor(bool clip)
	{
		_clipCursor = clip;
		if (clip)
		{
			RECT rect = GetWinClientRect();
			rect.top += 10;
			rect.right -= 10;
			rect.bottom -= 10;
			rect.left += 10;
			ClipCursor(&rect);
		}
	}
	POINT SystemMousePosition::SetCurPos(int x, int y)
	{
		SetCursorPos(x, y);
		POINT point;
		point.x = x;
		point.y = y;
		return point;
	}
	POINT SystemMousePosition::SetCurPos()
	{
		RECT rect = GetWinClientRect();
		POINT point;
		point.x = (rect.left + rect.right) / 2;
		point.y = (rect.top + rect.bottom) / 2;
		SetCursorPos(point.x, point.y);
		return point;
	}

	void SystemMousePosition::SetToCenter(bool set)
	{
		_toCenter = set;
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
		ResetWinSize();
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
		return _scrollDown < 0;
	}

	void SystemMousePosition::ResetWinSize()
	{
		RECT rect;
		GetClientRect(_window, &rect);
		_clientRectSize.x = rect.right;
		_clientRectSize.y = rect.bottom;
	}
}
