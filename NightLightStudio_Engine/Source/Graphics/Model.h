#pragma once
#include <vector>
#include "Mesh.h"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	struct Model
	{
		GLuint modelMatrixBO;
		std::vector<Mesh*> _meshes;

		Model() = default;
		~Model() = default;
	};
}