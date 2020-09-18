#pragma once

#include "GameStateManager.h"


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


class ENGINE_API FluffyUnicornEngine
{
	enum ENGINESTATE
	{
		ENGINE_QUIT = 0,
		ENGINE_UPDATE,
	};
	ENGINESTATE _engineState;
	float _prevdt;
public:
	FluffyUnicornEngine() = default;
	~FluffyUnicornEngine() = default;

	void Init();
	void Run();
};


//class FluffyUnicornEngine
//{
//	enum ENGINESTATE
//	{
//		ENGINE_QUIT = 0,
//		ENGINE_UPDATE,
//	};
//	ENGINESTATE _engineState;
//	float _prevdt;
//public:
//	FluffyUnicornEngine() = default;
//	~FluffyUnicornEngine() = default;
//
//	void Init();
//	void Run();
//};
//
