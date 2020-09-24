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

void ComponentPhysics::Read()
{
}

void ComponentPhysics::Write()
{
}
