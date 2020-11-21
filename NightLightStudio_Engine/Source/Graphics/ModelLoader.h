#pragma once
#include "Mesh.h"
#include "ModelManager.h"
#include "Model.h"
//#include "assimp/Importer.hpp"
//#include "assimp/scene.h"
//#include "assimp/postprocess.h"

namespace NS_GRAPHICS
{
	static std::string s_FbxFileFormat = ".fbx";

	class ModelLoader
	{
	/*	FbxManager* _fbxManager;
		FbxScene* _fbxScene;
		FbxImporter* _fbxImport;
		FbxAxisSystem _axisSystem;*/

		ModelManager* _modelManager;

		ModelLoader();
		~ModelLoader();

		//void ProcessMesh(FbxNode* node, Model*& model);

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}

		void Init();

		//void LoadFBX(const std::string& fileName, Mesh& mesh);
		bool LoadFBX(Model*& model);
		void LoadModel(const std::string& fileName);
		bool LoadCustomMesh(Model*& model);
		bool SaveCustomMesh(Model*& model);

		void DebugToFile(const std::string& fileName);
		//FbxVector4 Transform(FbxNode* node, FbxVector4 vector);
	};
}