#pragma once


#include "..\\Math\Vector.h"
//#include "ComponentTransform.h"
//#include "ComponentRigidBody.h"
#include "..\\Collision\CollisionMathLib.h"
#include "..\\..\ISerializable.h"
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
	NlMath::Point3D colliderPosition;
	ICollider() : colliderScale(0.5f, 0.5f, 0.5), colliderPosition(0.0f, 0.0f, 0.0f) {}
	ICollider(const NlMath::Vector3D& scale, NlMath::Point3D pos) :colliderScale(scale), colliderPosition(pos) {}
};

struct PlaneCollider : public ICollider
{
	NlMath::Vector3D center;
	NlMath::Vector3D extend;
	NlMath::Vector3D rotation;
	PlaneCollider();
	PlaneCollider(const PlaneCollider& rhs);
	PlaneCollider(
		NlMath::Vector3D _point, 
		NlMath::Vector3D _extend = NlMath::Vector3D(1, 1, 1),
		NlMath::Vector3D _rotation = NlMath::Vector3D(0, 0, 0));
	PlaneCollider& operator=(const PlaneCollider& rhs);
	

	void posUpdate(ComponentTransform* transform);
};

struct SphereCollider : public ICollider
{
	NlMath::Vector3D center;
	float radius;
	SphereCollider();
	SphereCollider(const SphereCollider& rhs);
	SphereCollider(NlMath::Vector3D Point, float Radius);
	SphereCollider& operator=(const SphereCollider& rhs);

	void posUpdate(ComponentTransform* transform);
};

struct AABBCollider : public ICollider
{
	NlMath::Vector3D vecMax;
	NlMath::Vector3D vecMin;
	AABBCollider();
	AABBCollider(const AABBCollider& rhs);
	AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin);
	AABBCollider& operator=(const AABBCollider& rhs);

	void posUpdate(ComponentTransform* transform);
};

struct OBBCollider : public ICollider
{
	 NlMath::Vector3D center;
	 NlMath::Vector3D extend;
	//rotation in radiant
	 NlMath::Matrix4x4 rotation;
	
	OBBCollider();
	OBBCollider(const OBBCollider& rhs);
	OBBCollider(NlMath::Vector3D _center, 
		NlMath::Vector3D _extend = NlMath::Vector3D(1, 1, 1),
		NlMath::Vector3D _rotation = NlMath::Vector3D(0, 0, 0));
	
	OBBCollider& operator=(const OBBCollider& rhs);

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
	CapsuleCollider(const CapsuleCollider& rhs);
	CapsuleCollider(
		NlMath::Vector3D _tip, 
		NlMath::Vector3D _base, 
		float _radius = 1,
		NlMath::Vector3D _rotation = NlMath::Vector3D(0, 0, 0));

	CapsuleCollider& operator=(const CapsuleCollider& rhs);

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
		//default
		Collider() : aabb() { };
	};

	//MyVector preCollisionData;
	//MyVector newCollisionData;

private:
	////////////////////////////data/////////////////////////////////
	//collider type is used to set collider
	COLLIDERS colliderType;
	//void updateColliderSer();
public:
	void SetColliderT(COLLIDERS col);
	void SetColliderT(const char* col);
	COLLIDERS GetColliderT();

	//////////////// Collider Scales, expose to editor///////////////////////////////////
	NlMath::Point3D center;
	NlMath::Vector3D extend;
	NlMath::Vector3D rotation;

	//turn colliding responds on and off
	bool isCollidable = true;   
	// For layering
	int colliderTag = 1;
	/// /////////////////////////////////////////////////////////////////////////////

	Collider collider;
	//time of collision for dynanmic collision
	float collisionTime;


	bool isCollide = false;
	int lod = 0;
	bool isTrigger = false;

	//to check trigger and enter
	bool triggerFlag = false;
	bool prevTriggerFlag = false;

	bool collisionFlag = false;
	bool prevCollisionFlag = false;


	////////////////////////////data end/////////////////////////////////
	ComponentCollider(COLLIDERS col);
	ComponentCollider(const char * col);
	ComponentCollider(ComponentCollider& rhs);
	ComponentCollider();

	void CollisionTimeReset();

	virtual void	Read(Value& , NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override;
	virtual Value	Write();
	virtual Value& Write(Value& val) { return val; };

	ComponentCollider& operator=(const ComponentCollider& rhs);

	virtual ComponentCollider* Clone()
	{
		ComponentCollider* newcomp = new ComponentCollider();
		*newcomp = *this;
		return newcomp;
	}

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