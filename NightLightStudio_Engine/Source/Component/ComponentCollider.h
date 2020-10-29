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

enum class COLRESTYPE
{
	AABB_AABB,
	AABB_SPHERE,
	AABB_CAPSULE,
	//AABB_OBB,

	SPHERE_AABB,
	SPHERE_SPHERE,
	SPHERE_CAPSULE,
	//SPHERE_OBB,

	CAPSULE_AABB,
	CAPSULE_SPHERE,
	CAPSULE_CAPSULE,
	//CAPSULE_OBB,

	//OBB_AABB,
	//OBB_SPHERE,
	//OBB_CAPSULE,
	OBB_OBB,

	//PLANE?

	COLRESTYPE_ERR // error
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
	NlMath::Vector3D colliderScale;
	ICollider() : colliderScale(1.0f, 1.0f, 1.0f) {}
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

	void posUpdate(ComponentTransform* transform);
};

struct SphereCollider : public ICollider
{
	NlMath::Vector3D center;
	float radius;
	SphereCollider();
	SphereCollider(NlMath::Vector3D Point, float Radius);

	void posUpdate(ComponentTransform* transform);
};

struct AABBCollider : public ICollider
{
	NlMath::Vector3D vecMax;
	NlMath::Vector3D vecMin;
	AABBCollider();
	AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin);

	void posUpdate(ComponentTransform* transform);
};

struct OBBCollider : public ICollider
{
	 NlMath::Vector3D center;
	 NlMath::Vector3D extend;
	//rotation in radiant
	 NlMath::Matrix4x4 rotation;
	
	OBBCollider();
	OBBCollider(NlMath::Vector3D _center, 
		NlMath::Vector3D _extend = NlMath::Vector3D(1, 1, 1),
		NlMath::Vector3D _rotation = NlMath::Vector3D(0, 0, 0));

	void posUpdate(ComponentTransform* transform);

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

	void posUpdate(ComponentTransform* transform);
};

typedef struct ComponentCollider : public ISerializable//: public IComponent
{
	union Collider
	{
		PlaneCollider	 plane;
		SphereCollider	sphere;
		AABBCollider	aabb;
		OBBCollider		obb;
		CapsuleCollider capsule;
		Collider() : aabb() { };
	};

	//MyVector preCollisionData;
	//MyVector newCollisionData;
	COLLIDERS colliderType;
	float collisionTime;
	// For layers
	int ColliderTag = 1;

	Collider collider;

	ComponentCollider(COLLIDERS col);

	ComponentCollider();

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