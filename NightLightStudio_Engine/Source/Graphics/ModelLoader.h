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
		Mesh* ProcessAnimatedMesh(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Needed for animations
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ProcessBone(aiNode* node, aiMesh* mesh, const aiScene* scene, Model*& model, Mesh* animatedMesh);
		bool CreateSkeletal(Joint& joint, aiNode* node, const aiScene* scene, Model*& model);
		void ProcessAnimation(aiNode* node, const aiScene* scene, Model*& model);
		void FillNode(Node& ourNode, aiNode* node, const aiScene* scene, Model*& model);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Helper Functions for conversion
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AssimpToGLM(const aiMatrix4x4& ai, glm::mat4& glm);
		void AssimpToGLM(const aiVector3D& ai, glm::vec3& glm);
		void AssimpToGLM(const aiQuaternion& ai, glm::quat& glm);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Loading & Saving Helper Functions
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool LoadFBX(Model*& model);

		bool LoadCustomModel(Model*& model);
		bool SaveCustomModel(const std::string& name,Model*& model);

		void SaveMeshVertex(std::ofstream& file, Mesh*& mesh, bool animated = false);
		bool LoadMeshVertex(std::ifstream& file, Mesh*& mesh, bool animated = false);

		void SaveSkeletal(std::ofstream& file, Joint*& joint);
		bool LoadSkeletal(std::ifstream& file, Joint*& joint);

		void SaveAnimation(std::ofstream& file, Animation*& anim);
		bool LoadAnimation(std::ifstream& file, Animation*& anim);


		//Just for debugging
		//void DebugToFile(const std::string& fileName);

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Main function that is called to load model
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void LoadModel(const std::string& fileName);
	};
}