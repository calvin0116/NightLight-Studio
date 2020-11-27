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
		for (int i = 0; i < mesh->mNumBones; i++) {
			const aiBone* bone = mesh->mBones[i];
			for (int j = 0; j < bone->mNumWeights; j++) {
				const aiVertexWeight* weight = &bone->mWeights[j];
				counts[weight->mVertexId]++;
				weights[weight->mVertexId] += weight->mWeight;
			}
		}
		int max = 0;
		int index = 0;
		for (int i = 0; i < mesh->mNumVertices; i++) {
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

				//model->_bones.push_back(bone);
				
				//std::cout << "Bone Weights : ";
				//for (unsigned int k = 0; k < mesh->mBones[j]->mNumWeights; k++)
				//{
				//	std::cout << mesh->mBones[j]->mWeights[k].mWeight << " ";
				//}
				//std::cout << std::endl;
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
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model);
		}
	}

	Mesh* ModelLoader::ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model)
	{
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

			for (int i = 0; i < node->mNumChildren; i++) 
			{
				BoneData childBone;
				CreateSkeletal(childBone, node->mChildren[i], scene, model);
				bone._childrenBones.push_back(childBone);
			}
			return true;
		}
		else 
		{
			//Find other bone in children
			for (int i = 0; i < node->mNumChildren; i++) 
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
		for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
		{
			Animation* newAnim = new Animation();
			aiAnimation* currAnim = scene->mAnimations[i];
			newAnim->_animName = scene->mAnimations[i]->mName.C_Str();

			if (currAnim->mTicksPerSecond != 0.0f)
			{
				newAnim->_ticksPerSecond = currAnim->mTicksPerSecond;
			}
			else
			{
				newAnim->_ticksPerSecond = 1.0f;
			}

			newAnim->_time = currAnim->mDuration * currAnim->mTicksPerSecond;

			for (unsigned int x = 0; x < currAnim->mNumChannels; ++x)
			{
				Animation::KeyFrames currFrame;

				for (unsigned int y = 0; y < currAnim->mChannels[x]->mNumPositionKeys; ++y)
				{
					currFrame._posTime.push_back(currAnim->mChannels[x]->mPositionKeys[y].mTime);
					glm::vec3 pos = glm::vec3(0.0f);
					AssimpToGLM(currAnim->mChannels[x]->mPositionKeys[y].mValue, pos);
					currFrame._position.push_back(pos);
				}

				for (unsigned int y = 0; y < currAnim->mChannels[x]->mNumRotationKeys; ++y)
				{
					currFrame._rotateTime.push_back(currAnim->mChannels[x]->mRotationKeys[y].mTime);
					glm::quat rotate;
					AssimpToGLM(currAnim->mChannels[x]->mRotationKeys[y].mValue, rotate);
					currFrame._rotation.push_back(rotate);
				}

				for (unsigned int y = 0; y < currAnim->mChannels[x]->mNumScalingKeys; ++y)
				{
					currFrame._scaleTime.push_back(currAnim->mChannels[x]->mScalingKeys[y].mTime);
					glm::vec3 scale = glm::vec3(0.0f);
					AssimpToGLM(currAnim->mChannels[x]->mScalingKeys[y].mValue, scale);
					currFrame._scale.push_back(scale);
				}

				newAnim->_frames[currAnim->mChannels[x]->mNodeName.C_Str()] = currFrame;
			}

			/*	float maxTime = newAnim->_frames[newAnim->_animName]._posTime[newAnim->_frames[newAnim->_animName]._posTime.size() - 1];
			if (newAnim->_time < newAnim->_frames[newAnim->_animName]._posTime[newAnim->_frames[newAnim->_animName]._posTime.size() - 1])
			{
				newAnim->_time = newAnim->_frames[newAnim->_animName]._posTime[newAnim->_frames[newAnim->_animName]._posTime.size() - 1];
			}*/

			model->_animations[newAnim->_animName] = newAnim;
		}
	}

	void ModelLoader::AssimpToGLM(const aiMatrix4x4& ai, glm::mat4& glm)
	{
		glm[0][0] = ai.a1; 
		glm[0][1] = ai.b1;  
		glm[0][2] = ai.c1; 
		glm[0][3] = ai.d1;

		glm[1][0] = ai.a2;
		glm[1][1] = ai.b2;  
		glm[1][2] = ai.c2;
		glm[1][3] = ai.d2;

		glm[2][0] = ai.a3; 
		glm[2][1] = ai.b3;  
		glm[2][2] = ai.c3; 
		glm[2][3] = ai.d3;

		glm[3][0] = ai.a4; 
		glm[3][1] = ai.b4;  
		glm[3][2] = ai.c4; 
		glm[3][3] = ai.d4;
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

	//void ModelLoader::CalcInterpolatedScaling(glm::vec3& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	//{
	//	if (pNodeAnim->mNumScalingKeys == 1) {
	//		Out.x = pNodeAnim->mScalingKeys[0].mValue.x;
	//		Out.y = pNodeAnim->mScalingKeys[0].mValue.y;
	//		Out.z = pNodeAnim->mScalingKeys[0].mValue.z;
	//		return;
	//	}

	//	unsigned ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	//	unsigned NextScalingIndex = (ScalingIndex + 1);
	//	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	//	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	//	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	//	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	//	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	//	aiVector3D Delta = End - Start;
	//	glm::vec3 deltaScale;
	//	deltaScale.x = Delta.x;
	//	deltaScale.y = Delta.y;
	//	deltaScale.z = Delta.z;

	//	glm::vec3 startScale;
	//	startScale.x = Start.x;
	//	startScale.y = Start.y;
	//	startScale.z = Start.z;

	//	glm::vec3 endScale;
	//	endScale.x = End.x;
	//	endScale.y = End.y;
	//	endScale.z = End.z;

	//	Out = startScale + endScale * deltaScale;
	//}

	//void ModelLoader::CalcInterpolatedRotation(glm::quat& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	//{
	//	// we need at least two values to interpolate...
	//	if (pNodeAnim->mNumRotationKeys == 1) {
	//		Out.x = pNodeAnim->mRotationKeys[0].mValue.x;
	//		Out.y = pNodeAnim->mRotationKeys[0].mValue.y;
	//		Out.z = pNodeAnim->mRotationKeys[0].mValue.z;
	//		Out.w = pNodeAnim->mRotationKeys[0].mValue.w;
	//		return;
	//	}

	//	unsigned RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	//	unsigned NextRotationIndex = (RotationIndex + 1);
	//	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	//	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	//	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	//	aiQuaternion currentRotate;
	//	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	//	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	//	aiQuaternion::Interpolate(currentRotate, StartRotationQ, EndRotationQ, Factor);
	//	currentRotate = currentRotate.Normalize();

	//	Out.x = currentRotate.x;
	//	Out.y = currentRotate.y;
	//	Out.z = currentRotate.z;
	//	Out.w = currentRotate.w;
	//}

	//void ModelLoader::CalcInterpolatedPosition(glm::vec3& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	//{
	//	if (pNodeAnim->mNumPositionKeys == 1) {
	//		Out.x = pNodeAnim->mPositionKeys[0].mValue.x;
	//		Out.y = pNodeAnim->mPositionKeys[0].mValue.y;
	//		Out.z = pNodeAnim->mPositionKeys[0].mValue.z;
	//		return;
	//	}

	//	unsigned PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	//	unsigned NextPositionIndex = (PositionIndex + 1);
	//	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	//	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	//	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	//	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	//	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	//	aiVector3D Delta = End - Start;
	//	
	//	glm::vec3 deltaPos;
	//	deltaPos.x = Delta.x;
	//	deltaPos.y = Delta.y;
	//	deltaPos.z = Delta.z;

	//	glm::vec3 startPos;
	//	startPos.x = Start.x;
	//	startPos.y = Start.y;
	//	startPos.z = Start.z;

	//	glm::vec3 endPos;
	//	endPos.x = End.x;
	//	endPos.y = End.y;
	//	endPos.z = End.z;

	//	Out = startPos + endPos * deltaPos;
	//}

	//unsigned ModelLoader::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	//{
	//	if (pNodeAnim->mNumScalingKeys > 0)
	//	{
	//		for (unsigned i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
	//			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
	//				return i;
	//			}
	//		}
	//	}
	//	return 0;
	//}

	//unsigned ModelLoader::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	//{
	//	if (pNodeAnim->mNumRotationKeys > 0)
	//	{
	//		for (unsigned i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
	//			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
	//				return i;
	//			}
	//		}
	//	}

	//	return 0;
	//}

	//unsigned ModelLoader::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	//{

	//	for (unsigned i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
	//		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
	//			return i;
	//		}
	//	}

	//	return 0;
	//}

	//const aiNodeAnim* ModelLoader::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
	//{
	//	for (unsigned i = 0; i < pAnimation->mNumChannels; i++) {
	//		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

	//		if (string(pNodeAnim->mNodeName.data) == NodeName) {
	//			return pNodeAnim;
	//		}
	//	}

	//	return NULL;
	//}

	//void ModelLoader::ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, const aiNode* pNode, const glm::mat4x4& ParentTransform, Model* model)
	//{
	//	string NodeName(pNode->mName.data);

	//	const aiAnimation* pAnimation = scene->mAnimations[0];

	//	
	//	glm::mat4 NodeTransformation;

	//	AssimpToGLM(pNode->mTransformation, NodeTransformation);

	//	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	//	if (pNodeAnim) {
	//		// Interpolate scaling and generate scaling transformation matrix
	//		//aiVector3D Scaling;
	//		glm::vec3 Scaling;
	//		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
	//		glm::mat4 ScalingM;
	//		glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));
	//		//ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

	//		// Interpolate rotation and generate rotation transformation matrix
	//		glm::quat rot;
	//		CalcInterpolatedRotation(rot, AnimationTime, pNodeAnim);
	//		glm::mat4 rotMat = glm::mat4_cast(rot);

	//		// Interpolate translation and generate translation transformation matrix
	//		glm::vec3 Translation;
	//		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
	//		//Matrix4f TranslationM;
	//		glm::mat4 TranslationM;
	//		glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));
	//		//TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

	//		// Combine the above transformations
	//		NodeTransformation = TranslationM * rotMat * ScalingM;
	//	}

	//	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	//	if (model->_boneMapping.find(NodeName) != model->_boneMapping.end()) {
	//		unsigned BoneIndex = model->_boneMapping[NodeName];
	//		//model->_bones[BoneIndex]._boneTransform = model->_globalInverseTransform * GlobalTransformation * model->_bones[BoneIndex]._boneTransformOffset;
	//	}

	//	for (unsigned i = 0; i < pNode->mNumChildren; i++) {
	//		ReadNodeHeirarchy(scene, AnimationTime, pNode->mChildren[i], GlobalTransformation, model);
	//	}
	//}

	bool ModelLoader::LoadFBX(Model*& model)
	{
		std::cout << "Loading FBX..." << std::endl;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(model->_fileName, aiProcess_Triangulate | 
																 aiProcess_FlipUVs | 
																 aiProcess_JoinIdenticalVertices |
																 aiProcess_LimitBoneWeights );

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "Load Failed. Reason: " << import.GetErrorString() << endl;
			return false;
		}

		SeeAllNode(scene->mRootNode, scene);

		if (scene->HasAnimations())
		{
			//SeeAllSkeleton(scene->mRootNode, scene, model);
			//SeeAllAnimation(scene->mRootNode, scene);

			//SeeAllAnimation(scene->mRootNode, scene);

			model->_isAnimated = true;
			model->_animatedMeshes.reserve(scene->mNumMeshes);

			AssimpToGLM(scene->mRootNode->mTransformation, model->_globalInverseTransform);
			model->_globalInverseTransform = glm::inverse(model->_globalInverseTransform);
			model->_poseTransform.resize(MAX_BONE_COUNT, glm::mat4(1.0f));

			ProcessNode(scene->mRootNode, scene, model);
			//Creates the skeleton hierarchy
			CreateSkeletal(model->_rootBone, scene->mRootNode, scene, model);
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