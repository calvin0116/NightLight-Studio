#include "ModelManager.h"
#include "../glm/mat4x4.hpp"
#include "ModelLoader.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define DRAW_WITH_LIGHT
//#define DRAW_WITH_COMPONENTS

namespace NS_GRAPHICS
{
	ModelManager::ModelManager()
		: //meshIDs{ 0 }, 
		_modelIDs{ 0 }
	{

	}

	ModelManager::~ModelManager()
	{

	}

//	int ModelManager::AddMesh(Mesh* const mesh)
//	{
//		meshes.push_back(mesh);
//
//		return meshIDs++;
//	}
//
//	int ModelManager::AddMesh(const std::string& meshkey)
//	{
//		auto check = meshlist.find(meshkey);
//
//		if (check != meshlist.end())
//		{
//			// Make new copy of mesh
//
//			Mesh* mesh = new Mesh();
//
//			mesh->_vertices = check->second->_vertices;
//			mesh->_indices = check->second->_indices;
//			mesh->_rgb = check->second->_rgb;
//			mesh->_uv = check->second->_uv;
//			mesh->_normals = check->second->_normals;
//
//#ifdef DRAW_WITH_COMPONENTS
//			glGenVertexArrays(1, &mesh->VAO);
//			glGenBuffers(1, &mesh->VBO);
//			glGenBuffers(1, &mesh->EBO);
//			glGenBuffers(1, &mesh->CBO);
//			glGenBuffers(1, &mesh->UVBO);
//			glGenBuffers(1, &mesh->ModelMatrixBO);
//
//			glBindVertexArray(mesh->VAO);
//
//			// pos attribute
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_vertices.size(), &mesh->_vertices[0], GL_STATIC_DRAW);
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
//			glEnableVertexAttribArray(0);
//
//
//			// uv attribute
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->UVBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh->_uv.size(), &mesh->_uv[0], GL_STATIC_DRAW);
//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
//			glEnableVertexAttribArray(1);
//
//			// color attribute
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->CBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_rgb.size(), &mesh->_rgb[0], GL_STATIC_DRAW);
//			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
//			glEnableVertexAttribArray(2);
//
//			// Indices
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->_indices.size(), &mesh->_indices[0], GL_STATIC_DRAW);
//
//			// Model Matrix
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
//
//			glEnableVertexAttribArray(3);
//			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
//			glEnableVertexAttribArray(4);
//			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
//			glEnableVertexAttribArray(5);
//			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
//			glEnableVertexAttribArray(6);
//			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
//
//			glVertexAttribDivisor(3, 1);
//			glVertexAttribDivisor(4, 1);
//			glVertexAttribDivisor(5, 1);
//			glVertexAttribDivisor(6, 1);
//#endif
//
//#ifdef DRAW_WITH_LIGHT
//			glGenVertexArrays(1, &mesh->VAO);
//			glGenBuffers(1, &mesh->VBO);
//			glGenBuffers(1, &mesh->UVBO);
//			glGenBuffers(1, &mesh->NBO);
//			glGenBuffers(1, &mesh->EBO);
//			glGenBuffers(1, &mesh->ModelMatrixBO);
//
//			glBindVertexArray(mesh->VAO);
//
//			// pos attribute
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_vertices.size(), &mesh->_vertices[0], GL_STATIC_DRAW);
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
//			glEnableVertexAttribArray(0);
//
//			// uv
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->UVBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh->_uv.size(), &mesh->_uv[0], GL_STATIC_DRAW);
//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
//			glEnableVertexAttribArray(1);
//
//			// norm
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->NBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_normals.size(), &mesh->_normals[0], GL_STATIC_DRAW);
//			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
//			glEnableVertexAttribArray(2);
//
//			// Indices
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->_indices.size(), &mesh->_indices[0], GL_STATIC_DRAW);
//
//			// Model Matrix
//			glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
//			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
//
//			glEnableVertexAttribArray(3);
//			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
//			glEnableVertexAttribArray(4);
//			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
//			glEnableVertexAttribArray(5);
//			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
//			glEnableVertexAttribArray(6);
//			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
//
//			glVertexAttribDivisor(3, 1);
//			glVertexAttribDivisor(4, 1);
//			glVertexAttribDivisor(5, 1);
//			glVertexAttribDivisor(6, 1);
//#endif
//
//			return AddMesh(mesh);
//		}
//
//		// Invalid key given
//#ifdef _DEBUG
//		std::cout << "ERROR: INVALID MESH KEY GIVEN, PLEASE LOAD MESH OR CHECK KEY" << std::endl;
//#endif
//		return 0;
//	}
//
//	void ModelManager::AddLoadedMesh(Mesh* mesh, const std::string& meshKey)
//	{
//		meshlist.insert({ meshKey, std::move(mesh) });
//	}

	int ModelManager::AddModel(Model* const model)
	{
		_models.push_back(model);

		return _modelIDs++;
	}

	int ModelManager::AddModel(const std::string& modelkey)
	{
		auto check = _modelList.find(modelkey);

		if (check != _modelList.end())
		{
			// Make new copy of model
			// Reason: Animation IS A FKER
			Model* model = new Model();
			size_t meshSize = check->second->_meshes.size();

			//MAYBE REMOVED NOT THE BEST WAY TO DO THIS
			for (size_t meshIndex = 0; meshIndex != meshSize; ++meshIndex)
			{
				Mesh* mesh = new Mesh();
				mesh->_vertices = check->second->_meshes[meshIndex]->_vertices;
				mesh->_indices = check->second->_meshes[meshIndex]->_indices;
				mesh->_rgb = check->second->_meshes[meshIndex]->_rgb;
				mesh->_uv = check->second->_meshes[meshIndex]->_uv;
				mesh->_normals = check->second->_meshes[meshIndex]->_normals;

				glGenVertexArrays(1, &mesh->VAO);
				glGenBuffers(1, &mesh->VBO);
				glGenBuffers(1, &mesh->EBO);
				//glGenBuffers(1, &mesh->CBO);
				glGenBuffers(1, &mesh->NBO);
				glGenBuffers(1, &mesh->UVBO);
				glGenBuffers(1, &mesh->ModelMatrixBO);

				glBindVertexArray(mesh->VAO);

				// pos attribute
				glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_vertices.size(), &mesh->_vertices[0], GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
				glEnableVertexAttribArray(0);


				// uv attribute
				if (mesh->_uv.size() > 0)
				{
					glBindBuffer(GL_ARRAY_BUFFER, mesh->UVBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh->_uv.size(), &mesh->_uv[0], GL_STATIC_DRAW);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
					glEnableVertexAttribArray(1);
				}

				// color attribute
				/*glBindBuffer(GL_ARRAY_BUFFER, mesh->CBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_rgb.size(), &mesh->_rgb[0], GL_STATIC_DRAW);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
				glEnableVertexAttribArray(2);*/

				// normals attribute
				glBindBuffer(GL_ARRAY_BUFFER, mesh->NBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_normals.size(), &mesh->_normals[0], GL_STATIC_DRAW);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
				glEnableVertexAttribArray(2);

				// Indices
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->_indices.size(), &mesh->_indices[0], GL_STATIC_DRAW);

				// Model Matrix
				glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
				glEnableVertexAttribArray(6);
				glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

				glVertexAttribDivisor(3, 1);
				glVertexAttribDivisor(4, 1);
				glVertexAttribDivisor(5, 1);
				glVertexAttribDivisor(6, 1);

				model->_meshes.push_back(mesh);
			}
			
			return AddModel(model);
		}

		// Invalid key given
#ifdef _DEBUG
		std::cout << "ERROR: INVALID MODEL KEY GIVEN, PLEASE LOAD MODEL OR CHECK KEY" << std::endl;
#endif
		return -1;
	}

	void ModelManager::AddLoadedModel(Model* model, const std::string& modelkey)
	{
		_modelList.insert({ modelkey, std::move(model) });
	}

	void ModelManager::Free()
	{
		DumpModelData();
	}

	void ModelManager::DumpModelData()
	{
		/*for (auto& i : meshes)
		{
			glDeleteBuffers(1, &i->VBO);
			glDeleteBuffers(1, &i->UVBO);
			glDeleteBuffers(1, &i->CBO);
			glDeleteBuffers(1, &i->EBO);
			glDeleteBuffers(1, &i->ModelMatrixBO);
			glDeleteVertexArrays(1, &i->VAO);

			delete i;
		}
		
		for (auto& j : meshlist)
		{
			 In case of allocated gl buffers made in meshlist
			 Though generally shouldn't be the case
			glDeleteBuffers(1, &j.second->VBO);
			glDeleteBuffers(1, &j.second->UVBO);
			glDeleteBuffers(1, &j.second->CBO);
			glDeleteBuffers(1, &j.second->EBO);
			glDeleteBuffers(1, &j.second->ModelMatrixBO);
			glDeleteVertexArrays(1, &j.second->VAO);

			delete j.second;
		}*/

		for (auto& m : _models)
		{
			for (auto& mesh : m->_meshes)
			{
				glDeleteBuffers(1, &mesh->VBO);
				glDeleteBuffers(1, &mesh->UVBO);
				glDeleteBuffers(1, &mesh->CBO);
				glDeleteBuffers(1, &mesh->EBO);
				glDeleteBuffers(1, &mesh->ModelMatrixBO);
				glDeleteVertexArrays(1, &mesh->VAO);

				delete mesh;
			}

			delete m;
		}

		for (auto& n : _modelList)
		{
			for (auto& del : n.second->_meshes)
			{
				glDeleteBuffers(1, &del->VBO);
				glDeleteBuffers(1, &del->UVBO);
				glDeleteBuffers(1, &del->CBO);
				glDeleteBuffers(1, &del->EBO);
				glDeleteBuffers(1, &del->ModelMatrixBO);
				glDeleteVertexArrays(1, &del->VAO);
				
				//Same variable as mesh list mesh pointer therefore commented out
				//Uncomment in future when the mesh pointer is pointing to valid thing
				delete del;
			}

			delete n.second;
		}
	}
}