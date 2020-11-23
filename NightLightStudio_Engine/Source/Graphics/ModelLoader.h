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

		void ProcessNode(aiNode* node, const aiScene* scene, Model*& model);
		Mesh* ProcessMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model);
		AnimatedMesh* ProcessAnimatedMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model);

		void ProcessBone(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model, AnimatedMesh* animatedMesh);

		void AiToGLM(const aiMatrix4x4& ai, glm::mat4& glm);

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