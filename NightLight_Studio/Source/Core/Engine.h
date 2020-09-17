#pragma once

#include "GameStateManager.h"

enum ENGINESTATE
{
	ENGINE_QUIT = 0,
	ENGINE_UPDATE,
};

class FluffyUnicornEngine
{
	ENGINESTATE _engineState;
	float _prevdt;
public:
	FluffyUnicornEngine() = default;
	~FluffyUnicornEngine() = default;

	void Init();
	void Run();
};

