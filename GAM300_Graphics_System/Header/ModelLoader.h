#pragma once
#include "FBX_SDK/fbxsdk.h"
#include "../Mesh.h"

namespace NS_GRAPHICS
{
	class ModelLoader
	{
		FbxManager* _fbxManager;

		ModelLoader();
		~ModelLoader();

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		void LoadFBX(const std::string& fileName, Mesh& mesh);
	};
}