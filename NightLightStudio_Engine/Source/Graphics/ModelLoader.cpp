#include "ModelLoader.h"
#include "../../framework.h"

namespace NS_GRAPHICS
{
	ModelLoader::ModelLoader() : _fbxManager{ nullptr }, _fbxScene{ nullptr }, _fbxImport{ nullptr },
		_axisSystem{ FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded }
	{
	}

	ModelLoader::~ModelLoader()
	{
		_fbxManager->Destroy();
	}

	void ModelLoader::TransverseChild(FbxNode* node, Model*& model)
	{
		FbxDouble3 translation = node->LclTranslation.Get();
		FbxDouble3 rotation = node->LclRotation.Get();
		FbxDouble3 scaling = node->LclScaling.Get();

		for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		{
			FbxNodeAttribute* nodeType = node->GetNodeAttributeByIndex(i);

			if (nodeType->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				Mesh* newMesh = new Mesh();
				FbxMesh* mesh = node->GetMesh();
				newMesh->_position = { (float)translation[0], (float)translation[1], (float)translation[2] };
				newMesh->_scale = { (float)scaling[0], (float)scaling[1], (float)scaling[2] };
				newMesh->_rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2] };

				const int vertexCount = mesh->GetControlPointsCount();
				FbxVector4* vertexs = mesh->GetControlPoints();
				newMesh->_vertices.reserve(vertexCount);

				for (int vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
				{
					glm::vec3 vertex = { (float)vertexs[vertexIndex][0],
										 (float)vertexs[vertexIndex][1],
										 (float)vertexs[vertexIndex][2] };

					newMesh->_vertices.push_back(vertex);
				}

				//Might update this part
				const int totalBufferSize = mesh->GetPolygonVertexCount();
				newMesh->_indices.reserve(totalBufferSize);

				const int polygonCount = mesh->GetPolygonCount();
				for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
				{
					const int verticeCount = mesh->GetPolygonSize(polygonIndex);
					for (int verticeIndex = 0; verticeIndex < verticeCount; ++verticeIndex)
					{
						//newMesh->_vertices.push_back(vertex);
						newMesh->_indices.push_back(mesh->GetPolygonVertex(polygonIndex, verticeIndex));
					}
				}

				mesh->Destroy();
				model->_meshes.push_back(newMesh);
			}
		}

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			TransverseChild(node->GetChild(i), model);
		}
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

	void ModelLoader::Init()
	{
		_fbxManager = FbxManager::Create();
		FbxIOSettings* settings = FbxIOSettings::Create(_fbxManager, IOSROOT);
		_fbxManager->SetIOSettings(settings);

		//Might be removed in the future
		if (!_fbxScene)
		{
			FBXSDK_printf("Error: Unable to create FBX scene!\n");
			exit(1);
		}
	}

	void ModelLoader::LoadFBX(const std::string& fileName, Model*& model)
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
		FbxAxisSystem currentAxis = _fbxScene->GetGlobalSettings().GetAxisSystem();
		if (currentAxis != _axisSystem)
		{
			_axisSystem.ConvertScene(_fbxScene);
		}

		// Convert Unit System Into Centimeter
		FbxSystemUnit currentSystemUnit = _fbxScene->GetGlobalSettings().GetSystemUnit();
		if (currentSystemUnit != FbxSystemUnit::cm)
		{
			FbxSystemUnit::cm.ConvertScene(_fbxScene);
		}

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter toTriangulate(_fbxManager);
		toTriangulate.Triangulate(_fbxScene, true, true);
		//Not sure needed anot
		//toTriangulate.SplitMeshesPerMaterial(_fbxScene, true);

		// Recursively goes through the scene to get all the meshes
		// Root nodes should not contain any attributes.
		FbxNode* root = _fbxScene->GetRootNode();
		if (root)
		{
			FbxNode* parentNode = root->GetParent();

			if (parentNode)
			{
				TransverseChild(parentNode, model);
			}

			for (int i = 0; i < root->GetChildCount(); i++)
			{
				TransverseChild(root->GetChild(i), model);
			}
		}
	}
}