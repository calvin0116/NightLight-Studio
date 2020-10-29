#pragma once
#include "ComponentCollider.h"

#include "Components.h"

#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

ComponentCollider::ComponentCollider(COLLIDERS _col)
	: collisionTime{ FLT_MAX }, colliderType(_col)
{

}

ComponentCollider::ComponentCollider() : collisionTime{ FLT_MAX }, colliderType(COLLIDERS::AABB) // default this
{
}

void ComponentCollider::CollisionTimeReset()
{
	collisionTime = FLT_MAX;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//// Sphere
SphereCollider::SphereCollider()
	: ICollider(), center{ 0 }, radius{ 0 }
{


}

SphereCollider::SphereCollider(NlMath::Vector3D Point, float Radius)
	: ICollider(), center{ Point }, radius{ Radius }
{
}

void SphereCollider::posUpdate(ComponentTransform* transform)
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
	: ICollider(), vecMax{ 0 }, vecMin{ 0 }
{
}

AABBCollider::AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin)
	: ICollider(), vecMax{ VecMax }, vecMin{ VecMin }
{
}

void AABBCollider::posUpdate(ComponentTransform* transform)
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
	: ICollider(), center(0), extend(0), rotation(1)
{
}

OBBCollider::OBBCollider(NlMath::Vector3D _center, NlMath::Vector3D _extend, NlMath::Vector3D _rotation)
	: ICollider(), center(_center), extend(_extend), rotation(1)
{
	glm::quat Quaternion(glm::radians((glm::vec3)_rotation));
	glm::mat4 Rotate = glm::mat4_cast(Quaternion);
	rotation = Rotate;
}

void OBBCollider::posUpdate(ComponentTransform* transform)
{
	glm::vec3 colscale(
		transform->_scale.x * colliderScale.x,
		transform->_scale.y * colliderScale.y,
		transform->_scale.z * colliderScale.z
	);

	center = transform->_position;
	extend = colscale;
	glm::quat Quaternion(glm::radians(transform->_rotation));
	glm::mat4 Rotate = glm::mat4_cast(Quaternion);
	rotation = Rotate;

}

//// OBB END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Plane
PlaneCollider::PlaneCollider() 
	: ICollider(), center(0, 0, 0), extend(1, 1, 1), rotation(0, 0, 0)
{
}

PlaneCollider::PlaneCollider(NlMath::Vector3D _point, NlMath::Vector3D _extend, NlMath::Vector3D _rotation) 
	: ICollider(), center(_point), extend(_extend), rotation(_rotation)
{
}

void PlaneCollider::posUpdate(ComponentTransform* transform)
{
}

//// Plane END
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Capsule
CapsuleCollider::CapsuleCollider()
	: ICollider(), tip(0, 0, 1), base(0, 0, -1), radius(1), rotation(0, 0, 0)
{
}

CapsuleCollider::CapsuleCollider(NlMath::Vector3D _tip, NlMath::Vector3D _base, float _radius, NlMath::Vector3D _rotation)
	: ICollider(), tip(_tip), base(_base), radius(_radius), rotation(_rotation)
{
}

void CapsuleCollider::posUpdate(ComponentTransform* transform)
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

	//tip = NlMath::Vector3D(0.0f, 0.5f, 0.0f);
	//base = NlMath::Vector3D(0.0f, -0.5f, 0.0f);
	tip = NlMath::Vector3D(0.0f, 1.0f, 0.0f);
	base = NlMath::Vector3D(0.0f, -1.0f, 0.0f);

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
