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

		std::cout << std::endl;

		++itr;
	}
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
