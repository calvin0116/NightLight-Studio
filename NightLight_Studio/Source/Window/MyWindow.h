#pragma once
#include "Header.h"
#include "main.h"
#include "Config.h"

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
// Global Variables:
#define MAX_LOADSTRING 100

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

class MyWindow
{
	int width, height;
    HINSTANCE hInst;
    // current instance
    WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
    WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


    //====== Windows functions ===============//
    //
    //  FUNCTION: MyRegisterClass()
    //
    //  PURPOSE: Registers the window class.
    //
    ATOM MyRegisterClass(HINSTANCE hInstance, int title)
    {
        //load title and window class name
        LoadStringW(hInstance, title, szTitle, MAX_LOADSTRING);
        LoadStringW(hInstance, IDC_PHYSICENGINE, szWindowClass, MAX_LOADSTRING);

        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PHYSICENGINE));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PHYSICENGINE);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        int result = RegisterClassExW(&wcex);
        if (!result)
        {
            system("pause");
        }
        return result;
    }

    //
    //   FUNCTION: InitInstance(HINSTANCE, int)
    //
    //   PURPOSE: Saves instance handle and creates main window
    //
    //   COMMENTS:
    //
    //        In this function, we save the instance handle in a global variable and
    //        create and display the main program window.
    //
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
        hInst = hInstance; // Store instance handle in our global variable

        HWND hWnd = CreateWindowEx(0,szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);
        //CW_USEDEFAULT
        if (!hWnd)
        {
            return FALSE;
        }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

public:
    MyWindow(HINSTANCE hInstance, int nCmdShow, int title)
    {

        ConfigData& cd =  CONFIG_DATA->GetConfigData();

        width = cd.width;
        height = cd.height;
        
        MyRegisterClass(hInstance, title);

        // Perform application initialization:
        if (!InitInstance(hInstance, nCmdShow))
        {
            std::cout << "Window creation failed" << std::endl;
        }
    }
    /*
    //Basic gateway function
    void Init()
    {};
    void Update()
    {};
    void Exit()
    {};
    */
};

