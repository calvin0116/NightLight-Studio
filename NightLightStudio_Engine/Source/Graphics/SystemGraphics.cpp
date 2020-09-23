#include "SystemGraphics.h"
#include "..\\Component\Components.h"

#include <iostream>

void SystemGraphics::OnFirstStart()
{
}

void SystemGraphics::Load()
{
}

void SystemGraphics::Init()
{
}

bool SystemGraphics::Update()
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

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// Get entity first then get component
		//// The reason why get entity first:
		//// If in the future we add parent-child to entities, 
		//// I intend to abstract obj->getChild() (returns child entity) or sth similar

		//// get the entity from the iterator
		//ComponentManager::ComponentSetManager::Entity obj = G_MAINCOMPSET.csmgr.getEntity(itr);

		//// get transform component
		//ComponentTransform* compT = reinterpret_cast<ComponentTransform*>(obj.getComponent(G_MAINCOMPSET.containerTransform));
		//if (compT != nullptr) // nullptr -> uninitialised or deleted
		//	std::cout << "Transform:" << compT->_position._x << std::endl;

		//// 
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// another way to get component
		ComponentTransform* compT = reinterpret_cast<ComponentTransform*>(G_MAINCOMPSET.csmgr.getComponent(G_MAINCOMPSET.containerTransform, itr));
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
	return true;
}

void SystemGraphics::Exit()
{
}
/*
void SystemGraphics::Free()
{
}*/

void SystemGraphics::Unload()
{
}
