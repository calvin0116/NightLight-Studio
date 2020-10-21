#pragma once


#include "..\\Math\Vector.h"
//#include "ComponentTransform.h"
//#include "ComponentRigidBody.h"
#include "..\\Collision\CollisionMathLib.h"
//#include "..\Core\MySystem.h"

class ComponentTransform;

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

enum class COLLIDERS
{
	PLANE = 0,
	AABB,
	SPHERE,
	OBB,
	CAPSULE
};

//typedef std::vector<ComponentCollider> MyVector;




struct ICollider
{

};

struct PlaneCollider : public ICollider
{
	NlMath::Vector3D center;
	NlMath::Vector3D extend;
	NlMath::Vector3D rotation;
	PlaneCollider();
	PlaneCollider(
		NlMath::Vector3D _point, 
		NlMath::Vector3D _extend = NlMath::Vector3D(1, 1, 1),
		NlMath::Vector3D _rotation = NlMath::Vector3D(0, 0, 0));

	void init(ComponentTransform* transform);
};

struct SphereCollider : public ICollider
{
	NlMath::Vector3D center;
	float radius;
	SphereCollider();
	SphereCollider(NlMath::Vector3D Point, float Radius);

	void init(ComponentTransform* transform);
};

struct AABBCollider : public ICollider
{
	NlMath::Vector3D vecMax;
	NlMath::Vector3D vecMin;
	AABBCollider();
	AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin);

	void init(ComponentTransform* transform);
};

struct OBBCollider : public ICollider
{
	 NlMath::Vector3D center;
	 NlMath::Vector3D extend;
	//rotation in radiant
	 NlMath::Vector3D rotation;
	
	OBBCollider();
	OBBCollider(NlMath::Vector3D _center, 
		NlMath::Vector3D _extend = NlMath::Vector3D(1, 1, 1),
		NlMath::Vector3D _rotation = NlMath::Vector3D(0, 0, 0));

	void init(ComponentTransform* transform);

};

struct CapsuleCollider : public ICollider
{
	NlMath::Vector3D tip;
	NlMath::Vector3D base;
	float radius;
	//rotation in radiant
	NlMath::Vector3D rotation;

	CapsuleCollider();
	CapsuleCollider(
		NlMath::Vector3D _tip, 
		NlMath::Vector3D _base, 
		float _radius = 1,
		NlMath::Vector3D _rotation = NlMath::Vector3D(0, 0, 0));

	void init(ComponentTransform* transform);
};

typedef struct ComponentCollider //: public IComponent
{
	union Collider
	{
		PlaneCollider	plane;
		SphereCollider	sphere;
		AABBCollider	aabb;
		OBBCollider		obb;
		CapsuleCollider capsule;
		Collider() {};
	};

	//MyVector preCollisionData;
	//MyVector newCollisionData;
	COLLIDERS colliderType;
	float collisionTime;
	// For layers
	int ColliderTag = 1;

	Collider collider;

	ComponentCollider(COLLIDERS col);

	void CollisionTimeReset();


}ColliderComponent;

//enum TYPE_COL
//{
//	SPHERE_COL,
//	AABB_COL
//};

//union u
//{
//	SphereCollider sc;
//	AABBCollider aabb;
//	u()
//	{};
//};
//
//struct ComponentColliderU
//{
//
//	TYPE_COL type;
//	u collider;
//
//	ComponentColliderU()
//	{
//	};
//
//	~ComponentColliderU() = default;
//};
//
//
//static void fn()
//{
//
//	ComponentColliderU col123;
//
//
//	switch (col123.type)
//	{
//	case SPHERE_COL:
//		col123.collider.sc;
//		break;
//
//
//
//	}
//
//
//}