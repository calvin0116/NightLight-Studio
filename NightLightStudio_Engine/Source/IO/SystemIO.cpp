#include "SystemIO.h"

#include "..\\Component\Components.h"
#include "..\\Core\SystemManager.h"

void SystemIO::OnFirstStart()
{
}



void SystemIO::Load()
{
	
	G_GSM.getScn(); // get the current scene
	G_GSM.setScn(1); // set the nxt scene

}

void SystemIO::Init()
{
}

void SystemIO::Update(float dt)
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
