#pragma once
#include "FBX_SDK/fbxsdk.h"
#include "Model.h"

namespace NS_GRAPHICS
{
	class ModelLoader
	{
		FbxManager* _fbxManager;
		FbxScene* _fbxScene;
		FbxImporter* _fbxImport;
		FbxAxisSystem _axisSystem;

		ModelLoader();
		~ModelLoader();

		void TransverseChild(FbxNode* node, Model*& model);

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		void Init();

		//void LoadFBX(const std::string& fileName, Mesh& mesh);
		void LoadFBX(const std::string& fileName, Model*& model);
	};
}