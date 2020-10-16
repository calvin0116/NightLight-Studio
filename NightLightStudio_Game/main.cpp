
#include "..\NightLightStudio_Engine\Source\Core\Engine.h"
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

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
		std::cout << "Debug Console Initialized." << std::endl;
	}
#endif

	FluffyUnicornEngine engine;

	engine.Init(hInstance);

	engine.Run();

	engine.Exit();

#ifdef _DEBUG
	FreeConsole();
#endif

	return 0;
}