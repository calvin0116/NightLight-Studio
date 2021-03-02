#include "ModelLoader.h"
#include "../../framework.h"
#include <iostream>
#include <string>
#include <../glm/gtx/quaternion.hpp>
#include <set>
#include "../tracy-master/Tracy.hpp"
#include <sstream>

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
		TracyMessageL("ModelLoader::ModelLoader: Model Loader Created");
		//std::cout << "Model Loader Created" << std::endl;
	}

	ModelLoader::~ModelLoader()
	{
	}

	////Debug Function
	//void MaxInfluencePerVertex(const aiMesh* mesh)
	//{
	//	std::vector<int> counts;
	//	std::vector<float> weights;
	//	counts.resize(mesh->mNumVertices);
	//	weights.resize(mesh->mNumVertices);
	//	for (unsigned i = 0; i < mesh->mNumBones; i++) {
	//		const aiBone* bone = mesh->mBones[i];
	//		for (unsigned j = 0; j < bone->mNumWeights; j++) {
	//			const aiVertexWeight* weight = &bone->mWeights[j];
	//			counts[weight->mVertexId]++;
	//			weights[weight->mVertexId] += weight->mWeight;
	//		}
	//	}
	//	int max = 0;
	//	int index = 0;
	//	for (unsigned i = 0; i < mesh->mNumVertices; i++) {
	//		if (max < counts[i])
	//		{
	//			max = counts[i];
	//			index = i;
	//		}
	//	}

	//	std::cout << "Max Bone Influence Per Vertex : " << max << std::endl;
	//	std::cout << "Total Weights at max bone influence index: " << weights[index] << std::endl;
	//}

	////Debug Function
	//void SeeAllSkeleton(aiNode* node, const aiScene* scene, Model*& model)
	//{
	//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	//	{
	//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

	//		std::cout << "Mesh Name: " << mesh->mName.C_Str() << std::endl;
	//		std::cout << "Node Name: " << node->mName.C_Str() << std::endl;

	//		for (unsigned int j = 0; j < mesh->mNumBones; j++)
	//		{
	//			std::cout << "Bone " << j << ": " << mesh->mBones[j]->mName.C_Str() << std::endl;
	//			Joint joint;

	//			joint._boneID = j;
	//			joint._boneName = mesh->mBones[j]->mName.C_Str();
	//		}
	//	}
	//	for (unsigned int i = 0; i < node->mNumChildren; i++)
	//	{
	//		SeeAllSkeleton(node->mChildren[i], scene, model);
	//	}
	//}

	////Debug Function
	//void SeeAllAnimation(aiNode* node, const aiScene* scene)
	//{
	//	UNREFERENCED_PARAMETER(node);

	//	for (unsigned int i = 0; i < scene->mNumAnimations; i++)
	//	{
	//		std::cout << "Animation " << i << ": " << scene->mAnimations[i]->mName.C_Str() << std::endl;
	//		for (unsigned int x = 0; x < scene->mAnimations[i]->mNumChannels; x++)
	//		{
	//			std::cout << "Animation Affecting bones " << scene->mAnimations[i]->mChannels[x]->mNodeName.C_Str() << std::endl;
	//			for (unsigned int y = 0; y < scene->mAnimations[i]->mChannels[x]->mNumPositionKeys; y++)
	//			{
	//				std::cout << "Frame time:  " << scene->mAnimations[i]->mChannels[x]->mPositionKeys[y].mTime << std::endl;
	//			}
	//		}
	//	}
	//}

	////Debug Function
	//void SeeAllNode(aiNode* node, const aiScene* scene)
	//{
	//	if (node->mParent)
	//	{
	//		std::cout << "Parented to : " << node->mParent->mName.C_Str() << std::endl;
	//	}
	//	std::cout << "	Node : " << node->mName.C_Str() << std::endl;
	//	for (unsigned int i = 0; i < node->mNumChildren; i++)
	//	{
	//		SeeAllNode(node->mChildren[i], scene);
	//	}
	//}

	void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Model*& model)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			TracyMessageL(std::string("ModelLoader::ProcessNode: Loading ").append(mesh->mName.C_Str()).c_str());
			//std::cout << "Loading " << mesh->mName.C_Str() << std::endl;
			
			if (model->_isAnimated)
			{
				model->_meshes.push_back(ProcessAnimatedMesh(node, mesh, scene, model));
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
				//If it has tangents
				vertexData._tangent.x = mesh->mTangents[i].x;
				vertexData._tangent.y = mesh->mTangents[i].y;
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

	Mesh* ModelLoader::ProcessAnimatedMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model)
	{
		//MaxInfluencePerVertex(mesh);
		Mesh* newAnimatedMesh = new Mesh();
		newAnimatedMesh->_nodeName = mesh->mName.C_Str();
		newAnimatedMesh->_vertexDatas.reserve((size_t)mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Mesh::VertexData vertexData{};
			Mesh::SkinData skinData{};

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
				//If it has tangent
				vertexData._tangent.x = mesh->mTangents[i].x;
				vertexData._tangent.y = mesh->mTangents[i].y;
			}

			//Explicitly zero out all
			skinData._boneID = glm::ivec4(0);
			skinData._boneWeights = glm::vec4(0.0f);

			newAnimatedMesh->_vertexDatas.push_back(vertexData);
			newAnimatedMesh->_skinDatas.push_back(skinData);
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

	void ModelLoader::ProcessBone(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model, Mesh* animatedMesh)
	{
		UNREFERENCED_PARAMETER(scene);
		UNREFERENCED_PARAMETER(node);

		for (size_t i = 0; i < mesh->mNumBones; i++) 
		{
			unsigned boneID = 0;
			std::string boneName = mesh->mBones[i]->mName.C_Str();

			if (model->_rootBone->_boneMapping.find(boneName) == model->_rootBone->_boneMapping.end())
			{
				// Allocate an index for a new bone
				boneID = model->_rootBone->_boneCount;
				model->_rootBone->_boneCount++;
				glm::mat4 boneOffset;
				AssimpToGLM(mesh->mBones[i]->mOffsetMatrix, boneOffset);

				model->_rootBone->_boneMapping[boneName] = std::make_pair(boneID, boneOffset);
			}
			else 
			{
				boneID = model->_rootBone->_boneMapping[boneName].first;
			}

			for (size_t j = 0; j < mesh->mBones[i]->mNumWeights; j++) 
			{
				unsigned vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;

				animatedMesh->_skinDatas[vertexID].AddBoneData(boneID, weight);
			}
		}
	}

	bool ModelLoader::CreateSkeletal(Joint& joint, aiNode* node, const aiScene* scene, Model*& model)
	{
		//Only if node is a bone
		if (model->_rootBone->_boneMapping.find(node->mName.C_Str()) != model->_rootBone->_boneMapping.end())
		{
			joint._boneName = node->mName.C_Str();
			joint._boneID = model->_rootBone->_boneMapping[joint._boneName].first;
			joint._boneTransformOffset = model->_rootBone->_boneMapping[joint._boneName].second;
			AssimpToGLM(node->mTransformation, joint._boneTransform);

			for (unsigned i = 0; i < node->mNumChildren; i++) 
			{
				Joint childJoint;
				if (CreateSkeletal(childJoint, node->mChildren[i], scene, model))
				{
					joint._childrenJoints.push_back(childJoint);
				}
			}
			return true;
		}
		else 
		{
			//Find other bone in children
			for (unsigned i = 0; i < node->mNumChildren; i++) 
			{
				if (CreateSkeletal(joint ,node->mChildren[i], scene, model))
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
		TracyMessageL("ModelLoader::LoadFBX: Loading FBX...");
		//std::cout << "Loading FBX..." << std::endl;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(model->_fileName, aiProcess_Triangulate | 
																 //aiProcess_FlipUVs | 
																 aiProcess_JoinIdenticalVertices |
																 aiProcess_LimitBoneWeights |
																 aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			TracyMessageL(std::string("ModelLoader::LoadFBX: Load Failed. Reason: ").append(import.GetErrorString()).c_str());
			//std::cout << "Load Failed. Reason: " << import.GetErrorString() << std::endl;
			return false;
		}

		if (scene->HasAnimations())
		{
			model->_isAnimated = true;
			model->_meshes.reserve(scene->mNumMeshes);

			AssimpToGLM(scene->mRootNode->mTransformation, model->_globalInverseTransform);
			model->_globalInverseTransform = glm::inverse(model->_globalInverseTransform);
			model->_poseTransform.resize(MAX_BONE_COUNT, glm::mat4(1.0f));

			//First get the vertex info and bone info
			model->_rootBone = new Skeleton();
			ProcessNode(scene->mRootNode, scene, model);
			//Then, create the skeleton hierarchy
			CreateSkeletal(model->_rootBone->_rootJoint, scene->mRootNode, scene, model);
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
			if (!LoadCustomModel(model))
			{
				delete model;
				return;
			}

			//If different pathing, saves another copy in our own asset folder
			/*if (model->_fileName != searchString)
			{
				model->_fileName = s_LocalPathName + name + s_ModelFileType;
				SaveCustomModel(std::string(s_LocalPathName + name),model);
			}*/
		}
		else
		{
			if (!LoadFBX(model))
			{
				delete model;
				return;
			}

			//model->_fileName = s_LocalPathName + name + s_ModelFileType;

			std::string saveFileName = s_LocalPathName + name;
			SaveCustomModel(saveFileName, model);
		}

		_modelManager->AddLoadedModel(model, model->_fileName);

#ifdef LOG_MODEL
		//std::string textFileName = model-> + ".txt";
		DebugToFile(model->_fileName);
#endif

		//TODO Saving custom mesh
	}
	bool ModelLoader::LoadCustomModel(Model*& model)
	{
		//In case of preceding backslash causing file to be unfindable
		if (model->_fileName[0] == '\\')
		{
			model->_fileName.erase(0, 1);
		}
		//Gets rid of warning for now
		std::ifstream modelFile;
		modelFile.open(model->_fileName.c_str());
		std::ifstream animationFile;
		std::ifstream skeletonFile;

		size_t meshSize = 0;
		size_t animSize = 0;

		if (!modelFile.is_open())
		{
			TracyMessageL("ModelLoader::LoadCustomModel: Fail to open model file");
			//std::cout << "Fail to opened model file" << std::endl;
			return false;
		}

		//Gets header info
		std::string input;
		if (!std::getline(modelFile, input))
		{
			TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
			return false;
		}

		if (input.find("IsAnimated") != std::string::npos)
		{
			model->_isAnimated = true;
			
			if (!std::getline(modelFile, input))
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}
			animationFile.open(input.c_str());
			if (!animationFile.is_open())
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Fail to open animation file");
				return false;
			}
				
			if (!std::getline(modelFile, input))
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}
			skeletonFile.open(input.c_str());
			if (!skeletonFile.is_open())
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Fail to open skeleton file");
				return false;
			}		

			if (!std::getline(modelFile, input))
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}
			try 
			{
				meshSize = std::stoi(input);
				model->_meshes.reserve(meshSize);
			}
			catch (std::invalid_argument&) 
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}

			if (!std::getline(modelFile, input))
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}
			try 
			{
				animSize = std::stoi(input);
			}
			catch (std::invalid_argument&) 
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}
		}
		else if (input.find("NotAnimated") != std::string::npos)
		{
			model->_isAnimated = false;
			
			if (!std::getline(modelFile, input))
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}
			try
			{
				meshSize = std::stoi(input);
			}
			catch (std::invalid_argument&)
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted Model File");
				return false;
			}
		}
		else
		{
			TracyMessageL("ModelLoader::LoadCustomModel: Corrupted model file");
			return false;
		}

		for (size_t meshIndex = 0; meshIndex < meshSize; ++meshIndex)
		{
			Mesh* newMesh = new Mesh();
			if (!LoadMeshVertex(modelFile, newMesh, model->_isAnimated))
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted model file");
				return false;
			}
			model->_meshes.push_back(newMesh);
		}

		if (model->_isAnimated)
		{
			for (size_t animCount = 0; animCount < animSize; ++animCount)
			{
				//LoadAnimation();
				//model->_animations.insert();
				Animation* newAnim = new Animation();
				if (!LoadAnimation(animationFile, newAnim))
				{
					TracyMessageL("ModelLoader::LoadCustomModel: Corrupted model file");
					return false;
				}
				model->_animations[newAnim->_animName] = newAnim;
			}

			model->_rootBone = new Skeleton();
			Joint* root = &model->_rootBone->_rootJoint;
			if(!LoadSkeletal(skeletonFile, root))
			{
				TracyMessageL("ModelLoader::LoadCustomModel: Corrupted model file");
				return false;
			}
		}

		if (model->_isAnimated)
		{
			skeletonFile.close();
			animationFile.close();
		}

		return true;
	}
	bool ModelLoader::SaveCustomModel(const std::string& fileName, Model*& model)
	{
		//Gets rid of warning for now
		std::ofstream modelFile;
		modelFile.open(fileName + s_ModelFileType);
		std::ofstream animationFile;
		std::ofstream skeletonFile;
		
		//Sets up header info
		if (model->_isAnimated)
		{
			animationFile.open(fileName + s_AnimationFileType);
			skeletonFile.open(fileName + s_SkeletonFileType);

			modelFile << "IsAnimated\n";
			modelFile << fileName + s_AnimationFileType + "\n";
			modelFile << fileName + s_SkeletonFileType + "\n";
			modelFile << model->_meshes.size() << "mesh\n";
			modelFile << model->_animations.size() << "anim\n";
		}
		else
		{
			modelFile << "NotAnimated\n";
			modelFile << model->_meshes.size() << "mesh" << "\n";
		}

		auto meshIterator = model->_meshes.begin();
		auto meshIteratorEnd = model->_meshes.end();

		while (meshIterator != meshIteratorEnd)
		{
			SaveMeshVertex(modelFile, *meshIterator, model->_isAnimated);
			++meshIterator;
		}

		if (model->_isAnimated)
		{
			//animationFile << model->_animations.size() << "anim\n";
			for (auto anim : model->_animations)
			{
				SaveAnimation(animationFile, anim.second);
			}

			Joint* root = &model->_rootBone->_rootJoint;
			SaveSkeletal(skeletonFile, root);
		}

		if (model->_isAnimated)
		{
			skeletonFile.close();
			animationFile.close();
		}
		modelFile.close();
		return true;
	}
	void ModelLoader::SaveMeshVertex(std::ofstream& file, Mesh*& mesh, bool animated)
	{
		size_t vertexCount = mesh->_vertexDatas.size();

		file << vertexCount << "vertex" << "\n";
		//file << mesh->_nodeName << "\n";
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

			file << "t: " << mesh->_vertexDatas[i]._tangent.x << ","
				<< mesh->_vertexDatas[i]._tangent.y << ","
				<< mesh->_vertexDatas[i]._tangent.z
				<< "\n";

			if (animated)
			{
				file << "id: " << mesh->_skinDatas[i]._boneID.x << ","
					<< mesh->_skinDatas[i]._boneID.y << ","
					<< mesh->_skinDatas[i]._boneID.z << ","
					<< mesh->_skinDatas[i]._boneID.w << "\n";

				file << "weights: " << mesh->_skinDatas[i]._boneWeights.x << ","
					<< mesh->_skinDatas[i]._boneWeights.y << ","
					<< mesh->_skinDatas[i]._boneWeights.z << ","
					<< mesh->_skinDatas[i]._boneWeights.w << "\n";
			}
		}
		file << mesh->_indices.size() << "indices" << "\n";
		int nextLineCount = 0;
		for (auto& indices : mesh->_indices)
		{
			file << indices;
			nextLineCount++;

			if (nextLineCount == 6)
			{
				file << "\n";
				nextLineCount = 0;
			}
			else
			{
				file << ",";
			}
		}
	}
	bool ModelLoader::LoadMeshVertex(std::ifstream& file, Mesh*& mesh, bool animated)
	{
		UNREFERENCED_PARAMETER(mesh);
		UNREFERENCED_PARAMETER(file);
		UNREFERENCED_PARAMETER(animated);
		std::string input;
		size_t vertexCount = 0;

		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
			return false;
		}

		try
		{
			vertexCount = std::stoi(input);
			mesh->_vertexDatas.reserve(vertexCount);

			if (animated)
			{
				mesh->_skinDatas.reserve(vertexCount);
			}
		}
		catch (std::invalid_argument&)
		{
			TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
			return false;
		}

		for (size_t i = 0; i < vertexCount; ++i)
		{
			Mesh::VertexData newVertex{};
			Mesh::SkinData newSkin{};
			std::string values;
			std::string toConvert;

			//Gets position
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
				return false;
			}
			values = input.substr(input.find(" ")+1);
			std::stringstream posValues(values);
			int posIndex = 0;
			while (std::getline(posValues, toConvert, ','))
			{
				try
				{
					if (posIndex == 3)
					{
						TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
						return false;
					}
					newVertex._position[posIndex] = std::stof(toConvert);
				}
				catch (std::invalid_argument&)
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
					return false;
				}

				posIndex++;
			}
			if (posIndex != 3)
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
				return false;
			}

			//Gets UV
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
				return false;
			}
			values = input.substr(input.find(" ") + 1);
			std::stringstream uvValues(values);
			int uvIndex = 0;
			while (std::getline(uvValues, toConvert, ','))
			{
				try
				{
					if (uvIndex == 2)
					{
						TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
						return false;
					}
					newVertex._uv[uvIndex] = std::stof(toConvert);
				}
				catch (std::invalid_argument&)
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
					return false;
				}

				uvIndex++;
			}
			if (uvIndex != 2)
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
				return false;
			}

			//Gets normals
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
				return false;
			}
			values = input.substr(input.find(" ") + 1);
			std::stringstream normValues(values);
			int normIndex = 0;
			while (std::getline(normValues, toConvert, ','))
			{
				try
				{
					if (normIndex == 3)
					{
						TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
						return false;
					}
					newVertex._normals[normIndex] = std::stof(toConvert);
				}
				catch (std::invalid_argument&)
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
					return false;
				}

				normIndex++;
			}
			if (normIndex != 3)
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
				return false;
			}

			//Gets tangents
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
				return false;
			}
			values = input.substr(input.find(" ") + 1);
			std::stringstream tanValues(values);
			int tanIndex = 0;
			while (std::getline(tanValues, toConvert, ','))
			{
				try
				{
					if (tanIndex == 3)
					{
						TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
						return false;
					}
					newVertex._tangent[tanIndex] = std::stof(toConvert);
				}
				catch (std::invalid_argument&)
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
					return false;
				}

				tanIndex++;
			}
			if (tanIndex != 3)
			{
				TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
				return false;
			}

			if (animated)
			{
				//Gets bone id
				if (!std::getline(file, input))
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
					return false;
				}
				values = input.substr(input.find(" ") + 1);
				std::stringstream idValues(values);
				int idIndex = 0;
				while (std::getline(idValues, toConvert, ','))
				{
					try
					{
						if (idIndex == 4)
						{
							TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
							return false;
						}
						newSkin._boneID[idIndex] = std::stoi(toConvert);
					}
					catch (std::invalid_argument&)
					{
						TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
						return false;
					}

					idIndex++;
				}
				if (idIndex != 4)
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
					return false;
				}

				//Gets bone weights
				if (!std::getline(file, input))
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
					return false;
				}
				values = input.substr(input.find(" ") + 1);
				std::stringstream weightValues(values);
				int weightIndex = 0;
				while (std::getline(weightValues, toConvert, ','))
				{
					try
					{
						if (weightIndex == 4)
						{
							TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
							return false;
						}
						newSkin._boneWeights[weightIndex] = std::stof(toConvert);
					}
					catch (std::invalid_argument&)
					{
						TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
						return false;
					}

					weightIndex++;
				}
				if (weightIndex != 4)
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
					return false;
				}
			}

			mesh->_vertexDatas.push_back(newVertex);
			if (animated)
			{
				mesh->_skinDatas.push_back(newSkin);
			}
		}

		//Get indices count
		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted model file");
			return false;
		}
		size_t indiceCount;
		std::string index;
		try
		{
			indiceCount = std::stoi(input);
			mesh->_indices.reserve(indiceCount);
		}
		catch (std::invalid_argument&)
		{
			TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
			return false;
		}
		int indicesIndex = 0;
		//Gets all indices
		while (std::getline(file, input))
		{
			std::stringstream indicesValue(input);

			while (std::getline(indicesValue, index, ','))
			{
				try
				{
					if (indicesIndex >= indiceCount)
					{
						TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
						return false;
					}
					mesh->_indices.push_back(std::stoi(index));
					++indicesIndex;
				}
				catch (std::invalid_argument&)
				{
					TracyMessageL("ModelLoader::LoadMeshVertex: Corrupted Model File");
					return false;
				}
			}

			if (mesh->_indices.size() == indiceCount)
			{
				break;
			}
		}

		return true;
	}
	void ModelLoader::SaveSkeletal(std::ofstream& file, Joint*& joint)
	{
		file << joint->_boneID << "\n" << joint->_boneName << "\n";
		
		file << "o"
			<< joint->_boneTransformOffset[0][0] << ","
			<< joint->_boneTransformOffset[0][1] << ","
			<< joint->_boneTransformOffset[0][2] << ","
			<< joint->_boneTransformOffset[0][3] << ","
			<< joint->_boneTransformOffset[1][0] << ","
			<< joint->_boneTransformOffset[1][1] << ","
			<< joint->_boneTransformOffset[1][2] << ","
			<< joint->_boneTransformOffset[1][3] << ","
			<< joint->_boneTransformOffset[2][0] << ","
			<< joint->_boneTransformOffset[2][1] << ","
			<< joint->_boneTransformOffset[2][2] << ","
			<< joint->_boneTransformOffset[2][3] << ","
			<< joint->_boneTransformOffset[3][0] << ","
			<< joint->_boneTransformOffset[3][1] << ","
			<< joint->_boneTransformOffset[3][2] << ","
			<< joint->_boneTransformOffset[3][3]
			<< "\n";

		file << "t"
			<< joint->_boneTransform[0][0] << ","
			<< joint->_boneTransform[0][1] << ","
			<< joint->_boneTransform[0][2] << ","
			<< joint->_boneTransform[0][3] << ","
			<< joint->_boneTransform[1][0] << ","
			<< joint->_boneTransform[1][1] << ","
			<< joint->_boneTransform[1][2] << ","
			<< joint->_boneTransform[1][3] << ","
			<< joint->_boneTransform[2][0] << ","
			<< joint->_boneTransform[2][1] << ","
			<< joint->_boneTransform[2][2] << ","
			<< joint->_boneTransform[2][3] << ","
			<< joint->_boneTransform[3][0] << ","
			<< joint->_boneTransform[3][1] << ","
			<< joint->_boneTransform[3][2] << ","
			<< joint->_boneTransform[3][3]
			<< "\n";

		file << joint->_childrenJoints.size() << "\n";

		for (size_t boneIndex = 0; boneIndex < joint->_childrenJoints.size(); ++boneIndex)
		{
			Joint* childJoint = &joint->_childrenJoints[boneIndex];
			SaveSkeletal(file, childJoint);
		}
	}
	bool ModelLoader::LoadSkeletal(std::ifstream& file, Joint*& joint)
	{
		UNREFERENCED_PARAMETER(file);
		UNREFERENCED_PARAMETER(joint);

		std::string input;
		//Get Bone ID
		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}
		try
		{
			joint->_boneID = std::stoi(input);
		}
		catch (std::invalid_argument&)
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}

		//Get bone name
		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}
		try
		{
			joint->_boneName = input;
		}
		catch (std::invalid_argument&)
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}

		//Get transform offset
		int to = 0;
		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}
		std::string values = input.substr(1);
		std::stringstream transOValues(values);
		std::string toConvert;
		while (std::getline(transOValues, toConvert, ','))
		{
			try
			{
				if (to == 16)
				{
					TracyMessageL("ModelLoader::LoadSkeletal: Corrupted Model File");
					return false;
				}

				joint->_boneTransformOffset[to/4][to%4] = std::stof(toConvert);

			}
			catch (std::invalid_argument&)
			{
				TracyMessageL("ModelLoader::LoadSkeletal: Corrupted Model File");
				return false;
			}

			++to;
		}
		if (to != 16)
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted Model File");
			return false;
		}

		//Get transform
		int t = 0;
		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}
		values = input.substr(1);
		std::stringstream transValues(values);
		while (std::getline(transValues, toConvert, ','))
		{
			try
			{
				if (t == 16)
				{
					TracyMessageL("ModelLoader::LoadSkeletal: Corrupted Model File");
					return false;
				}
				joint->_boneTransform[t / 4][t % 4] = std::stof(toConvert);

			}
			catch (std::invalid_argument&)
			{
				TracyMessageL("ModelLoader::LoadSkeletal: Corrupted Model File");
				return false;
			}

			++t;
		}
		if (t != 16)
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted Model File");
			return false;
		}

		size_t childSize = 0;
		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}
		try
		{
			childSize = std::stoi(input);
		}
		catch (std::invalid_argument&)
		{
			TracyMessageL("ModelLoader::LoadSkeletal: Corrupted model file");
			return false;
		}

		for (size_t childIndex = 0; childIndex < childSize; ++childIndex)
		{
			joint->_childrenJoints.push_back({});
			Joint* childJoint = &joint->_childrenJoints[childIndex];

			LoadSkeletal(file, childJoint);
		}

		return true;
	}
	void ModelLoader::SaveAnimation(std::ofstream& file, Animation*& anim)
	{
		file << anim->_animName << "," << anim->_time << "\n";
		file << anim->_frames.size() << "\n";

		for (auto frame : anim->_frames)
		{
			file << frame.first << "\n";

			//Position & Time
			size_t posSize = frame.second._position.size();
			file << posSize << "posFrame\n";
			for (size_t posIndex = 0;posIndex < posSize;++posIndex)
			{
				file << "p" << frame.second._posTime[posIndex] << ","
					<< frame.second._position[posIndex].x << ","
					<< frame.second._position[posIndex].y << ","
					<< frame.second._position[posIndex].z << "\n";	
			}

			size_t scaleSize = frame.second._scale.size();
			file << scaleSize << "scaleFrame\n";
			for (size_t scaleIndex = 0; scaleIndex < scaleSize; ++scaleIndex)
			{
				file << "s" << frame.second._scaleTime[scaleIndex] << ","
					<< frame.second._scale[scaleIndex].x << ","
					<< frame.second._scale[scaleIndex].y << ","
					<< frame.second._scale[scaleIndex].z << "\n";
			}

			size_t rotSize = frame.second._rotation.size();
			file << rotSize << "rotFrame\n";
			for (size_t rotIndex = 0; rotIndex < rotSize; ++rotIndex)
			{
				file << "r" << frame.second._rotateTime[rotIndex] << ","
					<< frame.second._rotation[rotIndex].x << ","
					<< frame.second._rotation[rotIndex].y << ","
					<< frame.second._rotation[rotIndex].z << ","
					<< frame.second._rotation[rotIndex].w << "\n";
			}
		}
	}

	bool ModelLoader::LoadAnimation(std::ifstream& file, Animation*& anim)
	{
		UNREFERENCED_PARAMETER(file);
		UNREFERENCED_PARAMETER(anim);
		std::string input;
		size_t frameSize = 0;

		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
			return false;
		}

		std::string animName = input.substr(0, input.find(","));
		std::string time = input.substr(input.find(",") + 1);
		anim->_animName = animName;
		try
		{
			anim->_time = std::stod(time);
		}
		catch (std::invalid_argument&)
		{
			TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
			return false;
		}
	

		if (!std::getline(file, input))
		{
			TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
			return false;
		}
		try
		{
			frameSize = std::stoi(input);
		}
		catch (std::invalid_argument&)
		{
			TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
			return false;
		}

		for (size_t frameIndex = 0; frameIndex < frameSize; ++frameIndex)
		{
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
				return false;
			}
			std::string boneName = input;
			anim->_frames[boneName] = Animation::KeyFrames{};

			//Get Position
			size_t posSize = 0;
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
				return false;
			}
			try
			{
				posSize = std::stoi(input);
			}
			catch (std::invalid_argument&)
			{
				TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
				return false;
			}

			for (size_t posIndex = 0; posIndex < posSize; ++posIndex)
			{
				int p = 0;
				if (!std::getline(file, input))
				{
					TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
					return false;
				}
				//Skips the p
				std::string values = input.substr(1);
				std::stringstream posValues(values);
				std::string toConvert;
				glm::vec3 pos;
				while (std::getline(posValues, toConvert, ','))
				{
					try
					{
						if (p == 4)
						{
							TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
							return false;
						}
						if (p == 0)
						{
							anim->_frames[boneName]._posTime.push_back(std::stod(toConvert));
						}
						else
						{
							pos[p - 1] = std::stof(toConvert);
						}

					}
					catch (std::invalid_argument&)
					{
						TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
						return false;
					}

					++p;
				}
				if (p != 4)
				{
					TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
					return false;
				}

				anim->_frames[boneName]._position.push_back(pos);
			}

			//Get Scale
			size_t scaleSize = 0;
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
				return false;
			}
			try
			{
				scaleSize = std::stoi(input);
			}
			catch (std::invalid_argument&)
			{
				TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
				return false;
			}

			for (size_t scaleIndex = 0; scaleIndex < scaleSize; ++scaleIndex)
			{
				int s = 0;
				if (!std::getline(file, input))
				{
					TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
					return false;
				}
				//Skips the s
				std::string values = input.substr(1);
				std::stringstream scaleValues(values);
				std::string toConvert;
				glm::vec3 scale;
				while (std::getline(scaleValues, toConvert, ','))
				{
					try
					{
						if (s == 4)
						{
							TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
							return false;
						}
						if (s == 0)
						{
							anim->_frames[boneName]._scaleTime.push_back(std::stod(toConvert));
						}
						else
						{
							scale[s - 1] = std::stof(toConvert);
						}

					}
					catch (std::invalid_argument&)
					{
						TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
						return false;
					}

					++s;
				}
				if (s != 4)
				{
					TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
					return false;
				}

				anim->_frames[boneName]._scale.push_back(scale);
			}

			//Get Rotate
			size_t rotSize = 0;
			if (!std::getline(file, input))
			{
				TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
				return false;
			}
			try
			{
				rotSize = std::stoi(input);
			}
			catch (std::invalid_argument&)
			{
				TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
				return false;
			}

			for (size_t rotIndex = 0; rotIndex < rotSize; ++rotIndex)
			{
				int r = 0;
				if (!std::getline(file, input))
				{
					TracyMessageL("ModelLoader::LoadAnimation: Corrupted model file");
					return false;
				}
				//Skips the r
				std::string values = input.substr(1);
				std::stringstream rotValues(values);
				std::string toConvert;
				glm::quat rot;
				while (std::getline(rotValues, toConvert, ','))
				{
					try
					{
						if (r == 5)
						{
							TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
							return false;
						}
						if (r == 0)
						{
							anim->_frames[boneName]._rotateTime.push_back(std::stod(toConvert));
						}
						else
						{
							rot[r - 1] = std::stof(toConvert);
						}

					}
					catch (std::invalid_argument&)
					{
						TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
						return false;
					}

					++r;
				}
				if (r != 5)
				{
					TracyMessageL("ModelLoader::LoadAnimation: Corrupted Model File");
					return false;
				}

				anim->_frames[boneName]._rotation.push_back(rot);
			}
		}

		return true;
	}

	/*void ModelLoader::DebugToFile(const std::string& fileName)
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
	}*/
}