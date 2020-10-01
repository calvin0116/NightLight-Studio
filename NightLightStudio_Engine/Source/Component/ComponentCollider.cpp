#pragma once
#include "ComponentCollider.h"



SphereCollider::SphereCollider() : point{ 0 }, radius{ 0 }
{
}

SphereCollider::SphereCollider(NlMath::Vector3D Point, float Radius)
	: point{ Point }, radius{ Radius }
{
}

bool SphereCollider::CheckCollision(AABBCollider& collider)
{
	return false;
}

bool SphereCollider::CheckCollision(SphereCollider& collider)
{
	return false;
}


AABBCollider::AABBCollider() : vecMax{ 0 }, vecMin{ 0 }
{
}

AABBCollider::AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin)
	: vecMax{ VecMax }, vecMin{ VecMin }
{
}

bool AABBCollider::CheckCollision(AABBCollider& collider)
{
	return false;
}

bool AABBCollider::CheckCollision(SphereCollider& collider)
{
	return false;
}

ComponentCollider::ComponentCollider()
	:tranformObject{ nullptr }, rigidBody{ nullptr }, collisionTime{FLT_MAX}
{
}

void ComponentCollider::CollisionTimeReset()
{
	collisionTime = FLT_MAX;
}
