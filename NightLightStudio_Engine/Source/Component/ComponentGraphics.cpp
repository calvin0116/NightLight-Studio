#include "ComponentGraphics.h"

#include "Components.h"

ComponentGraphics::ComponentGraphics()
	: _isActive{ true },
	MeshID{ NULL }, _textureFileName{}, _textureID{ NULL }
{
}

ComponentGraphics::ComponentGraphics(const int& meshID)
	: _isActive{ true },
	MeshID{ meshID }, _textureFileName{}, _textureID{ NULL }
{
}

void ComponentGraphics::AttachMesh(const int& meshID)
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
