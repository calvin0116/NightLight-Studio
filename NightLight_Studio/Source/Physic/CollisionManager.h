#pragma once
#include "ColliderComponent.h"

#include "Singleton.h"
#include "MySystem.h"

#include "Manifold.h"

class CollisionManager : public MySystem , public Singleton<CollisionManager>
{
private: 
	friend Singleton <CollisionManager>;
	//Data that needs to be collected
	//Pointer to a vector of collider component
	std::vector<ColliderComponent*>* collider_list;	//Can be changed according to needs

	//Sub Manager
	CollisionResolver* col_res;


protected:
	CollisionManager() {};
	~CollisionManager() {};

public:

	void FixedUpdate() {};

};

static CollisionManager* COLLISION_MAN = CollisionManager::GetInstance();