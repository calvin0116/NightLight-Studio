#pragma once
#include <vector>
#include <map> //Maybe need
#include "../glm/vec4.hpp"
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	struct Mesh
	{
		//For debug purposes
		std::string _nodeName;

		//////////////////////////////
		/// New variables for interleaving data and reducing BOs
		/// VERTEX DATA IS THE CONTROL POINTS. In Fbx SDK term.
		////////////////////////////// 
		struct VertexData
		{
			glm::vec3 _position;
			glm::vec2 _uv;
			glm::vec3 _normals;
		};

		//Vertex Data might not be needed
		std::vector<VertexData> _vertexDatas;
		std::vector<unsigned> _indices;

		GLuint VAO = NULL;
		GLuint VBO = NULL;
		GLuint EBO = NULL;
		
		GLuint ModelMatrixBO = NULL;

		//////////////////////////////
		/// ANIMATION STUFFS
		//////////////////////////////
		//FbxAnimStack _animStack;

		Mesh() = default;
		~Mesh() = default;
	};

	struct AnimatedMesh
	{
		//For debug purposes
		std::string _nodeName;

		//////////////////////////////
		/// New variables for interleaving data and reducing BOs
		/// VERTEX DATA IS THE CONTROL POINTS. In Fbx SDK term.
		////////////////////////////// 
		struct VertexData
		{
			glm::vec3 _position;
			glm::vec2 _uv;
			glm::vec3 _normals;
			glm::ivec4 _boneID;
			glm::vec4 _boneWeights;

			void AddBoneData(int boneID, float boneWeight)
			{
				for (int i = 0; i < 4; ++i)
				{
					if (_boneWeights[i] == 0.0f)
					{
						_boneID[i] = boneID;
						_boneWeights[i] = boneWeight;
						return;
					}
				}
			}
		};

		//Vertex Data might not be needed
		std::vector<VertexData> _vertexDatas;
		std::vector<unsigned> _indices;

		GLuint VAO = NULL;
		GLuint VBO = NULL;
		GLuint EBO = NULL;

		GLuint ModelMatrixBO = NULL;

		//////////////////////////////
		/// ANIMATION STUFFS
		//////////////////////////////

		AnimatedMesh() = default;
		~AnimatedMesh() = default;
	};
}