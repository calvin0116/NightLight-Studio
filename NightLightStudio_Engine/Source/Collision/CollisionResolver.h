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

	int collidingFrames;

	TriggerEvent() :
		entity1(nullptr, -1),
		entity2(nullptr, -1),
		collidingFrames(0)
	{
	}
	TriggerEvent(const Entity& object1, const Entity& object2) :
		entity1(object1),
		entity2(object2),
		collidingFrames(0)
	{
	}

	bool operator==(const TriggerEvent& rhs)
	{
		if (entity1.getId() == rhs.entity1.getId() && entity2.getId() == rhs.entity2.getId())
		{
			return true;
		}
		if (entity1.getId() == rhs.entity2.getId() && entity2.getId() == rhs.entity1.getId())
		{
			return true;
		}
		return false;
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

	int collidingFrames;
	bool doResolve;

	CollisionEvent() : 
		entity1(nullptr, -1),
		entity2(nullptr, -1),
		rigid1(nullptr),
		rigid2(nullptr),
		transform1(nullptr),
		transform2(nullptr),
		collider1(nullptr),
		collider2(nullptr),
		collidingFrames(0),
		doResolve(true)
	{
	}

	bool operator==(const CollisionEvent& rhs)
	{
		if (entity1.getId() == rhs.entity1.getId() && entity2.getId() == rhs.entity2.getId())
		{
			return true;
		}
		if (entity1.getId() == rhs.entity2.getId() && entity2.getId() == rhs.entity1.getId())
		{
			return true;
		}
		return false;
	}

	void operator!()
	{
		std::swap(collider1, collider2);
		std::swap(entity1, entity2);
		std::swap(inertia1, inertia2);
		std::swap(rigid1, rigid2);
		std::swap(transform1, transform2);
	}

};

struct CollsionResolver
{
	std::list<CollisionEvent> colEventList;
	std::list<TriggerEvent> trigEventList;

	void addCollisionEvent(CollisionEvent& newEvent);
	void addTriggerEvent(TriggerEvent& newEvent);
	void addTriggerEvent(const Entity& object1, const Entity& object2);

	void resolveCollision();

	void clear();

private:
	// helper functions
	void resolveEventNormally/*like you know with the NORMAL (lol)*/(CollisionEvent& _event);

	void resolveAABB(CollisionEvent& _event);

	void resolveAABB_sphere(CollisionEvent& _event, bool flip = false);

	void resolvesphere(CollisionEvent& _event);



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