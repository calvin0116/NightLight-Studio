#include "SystemGraphics.h"
#include "..\\Component\Components.h"
#include <iostream>
#include "..\\Component\ComponentTransform.h"


void SystemGraphics::OnFirstStart()
{
	std::cout << "SystemGraphics::OnFirstStart" << std::endl;
}

void SystemGraphics::Load()
{
	std::cout << "SystemGraphics::Load" << std::endl;
	std::cout << "Window's height:" << CONFIG_DATA->GetConfigData().height << std::endl;
	std::cout << "Window's width:" << CONFIG_DATA->GetConfigData().width << std::endl;
}

void SystemGraphics::Init()
{
	std::cout << "SystemGraphics::Init" << std::endl;
}

bool SystemGraphics::Update()
{
	// components are loaded in SystemIO Load()

	//std::cout << "SystemGraphics::Update:" << std::endl;
	auto itr = G_MAINCOMPSET.begin<ComponentRender>();
	auto itrEnd = G_MAINCOMPSET.end<ComponentRender>();
	while (itr != itrEnd)
	{
		// get the obj id
		//std::cout << "Object:" << G_MAINCOMPSET.getObjId(itr) << std::endl;

		// get the transform component from the iterator
		ComponentRender* compR = G_MAINCOMPSET.getComponent<ComponentRender>(itr);
		//std::cout << "Render:" << compR->id << " " << compR->c << std::endl;

		// get another component
		ComponentTransform* compT = G_MAINCOMPSET.getComponent<ComponentTransform>(itr);
		/*
		if (compT != nullptr) // nullptr -> uninitialised or deleted
			std::cout << "Transform:" << compT->_position.x << std::endl;
			*/
		_coreGraphics.AddToRenderSet(compR); // eg. add to render set
		// system should only manage the components
		// CoreGraphics should encapsulate all graphics subsystems aka provide an api for SystemGraphics to call

		//std::cout << std::endl;

		++itr;
	}


	// eg. UI can be put into a seperate component set, G_UICOMPSET
	// allows easier management of entities
	itr = G_UICOMPSET.begin<ComponentRender>();
	itrEnd = G_UICOMPSET.end<ComponentRender>();
	while (itr != itrEnd)
	{
		// get the obj id
		//std::cout << "Object:" << G_UICOMPSET.getObjId(itr) << std::endl;

		// get the transform component from the iterator
		ComponentRender* compR = reinterpret_cast<ComponentRender*>(*itr);
		//std::cout << "Render:" << compR->id << " " << compR->c << std::endl;

		// get the entity from the iterator
		ComponentManager::ComponentSetManager::Entity entity = G_UICOMPSET.getEntity(itr);

		// get transform component
		ComponentTransform* compT = entity.getComponent<ComponentTransform>();
		/*if (compT != nullptr) // nullptr -> uninitialised or deleted
			std::cout << "Transform:" << compT->_position.x << std::endl;
			*/
		//std::cout << std::endl;

		++itr;
	}

	_coreGraphics.Draw(); // eg. call draw fn

	//std::cout << "SystemGraphics::Update: END" << std::endl;
	return true;
}

void SystemGraphics::Exit()
{
	std::cout << "SystemGraphics::Exit" << std::endl;
}

void SystemGraphics::Unload()
{
	std::cout << "SystemGraphics::Unload" << std::endl;
}


void SystemGraphics::Free()
{
	std::cout << "SystemGraphics::Free" << std::endl;
}