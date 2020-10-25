#pragma once
#include "../FBX_SDK/fbxsdk.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "Model.h"

namespace NS_GRAPHICS
{
	static std::string s_FbxFileFormat = ".fbx";

	class ModelLoader
	{
		FbxManager* _fbxManager;
		FbxScene* _fbxScene;
		FbxImporter* _fbxImport;
		FbxAxisSystem _axisSystem;

		ModelLoader();
		~ModelLoader();

		void TransverseChild(FbxNode* node, Model*& model, int* meshIndex, const std::string& fileName, const std::string& customName = "");

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		void Init();

		//void LoadFBX(const std::string& fileName, Mesh& mesh);
		void LoadFBX(Model*& model, const std::string& fileName, const std::string& customName = "");
		void LoadModel(const std::string& fileName, const std::string& customName = "");
		void LoadCustomMesh(Model*& model, const std::string& fileName, const std::string& customName ="");
	};
}