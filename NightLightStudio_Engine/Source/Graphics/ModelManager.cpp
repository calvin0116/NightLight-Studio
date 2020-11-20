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

	size_t ModelManager::GetFreeIndex()
	{
		return std::distance(_usedStatus.begin(), std::find(_usedStatus.begin(), _usedStatus.end(), false)) - 1;
	}

	void ModelManager::SetFreeIndex(unsigned index, bool state)
	{
		_usedStatus[index] = state;
	}

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

				mesh->_vertexDatas = check->second->_meshes[meshIndex]->_vertexDatas;

				glGenVertexArrays(1, &mesh->VAO);
				glGenBuffers(1, &mesh->VBO);
				glGenBuffers(1, &mesh->ModelMatrixBO);

				glBindVertexArray(mesh->VAO);

				// vertex data attribute
				glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(NS_GRAPHICS::Mesh::VertexData) * mesh->_vertexDatas.size(), &mesh->_vertexDatas[0], GL_STATIC_DRAW);
				// position
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), (void*)0);
				glEnableVertexAttribArray(0);
				// uv attribute
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), (void*)sizeof(glm::vec3));
				glEnableVertexAttribArray(1);
				// normals attribute
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
				glEnableVertexAttribArray(2);

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

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

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