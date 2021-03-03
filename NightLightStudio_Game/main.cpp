
#include "..\NightLightStudio_Engine\Source\Core\Engine.h"
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

/// FOR THOSE INTERESTED: INCLUDING THIS ON TOP OF YOUR C++ FILES WILL GIVE YOU WHERE THE MEMORY LEAK OCCURS
/*
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
*/

//int main()
//{
//	FluffyUnicornEngine engine;
//
//	engine.Init();
//	
//	engine.Run();
//
//	engine.Exit();
//	return 0;
//}

// Convert to WinMain

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef _DEBUG
	// Spawn console for debugging
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitleA("DEBUGGING CONSOLE");
		//std::cout << "Debug Console Initialized." << std::endl;

		//Prevent closing from console window to show unnecessary mem leak
		HWND hwnd = ::GetConsoleWindow();
		if (hwnd != NULL)
		{
			HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
			if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
#endif

	FluffyUnicornEngine engine;

	engine.Init(hInstance);

	try
	{
		engine.Run();
	}
	catch (std::exception e)
	{
		// Logging?
		//engine.Exit();
		//throw;
		std::string errorMsg("Terminated due to unexpected error!\n");
		errorMsg.append(e.what());
		int msgboxID = MessageBox(
			NULL,
			errorMsg.c_str(),
			"Unexpected Error Occurred!",
			MB_ICONERROR | MB_OK | MB_DEFBUTTON1
		);
	}

	engine.Exit();

#ifdef _DEBUG
	FreeConsole();
#endif

	//NOTE: FOR ME TO SEE MEMORY LEAK
	//PS COMPUTER CMI
	//AUTO DUMPING DOESNT WORK FOR ME
	//UNCOMMENT TO SEE MORE LEAKS FOR ME
	//_CrtDumpMemoryLeaks();

	return 0;
}