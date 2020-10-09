#pragma once

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp" // glm::lookAt

namespace NS_GRAPHICS
{
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
		float cameraSpeed;

		// Variables for camera rotation
		// Given in radians
		float cameraYaw;	// x-axis rotation
		float cameraPitch;  // y-axis rotation

		//////////////////////////////////////////////////////////////
		///// PUBLIC FUNCTIONS
	public:
		Camera()
			: globalUp(0.f, 1.f, 0.f),
			cameraPos(0.f, 0.f, 5.f),
			cameraFront(0.f, 0.f, -1.f),
			cameraRight{ glm::normalize(glm::cross(cameraFront, globalUp)) },
			cameraUp{ glm::normalize(glm::cross(cameraRight, cameraFront)) },
			cameraSpeed{ 0.05f },
			cameraYaw{ 0.f }, // NUM_STEPS_PI / 2
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

		const float& GetSpeed()
		{
			return cameraSpeed;
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

		void SetCameraSpeed(const float& speed)
		{
			cameraSpeed = speed;
		}

		// Update vectors & position for current camera based on updated values
		void Update()
		{
			// Calculate the new Front vector
			glm::vec3 direction;
			direction.x = cos(cameraYaw) * cos(cameraPitch);
			direction.y = sin(cameraPitch);
			direction.z = sin(cameraYaw) * cos(cameraPitch);
			cameraFront = glm::normalize(direction);

			// Recalculate Right and Up vector based on updated front vector
			cameraRight = glm::normalize(glm::cross(cameraFront, globalUp));
			cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
		}
	};
}