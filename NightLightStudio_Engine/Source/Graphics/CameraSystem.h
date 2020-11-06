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

		// Zoom distance
		float zoomDistance;

		// the target the camera is looking at
		// tgt [] <---- >o camera
		glm::vec3 tgt;

		// viewVector
		// tgt [] <---- >o camera
		//          ^
		//          |
		//        this vector
		glm::vec3 viewVector;

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


		// Added by LJM
		void ForceUpdate();
		// Added by LJM
		Camera& GetCamera();

		// update the theird person camera
		void UpdateThirdPersonCamera();
		// update the view vector, view vector is normalsied
		void UpdateViewVector();
	};
}
