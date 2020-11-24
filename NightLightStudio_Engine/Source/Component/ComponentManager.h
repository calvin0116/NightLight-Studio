#pragma once


#include "../Core/MySystem.h"
#include "../../framework.h"

#include "ComponentMemoryManager.h"
#include <vector>
#include <map>
#include <typeinfo>

#include "LocalVector.h"



struct ISerializable;
//class ComponentTransform;

namespace NS_COMPONENT
{


// max children per parent // this can be technically be dynamic, but I will smash my head against the wall
#define MAX_CHILDREN 32

// this works on the assumption that IDRANGE < number of entities in a compset at any time
constexpr size_t IDRANGE = 1000000;        // <--| change these two
constexpr double RATIO_RT = 0.75;          // <--|
constexpr double RATIO_CH = 1 - RATIO_RT;  
constexpr size_t IDRANGE_RT = (size_t) (IDRANGE * RATIO_RT);
constexpr size_t IDRANGE_CH = (size_t) (IDRANGE * RATIO_CH);

// unique id allocation - rt is root entity - ch is child entites - they are in different containers so have different index
//  <-------- compset0 --------->    <-------- compset1 --------->    <-------- compset2 --------->
//  [          IDRANGE          ]    [          IDRANGE          ]    [          IDRANGE          ]
//  [ IDRANGE_RT ] [ IDRANGE_CH ]    [ IDRANGE_RT ] [ IDRANGE_CH ]    [ IDRANGE_RT ] [ IDRANGE_CH ]
//  change RATIO_RT to determine the size ratio of IDRANGE_RT : IDRANGE_CH

enum COMPONENTSETNAMES
{
	COMPONENT_MAIN = 0,
	COMPONENT_UI,
	COMPONENT_PREFABS,


	COMPONENT_END
};

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
//						template<typename T>
//						void RemoveComponent(int entityId)
//
//						// free entity // removes entity and all its decendants // recursive
//						void FreeEntity(int uid);
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
//                              // get uid of the entity
//								int getId();
//
//                              // generation of the entity root is 0
//								int getGeneration();
//
//                              // number of children this entity has
//								int getNumChildren();
//
//                              // number of decendants this entity has // WIP !!!
//								int getNumDecendants();
//
//                              // make a child that belongs to this entity
//								Entity makeChild(); // (<_<)
//
//                              // get the parent id of this entity // root entity returns -1
//								int getParentId();
//
//                              // gets a container of child uids
//								ChildContainerT* getChildren();
//
//						// begin and end iterators
//						template<typename T>
//						Iterator begin()
//						template<typename T>
//						Iterator end()
//                      // ComponentIteratorState::ITR_BOTH   // Iterate both root and child entities
//                      // ComponentIteratorState::ITR_ROOT   // Iterate both root only
//                      // ComponentIteratorState::ITR_CHILD  // Iterate both child only
//						template<typename T>
//						Iterator begin(ComponentIteratorState)
//						template<typename T>
//						Iterator end(ComponentIteratorState)
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

class ComponentManager : public MySystem, public Singleton<ComponentManager>
{
	friend Singleton<ComponentManager>;

public:
	typedef int ContainerID;
	//typedef int ComponentSetID;
	typedef ComponentMemoryManager::ComponentTypeSettings ContainerSettings;

	typedef LocalVector<int, MAX_CHILDREN> ChildContainerT;

	ComponentMemoryManager cmm;

	//class ComponentSet; // fwd decl
	class ComponentSetFactory; 
	class ComponentSetManager; 

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

		int unknown_ent_id;

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
			unknown_ent_id(0),
			objSize(0)
		{
		}
	};


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
		class EntityHandle; // fwd decl
		ComponentSet* compSet; // it is bound to a component set
	public:
		ComponentSetManager() = default;
		ComponentSetManager(ComponentSet* inCompSet) :
			compSet(inCompSet)
		{
		}

		//Storing of entity name according to object id
		std::map<int, std::string> EntityName;
		// build a new entity
		// returns entity id
		// Added default naming as Entity_<id>
		EntityHandle BuildEntity(std::string name = "Entity_");
	private:

		// build a new child Entity
		// 
		int BuildChildObject();

		// attach component to the entity // using container id // helper
		void* AttachComponent(ComponentManager::ContainerID compId, int entityId, const void* newComp);

		// void* AttachComponent(ManagerComponent::ContainerID compId, int objId); // allocate the component first

		// get component // using container id
		void* getComponent(ComponentManager::ContainerID compId, int entityId);

	public:
		// attach component to the entity // WARNING !!! DOES NOT check if the entity exists if the wrong entity id is passed in the behaviour is undefined
		template<typename T>
		T* AttachComponent(EntityHandle ent, const T& newComp_r)
		{
			const T* newComp = &newComp_r;

			int entityId = ent.objId;

			//static_assert(std::is_standard_layout_v<T>, "Component must be standard layout to memcpy the component");
			const std::type_info& tinf = typeid(T);
			
			//entityId -= compSet->idIndexModifier;
			if (entityId - compSet->idIndexModifier >= (int)IDRANGE_RT)
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

		template<typename T>
		T* AddComponent(EntityHandle ent)
		{
			//T comp; // not efficient but future me can deal with it
			//return compSetMgr->AttachComponent<T>(*this, comp);

			// not efficient but future me can deal with it
			//T* comp = reinterpret_cast<T*>(malloc(sizeof(T)));
			//if (comp == nullptr) throw;
			//memset(comp, 0, sizeof(T));
			//T* returnComp = AttachComponent<T>(ent, *comp);
			//free(reinterpret_cast<void*>(comp));

			//T* returnComp = compSetMgr->AttachComponent<T>(ent, T());

			//return returnComp;

			return ent.AddComponent<T>();
		}

	private:
		// remove component from entity // helper
		void RemoveComponent(ComponentManager::ContainerID compId, int entityId);

	public:
		template<typename T>
		void RemoveComponent(int entityId)
		{
			const std::type_info& tinf = typeid(T);

			//entityId -= compSet->idIndexModifier;
			if (entityId - compSet->idIndexModifier >= (int)IDRANGE_RT)
			{
				auto find = compSet->hashConIdMapChilds.find(tinf.hash_code());
				if (find == compSet->hashConIdMapChilds.end())
					throw; // gg
				RemoveComponent((*find).second, entityId); // <- pass in index, not uid
			}
			else
			{
				auto find = compSet->hashConIdMap.find(tinf.hash_code());
				if (find == compSet->hashConIdMap.end())
					throw; // gg
				RemoveComponent((*find).second, entityId); // <- pass in index, not uid
			}
		}

	private:
		// free entity // helper
		void UnBuildObject(int objId); // this removes 1 entity
	public:
		void FreeEntity(int uid); // this removes 1 entity and all descendants, leaf first

		// Iterator to iterate component container // !!! USE A TYPEDEF !!!
		class Iterator
		{
			friend ComponentSetManager; //
		public:
			enum class IteratorState
			{
				ITR_GG,
				ITR_ROOT,
				ITR_CHILD,
				ITR_BOTH
			};
		private:

			ComponentSetManager* compSetMgr; // the component set mgr this belongs to

			IteratorState itrDoState;
			IteratorState itrCurrState;

			ContainerID containerId; // id of the container iterating thru
			ContainerID containerIdChild;

			ComponentMemoryManager::MemConIterator memItr; // le iterator
			ComponentMemoryManager::MemConIterator memItrChild;

			int endRtObjIndex;

		public:
			// ctor
			Iterator() :
				compSetMgr(nullptr),
				itrDoState(IteratorState::ITR_GG),
				itrCurrState(IteratorState::ITR_GG),
				containerId(-1),
				containerIdChild(-1),
				endRtObjIndex(-1)
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
			int con = -1;
			if (itr.itrCurrState == Iterator::IteratorState::ITR_ROOT)
			{
				auto find = compSet->hashConIdMap.find(tinf.hash_code());
				if (find == compSet->hashConIdMap.end())
					throw; // gg
				con = (*find).second;
				if (itr.containerId == (*find).second)
				{
					return reinterpret_cast<T*>(*itr); // faster no lookup
				}
			}
			else
			{
				auto find = compSet->hashConIdMapChilds.find(tinf.hash_code());
				if (find == compSet->hashConIdMapChilds.end())
					throw; // gg
				con = (*find).second;
				if (itr.containerIdChild == (*find).second)
				{
					return reinterpret_cast<T*>(*itr); // faster no lookup
				}
			}

			return reinterpret_cast<T*>(getComponent(con, itr));
		}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ComponentSetManager::EntityContainer
// to enable this ->
//			for (Entity ent : entity.getEntityContainer())
//			{
//
//			}
//
//
	public:
		class EntityContainer
		{
			ComponentSetManager* compSetMgr; // component set obj belongs to
			bool isChild;
		public:

			EntityContainer(ComponentSetManager* _compSetMgr, bool _isChild = false)
				: compSetMgr(_compSetMgr), isChild(_isChild) {}

			struct EntityContainerIterator
			{
				friend EntityContainer;
				friend EntityHandle;

				ComponentSetManager* compSetMgr;
				ComponentMemoryManager::MemConIterator memItr;
				bool isChild;

			public:
				EntityHandle operator*()
				{

					ComponentManager::ComponentSet::ObjectData* currentObjdata = reinterpret_cast<ComponentManager::ComponentSet::ObjectData*>(*memItr);
					int hid = currentObjdata->objId + compSetMgr->compSet->idIndexModifier;
					if (isChild) hid += IDRANGE_RT;
					return EntityHandle(compSetMgr, hid);
				}

				void operator++()
				{
					++memItr;
				}

				bool operator!=(EntityContainerIterator& ent)
				{
					return memItr != ent.memItr;
				}
			};


			EntityContainerIterator begin()
			{
				EntityContainerIterator itr;
				itr.compSetMgr = compSetMgr;
				itr.isChild = isChild;
				if (isChild)
				{
					itr.memItr = compSetMgr->compSet->cmm.begin(compSetMgr->compSet->objContainerIdChilds);
				}
				else
				{
					itr.memItr = compSetMgr->compSet->cmm.begin(compSetMgr->compSet->objContainerId);
				}
				return itr;
			}

			EntityContainerIterator end()
			{
				EntityContainerIterator itr;
				itr.compSetMgr = compSetMgr;
				itr.isChild = isChild;
				if (isChild)
				{
					itr.memItr = compSetMgr->compSet->cmm.end(compSetMgr->compSet->objContainerIdChilds);
				}
				else
				{
					itr.memItr = compSetMgr->compSet->cmm.end(compSetMgr->compSet->objContainerId);
				}
				return itr;
			}
		};

		EntityContainer getEntityContainer()
		{
			return EntityContainer(this);
		}

// ComponentSetManager::EntityComponentContainer END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// an abstraction of the entity
		class EntityHandle
		{
			friend ComponentSetManager;

			ComponentSetManager* compSetMgr; // component set obj belongs to
			int objId; // Entity Id

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EntityComponentContainer
// to enable this ->
//			for (ISerializable& comp : entity.getEntityComponentContainer())
//			{
//
//			}
//
//
		public:
			class EntityComponentContainer
			{
				ComponentSetManager* compSetMgr; // component set obj belongs to
				int objId; // Entity Id
				bool isChild;
			public:

				EntityComponentContainer(ComponentSetManager* _compSetMgr, int _objId, bool _isChild = false)
					: compSetMgr(_compSetMgr), objId(_objId), isChild(_isChild) {}

				struct EntityComponentContainerIterator
				{
					friend EntityComponentContainer;

					ComponentSet::ObjectData::ComponentData* currentCompdata; // pointer to the data
					int currentCompInd; // current index
					int totalNumComp; // unnecessary var prob // maybe not


				private:
					void increment()
					{
						//void* voiddat = currentCompdata + 1;
						//char* chdat = reinterpret_cast<char*>(currentCompdata); 
						//chdat += sizeof(ComponentSet::ObjectData::ComponentData);
						++currentCompInd;
						++currentCompdata;
					}

				public:
					ISerializable* operator*()
					{
						return reinterpret_cast<ISerializable*>(currentCompdata->compPtr);
					}

				private:
					void incrementToNonEmptyElement()
					{
						//if (operator*() == nullptr || operator*() == (void*)0xFFFFFFFFFFFFFFFF)
						//{
						//}

						while (operator*() == nullptr || operator*() == (void*)0xFFFFFFFFFFFFFFFF)
						{
							if (currentCompInd != totalNumComp)
							{
								increment();
							}
							else
								break;
						}
					}

				public:
					void operator++()
					{
						increment();
						incrementToNonEmptyElement();
					}

					bool operator!=(EntityComponentContainerIterator& ent)
					{
						return currentCompInd != ent.currentCompInd;
					}
				};


				EntityComponentContainerIterator begin()
				{
					int objInd = objId - compSetMgr->compSet->idIndexModifier;
					char* obj;
					if (isChild)
					{
						obj = compSetMgr->compSet->cmm.getElementAt(compSetMgr->compSet->objContainerIdChilds, objInd - IDRANGE_RT);
					}
					else
					{
						obj = compSetMgr->compSet->cmm.getElementAt(compSetMgr->compSet->objContainerId, objInd);
					}

					// obj building
					//size_t size = sizeof(ManagerComponent::ComponentSet::Object);
					//size += sizeof(ManagerComponent::ComponentSet::Object::Component) * compSet->componentContainerIDs.size();

					obj += sizeof(ComponentManager::ComponentSet::ObjectData);

					EntityComponentContainerIterator returnItr;
					
					// find the no of components
					//returnItr.totalNumComp = compSetMgr->compSet->componentContainerIDs.size() / 2;
					returnItr.totalNumComp = (int)compSetMgr->compSet->hashConIdMap.size();

					if (returnItr.totalNumComp != (compSetMgr->compSet->componentContainerIDs.size() / 2)) throw; // shld be the same
					
					// set current index
					returnItr.currentCompInd = 0; 
					// set pointer
					returnItr.currentCompdata = reinterpret_cast<ComponentSet::ObjectData::ComponentData*>(obj);

					returnItr.incrementToNonEmptyElement();

					return returnItr;
				}

				EntityComponentContainerIterator end()
				{
					EntityComponentContainerIterator returnItr;
					returnItr.totalNumComp = (int)compSetMgr->compSet->hashConIdMap.size();
					returnItr.currentCompInd = returnItr.totalNumComp; // points to element after last
					returnItr.currentCompdata = nullptr; // dunnid unless theres operator--
					return returnItr;
				}
			};

			EntityComponentContainer getEntityComponentContainer()
			{
				return EntityComponentContainer(compSetMgr, objId);
			}

		private:
// EntityComponentContainer END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		public:
			// ctor
			EntityHandle(ComponentSetManager* csm, int oid);

			// default ctor
			EntityHandle() :
				compSetMgr(nullptr), 
				objId(-1)
			{}

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

			template<typename T>
			EntityHandle AttachComponent(T& comp)
			{
				compSetMgr->AttachComponent<T>(*this, comp);
				return *this;
			}

			template<typename T>
			EntityHandle AttachComponent(T&& comp = T())
			{
				compSetMgr->AttachComponent<T>(*this, comp);
				return *this;
			}

			template<typename T>
			T* AddComponent()
			{
				//T comp; // not efficient but future me can deal with it
				//return compSetMgr->AttachComponent<T>(*this, comp);

				// not efficient but future me can deal with it
				//T* comp = reinterpret_cast<T*>(malloc(sizeof(T)));
				//if (comp == nullptr) throw;
				//memset(comp, 0, sizeof(T));
				//T* returnComp = compSetMgr->AttachComponent<T>(*this, *comp);
				//free(reinterpret_cast<void*>(comp));


				T* returnComp = compSetMgr->AttachComponent<T>(*this, T());

				return returnComp;
			}

			template<typename T>
			void RemoveComponent()
			{
				compSetMgr->RemoveComponent<T>(objId);
			}

			void RemoveEntity()
			{
				compSetMgr->FreeEntity(objId);
				// add checks?
			}


			// param tgt_compSetMgr is the target set to copy to
			// param entityName is the name of the new entity
			ENGINE_API EntityHandle Copy(ComponentManager::ComponentSetManager* tgt_compSetMgr, std::string entityName = "Entity_");

			///////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////
			// parent and child entities

		private:
			char* getObj();

		public:

			int getId();

			int getGeneration();

			int getNumChildren();

			int getNumDecendants();

			EntityHandle makeChild(); // (<_<)

			int getParentId();

			ChildContainerT* getChildren(); // gets a container of child uids

			///////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////
		};

	private:
		// begin and end iterators // using container id // helper
		Iterator begin(ContainerID comT, ContainerID comTC = -1);
		Iterator end(ContainerID comT, ContainerID comTC = -1);
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

		// begin and end iterators
		template<typename T>
		Iterator begin(Iterator::IteratorState its)
		{
			const std::type_info& tinf = typeid(T);
			switch (its)
			{
			case Iterator::IteratorState::ITR_ROOT:
				{
					auto find = compSet->hashConIdMap.find(tinf.hash_code());
					if (find == compSet->hashConIdMap.end())
						throw; // gg
					return begin((*find).second);
					break;
				}
			case Iterator::IteratorState::ITR_CHILD:
				{
					auto findC = compSet->hashConIdMapChilds.find(tinf.hash_code());
					if (findC == compSet->hashConIdMapChilds.end())
						throw; // gg
					return begin(-1, (*findC).second);
					break;
				}
			case Iterator::IteratorState::ITR_BOTH:
				{
					auto find = compSet->hashConIdMap.find(tinf.hash_code());
					if (find == compSet->hashConIdMap.end())
						throw; // gg
					auto findC = compSet->hashConIdMapChilds.find(tinf.hash_code());
					if (findC == compSet->hashConIdMapChilds.end())
						throw; // gg
					return begin((*find).second, (*findC).second);
					break;
				}
			}
			throw;
		}

		template<typename T>
		Iterator end(Iterator::IteratorState its)
		{
			const std::type_info& tinf = typeid(T);
			switch (its)
			{
			case Iterator::IteratorState::ITR_ROOT:
			{
				auto find = compSet->hashConIdMap.find(tinf.hash_code());
				if (find == compSet->hashConIdMap.end())
					throw; // gg
				return end((*find).second);
				break;
			}
			case Iterator::IteratorState::ITR_CHILD:
			{
				auto findC = compSet->hashConIdMapChilds.find(tinf.hash_code());
				if (findC == compSet->hashConIdMapChilds.end())
					throw; // gg
				return end(-1, (*findC).second);
				break;
			}
			case Iterator::IteratorState::ITR_BOTH:
			{
				auto find = compSet->hashConIdMap.find(tinf.hash_code());
				if (find == compSet->hashConIdMap.end())
					throw; // gg
				auto findC = compSet->hashConIdMapChilds.find(tinf.hash_code());
				if (findC == compSet->hashConIdMapChilds.end())
					throw; // gg
				return end((*find).second, (*findC).second);
				break;
			}
			}
			throw;
		}

		// Get Entity and Entity Id
		int getObjId(Iterator itr);
		EntityHandle getEntity(Iterator itr);
		EntityHandle getEntity(int uid);


		//class ComponentTransform;
		EntityHandle getEntity(std::string str);


		// get component using entity name
		ComponentManager::ComponentSetManager::EntityHandle getEntityUsingEntName(std::string str);


		std::vector<EntityHandle> getEntityTagContainer(std::string str);


	private:

		char* getObjectComponent(ComponentManager::ContainerID compId, int objId, bool isChild = false);

	};
	
	//////////////////////////
	// ComponentManager vars
	std::map<ContainerID, ComponentSet*> ComponentSets;

	std::map<COMPONENTSETNAMES, ComponentSetManager> ComponentSetManagers;


	//
	//////////////////////////

	void AddComponentSet(COMPONENTSETNAMES id, ComponentSet* compSet); // adds teh component set to the manager

	ComponentSetManager* getComponentSetMgr(COMPONENTSETNAMES id);

	// add components here
	void ComponentCreation();

	void TestComponents();

	//// virtual fns for system calls
    // go check ISystem.h for virtual fns

	void Init() override;

	void Load() override {} ;

	void Free() override;

	void Exit() override;

	// clear container // param int container id
	void Clear(COMPONENTSETNAMES id);

	// clear all containers
	void Clear();

	////













////////////////////////////////////////////////
// Test functions // depreciated, sort of anyway
//
//private:
//
//	class IteratorM
//	{
//		friend ComponentManager;
//		ComponentMemoryManager::MemConIterator mci;
//	public:
//		IteratorM() = default;
//		IteratorM(const IteratorM& itr) = default;
//
//		// copy
//		IteratorM& operator=(IteratorM& itr) = default;
//		IteratorM& operator=(IteratorM&& itr) = default;
//
//		// comparison op
//		bool operator==(IteratorM& itr);
//		bool operator!=(IteratorM& itr);
//
//		// other op
//		char* operator*();
//		IteratorM& operator++();
//		IteratorM operator++(int); // post fix // slower
//	};
//
//	IteratorM end(ContainerID comT);
//	IteratorM begin(ContainerID comT);
//
//	ContainerID createNewComponentType(ContainerSettings set);
//
//	char* getElementAt(ContainerID comT, int index);
//
//	int insertIntoContainer(ContainerID comT, char* obj);
//	void removeFromContainer(ContainerID comT, int index);
//	void removeFromContainer(IteratorM& itr);
//	void freeAll();
//
//public:
	// test fn
	// depreciated
	//void test0(); 
	//void test1();
	//void test2();
};


} // NS



#include "ComponentManagerTypedefs.h"



