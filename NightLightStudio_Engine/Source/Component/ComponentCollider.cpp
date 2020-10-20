#pragma once
#include "ComponentCollider.h"



ComponentCollider::ComponentCollider(COLLIDERS _col)
	: collisionTime{ FLT_MAX }, colliderType(_col)
{

}

void ComponentCollider::CollisionTimeReset()
{
	collisionTime = FLT_MAX;
}




SphereCollider::SphereCollider() 
	:center{ 0 }, radius{ 0 }
{


}

SphereCollider::SphereCollider(NlMath::Vector3D Point, float Radius)
	: center{ Point }, radius{ Radius }
{
}

void SphereCollider::init(ComponentTransform* transform)
{
	
	center.x = transform->_position.x;
	center.y = transform->_position.y;
	center.z = transform->_position.z;

	radius = transform->_scale.x;
}




AABBCollider::AABBCollider() 
	:  vecMax{ 0 }, vecMin{ 0 }
{
}

AABBCollider::AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin)
	: vecMax{ VecMax }, vecMin{ VecMin }
{
}

void AABBCollider::init(ComponentTransform* transform)
{
	vecMax = transform->_position + transform->_scale;
	vecMin= transform->_position - transform->_scale;
}




OBBCollider::OBBCollider()
	:  center(0), extend(0), rotation(0)
{
}

OBBCollider::OBBCollider(NlMath::Vector3D _center, NlMath::Vector3D _extend, NlMath::Vector3D _rotation)
	: center(_center), extend(_extend), rotation(_rotation)
{
}

void OBBCollider::init(ComponentTransform* transform)
{
	center = transform->_nextPos;
	extend = transform->_scale;
	rotation = glm::radians(transform->_rotation);
}

PlaneCollider::PlaneCollider() :
	center(0, 0, 0), extend(1, 1, 1), rotation(0, 0, 0)
{
}

PlaneCollider::PlaneCollider(NlMath::Vector3D _point, NlMath::Vector3D _extend, NlMath::Vector3D _rotation) :
	center(_point), extend(_extend), rotation(_rotation)
{
}

void PlaneCollider::init(ComponentTransform* transform)
{
}

CapsuleCollider::CapsuleCollider()
	: tip(0, 0, 1), base(0, 0, -1), radius(1), rotation(0, 0, 0)
{
}

CapsuleCollider::CapsuleCollider(NlMath::Vector3D _tip, NlMath::Vector3D _base, float _radius, NlMath::Vector3D _rotation)
	: tip(_tip), base(_base), radius(_radius), rotation(_rotation)
{
}

void CapsuleCollider::init(ComponentTransform* transform)
{
}
