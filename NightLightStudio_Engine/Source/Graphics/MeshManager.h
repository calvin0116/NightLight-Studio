#pragma once

#include "../../framework.h"
#include "Mesh.h"
#include "Model.h"
#include "../Window/WndUtils.h"
#include <unordered_map>

namespace NS_GRAPHICS
{
	// Manages OpenGL Object IDs A.K.A Model "keys" for all graphics components
	// DOES NOT CONTAIN MODELS THEMSELVES
	class MeshManager
	{
		MeshManager();
		~MeshManager();

		// Value representing number of mesh IDs, used by components to track meshes
		// Value returned in AddMesh() represents mesh ID of current mesh made
		/*
		   E.g. total meshes = 0;
		   1 mesh added
		   meshID returned = 0;
		   meshIDs = 1;
		   (return meshIDs++;)

		   total meshes = 1;

		   1 mesh added
		   meshID returned = 1;
		   meshIDs = 2;
		   (return meshIDs++;)

		*/
		unsigned meshIDs;
		unsigned _modelIDs;

	public:
		// Unique Singleton instance
		static MeshManager& GetInstance()
		{
			static MeshManager instance;
			return instance;
		}

		// Object Mesh Instances
		// Used for actual drawing
		std::vector<Mesh*> meshes;
		std::vector<Model*> _models;

		// Loaded Meshes
		std::unordered_map<std::string, Mesh*> meshlist;
		std::unordered_map<std::string, Model*> _modelList;

		//std::unordered_map<std::string, Mesh> meshes;

		//graphicscomp->AttachMesh("Cylinder");

		// Adds new mesh instance to vector of mesh instances and returns mesh ID
		unsigned AddMesh(Mesh* const mesh);

		// Adds new mesh instance to vector of mesh instances and returns mesh ID
		// Takes in mesh key for mesh list
	    unsigned AddMesh(const std::string& meshkey);

		// Adds loaded mesh into master list, requires name as key and context
		// E.g.
		void AddLoadedMesh(Mesh* mesh, const std::string& meshKey);

		// Adds new model instance to vector of model instances and returns model ID
		unsigned AddModel(Model* const model);

		// Adds new model instance to vector of model instances and returns model ID
		// Takes in model key for model list
		unsigned AddModel(const std::string& modelkey);

		// Adds loaded model into master list, requires name as key and context
		// E.g.
		void AddLoadedModel(Model* model, const std::string& modelkey);

		void Free();

		// Free all existing OpenGL Objects and removes Meshes from memory, if any
		// This should be called upon graphics system shutdown
		void DumpMeshData();
	};
}