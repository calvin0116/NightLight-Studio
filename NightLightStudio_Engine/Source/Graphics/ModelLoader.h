#pragma once
#include "../FBX_SDK/fbxsdk.h"
#include "Mesh.h"
#include "MeshManager.h"

namespace NS_GRAPHICS
{
	static std::string s_FbxFileFormat = ".fbx";

	class ModelLoader
	{
		FbxManager* _fbxManager;
		FbxScene* _fbxScene;
		FbxImporter* _fbxImport;
		FbxAxisSystem _axisSystem;
		std::string _currentFile;

		ModelLoader();
		~ModelLoader();

		void TransverseChild(FbxNode* node, Mesh*& newMesh, unsigned offset = 0);

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		void Init();

		//void LoadFBX(const std::string& fileName, Mesh& mesh);
		unsigned LoadFBX(const std::string& fileName, const std::string& customName = "");
		unsigned LoadModel(const std::string& fileName, const std::string& customName = "");
		unsigned LoadCustomMesh(const std::string& fileName, const std::string& customName ="");
	};
}