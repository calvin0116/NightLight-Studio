
#include "..\NightLightStudio_Engine\Source\Core\Engine.h"

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


	// Get static pointer to window system
	//NS_WINDOW::pApp->SetAppInstance(hInstance);

	//NS_WINDOW::pApp->Init();

	/*while (1)
	{
		NS_WINDOW::pApp->Update();
		NS_GRAPHICS::pGraphicsSys->Render();
	}*/

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

	return 0;
}