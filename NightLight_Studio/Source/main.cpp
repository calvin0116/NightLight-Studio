// main.cpp : Defines the entry point for the application.
//

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
    MyWindowManager my_wind_manager;
    my_wind_manager.WindowsCreation(hInstance, nCmdShow);

    //System creation / Loading
    SYS_MAN->CombineLoad();

    while (gameLoop)
    {
        //System Init
        SYS_MAN->CombineInit();
        while (running)
        {
            //
            if (!my_wind_manager.Update())
            {
                running = false;
                gameLoop = false;
            }
        }
        //System clear
        SYS_MAN->Exit();
    }

    //System Delete
    SYS_MAN->DestroyInstance();
#ifdef _DEBUG
    system("pause");
#endif // DEBUG

    return  my_wind_manager.GetMsg().wParam; //(int)msg.wParam;
}
