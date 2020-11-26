#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"
#include "ModelManager.h"
#include "Model.h"
#include "../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

namespace NS_GRAPHICS
{
	static std::string s_FbxFileFormat = ".fbx";

	class ModelLoader
	{
		ModelManager* _modelManager;

		ModelLoader();
		~ModelLoader();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Default functions to get vertex data from assimp
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ProcessNode(aiNode* node, const aiScene* scene, Model*& model);
		Mesh* ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model);
		AnimatedMesh* ProcessAnimatedMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Needed for animations
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ProcessBone(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model, AnimatedMesh* animatedMesh);
		bool CreateSkeletal(BoneData& bone, aiNode* node, const aiScene* scene, Model*& model);
		void ProcessAnimation(aiNode* node, const aiScene* scene, Model*& model);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Helper Functions for conversion
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AssimpToGLM(const aiMatrix4x4& ai, glm::mat4& glm);
		void AssimpToGLM(const aiVector3D& ai, glm::vec3& glm);
		void AssimpToGLM(const aiQuaternion& ai, glm::quat& glm);

		//void CalcInterpolatedScaling(glm::vec3& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		//void CalcInterpolatedRotation(glm::quat& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		//void CalcInterpolatedPosition(glm::vec3& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		//unsigned FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		//unsigned FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		//unsigned FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		//const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
		//void ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, const aiNode* pNode, const glm::mat4x4& ParentTransform, Model* model);

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		//void LoadFBX(const std::string& fileName, Mesh& mesh);
		bool LoadFBX(Model*& model);
		void LoadModel(const std::string& fileName);
		bool LoadCustomMesh(Model*& model);
		bool SaveCustomMesh(Model*& model);

		void DebugToFile(const std::string& fileName);
	};
}