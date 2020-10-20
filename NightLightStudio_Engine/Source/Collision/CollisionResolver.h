#pragma once
#include <list>

#include "../Math/Vector.h"
#include "../Component/ComponentRigidBody.h"

struct CollisionEvent
{
	//for OBB angular physics
	bool rotationEnabledEvent = false;
	NlMath::Vector3D collisionPoint;

	//for AABB
	NlMath::Vector3D collisionNormal;

	//for all colision
	ComponentRigidBody* rigid1;
	ComponentRigidBody* rigid2;
};

struct CollsionResolver
{
	std::list<CollisionEvent> colEventList;
	void addCollisionEvent(const CollisionEvent& _event);

	void resolveCollision();

	void clear();
};