#pragma once
#include "ComponentCollider.h"



SphereCollider::SphereCollider() : point{ 0 }, radius{ 0 }
{
}

SphereCollider::SphereCollider(NlMath::Vector3D Point, float Radius)
	: point{ Point }, radius{ Radius }
{
}


AABBCollider::AABBCollider() : vecMax{ 0 }, vecMin{ 0 }
{
}

AABBCollider::AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin)
	: vecMax{ VecMax }, vecMin{ VecMin }
{
}
