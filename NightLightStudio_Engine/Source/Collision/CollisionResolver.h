#pragma once
#include <list>

#include "../Math/Vector.h"
#include "../Component/ComponentRigidBody.h"
#include "../Component/ComponentCollider.h"
#include "Contact.cpp"

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
};

struct CollsionResolver
{
	std::list<CollisionEvent> colEventList;
	void addCollisionEvent(const CollisionEvent& _event);

	void resolveCollision();

	void clear();

private:
	// helper functions
	void AABBResolve/*like you know with the NORMAL*/(const CollisionEvent& _event);
	
};