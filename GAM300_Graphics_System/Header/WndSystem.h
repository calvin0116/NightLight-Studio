/******************************************************************************/
/*!
\par
\file		WndSystem.h

\author     Teh Yu Yin, t.yuyin, 390005118
\par		t.yuyin\@digipen.edu
\date       7 September 2019

\brief      Header for the Window System

Copyright (C) 2019 DigiPen Institute of Technology. Reproduction or disclosure
of this file or its contents without the prior written consent of DigiPen
Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

// C++/Windows Headers
#include <string>

#include "WndUtils.h"

#define DEFAULTAPPWIDTH 1280
#define DEFAULTAPPHEIGHT 720
#define MINGLMAJORVER 3 // Minimal OpenGL Major version
#define MINGLMINORVER 3 // Minimal OpenGL Minor version

namespace NS_WINDOW
{
	class WndSystem
	{
		// Default Constructor
		WndSystem();
		// Conversion Constructor
		WndSystem(HINSTANCE hInstance, UINT appWidth, UINT appHeight, std::string appTitle);
		// Destructor
		virtual ~WndSystem();
	public:
		// Unique Singleton instance
		static WndSystem& GetInstance()
		{
			static WndSystem instance;
			return instance;
		}

		// Main Application Loop
		void Update();
		void Free();

		// Initialise the Window System
		void Init();

		// The WndProc function for Win32
		virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// Gets the Handler to Window
		HWND& GetHandlerToWindow();
		// Gets the Handler to the HDC (Hardware Device Context)
		HDC& GetHandlerToDeviceContext();
		// Gets the Handler to the first GLRenderContext
		HGLRC& GetHandlerToGLRenderContext();

		// Checks if the Window System has initialised
		bool HasInit() const;
		// Sets the System to swap buffers or not
		void SetSwapBuffers(bool set);
		// Sets fullscreen or windowed mode
		void SetFullScreenMode(bool set);
		// Checks if Window is active
		bool IsActiveWindow();

		// Get the width of the application
		UINT GetAppWidth() const;
		// Get the height of the applicaiton
		UINT GetAppHeight() const;
		// Checks if the game is fullscreen or windowed
		bool GetWindowedMode() const;

		// Sets the resolution of the application
		void SetAppResolution(int width, int height);
		// Gets the current resolution's height
		int GetResolutionHeight() const;
		// Gets the current resolution's width
		int GetResolutionWidth() const;

		void SetAppInstance(HINSTANCE&);

		void SetAppTitle(std::string&);

		// Sets size of window for windowed mode
		void SetWindowedSize(int width, int height);

	protected:
		HWND		hAppWnd;			// Handler to window
		HINSTANCE	hAppInstance;		// Handle to application instance
		HDC			hDevContext;		// Handle to device context
		HGLRC		hGLRenderContext;	// Handle to render context

		UINT		appWidth;			// Client Width
		UINT		appHeight;			// Client Height
		DWORD		windowStyleWM;		// Define Window Styles for window in windowed mode
		DWORD		windowStyleFM;		// Define Window Styles for window in fullscreen mode
		DWORD		windowStyle;		// Define Window Styles for window creation
		std::string appTitle;			// Window text at top
		bool		hasInit;			// Check if windows successfully initialised
		bool		swap;				// Check if double swap buffers is active for WndSystem
		bool        isWndMode;			// Check if current client is windowed or fullscreen
		bool		pausedByWindowSys;	// Check if the windowsystem paused the application
		bool		isActiveWindow;
		RECT        rDevDisplay;		// Coordinates of display device, used for fullscreen and positioning
		int         cDisplayPosX;		// Centralized position in virtual screen coordinates (X-coordinate)
		int         cDisplayPosY;		// Centralized position in virtual screen coordinates (Y-coordinate)
		int         iDisplayResW;		// Integer representing the width of the display resolution, defaults to current display's defaults
		int         iDisplayResH;		// Integer representing the height of the display resolution, defaults to current display's defaults


		// Initialize Window
		bool InitWindow();

		// Initialize OpenGL
		bool InitGL();

		// Destroy OpenGL
		void QuitGL();
	};
	extern WndSystem* pApp;
}