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

	void ModelLoader::TransverseChild(FbxNode* node, int* meshIndex, const std::string& fileName, const std::string& customName)
	{
		for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		{
			FbxNodeAttribute* nodeType = node->GetNodeAttributeByIndex(i);

			if (nodeType->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				FbxMesh* mesh = node->GetMesh();
				Mesh* newMesh = new Mesh();

				//CHECKS FOR THE FILE NAME
				std::string name;
				size_t pos = fileName.rfind("\\");
				//Get just the string after the last path
				if (pos != std::string::npos)
				{
					name = fileName.substr(pos + 1);
				}
				else
				{
					name = fileName;
				}

				//LOCAL FILE NAME
				name.erase(name.find("."));
				//CONVERSION TO CUSTOM FORMAT
				newMesh->_localFileName = s_LocalPathName + name + s_MeshFileType;

				//MESH NAME
				if (customName.empty())
				{
					if (*meshIndex)
					{
						newMesh->_meshName = name + std::to_string(*meshIndex + 1);
					}
					else
					{
						newMesh->_meshName = name;
					}
				}

				else
				{
					if (*meshIndex)
					{
						newMesh->_meshName = customName + std::to_string(*meshIndex + 1);
					}
					else
					{
						newMesh->_meshName = customName;
					}

				}

				//TEXTURE FILE NAME
				//PROBABLY WONT BE AUTO
				//MANUAL ATTACH OF TEXTURE NONE WILL BE PORTED OVER
				//FbxGeometry* geom = (FbxGeometry*)mesh;
				//FbxProperty property;
				//
				//if (geom->GetNode() != nullptr)
				//{
				//	////Just gets first texture for now
				//	//FbxSurfaceMaterial* material = geom->GetNode()->GetSrcObject<FbxSurfaceMaterial>(0);
				//	//property = material->FindProperty(FbxLayerElement::sTextureChannelNames[0]);

				//	//FbxTexture* texture = property.GetSrcObject<FbxTexture>(0);
				//	//FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

				//	int mat = geom->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
				//	for (int materialIndex = 0; materialIndex < mat; materialIndex++) {
				//		FbxSurfaceMaterial* material = geom->GetNode()->GetSrcObject<FbxSurfaceMaterial>(materialIndex);

				//		//go through all the possible textures
				//		if (material) {

				//			int textureIndex;
				//			FBXSDK_FOR_EACH_TEXTURE(textureIndex)
				//			{
				//				property = material->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);

				//				//FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, lMaterialIndex);
				//				if (property.IsValid())
				//				{
				//					int textureCount = property.GetSrcObjectCount<FbxTexture>();
				//					for (int id = 0; id < textureCount; ++id)
				//					{
				//						FbxTexture* texture = property.GetSrcObject<FbxTexture>(id);
				//						if (texture)
				//						{
				//							FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
				//							newMesh->_textureFileName = { fileTexture->GetRelativeFileName() };
				//							break;
				//						}
				//					}
				//				}
				//			}

				//		}//end if(lMaterial)

				//	}// end for lMaterialIndex     
				//}

				//GET THE VERTEX DATA
				const int vertexCount = mesh->GetControlPointsCount();
				FbxVector4* vertexs = mesh->GetControlPoints();
				newMesh->_vertices.reserve(vertexCount);
				newMesh->_vertexDatas.reserve(vertexCount);
				FbxDouble3 scaling = node->LclScaling.Get();

				for (int vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
				{
					//Control point vertex
					//0,2,1 as FBX exported it as X Z Y over X Y Z
					glm::vec3 vertex = { (float)vertexs[vertexIndex][0] * (float)scaling[0],
										 (float)vertexs[vertexIndex][2] * (float)scaling[2],
										 (float)vertexs[vertexIndex][1] * (float)scaling[1]};

					//OLD WAY
					newMesh->_vertices.push_back(vertex);
					newMesh->_rgb.push_back({ 1.0f,1.0f,1.0f });

					//NEW WAY
					Mesh::VertexData newVertex{ vertex, {1.0f,1.0f,1.0f} };
					newMesh->_vertexDatas.push_back(newVertex);

				}

				//GET THE INDICES, UV AND NORMALS
				//Might update this part
				const int totalBufferSize = mesh->GetPolygonVertexCount();
				newMesh->_indices.reserve(totalBufferSize);
				newMesh->_verticeDatas.reserve(totalBufferSize);

				int vertexID = 0;
				
				const int polygonCount = mesh->GetPolygonCount();
				for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
				{
					const int verticeCount = mesh->GetPolygonSize(polygonIndex);
					for (int verticeIndex = 0; verticeIndex < verticeCount; ++verticeIndex)
					{					
						int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, verticeIndex);

						Mesh::VerticeData verticeData;

						for (int uv = 0; uv < mesh->GetElementUVCount(); ++uv)
						{
							FbxGeometryElementUV* elementUV = mesh->GetElementUV(uv);

							switch (elementUV->GetMappingMode())
							{
							default:
								break;

							case FbxGeometryElement::eByPolygonVertex:
							{
								int uvIndex = mesh->GetTextureUVIndex(polygonIndex, verticeIndex);
								switch (elementUV->GetReferenceMode())
								{
								case FbxGeometryElement::eDirect:
								case FbxGeometryElement::eIndexToDirect:
								{
									FbxVector2 fbxUV = elementUV->GetDirectArray().GetAt(uvIndex);

									glm::vec2 currentUV = { fbxUV[0], fbxUV[1] };
									
									//OLD WAY
									newMesh->_uv.push_back(currentUV);

									//NEW WAY
									verticeData._uv = currentUV;
								}
								break;
								default:
									break; // other reference modes not shown here!
								}
							}
							break;
							}
						}

						//GET THE NORMALS
						FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();

						if (normalElement)
						{
							if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
							{
								FbxVector4 fbxNormal; 

								//reference mode is direct, the normal index is same as indexPolygonVertex.
								if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
								{
									fbxNormal = normalElement->GetDirectArray().GetAt(vertexID);
								}

								//reference mode is index-to-direct, get normals by the index-to-direct
								if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
								{
									int index = normalElement->GetIndexArray().GetAt(vertexID);
									fbxNormal = normalElement->GetDirectArray().GetAt(index);
								}

								glm::vec3 normal = { (float)fbxNormal[0],
														(float)fbxNormal[1],
														(float)fbxNormal[2] };

								//OLD WAY
								newMesh->_normals.push_back(normal);

								//NEW WAY
								verticeData._vNormals = normal;
							}
						}

						newMesh->_indices.push_back((unsigned short)controlPointIndex);
						newMesh->_verticeDatas.push_back(verticeData);
						vertexID++;
					}
				}

				mesh->Destroy();
				MeshManager::GetInstance().AddLoadedMesh(newMesh, customName);
				++(*meshIndex);
			}	
		}

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			TransverseChild(node->GetChild(i), meshIndex, fileName, customName);
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

	void ModelLoader::LoadFBX(const std::string& fileName, const std::string& customName)
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
		int offset = 0;
		FbxNode* root = _fbxScene->GetRootNode();
		if (root)
		{
			FbxNode* parentNode = root->GetParent();

			if (parentNode)
			{
				TransverseChild(parentNode, &offset, fileName, customName);
			}

			for (int i = 0; i < root->GetChildCount(); i++)
			{
				TransverseChild(root->GetChild(i), &offset, fileName, customName);
			}
		}
	}
	void ModelLoader::LoadModel(const std::string& fileName, const std::string& customName)
	{
		if (fileName.find(s_MeshFileType) != std::string::npos)
		{
			LoadCustomMesh(fileName, customName);
		}
		else
		{
			LoadFBX(fileName, customName);
		}
	}
	void ModelLoader::LoadCustomMesh(const std::string& fileName, const std::string& customName)
	{
		//Gets rid of warning for now
		(void)fileName, customName;
	}
}