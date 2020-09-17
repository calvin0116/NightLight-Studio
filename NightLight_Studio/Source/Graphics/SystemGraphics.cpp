#include "SystemGraphics.h"
#include <iostream>
#include "..\\Component\Components.h"


void SystemGraphics::OnFirstStart()
{
}

void SystemGraphics::Load()
{
}

void SystemGraphics::Init()
{
}

void SystemGraphics::Update(float dt)
{
	// components are loaded in SystemIO Load()

	std::cout << "SystemGraphics::Update:" << std::endl;
	auto itr = G_MAINCOMPSET.csmgr.begin(G_MAINCOMPSET.containerRender);
	auto itrEnd = G_MAINCOMPSET.csmgr.end(G_MAINCOMPSET.containerRender);
	while (itr != itrEnd)
	{
		// get the obj id
		std::cout << "Object:" << G_MAINCOMPSET.csmgr.getObjId(itr) << std::endl;

		// get the transform component from the iterator
		ComponentRender* compR = reinterpret_cast<ComponentRender*>(*itr);
		std::cout << "Render:" << compR->id << " " << compR->c << std::endl;

		// get the entity from the iterator
		ComponentManager::ComponentSetManager::Entity obj = G_MAINCOMPSET.csmgr.getEntity(itr);

		// get transform component
		ComponentTransform* compT = reinterpret_cast<ComponentTransform*>(obj.getComponent(G_MAINCOMPSET.containerTransform));
		if (compT != nullptr) // nullptr -> uninitialised or deleted
			std::cout << "Transform:" << compT->_position._x << std::endl;

		_coreGraphics.AddToRenderSet(compR); // eg. add to render set
		// system should only manage the components
		// CoreGraphics should encapsulate all graphics subsystems aka provide an api for SystemGraphics to call

		std::cout << std::endl;

		++itr;
	}


	// eg. UI can be put into a seperate component set, G_UICOMPSET
	// allows easier management of entities
	itr = G_UICOMPSET.csmgr.begin(G_UICOMPSET.containerRender);
	itrEnd = G_UICOMPSET.csmgr.end(G_UICOMPSET.containerRender);
	while (itr != itrEnd)
	{
		// get the obj id
		std::cout << "Object:" << G_UICOMPSET.csmgr.getObjId(itr) << std::endl;

		// get the transform component from the iterator
		ComponentRender* compR = reinterpret_cast<ComponentRender*>(*itr);
		std::cout << "Render:" << compR->id << " " << compR->c << std::endl;

		// get the entity from the iterator
		ComponentManager::ComponentSetManager::Entity obj = G_UICOMPSET.csmgr.getEntity(itr);

		// get transform component
		ComponentTransform* compT = reinterpret_cast<ComponentTransform*>(obj.getComponent(G_UICOMPSET.containerTransform));
		if (compT != nullptr) // nullptr -> uninitialised or deleted
			std::cout << "Transform:" << compT->_position._x << std::endl;

		std::cout << std::endl;

		++itr;
	}

	_coreGraphics.Draw(); // eg. call draw fn

	std::cout << "SystemGraphics::Update: END" << std::endl;
}

void SystemGraphics::Exit()
{
}

void SystemGraphics::Free()
{
}

void SystemGraphics::Unload()
{
}
