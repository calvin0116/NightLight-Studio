#pragma once
#include "../../framework.h"
//System Includes
#include "../Core/Singleton.h"
#include "../Core/MySystem.h"

//Sub system
#include "Json/Config.h"

class ENGINE_API SystemIO : public MySystem , public Singleton<SystemIO>
{
private:
	friend Singleton<SystemIO>;	//For singleton to work
public:
	void OnFirstStart();

	void EarlyLoad();

	void Init();

	void Update(float dt);

	void Exit();

	void Free();

	void Unload();
};

static SystemIO* SYS_IO = SystemIO::GetInstance();