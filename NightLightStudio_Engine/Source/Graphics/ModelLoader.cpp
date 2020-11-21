#include "ModelLoader.h"
#include "../../framework.h"
#include <iostream>
#include <string>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

//#define LOG_MODEL

namespace NS_GRAPHICS
{
	ModelLoader::ModelLoader() :
		_modelManager{ &ModelManager::GetInstance() }
	{
		std::cout << "Model Loader Created" << std::endl;
	}

	ModelLoader::~ModelLoader()
	{
	}

	void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Model*& model)
	{
		model->_meshes.reserve(node->mNumMeshes);
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			model->_meshes.push_back(ProcessMesh(node, mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model);
		}
	}

	Mesh* ModelLoader::ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene)
	{
		Mesh* newMesh = new Mesh();
		newMesh->_nodeName = mesh->mName.C_Str();
		newMesh->_vertexDatas.reserve((size_t)mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Mesh::VertexData vertexData;

			vertexData._position.x = mesh->mVertices[i].x;
			vertexData._position.y = mesh->mVertices[i].y;
			vertexData._position.z = mesh->mVertices[i].z;

			if (mesh->HasNormals())
			{
				//Only Load Normal if there is 1
				vertexData._normals.x = mesh->mNormals[i].x;
				vertexData._normals.y = mesh->mNormals[i].y;
				vertexData._normals.z = mesh->mNormals[i].z;
			}

			if (mesh->mTextureCoords[0])
			{
				//Takes the first set of uv datas
				vertexData._uv.x = mesh->mTextureCoords[0][i].x;
				vertexData._uv.y = mesh->mTextureCoords[0][i].y;
			}

			newMesh->_vertexDatas.push_back(vertexData);
		}

		newMesh->_indices.reserve((size_t)mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			
			newMesh->_indices.push_back(face.mIndices[0]);
			newMesh->_indices.push_back(face.mIndices[1]);
			newMesh->_indices.push_back(face.mIndices[2]);
		}

		return newMesh;
	}

	bool ModelLoader::LoadFBX(Model*& model)
	{
		std::cout << "Loading FBX..." << std::endl;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(model->_fileName, aiProcess_Triangulate | 
																 aiProcess_FlipUVs | 
																 aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "Load Failed. Reason: " << import.GetErrorString() << endl;
			return false;
		}

		ProcessNode(scene->mRootNode, scene, model);
		return true;
	}
	void ModelLoader::LoadModel(const std::string& fileName)
	{
		if (fileName.empty())
		{
			return;
		}

		Model* model = new Model();
		model->_fileName = fileName;

		//Checks for the file name
		std::string name;
		size_t pos = fileName.rfind("\\");
		//Get just the string after the last path
		if (pos != std::string::npos)
		{
			name = fileName.substr(pos + 1);
		}
		else
		{
			name = fileName;
		}

		pos = name.rfind(".");
		//Trim the extension to get the file name
		if (pos != std::string::npos)
		{
			name.erase(pos);
		}

		std::string searchString;
		searchString = fileName;
		//searchString = s_LocalPathName + name + s_ModelFileType;

		//First checks if the custom model existed to avoid unnecessary loading
		if (_modelManager->_modelList.find(searchString) != _modelManager->_modelList.end())
		{
			delete model;
			return;
		}

		//Loads the correct function based on extension
		if (fileName.find(s_ModelFileType) != std::string::npos)
		{
			if (!LoadCustomMesh(model))
			{
				delete model;
				return;
			}

			//If different pathing, saves another copy in our own asset folder
			if (model->_fileName != searchString)
			{
				model->_fileName = s_LocalPathName + name + s_ModelFileType;
				SaveCustomMesh(model);
			}
		}
		else
		{
			if (!LoadFBX(model))
			{
				delete model;
				return;
			}

			//model->_fileName = s_LocalPathName + name + s_ModelFileType;
			//SaveCustomMesh(model);
		}

		_modelManager->AddLoadedModel(model, model->_fileName);

#ifdef LOG_MODEL
		//std::string textFileName = model-> + ".txt";
		DebugToFile(model->_fileName);
#endif

		//TODO Saving custom mesh
	}
	bool ModelLoader::LoadCustomMesh(Model*& model)
	{
		if (model->_fileName[0] == '\\')
		{
			model->_fileName.erase(0, 1);
		}
		//Gets rid of warning for now
		std::ifstream meshFile;
		meshFile.open(model->_fileName.c_str());

		if (!meshFile.is_open())
		{
			std::cout << "Fail to opened model file" << std::endl;
			return false;
		}

		std::string input;
		while (std::getline(meshFile, input))
		{
			std::cout << "Reading Variables" << std::endl;
			if (input.find("BEGIN") != std::string::npos)
			{
				std::string vertexCount = input.substr(input.find(" ") + 1);
				Mesh* newMesh = new Mesh();
				newMesh->_vertexDatas.reserve((size_t)std::stoi(vertexCount));
				//newMesh->_indices.reserve((size_t)std::stoi(vertexCount));

				std::getline(meshFile, input);
				newMesh->_nodeName = input;

				Mesh::VertexData vertex;

				int index = 0;

				while (std::getline(meshFile, input, ' '))
				{
					if (input == "v:")
					{		
						std::getline(meshFile, input);
						std::string posX = input.substr(0, input.find(","));
						std::string nextPos = input.substr(input.find(",")+1);
						std::string posY = nextPos.substr(0, nextPos.find(","));
						std::string posZ = input.substr(input.rfind(",")+1);

						vertex._position.x = std::stof(posX);
						vertex._position.y = std::stof(posY);
						vertex._position.z = std::stof(posZ);

					}

					else if (input == "uv:")
					{
						std::getline(meshFile, input);
						std::string u = input.substr(0, input.find(","));
						std::string v = input.substr(input.rfind(",") + 1);

						vertex._uv.x = std::stof(u);
						vertex._uv.y = std::stof(v);
					}

					else if (input == "n:")
					{
						std::getline(meshFile, input);
						std::string normalX = input.substr(0, input.find(","));
						std::string normPos = input.substr(input.find(",") + 1);
						std::string normalY = normPos.substr(0, normPos.find(","));
						std::string normalZ = input.substr(input.rfind(",") + 1);

						vertex._normals.x = std::stof(normalX);
						vertex._normals.y = std::stof(normalY);
						vertex._normals.z = std::stof(normalZ);

						//newMesh->_indices.push_back((unsigned short)index);
						index++;
						newMesh->_vertexDatas.push_back(vertex);

						if (index >= std::stoi(vertexCount))
						{
							break;
						}
					}
				}

				std::getline(meshFile, input);
				model->_meshes.push_back(newMesh);
			}
		}

		meshFile.close();
		return true;
	}
	bool ModelLoader::SaveCustomMesh(Model*& model)
	{
		//Gets rid of warning for now
		std::ofstream meshFile;
		meshFile.open(model->_fileName.c_str());

		auto meshIterator = model->_meshes.begin();
		auto meshIteratorEnd = model->_meshes.end();

		while (meshIterator != meshIteratorEnd)
		{
			meshFile << "BEGIN " << (*meshIterator)->_vertexDatas.size() << "\n";
			meshFile << (*meshIterator)->_nodeName << "\n";

			size_t vertexCount = (*meshIterator)->_vertexDatas.size();
			for (size_t i = 0; i < vertexCount; ++i)
			{
				meshFile << "v: " << (*meshIterator)->_vertexDatas[i]._position.x << "," 
								  << (*meshIterator)->_vertexDatas[i]._position.y << ","
								  << (*meshIterator)->_vertexDatas[i]._position.z
								  << "\n";

				meshFile << "uv: " << (*meshIterator)->_vertexDatas[i]._uv.x << "," 
								   << (*meshIterator)->_vertexDatas[i]._uv.x
								   << "\n";

				meshFile << "n: " << (*meshIterator)->_vertexDatas[i]._normals.x << ","
								  << (*meshIterator)->_vertexDatas[i]._normals.y << ","
								  << (*meshIterator)->_vertexDatas[i]._normals.z
								  << "\n";
			}
			meshFile << "END\n";
			++meshIterator;
		}


		meshFile.close();
		return true;
	}
	void ModelLoader::DebugToFile(const std::string& fileName)
	{
		std::string name = fileName;
		name.erase(name.rfind("."));
		name.append(".txt");

		std::ofstream logFile;
		logFile.open(fileName.c_str());

		int lineCount = 0;
		size_t MeshSize = _modelManager->_modelList[fileName]->_meshes.size();
		for (size_t i = 0; i < MeshSize; ++i)
		{
			size_t verticeSize = _modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas.size();
			for (size_t x = 0; x < verticeSize; ++x)
			{
				logFile << "Vertex: X: " << _modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas[x]._position.x << " Y: " <<
											_modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas[x]._position.y << " Z: " <<
											_modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas[x]._position.z << "\n";

				lineCount++;
			}

			logFile << std::endl;
		}

		logFile << "Total Points : " << lineCount << std::endl;

		logFile.close();
	}
}