#pragma once

namespace NS_GRAPHICS
{
	class TextureLoader
	{
		TextureLoader();
		~TextureLoader();

	public:
		// Unique Singleton instance
		static TextureLoader& GetInstance()
		{
			static TextureLoader instance;
			return instance;
		}
	};
}