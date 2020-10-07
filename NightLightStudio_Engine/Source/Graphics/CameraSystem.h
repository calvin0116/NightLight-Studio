#pragma once

namespace NS_GRAPHICS
{
	class CameraSystem
	{
		CameraSystem();
		~CameraSystem();

	public:
		// Unique Singleton instance
		static CameraSystem& GetInstance()
		{
			static CameraSystem instance;
			return instance;
		}

		void Init();
	};
}
