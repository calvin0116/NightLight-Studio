#include "ComponentRigidBody.h"

ComponentRigidBody::ComponentRigidBody() :
	_static{ true }, 
	_mass{ 0 },
	_velocity{ 0 },
	_acceleration{ 0 },
	_pos{ 0 },
	_prevPos{ 0 }
{
}

ComponentRigidBody::~ComponentRigidBody()
{
}

NlMath::Vector3D ComponentRigidBody::GetVelocity() const
{
	return _velocity;
}

NlMath::Vector3D ComponentRigidBody::SetVelocity(NlMath::Vector3D velocity)
{
	_velocity = velocity;
	return _velocity;
}

NlMath::Vector3D ComponentRigidBody::GetAcceleration() const
{
	return _acceleration;
}

NlMath::Vector3D ComponentRigidBody::SetAcceleration(NlMath::Vector3D acceleration)
{
	_acceleration = acceleration;
	return _acceleration;
}

NlMath::Vector3D ComponentRigidBody::GetPostion() const
{
	return _pos;
}

NlMath::Vector3D ComponentRigidBody::GetPreviousPosition() const
{
	return _prevPos;
}

NlMath::Vector3D ComponentRigidBody::SetPreviousPosition(NlMath::Vector3D prevPos)
{
	_prevPos = prevPos;
	return _prevPos;
}

void ComponentRigidBody::Read()
{
}

void ComponentRigidBody::Write()
{
}
