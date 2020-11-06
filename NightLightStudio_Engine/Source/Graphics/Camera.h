#pragma once

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp" // glm::lookAt
#include "CameraSystem.h"

namespace NS_GRAPHICS
{
	/* Math variables for camera */
	static const float  EPSILON = 0.00001f;
	static const float  PI = 3.14159f;
	static const float  HALF_PI = PI / 2.f;
	static const float  ONE_ROT_STEP = PI / 1800.f; // Each step for rotation, should allow access in the future
	static const float  MAX_PITCH = 1.5534f;
	static const float  MIN_PITCH = -MAX_PITCH;

	class Camera
	{
		// World/Global Up Vector
		glm::vec3 globalUp;

		// Camera Vectors & Position(Point)
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraRight;
		glm::vec3 cameraUp;

		// Dictates how fast the camera will move
		float _rotation_sensitivity = 1000.f;
		float _drag_sensitivity = 200.f;
		float _zoom_sensitivity = 50.f;

		// Variables for camera rotation
		// Given in radians
		float cameraYaw;	// x-axis rotation (Rotation about y axis vector)
		float cameraPitch;  // y-axis rotation (Rotation about x axis vector)

		//////////////////////////////////////////////////////////////
		///// PUBLIC FUNCTIONS
	public:
		Camera()
			: globalUp(0.f, 1.f, 0.f),
			cameraPos(0.f, 20.f, 500.f),
			cameraFront(0.f, 0.f, -1.f),
			cameraRight{ glm::normalize(glm::cross(cameraFront, globalUp)) },
			cameraUp{ glm::normalize(glm::cross(cameraRight, cameraFront)) },
			cameraYaw{ -HALF_PI },
			cameraPitch{ 0.f }
		{
		}

		~Camera() = default;

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}

		const glm::vec3& GetPosition()
		{
			return cameraPos;
		}

		const glm::vec3& GetRight()
		{
			return cameraRight;
		}

		const glm::vec3& GetFront()
		{
			return cameraFront;
		}

		const glm::vec3& GetCameraUp()
		{
			return cameraUp;
		}

		const float& GetRotationSensitivity()
		{
			return _rotation_sensitivity;
		}

		const float& GetDragSensitivity()
		{
			return _drag_sensitivity;
		}

		const float& GetZoomSensitivity()
		{
			return _zoom_sensitivity;
		}

		const float& GetYaw()
		{
			return cameraYaw;
		}

		const float& GetPitch()
		{
			return cameraPitch;
		}

		void SetCameraPosition(const glm::vec3& position)
		{
			cameraPos = position;
		}

		void SetCameraYaw(const float& yaw)
		{
			cameraYaw = yaw;
		}

		void SetCameraPitch(const float& pitch)
		{
			cameraPitch = pitch;
		}

		void SetRotationSensitivity(const float& sensitivity)
		{
			_rotation_sensitivity = sensitivity;
		}

		void SetDragSensitivity(const float& sensitivity)
		{
			_drag_sensitivity = sensitivity;
		}

		void SetZoomSensitivity(const float& sensitivity)
		{
			_zoom_sensitivity = sensitivity;
		}

		// Update vectors & position for current camera based on updated values
		void Update()
		{
			// Calculate the new Front vector
			cameraFront.x = cos(cameraYaw) * cos(cameraPitch);
			cameraFront.y = sin(cameraPitch);
			cameraFront.z = sin(cameraYaw) * cos(cameraPitch);
			cameraFront = glm::normalize(cameraFront);

			// Recalculate Right and Up vector based on updated front vector
			cameraRight = glm::normalize(glm::cross(cameraFront, globalUp));
			cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
		}
	};
}