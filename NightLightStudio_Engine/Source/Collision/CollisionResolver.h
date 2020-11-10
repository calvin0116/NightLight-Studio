#pragma once
#include <list>

#include "../Math/Vector.h"
#include "../Component/ComponentRigidBody.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "Contact.cpp"
#include "../Component/ComponentManager.h"

struct TriggerEvent
{
	Entity entity1;
	Entity entity2;

	TriggerEvent() :
		entity1(nullptr, -1),
		entity2(nullptr, -1)
	{
	}
	TriggerEvent(const Entity& object1, const Entity& object2) :
		entity1(object1),
		entity2(object2)
	{
	}
};

struct CollisionEvent
{
	// Type of collision
	COLRESTYPE collisionType = COLRESTYPE::COLRESTYPE_ERR;

	//for OBB angular physics, aabb dont use this
	bool rotationEnabledEvent = false;
	Manifold manifold;

	//for AABB, obb dont use this
	SIDES colidingSide = SIDES::NO_COLLISION;
	NlMath::Vector3D collisionNormal;

	//for all colision
	ComponentRigidBody* rigid1;
	ComponentRigidBody* rigid2;

	ComponentTransform* transform1;
	ComponentTransform* transform2;
	ComponentCollider* collider1;
	ComponentCollider* collider2;

	Entity entity1;
	Entity entity2;

	CollisionEvent() : 
		entity1(nullptr, -1),
		entity2(nullptr, -1),
		rigid1(nullptr),
		rigid2(nullptr),
		transform1(nullptr),
		transform2(nullptr),
		collider1(nullptr),
		collider2(nullptr)
	{
	}

};

struct CollsionResolver
{
	std::list<CollisionEvent> colEventList;
	std::list<TriggerEvent> trigEventList;

	void addCollisionEvent(const CollisionEvent& newEvent);
	void addTriggerEvent(const TriggerEvent& newEvent);
	void addTriggerEvent(const Entity& object1, const Entity& object2);

	void resolveCollision();

	void clear();

private:
	// helper functions
	void resolveEventNormally/*like you know with the NORMAL*/(const CollisionEvent& _event);

	void resolveAABB(const CollisionEvent& _event);

	//void AABBResolve/*like you know with the NORMAL*/(const CollisionEvent& _event);
	
};