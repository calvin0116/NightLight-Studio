#pragma once
#include "../FBX_SDK/fbxsdk.h"

namespace NS_GRAPHICS
{
	class ModelLoader
	{
		FbxManager* _fbxManager;
		FbxScene* _fbxScene;
		FbxImporter* _fbxImport;

		ModelLoader();
		~ModelLoader();

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		//void LoadFBX(const std::string& fileName, Mesh& mesh);
		void LoadFBX(const std::string& fileName);
	};
}