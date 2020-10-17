#pragma once
#include <vector>
#include "Mesh.h"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	struct Model
	{
		GLuint ModelMatrixBO = NULL; // Buffer Object ID for Model Matrix

		std::vector<Mesh*> _meshes;

		Model() = default;
		~Model() = default;
	};
}