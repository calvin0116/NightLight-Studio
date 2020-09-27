#pragma once
#include "Components.h"
#include "..\\Math\Vector.h"


class ComponentCollider : public IComponent
{
	
public:
	
};

struct SphereCollider : public ComponentCollider
{
	NlMath::Vector3D point;
	float radius;
	SphereCollider();
	SphereCollider(NlMath::Vector3D Point, float Radius);
};

struct AABBCollider : public ComponentCollider
{
	NlMath::Vector3D vecMax;
	NlMath::Vector3D vecMin;
	AABBCollider();
	AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin);
};