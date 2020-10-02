#pragma once

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
	};
}