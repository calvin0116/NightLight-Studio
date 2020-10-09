#pragma once
//#include "../Core/System.h"

class SystemLogic //: public System
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