#pragma once

#include "../../framework.h"
#include "Model.h"
#include "../Window/WndUtils.h"
#include <unordered_map>

namespace NS_GRAPHICS
{
	// Manages OpenGL Object IDs A.K.A Model "keys" for all graphics components
	// DOES NOT CONTAIN MODELS THEMSELVES
	class ModelManager
	{
		ModelManager();
		~ModelManager();

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
		//unsigned meshIDs;
		unsigned _modelIDs;

	public:
		// Unique Singleton instance
		static ModelManager& GetInstance()
		{
			static ModelManager instance;
			return instance;
		}

		// Object Mesh Instances
		// Used for actual drawing
		//std::vector<Mesh*> meshes;
		std::vector<Model*> _models;

		// Loaded Meshes
		//std::unordered_map<std::string, Mesh*> meshlist;
		std::unordered_map<std::string, Model*> _modelList;

		//std::unordered_map<std::string, Mesh> meshes;

		//graphicscomp->AttachMesh("Cylinder");
		
		// Adds new mesh instance to vector of mesh instances and returns mesh ID
		//int AddMesh(Mesh* const mesh);

		// Adds new mesh instance to vector of mesh instances and returns mesh ID
		// Takes in mesh key for mesh list
	    //int AddMesh(const std::string& meshkey);

		// Adds loaded mesh into master list, requires name as key and context
		// E.g.
		//void AddLoadedMesh(Mesh* mesh, const std::string& meshKey);

		// Adds new model instance to vector of model instances and returns model ID
		int AddModel(Model* const model);

		// Adds new model instance to vector of model instances and returns model ID
		// Takes in model key for model list
		int AddModel(const std::string& modelkey);

		// Adds loaded model into master list, requires name as key and context
		// E.g.
		void AddLoadedModel(Model* model, const std::string& modelkey);

		void Free();

		// Free all existing OpenGL Objects and removes Models from memory, if any
		// This should be called upon graphics system shutdown
		void DumpModelData();
	};
}