#pragma once

#include "CoreGraphics.h"

class SystemGraphics
{
	CoreGraphics _coreGraphics;
public:
	void OnFirstStart();

	void Load();

	void Init();

	void Update(float dt);

	void Exit();

	void Free();

	void Unload();
};