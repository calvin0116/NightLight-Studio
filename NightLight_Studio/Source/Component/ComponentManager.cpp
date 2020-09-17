
#include "ComponentManager.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////
//// ComponentSetFactory

void ComponentManager::ComponentSetFactory::StartBuild()
{
	compSet = new ComponentSet();
}

ComponentManager::ContainerID ComponentManager::ComponentSetFactory::AddComponent(ComponentMemoryManager::ComponentTypeSettings set)
{
	// create new container and adds id to list
	ContainerID conid = compSet->cmm.createNewComponentType(set);
	compSet->componentContainerIDs.push_back(conid);

	return conid;
}

ComponentManager::ContainerID ComponentManager::ComponentSetFactory::AddComponent(size_t size)
{
	// sets only size, default settings
	ComponentMemoryManager::ComponentTypeSettings set;
	set.elementSize = size;
	return AddComponent(set); // calls fn
}

ComponentManager::ComponentSet* ComponentManager::ComponentSetFactory::Build()
{
	// build obj
	size_t size = sizeof(ComponentManager::ComponentSet::ObjectData);
	size += sizeof(ComponentManager::ComponentSet::ObjectData::ComponentData) * compSet->componentContainerIDs.size();

	// using default settings
	ComponentMemoryManager::ComponentTypeSettings set;
	set.elementSize = size;
	compSet->objSize = size;

	// save objContainerId // create obj container
	compSet->objContainerId = compSet->cmm.createNewComponentType(set);

	// set to nullptr and return
	ComponentManager::ComponentSet* returnCompSet = compSet;
	compSet = nullptr;
	return returnCompSet;
}


ComponentManager::ComponentSetFactory::~ComponentSetFactory()
{
	delete compSet;
}

//// ComponentSetFactory END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// ComponentSetManager

int ComponentManager::ComponentSetManager::BuildObject()
{
	// new obj instance
	char* newObj = reinterpret_cast<char*>(malloc(compSet->objSize));

	if (newObj != nullptr)
		memset(newObj, -1, compSet->objSize); // init
	else
		throw; // err

	// insert the obj
	int objId = compSet->cmm.insertIntoContainer(compSet->objContainerId, newObj);

	// free the obj instance
	free(reinterpret_cast<void*>(newObj));
	// get the obj from the container
	newObj = compSet->cmm.getElementAt(compSet->objContainerId, objId);

	// set the obj
	reinterpret_cast<ComponentManager::ComponentSet::ObjectData*>(newObj)->objId = objId;

	return objId;
}

void* ComponentManager::ComponentSetManager::AttachComponent(ComponentManager::ContainerID compId, int objId, void* newComp)
{
	// insert the component
	int newCId = compSet->cmm.insertIntoContainer(compId, reinterpret_cast<char*>(newComp));

	// set object component data
	ComponentSet::ObjectData::ComponentData* compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId));
	compData->containerId = compId; // set container id
	compData->compPtr = reinterpret_cast<void*>( // set component ptr within the container
		compSet->cmm.getElementAt(compId, newCId)
		);;
	compData->containerIndex = newCId; // set component index within the container

	// !


	// return the component
	return compData->compPtr;
}

void ComponentManager::ComponentSetManager::RemoveComponent(ComponentManager::ContainerID compId, int objId)
{
	// get the index
	ComponentSet::ObjectData::ComponentData* compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId));
	int index = compData->containerIndex;

	// remove
	compSet->cmm.removeFromContainer(compId, index);

	// !


	// reset
	compData->containerId = -1;
	compData->containerIndex = -1;
	compData->compPtr = nullptr;
}

void ComponentManager::ComponentSetManager::UnBuildObject(int objId)
{
	// get the object
	char* obj = compSet->cmm.getElementAt(compSet->objContainerId, objId);

	// uninit/free the components
	for (int id : compSet->componentContainerIDs)
	{
		// there is a better way to do it im just too lazy now

		ComponentSet::ObjectData::ComponentData* compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(id, objId));

		if (compData->containerId != -1 && compData->containerIndex != -1) // skip if uninit
		{
			// remove
			compSet->cmm.removeFromContainer(compData->containerId, compData->containerIndex);

			// !


			//reset
			compData->containerId = -1;
			compData->containerIndex = -1;
			compData->compPtr = nullptr;
		}
	}

	// uninit the obj
	reinterpret_cast<ComponentSet::ObjectData*>(obj)->objId = -1;

	// remove the object
	compSet->cmm.removeFromContainer(compSet->objContainerId, objId);
}

void* ComponentManager::ComponentSetManager::getComponent(ComponentManager::ContainerID compId, int objId)
{	
	// get the index
	ComponentSet::ObjectData::ComponentData* compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId));
	int index = compData->containerIndex;

	// compData->compPtr; // <-- 
	if (compData->compPtr == nullptr)
		return nullptr;
		//return compSet->cmm.getElementAt(compId, index); // accessing deleted component !!! <- can use this to return deleted data
	else if (compData->compPtr == (ComponentSet::ObjectData::ComponentData*)0xFFFFFFFFFFFFFFFF) // !
		return nullptr; // accessing uninitialised component !!!
	return compData->compPtr; 

	// get the component
	//return compSet->cmm.getElementAt(compId, index);
}

char* ComponentManager::ComponentSetManager::getObjectComponent(ComponentManager::ContainerID compId, int objId)
{
	char* obj = compSet->cmm.getElementAt(compSet->objContainerId, objId);

	// obj building
	//size_t size = sizeof(ManagerComponent::ComponentSet::Object);
	//size += sizeof(ManagerComponent::ComponentSet::Object::Component) * compSet->componentContainerIDs.size();

	obj += sizeof(ComponentManager::ComponentSet::ObjectData);

	// find the container id
	int n = 0;
	for (int id : compSet->componentContainerIDs)
	{
		if (id == compId)
			break;
		++n;
	}

	// get the position of the component data
	obj += sizeof(ComponentManager::ComponentSet::ObjectData::ComponentData) * n;

	// check
	//ComponentSet::Object::Component* compData = reinterpret_cast<ComponentSet::Object::Component*>(getObjectComponent(compId, objId));
	//if (compData->containerId != compId)
	//	throw;

	//for (int i = 0; i < compSet->componentContainerIDs.size(); ++i)
	//{
	//}

	return obj;
}


//////////////////////////
//// Iterator
//////////////////////////


ComponentManager::ComponentSetManager::Iterator ComponentManager::ComponentSetManager::begin(ContainerID comT)
{
	// comp itr
	ComponentSetManager::Iterator newItr;
	//newItr.compSet = this;
	newItr.containerId = comT;
	newItr.memItr = compSet->cmm.begin(comT);


	return newItr;
}

ComponentManager::ComponentSetManager::Iterator ComponentManager::ComponentSetManager::end(ContainerID comT)
{
	// comp itr
	ComponentSetManager::Iterator newItr;
	//newItr.compSet = this;
	newItr.containerId = comT;
	newItr.memItr = compSet->cmm.end(comT);


	return newItr;
}

int ComponentManager::ComponentSetManager::getObjId(Iterator itr)
{
	int objId = -1;

	//compSet->cmm.getElementAt(compSet->objContainerId, 0);
	auto itrOb = compSet->cmm.begin(compSet->objContainerId);
	auto itrObEnd = compSet->cmm.end(compSet->objContainerId);
	while (itrOb != itrObEnd)
	{
		char* obj = (*itrOb);

		ComponentSet::ObjectData* obj_o = reinterpret_cast<ComponentSet::ObjectData*>(obj);
		

		obj += sizeof(ComponentManager::ComponentSet::ObjectData);

		// find the container id
		int n = 0;
		for (int id : compSet->componentContainerIDs)
		{
			if (id == itr.containerId)
				break;
			++n;
		}

		// get the position of the component data
		obj += sizeof(ComponentManager::ComponentSet::ObjectData::ComponentData) * n;

		//check
		ComponentSet::ObjectData::ComponentData* comp = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(obj);
		if (comp->containerIndex == itr.memItr.getCurrentIndex())
		{
			objId = obj_o->objId;
			break;
		}

		++itrOb;
	}

	if (objId == -1) throw;

	return objId;
}

//////////////////////////
//// Object
//////////////////////////

ComponentManager::ComponentSetManager::Entity::Entity(ComponentSetManager* csm, int oid) : compSetMgr(csm), objId(oid)
{
}

void* ComponentManager::ComponentSetManager::Entity::getComponent(int compId)
{
	return compSetMgr->getComponent(compId, objId);
}


//////////////////////////

ComponentManager::ComponentSetManager::Entity ComponentManager::ComponentSetManager::getEntity(Iterator itr)
{
	Entity obj(this, this->getObjId(itr));
	//obj.compSetMgr = this;
	//obj.objId = this->getObjId(itr);

	return obj;
}


//// ComponentSetManager END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// ComponentManager::ComponentSetManager::Iterator


bool ComponentManager::ComponentSetManager::Iterator::operator==(ComponentManager::ComponentSetManager::Iterator& itr)
{

	return (memItr == itr.memItr); 
}

bool ComponentManager::ComponentSetManager::Iterator::operator!=(ComponentManager::ComponentSetManager::Iterator& itr)
{

	return (memItr != itr.memItr); 
}

void* ComponentManager::ComponentSetManager::Iterator::operator*()
{
	return reinterpret_cast<void*>(*memItr);
}

ComponentManager::ComponentSetManager::Iterator& ComponentManager::ComponentSetManager::Iterator::operator++()
{
	++memItr;

	return *this;
}

ComponentManager::ComponentSetManager::Iterator ComponentManager::ComponentSetManager::Iterator::operator++(int)
{
	ComponentManager::ComponentSetManager::Iterator newItr = *this; // post

	++memItr;

	return newItr;
}

//// ComponentManager::ComponentSet::Iterator END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////

void ComponentManager::AddComponentSet(ComponentSet* compSet)
{
	// define new component type
	ContainerID newComponentSetID = ComponentSets.size();
	while (ComponentSets.find(newComponentSetID) != ComponentSets.end())
	{
		// find a free id
		newComponentSetID++;
	} // loop ends when id is not found -> unique id

	ComponentSets.insert(std::pair<ContainerID, ComponentSet*>(newComponentSetID, compSet));
}

void ComponentManager::Free()
{
	for (std::pair<ContainerID, ComponentSet*> p : ComponentSets)
	{
		ComponentSet* cs = p.second;
		cs->cmm.freeAll();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////








// test

bool ComponentManager::IteratorM::operator==(IteratorM& itr)
{
	return mci == itr.mci;
}

bool ComponentManager::IteratorM::operator!=(IteratorM& itr)
{
	return mci != itr.mci;
}

char* ComponentManager::IteratorM::operator*()
{
	return *mci;
}

ComponentManager::IteratorM& ComponentManager::IteratorM::operator++()
{
	++mci;
	return *this;
}

ComponentManager::IteratorM ComponentManager::IteratorM::operator++(int)
{
	mci++;
	return *this;
}

ComponentManager::IteratorM ComponentManager::end(ContainerID comT)
{
	IteratorM itr;
	itr.mci = cmm.end(comT);
	return itr;
}

ComponentManager::IteratorM ComponentManager::begin(ContainerID comT)
{
	IteratorM itr;
	itr.mci = cmm.begin(comT);
	return itr;
}

ComponentManager::ContainerID ComponentManager::createNewComponentType(ContainerSettings set)
{
	return cmm.createNewComponentType(set);
}

char* ComponentManager::getElementAt(ContainerID comT, int index)
{
	return cmm.getElementAt(comT, index);
}

int ComponentManager::insertIntoContainer(ContainerID comT, char* obj)
{
	return cmm.insertIntoContainer(comT, obj);
}

void ComponentManager::removeFromContainer(ContainerID comT, int index)
{
	cmm.removeFromContainer(comT, index);
}

void ComponentManager::removeFromContainer(IteratorM& itr)
{
	cmm.removeFromContainer(itr.mci);
}

void ComponentManager::freeAll()
{
	cmm.freeAll();
}










///////////////////////////////////////////////////////////////////////////////////////////////////
////  Test Fns


void ComponentManager::test0()
{
	struct ComponentT
	{
		float vert[4];
		int id;
	};

	struct ComponentT2
	{
		int id;
		float vertT[16];
		int t1;
		int t2;
		float vert[4];
	};

	std::cout << "Hello World!\n";

	std::cout << sizeof(char) << "\n";

	ComponentMemoryManager comMgr;

	ComponentMemoryManager::ComponentTypeSettings csetT;
	csetT.elementSize = sizeof(ComponentT);

	ComponentMemoryManager::ComponentType ctT = comMgr.createNewComponentType(csetT);


	for (int i = 0; i < 1000; ++i)
	{
		ComponentT newComp
		{
			{1.0f, 2.0f, 3.0f, 4.0f},
			i
		};

		comMgr.insertIntoContainer(ctT, reinterpret_cast<char*>(&newComp));
	}

	ComponentT* cT = reinterpret_cast<ComponentT*>(comMgr.getElementAt(ctT, 0));

	std::cout << cT->id << std::endl;
	std::cout << cT->vert[0] << std::endl;
	std::cout << cT->vert[3] << std::endl;

	comMgr.removeFromContainer(ctT, 159);
	comMgr.removeFromContainer(ctT, 784);

	ComponentMemoryManager::MemConIterator itr = comMgr.begin(ctT);
	itr = comMgr.begin(ctT);
	auto itrEnd = comMgr.end(ctT);
	int i = 0;

	while (itr != itrEnd)
	{
		ComponentT* testC = reinterpret_cast<ComponentT*>(*itr);

		if (i != testC->id)
		{
			std::cout << testC->id << std::endl;
			std::cout << testC->vert[2] << std::endl;

			++i;
		}

		++i;
		++itr;
	}


	// DUN ANYHOW FREE !!!
	comMgr.freeAll();
}

void ComponentManager::test1()
{
	struct ComponentT
	{
		float vert[4];
		int id;
	};

	//ManagerComponent::ComponentType;

	ComponentManager mgrCom;

	//mgrCom.test();
	//std::cout << "Test!\n";

	// settings
	ComponentManager::ContainerSettings set;
	// set component size
	set.elementSize = sizeof(ComponentT);

	// create container // returns a container id
	ComponentManager::ContainerID container1 = mgrCom.createNewComponentType(set);

	int n = 0;

	for (int i = 0; i < 1000; ++i)
	{
		// init a new component
		ComponentT newComponent{ {1.11f, 2.22f, 3.33f, 4.44f}, i };

		// insert the component
		mgrCom.insertIntoContainer(container1, reinterpret_cast<char*>(&newComponent)); // can use templates ?

		++n;
	}

	std::cout << "Obj created :" << n << std::endl;

	// get component at index
	ComponentT* comT = reinterpret_cast<ComponentT*>((mgrCom.getElementAt(container1, 777)));
	std::cout << "Should read 777 :" << comT->id << std::endl;

	// del component at index
	mgrCom.removeFromContainer(container1, 777);
	std::cout << "mgrCom.removeFromContainer(container1, 777);" << std::endl;

	// get component at index
	comT = reinterpret_cast<ComponentT*>((mgrCom.getElementAt(container1, 777)));
	std::cout << "Should read 0, since removed :" << comT->id << std::endl;

	// this index includes empty slots

	// iteration
	auto itr = mgrCom.begin(container1);
	auto itrEnd = mgrCom.end(container1);
	int i = 0;
	while (itr != itrEnd)
	{
		comT = reinterpret_cast<ComponentT*>(*itr);
		std::cout << comT->id << " ";
		++itr;
		++i;
	}
	std::cout << std::endl;
	std::cout << "1000 - 1 removed = 999 left:" << i << std::endl;

	// remove with itr
	// iteration
	itr = mgrCom.begin(container1);
	itrEnd = mgrCom.end(container1);
	i = 0;
	while (itr != itrEnd)
	{
		comT = reinterpret_cast<ComponentT*>(*itr);
		std::cout << comT->id << " ";

		if (comT->id == 861)
		{
			std::cout << "\n Remove:" << comT->id << " \n";
			mgrCom.removeFromContainer(itr);
			itrEnd = mgrCom.end(container1); // need to update the end itr !!
			std::cout << "\n After Remove:" << comT->id << " \n";

			--i; // remove count
		}

		++itr;
		++i;
	}
	std::cout << std::endl;
	std::cout << "999 - 1 removed = 998 left:" << i << std::endl;



	// dun anyhow free
	mgrCom.freeAll();
}

void ComponentManager::test2()
{


	struct ComponentT
	{
		float vert[4];
		int id;
	};

	struct ComponentT1
	{
		int id0;
		float s[16];
		float r[16];
		float t[16];
		int id1;
	};

	ComponentManager mgrCom;

	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	// init the build
	comsetFac.StartBuild();

	// adds a component using component size
	ComponentManager::ContainerID containerT0 = comsetFac.AddComponent(sizeof(ComponentT)); // save the container id

	// adds a component using settings
	ComponentManager::ContainerSettings set;
	set.elementSize = sizeof(ComponentT1);
	ComponentManager::ContainerID containerT1 = comsetFac.AddComponent(set); // save the container id

	// builds the component
	ComponentManager::ComponentSet* compSet0 = comsetFac.Build();

	// component manager
	ComponentManager mgrComp;

	// adds the component set to the manager
	mgrComp.AddComponentSet(compSet0);

	// init component set manager
	ComponentManager::ComponentSetManager compSetMgr(compSet0);

	// build a new object
	int newObjId = compSetMgr.BuildObject();

	// attach component
	ComponentT newComponentT =
	{
		{1.11f, 2.22f, 3.33f, 4.44f},
		newObjId
	};
	void* newObjPtr = compSetMgr.AttachComponent(containerT0, newObjId, &newComponentT);

	// get component
	ComponentT* componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, newObjId));
	std::cout << "Test  get component:" << std::endl;
	std::cout << componentTest->id << std::endl;
	std::cout << componentTest->vert[0] << std::endl;
	std::cout << componentTest->vert[3] << std::endl;


	// remove component
	compSetMgr.RemoveComponent(containerT0, newObjId);

	// remove object
	compSetMgr.UnBuildObject(newObjId);

	// try get component after remove
	componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, newObjId));
	std::cout << "Test  try get component after remove:" << std::endl;
	std::cout << componentTest->id << std::endl;
	std::cout << componentTest->vert[0] << std::endl;
	std::cout << componentTest->vert[3] << std::endl;


	// insert stress test
	std::vector<int> objIds;
	for (int i = 0; i < 1000; ++i)
	{
		// new obj
		newObjId = compSetMgr.BuildObject();
		objIds.push_back(newObjId);

		// attach component
		ComponentT newComponentTN =
		{
			{1.11f, 2.22f, 3.33f, 4.44f},
			newObjId
		};
		compSetMgr.AttachComponent(containerT0, newObjId, &newComponentTN);
	}

	// get stress tess
	std::cout << "Test  get stress:" << std::endl;
	for (int id : objIds)
	{
		componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, id));
		std::cout << "Test :";
		std::cout << componentTest->id << " ";
		//std::cout << componentTest->vert[0] << " ";
		//std::cout << componentTest->vert[3] << std::endl;
	}

	// insert into containerT1
	std::cout << "\nTest  insert containerT1:" << std::endl;
	std::vector<int> obj2Ids;
	for (int i = 0; i < 1000; ++i)
	{
		// new obj
		newObjId = compSetMgr.BuildObject();
		obj2Ids.push_back(newObjId);

		// attach component
		ComponentT newComponentTN =
		{
			{1.11f, 2.22f, 3.33f, 4.44f},
			newObjId
		};
		compSetMgr.AttachComponent(containerT0, newObjId, &newComponentTN);

		// attach component
		ComponentT1 newComponentT1 =
		{
			newObjId,
			{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
			{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
			{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
			newObjId,
		};
		compSetMgr.AttachComponent(containerT1, newObjId, &newComponentT1);
	}

	// get from containerT1
	std::cout << "Test  get containerT1:" << std::endl;
	for (int id : obj2Ids)
	{
		ComponentT1* componentTest1 = reinterpret_cast<ComponentT1*>(compSetMgr.getComponent(containerT1, id));
		std::cout << "T1 :";
		std::cout << componentTest1->id0 << " ";
		std::cout << componentTest1->id1 << " ";

		componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, id));
		std::cout << "T0 :";
		std::cout << componentTest->id << " ";
	}


	// ITERATOR
	std::cout << "\n\nTest  ITERATOR containerT1:" << std::endl;
	ComponentManager::ComponentSetManager::Iterator itr = compSetMgr.begin(containerT1);
	auto itrEnd = compSetMgr.end(containerT1);

	while (itr != itrEnd)
	{
		ComponentT1* comTest1 = reinterpret_cast<ComponentT1*>(*itr);
		std::cout << "T1 :";
		std::cout << comTest1->id0 << " ";
		std::cout << comTest1->id1 << " ";

		++itr;
	}


	std::cout << "\n\nTest  ITERATOR containerT0:" << std::endl;
	itr = compSetMgr.begin(containerT0);
	itrEnd = compSetMgr.end(containerT0);

	int c = 0;

	while (itr != itrEnd)
	{
		ComponentT* comTest0 = reinterpret_cast<ComponentT*>(*itr);
		std::cout << "T0:";
		std::cout << comTest0->id << " ";

		std::cout << "ID:";
		std::cout << compSetMgr.getObjId(itr) << " ";

		//if (itr.getObjId() != -1)
		//{
		//	ComponentT1* comTest1 = reinterpret_cast<ComponentT1*>(compSetMgr.getComponent(containerT1, itr.getObjId()));
		//	std::cout << "T1 :";
		//	std::cout << comTest1->id0 << " ";
		//}
		//else
		//{
		//	std::cout << "T1 : - ";
		//}

		ComponentManager::ComponentSetManager::Entity obj = compSetMgr.getEntity(itr);

		ComponentT1* comTest1 = reinterpret_cast<ComponentT1*>(obj.getComponent(containerT1));
		if (comTest1 != nullptr)
		{
			std::cout << "T1:";
			std::cout << comTest1->id0 << " ";
		}
		else
		{
			std::cout << "T1:-";
		}


		if (c == 1)
		{
			int j = 0;
		}

		++c;
		++itr;
	}


	mgrCom.Free();
}


