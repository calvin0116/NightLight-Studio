#include "ModelLoader.h"

namespace NS_GRAPHICS
{
	ModelLoader::ModelLoader()
	{
		_fbxManager = FbxManager::Create();
		FbxIOSettings* settings = FbxIOSettings::Create(_fbxManager, IOSROOT);
		_fbxManager->SetIOSettings(settings);
	}

	ModelLoader::~ModelLoader()
	{
		_fbxManager->Destroy();
	}
	void ModelLoader::LoadFBX(const std::string& fileName, Mesh& mesh)
	{
		FbxImporter* fbxImport = FbxImporter::Create(_fbxManager, "");

		if (!fbxImport->Initialize(fileName.c_str(), -1, _fbxManager->GetIOSettings())) {
			exit(-1);
		}

		mesh.fbxScene = FbxScene::Create(_fbxManager, fileName.c_str());
		fbxImport->Import(mesh.fbxScene);
		fbxImport->Destroy();
	}
}