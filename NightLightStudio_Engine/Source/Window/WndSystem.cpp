/******************************************************************************/
/*!
\par
\file		WndSystem.cpp

\author     Teh Yu Yin, t.yuyin, 390005118
\par		t.yuyin\@digipen.edu
\date       7 September 2019

\brief      Code for the Window System

Copyright (C) 2019 DigiPen Institute of Technology. Reproduction or disclosure
of this file or its contents without the prior written consent of DigiPen
Institute of Technology is prohibited.
*/
/******************************************************************************/

// SAE Headers
#include "WndSystem.h"
#include "../IO/Json/Config.h"

#include "../Graphics/GraphicsSystem.h"


#ifdef _EDITOR
#include "../Editor/imgui/imgui_impl_win32.h"
#include "../Input/SystemInput.h"
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
#include "../Input/SystemInput.h"

#include "../Core/SceneManager.h"

// Tracy
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"

#define APPTITLE "NightLight Studios: Rebirth"

namespace NS_WINDOW
{

	// Throws window message via user-defined msg proc from global space
	/*LRESULT CALLBACK MainWinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (SYS_WINDOW)
			return SYS_WINDOW->WndProc(hwnd, msg, wParam, lParam);

		else
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}*/

	// Default constructor
	WndSystem::WndSystem()
		: //hAppInstance{ NULL },
		hAppWnd{ NULL },
		//hDevContext{ NULL },
		//hGLRenderContext{ NULL },
		appWidth{ DEFAULTAPPWIDTH },
		appHeight{ DEFAULTAPPHEIGHT },
		appTitle{ "Debug Console" },
		/*windowStyleWM{ WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE },
		windowStyleFM{ WS_POPUP | WS_VISIBLE },
		windowStyle{ WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX },*/
		hasInit{ false },
		swap{ true },
		isWndMode{ true },
		pausedByWindowSys{false},
		isActiveWindow{true},
		cDisplayPosX{ 0 },
		cDisplayPosY{ 0 },
		rDevDisplay{ 0, 0, 0, 0 },
		iDisplayResW{ GetSystemMetrics(SM_CXSCREEN) },
		iDisplayResH{ GetSystemMetrics(SM_CYSCREEN) },
		_glfwWnd{ nullptr }
	{
		// Default App Setup, no extra parameters have been set
	}

	// Conversion Constructor
	WndSystem::WndSystem(HINSTANCE hInstance, UINT appWidth, UINT appHeight, std::string appTitle)
		: //hAppInstance{ hInstance },
		hAppWnd{ NULL },
		//hDevContext{ NULL },
		//hGLRenderContext{ NULL },
		appWidth{ appWidth },
		appHeight{ appHeight },
		appTitle{ appTitle },
		/*windowStyleWM{ WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE },
		windowStyleFM{ WS_POPUP | WS_VISIBLE },
		windowStyle{ WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX },*/
		hasInit{ false },
		swap{ true },
		isWndMode{ true },
		pausedByWindowSys{ false },
		isActiveWindow{ true },
		cDisplayPosX{ 0 },
		cDisplayPosY{ 0 },
		rDevDisplay{ 0, 0, 0, 0 },
		iDisplayResW{ GetSystemMetrics(SM_CXSCREEN) },
		iDisplayResH{ GetSystemMetrics(SM_CYSCREEN) },
		_glfwWnd{ nullptr }
	{
		// Specialized App Setup. Window height, width and title can be set externally
		UNREFERENCED_PARAMETER(hInstance);
	}

	// Destructor
	WndSystem::~WndSystem()
	{
	}

	void WndSystem::Load()
	{
		SetWindowedSize(CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height);
		//SetWindowedSize(1920, 1080);
	}

	// Window Update Loop
	void WndSystem::Update()
	{
		// Tracy
		// Zone Color: Peach
		ZoneScopedNC("Window", 0xfabaa5);

		// Allows for definition of keyboard shortcuts to use for game application
		// Uncomment and edit if required
		//HACCEL hAccelTable = LoadAccelerators(hAppInstance, MAKEINTRESOURCE(IDC_SEAARCHENGINE));

		// Main Message Loop
		//MSG msg = { 0 }; // Set all values to 0

		//while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//{
		//	// Send message for engine to shutdown
		//	// Then shut down window
		//	// Temporarily only frees window system resources before exit
		//	if (msg.message == WM_QUIT)
		//	{
		//		NS_WINDOW::SYS_WINDOW->Free();
		//		//return false;
		//		NS_SCENE::SYS_SCENE_MANAGER->SetNextScene(NS_SCENE::EXIT_SCENCE);
		//	}

		//	// Uncomment and edit if accelerator table is implemented
		//	/*if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		//	{
		//		TranslateMessage(&msg);
		//		DispatchMessage(&msg);
		//	}*/

		//	// Delete this section if accelerator table is implemented
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);
		//}



		// Swap buffers between current shown and current to draw on before next iteration
		/*if (swap)
			SwapBuffers(hDevContext);*/

		//return true; // Return error msg for breaking out of update loop

		// GLFW Implementation

		// If instruction to exit is received
		if (glfwWindowShouldClose(_glfwWnd))
		{
			NS_WINDOW::SYS_WINDOW->Free();
			NS_SCENE::SYS_SCENE_MANAGER->SetNextScene(NS_SCENE::EXIT_SCENCE);
			return;
		}

		{
			// Cyan color
			ZoneScopedNC("glfwSwapBuffers", 0x03f4fc);
			glfwSwapBuffers(_glfwWnd);
		}
		
		{
			// Pink color
			ZoneScopedNC("glfwPollEvents", 0xff00d9);
			glfwPollEvents();
		}
		
	}

	// Frees the resources used by the System
	void WndSystem::Free()
	{
		QuitGL();
	}

	bool WndSystem::InitWindow()
	{
		// Default initialization
		// Create Window Class, different from C++ classes
		//WNDCLASSEX wcex;
		//ZeroMemory(&wcex, sizeof(WNDCLASSEX));
		//wcex.cbClsExtra = NULL;
		//wcex.cbWndExtra = NULL;
		//wcex.cbSize = sizeof(WNDCLASSEX);
		//wcex.style = CS_HREDRAW | CS_VREDRAW;
		//wcex.hInstance = hAppInstance;
		//wcex.lpfnWndProc = MainWinProc;                          // User-defined. Window Event Handler
		//wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);            // Create taskbar icon for app window, can make based on external resource
		//wcex.hCursor = LoadCursor(NULL, IDC_ARROW);              // Default cursor
		//wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); // Default background
		//wcex.lpszMenuName = NULL;                                // Base Resource, undefined (Basically the top toolbar of windows, not to be confused with upper right)
		//wcex.lpszClassName = "GLClientClass";
		//wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);          // The small print of hIcon at upper-left corner of window

		//// Error checking for any failed initializations
		//if (!RegisterClassEx(&wcex))
		//	return OutErrorMsg("Failed to Register Window Class (Extended)");

		// Set Window Rect to ensure correct Client Size(User-defined)
		// Using windowed mode settings by default
		/*RECT r = { 0, 0, static_cast<long>(appWidth), static_cast<long>(appHeight) };
		AdjustWindowRect(&r, windowStyleWM, false);
		int width = r.right - r.left;
		int height = r.bottom - r.top;
		int monitorX = GetSystemMetrics(SM_CXSCREEN);
		int monitorY = GetSystemMetrics(SM_CYSCREEN);
		int x = monitorX / 2 - width / 2;
		int y = monitorY / 2 - height / 2;*/

		// Create the window
		// WS_EX_ACCEPTFILES allows the window to accept drag-n-drop files, for QoL
		//hAppWnd = CreateWindowExA(WS_EX_ACCEPTFILES, wcex.lpszClassName, appTitle.data(), windowStyleWM, x, y, width, height, NULL, NULL, hAppInstance, NULL);

		/*if (!hAppWnd)
			return OutErrorMsg("Failed to create Window");*/

		// Show created window if all is successful
		//ShowWindow(hAppWnd, SW_SHOW);

		//////////////////////////////////////
		// GLFW Implementation
		glfwInit();

		// Set opengl versions
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MINGLMAJORVER);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINGLMINORVER);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);		// Allows resizing of windowed window
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);		// Ensure visibility of window upon creation
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);		// Include minimize, maximize and close widgets at top right corner of window
		glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);		// Do not give input focus upon creation
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);   // Allow double buffering

		glfwWindowHint(GLFW_SAMPLES, 2);		// Number of samples for multisampling, 2 for default
		glfwWindowHint(GLFW_RED_BITS, 8);		// Desired level of bits for red component of framebuffer, default is 8 but we will be explicit
		glfwWindowHint(GLFW_GREEN_BITS, 8);		// Desired level of bits for green component of framebuffer, default is 8 but we will be explicit
		glfwWindowHint(GLFW_BLUE_BITS, 8);		// Desired level of bits for blue component of framebuffer, default is 8 but we will be explicit
		glfwWindowHint(GLFW_ALPHA_BITS, 8);		// Desired level of bits for alpha component of framebuffer, default is 8 but we will be explicit
		glfwWindowHint(GLFW_DEPTH_BITS, 24);	// Desired level of bits for depth component of framebuffer, default is 24 but we will be explicit
		glfwWindowHint(GLFW_STENCIL_BITS, 24);	// Desired level of bits for stencil component of framebuffer, default is 8

		glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); // Set the highest available refresh rate for full screen
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);	   // Allow capabilities for sRGB support

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);  // Create context for specified opengl profile, in this case compatibility

		_glfwWnd = glfwCreateWindow(appWidth, appHeight, appTitle.c_str(), NULL, NULL);

		if (_glfwWnd == nullptr)
		{
			TracyMessageL("WndSystem ERROR: Creation of GLFW Window failed! Please check InitWindow()");
			glfwTerminate(); // Avoid mem leaks
			return false; // Throw error and abort, too big to just ignore and continue other stuff
		}

		glfwMakeContextCurrent(_glfwWnd); // If success, set the created GLFW window object's context as current

		glfwSwapInterval(1);			  // Fixes fps issue(?)

		hAppWnd = glfwGetWin32Window(_glfwWnd);

#ifdef _DEBUG
		ShowCursor(true);
#else
		ShowCursor(true);
#endif

		return true; // Successful Initialization
	}

	// Initialise OpenGL
	bool WndSystem::InitGL()
	{
		/* We need to create the old, dummy OpenGL device context and dummy window
		   before we can make the new OpenGL render context of a specific profile */

		   // We need the temporary window as pixel format can only be set to a window once
		   // Due to the use of the advanced pixel format, a fresh window is needed later
		   //HWND tmpWnd = CreateWindow("GLClientClass", "Temp WND", m_WindowStyle, 0, 0, 1, 1, NULL, NULL, m_hAppInstance, NULL);

		//hDevContext = GetDC(hAppWnd); // Window must already be created

		//// Create PFD (Pixel Format Descriptor), determines pixel behavior and allows interface with opengl
		//PIXELFORMATDESCRIPTOR pfd;
		//ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
		//pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		//pfd.nVersion = 1;
		//pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		//pfd.iPixelType = PFD_TYPE_RGBA;
		//pfd.cColorBits = 32;
		//pfd.cAlphaBits = 8;
		//pfd.cDepthBits = 32;
		//pfd.cStencilBits = 8;

		//int format = ChoosePixelFormat(hDevContext, &pfd);

		//if (!SetPixelFormat(hDevContext, format, &pfd))
		//	return OutErrorMsg("Failed to set Pixel Format(PFD)");

		//// Create temporary OpenGL Render Context
		//HGLRC tmpRContext = wglCreateContext(hDevContext);

		//if (!wglMakeCurrent(hDevContext, tmpRContext))
		//	return OutErrorMsg("Failed to create and show Render Context");

		//// Initialize GLEW (Library)
		//if (glewInit())
		//	return OutErrorMsg("Failed to Initialize GLEW");

		//// Define function pointers for WGL because windows OpenGL does not support this
		//PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
		//wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

		//PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
		//wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));

		//// Now make the actual Modern OpenGL device and render contexts and window
		////const int pixelAttribs[] =
		////{
		////	WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		////	WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		////	WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		////	WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		////	WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		////	WGL_COLOR_BITS_ARB, 32,
		////	WGL_ALPHA_BITS_ARB, 8,
		////	WGL_DEPTH_BITS_ARB, 32,
		////	WGL_STENCIL_BITS_ARB, 8,
		////	WGL_SAMPLE_BUFFERS_ARB, 1, // Must be 1
		////	WGL_SAMPLES_ARB, 4,
		////	0
		////};

		////static PIXELFORMATDESCRIPTOR    dummyPFD = {
		////	sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		////	1,                              // Version of this structure
		////	PFD_DRAW_TO_WINDOW |            // Draw to Window (not to bitmap)
		////	PFD_SUPPORT_OPENGL |            // Support OpenGL calls in window
		////	PFD_DOUBLEBUFFER |              // Double buffered mode
		////	PFD_STEREO_DONTCARE |
		////	0,
		////	PFD_TYPE_RGBA,                  // RGBA Color mode
		////	32,								// Want the display bit depth
		////	0,0,0,0,0,0,                    // Not used to select mode
		////	0,0,                            // Not used to select mode
		////	0,0,0,0,0,                      // Not used to select mode
		////	32,								// Size of depth buffer
		////	8,								// bit stencil
		////	0,                              // Not used to select mode
		////	PFD_MAIN_PLANE,                 // Draw in main plane
		////	0,                              // Not used to select mode
		////	0,0,0
		////};

		////int pformat;
		////UINT numFormats;

		////wglChoosePixelFormatARB(hDevContext, pixelAttribs, NULL, 1, &pformat, &numFormats);

		////DescribePixelFormat(hDevContext, pformat, sizeof(dummyPFD), &dummyPFD);
		////if (!SetPixelFormat(hDevContext, pformat, &dummyPFD))
		////	return OutErrorMsg("Failed to set Pixel Format(PFD)");

		//// Set attributes for new OpenGL rendering context (Version 3.3)
		//static int contextAttribs[] =
		//{
		//	WGL_CONTEXT_MAJOR_VERSION_ARB, MINGLMAJORVER,
		//	WGL_CONTEXT_MINOR_VERSION_ARB, MINGLMINORVER,
		//	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		//	0
		//};

		//// Create the actual rendering context to use with the context attributes
		//hGLRenderContext = wglCreateContextAttribsARB(hDevContext, 0, contextAttribs);

		//// Delete the old temporary context and window and set the current to the new render context
		//wglMakeCurrent(NULL, NULL);
		//wglDeleteContext(tmpRContext);
		//wglMakeCurrent(hDevContext, hGLRenderContext);

		// GLFW Implementation
		// Technically there's nothing much else for it, since most of the context preferences were set in InitWnd()	

		// Initialize GLAD, our extension wrangler
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			TracyMessageL("WndSystem ERROR: Failed to initialize GLAD, please check InitGL()");
			return false; // Abort, window system errors are too crucial to ignore
		}

		return true;
	}

	// Destroys OpenGL
	void WndSystem::QuitGL()
	{
		// Free window and created contexts (render and device)
		// Contexts to be deleted must not be current
		//wglMakeCurrent(NULL, NULL);				// Call to make render and device contexts not current, instead set to NULL
		//wglDeleteContext(hGLRenderContext);		// Destructor for render context
		//ReleaseDC(hAppWnd, hDevContext);		// Destructor for device context
		//DestroyWindow(hAppWnd);

		// GLFW Implementation 
		//glfwTerminate();
	}

	// Initialises the System
	void WndSystem::Init()
	{
		if (hasInit)
			return;

		// Get handler
		/*hAppInstance = SAE::ENGINE->GetHandler();
		try {
			appWidth = SAE::ENGINE->GetWinWidth();
		}
		catch (...) {
			appWidth = 1600;
		}
		try {
			appHeight = SAE::ENGINE->GetWinHeight();
		}
		catch (...) {
			appHeight = 600;
		}
		try {
			appTitle = SAE::ENGINE->GetTitle();
		}
		catch (...) {
			appTitle = std::string("SEA ARCH ENGINE");
		}*/

		// TO CHANGE
		// Initialize window with default values here
		//SYS_WINDOW->SetWindowedSize(DEFAULTAPPWIDTH, DEFAULTAPPHEIGHT);

		std::string s(APPTITLE);

		SYS_WINDOW->SetAppTitle(s);
		
		SetWindowedSize(CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height);

		// Window Creation Function Call
		if (!InitWindow())
		{
			//bool WindowHasInit = false;
			TracyMessageL("WndSystem::Init: CLIENT WINDOW INITIALIZATION FAILED");
			SPEEDLOG("WndSystem::Init: CLIENT WINDOW INITIALIZATION FAILED");
			//std::cout << "CLIENT WINDOW INITIALIZATION FAILED" << std::endl;
			hasInit = false;
			return;
		}

		// GL Setup Function Call
		if (!InitGL())
		{
			//bool DummyGLHasInit = false;
			TracyMessageL("WndSystem::Init: GL INITIALIZATION FAILED");
			SPEEDLOG("WndSystem::Init: GL INITIALIZATION FAILED");
			//std::cout << "GL INITIALIZATION FAILED" << std::endl;
			hasInit = false;
			return;
		}

		hasInit = true;
		SetFullScreenMode(CONFIG_DATA->GetConfigData().toFullScreen); //<- Use json to control

		// Set callback for mouse scroll
		glfwSetScrollCallback(_glfwWnd, [](GLFWwindow * window, double xoffset, double yoffset)
		{
			UNREFERENCED_PARAMETER(window);
			UNREFERENCED_PARAMETER(xoffset);
			SYS_INPUT->GetSystemMousePos().SetScroll(static_cast<short>(yoffset));
		});

		// Set callback for resizing
		glfwSetFramebufferSizeCallback(_glfwWnd, [](GLFWwindow* window, int width, int height)
		{
			// Update rect in SystemMousePosition
			SYS_INPUT->GetSystemMousePos().ResetWinSize();

			// Assign values for reference
			SYS_WINDOW->appWidth = width;
			SYS_WINDOW->appHeight = height;

			UNREFERENCED_PARAMETER(window);
			glViewport(0, 0, width, height);

			// Set all render targets of framebuffers appropriately for resize (Deferred shading)
			glBindFramebuffer(GL_FRAMEBUFFER, NS_GRAPHICS::SYS_GRAPHICS->GetGeometryBuffer());

			// Re-generate data buffers for geometry buffer + render targets

			// Position
			glBindTexture(GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetPositionAlphaRT());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetPositionAlphaRT(), 0);

			// Calculated normals/normal map + metallic
			glBindTexture(GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetNormalMapAndMetallicRT());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetNormalMapAndMetallicRT(), 0);

			// Albedo/Diffuse map + roughness
			glBindTexture(GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetAlbedoMapAndRoughnessRT());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetAlbedoMapAndRoughnessRT(), 0);

			// Ambient Occlusion
			glBindTexture(GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetAmbientOcclusionRT());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, NS_GRAPHICS::SYS_GRAPHICS->GetAmbientOcclusionRT(), 0);

			// Tell opengl which color attachments to use for rendering
			unsigned int _renderTargets[4]
				= { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(4, _renderTargets);

			// Depth buffer
			// Delete original buffer first due to glRenderbufferStorage being unable to resize
			glDeleteRenderbuffers(1, &NS_GRAPHICS::SYS_GRAPHICS->GetDepthBuffer());

			glCreateRenderbuffers(1, &NS_GRAPHICS::SYS_GRAPHICS->GetDepthBuffer());
			glBindRenderbuffer(GL_RENDERBUFFER, NS_GRAPHICS::SYS_GRAPHICS->GetDepthBuffer());
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, NS_GRAPHICS::SYS_GRAPHICS->GetDepthBuffer());

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});

		return;
	}

	void WndSystem::Exit()
	{
		glfwTerminate();
		DestroyInstance();
	}

	// Window Event/Message Handler

	// WM = Windows Message. Determined based on user input
	// (e.g. Close application via upper right X) will send WM_CLOSE, which sends WM_DESTROY
	// Note: Only messages we want to specifically handle should be here, else windows will handle them as per default
//	LRESULT WndSystem::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//	{
////#ifdef _EDITOR
////		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
////			return true;
////#endif
//		switch (msg)
//		{
////#ifdef _EDITOR
//		case WM_MOUSEWHEEL:
//		{
//			// Like seriously i don't know how else to get mouse wheels
//			SYS_INPUT->GetSystemMousePos().SetScroll((GET_WHEEL_DELTA_WPARAM(wParam)));
//			break;
//		}
////#endif
//		/*case WM_ACTIVATEAPP: // Used to handle pausing/playing upon alt-tab
//			break;*/
//		case WM_DESTROY:
//			// Perform necessary cleanup
//			SetFullScreenMode(false);
//			PostQuitMessage(0);
//			break;
//
//		/*case WM_PAINT:
//			PAINTSTRUCT ps;
//			HDC hdc = BeginPaint(hWnd, &ps);
//
//			UNREFERENCED_PARAMETER(hdc);
//			EndPaint(hWnd, &ps);
//			break;*/
//
//		default:
//			// If no specified message is handled above, throw to windows to handle as per default
//			return DefWindowProc(hWnd, msg, wParam, lParam);
//		}
//
//		return 0;
//	}

	// Gets the Handler to Window
	HWND& WndSystem::GetHandlerToWindow()
	{
		return hAppWnd;
	}

	//// Gets the Handler to the HDC (Hardware Device Context)
	//HDC& WndSystem::GetHandlerToDeviceContext()
	//{
	//	return hDevContext;
	//}

	//// Gets the Handler to the first GLRenderContext
	//HGLRC& WndSystem::GetHandlerToGLRenderContext()
	//{
	//	return hGLRenderContext;
	//}

	GLFWwindow* WndSystem::GetGLFWWnd()
	{
		return _glfwWnd;
	}

	// Checks if the Window System has initialised
	bool WndSystem::HasInit() const
	{
		return hasInit;
	}

	// Sets the System to swap buffers or not
	void WndSystem::SetSwapBuffers(bool set)
	{
		swap = set;
	}

	// Sets fullscreen or windowed mode
	void WndSystem::SetFullScreenMode(bool set)
	{
		if (hasInit)
		{
			if (isWndMode && set) // To switch to fullscreen & client is currently windowed
			{
				//// Change resolution to fit window
				//DEVMODE resolutionSetting;
				//resolutionSetting.dmSize = sizeof(DEVMODE);
				//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &resolutionSetting);
				//resolutionSetting.dmPelsWidth = iDisplayResW;
				//resolutionSetting.dmPelsHeight = iDisplayResH;
				//resolutionSetting.dmBitsPerPel = 32;
				//resolutionSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT ;

				//ChangeDisplaySettings(&resolutionSetting, CDS_FULLSCREEN);

				//SetWindowLongPtr(hAppWnd, GWL_STYLE, windowStyleFM);
				//SetWindowPos(hAppWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWWINDOW);

				//InvalidateRect(hAppWnd, NULL, TRUE);
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

				glfwSetWindowMonitor(_glfwWnd, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);

				RECT rect; // Making a rect to ensure that size is proper
				GetClientRect(GetHandlerToWindow(), &rect);
				glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);

				// Update rect in SystemMousePosition
				SYS_INPUT->GetSystemMousePos().ResetWinSize();

				//glViewport(0, 0, appWidth, appHeight);
				//SAE_GRAPHICS::system->SetViewport(0, 0); // Reset the viewport position to fit resize

				isWndMode = false;
			}
			else if (!isWndMode && !set) // To switch to windowed & client is currently fullscreen
			{
				//DEVMODE resolutionSetting;
				//resolutionSetting.dmSize = sizeof(DEVMODE);
				//EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &resolutionSetting);
				//resolutionSetting.dmPelsWidth = iDisplayResW;
				//resolutionSetting.dmPelsHeight = iDisplayResH;
				//resolutionSetting.dmBitsPerPel = 32;
				//resolutionSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				//rDevDisplay = { 0, 0, static_cast<long>(appWidth), static_cast<long>(appHeight) };
				//AdjustWindowRect(&rDevDisplay, windowStyleWM, false);
				//cDisplayPosX = (GetSystemMetrics(SM_CXSCREEN) / 2) - static_cast<int>(((rDevDisplay.right - rDevDisplay.left) / 2));
				//cDisplayPosY = (GetSystemMetrics(SM_CYSCREEN) / 2) - static_cast<int>(((rDevDisplay.bottom - rDevDisplay.top) / 2));

				//// Change resolution to fit window
				//ChangeDisplaySettings(&resolutionSetting, 0);

				//SetWindowLongPtr(hAppWnd, GWL_STYLE, windowStyleWM);

				//SetWindowPos(hAppWnd, HWND_TOP, cDisplayPosX, cDisplayPosY, appWidth, appHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

				//InvalidateRect(hAppWnd, NULL, TRUE);

				// 2nd param must be null for windowed mode
				glfwSetWindowMonitor(_glfwWnd, nullptr, 0, 0, appWidth, appHeight, 0);

				RECT rect; // Making a rect to ensure that size is proper
				GetClientRect(GetHandlerToWindow(), &rect);
				glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);

				// Update rect in SystemMousePosition
				SYS_INPUT->GetSystemMousePos().ResetWinSize();

				//SAE_GRAPHICS::system->SetViewport(0, 0); // Reset the viewport position to fit resize

				isWndMode = true;
			}
		}
	}

	// Checks if Window is active
	bool WndSystem::IsActiveWindow()
	{
		return isActiveWindow;
	}

	// Get the width of the application
	UINT WndSystem::GetAppWidth() const
	{
		return appWidth;
	}

	// Get the height of the application
	UINT WndSystem::GetAppHeight() const
	{
		return appHeight;
	}

	// Checks if the game is fullscreen or windowed
	bool WndSystem::GetWindowedMode() const
	{
		return isWndMode;
	}

	// Sets the resolution of the application
	void WndSystem::SetAppResolution(int width, int height)
	{
		if ((width > 0) && (height > 0))
		{
			iDisplayResW = width;
			iDisplayResH = height;
		}
	}

	// Gets the current resolution's height
	int WndSystem::GetResolutionHeight() const
	{
		return iDisplayResH;
	}

	// Gets the current resolution's width
	int WndSystem::GetResolutionWidth() const
	{
		return iDisplayResW;
	}
	/*void WndSystem::SetAppInstance(HINSTANCE& hInstance)
	{
		hAppInstance = hInstance;
	}*/
	void WndSystem::SetAppTitle(std::string& s)
	{
		appTitle = s;
	}
	void WndSystem::SetWindowedSize(int width, int height)
	{
		if (width > 0 && height > 0)
		{
			appWidth = width;
			appHeight = height;
		}

		// GLFW Implementation
		// If initialization is done, we need to use glfw to send updated resize over
		if (hasInit)
		{
			glfwSetWindowSize(_glfwWnd, appWidth, appHeight);
		}
	}
	void WndSystem::SetCursorVisibility(const bool& set)
	{
		if (set == false)
		{
			glfwSetInputMode(_glfwWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			return;
		}
		else
		{
			// Enter here if set is true
			glfwSetInputMode(_glfwWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}