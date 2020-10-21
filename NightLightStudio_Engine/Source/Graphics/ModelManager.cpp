#include "ModelManager.h"

namespace NS_GRAPHICS
{
	ModelManager::ModelManager()
		: meshIDs{ 0 }
	{

	}

	ModelManager::~ModelManager()
	{

	}
	unsigned ModelManager::AddMesh(Mesh* const mesh)
	{
		meshes.push_back(mesh);

		return meshIDs++;
	}
	void ModelManager::Free()
	{
		DumpMeshData();
	}

	void ModelManager::DumpMeshData()
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