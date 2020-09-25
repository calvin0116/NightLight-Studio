#include "CollisionMathLib.h"

AABB::AABB() :VecMax{ 0 }, VecMin{ 0 }
{
}

AABB::AABB(NlMath::Vector3D max, NlMath::Vector3D min)
{
	VecMax = max;
	VecMin = min;
}
