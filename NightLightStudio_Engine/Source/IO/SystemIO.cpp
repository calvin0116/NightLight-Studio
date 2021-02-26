#include "SystemIO.h"

#include "..\\Component\Components.h"
#include "..\\Core\SystemManager.h"


void SystemIO::Load()
{
	TracyMessageL("SystemIO::Load");
	//std::cout << "SystemIO::Load" << std::endl;
	//G_GSM.getScn(); // get the current scene
	//G_GSM.setScn(1); // set the nxt scene

	CONFIG_DATA->Load();
}

void SystemIO::Init()
{
	TracyMessageL("SystemIO::Init");
	//std::cout << "SystemIO::OnFirstStart" << std::endl;
	CONFIG_DATA->Init();
}

void SystemIO::GameInit()
{
}

void SystemIO::GameLoad()
{
}

void SystemIO::Update()
{
}

void SystemIO::GameExit()
{
}


void SystemIO::Free()
{
	CONFIG_DATA->Free();
}

void SystemIO::Exit()
{
	CONFIG_DATA->Exit();
	DestroyInstance();
}

