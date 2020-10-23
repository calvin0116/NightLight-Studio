#pragma once
#include "ComponentCollider.h"

#include "Components.h"

#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

ComponentCollider::ComponentCollider(COLLIDERS _col)
	: collisionTime{ FLT_MAX }, colliderType(_col)
{

}

void ComponentCollider::CollisionTimeReset()
{
	collisionTime = FLT_MAX;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//// Sphere
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

	radius = transform->_scale.x * colliderScale.x;
}

//// Sphere END
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//// AABB
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
	glm::vec3 colscale(
		transform->_scale.x * colliderScale.x,
		transform->_scale.y * colliderScale.y,
		transform->_scale.z * colliderScale.z
	);

	vecMax = transform->_position + colscale;
	vecMin= transform->_position - colscale;

	//vecMax = transform->_position + transform->_scale;
	//vecMin= transform->_position - transform->_scale;
}

//// AABB END
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//// OBB
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
	glm::vec3 colscale(
		transform->_scale.x * colliderScale.x,
		transform->_scale.y * colliderScale.y,
		transform->_scale.z * colliderScale.z
	);

	center = transform->_position;
	//extend = transform->_scale;
	extend = colscale;
	rotation = glm::radians(transform->_rotation);
}

//// OBB END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Plane
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

//// Plane END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Capsule
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
	glm::vec3 colscale(
		transform->_scale.x * colliderScale.x,
		transform->_scale.y * colliderScale.y,
		transform->_scale.z * colliderScale.z
	);

	radius = colscale.x;

	//tip = transform->_position;
	//base = transform->_position;
	//tip.y += transform->_scale.y * 0.5f;
	//base.y += -transform->_scale.y * 0.5f;

	tip = NlMath::Vector3D(0.0f, 0.5f, 0.0f);
	base = NlMath::Vector3D(0.0f, -0.5f, 0.0f);

	rotation = glm::radians(transform->_rotation);

	// get transform mtx // use mat3 ??
	glm::mat4 translate = glm::translate(glm::mat4(1.f), transform->_position);

	glm::quat quaternion(glm::radians(transform->_rotation));
	glm::mat4 rotate = glm::mat4_cast(quaternion);

	glm::mat4 scale = glm::scale(glm::mat4(1.f), colscale);

	glm::mat4 tmtx(translate * rotate * scale);

	// apply transform mtx
	tip = tmtx * tip;
	base = tmtx * base;
}

//// Capsule END
///////////////////////////////////////////////////////////////////////////////////////////////////
