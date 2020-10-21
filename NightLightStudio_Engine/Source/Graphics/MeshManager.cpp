#include "MeshManager.h"

namespace NS_GRAPHICS
{
	MeshManager::MeshManager()
		: meshIDs{ 0 }
	{

	}

	MeshManager::~MeshManager()
	{

	}
	unsigned MeshManager::AddMesh(Mesh* const mesh)
	{
		meshes.push_back(mesh);

		return meshIDs++;
	}
	void MeshManager::Free()
	{
		DumpMeshData();
	}

	void MeshManager::DumpMeshData()
	{
		for (auto& i : meshes)
		{
			glDeleteBuffers(1, &i->VBO);
			glDeleteBuffers(1, &i->UVBO);
			glDeleteBuffers(1, &i->CBO);
			glDeleteBuffers(1, &i->EBO);
			glDeleteBuffers(1, &i->ModelMatrixBO);
			glDeleteVertexArrays(1, &i->VAO);

			delete i;
		}
	}
}