
#include "..\NightLightStudio_Engine\Source\Core\Engine.h"

int main()
{
	FluffyUnicornEngine engine;

	engine.Init();
	
	engine.Run();


	return 0;
}

// Convert to WinMain

//int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//	UNREFERENCED_PARAMETER(nShowCmd);
//
//
//	// Get static pointer to window system
//	//NS_WINDOW::pApp->SetAppInstance(hInstance);
//
//	//NS_WINDOW::pApp->Init();
//
//	/*while (1)
//	{
//		NS_WINDOW::pApp->Update();
//		NS_GRAPHICS::pGraphicsSys->Render();
//	}*/
//
//	FluffyUnicornEngine engine;
//
//	engine.Init(hInstance);
//
//	engine.Run();
//
//	return 0;
//}