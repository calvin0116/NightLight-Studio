#include "ModelLoader.h"
#include "../../framework.h"

namespace NS_GRAPHICS
{
	ModelLoader::ModelLoader() : _fbxScene{ NULL }, _fbxImport{ NULL }
	{
		_fbxManager = FbxManager::Create();
		FbxIOSettings* settings = FbxIOSettings::Create(_fbxManager, IOSROOT);
		_fbxManager->SetIOSettings(settings);

		//Might be removed in the future
		if (!_fbxManager)
		{
			std::cout << "Error: Unable to create FBX scene!" << std::endl;
			//exit(1);
		}
	}

	ModelLoader::~ModelLoader()
	{
		_fbxManager->Destroy();
	}

	/*void ModelLoader::LoadFBX(const std::string& fileName, Mesh& mesh)
	{
		FbxImporter* fbxImport = FbxImporter::Create(_fbxManager, "");

		if (!fbxImport->Initialize(fileName.c_str(), -1, _fbxManager->GetIOSettings())) {
			exit(-1);
		}

		mesh.fbxScene = FbxScene::Create(_fbxManager, fileName.c_str());
		fbxImport->Import(mesh.fbxScene);
		fbxImport->Destroy();
	}*/

	void ModelLoader::LoadFBX(const std::string& fileName)
	{
		if (_fbxScene)
		{
			_fbxScene->Destroy();
			_fbxScene = NULL;
		}

		// Create an importer using the SDK manager.
		_fbxImport = FbxImporter::Create(_fbxManager, "");

		// Load the fbx using the importer.
		if (!_fbxImport->Initialize(fileName.c_str(), -1, _fbxManager->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			//printf("%s\n", fileName.c_str());
			printf("Error returned: %s\n\n", _fbxImport->GetStatus().GetErrorString());
			exit(-1);
		}

		_fbxScene = FbxScene::Create(_fbxManager, fileName.c_str());
		_fbxImport->Import(_fbxScene);
		_fbxImport->Destroy();

		// Convert Axis System
		FbxAxisSystem SceneAxisSystem = _fbxScene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		if (SceneAxisSystem != OurAxisSystem)
		{
			OurAxisSystem.ConvertScene(_fbxScene);
		}

		// Convert Unit System
		// Convert to CM
		FbxSystemUnit SceneSystemUnit = _fbxScene->GetGlobalSettings().GetSystemUnit();

		if (SceneSystemUnit.GetScaleFactor() != 1.0)
		{
			FbxSystemUnit::cm.ConvertScene(_fbxScene);
		}

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter lGeomConverter(_fbxManager);
		lGeomConverter.Triangulate(_fbxScene, true, true);
		lGeomConverter.SplitMeshesPerMaterial(_fbxScene, true);
	}
}