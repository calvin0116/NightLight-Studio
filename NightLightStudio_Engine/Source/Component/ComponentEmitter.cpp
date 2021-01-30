#include "ComponentEmitter.h"
#include "../Graphics/SystemEmitter.h"
#include "Components.h"
#include "ComponentManager.h"

ComponentEmitter::ComponentEmitter()
	: _isActive{ true }
{
	strcpy_s(ser_name, "EmitterComponent");
}

ComponentEmitter::ComponentEmitter(const int& emitterID)
	: _isActive{ true }
{
	strcpy_s(ser_name, "EmitterComponent");
}

void ComponentEmitter::SetEmitterID(const int& modelID)
{
}

int ComponentEmitter::GetEmitterID()
{
	return 0;
}

void ComponentEmitter::AddImage(std::string filename)
{
}

ComponentEmitter::~ComponentEmitter()
{
}

void ComponentEmitter::Read(Value&)
{
}

Value ComponentEmitter::Write()
{
	return Value();
}
