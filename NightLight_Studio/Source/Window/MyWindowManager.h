#pragma once

#include "MyWindow.h"
#include "Singleton.h"

#include <io.h>
//#include <stdlib.h>
#include <shobjidl.h>
#include <comdef.h>
#include <fcntl.h>



class MyWindowManager : public Singleton<MyWindowManager>
{
private:
	//For singleton to access constructor / destructor
	friend Singleton<MyWindowManager>;

	HINSTANCE hInstance;
	MSG msg;

public:
	void OpenDebugConsole()
	{
		HANDLE stdHandle;
		int hConsole;
		FILE* fp;

		AllocConsole();
		stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		hConsole = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
		fp = _fdopen(hConsole, "w");
		freopen_s(&fp, "CONOUT$", "w", stdout);

		printf("Hello console on");
	}

	void WindowsCreation(HINSTANCE _hInstance, int nCmdShow)
	{
		OpenDebugConsole();

        hInstance = _hInstance;
		MyWindow MyWind(hInstance, nCmdShow, IDS_APP_TITLE);
		//MyWindow MyWind(hInstance, nCmdShow, IDS_APP_TITLE);
		std::cout << "Window creation complete" << std::endl;
	}

	bool Update()
	{
		//std::cout << "Hello" << std::endl;

        HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHYSICENGINE));

        // Main message loop:
        //while (GetMessage(&msg, nullptr, 0, 0))
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//std::cout << msg.message << std::endl;
			if (msg.message == WM_QUIT)
			{
				std::cout << "Quiting..." << std::endl;
				return false;
			}
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
		return true;
	}

	MSG GetMsg()
	{
		return msg;
	}

};

//Window Manager
static MyWindowManager* const WIN_MAN = MyWindowManager::GetInstance();
