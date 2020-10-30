#include "ComponentRigidBody.h"

#include "Components.h"
//#include "ComponentTransform.h"

constexpr float GRAVITY_DEF = 0.01f;

ComponentRigidBody::ComponentRigidBody() :
	force(0.0f, 0.0f, 0.0f),
	acceleration(0.0f, 0.0f, 0.0f),
	velocity(0.0f, 0.0f, 0.0f),
	prevPos(0.0f, 0.0f, 0.0f),
	nextPos(0.0f, 0.0f, 0.0f),
	isStatic(true),
	mass(0.0f),
	isGravity(false),
	gravity(0.0f, -GRAVITY_DEF, 0.0f),
	forceHandle(-1) //default -1 to ininit
{}

ComponentRigidBody::~ComponentRigidBody()
{
}

