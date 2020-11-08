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

		// use thrid person cam?
		bool useThridPersonCam;
		// can the cam rotate?
		bool canThridPersonCamRotate;
		// can the cam zoom?
		bool canThridPersonCamZoom;

		float theThridPersonCamPitch;
		float theThridPersonCamYaw;

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

		void SetRotationSensitivity(const float& sensitivity);
		void SetDragSensitivity(const float& sensitivity);
		void SetZoomSensitivity(const float& sensitivity);

		const float& GetRotationSensitivity();
		const float& GetDragSensitivity();
		const float& GetZoomSensitivity();

		// Added by LJM
		void ForceUpdate();
		// Added by LJM
		Camera& GetCamera();

		// update the theird person camera
		void UpdateThirdPersonCamera();
		// update the view vector, view vector is normalsied
		void UpdateViewVector();

		// get viewVector
		glm::vec3 GetViewVector();
		// get viewVector on XZ plane
		glm::vec3 GetXZViewVector();
		// get viewVector on XZ plane, rotate @Y 180 deg
		glm::vec3 GetXZViewVector_Back();
		// get viewVector on XZ plane, rotate @Y 90 deg
		glm::vec3 GetXZViewVector_Left();
		// get viewVector on XZ plane, rotate @Y -90 deg
		glm::vec3 GetXZViewVector_Right();

		// set weather to use thrid person cam
		void SetUseThridPersonCam(bool set);
		// toggle weather to use thrid person cam
		void ToggleUseThridPersonCam();
		// toggle weather thrid person cam can rotate
		void SetThridPersonCamCanRotateAnot(bool set);
		// toggle weather thrid person cam can zoom
		void SetThridPersonCamCanZoomAnot(bool set);
		// set the target location of the thrid person cam
		void SetThridPersonCamTarget(glm::vec3 tgt);
		// set the camera distance of the thrid person cam
		void SetThridPersonCamDistance(float dist);
		// set pitch and yaw
		void SetThridPersonCamPitchAndYaw(float pitch, float yaw);
	};
}
