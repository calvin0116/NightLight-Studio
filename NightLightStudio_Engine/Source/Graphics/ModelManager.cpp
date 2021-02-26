#include "ModelManager.h"
#include "../glm/mat4x4.hpp"
#include "ModelLoader.h"
#include "../tracy-master/Tracy.hpp"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace NS_GRAPHICS
{
	ModelManager::ModelManager()
		: //meshIDs{ 0 }, 
		_modelIDs{ 0 }
	{
		_models.resize(INITIAL_MAX_COUNT, nullptr);
		_usedStatus.resize(INITIAL_MAX_COUNT, false);
	}

	ModelManager::~ModelManager()
	{

	}

	size_t ModelManager::GetFreeIndex()
	{
		return std::distance(_usedStatus.begin(), std::find(_usedStatus.begin(), _usedStatus.end(), false));
	}

	int ModelManager::AddModel(Model* const model)
	{
		size_t index = GetFreeIndex();

		//Full time to resize
		if (index >= _models.size())
		{
			_models.push_back(model);
			_usedStatus.push_back(true);
		}
		else
		{
			_models[index] = model;
			_usedStatus[index] = true;
		}

		return (int)index;
	}

	int ModelManager::AddModel(const std::string& modelkey)
	{
		// Check for duplicate loads
		auto check = _modelList.find(modelkey);

		if (check != _modelList.end())
		{
			// Make new copy of model
			// Reason: Animation IS A FKER
			// Due to this, only animation requires unique models
			if (check->second->_isAnimated)
			{
				Model* model = new Model();
				size_t meshSize = check->second->_meshes.size();
				model->_isAnimated = check->second->_isAnimated;
				model->_rootBone = new Skeleton();
				model->_rootBone->_boneMapping = check->second->_rootBone->_boneMapping;
				model->_rootBone->_boneCount = check->second->_rootBone->_boneCount;
				model->_rootBone->_rootJoint = check->second->_rootBone->_rootJoint;

				model->_rootNode = check->second->_rootNode;
				model->_globalInverseTransform = check->second->_globalInverseTransform;

				//MAYBE REMOVED NOT THE BEST WAY TO DO THIS
				for (size_t meshIndex = 0; meshIndex != meshSize; ++meshIndex)
				{
					Mesh* mesh = new Mesh();

					mesh->_vertexDatas = check->second->_meshes[meshIndex]->_vertexDatas;
					mesh->_skinDatas = check->second->_meshes[meshIndex]->_skinDatas;
					mesh->_indices = check->second->_meshes[meshIndex]->_indices;

					glGenVertexArrays(1, &mesh->VAO);
					glGenBuffers(1, &mesh->VBO);
					glGenBuffers(1, &mesh->EBO);
					glGenBuffers(1, &mesh->ModelMatrixBO);
					glGenBuffers(1, &mesh->BoneBO);

					glBindVertexArray(mesh->VAO);

					// vertex data attribute
					glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(NS_GRAPHICS::Mesh::VertexData) * mesh->_vertexDatas.size(), &mesh->_vertexDatas[0], GL_STATIC_DRAW);
					// position
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), 
						(void*)0);  // 0
					glEnableVertexAttribArray(0);
					// uv attribute
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData),
						(void*)sizeof(glm::vec3)); // pos
					glEnableVertexAttribArray(1);
					// normals attribute
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData),
						(void*)(sizeof(glm::vec3) + sizeof(glm::vec2))); // pos + uv
					glEnableVertexAttribArray(2);
					// tangent attribute
					glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData),
						(void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3))); // pos + uv + norm
					glEnableVertexAttribArray(3);

					// skin data attribute
					glBindBuffer(GL_ARRAY_BUFFER, mesh->BoneBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(NS_GRAPHICS::Mesh::SkinData) * mesh->_skinDatas.size(), &mesh->_skinDatas[0], GL_STATIC_DRAW);
					// bone id
					glVertexAttribIPointer(4, 4, GL_INT, sizeof(NS_GRAPHICS::Mesh::SkinData),(void*)0);
					glEnableVertexAttribArray(4);
					// bone weight
					glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::SkinData),(void*)(sizeof(glm::ivec4))); //boneID
					glEnableVertexAttribArray(5);

					// Indices
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mesh->_indices.size(), &mesh->_indices[0], GL_STATIC_DRAW);

					// Model Matrix
					glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

					glEnableVertexAttribArray(6);
					glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
					glEnableVertexAttribArray(7);
					glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
					glEnableVertexAttribArray(8);
					glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
					glEnableVertexAttribArray(9);
					glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

					glVertexAttribDivisor(6, 1);
					glVertexAttribDivisor(7, 1);
					glVertexAttribDivisor(8, 1);
					glVertexAttribDivisor(9, 1);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);

					model->_meshes.push_back(mesh);
				}

				for (auto& anim: check->second->_animations)
				{
					Animation* newAnim = new Animation();
					newAnim->_animName = anim.second->_animName;
					newAnim->_frames = anim.second->_frames;
					newAnim->_time = anim.second->_time;

					model->_animations.insert(std::make_pair(newAnim->_animName, newAnim));
				}

				return AddModel(model);
			}
			//Non animated
			else
			{
				// For non animated models, we can perform instancing by setting reference model
				// Same VAO, VBO and EBO
				// Model matrix attribute will be different across all instances
				// Create large model matrix BO to store required size for number of instances
				// 
				size_t meshSize = check->second->_meshes.size();

				//MAYBE REMOVED NOT THE BEST WAY TO DO THIS
				for (size_t meshIndex = 0; meshIndex != meshSize; ++meshIndex)
				{
					//Unintialized model
					if (check->second->_meshes[meshIndex]->VAO == 0)
					{
						glGenVertexArrays(1, &check->second->_meshes[meshIndex]->VAO);
						glGenBuffers(1, &check->second->_meshes[meshIndex]->VBO);
						glGenBuffers(1, &check->second->_meshes[meshIndex]->EBO);
						glGenBuffers(1, &check->second->_meshes[meshIndex]->ModelMatrixBO);

						glBindVertexArray(check->second->_meshes[meshIndex]->VAO);

						// vertex data attribute
						glBindBuffer(GL_ARRAY_BUFFER, check->second->_meshes[meshIndex]->VBO);
						glBufferData(GL_ARRAY_BUFFER, sizeof(NS_GRAPHICS::Mesh::VertexData) * check->second->_meshes[meshIndex]->_vertexDatas.size(), &check->second->_meshes[meshIndex]->_vertexDatas[0], GL_STATIC_DRAW);
						// position
						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), 
							(void*)0); // 0
						glEnableVertexAttribArray(0);
						// uv attribute
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), 
							(void*)sizeof(glm::vec3)); // pos
						glEnableVertexAttribArray(1);
						// normals attribute
						glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), 
							(void*)(sizeof(glm::vec3) + sizeof(glm::vec2))); // pos + uv
						glEnableVertexAttribArray(2);
						// tangent attribute
						glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(NS_GRAPHICS::Mesh::VertexData), 
							(void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3))); // pos + uv + norm
						glEnableVertexAttribArray(3);

						// Indices
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, check->second->_meshes[meshIndex]->EBO);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * check->second->_meshes[meshIndex]->_indices.size(), &check->second->_meshes[meshIndex]->_indices[0], GL_STATIC_DRAW);

						// Model Matrix
						glBindBuffer(GL_ARRAY_BUFFER, check->second->_meshes[meshIndex]->ModelMatrixBO);
						glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

						glEnableVertexAttribArray(4);
						glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
						glEnableVertexAttribArray(5);
						glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
						glEnableVertexAttribArray(6);
						glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
						glEnableVertexAttribArray(7);
						glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

						glVertexAttribDivisor(4, 1);
						glVertexAttribDivisor(5, 1);
						glVertexAttribDivisor(6, 1);
						glVertexAttribDivisor(7, 1);

						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindVertexArray(0);
					}
				}

				return AddModel(check->second);
			}
		}

		// Invalid key given
#ifdef _DEBUG
		TracyMessageL("ModelManager::AddModel: ERROR: INVALID MODEL KEY GIVEN, PLEASE LOAD MODEL OR CHECK KEY");
		//std::cout << "ERROR: INVALID MODEL KEY GIVEN, PLEASE LOAD MODEL OR CHECK KEY" << std::endl;
#endif
		return -1;
	}

	bool ModelManager::RemoveModelByID(const int& index)
	{
		if (_models[index] == nullptr || index >= _models.size())
		{
			return false;
		}

		if (_models[index]->_isAnimated)
		{
			for (auto& mesh : _models[index]->_meshes)
			{
				glDeleteBuffers(1, &mesh->VBO);
				glDeleteBuffers(1, &mesh->ModelMatrixBO);
				glDeleteBuffers(1, &mesh->EBO);
				glDeleteVertexArrays(1, &mesh->VAO);

				delete mesh;
			}

			for (auto& anim : _models[index]->_animations)
			{
				delete anim.second;
			}

			delete _models[index];
		}

		_models[index] = nullptr;
		_usedStatus[index] = false;

		return true;
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
			if (m)
			{
				if (m->_isAnimated)
				{
					for (auto& mesh : m->_meshes)
					{
						glDeleteBuffers(1, &mesh->VBO);
						glDeleteBuffers(1, &mesh->ModelMatrixBO);
						glDeleteBuffers(1, &mesh->EBO);
						glDeleteVertexArrays(1, &mesh->VAO);

						delete mesh;
					}

					for (auto& anim : m->_animations)
					{
						delete anim.second;
					}

					delete m->_rootBone;
					delete m;
				}
			}
		}

		for (auto& n : _modelList)
		{
			if (n.second->_isAnimated)
			{
				for (auto& del : n.second->_meshes)
				{
					glDeleteBuffers(1, &del->VBO);
					glDeleteBuffers(1, &del->ModelMatrixBO);
					glDeleteBuffers(1, &del->EBO);
					glDeleteVertexArrays(1, &del->VAO);

					//Same variable as mesh list mesh pointer therefore commented out
					//Uncomment in future when the mesh pointer is pointing to valid thing
					delete del;
				}
				for (auto& anim : n.second->_animations)
				{
					delete anim.second;
				}
			}
			else
			{
				for (auto& del : n.second->_meshes)
				{
					glDeleteBuffers(1, &del->VBO);
					glDeleteBuffers(1, &del->ModelMatrixBO);
					glDeleteBuffers(1, &del->EBO);
					glDeleteVertexArrays(1, &del->VAO);

					//Same variable as mesh list mesh pointer therefore commented out
					//Uncomment in future when the mesh pointer is pointing to valid thing
					delete del;
				}
			}

			if (n.second->_rootBone)
			{
				delete n.second->_rootBone;
			}
			delete n.second;
		}
	}
}