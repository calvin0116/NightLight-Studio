#pragma once

namespace NS_GRAPHICS
{
	class DebugManager
	{
		DebugManager();
		~DebugManager();

	public:
		// Unique Singleton instance
		static DebugManager& GetInstance()
		{
			static DebugManager instance;
			return instance;
		}
	};
}
