#pragma once
#include "ComponentCollider.h"


ComponentCollider::ComponentCollider()
	:tranformObject{ nullptr }, rigidBody{ nullptr }, collisionTime{ FLT_MAX }
{

}

void ComponentCollider::CollisionTimeReset()
{
	collisionTime = FLT_MAX;
}


SphereCollider::SphereCollider() : center{ 0 }, radius{ 0 }
{
}

SphereCollider::SphereCollider(NlMath::Vector3D Point, float Radius)
	: center{ Point }, radius{ Radius }
{
}



AABBCollider::AABBCollider() : vecMax{ 0 }, vecMin{ 0 }
{
}

AABBCollider::AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin)
	: vecMax{ VecMax }, vecMin{ VecMin }
{
}



OBBCollider::OBBCollider()
	: center(0), extend(0), rotation(0)
{
}

OBBCollider::OBBCollider(NlMath::Vector3D _center, NlMath::Vector3D _extend, NlMath::Vector3D _rotation)
	: center(_center), extend(_extend), rotation(_rotation)
{
}

PlaneCollider::PlaneCollider() :
	center(0, 0, 0), extend(1, 1, 1), rotation(0, 0, 0)
{
}

PlaneCollider::PlaneCollider(NlMath::Vector3D _point, NlMath::Vector3D _extend, NlMath::Vector3D _rotation) :
	center(_point), extend(_extend), rotation(_rotation)
{
}
