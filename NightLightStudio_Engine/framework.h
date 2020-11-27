#pragma once

// Seemingly no purpose here so commented to remove warning
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
//#include <windows.h>
//#define NOGDI
//THIS IS TO PREVENT STD::MIN AND MIN MACRO CONFLICT
#define NOMINMAX

//C++ Header
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

//Json file path
const std::string json_path = "../NightLightStudio_Game/JsonFile";
const std::string prefab_path = "../NightLightStudio_Game/JsonFile";
//#ifdef ENGINE_EXPORTS
//#define ENGINE_API __declspec(dllexport)
//#else
//#define ENGINE_API __declspec(dllimport)
//#endif


// References
// https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2019
// https://docs.microsoft.com/en-us/cpp/cpp/dllexport-dllimport?view=vs-2019
// https://docs.microsoft.com/en-us/cpp/build/linking-an-executable-to-a-dll?view=vs-2019
// https://docs.microsoft.com/en-us/cpp/cpp/using-dllimport-and-dllexport-in-cpp-classes?view=vs-2019
// https://docs.microsoft.com/en-us/visualstudio/ide/how-to-create-and-remove-project-dependencies?view=vs-2019

#define ENGINE_API   __declspec( dllexport )

#define _EDITOR