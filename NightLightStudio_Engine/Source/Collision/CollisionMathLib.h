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
struct CapsuleCollider;

namespace NlMath
{

	Vector3D ClosestPointOnLineSegment(Vector3D segmentPointA, Vector3D segmentPointB, Vector3D CheckPoint);

	bool PointInAABB(const AABBCollider& tBox, const NlMath::Vector3D& vecPoint);

	bool AABBtoAABB(const AABBCollider& tBox1, const AABBCollider& tBox2);

	bool PlaneToPlane(const PlaneCollider& tPlane1, const PlaneCollider& tPlane2);

	bool OBBToOBB(const OBBCollider& tBox1, const OBBCollider& tBox2, Vector3D& normal);

	bool CapsuleToCapsule(const CapsuleCollider& tCap1, const CapsuleCollider& tCap2, Vector3D& normal);

	//return all 8 points of a cube from AABB collider 
	std::vector<NlMath::Vector3D> AllPointsOf(const AABBCollider& tBox);

	SIDES AABB_AABBCollision(const AABBCollider& tBox1, const AABBCollider& tBox2);

	NlMath::Vector3D Sphere_SphereCollision(const SphereCollider& tSpr1, const SphereCollider& tSpr2);

	SIDES AABB_SphereCollision(const AABBCollider& tBox1, const SphereCollider& tSpr1, NlMath::Vector3D& circleVec);

	SIDES AABBToCapsule(const AABBCollider& tCap1, const CapsuleCollider& tCap2, NlMath::Vector3D& nromal);

	bool CapsuleToSphere(const CapsuleCollider& tCap1, const SphereCollider& tSpr2, Vector3D& normal);


	// some 2d checks we may or may not need in the future
	// we can change them to 3d if we need them

	bool Point_Rectangle_2D(Vec2 point, Vec2 rectVertA, Vec2 rectVertB, Vec2 rectVertC, Vec2 rectVertD);

	bool Line_Circle_2D(Vec2 circleCenter, float circleRadius, Vec2 rectVertA, Vec2 rectVertB);
	
	bool Circle_Rectangle_2D(Vec2 circleCenter, float circleRadius, Vec2 rectVertA, Vec2 rectVertB, Vec2 rectVertC, Vec2 rectVertD);
}



