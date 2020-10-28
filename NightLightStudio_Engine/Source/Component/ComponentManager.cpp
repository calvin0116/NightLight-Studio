
#include "ComponentManager.h"

#include <iostream>

#include <functional>
#include <string>


#include "..//Component/Components.h"
#include "..//Component/Components.h"
#include "..//Component/ComponentTransform.h"
#include "..//Component/ComponentRigidBody.h"
#include "..//Component/ComponentCollider.h"
#include "..//Component/ComponentGraphics.h"

#include "../Component/LocalVector.h"

#include "../Core/SceneManager.h"

namespace NS_COMPONENT
{



// local g var
static int G_CURRIDMOD = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
//// ComponentSetFactory

void ComponentManager::ComponentSetFactory::StartBuild()
{
	compSet = new ComponentSet();
}

ComponentManager::ContainerID ComponentManager::ComponentSetFactory::AddComponentContainer(ComponentMemoryManager::ComponentTypeSettings set)
{
	// create new container and adds id to list
	ContainerID conid = compSet->cmm.createNewComponentType(set);
	compSet->componentContainerIDs.push_back(conid);

	return conid;
}

ComponentManager::ContainerID ComponentManager::ComponentSetFactory::AddComponentContainer(size_t size)
{
	// sets only size, default settings
	ComponentMemoryManager::ComponentTypeSettings set;
	set.elementSize = size;
	return AddComponentContainer(set); // calls fn
}

ComponentManager::ComponentSet* ComponentManager::ComponentSetFactory::Build()
{
	// build obj
	size_t size = sizeof(ComponentManager::ComponentSet::ObjectData);
	size += sizeof(ComponentManager::ComponentSet::ObjectData::ComponentData) * compSet->hashConIdMap.size();

	// using default settings
	ComponentMemoryManager::ComponentTypeSettings set;
	set.elementSize = size;
	compSet->objSize = size;

	// save objContainerId // create obj container
	compSet->objContainerId = compSet->cmm.createNewComponentType(set);
	// container for childs
	compSet->objContainerIdChilds = compSet->cmm.createNewComponentType(set);

	// set
	G_CURRIDMOD += IDRANGE;
	compSet->idIndexModifier = G_CURRIDMOD;

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

ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::BuildEntity(std::string name)
{
	// new obj instance
	char* newObj = reinterpret_cast<char*>(malloc(compSet->objSize));

	if (newObj != nullptr)
		memset(newObj, -1, compSet->objSize); // init
	else
		throw; // err

	// insert the obj
	int objId = compSet->cmm.insertIntoContainer(compSet->objContainerId, newObj);

	// check
	if (objId > IDRANGE_RT)
	{
		throw; // cannot, objects exceeded
	}

	// free the obj instance
	free(reinterpret_cast<void*>(newObj));
	// get the obj from the container
	newObj = compSet->cmm.getElementAt(compSet->objContainerId, objId);

	// set the obj
	//reinterpret_cast<ComponentManager::ComponentSet::ObjectData*>(newObj)->objInd = objId;
	ComponentManager::ComponentSet::ObjectData* newObj_o = reinterpret_cast<ComponentManager::ComponentSet::ObjectData*>(newObj);
	newObj_o->objId = objId;
	//newObj_o->parentObjId = -1; //it already is -1
	newObj_o->children.numChild = 0;
	newObj_o->children.numDecendants = 0;
	newObj_o->children.childIDs.init();
	newObj_o->children.generation = 0;

	//===Added by Teck Wei for name getting====//
	static int unknown_ent_id = 0;
	if (name == "Entity_")
	{
		name.append(std::to_string(unknown_ent_id));
		++unknown_ent_id;
	}
	NS_SCENE::SYS_SCENE_MANAGER->EntityName[objId + compSet->idIndexModifier] = name;
	//----------------------------------------//
	//return objId + compSet->idIndexModifier;

	return (EntityHandle(this, objId + compSet->idIndexModifier));
}


int ComponentManager::ComponentSetManager::BuildChildObject()
{
	// new obj instance
	char* newObj = reinterpret_cast<char*>(malloc(compSet->objSize));

	if (newObj != nullptr)
		memset(newObj, -1, compSet->objSize); // init
	else
		throw; // err

	// insert the obj
	int objId = compSet->cmm.insertIntoContainer(compSet->objContainerIdChilds, newObj);

	// check
	if (objId > IDRANGE_CH)
	{
		throw; // cannot, objects exceeded
	}

	// free the obj instance
	free(reinterpret_cast<void*>(newObj));

	// get the obj from the container
	newObj = compSet->cmm.getElementAt(compSet->objContainerIdChilds, objId);

	// set the obj
	//reinterpret_cast<ComponentManager::ComponentSet::ObjectData*>(newObj)->objInd = objId;
	ComponentManager::ComponentSet::ObjectData* newObj_o = reinterpret_cast<ComponentManager::ComponentSet::ObjectData*>(newObj);
	newObj_o->objId = objId;
	//newObj_o->parentObjId = -1; //it already is -1
	newObj_o->children.numChild = 0;
	newObj_o->children.numDecendants = 0;
	newObj_o->children.childIDs.init();

	return objId + compSet->idIndexModifier + IDRANGE_RT;

	//return objId;
}

void* ComponentManager::ComponentSetManager::AttachComponent(ComponentManager::ContainerID compId, int objId, const void* newComp)
{
	// insert the component
	int newCId = compSet->cmm.insertIntoContainer(compId, reinterpret_cast<const char*>(newComp));

	// set object component data

	ComponentSet::ObjectData::ComponentData* compData = nullptr;

	objId -= compSet->idIndexModifier; // // 
	if (objId >= IDRANGE_RT)
	{
		objId -= IDRANGE_RT;
		compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId, true));
	}
	else
	{
		compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId));
	}
	if (compData == nullptr) throw;

	// !
	compData->containerId = compId; // set container id
	compData->compPtr = reinterpret_cast<void*>( // set component ptr within the container
		compSet->cmm.getElementAt(compId, newCId)
		);;
	compData->containerIndex = newCId; // set component index within the container


	// return the component
	return compData->compPtr;
}

void ComponentManager::ComponentSetManager::RemoveComponent(ComponentManager::ContainerID compId, int objId)
{
	// get the index
	objId -= compSet->idIndexModifier; // // 

	ComponentSet::ObjectData::ComponentData* compData = nullptr;

	if (objId >= IDRANGE_RT)
	{
		objId -= IDRANGE_RT;
		compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId, true));
	}
	else
	{
		compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId));
	}
	if (compData == nullptr) throw;

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

	//int deletingObjId = objId;
	objId -= compSet->idIndexModifier;

	char* obj = nullptr;

	bool isChild = false;

	if (objId >= IDRANGE_RT)
	{
		isChild = true;
		// child

		objId -= IDRANGE_RT;
		obj = compSet->cmm.getElementAt(compSet->objContainerIdChilds, objId);

		// uninit/free the components
		for (auto p : compSet->hashConIdMapChilds)
		{
			int currentId = p.second;

			// probably a better way to do this !!

			ComponentSet::ObjectData::ComponentData* compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(currentId, objId, true));

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
	}
	else
	{
		// root

		obj = compSet->cmm.getElementAt(compSet->objContainerId, objId);

		// uninit/free the components
		for (auto p : compSet->hashConIdMap)
		{
			int currentId = p.second;

			// probably a better way to do this !!

			ComponentSet::ObjectData::ComponentData* compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(currentId, objId));

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
	}
	if (obj == nullptr) throw;

	ComponentSet::ObjectData* objData = reinterpret_cast<ComponentSet::ObjectData*>(obj);

	// uninit the obj

	objData->children.childIDs.clear(); 
	objData->children.generation = -1;
	objData->children.numChild = -1;




	// set the parent
	//ComponentSet::ObjectData* objRootParent = nullptr;
	//if (objData->parentObjId != -1)
	//{
	//	int parentObjId = objData->parentObjId - compSet->idIndexModifier;
	//	if (parentObjId >= IDRANGE_RT)
	//	{
	//		// child

	//		parentObjId -= IDRANGE_RT;

	//		objRootParent = reinterpret_cast<ComponentSet::ObjectData*>(
	//			compSet->cmm.getElementAt(compSet->objContainerIdChilds, parentObjId)
	//		);
	//	}
	//	else
	//	{
	//		//root
	//		objRootParent = reinterpret_cast<ComponentSet::ObjectData*>(
	//			compSet->cmm.getElementAt(compSet->objContainerId, parentObjId)
	//			);
	//	}
	//	if (objRootParent == nullptr) throw;

	//	--objRootParent->children.numChild;

	//	// cant do this here
	//	//auto itr = objRootParent->children.childIDs.begin();
	//	//auto itrEnd = objRootParent->children.childIDs.end();
	//	//while (itr != itrEnd)
	//	//{
	//	//	if (*itr == deletingObjId)
	//	//	{
	//	//		objRootParent->children.childIDs.erase(itr);
	//	//		break;
	//	//	}
	//	//	++itr;
	//	//}


	//	//objRootParent->children.numDecendants; // TODO !!!

	//}

	objData->children.numDecendants = -1;
	objData->parentObjId = -1;
	objData->objId = -1;


	// remove the object
	if (isChild)
	{
		compSet->cmm.removeFromContainer(compSet->objContainerIdChilds, objId);
	}
	else
	{
		compSet->cmm.removeFromContainer(compSet->objContainerId, objId);
	}
}

void ComponentManager::ComponentSetManager::FreeEntity(int objId)
{
	std::function<void(int)> delObj = [&](int thisid)
	{
		int getIndex = thisid;
		// get the index
		getIndex -= compSet->idIndexModifier; // // 

		ComponentSet::ObjectData* objData = nullptr;

		if (getIndex >= IDRANGE_RT)
		{
			// child

			getIndex -= IDRANGE_RT;
			objData = reinterpret_cast<ComponentSet::ObjectData*>(
				compSet->cmm.getElementAt(compSet->objContainerIdChilds, getIndex)
				);
		}
		else
		{
			// root

			objData = reinterpret_cast<ComponentSet::ObjectData*>(
				compSet->cmm.getElementAt(compSet->objContainerId, getIndex)
				);
		}
		if (objData == nullptr) throw;
		// get obj Data

		for (int childId : objData->children.childIDs)
		{
			delObj(childId);
		}
		UnBuildObject(thisid); // delete childs first then del this
	};


	int deletingObjId = objId;
	objId -= compSet->idIndexModifier;
	char* obj = nullptr;
	if (objId >= IDRANGE_RT)
	{
		// child

		objId -= IDRANGE_RT;
		obj = compSet->cmm.getElementAt(compSet->objContainerIdChilds, objId);

	}
	else
	{
		// root

		obj = compSet->cmm.getElementAt(compSet->objContainerId, objId);
	}
	if (obj == nullptr) throw;
	ComponentSet::ObjectData* objData = reinterpret_cast<ComponentSet::ObjectData*>(obj);
	int parentObjId = objData->parentObjId;
	// need to get the parent obj

	//ComponentSet::ObjectData* t = reinterpret_cast<ComponentSet::ObjectData*>(
	//	compSet->cmm.getElementAt(compSet->objContainerId, parentObjId - compSet->idIndexModifier)
	//	);

	delObj(deletingObjId);

	// delete child entry in parent !!
	ComponentSet::ObjectData* objRootParent = nullptr;
	if (parentObjId != -1)
	{
		parentObjId = parentObjId - compSet->idIndexModifier;
		if (parentObjId >= IDRANGE_RT)
		{
			// child

			parentObjId -= IDRANGE_RT;

			objRootParent = reinterpret_cast<ComponentSet::ObjectData*>(
				compSet->cmm.getElementAt(compSet->objContainerIdChilds, parentObjId)
				);
		}
		else
		{
			//root
			objRootParent = reinterpret_cast<ComponentSet::ObjectData*>(
				compSet->cmm.getElementAt(compSet->objContainerId, parentObjId)
				);
		}
		if (objRootParent == nullptr) throw;

		auto itr = objRootParent->children.childIDs.begin();
		auto itrend = objRootParent->children.childIDs.end();
		while (itr != itrend)
		{
			if (*itr == deletingObjId)
			{
				objRootParent->children.childIDs.erase(itr);
				break;
			}
			++itr;
		}

		--objRootParent->children.numChild;

		//	//objRootParent->children.numDecendants; // TODO !!!
	}
}

void* ComponentManager::ComponentSetManager::getComponent(ComponentManager::ContainerID compId, int objId)
{	
	// get the index
	objId -= compSet->idIndexModifier;

	ComponentSet::ObjectData::ComponentData* compData;// = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId));
	//int index = compData->containerIndex;

	//
	if (objId >= IDRANGE_RT)
	{
		objId -= IDRANGE_RT;
		compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId, true));
	}
	else
	{
		compData = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(getObjectComponent(compId, objId));
	}

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

char* ComponentManager::ComponentSetManager::getObjectComponent(ComponentManager::ContainerID compId, int objId, bool isChild)
{
	char* obj;
	if (isChild)
	{
		obj = compSet->cmm.getElementAt(compSet->objContainerIdChilds, objId);
	}
	else
	{
		obj = compSet->cmm.getElementAt(compSet->objContainerId, objId);
	}

	// obj building
	//size_t size = sizeof(ManagerComponent::ComponentSet::Object);
	//size += sizeof(ManagerComponent::ComponentSet::Object::Component) * compSet->componentContainerIDs.size();

	obj += sizeof(ComponentManager::ComponentSet::ObjectData);

	// find the container id
	int n = 0;
	for (int currentId : compSet->componentContainerIDs)
	{
		if (currentId == compId)
			break;
		++n;
	}

	// get the position of the component data
	obj += sizeof(ComponentManager::ComponentSet::ObjectData::ComponentData) * (n/2);

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


ComponentManager::ComponentSetManager::Iterator ComponentManager::ComponentSetManager::begin(ContainerID comT, ContainerID comTC)
{
	// comp itr
	ComponentSetManager::Iterator newItr;

	if (comTC == -1)
	{
		newItr.itrDoState = Iterator::IteratorState::ITR_ROOT;
		newItr.itrCurrState = Iterator::IteratorState::ITR_ROOT;
	}
	else
	{
		newItr.memItrChild = compSet->cmm.begin(comTC);
	}
	if (comT == -1)
	{
		newItr.itrDoState = Iterator::IteratorState::ITR_CHILD;
		newItr.itrCurrState = Iterator::IteratorState::ITR_CHILD;
	}
	else
	{
		newItr.memItr = compSet->cmm.begin(comT);
	}
	if (comTC != -1 && comT != -1)
	{
		newItr.itrDoState = Iterator::IteratorState::ITR_BOTH;
		newItr.itrCurrState = Iterator::IteratorState::ITR_ROOT;
		newItr.endRtObjIndex = compSet->cmm.end(comT).getCurrentObjIndex();
	}

	newItr.compSetMgr = this;
	newItr.containerId = comT;
	newItr.containerIdChild = comTC;
	
	return newItr;
}

ComponentManager::ComponentSetManager::Iterator ComponentManager::ComponentSetManager::end(ContainerID comT, ContainerID comTC)
{
	// comp itr
	ComponentSetManager::Iterator newItr;

	if (comTC == -1)
	{
		newItr.itrDoState = Iterator::IteratorState::ITR_ROOT;
		newItr.itrCurrState = Iterator::IteratorState::ITR_ROOT;
	}
	else
	{
		newItr.memItrChild = compSet->cmm.end(comTC);
	}
	if (comT == -1)
	{
		newItr.itrDoState = Iterator::IteratorState::ITR_CHILD;
		newItr.itrCurrState = Iterator::IteratorState::ITR_CHILD;
	}
	else
	{
		newItr.memItr = compSet->cmm.end(comT);
	}
	if (comTC != -1 && comT != -1)
	{
		newItr.itrDoState = Iterator::IteratorState::ITR_BOTH;
		newItr.itrCurrState = Iterator::IteratorState::ITR_CHILD;
		newItr.endRtObjIndex = compSet->cmm.end(comT).getCurrentObjIndex();
	}

	newItr.compSetMgr = this;
	newItr.containerId = comT;
	newItr.containerIdChild = comTC;

	return newItr;
}

void* ComponentManager::ComponentSetManager::getComponent(ComponentManager::ContainerID compId, Iterator itr)
{
	if (itr.compSetMgr != this) // check if its the same mgr
		throw;

	ComponentManager::ComponentSetManager::EntityHandle obj = getEntity(itr);
	return obj.getComponent(compId);
}

int ComponentManager::ComponentSetManager::getObjId(Iterator itr)
{
	int containerId = itr.itrCurrState == Iterator::IteratorState::ITR_ROOT ? itr.containerId : itr.containerIdChild;
	int objContainerId = itr.itrCurrState == Iterator::IteratorState::ITR_ROOT ? compSet->objContainerId : compSet->objContainerIdChilds;

	int objId = -1;


	//compSet->cmm.getElementAt(compSet->objContainerId, 0);
	auto itrOb = compSet->cmm.begin(objContainerId);
	auto itrObEnd = compSet->cmm.end(objContainerId);
	while (itrOb != itrObEnd)
	{
		char* obj = (*itrOb);

		ComponentSet::ObjectData* obj_o = reinterpret_cast<ComponentSet::ObjectData*>(obj);
		

		obj += sizeof(ComponentManager::ComponentSet::ObjectData);

		// find the container id
		int n = 0;
		for (int currentId : compSet->componentContainerIDs)
		{
			if (currentId == containerId)
				break;
			++n;
		}

		// get the position of the component data
		obj += sizeof(ComponentManager::ComponentSet::ObjectData::ComponentData) * (n/2);

		//check
		ComponentSet::ObjectData::ComponentData* comp = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(obj);
		if (itr.itrCurrState == Iterator::IteratorState::ITR_ROOT)
		{
			if (comp->containerIndex == itr.memItr.getCurrentIndex())
			//if (comp->containerId == itr.containerId) //memItr.getCurrentIndex())
			{
				objId = obj_o->objId;
				break;
			}
		}
		else
		{
			if (comp->containerIndex == itr.memItrChild.getCurrentIndex())
			{
				objId = obj_o->objId;
				break;
			}
		}

		++itrOb;
	}

	if (objId == -1) throw;

	//return objId + compSet->idIndexModifier;
	return itr.itrCurrState == Iterator::IteratorState::ITR_ROOT ? objId + compSet->idIndexModifier : objId + compSet->idIndexModifier + IDRANGE_RT;
}

//////////////////////////
//// Entity
////			ComponentManager::ComponentSetManager::Entity
//////////////////////////

ComponentManager::ComponentSetManager::EntityHandle::EntityHandle(ComponentSetManager* csm, int oid) : compSetMgr(csm), objId(oid)
{
}

void* ComponentManager::ComponentSetManager::EntityHandle::getComponent(int compId)
{
	return compSetMgr->getComponent(compId, objId);
}

char* ComponentManager::ComponentSetManager::EntityHandle::getObj()
{
	// get the index first then get the container id then get the obj data

	int objIndex = objId - compSetMgr->compSet->idIndexModifier;

	if (objIndex >= IDRANGE_RT)
	{
		objIndex -= IDRANGE_RT;
		char* obj = compSetMgr->compSet->cmm.getElementAt(compSetMgr->compSet->objContainerIdChilds, objIndex);
		return obj;
	}
	else
	{
		char* obj = compSetMgr->compSet->cmm.getElementAt(compSetMgr->compSet->objContainerId, objIndex);
		return obj;
	}
	// getElementAt is technically O(1), 
	// though there is a search for container and its metadata, 
	// but as long as the number of containers is small there shldnt be a huge impact
	// number of containers is proportional to number of types of components
}

int ComponentManager::ComponentSetManager::EntityHandle::getId()
{
	return objId;
}

int ComponentManager::ComponentSetManager::EntityHandle::getGeneration()
{
	ComponentSet::ObjectData* obj = reinterpret_cast<ComponentSet::ObjectData*>(getObj());
	return obj->children.generation;
}

int ComponentManager::ComponentSetManager::EntityHandle::getNumChildren()
{
	ComponentSet::ObjectData* obj = reinterpret_cast<ComponentSet::ObjectData*>(getObj());
	return obj->children.numChild;
}

int ComponentManager::ComponentSetManager::EntityHandle::getNumDecendants()
{
	ComponentSet::ObjectData* obj = reinterpret_cast<ComponentSet::ObjectData*>(getObj());
	return obj->children.numDecendants;
}

ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::EntityHandle::makeChild()
{

	ComponentSet::ObjectData* objParent = reinterpret_cast<ComponentSet::ObjectData*>(getObj());

	///////////////// !!!
	int childUid = compSetMgr->BuildChildObject();

	//ComponentSet::ObjectData* objChild = reinterpret_cast<ComponentSet::ObjectData*>(getObj());
	ComponentManager::ComponentSetManager::EntityHandle entChi = compSetMgr->getEntity(childUid);
	ComponentSet::ObjectData* objChild = reinterpret_cast<ComponentSet::ObjectData*>(
		entChi.getObj()
	);

	objChild->children.generation = 1 + objParent->children.generation;

	objParent->children.childIDs.push_back(childUid);


	//++(objParent->children.numDecendants); // need to recursively update !!

	++(objParent->children.numChild);

	if (objParent->children.generation > 0)
	{
		// parent is not root
		objChild->parentObjId = objParent->objId + compSetMgr->compSet->idIndexModifier + IDRANGE_RT;
	}
	else
	{
		// parent is root
		objChild->parentObjId = objParent->objId + compSetMgr->compSet->idIndexModifier;
	}



	return EntityHandle(compSetMgr, childUid);
}

int ComponentManager::ComponentSetManager::EntityHandle::getParentId()
{
	ComponentSet::ObjectData* obj = reinterpret_cast<ComponentSet::ObjectData*>(getObj());
	return obj->parentObjId;
}

ComponentManager::ChildContainerT* ComponentManager::ComponentSetManager::EntityHandle::getChildren()
{
	ComponentSet::ObjectData* objParent = reinterpret_cast<ComponentSet::ObjectData*>(getObj());

	return &(objParent->children.childIDs);
}


//// Entity END
//////////////////////////

ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::getEntity(Iterator itr)
{
	EntityHandle obj(this, this->getObjId(itr));
	//obj.compSetMgr = this;
	//obj.objId = this->getObjId(itr);

	return obj;
}

ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::getEntity(int uid)
{
	// need to chk if uid valid // within range
	EntityHandle obj(this, uid);
	return obj;
}


//// ComponentSetManager END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// ComponentManager::ComponentSetManager::Iterator


bool ComponentManager::ComponentSetManager::Iterator::operator==(ComponentManager::ComponentSetManager::Iterator& itr)
{
	switch (itrDoState)
	{
	case IteratorState::ITR_ROOT:
		return (memItr == itr.memItr);
		break;

	case IteratorState::ITR_CHILD:
		return (memItrChild == itr.memItrChild);
		break;

	case IteratorState::ITR_BOTH:
		if (itrCurrState == IteratorState::ITR_ROOT)
		{
			return false;
		}
		else if (itrCurrState == IteratorState::ITR_CHILD)
		{
			return (memItrChild == itr.memItrChild);
		}
		else
			throw;
		break;
	}

	throw;
}

bool ComponentManager::ComponentSetManager::Iterator::operator!=(ComponentManager::ComponentSetManager::Iterator& itr)
{
	switch (itrDoState)
	{
	case IteratorState::ITR_ROOT:
		return (memItr != itr.memItr);
		break;

	case IteratorState::ITR_CHILD:
		return (memItrChild != itr.memItrChild);
		break;

	case IteratorState::ITR_BOTH:
		if (itrCurrState == IteratorState::ITR_ROOT)
		{
			return true;
		}
		else if (itrCurrState == IteratorState::ITR_CHILD)
		{
			return (memItrChild != itr.memItrChild);
		}
		else
			throw;
		break;
	}

	throw;
}

void* ComponentManager::ComponentSetManager::Iterator::operator*()
{
	switch (itrDoState)
	{
	case IteratorState::ITR_ROOT:
		return reinterpret_cast<void*>(*memItr);
		break;

	case IteratorState::ITR_CHILD:
		return reinterpret_cast<void*>(*memItrChild);
		break;

	case IteratorState::ITR_BOTH:
		if (itrCurrState == IteratorState::ITR_ROOT)
		{
			return reinterpret_cast<void*>(*memItr);
		}
		else if (itrCurrState == IteratorState::ITR_CHILD)
		{
			return reinterpret_cast<void*>(*memItrChild);
		}
		else
			throw;
		break;
	}

	throw;

}

ComponentManager::ComponentSetManager::Iterator& ComponentManager::ComponentSetManager::Iterator::operator++()
{
	switch (itrDoState)
	{
	case IteratorState::ITR_ROOT:
		++memItr;
		return *this;
		break;

	case IteratorState::ITR_CHILD:
		++memItrChild;
		return *this;
		break;

	case IteratorState::ITR_BOTH:
		if (itrCurrState == IteratorState::ITR_ROOT)
		{
			++memItr;
			if (endRtObjIndex == memItr.getCurrentObjIndex())
			{
				itrCurrState = IteratorState::ITR_CHILD;
			}
			return *this;
		}
		else if (itrCurrState == IteratorState::ITR_CHILD)
		{
			++memItrChild;
			return *this;
		}
		else
			throw;
		break;
	}

	throw;
}

ComponentManager::ComponentSetManager::Iterator ComponentManager::ComponentSetManager::Iterator::operator++(int)
{
	switch (itrDoState)
	{
	case IteratorState::ITR_ROOT:
	{
		ComponentManager::ComponentSetManager::Iterator newItr = *this; // post
		++memItr;
		return newItr;
		break;
	}

	case IteratorState::ITR_CHILD:
	{
		ComponentManager::ComponentSetManager::Iterator newItr = *this; // post
		++memItrChild;
		return newItr;
		break;
	}

	case IteratorState::ITR_BOTH:
		ComponentManager::ComponentSetManager::Iterator newItr = *this; // post
		if (itrCurrState == IteratorState::ITR_ROOT)
		{
			++memItr;
			if (endRtObjIndex == memItr.getCurrentObjIndex())
			{
				itrCurrState = IteratorState::ITR_CHILD;
			}
			return newItr;
		}
		else if (itrCurrState == IteratorState::ITR_CHILD)
		{
			++memItrChild;
			return newItr;
		}
		else
			throw;
		break;
	}

	throw;
}

//// ComponentManager::ComponentSet::Iterator END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////

void ComponentManager::AddComponentSet(COMPONENTSETNAMES idname, ComponentSet* compSet)
{
	// define new component type
	ContainerID newComponentSetID = (int)ComponentSets.size();
	while (ComponentSets.find(newComponentSetID) != ComponentSets.end())
	{
		// find a free id
		newComponentSetID++;
	} // loop ends when id is not found -> unique id

	ComponentSets.insert(std::pair<ContainerID, ComponentSet*>(newComponentSetID, compSet));

	// can try using a vector // dun need this to be sorted
	//ComponentSetManagers.try_emplace(idname, ComponentSetManager(compSet)); // idk this doesnt work?

	//ComponentSetManagers.insert(std::pair<COMPONENTSETNAMES, ComponentSet *>((COMPONENTSETNAMES)newComponentSetID, compSet));
	//ComponentSetManagers[idname].compSet = compSet;

	ComponentSetManagers.try_emplace(idname, ComponentSetManager(compSet)); // idk this doesnt work?
	ComponentSetManagers[idname].compSet = compSet;
}

ComponentManager::ComponentSetManager* ComponentManager::getComponentSetMgr(COMPONENTSETNAMES idname)
{	
	ComponentSetManager* csm_p = &(ComponentSetManagers[idname]);
	return csm_p;
}

void ComponentManager::ComponentCreation()
{


	// factory
	ComponentManager::ComponentSetFactory comsetFac;

	auto build = [&](COMPONENTSETNAMES id)
	{
		// Building another component set
		comsetFac.StartBuild();

		comsetFac.AddComponentContainer<ComponentTransform>();
		comsetFac.AddComponentContainer<ComponentTest0>();
		comsetFac.AddComponentContainer<ComponentCollider>();
		comsetFac.AddComponentContainer<ComponentRigidBody>();
		comsetFac.AddComponentContainer<ComponentGraphics>();
		//comsetFac.AddComponentContainer<ComponentInput>();
		//comsetFac.AddComponentContainer<ComponentLogic>();
		//comsetFac.AddComponentContainer<ComponentCamera>();
		//comsetFac.AddComponentContainer<ComponentLight>();


		// builds the component set
		ComponentManager::ComponentSet* cs = comsetFac.Build();
		// adds the component set to the component manager
		SYS_COMPONENT->AddComponentSet(id, cs);

		return cs;
	};

	// init component set manager

	build(COMPONENT_MAIN);
	build(COMPONENT_UI);
	build(COMPONENT_PREFABS);

	//SYS_COMPONENT->getComponentSetMgr(COMPONENT_MAIN);
	//SYS_COMPONENT->getComponentSetMgr(COMPONENT_UI);
	//G_MAINCOMPSET = build();
	//G_UICOMPSET = build();
}

void ComponentManager::TestComponents()
{

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// BUILD COMPONENT SAMPLE
	{

		// WHILE OBJECTS
		// Start of creation and Entity
		Entity newEntity = G_ECMANAGER->BuildEntity();

		// WHILE COMPONENTS
		// Creation
		ComponentTest0 newCompComponentTest0
		{
			0,
			"Hello World",
			{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f}
		};
		G_ECMANAGER->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);
		// WHILE COMPONENTS END
		// WHILE OBJECTS END

		// add 2nd obj 
		newEntity = G_ECMANAGER->BuildEntity();

		ComponentTransform compT;
		compT._position.x = 1.11f;

		G_ECMANAGER->AttachComponent<ComponentTransform>(newEntity, compT);

		ComponentRigidBody compRB;
		G_ECMANAGER->AttachComponent<ComponentRigidBody>(newEntity, compRB);

		newCompComponentTest0.id = 1;
		G_ECMANAGER->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

		// add 3rd obj 
		newEntity = G_ECMANAGER->BuildEntity();
		G_ECMANAGER->AttachComponent<ComponentRigidBody>(newEntity, compRB);


		// add obj to another component set
		newCompComponentTest0.id = 999;
		newEntity = G_ECMANAGER_UI->BuildEntity();
		G_ECMANAGER_UI->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

		//// add to another compset
		newEntity = G_ECMANAGER_UI->BuildEntity();
		newCompComponentTest0.id = 123;
		G_ECMANAGER_UI->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

		///////////////////////////////
		// hierarchical entity test
		{
			std::cout << std::endl;
			std::cout << "/////////////////////////////////////" << std::endl;
			std::cout << "// hierarchical entity test - LOAD" << std::endl;
			std::cout << std::endl;

			Entity entity = G_ECMANAGER_UI->getEntity(newEntity.getId());


			int numChild = entity.getNumChildren();
			int numDec = entity.getNumDecendants();
			int parentuid = entity.getParentId();

			std::cout << "numChild:" << numChild << std::endl;
			std::cout << "numDec:" << numDec << std::endl;
			std::cout << "parentuid:" << parentuid << std::endl;

			Entity childEntity = entity.makeChild();
			ComponentManager::ChildContainerT* childrens = entity.getChildren();

			std::cout << std::endl;

			int numChild_child = childEntity.getNumChildren();
			int numDec_child = childEntity.getNumDecendants();
			int parentuid_child = childEntity.getParentId();

			std::cout << "numChild_child:" << childEntity.getId() << std::endl;
			std::cout << "numChild_child:" << numChild_child << std::endl;
			std::cout << "numDec_child:" << numDec_child << std::endl;
			std::cout << "parentuid_child:" << parentuid_child << std::endl;

			newCompComponentTest0.id = 54321;
			G_ECMANAGER_UI->AttachComponent<ComponentTest0>(childEntity, newCompComponentTest0);

			std::cout << std::endl;
			std::cout << "// hierarchical entity test - LOAD  END" << std::endl;
			std::cout << "////////////////////////////////////" << std::endl;
			std::cout << std::endl;
		}
		{
			// make 2 more childs from the entity
			Entity entity = G_ECMANAGER_UI->getEntity(newEntity.getId());
			//entity.AttachComponent<ComponentTest0>();

			// child1
			Entity childEntity1 = entity.makeChild();
			newCompComponentTest0.id = 5432;
			G_ECMANAGER_UI->AttachComponent<ComponentTest0>(childEntity1, newCompComponentTest0);


			// make child of child - grandchild
			Entity grandChildEntity0 = childEntity1.makeChild();
			newCompComponentTest0.id = 777;
			G_ECMANAGER_UI->AttachComponent<ComponentTest0>(grandChildEntity0, newCompComponentTest0);


			// child2
			Entity childEntity2 = entity.makeChild();
			newCompComponentTest0.id = 543;
			G_ECMANAGER_UI->AttachComponent<ComponentTest0>(childEntity2, newCompComponentTest0);
		}
		{
			// adding more test objects

			newEntity = G_ECMANAGER_UI->BuildEntity();
			newCompComponentTest0.id = 1234;
			G_ECMANAGER_UI->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);

			compT._position.x = 2.22f;
			G_ECMANAGER_UI->AttachComponent<ComponentTransform>(newEntity, compT);

			{
				// adding childs
				Entity entity = G_ECMANAGER_UI->getEntity(newEntity.getId());

				// child1
				Entity childEntity1 = entity.makeChild();
				newCompComponentTest0.id = 7771;
				G_ECMANAGER_UI->AttachComponent<ComponentTest0>(childEntity1, newCompComponentTest0);


				// make child of child - grandchild
				Entity grandChildEntity0 = childEntity1.makeChild();
				newCompComponentTest0.id = 8881;
				G_ECMANAGER_UI->AttachComponent<ComponentTest0>(grandChildEntity0, newCompComponentTest0);

				// make 2nd grandchild
				Entity grandChildEntity1 = childEntity1.makeChild();
				newCompComponentTest0.id = 8882;
				G_ECMANAGER_UI->AttachComponent<ComponentTest0>(grandChildEntity1, newCompComponentTest0);

				compT._position.x = 3.33f;
				G_ECMANAGER_UI->AttachComponent<ComponentTransform>(grandChildEntity1, compT);

				// make 3rd grandchild
				Entity grandChildEntity2 = childEntity1.makeChild();
				newCompComponentTest0.id = 8883;
				G_ECMANAGER_UI->AttachComponent<ComponentTest0>(grandChildEntity2, newCompComponentTest0);

				// child2
				Entity childEntity2 = entity.makeChild();
				newCompComponentTest0.id = 7772;
				G_ECMANAGER_UI->AttachComponent<ComponentTest0>(childEntity2, newCompComponentTest0);


			}

			newEntity = G_ECMANAGER_UI->BuildEntity();
			newCompComponentTest0.id = 1235;
			G_ECMANAGER_UI->AttachComponent<ComponentTest0>(newEntity, newCompComponentTest0);
		}
		// hierarchical entity test END
		///////////////////////////////
	}
	//// BUILD COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////////////////////
	//// GET COMPONENT SAMPLE
	//{
	//	std::cout << std::endl;
	//	std::cout << "////////////////////////////////////" << std::endl;
	//	std::cout << "// Component Sample" << std::endl;
	//	std::cout << std::endl;



	//	std::cout << std::endl;
	//	std::cout << "// Component Sample END" << std::endl;
	//	std::cout << "////////////////////////////////////" << std::endl;
	//	std::cout << std::endl;
	//}
	{
		std::cout << std::endl;
		std::cout << "////////////////////////////////////" << std::endl;
		std::cout << "// Test Components:" << std::endl;
		std::cout << std::endl;

		std::cout << std::endl;
		std::cout << "// Test Iterator:" << std::endl;
		std::cout << std::endl;

		//

		// G_UICOMPSET
		// ComponentIteratorState::ITR_BOTH   // Iterate both root and child entities // this will iterate root first then child // child is not sorted by generation, not yet anyway
		// ComponentIteratorState::ITR_ROOT   // Iterate both root only
		// ComponentIteratorState::ITR_CHILD  // Iterate both child only

		auto testItr = [&](ComponentIteratorState st)
		{
			auto itrT = G_ECMANAGER_UI->begin<ComponentTest0>(st);
			auto itrTEnd = G_ECMANAGER_UI->end<ComponentTest0>(st);
			while (itrT != itrTEnd)
			{
				// get the obj id
				std::cout << "Object:" << G_ECMANAGER_UI->getObjId(itrT) << std::endl;

				// get the transform component from the iterator
				ComponentTest0* compR = G_ECMANAGER_UI->getComponent<ComponentTest0>(itrT);
				std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

				// get another component
				ComponentTransform* compT = G_ECMANAGER_UI->getComponent<ComponentTransform>(itrT);

				if (compT != nullptr) // nullptr -> uninitialised or deleted
					std::cout << "Transform:" << compT->_position.x << std::endl;

				// get entity
				Entity entity = G_ECMANAGER_UI->getEntity(itrT);

				std::cout << "ID: " << entity.getId() << std::endl;
				std::cout << "Parent ID: " << entity.getParentId() << std::endl;
				std::cout << "Generation: " << entity.getGeneration() << std::endl;

				// can get component from entity too
				ComponentTransform* compT1 = entity.getComponent<ComponentTransform>();
				if (compT != compT1) throw;

				std::cout << std::endl;

				++itrT;
			}
		};
		testItr(ComponentIteratorState::ITR_BOTH);
		//testItr(ComponentIteratorState::ITR_ROOT);
		//testItr(ComponentIteratorState::ITR_CHILD);


		std::cout << std::endl;
		std::cout << "// Test Iterator END" << std::endl;
		std::cout << std::endl;


		std::cout << std::endl;
		std::cout << "// Test Get Components:" << std::endl;
		std::cout << std::endl;

		// G_MAINCOMPSET
		auto itr = G_ECMANAGER->begin<ComponentTest0>();
		auto itrEnd = G_ECMANAGER->end<ComponentTest0>();
		while (itr != itrEnd)
		{
			// get the obj id
			std::cout << "Object:" << G_ECMANAGER->getObjId(itr) << std::endl;

			// get the transform component from the iterator
			ComponentTest0* compR = G_ECMANAGER->getComponent<ComponentTest0>(itr);
			std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

			// get another component
			ComponentTransform* compT = G_ECMANAGER->getComponent<ComponentTransform>(itr);

			if (compT != nullptr) // nullptr -> uninitialised or deleted
				std::cout << "Transform:" << compT->_position.x << std::endl;

			std::cout << std::endl;

			++itr;
		}

		//
		int toDel0 = -1;
		int toDel1 = -1;
		int toDel2 = -1;

		auto print = [&]()
		{
			// G_UICOMPSET
			itr = G_ECMANAGER_UI->begin<ComponentTest0>();
			itrEnd = G_ECMANAGER_UI->end<ComponentTest0>();
			while (itr != itrEnd)
			{
				// get the obj id
				std::cout << std::endl;
				std::cout << "Object:" << G_ECMANAGER_UI->getObjId(itr) << std::endl;

				// get the transform component from the iterator
				ComponentTest0* compR = reinterpret_cast<ComponentTest0*>(*itr);
				std::cout << "ComponentTest0:" << compR->id << " " << compR->c << std::endl;

				// get the entity from the iterator
				Entity entity = G_ECMANAGER_UI->getEntity(itr);

				// get transform component
				ComponentTransform* compT = entity.getComponent<ComponentTransform>();
				if (compT != nullptr) // nullptr -> uninitialised or deleted
					std::cout << "Transform:" << compT->_position.x << std::endl;

				// get id for remove
				if (compR->id == 1234)
				{
					toDel1 = G_ECMANAGER_UI->getObjId(itr);
				}

				////////////
				// childrens

				// recursive fn to do to all children

				std::function<void(ComponentManager::ChildContainerT*)> doChildrens = [&](ComponentManager::ChildContainerT* childrens)
				{
					for (int uid : *childrens)
					{
						Entity childEntity = G_ECMANAGER_UI->getEntity(uid);


						std::cout << std::endl << "Print child:" << std::endl;

						std::cout << "childEntity generation:" << childEntity.getGeneration() << std::endl;

						std::cout << "childEntity id:" << childEntity.getId() << std::endl;
						std::cout << "childEntity numChild:" << childEntity.getNumChildren() << std::endl;
						std::cout << "childEntity numDec:" << childEntity.getNumDecendants() << std::endl;
						std::cout << "childEntity parentuid:" << childEntity.getParentId() << std::endl;

						compR = childEntity.getComponent<ComponentTest0>();
						std::cout << "Child ComponentTest0:" << compR->id << " " << compR->c << std::endl;

						// get id for remove
						if (compR->id == 8882)
						{
							toDel0 = childEntity.getId();
						}
						if (compR->id == 5432)
						{
							toDel2 = childEntity.getId();
						}

						// get transform component
						ComponentTransform* compT = childEntity.getComponent<ComponentTransform>();
						if (compT != nullptr) // nullptr -> uninitialised or deleted
							std::cout << "Transform:" << compT->_position.x << std::endl;

						std::cout << std::endl;

						// call recursive fn for each child
						doChildrens(G_ECMANAGER_UI->getEntity(uid).getChildren());
					}
				};

				doChildrens(entity.getChildren());

				//
				////////////

				std::cout << "Object:" << G_ECMANAGER_UI->getObjId(itr) << " END" << std::endl;
				std::cout << std::endl;

				++itr;
			}
		};

		print();

		std::cout << std::endl;
		std::cout << "// Test Get Components END" << std::endl;
		std::cout << std::endl;





		std::cout << std::endl;
		std::cout << "// Test Remove Components:" << std::endl;
		std::cout << std::endl;

		G_ECMANAGER_UI->RemoveComponent<ComponentTransform>(toDel0);
		//print();

		G_ECMANAGER_UI->FreeEntity(toDel2);
		//print();

		G_ECMANAGER_UI->FreeEntity(toDel1);
		print();

		std::cout << std::endl;
		std::cout << "// Test Remove Components END" << std::endl;
		std::cout << std::endl;




		std::cout << "// Test Components END" << std::endl;
		std::cout << "////////////////////////////////////" << std::endl;
		std::cout << std::endl;

	}
	//// GET COMPONENT END
	///////////////////////////////////////////////////////////////////////////////////////////////

}

void ComponentManager::Init()
{


	///////////////////////////////////////////////////////////////////////////////////////////////
	// me test stuff
	{
		std::cout << std::endl;
		std::cout << "/////////////////////////////////////" << std::endl;
		std::cout << "//  me test stuff:" << std::endl;
		std::cout << std::endl;
		TestVector tv;
		tv.testMyVector();
		std::cout << std::endl;
		std::cout << "//  me test stuff END" << std::endl;
		std::cout << "/////////////////////////////////////" << std::endl;
		std::cout << std::endl;
	}
	// me test stuff END
	///////////////////////////////////////////////////////////////////////////////////////////////

	TestComponents();


}

void ComponentManager::Free()
{
	for (std::pair<ContainerID, ComponentSet*> p : ComponentSets)
	{
		ComponentSet* cs = p.second;
		cs->cmm.freeAll();
		delete cs;
		//free(cs);
		p.second = nullptr;
	}
	ComponentSets.clear();
}

void ComponentManager::Clear(COMPONENTSETNAMES id)
{
	//if (id == COMPONENT_MAIN) {}

	auto itr = ComponentSets.find((int)id);

	if (itr == ComponentSets.end()) throw;

	itr.operator*().second->cmm.clearAll();

	//for (std::pair<ContainerID, ComponentSet*> p : ComponentSets)
	//{
	//	int index = p.first;
	//	if (index == id)
	//	{
	//		ComponentSet* cs = p.second;
	//		cs->cmm.freeAll();
	//		break;
	//	}
	//}
}

void ComponentManager::Clear()
{
	for (std::pair<ContainerID, ComponentSet*> p : ComponentSets)
	{
		ComponentSet* cs = p.second;
		cs->cmm.clearAll();
	}
}

void ComponentManager::Exit()
{
	DestroyInstance();
}

///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
////  Test Fns
//// 

// depreciated test below // just for my reference

// test
//
//bool ComponentManager::IteratorM::operator==(IteratorM& itr)
//{
//	return mci == itr.mci;
//}
//
//bool ComponentManager::IteratorM::operator!=(IteratorM& itr)
//{
//	return mci != itr.mci;
//}
//
//char* ComponentManager::IteratorM::operator*()
//{
//	return *mci;
//}
//
//ComponentManager::IteratorM& ComponentManager::IteratorM::operator++()
//{
//	++mci;
//	return *this;
//}
//
//ComponentManager::IteratorM ComponentManager::IteratorM::operator++(int)
//{
//	mci++;
//	return *this;
//}
//
//ComponentManager::IteratorM ComponentManager::end(ContainerID comT)
//{
//	IteratorM itr;
//	itr.mci = cmm.end(comT);
//	return itr;
//}
//
//ComponentManager::IteratorM ComponentManager::begin(ContainerID comT)
//{
//	IteratorM itr;
//	itr.mci = cmm.begin(comT);
//	return itr;
//}
//
//ComponentManager::ContainerID ComponentManager::createNewComponentType(ContainerSettings set)
//{
//	return cmm.createNewComponentType(set);
//}
//
//char* ComponentManager::getElementAt(ContainerID comT, int index)
//{
//	return cmm.getElementAt(comT, index);
//}
//
//int ComponentManager::insertIntoContainer(ContainerID comT, char* obj)
//{
//	return cmm.insertIntoContainer(comT, obj);
//}
//
//void ComponentManager::removeFromContainer(ContainerID comT, int index)
//{
//	cmm.removeFromContainer(comT, index);
//}
//
//void ComponentManager::removeFromContainer(IteratorM& itr)
//{
//	cmm.removeFromContainer(itr.mci);
//}
//
//void ComponentManager::freeAll()
//{
//	cmm.freeAll();
//}
//
//


///////////////////////////////////////////////////////////////////////////////////////////////////
////  Test Fns
//// 

// depreciated test below // just for my reference

//
//void ComponentManager::test0()
//{
//	struct ComponentT
//	{
//		float vert[4];
//		int id;
//	};
//
//	struct ComponentT2
//	{
//		int id;
//		float vertT[16];
//		int t1;
//		int t2;
//		float vert[4];
//	};
//
//	std::cout << "Hello World!\n";
//
//	std::cout << sizeof(char) << "\n";
//
//	ComponentMemoryManager comMgr;
//
//	ComponentMemoryManager::ComponentTypeSettings csetT;
//	csetT.elementSize = sizeof(ComponentT);
//
//	ComponentMemoryManager::ComponentType ctT = comMgr.createNewComponentType(csetT);
//
//
//	for (int i = 0; i < 1000; ++i)
//	{
//		ComponentT newComp
//		{
//			{1.0f, 2.0f, 3.0f, 4.0f},
//			i
//		};
//
//		comMgr.insertIntoContainer(ctT, reinterpret_cast<char*>(&newComp));
//	}
//
//	ComponentT* cT = reinterpret_cast<ComponentT*>(comMgr.getElementAt(ctT, 0));
//
//	std::cout << cT->id << std::endl;
//	std::cout << cT->vert[0] << std::endl;
//	std::cout << cT->vert[3] << std::endl;
//
//	comMgr.removeFromContainer(ctT, 159);
//	comMgr.removeFromContainer(ctT, 784);
//
//	ComponentMemoryManager::MemConIterator itr = comMgr.begin(ctT);
//	itr = comMgr.begin(ctT);
//	auto itrEnd = comMgr.end(ctT);
//	int i = 0;
//
//	while (itr != itrEnd)
//	{
//		ComponentT* testC = reinterpret_cast<ComponentT*>(*itr);
//
//		if (i != testC->id)
//		{
//			std::cout << testC->id << std::endl;
//			std::cout << testC->vert[2] << std::endl;
//
//			++i;
//		}
//
//		++i;
//		++itr;
//	}
//
//
//	// DUN ANYHOW FREE !!!
//	comMgr.freeAll();
//}

// depreciated
//void ComponentManager::test1()
//{
	// depreciated
	//struct ComponentT
	//{
	//	float vert[4];
	//	int id;
	//};

	////ManagerComponent::ComponentType;

	//ComponentManager mgrCom;

	////mgrCom.test();
	////std::cout << "Test!\n";

	//// settings
	//ComponentManager::ContainerSettings set;
	//// set component size
	//set.elementSize = sizeof(ComponentT);

	//// create container // returns a container id
	//ComponentManager::ContainerID container1 = mgrCom.createNewComponentType(set);

	//int n = 0;

	//for (int i = 0; i < 1000; ++i)
	//{
	//	// init a new component
	//	ComponentT newComponent{ {1.11f, 2.22f, 3.33f, 4.44f}, i };

	//	// insert the component
	//	mgrCom.insertIntoContainer(container1, reinterpret_cast<char*>(&newComponent)); // can use templates ?

	//	++n;
	//}

	//std::cout << "Obj created :" << n << std::endl;

	//// get component at index
	//ComponentT* comT = reinterpret_cast<ComponentT*>((mgrCom.getElementAt(container1, 777)));
	//std::cout << "Should read 777 :" << comT->id << std::endl;

	//// del component at index
	//mgrCom.removeFromContainer(container1, 777);
	//std::cout << "mgrCom.removeFromContainer(container1, 777);" << std::endl;

	//// get component at index
	//comT = reinterpret_cast<ComponentT*>((mgrCom.getElementAt(container1, 777)));
	//std::cout << "Should read 0, since removed :" << comT->id << std::endl;

	//// this index includes empty slots

	//// iteration
	//auto itr = mgrCom.begin(container1);
	//auto itrEnd = mgrCom.end(container1);
	//int i = 0;
	//while (itr != itrEnd)
	//{
	//	comT = reinterpret_cast<ComponentT*>(*itr);
	//	std::cout << comT->id << " ";
	//	++itr;
	//	++i;
	//}
	//std::cout << std::endl;
	//std::cout << "1000 - 1 removed = 999 left:" << i << std::endl;

	//// remove with itr
	//// iteration
	//itr = mgrCom.begin(container1);
	//itrEnd = mgrCom.end(container1);
	//i = 0;
	//while (itr != itrEnd)
	//{
	//	comT = reinterpret_cast<ComponentT*>(*itr);
	//	std::cout << comT->id << " ";

	//	if (comT->id == 861)
	//	{
	//		std::cout << "\n Remove:" << comT->id << " \n";
	//		mgrCom.removeFromContainer(itr);
	//		itrEnd = mgrCom.end(container1); // need to update the end itr !!
	//		std::cout << "\n After Remove:" << comT->id << " \n";

	//		--i; // remove count
	//	}

	//	++itr;
	//	++i;
	//}
	//std::cout << std::endl;
	//std::cout << "999 - 1 removed = 998 left:" << i << std::endl;



	//// dun anyhow free
	//mgrCom.freeAll();
//}

//void ComponentManager::test2()
//{
	// depreciated

	//struct ComponentT
	//{
	//	float vert[4];
	//	int id;
	//};

	//struct ComponentT1
	//{
	//	int id0;
	//	float s[16];
	//	float r[16];
	//	float t[16];
	//	int id1;
	//};

	//ComponentManager mgrCom;

	//// factory
	//ComponentManager::ComponentSetFactory comsetFac;

	//// init the build
	//comsetFac.StartBuild();

	//// adds a component using component size
	//ComponentManager::ContainerID containerT0 = comsetFac.AddComponentContainer(sizeof(ComponentT)); // save the container id

	//// adds a component using settings
	//ComponentManager::ContainerSettings set;
	//set.elementSize = sizeof(ComponentT1);
	//ComponentManager::ContainerID containerT1 = comsetFac.AddComponentContainer(set); // save the container id

	//// builds the component
	//ComponentManager::ComponentSet* compSet0 = comsetFac.Build();

	//// component manager
	//ComponentManager mgrComp;

	//// adds the component set to the manager
	//mgrComp.AddComponentSet(compSet0);

	//// init component set manager
	//ComponentManager::ComponentSetManager compSetMgr(compSet0);

	//// build a new object
	//int newObjId = compSetMgr.BuildObject();

	//// attach component
	//ComponentT newComponentT =
	//{
	//	{1.11f, 2.22f, 3.33f, 4.44f},
	//	newObjId
	//};
	//void* newObjPtr = compSetMgr.AttachComponent(containerT0, newObjId, &newComponentT);

	//// get component
	//ComponentT* componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, newObjId));
	//std::cout << "Test  get component:" << std::endl;
	//std::cout << componentTest->id << std::endl;
	//std::cout << componentTest->vert[0] << std::endl;
	//std::cout << componentTest->vert[3] << std::endl;


	//// remove component
	//compSetMgr.RemoveComponent(containerT0, newObjId);

	//// remove object
	//compSetMgr.UnBuildObject(newObjId);

	//// try get component after remove
	//componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, newObjId));
	//std::cout << "Test  try get component after remove:" << std::endl;
	//std::cout << componentTest->id << std::endl;
	//std::cout << componentTest->vert[0] << std::endl;
	//std::cout << componentTest->vert[3] << std::endl;


	//// insert stress test
	//std::vector<int> objIds;
	//for (int i = 0; i < 1000; ++i)
	//{
	//	// new obj
	//	newObjId = compSetMgr.BuildObject();
	//	objIds.push_back(newObjId);

	//	// attach component
	//	ComponentT newComponentTN =
	//	{
	//		{1.11f, 2.22f, 3.33f, 4.44f},
	//		newObjId
	//	};
	//	compSetMgr.AttachComponent(containerT0, newObjId, &newComponentTN);
	//}

	//// get stress tess
	//std::cout << "Test  get stress:" << std::endl;
	//for (int id : objIds)
	//{
	//	componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, id));
	//	std::cout << "Test :";
	//	std::cout << componentTest->id << " ";
	//	//std::cout << componentTest->vert[0] << " ";
	//	//std::cout << componentTest->vert[3] << std::endl;
	//}

	//// insert into containerT1
	//std::cout << "\nTest  insert containerT1:" << std::endl;
	//std::vector<int> obj2Ids;
	//for (int i = 0; i < 1000; ++i)
	//{
	//	// new obj
	//	newObjId = compSetMgr.BuildObject();
	//	obj2Ids.push_back(newObjId);

	//	// attach component
	//	ComponentT newComponentTN =
	//	{
	//		{1.11f, 2.22f, 3.33f, 4.44f},
	//		newObjId
	//	};
	//	compSetMgr.AttachComponent(containerT0, newObjId, &newComponentTN);

	//	// attach component
	//	ComponentT1 newComponentT1 =
	//	{
	//		newObjId,
	//		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
	//		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
	//		{1.11f, 2.22f, 3.33f, 4.44f, 5.55f, 6.66f, 7.77f, 8.88f, 9.99f, 10.10f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f},
	//		newObjId,
	//	};
	//	compSetMgr.AttachComponent(containerT1, newObjId, &newComponentT1);
	//}

	//// get from containerT1
	//std::cout << "Test  get containerT1:" << std::endl;
	//for (int id : obj2Ids)
	//{
	//	ComponentT1* componentTest1 = reinterpret_cast<ComponentT1*>(compSetMgr.getComponent(containerT1, id));
	//	std::cout << "T1 :";
	//	std::cout << componentTest1->id0 << " ";
	//	std::cout << componentTest1->id1 << " ";

	//	componentTest = reinterpret_cast<ComponentT*>(compSetMgr.getComponent(containerT0, id));
	//	std::cout << "T0 :";
	//	std::cout << componentTest->id << " ";
	//}


	//// ITERATOR
	//std::cout << "\n\nTest  ITERATOR containerT1:" << std::endl;
	//ComponentManager::ComponentSetManager::Iterator itr = compSetMgr.begin(containerT1);
	//auto itrEnd = compSetMgr.end(containerT1);

	//while (itr != itrEnd)
	//{
	//	ComponentT1* comTest1 = reinterpret_cast<ComponentT1*>(*itr);
	//	std::cout << "T1 :";
	//	std::cout << comTest1->id0 << " ";
	//	std::cout << comTest1->id1 << " ";

	//	++itr;
	//}


	//std::cout << "\n\nTest  ITERATOR containerT0:" << std::endl;
	//itr = compSetMgr.begin(containerT0);
	//itrEnd = compSetMgr.end(containerT0);

	//int c = 0;

	//while (itr != itrEnd)
	//{
	//	ComponentT* comTest0 = reinterpret_cast<ComponentT*>(*itr);
	//	std::cout << "T0:";
	//	std::cout << comTest0->id << " ";

	//	std::cout << "ID:";
	//	std::cout << compSetMgr.getObjId(itr) << " ";

	//	//if (itr.getObjId() != -1)
	//	//{
	//	//	ComponentT1* comTest1 = reinterpret_cast<ComponentT1*>(compSetMgr.getComponent(containerT1, itr.getObjId()));
	//	//	std::cout << "T1 :";
	//	//	std::cout << comTest1->id0 << " ";
	//	//}
	//	//else
	//	//{
	//	//	std::cout << "T1 : - ";
	//	//}

	//	ComponentManager::ComponentSetManager::Entity obj = compSetMgr.getEntity(itr);

	//	ComponentT1* comTest1 = reinterpret_cast<ComponentT1*>(obj.getComponent(containerT1));
	//	if (comTest1 != nullptr)
	//	{
	//		std::cout << "T1:";
	//		std::cout << comTest1->id0 << " ";
	//	}
	//	else
	//	{
	//		std::cout << "T1:-";
	//	}


	//	if (c == 1)
	//	{
	//		int j = 0;
	//	}

	//	++c;
	//	++itr;
	//}


	//mgrCom.Free();
//}

} // NS
