#include "ModelLoader.h"
#include "../../framework.h"
#include "../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace NS_GRAPHICS
{
	ModelLoader::ModelLoader() : _fbxManager{ nullptr }, _fbxScene{ nullptr }, _fbxImport{ nullptr },
		_axisSystem{ FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded },
		_modelManager{ &ModelManager::GetInstance() }
	{
		std::cout << "Model Loader Created" << std::endl;
	}

	ModelLoader::~ModelLoader()
	{
		_fbxManager->Destroy();
	}

	void ModelLoader::TransverseChild(FbxNode* node, Model*& model, int* meshIndex, const std::string& fileName, const std::string& customName)
	{
		for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		{
			FbxNodeAttribute* nodeType = node->GetNodeAttributeByIndex(i);

			if (nodeType->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				FbxMesh* mesh = node->GetMesh();

				//Old way of loading fbx
				Mesh* newMesh = new Mesh();

				//New way of loading fbx
				//Ignore naming
				Mesh* newMeshNew = new Mesh();

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
				//OLD
				newMesh->_localFileName = s_LocalPathName + name + s_ModelFileType;
				//NEW
				newMeshNew->_localFileName = s_LocalPathName + name + s_ModelFileType;

				//MESH NAME
				if (customName.empty())
				{
					if (*meshIndex)
					{
						//OLD
						newMesh->_meshName = name + std::to_string(*meshIndex + 1);
						//NEW
						newMeshNew->_meshName = name + std::to_string(*meshIndex + 1);
					}
					else
					{
						//OLD
						newMesh->_meshName = name;
						//NEW
						newMeshNew->_meshName = name;
					}
				}

				else
				{
					if (*meshIndex)
					{
						//OLD
						newMesh->_meshName = customName + std::to_string(*meshIndex + 1);
						//NEW
						newMeshNew->_meshName = customName + std::to_string(*meshIndex + 1);
					}
					else
					{
						//OLD
						newMesh->_meshName = customName;
						//NEW
						newMeshNew->_meshName = customName;
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


				std::cout << node->GetName() << ":" << std::endl;

				//FbxDouble3 translation = node->LclTranslation.Get();
				//FbxDouble3 rotation = node->LclRotation.Get();
				//FbxDouble3 scaling = node->LclScaling.Get();
				////FbxDouble3 test = node->GeometricTranslation.Get();

				//std::cout << translation[0] << ", " << translation[1] << ", " << translation[2] << std::endl;
				//std::cout << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << std::endl;
				//std::cout << scaling[0] << ", " << scaling[1] << ", " << scaling[2] << std::endl;

				//std::cout << test[0] << ", " << test[1] << ", " << test[2] << std::endl;

				//const char* nodeName = node->GetName();
				//glm::mat4 modelMatrix(1);
				//glm::mat4 translateMatrix(1);
				//glm::mat4 scaleMatrix(1);

				//glm::translate(translateMatrix, glm::vec3(translation[0], translation[1], translation[2]));
				//glm::quat quaternion(glm::radians(glm::vec3(rotation[0], rotation[1], rotation[2])));
				//glm::mat4 rotateMatrix = glm::mat4_cast(quaternion);
				//glm::scale(scaleMatrix, glm::vec3(scaling[0], scaling[1], scaling[2]));

				//modelMatrix = translateMatrix * rotateMatrix * scaleMatrix;

				//for (int vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
				//{
				//	//Control point vertex
				//	glm::vec3 vertex;

				//	vertex = { (float)vertexs[vertexIndex][0],
				//			   (float)vertexs[vertexIndex][1],
				//			   (float)vertexs[vertexIndex][2]};

				//	vertex = modelMatrix * glm::vec4(vertex, 1.0);

				//	//OLD WAY OF ROTATION
				//	//if (fileName.find(s_FbxFileFormat) != std::string::npos)
				//	//{
				//	//	vertex = { (float)vertexs[vertexIndex][0],
				//	//			   (float)vertexs[vertexIndex][2],
				//	//			   -(float)vertexs[vertexIndex][1]};
				//	//}
				//	//else
				//	//{

				//	//	vertex = { (float)vertexs[vertexIndex][0],
				//	//			   (float)vertexs[vertexIndex][1],
				//	//			   (float)vertexs[vertexIndex][2] };

				//	//}

				//	//vertex = modelMatrix * glm::vec4(vertex, 1.0);

				//	/////////////////////////////////
				//	/// Mesh Way
				//	/////////////////////////////////
				//	//OLD WAY 
				//	newMesh->_vertices.push_back(vertex);
				//	newMesh->_rgb.push_back({ 1.0f,1.0f,1.0f });

				//	//NEW WAY
				//	Mesh::VertexData newVertex{ vertex, {1.0f,1.0f,1.0f} };
				//	newMesh->_vertexDatas.push_back(newVertex);

				//	/////////////////////////////////
				//	/// Model Way
				//	/////////////////////////////////
				//	//OLD WAY
				//	newMeshNew->_vertices.push_back(vertex);
				//	newMeshNew->_rgb.push_back({ 1.0f,1.0f,1.0f });

				//	//NEW WAY
				//	newMeshNew->_vertexDatas.push_back(newVertex);

				//}

				//GET THE INDICES, UV AND NORMALS
				//Might update this part
				const int totalBufferSize = mesh->GetPolygonVertexCount();
				FbxVector4* vertexs = mesh->GetControlPoints();
				//OLD WAY
				newMesh->_indices.reserve(totalBufferSize);
				newMesh->_fragmentDatas.reserve(totalBufferSize);
				//NEW WAY
				newMeshNew->_indices.reserve(totalBufferSize);
				newMeshNew->_fragmentDatas.reserve(totalBufferSize);

				//OLD
				newMesh->_vertices.reserve(totalBufferSize);
				newMesh->_vertexDatas.reserve(totalBufferSize);
				//NEW
				newMeshNew->_vertices.reserve(totalBufferSize);
				newMeshNew->_vertexDatas.reserve(totalBufferSize);

				int vertexID = 0;
				
				const int polygonCount = mesh->GetPolygonCount();
				for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
				{
					const int verticeCount = mesh->GetPolygonSize(polygonIndex);
					for (int verticeIndex = 0; verticeIndex < verticeCount; ++verticeIndex)
					{					
						int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, verticeIndex);

						//NEW POSITION HERE
						//Control point vertex
						if (controlPointIndex < 0)
						{
							//<0 means corrupted mesh
							delete newMeshNew;
							delete newMesh;

							return;
						}

						FbxVector4 finalPosition = GetTransform(node ,vertexs[controlPointIndex]);

						glm::vec3 vertex;

						vertex = { (float)finalPosition[0],
									(float)finalPosition[1],
									(float)finalPosition[2]};

						//FBX FIX 
						if (fileName.find(s_FbxFileFormat) != std::string::npos)
						{
							glm::quat quaternion(glm::radians(glm::vec3(90.0f, 0.0f, 0.0f)));
							glm::mat4 rotateMatrix = glm::mat4_cast(quaternion);

							vertex = rotateMatrix * glm::vec4(vertex, 1.0f);
						}

						//vertex = modelMatrix * glm::vec4(vertex, 1.0);

						/////////////////////////////////
						/// Mesh Way
						/////////////////////////////////
						//OLD WAY 
						newMesh->_vertices.push_back(vertex);
						newMesh->_rgb.push_back({ 1.0f,1.0f,1.0f });

						//NEW WAY
						Mesh::VertexData newVertex{ vertex, {1.0f,1.0f,1.0f} };
						newMesh->_vertexDatas.push_back(newVertex);

						/////////////////////////////////
						/// Model Way
						/////////////////////////////////
						//OLD WAY
						newMeshNew->_vertices.push_back(vertex);
						newMeshNew->_rgb.push_back({ 1.0f,1.0f,1.0f });

						//NEW WAY
						newMeshNew->_vertexDatas.push_back(newVertex);

						Mesh::FragmentData fragmentData;

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
									newMeshNew->_uv.push_back(currentUV);

									//NEW WAY
									fragmentData._uv = currentUV;
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
								newMeshNew->_normals.push_back(normal);

								//NEW WAY
								fragmentData._vNormals = normal;
							}
						}

						/////////////////////////////////
						/// Mesh Way
						/////////////////////////////////
						newMesh->_indices.push_back((unsigned short)vertexID);
						newMesh->_fragmentDatas.push_back(fragmentData);

						/////////////////////////////////
						/// Model Way
						/////////////////////////////////
						newMeshNew->_indices.push_back((unsigned short)vertexID);
						newMeshNew->_fragmentDatas.push_back(fragmentData);

						vertexID++;
					}
				}

				mesh->Destroy();

				//OLD WAY
				//ModelManager::GetInstance().AddLoadedMesh(newMesh, newMesh->_meshName);
				delete newMesh;

				//NEW WAY
				model->_meshes.push_back(newMeshNew);
				++(*meshIndex);
			}	
		}

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			TransverseChild(node->GetChild(i), model, meshIndex, fileName, customName);
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

	void ModelLoader::LoadFBX(Model*& model, const std::string& fileName, const std::string& customName)
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
				TransverseChild(parentNode, model,&offset, fileName, customName);
			}

			for (int i = 0; i < root->GetChildCount(); i++)
			{
				TransverseChild(root->GetChild(i), model, &offset, fileName, customName);
			}
		}
	}
	void ModelLoader::LoadModel(const std::string& fileName, const std::string& customName)
	{
		Model* model = new Model();

		//Checks for the file name
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

		//Trim the extension to get the file name
		name.erase(name.rfind("."));
		model->_fileName = s_LocalPathName + name + s_ModelFileType;

		//Model name if empty use file name
		if (customName.empty())
		{
			model->_modelName = name;
		}
		else
		{
			model->_modelName = customName;
		}

		//First checks if the model existed to avoid unnecessary loading
		if (_modelManager->_modelList.find(model->_modelName) != _modelManager->_modelList.end())
		{
			delete model;
			return;
		}

		//Loads the correct function based on extension
		if (fileName.find(s_ModelFileType) != std::string::npos)
		{
			LoadCustomMesh(model, fileName, customName);
		}
		else
		{
			LoadFBX(model, fileName, customName);
		}

		_modelManager->AddLoadedModel(model, model->_modelName);
#ifdef _DEBUG
		std::string textFileName = model->_modelName + ".txt";
		DebugToFile(model->_modelName, textFileName);
#endif

		//TODO Saving custom mesh
	}
	void ModelLoader::LoadCustomMesh(Model*& model, const std::string& fileName, const std::string& customName)
	{
		//Gets rid of warning for now
		(void)model, fileName, customName;
	}
	void ModelLoader::DebugToFile(const std::string& meshName, const std::string& fileName)
	{
		std::ofstream logFile;
		logFile.open(fileName.c_str());

		int lineCount = 0;
		size_t MeshSize = _modelManager->_modelList[meshName]->_meshes.size();
		for (size_t i = 0; i < MeshSize; ++i)
		{
			size_t verticeSize = _modelManager->_modelList[meshName]->_meshes[i]->_vertices.size();
			for (size_t x = 0; x < verticeSize; ++x)
			{
				logFile << "Vertex: X: " << _modelManager->_modelList[meshName]->_meshes[i]->_vertices[x].x << " Y: " <<
											_modelManager->_modelList[meshName]->_meshes[i]->_vertices[x].y << " Z: " <<
											_modelManager->_modelList[meshName]->_meshes[i]->_vertices[x].z << "\n";

				lineCount++;
			}

			logFile << std::endl;
		}

		logFile << "Total Points : " << lineCount << std::endl;

		logFile.close();
	}

	FbxVector4 ModelLoader::GetTransform(FbxNode* node, FbxVector4 vector) 
	{
		FbxAMatrix geomMatrix;
		geomMatrix.SetIdentity();
		if (node->GetNodeAttribute())
		{
			const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot);
			const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot);
			const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot);
			geomMatrix.SetT(lT);
			geomMatrix.SetR(lR);
			geomMatrix.SetS(lS);
		}
		FbxAMatrix localMatrix = node->EvaluateLocalTransform();

		FbxNode* parentNode = node->GetParent();
		FbxAMatrix parentMatrix = parentNode->EvaluateLocalTransform();
		while (parentNode != NULL)
		{
			parentMatrix = parentNode->EvaluateLocalTransform() * parentMatrix;
			parentNode = parentNode->GetParent();
		}

		FbxAMatrix matrix = parentMatrix * localMatrix * geomMatrix;
		FbxVector4 result = matrix.MultT(vector);
		return result;
	}
}