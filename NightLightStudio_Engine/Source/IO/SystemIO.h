#pragma once
#include "../../framework.h"
//System Includes
#include "../Core/Singleton.h"
#include "../Core/MySystem.h"

//Sub system
#include "Json/Config.h"

//~!Engine Updated
class SystemIO : public MySystem , public Singleton<SystemIO>
{
private:
	friend Singleton<SystemIO>;	//For singleton to work
public:
	void Load() override;
	void Init() override;
	void GameLoad() override;
	void GameInit() override;
	void Update() override;
	void GameExit() override;
	void Free() override;
	void Exit() override;

	//void Unload();
};

static SystemIO* SYS_IO = SystemIO::GetInstance();