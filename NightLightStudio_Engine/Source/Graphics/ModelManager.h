#pragma once

#include "../../framework.h"
#include "Model.h"

namespace NS_GRAPHICS
{
	class ModelManager
	{
		ModelManager();
		~ModelManager();

	public:
		// Unique Singleton instance
		static ModelManager& GetInstance()
		{
			static ModelManager instance;
			return instance;
		}

		std::vector<Model*> models;
	};
}