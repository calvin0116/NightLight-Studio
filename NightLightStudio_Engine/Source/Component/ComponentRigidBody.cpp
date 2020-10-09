#include "ComponentRigidBody.h"

ComponentRigidBody::ComponentRigidBody() :
	isStatic{ true }, 
	mass{ 0 },
	velocity{ 0 },
	acceleration{ 0 },
	prevPos{ 0 }
{
}

ComponentRigidBody::~ComponentRigidBody()
{
}

void ComponentRigidBody::Read()
{
}

void ComponentRigidBody::Write()
{
}