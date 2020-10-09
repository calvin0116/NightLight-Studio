#pragma once
#include <vector>
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"

namespace NS_GRAPHICS
{
	struct Mesh
	{
		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec2> _uv;
		std::vector<glm::vec3> _normals;
		std::vector<glm::vec3> _faceNormals;
		std::vector<int> _indices;
		//std::vector<unsigned> _textures; // each face has its own texture

		Mesh() = default;
		~Mesh() = default;
	};
}