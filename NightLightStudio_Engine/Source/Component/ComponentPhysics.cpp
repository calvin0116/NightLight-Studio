#include "ComponentPhysics.h"

ComponentPhysics::ComponentPhysics() :
	_static{ true }, 
	_mass{ 0 },
	_velocity{ 0 },
	_acceleration{ 0 },
	_pos{ 0 },
	_prevPos{ 0 }
{
}

ComponentPhysics::~ComponentPhysics()
{
}

NlMath::Vector3D ComponentPhysics::GetVelocity() const
{
	return _velocity;
}

NlMath::Vector3D ComponentPhysics::SetVelocity(NlMath::Vector3D velocity)
{
	_velocity = velocity;
	return _velocity;
}

NlMath::Vector3D ComponentPhysics::GetAcceleration() const
{
	return _acceleration;
}

NlMath::Vector3D ComponentPhysics::SetAcceleration(NlMath::Vector3D acceleration)
{
	_acceleration = acceleration;
	return _acceleration;
}

NlMath::Vector3D ComponentPhysics::GetPostion() const
{
	return _pos;
}

NlMath::Vector3D ComponentPhysics::GetPreviousPosition() const
{
	return _prevPos;
}

NlMath::Vector3D ComponentPhysics::SetPreviousPosition(NlMath::Vector3D prevPos)
{
	_prevPos = prevPos;
	return _prevPos;
}

void ComponentPhysics::Read()
{
}

void ComponentPhysics::Write()
{
}
