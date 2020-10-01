#pragma once
#include "Components.h"
#include "..\\Math\Vector.h"
#include "ComponentTransform.h"
#include "ComponentRigidBody.h"

enum class SIDES
{
	NO_COLLISION = 0,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	FRONT,
	BACK
};

class SphereCollider;
class AABBCollider;

class ComponentCollider : public IComponent
{
	
public:
	std::vector<ComponentCollider*> preCollisionData;
	std::vector<ComponentCollider*> newCollisionData;
	ComponentTransform* tranformObject;
	ComponentRigidBody* rigidBody;
	float collisionTime;

	// For layers
	int ColliderTag = 1;

	ComponentCollider() = default;
	void CollisionTimeReset();

};

struct SphereCollider : public ComponentCollider
{
	NlMath::Vector3D point;
	float radius;
	SphereCollider();
	SphereCollider(NlMath::Vector3D Point, float Radius);
	bool CheckCollision(AABBCollider& collider);
	bool CheckCollision(SphereCollider& collider);
};

struct AABBCollider : public ComponentCollider
{
	NlMath::Vector3D vecMax;
	NlMath::Vector3D vecMin;
	AABBCollider();
	AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin);
	bool CheckCollision(AABBCollider& collider);
	bool CheckCollision(SphereCollider& collider);
};