#include "ComponentGraphics.h"

#include "Components.h"

ComponentGraphics::ComponentGraphics()
	: _isActive{ true },
	MeshID{ NULL }, _textureFileName{}, _textureID{ NULL }
{
	strcpy_s(ser_name ,"GraphicsComponent");
}

ComponentGraphics::ComponentGraphics(const int& meshID)
	: _isActive{ true },
	MeshID{ meshID }, _textureFileName{}, _textureID{ NULL }
{
	strcpy_s(ser_name, "GraphicsComponent");
}

void ComponentGraphics::AttachMesh(const int& meshID)
{
	MeshID = meshID;
}

ComponentGraphics::~ComponentGraphics()
{
}
