#pragma once

namespace NS_GRAPHICS
{
	class LightSystem
	{
		LightSystem();
		~LightSystem();

	public:
		// Unique Singleton instance
		static LightSystem& GetInstance()
		{
			static LightSystem instance;
			return instance;
		}
	};
}
