// ConsoleApplicationT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ComponentManager.h"

int ComponentTest()
{
	struct ComponentTransform
	{
		int objId0;
		float s[16];
		float r[16];
		float t[16];
		int objId1;
	};

	struct ComponentRender
	{
		int objId0;
		float fluffyunicorn[4];
		int objId1;
	};

	struct ComponentPhysics
	{
		int objId0;
		int halo[4];
		int objId1;
	};



	ComponentManager mgrCom;

	//mgrCom.test0();
	//std::cout << "Test!\n";

	//mgrCom.test1();
	//std::cout << "Test!\n";

	//mgrCom.test2();
	//std::cout << "Test!\n";


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Init
	//// init with ComponentSetFactory
	//// StartBuild -> add components -> Build
	//// produces a component set

	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	// init the build
	comsetFac.StartBuild();

	// adds a component using component size // adding ComponentTransform
	ComponentManager::ContainerID containerTransform = comsetFac.AddComponent(sizeof(ComponentTransform)); // save the container id

	// adds a component using settings // ComponentRender
	ComponentManager::ContainerSettings set;
	set.elementSize = sizeof(ComponentRender);
	ComponentManager::ContainerID containerRender = comsetFac.AddComponent(set); // save the container id

	// adding ComponentPhysics
	ComponentManager::ContainerID containerPhysics = comsetFac.AddComponent(sizeof(ComponentPhysics)); // save the container id

	// builds the component set
	ComponentManager::ComponentSet* compSet0 = comsetFac.Build();

	//// Init END
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// component manager supposed to manage the components 
	//// right now it only has free

	// component manager
	ComponentManager mgrComp;

	// adds the component set to the manager
	mgrComp.AddComponentSet(compSet0);

	//// 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// component set manager manages a set of components
	//// we can have multiple sets of components for different things, like game objects / ui / camera / etc.

	// init component set manager
	ComponentManager::ComponentSetManager compSetMgr(compSet0);

	////////////////////////////////////////////////
	// build a new object 0
	int newObjId0 = compSetMgr.BuildObject();

	// attach component
	ComponentTransform newComponentT =
	{
		newObjId0,
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
		newObjId0,
	};
	compSetMgr.AttachComponent(containerTransform, newObjId0, &newComponentT); // attach to the correct container and obj

	// attach another component
	ComponentRender newComponentR =
	{
		newObjId0,
		{1.11f, 2.22f, 3.33f, 4.44f},
		newObjId0,
	};
	compSetMgr.AttachComponent(containerRender, newObjId0, &newComponentR); // attach to the correct container and obj
	//
	////////////////////////////////////////////////
	// build object 1
	int newObjId1 = compSetMgr.BuildObject();
	newComponentT.objId0 = newObjId1;
	newComponentT.objId1 = newObjId1;
	newComponentR.objId0 = newObjId1;
	newComponentR.objId1 = newObjId1;
	ComponentPhysics newComponentP =
	{
		newObjId1,
		{111, 222, 333, 444},
		newObjId1,
	};
	compSetMgr.AttachComponent(containerTransform, newObjId1, &newComponentT);
	compSetMgr.AttachComponent(containerRender, newObjId1, &newComponentR);
	compSetMgr.AttachComponent(containerPhysics, newObjId1, &newComponentP);
	//
	////////////////////////////////////////////////
	// build object 2
	int newObjId2 = compSetMgr.BuildObject();
	newComponentT.objId0 = newObjId2;
	newComponentT.objId1 = newObjId2;
	newComponentP.objId0 = newObjId2;
	newComponentP.objId1 = newObjId2;
	compSetMgr.AttachComponent(containerTransform, newObjId2, &newComponentT);
	compSetMgr.AttachComponent(containerPhysics, newObjId2, &newComponentP);
	//
	////////////////////////////////////////////////
	// build object 3
	int newObjId3 = compSetMgr.BuildObject();
	newComponentT.objId0 = newObjId3;
	newComponentT.objId1 = newObjId3;
	newComponentR.objId0 = newObjId3;
	newComponentR.objId1 = newObjId3;
	newComponentP.objId0 = newObjId3;
	newComponentP.objId1 = newObjId3;
	compSetMgr.AttachComponent(containerTransform, newObjId3, &newComponentT);
	compSetMgr.AttachComponent(containerRender, newObjId3, &newComponentR);
	compSetMgr.AttachComponent(containerPhysics, newObjId3, &newComponentP);
	//
	////////////////////////////////////////////////

	// get components from object 1
	ComponentTransform* componentTransform = reinterpret_cast<ComponentTransform*>(compSetMgr.getComponent(containerTransform, newObjId1));
	ComponentRender* componentRender = reinterpret_cast<ComponentRender*>(compSetMgr.getComponent(containerRender, newObjId1));
	ComponentPhysics* componentPhysics = reinterpret_cast<ComponentPhysics*>(compSetMgr.getComponent(containerPhysics, newObjId1));
	std::cout << "Get component 1:" << std::endl;
	std::cout << componentTransform->objId0 << std::endl;
	std::cout << componentTransform->objId1 << std::endl;
	std::cout << componentRender->objId1 << std::endl;
	std::cout << componentRender->objId1 << std::endl;
	std::cout << componentPhysics->objId1 << std::endl;
	std::cout << componentPhysics->objId1 << std::endl;
	// should print all 1 for component 1

	// fn to print all components with transform using the iterator
	auto printAllTransform = [&]()
	{
		std::cout << "Print all:" << std::endl;
		auto itr = compSetMgr.begin(containerTransform);
		auto itrEnd = compSetMgr.end(containerTransform);
		while (itr != itrEnd)
		{
			// get the obj id
			std::cout << "Object:" << compSetMgr.getObjId(itr) << std::endl;

			// get the transform component from the iterator
			ComponentTransform* compT = reinterpret_cast<ComponentTransform*>(*itr);
			std::cout << "Transform:" << compT->objId0 << " " << compT->objId1 << std::endl;

			// get the object from the iterator
			ComponentManager::ComponentSetManager::Entity obj = compSetMgr.getEntity(itr);

			// print render component
			ComponentRender* compR = reinterpret_cast<ComponentRender*>(obj.getComponent(containerRender));
			if (compR != nullptr) // nullptr -> uninitialised or deleted
				std::cout << "Render:" << compR->objId0 << " " << compR->objId1 << std::endl;

			// print physics component
			ComponentPhysics* compP = reinterpret_cast<ComponentPhysics*>(obj.getComponent(containerPhysics));
			if(compP != nullptr) // nullptr -> uninitialised or deleted 
				std::cout << "Physics:" << compP->objId0 << " " << compP->objId1 << std::endl;

			std::cout << std::endl;

			++itr;
		}
	};

	printAllTransform();


	std::cout << "Remove render from object 1:" << std::endl;
	compSetMgr.RemoveComponent(containerRender, newObjId1);

	printAllTransform();


	std::cout << "Remove object 2:" << std::endl;
	compSetMgr.UnBuildObject(newObjId2);

	printAllTransform();


	std::cout << "Remove object 0:" << std::endl;
	compSetMgr.UnBuildObject(newObjId0);

	printAllTransform();

	//// 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Free

	mgrComp.Free(); // rmb to free

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
