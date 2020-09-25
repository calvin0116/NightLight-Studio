#pragma once
#include "..\Core\MySystem.h"

class ENGINE_API SystemLogic
{
public:
	void OnFirstStart();

	void Load();

	void Init();

	void Update(float dt);

	void Exit();

	void Free();

	void Unload();
};