#include "SystemManager.h"
#include "Systems.h"


#include "../IO/Json/Config.h"

#include <functional>

#include "..//Component/Components.h"



#define DOTEST 1

// Do not touch
//**! Update comments please thanks
void MySystemManager::StartUp(HINSTANCE& hInstance)
{
  // === Insert your system here to get them running === //
  // === Please follow how PhysicManager is created  === // 
	//Systems[SYS_PHYSICS] = PhysicManager::GetInstance();
	
	Systems[S_PRIORITY::SP_WINDOW] = NS_WINDOW::SYS_WINDOW;
	Systems[S_PRIORITY::SP_GRAPHICS] = NS_GRAPHICS::SYS_GRAPHICS;
	Systems[S_PRIORITY::SP_INPUT] = SYS_INPUT;
	Systems[S_PRIORITY::SP_IO] = SYS_IO;
	Systems[S_PRIORITY::SP_AUDIO] = SYS_AUDIO;
	Systems[S_PRIORITY::SP_SCENEMANAGER] = NS_SCENE::SYS_SCENE_MANAGER;
	Systems[S_PRIORITY::SP_PHYSICS] = NS_PHYSICS::SYS_PHYSICS;
	Systems[S_PRIORITY::SP_COMPONENT] = SYS_COMPONENT;
	Systems[S_PRIORITY::SP_COLLISION] = NS_COLLISION::SYS_COLLISION;
  Systems[S_PRIORITY::SP_LOGIC] = NS_LOGIC::SYS_LOGIC;
	
	
#ifdef _EDITOR
	Systems[S_PRIORITY::SP_EDITOR] = SYS_EDITOR;
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// SET UP WINDOW(CLIENT) INSTANCE
	NS_WINDOW::SYS_WINDOW->SetAppInstance(hInstance);
	//// SET UP WINDOW(CLIENT) INSTANCE END
	///////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//// Add components in fn
	SYS_COMPONENT->ComponentCreation();
	//// 
	/////////////////////////////////////////////////////////////////////////////////////////////////

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


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////// sample for get entity by tag
	{
		Entity newEntity = G_ECMANAGER->BuildEntity();
		ComponentTransform newtransform;
		newtransform._entityName = "test";
		newEntity.AttachComponent<ComponentTransform>(newtransform);
	}
	{
		Entity newEntity = G_ECMANAGER->BuildEntity();
		ComponentTransform newtransform;
		newtransform._entityName = "test0";
		newEntity.AttachComponent<ComponentTransform>(newtransform);
	}
	{
		Entity newEntity = G_ECMANAGER->BuildEntity();
		ComponentTransform newtransform;
		newtransform._entityName = "test";
		newEntity.AttachComponent<ComponentTransform>(newtransform);
	}

	std::vector<Entity> entityContainer = G_ECMANAGER->getEntityTagContainer("test");

	for (Entity e : entityContainer)
	{
		int id = e.getId();
		(void)id;
	}
	//////// sample for get entity by tag END
	/////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Systems OnFirst start
	for (auto my_sys : Systems)
		my_sys.second->Load();
	// Systems OnFirst start END
	///////////////////////////////////////////////////////////////////////////////////////////////



}

void MySystemManager::FixedUpdate()
{
#ifdef _EDITOR
	int i = 0;
#endif

	for (auto my_sys : Systems)
	{
#ifdef _EDITOR
		std::vector<float>* sysUsage = SYS_EDITOR->GetSystemsUsage();

		LARGE_INTEGER start{}, end{}, elapsed{};
		LARGE_INTEGER freq{};

		if (sysUsage)
		{
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
		}
		//auto t1 = std::chrono::high_resolution_clock::now();
#endif

		my_sys.second->FixedUpdate();

#ifdef _EDITOR

		if (sysUsage)
		{
			QueryPerformanceCounter(&end);
			elapsed.QuadPart = end.QuadPart - start.QuadPart;
			elapsed.QuadPart *= 1000000;
			elapsed.QuadPart /= freq.QuadPart;

			if (sysUsage->size() > i)
				sysUsage->at(i) += (float)elapsed.QuadPart;
			else
				sysUsage->push_back((float)elapsed.QuadPart);
			++i;
		}
		//auto t2 = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		//SYS_EDITOR->GetSystemsUsage()->push_back(duration);
#endif
	}
}

void MySystemManager::Update()
{
#ifdef _EDITOR
	int i = 0;
#endif

	for (auto my_sys : Systems)
	{
#ifdef _EDITOR
		std::vector<float>* sysUsage = SYS_EDITOR->GetSystemsUsage();

		LARGE_INTEGER start{}, end{}, elapsed{};
		LARGE_INTEGER freq{};

		if (sysUsage)
		{
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
		}
		//auto t1 = std::chrono::high_resolution_clock::now();
#endif
		my_sys.second->Update();

#ifdef _EDITOR

		if (sysUsage)
		{
			QueryPerformanceCounter(&end);
			elapsed.QuadPart = end.QuadPart - start.QuadPart;
			elapsed.QuadPart *= 1000000;
			elapsed.QuadPart /= freq.QuadPart;

			if (sysUsage->size() > i)
				sysUsage->at(i) += (float)elapsed.QuadPart;
			else
				sysUsage->push_back((float)elapsed.QuadPart);
			++i;
		}
		//auto t2 = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		//SYS_EDITOR->GetSystemsUsage()->push_back(duration);
#endif
	}
}
