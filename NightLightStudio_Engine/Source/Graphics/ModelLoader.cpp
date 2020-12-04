#include "ModelLoader.h"
#include "../../framework.h"
#include <iostream>
#include <string>
#include <../glm/gtx/quaternion.hpp>
#include <set>

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

	//Debug Function
	void MaxInfluencePerVertex(const aiMesh* mesh)
	{
		std::vector<int> counts;
		std::vector<float> weights;
		counts.resize(mesh->mNumVertices);
		weights.resize(mesh->mNumVertices);
		for (unsigned i = 0; i < mesh->mNumBones; i++) {
			const aiBone* bone = mesh->mBones[i];
			for (unsigned j = 0; j < bone->mNumWeights; j++) {
				const aiVertexWeight* weight = &bone->mWeights[j];
				counts[weight->mVertexId]++;
				weights[weight->mVertexId] += weight->mWeight;
			}
		}
		int max = 0;
		int index = 0;
		for (unsigned i = 0; i < mesh->mNumVertices; i++) {
			if (max < counts[i])
			{
				max = counts[i];
				index = i;
			}
		}

		std::cout << "Max Bone Influence Per Vertex : " << max << std::endl;
		std::cout << "Total Weights at max bone influence index: " << weights[index] << std::endl;
	}

	//Debug Function
	void SeeAllSkeleton(aiNode* node, const aiScene* scene, Model*& model)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			std::cout << "Mesh Name: " << mesh->mName.C_Str() << std::endl;
			std::cout << "Node Name: " << node->mName.C_Str() << std::endl;

			for (unsigned int j = 0; j < mesh->mNumBones; j++)
			{
				std::cout << "Bone " << j << ": " << mesh->mBones[j]->mName.C_Str() << std::endl;
				BoneData bone;

				bone._boneID = j;
				bone._boneName = mesh->mBones[j]->mName.C_Str();
			}
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			SeeAllSkeleton(node->mChildren[i], scene, model);
		}
	}

	//Debug Function
	void SeeAllAnimation(aiNode* node, const aiScene* scene)
	{
		UNREFERENCED_PARAMETER(node);

		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			std::cout << "Animation " << i << ": " << scene->mAnimations[i]->mName.C_Str() << std::endl;
			for (unsigned int x = 0; x < scene->mAnimations[i]->mNumChannels; x++)
			{
				std::cout << "Animation Affecting bones " << scene->mAnimations[i]->mChannels[x]->mNodeName.C_Str() << std::endl;
				for (unsigned int y = 0; y < scene->mAnimations[i]->mChannels[x]->mNumPositionKeys; y++)
				{
					std::cout << "Frame time:  " << scene->mAnimations[i]->mChannels[x]->mPositionKeys[y].mTime << std::endl;
				}
			}
		}
	}

	//Debug Function
	void SeeAllNode(aiNode* node, const aiScene* scene)
	{
		if (node->mParent)
		{
			std::cout << "Parented to : " << node->mParent->mName.C_Str() << std::endl;
		}
		std::cout << "	Node : " << node->mName.C_Str() << std::endl;
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			SeeAllNode(node->mChildren[i], scene);
		}
	}

	void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Model*& model)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			std::cout << "Loading " << mesh->mName.C_Str() << std::endl;
			
			if (model->_isAnimated)
			{
				model->_animatedMeshes.push_back(ProcessAnimatedMesh(node, mesh, scene, model));
			}
			else
			{
				model->_meshes.push_back(ProcessMesh(node, mesh, scene, model));
			}
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model);
		}
	}

	Mesh* ModelLoader::ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model)
	{
		UNREFERENCED_PARAMETER(model);
		UNREFERENCED_PARAMETER(node);
		UNREFERENCED_PARAMETER(scene);

		Mesh* newMesh = new Mesh();
		newMesh->_nodeName = mesh->mName.C_Str();
		newMesh->_vertexDatas.reserve((size_t)mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Mesh::VertexData vertexData{};

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

			if (mesh->HasTangentsAndBitangents())
			{
				vertexData._tangent.x = mesh->mTangents[i].x;
				vertexData._tangent.y = mesh->mTangents[i].y;
				vertexData._biTangent.x = mesh->mBitangents[i].x;
				vertexData._biTangent.y = mesh->mBitangents[i].y;
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

	AnimatedMesh* ModelLoader::ProcessAnimatedMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model)
	{
		//MaxInfluencePerVertex(mesh);
		AnimatedMesh* newAnimatedMesh = new AnimatedMesh();
		newAnimatedMesh->_nodeName = mesh->mName.C_Str();
		newAnimatedMesh->_vertexDatas.reserve((size_t)mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			AnimatedMesh::VertexData vertexData{};

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

			if (mesh->HasTangentsAndBitangents())
			{
				//If it has tangent, it has bitangent
				vertexData._tangent.x = mesh->mTangents[i].x;
				vertexData._tangent.y = mesh->mTangents[i].y;
				vertexData._biTangent.x = mesh->mBitangents[i].x;
				vertexData._biTangent.y = mesh->mBitangents[i].y;
			}

			//Explicitly zero out all
			vertexData._boneID = glm::ivec4(0.0f);
			vertexData._boneWeights = glm::vec4(0.0f);

			newAnimatedMesh->_vertexDatas.push_back(vertexData);
		}

		ProcessBone(node, mesh, scene, model, newAnimatedMesh);

		newAnimatedMesh->_indices.reserve((size_t)mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			newAnimatedMesh->_indices.push_back(face.mIndices[0]);
			newAnimatedMesh->_indices.push_back(face.mIndices[1]);
			newAnimatedMesh->_indices.push_back(face.mIndices[2]);
		}

		return newAnimatedMesh;
	}

	void ModelLoader::ProcessBone(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model, AnimatedMesh* animatedMesh)
	{
		UNREFERENCED_PARAMETER(scene);
		UNREFERENCED_PARAMETER(node);

		for (size_t i = 0; i < mesh->mNumBones; i++) 
		{
			unsigned boneID = 0;
			std::string boneName = mesh->mBones[i]->mName.C_Str();

			if (model->_boneMapping.find(boneName) == model->_boneMapping.end()) 
			{
				// Allocate an index for a new bone
				boneID = model->_boneCount;
				model->_boneCount++;
				glm::mat4 boneOffset;
				AssimpToGLM(mesh->mBones[i]->mOffsetMatrix, boneOffset);

				model->_boneMapping[boneName] = std::make_pair(boneID, boneOffset);
			}
			else 
			{
				boneID = model->_boneMapping[boneName].first;
			}

			for (size_t j = 0; j < mesh->mBones[i]->mNumWeights; j++) 
			{
				unsigned vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;

				animatedMesh->_vertexDatas[vertexID].AddBoneData(boneID, weight);
			}
		}
	}

	bool ModelLoader::CreateSkeletal(BoneData& bone, aiNode* node, const aiScene* scene, Model*& model)
	{
		//Only if node is a bone
		if (model->_boneMapping.find(node->mName.C_Str()) != model->_boneMapping.end()) 
		{
			bone._boneName = node->mName.C_Str();
			bone._boneID = model->_boneMapping[bone._boneName].first;
			bone._boneTransformOffset = model->_boneMapping[bone._boneName].second;
			AssimpToGLM(node->mTransformation, bone._boneTransform);

			for (unsigned i = 0; i < node->mNumChildren; i++) 
			{
				BoneData childBone;
				if (CreateSkeletal(childBone, node->mChildren[i], scene, model))
				{
					bone._childrenBones.push_back(childBone);
				}
			}
			return true;
		}
		else 
		{
			//Find other bone in children
			for (unsigned i = 0; i < node->mNumChildren; i++) 
			{
				if (CreateSkeletal(bone ,node->mChildren[i], scene, model))
				{
					return true;
				}

			}
		}
		return false;
	}

	void ModelLoader::ProcessAnimation(aiNode* node, const aiScene* scene, Model*& model)
	{
		UNREFERENCED_PARAMETER(node);

		for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
		{
			Animation* newAnim = new Animation();
			aiAnimation* currAnim = scene->mAnimations[i];
			newAnim->_animName = scene->mAnimations[i]->mName.C_Str();

			double tickPerSec = 0.0f;

			if (currAnim->mTicksPerSecond != 0.0f)
			{
				tickPerSec = currAnim->mTicksPerSecond;
			}
			else
			{
				tickPerSec = 1.0f;
			}

			//double smallestPossibleTime = 1000000;
			newAnim->_time = currAnim->mDuration / tickPerSec;

			for (unsigned int x = 0; x < currAnim->mNumChannels; ++x)
			{
				Animation::KeyFrames currFrame;

				currFrame._position.reserve(currAnim->mChannels[x]->mNumPositionKeys);
				for (unsigned int y = 0; y < currAnim->mChannels[x]->mNumPositionKeys; ++y)
				{
					currFrame._posTime.push_back(currAnim->mChannels[x]->mPositionKeys[y].mTime / tickPerSec);
					glm::vec3 pos = glm::vec3(0.0f);
					AssimpToGLM(currAnim->mChannels[x]->mPositionKeys[y].mValue, pos);
					currFrame._position.push_back(pos);
				}

				//smallestPossibleTime = std::min(currFrame._posTime[currFrame._posTime.size()-1], newAnim->_time);

				currFrame._rotation.reserve(currAnim->mChannels[x]->mNumRotationKeys);
				for (unsigned int y = 0; y < currAnim->mChannels[x]->mNumRotationKeys; ++y)
				{
					currFrame._rotateTime.push_back(currAnim->mChannels[x]->mRotationKeys[y].mTime / tickPerSec);
					glm::quat rotate;
					AssimpToGLM(currAnim->mChannels[x]->mRotationKeys[y].mValue, rotate);
					currFrame._rotation.push_back(rotate);
				}

				currFrame._scale.reserve(currAnim->mChannels[x]->mNumScalingKeys);
				for (unsigned int y = 0; y < currAnim->mChannels[x]->mNumScalingKeys; ++y)
				{
					currFrame._scaleTime.push_back(currAnim->mChannels[x]->mScalingKeys[y].mTime / tickPerSec);
					glm::vec3 scale = glm::vec3(0.0f);
					AssimpToGLM(currAnim->mChannels[x]->mScalingKeys[y].mValue, scale);
					currFrame._scale.push_back(scale);
				}

				newAnim->_frames[currAnim->mChannels[x]->mNodeName.C_Str()] = currFrame;
			}

			model->_animations[newAnim->_animName] = newAnim;
		}
	}
	
	//Debug function
	void ModelLoader::FillNode(Node& ourNode, aiNode* node, const aiScene* scene, Model*& model)
	{
		ourNode._nodeName = node->mName.C_Str();
		AssimpToGLM(node->mTransformation, ourNode._nodeTransform);
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{	
			ourNode._childrenNode.push_back(Node());
			FillNode(ourNode._childrenNode[i], node->mChildren[i], scene, model);
		}
	}

	void ModelLoader::AssimpToGLM(const aiMatrix4x4& ai, glm::mat4& glm)
	{
		glm[0].x = ai.a1; 
		glm[0].y = ai.b1; 
		glm[0].z = ai.c1; 
		glm[0].w = ai.d1;

		glm[1].x = ai.a2; 
		glm[1].y = ai.b2; 
		glm[1].z = ai.c2; 
		glm[1].w = ai.d2;

		glm[2].x = ai.a3; 
		glm[2].y = ai.b3; 
		glm[2].z = ai.c3; 
		glm[2].w = ai.d3;

		glm[3].x = ai.a4; 
		glm[3].y = ai.b4; 
		glm[3].z = ai.c4; 
		glm[3].w = ai.d4;
	}

	void ModelLoader::AssimpToGLM(const aiVector3D& ai, glm::vec3& glm)
	{
		glm.x = ai.x;
		glm.y = ai.y;
		glm.z = ai.z;
	}

	void ModelLoader::AssimpToGLM(const aiQuaternion& ai, glm::quat& glm)
	{
		glm.x = ai.x;
		glm.y = ai.y;
		glm.z = ai.z;
		glm.w = ai.w;
	}

	bool ModelLoader::LoadFBX(Model*& model)
	{
		std::cout << "Loading FBX..." << std::endl;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(model->_fileName, aiProcess_Triangulate | 
																 aiProcess_FlipUVs | 
																 aiProcess_JoinIdenticalVertices |
																 aiProcess_LimitBoneWeights |
																 aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "Load Failed. Reason: " << import.GetErrorString() << endl;
			return false;
		}

		if (scene->HasAnimations())
		{
			model->_isAnimated = true;
			model->_animatedMeshes.reserve(scene->mNumMeshes);

			AssimpToGLM(scene->mRootNode->mTransformation, model->_globalInverseTransform);
			model->_globalInverseTransform = glm::inverse(model->_globalInverseTransform);
			model->_poseTransform.resize(MAX_BONE_COUNT, glm::mat4(1.0f));

			//First get the vertex info and bone info
			ProcessNode(scene->mRootNode, scene, model);
			//Then, create the skeleton hierarchy
			CreateSkeletal(model->_rootBone, scene->mRootNode, scene, model);
			//Finally, get the animation data
			ProcessAnimation(scene->mRootNode, scene, model);
		}
		else
		{
			model->_isAnimated = false;
			model->_meshes.reserve(scene->mNumMeshes);
			ProcessNode(scene->mRootNode, scene, model);
		}

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

			//Reads vertex data
			if (input.find("VERTEX") != std::string::npos)
			{
				std::string vertexCount = input.substr(input.find(" ") + 1);
				Mesh* newMesh = new Mesh();
				newMesh->_vertexDatas.reserve((size_t)std::stoi(vertexCount));
				//newMesh->_indices.reserve((size_t)std::stoi(vertexCount));

				std::getline(meshFile, input);
				newMesh->_nodeName = input;

				Mesh::VertexData vertex;

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
						//index++;
						newMesh->_vertexDatas.push_back(vertex);

						//if (index >= std::stoi(vertexCount))
						//{
						//	break;
						//}
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

		if (model->_isAnimated)
		{
			auto meshIterator = model->_animatedMeshes.begin();
			auto meshIteratorEnd = model->_animatedMeshes.end();

			while (meshIterator != meshIteratorEnd)
			{
				++meshIterator;
			}
		}
		else
		{
			auto meshIterator = model->_meshes.begin();
			auto meshIteratorEnd = model->_meshes.end();

			while (meshIterator != meshIteratorEnd)
			{
				SaveMeshVertex(meshFile, *meshIterator);
				meshFile << "END\n";
				++meshIterator;
			}
		}

		meshFile.close();
		return true;
	}
	void ModelLoader::SaveMeshVertex(std::ofstream& file, Mesh*& mesh)
	{
		file << "VERTEX " << mesh->_vertexDatas.size() << "\n";
		file << mesh->_nodeName << "\n";

		size_t vertexCount = mesh->_vertexDatas.size();
		for (size_t i = 0; i < vertexCount; ++i)
		{
			file << "v: " << mesh->_vertexDatas[i]._position.x << ","
				<< mesh->_vertexDatas[i]._position.y << ","
				<< mesh->_vertexDatas[i]._position.z
				<< "\n";

			file << "uv: " << mesh->_vertexDatas[i]._uv.x << ","
				<< mesh->_vertexDatas[i]._uv.x
				<< "\n";

			file << "n: " << mesh->_vertexDatas[i]._normals.x << ","
				<< mesh->_vertexDatas[i]._normals.y << ","
				<< mesh->_vertexDatas[i]._normals.z
				<< "\n";
		}

		file << mesh->_indices.size() << " index ";
		int nextLineCount = 0;
		for (auto& indices : mesh->_indices)
		{
			file << indices << ",";
			nextLineCount++;

			if (nextLineCount == 6)
			{
				file << "\n";
				nextLineCount = 0;
			}
		}
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