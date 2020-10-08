#pragma once

#include "ComponentMemoryManager.h"
#include <vector>
#include <map>
#include <typeinfo>

#include "LocalVector.h"

// max children per parent // this can be technically be dynamic, but I will smash my head against the wall
#define MAX_CHILDREN 32


// this works on the assumption that IDRANGE < number of entities in a compset at any time
//#define IDRANGE 1000000
constexpr size_t IDRANGE = 1000000;       // <--
constexpr float RATIO_RT = 0.75;          // <--
constexpr float RATIO_CH = 1 - RATIO_RT;
constexpr size_t IDRANGE_RT = IDRANGE * RATIO_RT;
constexpr size_t IDRANGE_CH = IDRANGE * RATIO_CH;

// unique id allocation - rt is root entity - ch is child entites - they are in different containers so have different index
//  <-------- compset0 --------->    <-------- compset1 --------->    <-------- compset2 --------->
//  [          IDRANGE          ]    [          IDRANGE          ]    [          IDRANGE          ]    
//  [ IDRANGE_RT ] [ IDRANGE_CH ]    [ IDRANGE_RT ] [ IDRANGE_CH ]    [ IDRANGE_RT ] [ IDRANGE_CH ]



//**! Comment more !**//

//**! I need to clean up my code !**//


//																																																	
// summary of component lifecycle	(sequence diag)																																					
//																																																	
//																												.																					
//  (program starts)																(public)(to other systems)	.	(private)(within component system)												
//				ComponentManager																				.																					
//						|																						.																					
//						|				ComponentSetFactory														.																					
//						|		(init)				|	builds  ------------------------------------------------->	ComponentSet			(has several "ComponentContainer" for each component)	
//						|							|															.			|	builds		 "ComponentContainer" -------> "ComponentContainer"		
//						|							|															.			|			(it is actually managed					|					
//						|							|	returns		ComponentSetManager	<---------------- ptr ----	ComponentSet			by ComponentMemoryManager,			|					
//						|						    X deallocated				|								.			|				the container is an					|					
//						|				(the deallocation here					|								.			|				abstraction)						|					
//						|					is to prevent erronous				|								.			|													|					
//						|					modifications to the 				|								.			|													|					
//						|					comnponentSet containers)			|								.			|													|					
//						|														|								.			|													|					
//						v														|								.			|													|					
//              ComponentManager <------------------------------ ptr in	-----	|								.			|													|					
//						|														|								.			|													|					
//						|														|								.			|													|					
//						|														v								.			|													|					
//						|											ComponentSetManager							.			v													|					
//						|						(in various systems)			|	manages ---------------------->	ComponentSet												|					
//						|														|		do	set, get, del		.			|	manages ------------------------------>	"ComponentContainer"		
//						|														|								.			|		do	set, get, del	on component ------->	|					
//						|														|								.			|													|					
//						|														|	has class Iterator ------------------>	|													|					
//						|														|		iterates a component	.			|													|					
//						|														|	has class Entity -------------------->	|													|					
//						|														|		abstraction of an  entity			|													|					
//						v														|								.			|													|					
//				ComponentManager												v								.			|													|					
//	(when system 		|	updates ----------------------------->	ComponentSetManager							.			|													|					
//		updates)		|		do defragment, etc.								|	operations on ----------------> ComponentSet												|					
//						|			(WIP)										|								.			|	---> ...										|					
//						|														|								.			|													|					
//				ComponentManager												v								.			|													|					
//	(system frees)		|	frees ------------------------------->	ComponentSetManager							.			v													|					
//						|														|	frees ------------------------> ComponentSet												|					
//						|														|								.			|	frees ---------------------------------> "ComponentContainer"		
//						|														|								.			|													|					
//						|														|								.			|													X deallocated		
//						|														|								.			X deallocated															
//						|														X deallocated					.																					
//						|																						.																					
//						|																						.																					
//	(program ends)		X deallocated																			.																					
//																																																	
//
//
//
// summary of public api functions
//
//		class ComponentManager
//
//				class ComponentSetFactory
//
//						// start building
//						void StartBuild();
//
//						// adds a container of typename T
//						template<typename T>
//						ComponentManager::ContainerID AddComponentContainer()
//
//						
//						// build the component set
//						// returns ptr to the component set
//						ComponentManager::ComponentSet* Build();
//
//				class ComponentSetManager
//
//						// attach component to the entity // WARNING !!! DOES NOT check if the entity exists if the wrong entity id is passed in the behaviour is undefined
//						template<typename T>
//						T* AttachComponent(int entityId, T* newComp)
//
//						// remove component from entity
//						void RemoveComponent(ComponentManager::ContainerID compId, int entityId);
//
//						// free entity
//						void UnBuildObject(int objId);
//
//						class Iterator
//
//								// comparison
//								bool operator==(Iterator& itr);
//								bool operator!=(Iterator& itr);
//								
//								// deref
//								void* operator*();
//
//								// increment
//								Iterator& operator++();
//								Iterator operator++(int); // post fix // slower
//
//						// get component
//						template<typename T>
//						T* getComponent(Iterator itr)
//
//						class Entity
//
//								// get component of the entity
//								template<typename T>
//								T* getComponent()
//
//						// begin and end iterators
//						template<typename T>
//						Iterator begin()
//						template<typename T>
//						Iterator end()
// 
//						// Get Entity and Entity Id
//						int getObjId(Iterator itr);
//						Entity getEntity(Iterator itr);
//                      Entity getEntity(int uid); // get entity with entity unique id
//
//
//				// component set and its dependents are all private
//				class ComponentSet
//
//						class ObjectData
//
//								struct ChildData
//
//								struct ComponentData
//


class ComponentManager
{
public:
	typedef int ContainerID;
	//typedef int ComponentSetID;
	typedef ComponentMemoryManager::ComponentTypeSettings ContainerSettings;

	typedef LocalVector<int, MAX_CHILDREN> ChildContainerT;

	ComponentMemoryManager cmm;

	class ComponentSet; // fwd decl

	class ComponentSetFactory
	{
		ComponentSet* compSet; // building this compset
	public:
		// Start building
		void StartBuild();


	private: 
		// adds a container // using container id // helper
		ComponentManager::ContainerID AddComponentContainer(ComponentMemoryManager::ComponentTypeSettings set);
		// adds a container using component size // using container id // helper
		ComponentManager::ContainerID AddComponentContainer(size_t size);
	public:

		// adds a container of typename T
		template<typename T>
		ComponentManager::ContainerID AddComponentContainer(ComponentMemoryManager::ComponentTypeSettings set)
		{
			// create new container
			set.elementSize = sizeof(T);
			ComponentManager::ContainerID newid = AddComponentContainer(set);

			// get type
			const std::type_info& tinf = typeid(T);

			// insert to type,id map
			compSet->hashConIdMap.insert(std::make_pair(tinf.hash_code(), newid));

			// create container for child
			newid = AddComponentContainer(set);
			compSet->hashConIdMapChilds.insert(std::make_pair(tinf.hash_code(), newid));

			return newid;
		}

		// adds a container of typename T
		template<typename T>
		ComponentManager::ContainerID AddComponentContainer()
		{
			ComponentMemoryManager::ComponentTypeSettings set;
			return AddComponentContainer<T>(set);
		}

		// build the component set
		// returns ptr to the component set
		ComponentManager::ComponentSet* Build();

		// ctor
		ComponentSetFactory() :
			compSet(nullptr)
		{
		}

		// dtor
		~ComponentSetFactory();
	};

	// ComponentSetManager
	// Manages Entity components
	// probably should be renamed Entities Manager !!!

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

		// build a new entity
		// returns entity id
		int BuildObject(); 

	private:

		// build a new child Entity
		// 
		int BuildChildObject();

		// attach component to the entity // using container id // helper
		void* AttachComponent(ComponentManager::ContainerID compId, int entityId, void* newComp);

		// void* AttachComponent(ManagerComponent::ContainerID compId, int objId); // allocate the component first

		// get component // using container id
		void* getComponent(ComponentManager::ContainerID compId, int entityId);

	public:
		// attach component to the entity // WARNING !!! DOES NOT check if the entity exists if the wrong entity id is passed in the behaviour is undefined
		template<typename T>
		T* AttachComponent(int entityId, T* newComp)
		{
			const std::type_info& tinf = typeid(T);
			
			//entityId -= compSet->idIndexModifier;
			if (entityId - compSet->idIndexModifier >= IDRANGE_RT)
			{
				auto find = compSet->hashConIdMapChilds.find(tinf.hash_code());
				if (find == compSet->hashConIdMapChilds.end())
					throw; // gg
				return reinterpret_cast<T*>(
					AttachComponent((*find).second, entityId, newComp) // <- pass in index, not uid
					);
			}
			else
			{
				auto find = compSet->hashConIdMap.find(tinf.hash_code());
				if (find == compSet->hashConIdMap.end())
					throw; // gg
				return reinterpret_cast<T*>(
					AttachComponent((*find).second, entityId, newComp) // <- pass in index, not uid
					);
			}
		}

		// remove component from entity
		void RemoveComponent(ComponentManager::ContainerID compId, int entityId);
		// TODO for child

		// free entity
		void UnBuildObject(int objId);
		// TODO for child

		// Iterator to iterate component container // !!! USE A TYPEDEF !!!
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

		// get the component // helper
		void* getComponent(ComponentManager::ContainerID compId, Iterator itr);

	private:
		// get component // using container id // helper
		template<typename T>
		T* getComponent(ComponentManager::ContainerID compId, Iterator itr)
		{
			return reinterpret_cast<T*>(getComponent(compId, itr));
		}

	public:
		// get component
		template<typename T>
		T* getComponent(Iterator itr)
		{
			const std::type_info& tinf = typeid(T);

			auto find = compSet->hashConIdMap.find(tinf.hash_code());
			if (find == compSet->hashConIdMap.end())
				throw; // gg

			if (itr.containerId == (*find).second)
			{
				return reinterpret_cast<T*>(*itr); // faster no lookup
			}

			return reinterpret_cast<T*>(getComponent((*find).second, itr));
		}




		// an abstraction of the entity
		class Entity
		{
			friend ComponentSetManager;

			ComponentSetManager* compSetMgr; // component set obj belongs to
			int objId; // Entity Id

		public:
			// ctor
			Entity(ComponentSetManager* csm, int oid);

		private:
			// get component of the entity // using container id // helper
			void* getComponent(int compId);

		public:
			// get component of the entity
			template<typename T>
			T* getComponent()
			{
				const std::type_info& tinf = typeid(T);

				//objId -= compSetMgr->compSet->idIndexModifier;
				if (objId - compSetMgr->compSet->idIndexModifier >= IDRANGE_RT)
				{
					auto find = compSetMgr->compSet->hashConIdMapChilds.find(tinf.hash_code());
					if (find == compSetMgr->compSet->hashConIdMapChilds.end())
						throw; // gg

					return reinterpret_cast<T*>(getComponent((*find).second));
				}
				else
				{
					auto find = compSetMgr->compSet->hashConIdMap.find(tinf.hash_code());
					if (find == compSetMgr->compSet->hashConIdMap.end())
						throw; // gg

					return reinterpret_cast<T*>(getComponent((*find).second));
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////
			// parent and child entities

		private:
			char* getObj();

		public:

			int getId();

			int getNumChildren();

			int getNumDecendants();

			Entity makeChild(); // (<_<)

			int getParentId();

			ChildContainerT* getChildren(); // ???

			class IteratorChildEntity
			{

			};


			///////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////
		};

	private:
		// begin and end iterators // using container id // helper
		Iterator begin(ContainerID comT);
		Iterator end(ContainerID comT);
	public:

		// begin and end iterators
		template<typename T>
		Iterator begin()
		{
			const std::type_info& tinf = typeid(T);

			auto find = compSet->hashConIdMap.find(tinf.hash_code());
			if (find == compSet->hashConIdMap.end())
				throw; // gg

			return begin((*find).second);
		}

		template<typename T>
		Iterator end()
		{
			const std::type_info& tinf = typeid(T);

			auto find = compSet->hashConIdMap.find(tinf.hash_code());
			if (find == compSet->hashConIdMap.end())
				throw; // gg

			return end((*find).second);
		}

		// Get Entity and Entity Id
		int getObjId(Iterator itr);
		Entity getEntity(Iterator itr);
		Entity getEntity(int uid);

	private:

		char* getObjectComponent(ComponentManager::ContainerID compId, int objId, bool isChild = false);

	};

	class ComponentSet
	{
	//public:
		friend ComponentSetFactory; // allows factory to build
		friend ComponentSetManager; // allows mgr to mg
		friend ComponentManager;

		ComponentMemoryManager cmm; // contains container for object and containers for components

		ComponentManager::ContainerID objContainerId; // container id for obj
		ComponentManager::ContainerID objContainerIdChilds; // container id for obj childs

		std::vector<ComponentManager::ContainerID> componentContainerIDs; // container id for comp
		size_t objSize; // sizeof of the obj class -> [obj] [Components] * componentSize

		int idIndexModifier;

		// maps typeid to container id
		std::map<size_t, ComponentManager::ContainerID> hashConIdMap; // std::type_info.hash_code() , id
		std::map<size_t, ComponentManager::ContainerID> hashConIdMapChilds; // std::type_info.hash_code() , id

		//std::map<ComponentManager::ContainerID, std::map<int, int>> compObjIndMap; // maps comp to obj id // O(log n)

		class ObjectData
		{
		public:
			friend ComponentSetFactory; // allows factory to build
			friend ComponentSetManager; // allows factory to build

			int objId; // unique id of the object

			int parentObjId;

			struct ChildData
			{
				int generation;
				int numDecendants;
				int numChild;
				//int childIDs[MAX_CHILDREN];
				//LocalVector<int, MAX_CHILDREN> childIDs;
				ChildContainerT childIDs;
				ChildData() :
					generation(0),
					numDecendants(0),
					numChild(0),
					childIDs()
				{};
			};

			ChildData children;

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
			objContainerIdChilds(-1),
			componentContainerIDs(),
			idIndexModifier(0),
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


extern ComponentManager G_COMPMGR;








