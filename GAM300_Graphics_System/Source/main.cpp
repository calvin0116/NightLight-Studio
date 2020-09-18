#include "WndUtils.h"
#include "WndSystem.h"

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	// Get static pointer to window system
	NS_WINDOW::pApp->SetAppInstance(hInstance);

	NS_WINDOW::pApp->Init();

	while (1)
	{
		NS_WINDOW::pApp->Update();
	}

	return 0;
}