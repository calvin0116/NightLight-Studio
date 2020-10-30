#pragma once
#include <vector>
#include <map> //Maybe need
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	//Local Path For Custom Graphics Files
	static std::string s_LocalPathName = "Resources\\Mesh\\";
	static std::string s_MeshFileType = ".mesh";

	struct Mesh
	{
		//FOR SAVING AND LOADING MIGHT NEED
		//EXAMPLE: IF fbx file named "Test1.fbx", using model loader to create a new mesh files, it 
		//loads the fbx file into our custom mesh format. We will need some way to serialize and load in therefore
		//this is needed. It will become Test1.mesh or something?
		std::string _localFileName;

		//Manual Input
		std::string _meshName;

		//////////////////////////////
		/// New variables for interleaving data and reducing BOs
		/// VERTEX DATA IS THE CONTROL POINTS. In Fbx SDK term.
		/// While FragmentData is the data needed for caluclating light and texture. 
		////////////////////////////// 
		struct VertexData
		{
			glm::vec3 _position;
			glm::vec3 _rgb;
		};

		struct FragmentData
		{
			glm::vec2 _uv;
			glm::vec3 _vNormals;
		};

		// Designer Manual Input
		struct MaterialData
		{
			glm::vec3 _diffuse;  // Base color
			glm::vec3 _ambient;  // Reflective color under ambient lighting
			glm::vec3 _specular; // Highlight color
			float _shininess;    // Scattering / radius of specularity

			MaterialData()
				: _diffuse{ 0.5f,0.5f,0.5f },
				_ambient{ 1.f, 1.f, 1.f }, 
				_specular{ 1.f,1.f,1.f },
				_shininess{ 32.f } {}

			~MaterialData() {}
		};

		//Vertex Data might not be needed
		std::vector<VertexData> _vertexDatas;
		std::vector<FragmentData> _fragmentDatas;

		MaterialData _materialData;

		//Still needed
		std::vector<unsigned short> _indices;

		GLuint VAO = NULL;
		GLuint VBO = NULL;
		GLuint EBO = NULL;
		
		GLuint ModelMatrixBO = NULL;

		

		//////////////////////////////
		/// ANIMATION STUFFS
		//////////////////////////////
		//FbxAnimStack _animStack;

		//////////////////////////////
		/// ** TEMPORARY BEFORE NEW DATA INTEGRATION
		/// OLD DATA VARIABLES
		/// #Some of the old stuff like _vertices might be used instead of _vertexDatas
		//////////////////////////////
		GLuint CBO = NULL;			// Color Buffer object
		GLuint UVBO = NULL;
		GLuint NBO = NULL; // normals buffer object

		//RGBA IN FUTURE?
		std::vector<glm::vec3> _rgb;

		//Combined into a vector of vertex datas
		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec2> _uv;
		std::vector<glm::vec3> _normals;

		//If ever need to calculate
		std::vector<glm::vec3> _fNormals;

		Mesh() = default;
		~Mesh() = default;
	};
}