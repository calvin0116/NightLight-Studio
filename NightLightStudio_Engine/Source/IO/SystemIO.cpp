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

	// build objects from json !!! 
	// - shift this to a seperate class/function !!!

	void* newComponent = nullptr;

	// WHILE OBJECTS
	int newObjId = G_MAINCOMPSET.csmgr.BuildObject();

	// WHILE COMPONENTS
	ComponentRender newCompComponentRender
	{
		0,
		"Hello World",
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
	};
	newComponent = &newCompComponentRender;
	G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);
	// WHILE COMPONENTS END
	// WHILE OBJECTS END

	// add 2nd obj 
	newObjId = G_MAINCOMPSET.csmgr.BuildObject();

	ComponentTransform compT;
	compT._position._x = 1.11f;
	G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerTransform, newObjId, &compT);

	newCompComponentRender.id = 1;
	G_MAINCOMPSET.csmgr.AttachComponent(G_MAINCOMPSET.containerRender, newObjId, newComponent);


	// add obj to another component set
	newCompComponentRender.id = 999;
	newObjId = G_UICOMPSET.csmgr.BuildObject();
	G_UICOMPSET.csmgr.AttachComponent(G_UICOMPSET.containerRender, newObjId, newComponent);

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
