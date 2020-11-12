#include "ModelLoader.h"
#include "../../framework.h"
#include "../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"
#include <iostream>
#include <string>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

//#define LOG_MODEL

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

	void ModelLoader::ProcessMesh(FbxNode* node, Model*& model)
	{
		std::cout << "Model Loader: Checking " << node->GetName() << std::endl;

		for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		{
			FbxNodeAttribute* nodeType = node->GetNodeAttributeByIndex(i);

			if (nodeType->GetAttributeType() == FbxNodeAttribute::eMesh)
			{
				FbxMesh* mesh = node->GetMesh();

				//Old way of loading fbx
				Mesh* newMesh = new Mesh();

				std::cout << "Model Loader: Loading " << node->GetName() << std::endl;
				newMesh->_nodeName = node->GetName();

				//GET THE INDICES, UV AND NORMALS
				//Might update this part
				const int totalBufferSize = mesh->GetPolygonVertexCount();
				FbxVector4* vertexs = mesh->GetControlPoints();

				newMesh->_indices.reserve(totalBufferSize);
				newMesh->_vertexDatas.reserve(totalBufferSize);

				int vertexID = 0;
				
				const int polygonCount = mesh->GetPolygonCount();
				//Loops Faces
				for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
				{
					const int verticeCount = mesh->GetPolygonSize(polygonIndex);
					//Loops vertices within faces
					for (int verticeIndex = 0; verticeIndex < verticeCount; ++verticeIndex)
					{					
						int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, verticeIndex);

						//Checks for corrupted mesh
						if (controlPointIndex < 0)
						{
							//<0 means corrupted mesh
							delete newMesh;

							return;
						}

						FbxVector4 finalPosition = Transform(node ,vertexs[controlPointIndex]);

						glm::vec3 vertex;

						vertex = { (float)finalPosition[0],
									(float)finalPosition[1],
									(float)finalPosition[2]};


						Mesh::VertexData newVertex;
						newVertex._position = vertex;

						FbxGeometryElementUV* elementUV = mesh->GetElementUV(0);

						//Assumes 1 uv mapping
						if (elementUV)
						{
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
									currentUV.y = std::abs(1.0f - currentUV.y);

									newVertex._uv = currentUV;
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

								newVertex._normals = normal;
							}
						}

						newMesh->_indices.push_back((unsigned short)vertexID);
						newMesh->_vertexDatas.push_back(newVertex);

						vertexID++;
					}
				}

				mesh->Destroy();

				//NEW WAY
				model->_meshes.push_back(newMesh);
			}	
		}

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			ProcessMesh(node->GetChild(i), model);
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

	bool ModelLoader::LoadFBX(Model*& model)
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
		if (!_fbxImport->Initialize(model->_fileName.c_str(), -1, _fbxManager->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("%s\n", model->_fileName.c_str());
			printf("Error returned: %s\n\n", _fbxImport->GetStatus().GetErrorString());
			return false;
			//exit(-1);
		}

		_fbxScene = FbxScene::Create(_fbxManager, model->_fileName.c_str());
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
		toTriangulate.SplitMeshesPerMaterial(_fbxScene, true);

		// Recursively goes through the scene to get all the meshes
		// Root nodes should not contain any attributes.
		//int offset = 0;
		FbxNode* root = _fbxScene->GetRootNode();
		if (root)
		{
			/*FbxNode* parentNode = root->GetParent();

			if (parentNode)
			{
				ProcessMesh(parentNode, model);
			}*/

			for (int i = 0; i < root->GetChildCount(); i++)
			{
				ProcessMesh(root->GetChild(i), model);
			}
		}

		return true;
	}
	void ModelLoader::LoadModel(const std::string& fileName)
	{
		if (fileName.empty())
		{
			return;
		}

		Model* model = new Model();
		model->_fileName = fileName;

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

		pos = name.rfind(".");
		//Trim the extension to get the file name
		if (pos != std::string::npos)
		{
			name.erase(pos);
		}

		std::string searchString;
		searchString = fileName;
		//searchString = s_LocalPathName + name + s_ModelFileType;

		//First checks if the custom model existed to avoid unnecessary loading
		if (_modelManager->_modelList.find(searchString) != _modelManager->_modelList.end())
		{
			delete model;
			return;
		}

		//Loads the correct function based on extension
		if (fileName.find(s_ModelFileType) != std::string::npos)
		{
			if (!LoadCustomMesh(model))
			{
				delete model;
				return;
			}

			//If different pathing, saves another copy in our own asset folder
			if (model->_fileName != searchString)
			{
				model->_fileName = s_LocalPathName + name + s_ModelFileType;
				SaveCustomMesh(model);
			}
		}
		else
		{
			if (!LoadFBX(model))
			{
				delete model;
				return;
			}

			//model->_fileName = s_LocalPathName + name + s_ModelFileType;
			//SaveCustomMesh(model);
		}

		_modelManager->AddLoadedModel(model, model->_fileName);

#ifdef LOG_MODEL
		//std::string textFileName = model-> + ".txt";
		DebugToFile(model->_fileName);
#endif

		//TODO Saving custom mesh
	}
	bool ModelLoader::LoadCustomMesh(Model*& model)
	{
		if (model->_fileName[0] == '\\')
		{
			model->_fileName.erase(0, 1);
		}
		//Gets rid of warning for now
		std::ifstream meshFile;
		meshFile.open(model->_fileName.c_str());

		if (!meshFile.is_open())
		{
			std::cout << "Fail to opened model file" << std::endl;
			return false;
		}

		std::string input;
		while (std::getline(meshFile, input))
		{
			std::cout << "Reading Variables" << std::endl;
			if (input.find("BEGIN") != std::string::npos)
			{
				std::string vertexCount = input.substr(input.find(" ") + 1);
				Mesh* newMesh = new Mesh();
				newMesh->_vertexDatas.reserve((size_t)std::stoi(vertexCount));
				newMesh->_indices.reserve((size_t)std::stoi(vertexCount));

				std::getline(meshFile, input);
				newMesh->_nodeName = input;

				Mesh::VertexData vertex;

				int index = 0;

				while (std::getline(meshFile, input, ' '))
				{
					if (input == "v:")
					{		
						std::getline(meshFile, input);
						std::string posX = input.substr(0, input.find(","));
						std::string nextPos = input.substr(input.find(",")+1);
						std::string posY = nextPos.substr(0, nextPos.find(","));
						std::string posZ = input.substr(input.rfind(",")+1);

						vertex._position.x = std::stof(posX);
						vertex._position.y = std::stof(posY);
						vertex._position.z = std::stof(posZ);

					}

					else if (input == "uv:")
					{
						std::getline(meshFile, input);
						std::string u = input.substr(0, input.find(","));
						std::string v = input.substr(input.rfind(",") + 1);

						vertex._uv.x = std::stof(u);
						vertex._uv.y = std::stof(v);
					}

					else if (input == "n:")
					{
						std::getline(meshFile, input);
						std::string normalX = input.substr(0, input.find(","));
						std::string normPos = input.substr(input.find(",") + 1);
						std::string normalY = normPos.substr(0, normPos.find(","));
						std::string normalZ = input.substr(input.rfind(",") + 1);

						vertex._normals.x = std::stof(normalX);
						vertex._normals.y = std::stof(normalY);
						vertex._normals.z = std::stof(normalZ);

						newMesh->_indices.push_back((unsigned short)index);
						index++;
						newMesh->_vertexDatas.push_back(vertex);

						if (index >= std::stoi(vertexCount))
						{
							break;
						}
					}
				}

				std::getline(meshFile, input);
				model->_meshes.push_back(newMesh);
			}
		}

		meshFile.close();
		return true;
	}
	bool ModelLoader::SaveCustomMesh(Model*& model)
	{
		//Gets rid of warning for now
		std::ofstream meshFile;
		meshFile.open(model->_fileName.c_str());

		auto meshIterator = model->_meshes.begin();
		auto meshIteratorEnd = model->_meshes.end();

		while (meshIterator != meshIteratorEnd)
		{
			meshFile << "BEGIN " << (*meshIterator)->_vertexDatas.size() << "\n";
			meshFile << (*meshIterator)->_nodeName << "\n";

			size_t vertexCount = (*meshIterator)->_vertexDatas.size();
			for (size_t i = 0; i < vertexCount; ++i)
			{
				meshFile << "v: " << (*meshIterator)->_vertexDatas[i]._position.x << "," 
								  << (*meshIterator)->_vertexDatas[i]._position.y << ","
								  << (*meshIterator)->_vertexDatas[i]._position.z
								  << "\n";

				meshFile << "uv: " << (*meshIterator)->_vertexDatas[i]._uv.x << "," 
								   << (*meshIterator)->_vertexDatas[i]._uv.x
								   << "\n";

				meshFile << "n: " << (*meshIterator)->_vertexDatas[i]._normals.x << ","
								  << (*meshIterator)->_vertexDatas[i]._normals.y << ","
								  << (*meshIterator)->_vertexDatas[i]._normals.z
								  << "\n";
			}
			meshFile << "END\n";
			++meshIterator;
		}


		meshFile.close();
		return true;
	}
	void ModelLoader::DebugToFile(const std::string& fileName)
	{
		std::string name = fileName;
		name.erase(name.rfind("."));
		name.append(".txt");

		std::ofstream logFile;
		logFile.open(fileName.c_str());

		int lineCount = 0;
		size_t MeshSize = _modelManager->_modelList[fileName]->_meshes.size();
		for (size_t i = 0; i < MeshSize; ++i)
		{
			size_t verticeSize = _modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas.size();
			for (size_t x = 0; x < verticeSize; ++x)
			{
				logFile << "Vertex: X: " << _modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas[x]._position.x << " Y: " <<
											_modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas[x]._position.y << " Z: " <<
											_modelManager->_modelList[fileName]->_meshes[i]->_vertexDatas[x]._position.z << "\n";

				lineCount++;
			}

			logFile << std::endl;
		}

		logFile << "Total Points : " << lineCount << std::endl;

		logFile.close();
	}

	FbxVector4 ModelLoader::Transform(FbxNode* node, FbxVector4 vector)
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