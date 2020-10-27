#include "ComponentGraphics.h"

#include "Components.h"

ComponentGraphics::ComponentGraphics()
	: isActive{ true },
	MeshID{ NULL }, _textureFileName{}, _textureID{ NULL }
{
}

ComponentGraphics::ComponentGraphics(const unsigned& meshID)
	: isActive{ true },
	MeshID{ meshID }, _textureFileName{}, _textureID{ NULL }
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
