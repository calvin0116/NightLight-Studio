#include "ModelLoader.h"
#include "../../framework.h"

namespace NS_GRAPHICS
{
	ModelLoader::ModelLoader() : _fbxManager{ nullptr }, _fbxScene{ nullptr }, _fbxImport{ nullptr },
		_axisSystem{ FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded }
	{
		std::cout << "Model Loader Created" << std::endl;
	}

	ModelLoader::~ModelLoader()
	{
		_fbxManager->Destroy();
	}

	void ModelLoader::TransverseChild(FbxNode* node, Model*& model)
	{
		for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		{
			FbxNodeAttribute* nodeType = node->GetNodeAttributeByIndex(i);

			if (nodeType->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				Mesh* newMesh = new Mesh();
				FbxMesh* mesh = node->GetMesh();

				//GET THE VERTEX DATA
				const int vertexCount = mesh->GetControlPointsCount();
				FbxVector4* vertexs = mesh->GetControlPoints();
				newMesh->_vertices.reserve(vertexCount);
				FbxDouble3 scaling = node->LclScaling.Get();

				for (int vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
				{
					glm::vec3 vertex = { (float)vertexs[vertexIndex][0] * (float)scaling[0],
										 (float)vertexs[vertexIndex][1] * (float)scaling[1],
										 (float)vertexs[vertexIndex][2] * (float)scaling[2]};

					newMesh->_vertices.push_back(vertex);
				}

				//GET THE INDICES
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

				//GET THE NORMALS
				FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();

				if (normalElement)
				{
					//mapping mode is by control points. The mesh should be smooth and soft.
					//we can get normals by retrieving each control point
					if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
					{
						for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
						{
							int normalIndex = 0;
							//reference mode is direct, the normal index is same as vertex index.
							//get normals by the index of control vertex
							if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
								normalIndex = vertexIndex;

							//reference mode is index-to-direct, get normals by the index-to-direct
							if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
								normalIndex = normalElement->GetIndexArray().GetAt(vertexIndex);

							//Got normals of each vertex.
							FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);

							glm::vec3 vertex = { (float)normal[0],
												 (float)normal[1],
												 (float)normal[2]};

							newMesh->_normals.push_back(vertex);
						}
					}
					else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{

						int indexPolygonVertex = 0;
						//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
						for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
						{
							//get polygon size, you know how many vertices in current polygon.
							const int verticeCount = mesh->GetPolygonSize(polygonIndex);
							//retrieve each vertex of current polygon.
							for (int i = 0; i < verticeCount; i++)
							{
								int normalIndex = 0;
								//reference mode is direct, the normal index is same as indexPolygonVertex.
								if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
									normalIndex = indexPolygonVertex;

								//reference mode is index-to-direct, get normals by the index-to-direct
								if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
									normalIndex = normalElement->GetIndexArray().GetAt(indexPolygonVertex);

								//Got normals of each polygon-vertex.
								FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);

								glm::vec3 vertex = { (float)normal[0],
													 (float)normal[1],
													 (float)normal[2]};

								newMesh->_normals.push_back(vertex);

								//std::cout << "Normals " << polygonIndex << ": " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;

								++indexPolygonVertex;
							}
						}

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
		if (!_fbxManager)
		{
			printf("Error: Unable to create FBX scene!\n");
			exit(1);
		}
	}

	void ModelLoader::LoadFBX(const std::string& fileName, Model*& model)
	{
		std::cout << "Loading FBX..." << std::endl;
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
			//exit(-1);
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