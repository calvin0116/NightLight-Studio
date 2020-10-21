#pragma once
#include <vector>
#include <map> //Maybe need
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	//Local Path For Custom Graphics Files
	static std::string s_LocalPathName = "Resources\Mesh";
	static std::string s_MeshFileType = "mesh";

	struct Mesh
	{
		//FOR SAVING AND LOADING MIGHT NEED
		//EXAMPLE: IF fbx file named "Test1.fbx", using model loader to create a new mesh files, it 
		//loads the fbx file into our custom mesh format. We will need some way to serialize and load in therefore
		//this is needed. It will become Test1.mesh or something?
		std::string _localFileName;

		//////////////////////////////
		/// New variables for interleaving data and reducing BOs
		/// VERTEX DATA IS THE CONTROL POINTS. In Fbx SDK term.
		/// While VerticeData is the vertices data per face. Ex. 1 Cube have 8 control points
		/// but 6 faces of 6 vertices with a combined of 36 data. Normals and UVs falls within
		/// VerticeData
		////////////////////////////// 
		struct VertexData
		{
			glm::vec3 _position;
			glm::vec3 _rgb;	
			// Indices should be stored separately as per default
		};

		struct VerticeData
		{
			glm::vec2 _uv;
			glm::vec3 _vNormals;
		};

		struct MaterialData
		{
			float _diffuse;
			float _ambient;
			float _specular;
			float _emissive;
			float _shininess;
			float _reflectivity;
			float _opacity;
		};

		std::vector<VertexData> _vertexDatas;
		std::vector<VerticeData> _verticeDatas;

		GLuint VAO = NULL;
		GLuint VBO = NULL;
		GLuint EBO = NULL;
		
		GLuint ModelMatrixBO = NULL;

		//////////////////////////////

		// ** TEMPORARY BEFORE NEW DATA INTEGRATION
		GLuint CBO = NULL;			// Color Buffer object
		GLuint UVBO = NULL;

		//RGBA IN FUTURE?
		std::vector<glm::vec3> _rgb;

		//Combined into a vector of vertex datas
		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec2> _uv;
		std::vector<glm::vec3> _normals;
		std::vector<glm::vec3> _fNormals; //If ever need to calculate

		//Still needed
		std::vector<unsigned short> _indices;

		unsigned _textureFileName;
		
		//std::vector<unsigned> _textures; // each face has its own texture

		Mesh() = default;
		~Mesh() = default;
	};
}