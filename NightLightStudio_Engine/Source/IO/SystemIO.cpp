#include "SystemIO.h"

#include "..\\Component\Components.h"
#include "..\\Core\SystemManager.h"

void SystemIO::OnFirstStart()
{
	std::cout << "SystemIO::OnFirstStart" << std::endl;
	CONFIG_DATA->OnFirstStart();
}



void SystemIO::Load()
{
	std::cout << "SystemIO::Load" << std::endl;
	//G_GSM.getScn(); // get the current scene
	//G_GSM.setScn(1); // set the nxt scene
	CONFIG_DATA->Load();
}

void SystemIO::Init()
{
}

void SystemIO::Update(float )
{
}

void SystemIO::Exit()
{
}

void SystemIO::Free()
{
}

void SystemIO::Unload()
{
}
