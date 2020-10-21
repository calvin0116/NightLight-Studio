#include "ComponentGraphics.h"

ComponentGraphics::ComponentGraphics()
	: isActive{ true },
	MeshID{ NULL }
{
}

ComponentGraphics::ComponentGraphics(const unsigned& meshID)
	: isActive{ true },
	MeshID{ meshID }
{
}

void ComponentGraphics::AssignMeshID(const unsigned& meshID)
{
	MeshID = meshID;
}

ComponentGraphics::~ComponentGraphics()
{
}

void ComponentGraphics::Read()
{
}

void ComponentGraphics::Write()
{
}
