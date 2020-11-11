#pragma once
#include <string>
#include "ComponentCollider.h"

#include "Components.h"

#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"


void ComponentCollider::SetColliderT(COLLIDERS _col)
{
	switch (_col)
	{
	case COLLIDERS::PLANE:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.plane = PlaneCollider();
		colliderType = COLLIDERS::PLANE;
		break;
	}
	case COLLIDERS::AABB:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.aabb = AABBCollider();
		colliderType = COLLIDERS::AABB;
		break;
	}
	case COLLIDERS::SPHERE:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.sphere = SphereCollider();
		colliderType = COLLIDERS::SPHERE;
		break;
	}
	case COLLIDERS::OBB:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.obb = OBBCollider();
		colliderType = COLLIDERS::OBB;
		break;
	}
	case COLLIDERS::CAPSULE:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.capsule = CapsuleCollider();
		colliderType = COLLIDERS::CAPSULE;
		break;
	}
	}
}

void ComponentCollider::SetColliderT(const char* _col)
{
	std::string colliderName = _col;
	if (colliderName == "OBB")
	{
		colliderType = COLLIDERS::OBB;
		collider.obb = OBBCollider();
	}
	else if (colliderName == "AABB")
	{
		colliderType = COLLIDERS::AABB;
		collider.aabb = AABBCollider();
	}
	else if (colliderName == "PLANE")
	{
		colliderType = COLLIDERS::PLANE;
		collider.plane = PlaneCollider();
	}
	else if (colliderName == "CAPSULE")
	{
		colliderType = COLLIDERS::CAPSULE;
		collider.capsule = CapsuleCollider();
	}
	else if (colliderName == "SPHERE")
	{
		colliderType = COLLIDERS::SPHERE;
		collider.sphere = SphereCollider();
	}
}

COLLIDERS ComponentCollider::GetColliderT()
{
	return colliderType;
}

ComponentCollider::ComponentCollider(COLLIDERS _col)
	: collisionTime{ FLT_MAX }, colliderType(_col)
{
	SetColliderT(_col);
}

ComponentCollider::ComponentCollider(const char* col) 
	: collisionTime(FLT_MAX), colliderType(COLLIDERS::AABB)
{
	strcpy_s(ser_name, "ColliderComponent");

	SetColliderT(col);
}

ComponentCollider::ComponentCollider(ComponentCollider& rhs)
	:colliderType(rhs.colliderType), collisionTime(FLT_MAX), colliderTag(rhs.colliderTag)
{
	switch (colliderType)
	{
	case COLLIDERS::PLANE:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.plane = PlaneCollider(rhs.collider.plane);
		break;
	}
	case COLLIDERS::AABB:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.aabb = AABBCollider(rhs.collider.aabb);
		break;
	}
	case COLLIDERS::SPHERE:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.sphere = SphereCollider(rhs.collider.sphere);
		break;
	}
	case COLLIDERS::OBB:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.obb = OBBCollider(rhs.collider.obb);
		break;
	}
	case COLLIDERS::CAPSULE:
	{
		strcpy_s(ser_name, "ColliderComponent");
		collider.capsule = CapsuleCollider(rhs.collider.capsule);
		break;
	}
	}
}

ComponentCollider::ComponentCollider() : collisionTime{ FLT_MAX }, colliderType(COLLIDERS::AABB) // default this
{
	strcpy_s(ser_name, "ColliderComponent");
}

void ComponentCollider::CollisionTimeReset()
{
	collisionTime = FLT_MAX;
}

void ComponentCollider::Read(Value& val)
{
	//get collider type
	if (val.FindMember("colliderType") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
	{
		std::string colliderName = val["colliderType"].GetString();
		if (colliderName == "OBB")
		{
			colliderType = COLLIDERS::OBB;
			collider.obb = OBBCollider();
		}
		else if (colliderName == "AABB")
		{
			colliderType = COLLIDERS::AABB;
			collider.aabb = AABBCollider();
		}
		else if (colliderName == "PLANE")
		{
			colliderType = COLLIDERS::PLANE;
			collider.plane = PlaneCollider();
		}
		else if (colliderName == "CAPSULE")
		{
			colliderType = COLLIDERS::CAPSULE;
			collider.capsule = CapsuleCollider();
		}
		else if (colliderName == "SPHERE")
		{
			colliderType = COLLIDERS::SPHERE;
			collider.sphere = SphereCollider();
		}
    // Set is collidable
    if (val.FindMember("isCollidable") == val.MemberEnd())
      std::cout << "No isCollidable data has been found" << std::endl;
    else
      isCollidable = val["isCollidable"].GetBool();
	}

}

inline Value ComponentCollider::Write()
{ 
	Value val(rapidjson::kObjectType);
	switch (colliderType)
	{
	case COLLIDERS::PLANE:
		
		NS_SERIALISER::ChangeData(&val, "colliderType", rapidjson::StringRef("PLANE"));		//custom enum
		break;
	case COLLIDERS::AABB:
		NS_SERIALISER::ChangeData(&val, "colliderType", rapidjson::StringRef("AABB"));		//custom enum
		break;
	case COLLIDERS::SPHERE:
		NS_SERIALISER::ChangeData(&val, "colliderType", rapidjson::StringRef("SPHERE"));		//custom enum
		break;
	case COLLIDERS::OBB:
		NS_SERIALISER::ChangeData(&val, "colliderType", rapidjson::StringRef("OBB"));		//custom enum
		break;
	case COLLIDERS::CAPSULE:
		NS_SERIALISER::ChangeData(&val, "colliderType", rapidjson::StringRef("CAPSULE"));		//custom enum
		break;
	default:
		break;
	}
  // Other component variables
  NS_SERIALISER::ChangeData(&val, "isCollidable", isCollidable);		//Bool

	return val;

	//Value val(rapidjson::kObjectType);

	//NS_SERIALISER::ChangeData(&val, "isStatic", isStatic);		//Bool

	//Value force_val(rapidjson::kArrayType);
	//force_val.PushBack(force.x, global_alloc);
	//force_val.PushBack(force.y, global_alloc);
	//force_val.PushBack(force.z, global_alloc);

	//NS_SERIALISER::ChangeData(&val, "Force", force_val);		//Array

	//Value accel_val(rapidjson::kArrayType);
	//accel_val.PushBack(acceleration.x, global_alloc);
	//accel_val.PushBack(acceleration.y, global_alloc);
	//accel_val.PushBack(acceleration.z, global_alloc);

	//NS_SERIALISER::ChangeData(&val, "Accelaration", accel_val);	//Array


	//return val;
}

ComponentCollider& ComponentCollider::operator=(const ComponentCollider& rhs)
{
	//collider type is used to set collider
	colliderType = rhs.colliderType;
	collisionTime = FLT_MAX;
	colliderTag = rhs.colliderTag;
	isCollide = rhs.isCollide;

	switch (colliderType)
	{
	case COLLIDERS::PLANE:
	{
		collider.plane = (rhs.collider.plane);
		break;
	}
	case COLLIDERS::AABB:
	{
		collider.aabb = (rhs.collider.aabb);
		break;
	}
	case COLLIDERS::SPHERE:
	{
		collider.sphere = (rhs.collider.sphere);
		break;
	}
	case COLLIDERS::OBB:
	{
		collider.obb = (rhs.collider.obb);
		break;
	}
	case COLLIDERS::CAPSULE:
	{
		collider.capsule = (rhs.collider.capsule);
		break;
	}
	}
	return *this;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//// Sphere
SphereCollider::SphereCollider()
	: ICollider(), center{ colliderPosition }, radius{ colliderScale.x }
{


}

SphereCollider::SphereCollider(const SphereCollider& rhs)
	:center(rhs.center ), radius(rhs.radius)
{
}

SphereCollider::SphereCollider(NlMath::Vector3D Point, float Radius)
	: ICollider(), center{ Point+ colliderPosition }, radius{ Radius + colliderScale.x }
{
}

SphereCollider& SphereCollider::operator=(const SphereCollider& rhs)
{
	center = rhs.center + colliderPosition;
	radius = rhs.radius * colliderScale.x;
	return *this;
}

void SphereCollider::posUpdate(ComponentTransform* transform)
{
	center = transform->_position + colliderPosition.x;
	radius = transform->_scale.x * colliderScale.x;
}

//// Sphere END
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//// AABB
AABBCollider::AABBCollider() 
	: ICollider(), vecMax{ colliderPosition + colliderScale}, vecMin{ colliderPosition- colliderScale }
{
}

AABBCollider::AABBCollider(const AABBCollider& rhs)
	: vecMax(rhs.vecMax), vecMin(rhs.vecMin)
{
}

AABBCollider::AABBCollider(NlMath::Vector3D VecMax, NlMath::Vector3D VecMin)
	: ICollider(), vecMax{ VecMax+ colliderPosition }, vecMin{ VecMin + colliderPosition }
{
}

AABBCollider& AABBCollider::operator=(const AABBCollider& rhs)
{
	vecMax = rhs.vecMax;
	vecMin = rhs.vecMin;
	return *this;
}

void AABBCollider::posUpdate(ComponentTransform* transform)
{
	glm::vec3 colscale(
		transform->_scale.x * colliderScale.x,
		transform->_scale.y * colliderScale.y,
		transform->_scale.z * colliderScale.z
	);

	vecMax = transform->_position + colscale + colliderPosition;
	vecMin= transform->_position - colscale + colliderPosition;

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

OBBCollider::OBBCollider(const OBBCollider& rhs)
	: center(rhs.center), extend(rhs.extend), rotation(rhs.rotation)
{
}

OBBCollider::OBBCollider(NlMath::Vector3D _center, NlMath::Vector3D _extend, NlMath::Vector3D _rotation)
	: ICollider(), center(_center), extend(_extend), rotation(1)
{
	glm::quat Quaternion(glm::radians((glm::vec3)_rotation));
	glm::mat4 Rotate = glm::mat4_cast(Quaternion);
	rotation = Rotate;
}

OBBCollider& OBBCollider::operator=(const OBBCollider& rhs)
{
	center = rhs.center;
	rotation = rhs.rotation;
	extend = rhs.extend;
	return *this;
}

void OBBCollider::posUpdate(ComponentTransform* transform)
{
	glm::vec3 colscale(
		transform->_scale.x * colliderScale.x,
		transform->_scale.y * colliderScale.y,
		transform->_scale.z * colliderScale.z
	);

	center = transform->_position + colliderPosition;
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

PlaneCollider::PlaneCollider(const PlaneCollider& rhs)
	:center(rhs.center),extend(rhs.extend),rotation(rhs.rotation)
{
}

PlaneCollider::PlaneCollider(NlMath::Vector3D _point, NlMath::Vector3D _extend, NlMath::Vector3D _rotation) 
	: ICollider(), center(_point), extend(_extend), rotation(_rotation)
{
}

PlaneCollider& PlaneCollider::operator=(const PlaneCollider& rhs)
{
	center = rhs.center;
	extend = rhs.extend;
	rotation = rhs.rotation;
	return *this;
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

CapsuleCollider::CapsuleCollider(const CapsuleCollider& rhs)
	: tip(rhs.tip), base(rhs.base), radius(rhs.radius), rotation(rhs.rotation)
{
}

CapsuleCollider::CapsuleCollider(NlMath::Vector3D _tip, NlMath::Vector3D _base, float _radius, NlMath::Vector3D _rotation)
	: ICollider(), tip(_tip), base(_base), radius(_radius), rotation(_rotation)
{
}

CapsuleCollider& CapsuleCollider::operator=(const CapsuleCollider& rhs)
{
	tip = rhs.tip;
	base = rhs.base;
	radius = rhs.radius;
	rotation = rhs.rotation;
	return *this;
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
	tip = (glm::vec3)(tmtx * tip) + colliderPosition;
	base = (glm::vec3)(tmtx * base) + colliderPosition;
}

//// Capsule END
///////////////////////////////////////////////////////////////////////////////////////////////////
