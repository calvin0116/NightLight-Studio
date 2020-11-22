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
	//for AABB, obb dont use this
	SIDES colidingSide = SIDES::NO_COLLISION;
	NlMath::Vector3D collisionNormal;


	//for OBB angular physics, aabb dont use this
	bool rotationEnabledEvent = false;
	Manifold manifold;

	//calculated during solve
	NlMath::Matrix4x4 inertia1;
	NlMath::Matrix4x4 inertia2;
	

	//for all colision, to be set on creation
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

	///////////////////////////angular physics////////////////////////////////
	unsigned int _iteration = 1;
	//bool frictionEnabled = false;

	void ComputeBasis(const NlMath::Vec3& normal, NlMath::Vec3* tangentVec1, NlMath::Vec3* tangentVec2);

	// Increasing the iteration count increases the CPU cost of simulating
	// Scene.Step(). Decreasing the iterations makes the simulation less
	// realistic (convergent). A good iteration number range is 5 to 20.
	// Only positive numbers are accepted. Non-positive and negative
	// inputs set the iteration count to 1.
	void setIteration(unsigned int iteration);

	// Enables or disables rigid body sleeping. Sleeping is an effective CPU
	// optimization where bodies are put to sleep if they don't move much.
	// Sleeping bodies sit in memory without being updated, until the are
	// touched by something that wakes them up. The default is enabled.
	//void SetAllowSleep(bool allowSleep);

	// Friction occurs when two rigid bodies have shapes that slide along one
	// another. The friction force resists this sliding motion.
	//void SetEnableFriction(bool enabled);

	NlMath::Matrix4x4 ComputeIntertia(OBBCollider* collider, ComponentRigidBody* rigid);

	void init(CollisionEvent& colEvent);
	void preSolveOBB(CollisionEvent& colEvent);
	void solveOBB(CollisionEvent& colEvent);

	float Clamp(float min, float max, float a);
};