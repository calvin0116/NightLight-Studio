#pragma once

#include "Camera.h"
#include "..\Core\DeltaTime.h"

namespace NS_GRAPHICS
{
	// Const variables used for handling camera movement

	class CameraSystem
	{
		CameraSystem();
		~CameraSystem();

		// Currently only 1 camera
		// Will be changed to vector of cameras in the future
		Camera _camera;

		// Check if rotation has been changed
		bool updatedRot;

		// Check if view matrix has updated at all
		bool updated;

	public:
		// Unique Singleton instance
		static CameraSystem& GetInstance()
		{
			static CameraSystem instance;
			return instance;
		}

		void Init();

		void Update();

		bool CheckUpdate();

		void AckUpdate();

		glm::mat4 GetViewMatrix();

		glm::vec3 GetCurrentCameraPosition();
	};
}
