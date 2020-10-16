#pragma once
#include <vector>
#include <map> //Maybe need
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	struct Mesh
	{
		//////////////////////////////
		/// New variables for interleaving data and reducing BOs
		struct VertexData
		{
			glm::vec3 position;
			glm::vec3 rgb;
			glm::vec2 uv;
			glm::vec3 normals;
			
			// Indices should be stored separately as per default
		};

		//std::vector<VertexData> vertices;

		GLuint VAO = NULL;
		GLuint VBO = NULL;
		GLuint EBO = NULL;
		GLuint modelMatrixBO = NULL;

		/// //////////////////////////

		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec2> _uv;
		std::vector<glm::vec3> _normals;
		std::vector<glm::vec3> _faceNormals; //If ever need to calculate
		std::vector<int> _indices;
		//std::vector<unsigned> _textures; // each face has its own texture

		Mesh() = default;
		~Mesh() = default;
	};
}