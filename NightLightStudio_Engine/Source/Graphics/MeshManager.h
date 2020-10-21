#pragma once

#include "../../framework.h"
#include "Mesh.h"
#include "../Window/WndUtils.h"

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

	public:
		// Unique Singleton instance
		static MeshManager& GetInstance()
		{
			static MeshManager instance;
			return instance;
		}

		std::vector<Mesh*> meshes;

		// Adds new mesh to vector of meshes and returns mesh ID
		unsigned AddMesh(Mesh* const mesh);

		void Free();

		// Free all existing OpenGL Objects and removes Meshes from memory, if any
		// This should be called upon graphics system shutdown
		void DumpMeshData();
	};
}