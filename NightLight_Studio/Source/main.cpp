// main.cpp : Defines the entry point for the application.
//
#include "main.h"
#include "SystemManager.h"
// Forward declarations of functions included in this code module:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);

//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //System Manager Start up
    SYS_MAN->StartUp();
    //Load config data
    CONFIG_DATA->LoadUp();
    
    //Flag for game and system loop
    bool running = true;
    bool gameLoop = true;
    
    //Windows creation
    WIN_MAN->WindowsCreation(hInstance, nCmdShow);

    //System creation / Loading
    SYS_MAN->CombineLoad();

    while (gameLoop)
    {
        //System Init
        SYS_MAN->CombineInit();
        while (running)
        {
            SYS_MAN->CombineUpdate();

            if (!WIN_MAN->Update())
            {
                running = false;
                gameLoop = false;
            }
        }
        //System clear
        SYS_MAN->Exit();
    }

#ifdef _DEBUG
    system("pause");
#endif // DEBUG
    int m = WIN_MAN->GetMsg().wParam;

    //Release of system resource
    SYS_MAN->Unload();
    //Release of engine resource
    CONFIG_DATA->DestroyInstance();
    WIN_MAN->DestroyInstance();
    SYS_MAN->DestroyInstance();
    return  m; //(int)msg.wParam;
}
