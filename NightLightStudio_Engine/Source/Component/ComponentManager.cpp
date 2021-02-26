
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

#include "..\..\\ISerializable.h"



#include "ComponentManagerTypedefs.h"

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
	//static int unknown_ent_id = 0;
	if (name == "Entity_")
	{
		name.append(std::to_string(compSet->unknown_ent_id));
		++compSet->unknown_ent_id;
	}
	
	EntityName[objId + compSet->idIndexModifier] = name;

	//EntityName[objId + compSet->idIndexModifier] = name;
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
	//// insert the component
	//int newCId = compSet->cmm.insertIntoContainer(compId, reinterpret_cast<const char*>(newComp));

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

	if (/*compData->compPtr != nullptr || */compData->containerId != -1 || compData->containerIndex != -1)
		//throw; // check for double allocation
		return nullptr;

	// insert the component // shifted the insert component AFTER the check 
	int newCId = compSet->cmm.insertIntoContainer(compId, reinterpret_cast<const char*>(newComp));

	// !
	compData->containerId = compId; // set container id
	compData->compPtr = reinterpret_cast<void*>( // set component ptr within the container
		compSet->cmm.getElementAt(compId, newCId)
		);;
	compData->containerIndex = newCId; // set component index within the container


	ISerializable* ser = reinterpret_cast<ISerializable*>(compData->compPtr);
	ser->objId = objId + compSet->idIndexModifier;

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

	if (compData->containerId == -1 || compData->containerIndex == -1)
		throw; // prevent double remove causing undefined behaviour

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


	ISerializable* ser = reinterpret_cast<ISerializable*>(*itr);


	return ser->objId;

	
	//if(ser->objId != 0xcccccccc) // 0xcccccccc == -858993460 // uninit value
	//	return ser->objId;

	// the below code is not O(1) // depreciated

	//int containerId = itr.itrCurrState == Iterator::IteratorState::ITR_ROOT ? itr.containerId : itr.containerIdChild;
	//int objContainerId = itr.itrCurrState == Iterator::IteratorState::ITR_ROOT ? compSet->objContainerId : compSet->objContainerIdChilds;

	//int objId = -1;


	////compSet->cmm.getElementAt(compSet->objContainerId, 0);
	//auto itrOb = compSet->cmm.begin(objContainerId);
	//auto itrObEnd = compSet->cmm.end(objContainerId);
	//while (itrOb != itrObEnd)
	//{
	//	char* obj = (*itrOb);

	//	ComponentSet::ObjectData* obj_o = reinterpret_cast<ComponentSet::ObjectData*>(obj);
	//	

	//	obj += sizeof(ComponentManager::ComponentSet::ObjectData);

	//	// find the container id
	//	int n = 0;
	//	for (int currentId : compSet->componentContainerIDs)
	//	{
	//		if (currentId == containerId)
	//			break;
	//		++n;
	//	}

	//	// get the position of the component data
	//	obj += sizeof(ComponentManager::ComponentSet::ObjectData::ComponentData) * (n/2);

	//	//check
	//	ComponentSet::ObjectData::ComponentData* comp = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(obj);
	//	if (itr.itrCurrState == Iterator::IteratorState::ITR_ROOT)
	//	{
	//		if (comp->containerIndex == itr.memItr.getCurrentIndex())
	//		//if (comp->containerId == itr.containerId) //memItr.getCurrentIndex())
	//		{
	//			objId = obj_o->objId;
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		if (comp->containerIndex == itr.memItrChild.getCurrentIndex())
	//		{
	//			objId = obj_o->objId;
	//			break;
	//		}
	//	}

	//	++itrOb;
	//}

	//if (objId == -1) throw;

	////return objId + compSet->idIndexModifier;
	//return itr.itrCurrState == Iterator::IteratorState::ITR_ROOT ? objId + compSet->idIndexModifier : objId + compSet->idIndexModifier + IDRANGE_RT;
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

	if (objId == -1) return nullptr; // if objId is -1 it will crash 
	//if (objId == -1) throw; // can use throw to debug !!

	return compSetMgr->getComponent(compId, objId);
}


// param tgt_compSetMgr is the target set to copy to
// param entityName is the name of the new entity

inline ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::EntityHandle::Copy(ComponentManager::ComponentSetManager* tgt_compSetMgr, std::string entityName)
{
	// does not copy children // copy to root

	EntityHandle newEntity = tgt_compSetMgr->BuildEntity(entityName);

	for (ISerializable* comp : getEntityComponentContainer())
	{
		ISerializable* newComp = comp->Clone();

		//const std::type_info& tinf = typeid(T);

		const std::type_info& newComp_tinf = typeid(*newComp);


		//const std::type_info& comp_tinf = typeid(*comp);

		//size_t newhcode = newComp_tinf.hash_code();
		//size_t hcode = comp_tinf.hash_code();

		int conInd = -1;

		auto find = tgt_compSetMgr->compSet->hashConIdMap.find(newComp_tinf.hash_code());

		if (find == tgt_compSetMgr->compSet->hashConIdMap.end())
			throw; // gg

		conInd = (*find).second;

		tgt_compSetMgr->AttachComponent(conInd, newEntity.getId(), newComp);

		delete newComp; // !!

	}

	return newEntity;

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

int ComponentManager::ComponentSetManager::EntityHandle::getId() const
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

ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::getEntity(std::string str)
{
	auto itr = begin<ComponentTransform>();
	auto endItr = end<ComponentTransform>();
	for (; itr != endItr; ++itr)
	{
		ComponentTransform* trans = getComponent<ComponentTransform>(itr);
		if (trans && trans->_entityName.toString() == str)
		{
			return getEntity(itr);
		}
	}
	return EntityHandle(nullptr, -1);
}

std::vector<ComponentManager::ComponentSetManager::EntityHandle> ComponentManager::ComponentSetManager::getEntityList(std::string str)
{
	std::vector<ComponentManager::ComponentSetManager::EntityHandle> ent_list;
	auto itr = begin<ComponentTransform>();
	auto endItr = end<ComponentTransform>();
	for (; itr != endItr; ++itr)
	{
		ComponentTransform* trans = getComponent<ComponentTransform>(itr);
		if (trans && trans->_entityName.toString() == str)
		{
			ent_list.emplace_back(getEntity(itr));
		}
	}
	return ent_list;
}

ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::getEntityUsingEntName(std::string str)
{
	/*
	auto itr = SYS_COMPONENT->EntityName.begin();
	auto itrEnd = SYS_COMPONENT->EntityName.end();

	while (itr != itrEnd)
	{
		if((*itr)->)

		++itr;
	}*/

	for (auto& i : G_ECMANAGER->EntityName)
	{
		if (i.second == str)
			return getEntity(i.first);
	}
	/*
	for (Entity ent : getEntityContainer())
	{

		auto itr = SYS_COMPONENT->EntityName.find(ent.getId());

		if (itr != SYS_COMPONENT->EntityName.end())
		{
			// found !!
			
		}
	}*/
	// not found
	return EntityHandle(nullptr, -1);
}

std::vector<ComponentManager::ComponentSetManager::EntityHandle> ComponentManager::ComponentSetManager::getEntityTagContainer(std::string str)
{
	std::vector<EntityHandle> container;

	auto itr = begin<ComponentTransform>();
	auto endItr = end<ComponentTransform>();
	for (; itr != endItr; ++itr)
	{
		ComponentTransform* trans = getComponent<ComponentTransform>(itr);
		if (trans && trans->_entityName.toString() == str)
		{
			Entity ent = getEntity(itr);
			container.push_back(ent);
		}
	}

	return container;
}

ComponentManager::ComponentSetManager::EntityHandle ComponentManager::ComponentSetManager::getEntity(ISerializable* component)
{
	int getid = component->objId;

	return EntityHandle(this, getid);
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

ComponentManager::ComponentSetManager::Iterator& ComponentManager::ComponentSetManager::Iterator::operator--()
{

	switch (itrDoState)
	{
	case IteratorState::ITR_ROOT:
		--memItr;
		return *this;
		break;

	case IteratorState::ITR_CHILD:
		--memItrChild;
		return *this;
		break;

	case IteratorState::ITR_BOTH:
		if (itrCurrState == IteratorState::ITR_ROOT)
		{
			--memItr;
			if (endRtObjIndex == memItr.getCurrentObjIndex())
			{
				itrCurrState = IteratorState::ITR_CHILD;
			}
			return *this;
		}
		else if (itrCurrState == IteratorState::ITR_CHILD)
		{
			--memItrChild;
			return *this;
		}
		else
			throw;
		break;
	}

	throw;
}

ComponentManager::ComponentSetManager::Iterator ComponentManager::ComponentSetManager::Iterator::operator--(int)
{
	switch (itrDoState)
	{
	case IteratorState::ITR_ROOT:
	{
		ComponentManager::ComponentSetManager::Iterator newItr = *this; // post
		--memItr;
		return newItr;
		break;
	}

	case IteratorState::ITR_CHILD:
	{
		ComponentManager::ComponentSetManager::Iterator newItr = *this; // post
		--memItrChild;
		return newItr;
		break;
	}

	case IteratorState::ITR_BOTH:
		ComponentManager::ComponentSetManager::Iterator newItr = *this; // post
		if (itrCurrState == IteratorState::ITR_ROOT)
		{
			--memItr;
			if (endRtObjIndex == memItr.getCurrentObjIndex())
			{
				itrCurrState = IteratorState::ITR_CHILD;
			}
			return newItr;
		}
		else if (itrCurrState == IteratorState::ITR_CHILD)
		{
			--memItrChild;
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

	auto build = [&](COMPONENTSETNAMES _id)
	{
		// Building another component set
		comsetFac.StartBuild();

		comsetFac.AddComponentContainer<ComponentTransform>();
		//comsetFac.AddComponentContainer<ComponentTest0>();
		comsetFac.AddComponentContainer<ComponentCollider>();
		comsetFac.AddComponentContainer<ComponentRigidBody>();
		comsetFac.AddComponentContainer<ComponentGraphics>();
		comsetFac.AddComponentContainer<ComponentAudio>();
		comsetFac.AddComponentContainer<ComponentLoadAudio>();
		comsetFac.AddComponentContainer<ComponentScript>();
		//comsetFac.AddComponentContainer<ComponentInput>();
		//comsetFac.AddComponentContainer<ComponentLogic>();
		//comsetFac.AddComponentContainer<ComponentCamera>();
		comsetFac.AddComponentContainer<ComponentLight>();
		comsetFac.AddComponentContainer<ComponentCanvas>();
		comsetFac.AddComponentContainer<ComponentAnimation>();
		comsetFac.AddComponentContainer<ComponentEmitter>();
		comsetFac.AddComponentContainer<ComponentCamera>();
		
		comsetFac.AddComponentContainer<ComponentPlayerStats>();
		comsetFac.AddComponentContainer<ComponentCauldronStats>();

		// Temporary C++ Scripting component
		comsetFac.AddComponentContainer<ComponentCScript>();
		comsetFac.AddComponentContainer<ComponentVariables>();

		comsetFac.AddComponentContainer<ComponentNavigator>();
		comsetFac.AddComponentContainer<WayPointMapComponent>();
		comsetFac.AddComponentContainer<WayPointComponent>();

		// builds the component set
		ComponentManager::ComponentSet* cs = comsetFac.Build();
		// adds the component set to the component manager
		SYS_COMPONENT->AddComponentSet(_id, cs);

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
	//SYS_COMPONENT->Clear(NS_COMPONENT::COMPONENT_MAIN);
	//SYS_COMPONENT->Clear();

	//Entity newEntity = G_ECMANAGER->BuildEntity();
	//ComponentCollider* OBBT = newEntity.AddComponent<ComponentCollider>();
	//ComponentCollider* OBBT = G_ECMANAGER->AddComponent<ComponentCollider>(newEntity);
	//ComponentCollider OBB1(COLLIDERS::OBB);
	//*OBBT = OBB1;




	///////////////////////////////////////////////////////////////////////////////////////////////////
	////// sample for iterating ISerializable in entity

	//// create a sample entity here
	//Entity newEntity = G_ECMANAGER->BuildEntity();
	//newEntity.AttachComponent<ComponentTransform>();
	//newEntity.AttachComponent<ComponentCollider>();
	//newEntity.AttachComponent<ComponentCollider>();

	//newEntity.RemoveComponent<ComponentCollider>();
	//newEntity.RemoveComponent<ComponentCollider>();

	////newEntity.AttachComponent<ComponentTest0>();
	////newEntity.AttachComponent<ComponentRigidBody>();
	////newEntity.AttachComponent<ComponentGraphics>(); 
	//// uninit graphics component will crash at graphics.cpp line 178 Mesh* mesh = meshManager->meshes[graphicsComp->MeshID];
	//// the id not init apparently

	/////////////////////////////////////////////////////////////////////////////////////
	//// using iterator
	//auto con = newEntity.getEntityComponentContainer();
	//auto itr = con.begin();
	//auto itrEnd = con.end();
	//while (itr != itrEnd)
	//{
	//	ISerializable* comp = (*itr);
	//	comp->Write();
	//	++itr; // rmb to increment itr
	//}
	//// using iterator END
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	//// using for each
	//for (ISerializable* comp : newEntity.getEntityComponentContainer())
	//{
	//	(void)comp;
	//	comp->Write();
	//}
	//// using for each END
	/////////////////////////////////////////////////////////////////////////////////////

	////// sample for iterating ISerializable in entity END
	///////////////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////// sample for iterating Entity

	//Entity newEntity = G_ECMANAGER->BuildEntity();
	//newEntity = G_ECMANAGER->BuildEntity();
	//newEntity = G_ECMANAGER->BuildEntity();

	/////////////////////////////////////////////////////////////////////////////////////
	//// using iterator
	//auto con = G_ECMANAGER->getEntityContainer();
	//auto itr = con.begin();
	//auto itrEnd = con.end();
	//while (itr != itrEnd)
	//{
	//	Entity ent = (*itr);
	//	ent.AttachComponent<ComponentTest0>();
	//	++itr; // rmb to increment itr
	//}
	//// using iterator END
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	//// using for each
	//for (Entity ent : G_ECMANAGER->getEntityContainer())
	//{
	//	ent.RemoveComponent<ComponentTest0>();
	//}
	//// using for each END
	/////////////////////////////////////////////////////////////////////////////////////


	//////// sample for iterating Entity END
	/////////////////////////////////////////////////////////////////////////////////////////////////////





	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////// sample for entity copy

	//// create a entity in prefab
	//Entity entity = G_ECMANAGER_PREFABS->BuildEntity();
	//ComponentTransform compTransform;
	//compTransform._position = NlMath::Vector3D(1.0f, 2.0f, 3.0f);
	//entity.AttachComponent<ComponentTransform>(compTransform);
	//entity.AttachComponent<ComponentCollider>();
	//entity.AttachComponent<ComponentRigidBody>();

	//// copy to main
	//Entity newEntity = entity.Copy(G_ECMANAGER, "NewCopy");
	//ComponentTransform* newtransform = newEntity.getComponent<ComponentTransform>();

	//// iterate main
	//for (Entity ent : G_ECMANAGER->getEntityContainer())
	//{
	//	ComponentTransform* transform = ent.getComponent<ComponentTransform>();

	//	std::cout << transform->_position.x << " " << transform->_position.y << " " << transform->_position.z << " " << std::endl;

	//	for (ISerializable* comp : ent.getEntityComponentContainer())
	//	{
	//		(void)comp;
	//		ISerializable* newComp = comp->Clone();
	//		(void)newComp;
	//		delete newComp;
	//	}
	//}

	//////// sample for entity copy END
	/////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	////////// sample for get entity by tag
	//{
	//	Entity newEntity = G_ECMANAGER->BuildEntity();
	//	ComponentTransform newtransform;
	//	newtransform._entityName = "test";
	//	newEntity.AttachComponent<ComponentTransform>(newtransform);
	//}
	//{
	//	Entity newEntity = G_ECMANAGER->BuildEntity();
	//	ComponentTransform newtransform;
	//	newtransform._entityName = "test0";
	//	newEntity.AttachComponent<ComponentTransform>(newtransform);
	//}
	//{
	//	Entity newEntity = G_ECMANAGER->BuildEntity();
	//	ComponentTransform newtransform;
	//	newtransform._entityName = "test";
	//	newEntity.AttachComponent<ComponentTransform>(newtransform);
	//}

	//std::vector<Entity> entityContainer = G_ECMANAGER->getEntityTagContainer("test");

	//for (Entity e : entityContainer)
	//{
	//	int id = e.getId();
	//	(void)id;
	//}
	////////// sample for get entity by tag END
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// Test
	//Entity newEntity = G_ECMANAGER->BuildEntity();
	//G_ECMANAGER->AddComponent<ComponentTransform>(newEntity);
	//


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	////////// parent and child SAMPLE

	//Entity newEntity = G_ECMANAGER->BuildEntity();
	//ComponentTransform* ct = newEntity.AddComponent<ComponentTransform>();
	//ct->_entityName = "root0";

	//std::function<Entity(Entity, std::string)> makeChild = [&](Entity parentEntity, std::string childName)
	//{
	//	Entity newChildEntity = parentEntity.makeChild();
	//	ComponentTransform* ct = newChildEntity.AddComponent<ComponentTransform>();
	//	ct->_entityName = childName;

	//	if(newChildEntity.getGeneration() < 3)
	//		for (int i = 0; i < 2; ++i)
	//		{
	//			std::string newChildName = childName;
	//			newChildName.append("_child").append(std::to_string(i));
	//			makeChild(newChildEntity, newChildName);
	//		}

	//	return newChildEntity;
	//};

	//makeChild(newEntity, "root0_child0");
	//makeChild(newEntity, "root0_child1");

	//newEntity = G_ECMANAGER->BuildEntity();
	//ct = newEntity.AddComponent<ComponentTransform>();
	//ct->_entityName = "root1";
	//makeChild(newEntity, "root1_child0");
	//


	//auto print = [&]()
	//{
	//	// G_UICOMPSET
	//	auto itr = G_ECMANAGER->begin<ComponentTransform>();
	//	auto itrEnd = G_ECMANAGER->end<ComponentTransform>();
	//	while (itr != itrEnd)
	//	{
	//		// get the entity from the iterator
	//		Entity entity = G_ECMANAGER->getEntity(itr);

	//		std::cout << "Root Entity id:" << entity.getId() << std::endl;

	//		// get transform component
	//		ComponentTransform* compT = entity.getComponent<ComponentTransform>();
	//		if (compT != nullptr) // nullptr -> uninitialised or deleted
	//			std::cout << "name:" << compT->_entityName.c_str() << std::endl;

	//		////////////
	//		// childrens

	//		// recursive fn to do to all children

	//		auto printSpacing = [](int no)
	//		{
	//			for (int i = 0; i < no; ++i)
	//				std::cout << "  ";
	//		};

	//		std::function<void(NS_COMPONENT::ComponentManager::ChildContainerT*)> doChildrens = [&](NS_COMPONENT::ComponentManager::ChildContainerT* childrens)
	//		{
	//			for (int uid : *childrens)
	//			{
	//				Entity childEntity = G_ECMANAGER->getEntity(uid);

	//				std::cout << std::endl;
	//				printSpacing(childEntity.getGeneration());
	//				std::cout << "Print child : " << std::endl;

	//				printSpacing(childEntity.getGeneration());
	//				std::cout << "childEntity generation :" << childEntity.getGeneration() << std::endl;

	//				printSpacing(childEntity.getGeneration());
	//				std::cout << "childEntity id         :" << childEntity.getId() << std::endl;
	//				printSpacing(childEntity.getGeneration());
	//				std::cout << "childEntity numChild   :" << childEntity.getNumChildren() << std::endl;
	//				printSpacing(childEntity.getGeneration());
	//				std::cout << "childEntity numDec     :" << childEntity.getNumDecendants() << std::endl;
	//				printSpacing(childEntity.getGeneration());
	//				std::cout << "childEntity parentuid  :" << childEntity.getParentId() << std::endl;

	//				// get transform component
	//				ComponentTransform* compT = childEntity.getComponent<ComponentTransform>();
	//				if (compT != nullptr) // nullptr -> uninitialised or deleted
	//				{
	//					printSpacing(childEntity.getGeneration());
	//					std::cout << "name                   :" << compT->_entityName.c_str() << std::endl;
	//				}

	//				std::cout << std::endl;

	//				// call recursive fn for each child
	//				doChildrens(G_ECMANAGER->getEntity(uid).getChildren());
	//			}
	//		};

	//		// call fn
	//		doChildrens(entity.getChildren());

	//		// iterate
	//		++itr;
	//	}
	//};


	//print();


	//print();

	////////// parent and child SAMPLE END
	///////////////////////////////////////////////////////////////////////////////////////////////////////



	////////////
	// TEST 
	//Entity newEntity0 = G_ECMANAGER->BuildEntity();
	//ComponentTransform* ct0 = newEntity0.AddComponent<ComponentTransform>();
	//ct0->_entityName = "root0";

	//Entity newEntity1 = G_ECMANAGER->BuildEntity();
	//ComponentTransform* ct1 = newEntity1.AddComponent<ComponentTransform>();
	//ct1->_entityName = "root1";

	//Entity newEntity2 = G_ECMANAGER->BuildEntity();
	//ComponentTransform* ct2 = newEntity2.AddComponent<ComponentTransform>();
	//ct2->_entityName = "root2";

	//Entity getEnt = G_ECMANAGER->getEntity(ct1);

	//ComponentTransform* getCt = getEnt.getComponent<ComponentTransform>();

	//if (getCt == ct1)
	//{
	//	std::cout << "yay" << std::endl;
	//}
	//else
	//{
	//	std::cout << "nay" << std::endl;
	//}
	// TEST  END
	////////////


	//// TEST LV
	//LocalVector<int> tlv;
	//for (int i = 0; i < 10; ++i)
	//	tlv.push_back(i);

	//for (int& lvint : tlv)
	//{
	//	std::cout << lvint << std::endl;
	//}
	//std::cout << std::endl;

	//tlv.pop_back();


	//for (int& lvint : tlv)
	//{
	//	std::cout << lvint << std::endl;
	//}
	//std::cout << std::endl;

	//tlv.pop_back();
	//tlv.pop_back();
	//tlv.pop_back();

	//for (int& lvint : tlv)
	//{
	//	std::cout << lvint << std::endl;
	//}
	//std::cout << std::endl;

	//std::cout << tlv.back() << std::endl;

	//tlv.pop_back();
	//tlv.pop_back();

	//// END TEST

	////////
	//Entity newEntity0 = G_ECMANAGER->BuildEntity();
	//newEntity0.AddComponent<ComponentGraphics>();
	//newEntity0.AddComponent<ComponentTransform>();
	//ComponentGraphics* compGfx = newEntity0.getComponent<ComponentGraphics>();
	//ComponentTransform* compTx = newEntity0.getComponent<ComponentTransform>();
	//compTx->_position.x = 0.0f;
	//compGfx->_modelID = 0;

	//for (int i = 1; i < 1000; ++i)
	//{
	//	newEntity0 = G_ECMANAGER->BuildEntity();
	//	newEntity0.AddComponent<ComponentGraphics>();
	//	newEntity0.AddComponent<ComponentTransform>();
	//	ComponentGraphics* compGfx = newEntity0.getComponent<ComponentGraphics>();
	//	ComponentTransform* compTx = newEntity0.getComponent<ComponentTransform>();
	//	compTx->_position.x = 0.0f + i;
	//	compGfx->_modelID = i;
	//}

	//int count = 0;

	//auto itr = G_ECMANAGER->begin<ComponentGraphics>();
	//auto itrEnd = G_ECMANAGER->end<ComponentGraphics>();
	//for (; itr != itrEnd; ++itr, ++count)
	//{
	//	//{
	//	//	// get components from iterator
	//	//	ComponentGraphics* compGfx = G_ECMANAGER->getComponent<ComponentGraphics>(itr);
	//	//	ComponentTransform* compTx = G_ECMANAGER->getComponent<ComponentTransform>(itr);
	//	//}

	//	{

	//		if (count == 328)
	//		{
	//			int k = 1;
	//		}

	//		{
	//			// get entity from iterator, then get components from entity
	//			Entity entity = G_ECMANAGER->getEntity(itr);
	//			ComponentTransform* compTx = entity.getComponent<ComponentTransform>();
	//			ComponentGraphics* compGfx = entity.getComponent<ComponentGraphics>();
	//			int k = 1;
	//		}

	//		if (count == 328)
	//		{
	//			--itr;
	//			Entity entity = G_ECMANAGER->getEntity(itr);
	//			ComponentTransform* compTx = entity.getComponent<ComponentTransform>();
	//			ComponentGraphics* compGfx = entity.getComponent<ComponentGraphics>();
	//			int k = 1;
	//		}
	//	}

	//	{
	//		// get entity from component, then get components from entity
	//		ComponentTransform* compTx = G_ECMANAGER->getComponent<ComponentTransform>(itr);
	//		Entity entity = G_ECMANAGER->getEntity(compTx);
	//		ComponentGraphics* compGfx = entity.getComponent<ComponentGraphics>();
	//		int k = 1;
	//	}

	//	{
	//		// test
	//		// get entity from component, then get components from entity
	//		ComponentGraphics* compGfx = G_ECMANAGER->getComponent<ComponentGraphics>(itr);
	//		Entity entity = G_ECMANAGER->getEntity(compGfx);
	//		ComponentTransform* compTx = entity.getComponent<ComponentTransform>();
	//		int k = 1;
	//	}

	//}

	//newEntity0 = G_ECMANAGER->BuildEntity();
	//newEntity0.AddComponent<ComponentGraphics>();
	//newEntity0.AddComponent<ComponentTransform>();
	//ComponentGraphics* g = newEntity0.getComponent<ComponentGraphics>();
	//ComponentTransform* t = newEntity0.getComponent<ComponentTransform>();

	////////



	//

	//Entity newEntity0 = G_ECMANAGER->BuildEntity();
	//newEntity0 = G_ECMANAGER->BuildEntity();

	//newEntity0.AddComponent<ComponentGraphics>();

	//ComponentGraphics* g = newEntity0.getComponent<ComponentGraphics>();

	//newEntity0.AttachComponent<ComponentTransform>();

	//ComponentTransform* t = newEntity0.getComponent<ComponentTransform>();


	//newEntity0 = G_ECMANAGER->BuildEntity();
	//newEntity0 = G_ECMANAGER->BuildEntity();

	//newEntity0.getId();

	//newEntity0.getParentId();

	////G_ECMANAGER->FreeEntity();

	//Entity getE = G_ECMANAGER->getEntity(g);

	//getE = G_ECMANAGER->getEntity(t);

	//G_ECMANAGER->getEntity("t");

	//G_ECMANAGER->getEntity(1);

	//G_ECMANAGER->getEntity(G_ECMANAGER->begin<ComponentGraphics>());

	//G_ECMANAGER->getObjId(G_ECMANAGER->begin<ComponentGraphics>());


	//newEntity0.getComponent<ComponentGraphics>();


	//// 


	//G_ECMANAGER->getComponent<ComponentTransform>(G_ECMANAGER->begin<ComponentGraphics>());


	////G_ECMANAGER->getComponent<ComponentGraphics>((NS_COMPONENT::ComponentManager::ContainerID) 1, G_ECMANAGER->begin<ComponentGraphics>());

	////G_ECMANAGER->getComponent(1, 1);

	//G_ECMANAGER->getComponent(1, G_ECMANAGER->begin<ComponentGraphics>());


	for (int i = 0; i < 1000; ++i)
	{

		Entity newEntity0 = G_ECMANAGER->BuildEntity();

		newEntity0.AttachComponent<ComponentTransform>();
		newEntity0.AttachComponent<ComponentRigidBody>();

		//newEntity0.AddComponent<ComponentTransform>();
		//newEntity0.AddComponent<ComponentRigidBody>();
	}

	auto itr = G_ECMANAGER->begin<ComponentRigidBody>();
	auto itrEnd = G_ECMANAGER->end<ComponentRigidBody>();


	//for (int i = 0; i < 10000000; ++i)
	//{
	//	for (; itr != itrEnd; ++itr)
	//	{

	//		ComponentTransform* compT = G_ECMANAGER->getComponent<ComponentTransform>(itr);

	//		ComponentRigidBody* compR = G_ECMANAGER->getComponent<ComponentRigidBody>(itr);

	//		G_ECMANAGER->getEntity(itr);

	//		G_ECMANAGER->getEntity(compT);

	//		newEntity0.getComponent<ComponentGraphics>();

	//	}
	//}

	// ###TESTCODE###

	//G_ECMANAGER->AddComponent<ComponentRigidBody>(newEntity0);
	//G_ECMANAGER->AttachComponent(newEntity0, ComponentRigidBody());



	//ComponentTransform* compT;
	//for (int i = 0; i < 1000000; ++i)
	//{
	//	compT = G_ECMANAGER->getComponent<ComponentTransform>(itr);
	//	compT->_nextPos.x = (float)i;
	//}

	//ComponentRigidBody* compR;
	//for (int i = 0; i < 1000000; ++i)
	//{
	//	compR = G_ECMANAGER->getComponent<ComponentRigidBody>(itr);
	//	compR->mass = (float)i;
	//}

	//for (int i = 0; i < 1000000; ++i)
	//{
	//	if (itr != itrEnd)
	//	{
	//		compR->isActive = false;
	//	}
	//}

	//Entity newEntity0;
	//for (int i = 0; i < 1000000; ++i)
	//{
	//	newEntity0 = G_ECMANAGER->getEntity(itr);
	//}

	//for (int i = 0; i < 1000000; ++i)
	//{
	//	newEntity0 = G_ECMANAGER->getEntity(compT);
	//}

	//for (int i = 0; i < 1000000; ++i)
	//{
	//	newEntity0.getComponent<ComponentGraphics>();
	//}

	//for (int i = 0; i < 1000000; ++i)
	//{
	//	++itr;
	//	--itr;
	//}


	Clear();

}

void ComponentManager::Init()
{

	/*
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
	*/
	//Clear();
}

void ComponentManager::Free()
{
	for (std::pair<ContainerID, ComponentSet*> p : ComponentSets)
	{
		ComponentSet* cs = p.second;
		cs->cmm.freeAll();
		cs->unknown_ent_id = 0;
		delete cs;
		//free(cs);
		p.second = nullptr;
	}
	ComponentSets.clear();

}

void ComponentManager::Clear(COMPONENTSETNAMES _id)
{
	//if (id == COMPONENT_MAIN) {}

	auto itr = ComponentSets.find((int)_id);

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
