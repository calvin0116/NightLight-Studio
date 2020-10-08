#pragma once
#include <vector>
#include "Mesh.h"

namespace NS_GRAPHICS
{
	struct Model
	{
		std::vector<Mesh*> _meshes;

		Model() = default;
		~Model() = default;
	};
}