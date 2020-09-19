#pragma once

#include "ComponentMemoryManager.h"
#include <vector>
#include <map>

//**! Comment more !**//

class ComponentManager
{
public:
	typedef int ContainerID;
	//typedef int ComponentSetID;
	typedef ComponentMemoryManager::ComponentTypeSettings ContainerSettings;

	ComponentMemoryManager cmm;


	class ComponentSet; // fwd decl

	class ComponentSetFactory
	{
		ComponentSet* compSet;
	public:
		void StartBuild();
		ComponentManager::ContainerID AddComponent(ComponentMemoryManager::ComponentTypeSettings set);
		ComponentManager::ContainerID AddComponent(size_t size);
		ComponentManager::ComponentSet* Build();
		ComponentSetFactory() :
			compSet(nullptr)
		{
		}
		~ComponentSetFactory();
	};

	// ComponentSetManager
	// Manages Entity components
	class ComponentSetManager // builds objects into component sets
	{
	public: // !
		ComponentSet* compSet; // it is bound to a component set
	public:
		ComponentSetManager() = default;
		ComponentSetManager(ComponentSet* inCompSet) :
			compSet(inCompSet)
		{
		}

		int BuildObject(); // returns obj id

		void* AttachComponent(ComponentManager::ContainerID compId, int objId, void* newComp);

		// void* AttachComponent(ManagerComponent::ContainerID compId, int objId); // allocate the component first

		void RemoveComponent(ComponentManager::ContainerID compId, int objId);

		void UnBuildObject(int objId);

		void* getComponent(ComponentManager::ContainerID compId, int objId);

		class Iterator
		{
			friend ComponentSetManager; //

			ContainerID containerId; // id of the container iterating thru
			ComponentSetManager* compSetMgr; // the component set mgr this belongs to

			ComponentMemoryManager::MemConIterator memItr;

		public:
			// ctor
			Iterator() :
				containerId(-1),
				compSetMgr(nullptr)
			{
			}

			Iterator(const Iterator& itr) = default;

			// copy
			Iterator& operator=(Iterator& itr) = default;
			Iterator& operator=(Iterator&& itr) = default;

			// comparison
			bool operator==(Iterator& itr);
			bool operator!=(Iterator& itr);

			// deref
			void* operator*();

			// increment
			Iterator& operator++();
			Iterator operator++(int); // post fix // slower

		private:
			//
		};

		void* getComponent(ComponentManager::ContainerID compId, Iterator itr);

		class Entity
		{
			friend ComponentSetManager;

			ComponentSetManager* compSetMgr; // component set obj belongs to
			int objId;

		public:
			Entity(ComponentSetManager* csm, int oid);

			void* getComponent(int compId);
		};

		Iterator begin(ContainerID comT);
		Iterator end(ContainerID comT);

		int getObjId(Iterator itr);
		Entity getEntity(Iterator itr);

	private:

		char* getObjectComponent(ComponentManager::ContainerID compId, int objId);

	};

	class ComponentSet
	{
	//public:
		friend ComponentSetFactory; // allows factory to build
		friend ComponentSetManager; // allows factory to build
		friend ComponentManager;

		ComponentMemoryManager cmm; // contains container for object and containers for components
		ComponentManager::ContainerID objContainerId; // container id for obj
		std::vector<ComponentManager::ContainerID> componentContainerIDs; // container id for comp
		size_t objSize; // sizeof of the obj class -> [obj] [Components] * componentSize

		//std::map<ComponentManager::ContainerID, std::map<int, int>> compObjIndMap; // maps comp to obj id // O(log n)

		class ObjectData
		{
		//public:
			friend ComponentSetFactory; // allows factory to build
			friend ComponentSetManager; // allows factory to build

			int objId; // unique id of the object

			struct ComponentData // representation of a component
			{
				ComponentManager::ContainerID containerId; // id of the container
				int containerIndex; // index of component within the container
				void* compPtr; // pointer to the component
			};
		};

	public:
		ComponentSet() :
			cmm(),
			objContainerId(-1),
			componentContainerIDs(),
			objSize(0)
		{
		}
	};


	////

	std::map<ContainerID, ComponentSet*> ComponentSets;

	void AddComponentSet(ComponentSet* compSet); // adds teh component set to the manager

	// 

	//void Init();

	//void Load();

	//void Update();

	//void Unload();

	void Free();

	////













	///////////////////
	// Test functions

private:

	class IteratorM
	{
		friend ComponentManager;
		ComponentMemoryManager::MemConIterator mci;
	public:
		IteratorM() = default;
		IteratorM(const IteratorM& itr) = default;

		// copy
		IteratorM& operator=(IteratorM& itr) = default;
		IteratorM& operator=(IteratorM&& itr) = default;

		// comparison op
		bool operator==(IteratorM& itr);
		bool operator!=(IteratorM& itr);

		// other op
		char* operator*();
		IteratorM& operator++();
		IteratorM operator++(int); // post fix // slower
	};

	IteratorM end(ContainerID comT);
	IteratorM begin(ContainerID comT);

	ContainerID createNewComponentType(ContainerSettings set);

	char* getElementAt(ContainerID comT, int index);

	int insertIntoContainer(ContainerID comT, char* obj);
	void removeFromContainer(ContainerID comT, int index);
	void removeFromContainer(IteratorM& itr);
	void freeAll();

public:
	// test fn
	void test0(); 
	void test1();
	void test2();
};










