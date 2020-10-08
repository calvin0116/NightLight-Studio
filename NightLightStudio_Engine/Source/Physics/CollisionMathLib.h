#pragma once
//outside lib
#include <vector>

//engine lib
#include "..\\Component\ComponentTransform.h"
#include "..\\Component\ComponentCollider.h"
#include "..\\Component\ComponentRigidBody.h"
#include "..\\Math\Vector.h"
#include "..\\Math\Matrix4x4.h"

enum class SIDES;
struct PlaneCollider;
struct AABBCollider;
struct SphereCollider;
struct OBBCollider;

namespace NlMath
{
	bool PointInAABB(const AABBCollider& tBox, const NlMath::Vector3D& vecPoint);

	bool AABBtoAABB(const AABBCollider& tBox1, const AABBCollider& tBox2);

	bool PlaneToPlane(const PlaneCollider& tPlane1, const PlaneCollider& tPlane2);

	bool OBBToOBB(const OBBCollider& tBox1, const OBBCollider& tBox2);

	//return all 8 points of a cube from AABB collider 
	std::vector<NlMath::Vector3D> AllPointsOf(const AABBCollider& tBox);

	SIDES AABB_AABBCollision(const AABBCollider& tBox1, const AABBCollider& tBox2);

	NlMath::Vector3D Sphere_SphereCollision(const SphereCollider& tSpr1, const SphereCollider& tSpr2);

	SIDES AABB_SphereCollision(const AABBCollider& tBox1, const SphereCollider& tSpr1, NlMath::Vector3D& circleVec);



	
}



