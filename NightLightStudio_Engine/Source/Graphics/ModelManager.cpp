#include "ModelManager.h"

namespace NS_GRAPHICS
{
	ModelManager::ModelManager()
	{

	}

	ModelManager::~ModelManager()
	{

	}
	void ModelManager::FreeGLObjects()
	{
		for (auto& i : VAOs)
		{
			glDeleteVertexArrays(1, &i);
		}

		for (auto& j : VBOs)
		{
			glDeleteBuffers(1, &j);
		}

		for (auto& k : VBOs)
		{
			glDeleteBuffers(1, &k);
		}

		for (auto& l : ModelMatrixBOs)
		{
			glDeleteBuffers(1, &l);
		}
	}
}