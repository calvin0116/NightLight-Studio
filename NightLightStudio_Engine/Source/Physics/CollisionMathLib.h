#pragma once
//outside lib
#include <vector>

//engine lib
#include "..\\Component\ComponentTransform.h"
#include "..\\Component\ComponentCollider.h"
#include "..\\Component\ComponentRigidBody.h"
#include "..\\Math\Vector.h"



bool PointInAABB(const AABBCollider& tBox, const NlMath::Vector3D& vecPoint);

bool AABBtoAABB(const AABBCollider& tBox1, const AABBCollider& tBox2);

//return all 8 points of a cube from AABB collider 
std::vector<NlMath::Vector3D> AllPointsOf(const AABBCollider& tBox);

SIDES AABBCheckCollisionSides(const AABBCollider& tBox1, const AABBCollider& tBox2);