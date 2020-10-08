#pragma once

#include "Camera.h"

namespace NS_GRAPHICS
{
	// Const variables used for handling camera movement
	/*  The step for rotation */
	static const float  PI = 3.14159f;
	static const int    NUM_STEPS_PI = 36;
	static const int    TWO_NUM_STEPS_PI = 2 * NUM_STEPS_PI;
	static const float  ONE_STEP = PI / NUM_STEPS_PI;

	/*  Limit for camera angle */
	static const int EYE_MAX_ALPHA = NUM_STEPS_PI / 2 - 1;
	static const int EYE_MIN_ALPHA = -EYE_MAX_ALPHA + 1;
	static const int EYE_ANGLE_STEP = 1;

	/*  Limit for camera distance from the origin */
	static const int EYE_MAX_RADIUS = 40;
	static const int EYE_MIN_RADIUS = 15;
	static const int EYE_RADIUS_STEP = 1;

	class CameraSystem
	{
		CameraSystem();
		~CameraSystem();

		// Currently only 1 camera
		// Will be changed to vector of cameras in the future
		Camera _camera;

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

		glm::mat4 GetViewMatrix();
	};
}
