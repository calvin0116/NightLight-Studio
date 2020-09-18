#pragma once

namespace NS_GRAPHICS
{
	class ModelLoader
	{
		ModelLoader();
		~ModelLoader();

	public:
		// Unique Singleton instance
		static ModelLoader& GetInstance()
		{
			static ModelLoader instance;
			return instance;
		}
	};
}