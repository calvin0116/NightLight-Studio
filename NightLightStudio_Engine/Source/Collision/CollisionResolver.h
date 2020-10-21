#pragma once
#include <list>

#include "../Math/Vector.h"
#include "../Component/ComponentRigidBody.h"
#include "../Component/ComponentCollider.h"

struct CollisionEvent
{
	//for OBB angular physics, aabb dont use this
	bool rotationEnabledEvent = false;
	NlMath::Vector3D collisionPoint;
	NlMath::Vector3D collisionNormal;

	//for AABB, obb dont use this
	SIDES colidingSide = SIDES::NO_COLLISION;

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