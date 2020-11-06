#include "CameraSystem.h"
#include "../Input/SystemInput.h"

#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

namespace NS_GRAPHICS
{
	CameraSystem::CameraSystem()
		: _camera(),
		updatedRot{ false },
		updated{ false },
		zoomDistance{ 100.0f },
		tgt(0.0f, 0.0f, 0.0f)
	{
	}

	CameraSystem::~CameraSystem()
	{

	}

	void CameraSystem::Init()
	{

#define TEST_TARGET_MOVE_STEP 0.5f;
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_FRONT", SystemInput_ns::IKEY_W, "TARGET_MOVE_FRONT", SystemInput_ns::OnHold, [this]()
		{
			glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);
			tgt += vec * TEST_TARGET_MOVE_STEP;
			//tgt.z -= TEST_TARGET_MOVE_STEP;
		});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_BACK", SystemInput_ns::IKEY_S, "TARGET_MOVE_BACK", SystemInput_ns::OnHold, [this]()
		{
			glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);

			glm::quat quaternion(glm::radians(glm::vec3(0.0f, 180.0f, 0.0f)));
			glm::mat4 rotate = glm::mat4_cast(quaternion);

			vec = rotate * glm::vec4(vec, 1.0f);

			tgt += vec * TEST_TARGET_MOVE_STEP;
			//tgt.z += TEST_TARGET_MOVE_STEP;
		});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_LEFT", SystemInput_ns::IKEY_A, "TARGET_MOVE_LEFT", SystemInput_ns::OnHold, [this]()
		{
			glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);

			glm::quat quaternion(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
			glm::mat4 rotate = glm::mat4_cast(quaternion);

			vec = rotate * glm::vec4(vec, 1.0f);

			tgt += vec * TEST_TARGET_MOVE_STEP;
			//tgt.x -= TEST_TARGET_MOVE_STEP;
		});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_RIGHT", SystemInput_ns::IKEY_D, "TARGET_MOVE_RIGHT", SystemInput_ns::OnHold, [this]()
		{
			glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);

			glm::quat quaternion(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));
			glm::mat4 rotate = glm::mat4_cast(quaternion);

			vec = rotate * glm::vec4(vec, 1.0f);

			tgt += vec * TEST_TARGET_MOVE_STEP;
			//tgt.x += TEST_TARGET_MOVE_STEP;
		});




		// Initialize all required cameras(if any)
		// Currently only one test camera, thus no initialization required

		// Register keys required
		// Move camera based on axis vectors
		
		//Needed for alt key checking in input system
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ALT_REGISTER", SystemInput_ns::IKEY_ALT);

		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_XY", SystemInput_ns::IMOUSE_MBUTTON, "XY_CAMERA_MOVE", SystemInput_ns::OnHold, [this]()
			{
				//Only if mouse + alt button is pressed, camera will move.
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
				{
					glm::vec2 mousePos = SYS_INPUT->GetSystemMousePos().GetRelativeDragVec();
					//handles topdown
					glm::vec3 newCameraOffset = _camera.GetCameraUp() * -mousePos.y * _camera.GetDragSensitivity();
					//newCameraOffset *= DELTA_T->dt;
					//_camera.SetCameraPosition(_camera.GetPosition() + newCameraOffset);

					//handles leftright
					newCameraOffset += _camera.GetRight() * -mousePos.x * _camera.GetDragSensitivity();
					//newCameraOffset *= DELTA_T->dt;

					//Position += CameraUp * velocity;
					_camera.SetCameraPosition(_camera.GetPosition() + newCameraOffset);
					updated = true;
				}
			});

		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_Z", SystemInput_ns::IKEY_ALT, "Z_CAMERA_MOVE", SystemInput_ns::OnHold, [this]()
		{
			//Only if mouse wheel + alt button is pressed, camera will move.
			if (SYS_INPUT->GetSystemMousePos().GetIfScrollUp())
			{
				_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetFront() * _camera.GetZoomSensitivity());
				updated = true;
			}
			else if (SYS_INPUT->GetSystemMousePos().GetIfScrollDown())
			{
				_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetFront() * ZOOM_SENSITIVITY);
				updated = true;
			}
		});

		/*SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_DOWN", SystemInput_ns::IKEY_DOWN, "DOWN", SystemInput_ns::OnHold, [this]()
			{
				//Position -= CameraUp * velocity;
				_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetCameraUp() * _camera.GetSpeed());
				updated = true;
			});

		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_Z", SystemInput_ns::IKEY_W, "W", SystemInput_ns::OnHold, [this]()
			{
			//Only if mouse left button is pressed, camera will moved with w.
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IMOUSE_LBUTTON))
				{
					//Position += Front * velocity;
					_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetFront() * _camera.GetSpeed());
						updated = true;
				}
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_LEFT", SystemInput_ns::IKEY_A, "A", SystemInput_ns::OnHold, [this]()
			{
			//Only if mouse left button is pressed, camera will moved with a.
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IMOUSE_LBUTTON))
				{
					//Position -= Right * velocity;
					_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetRight() * _camera.GetSpeed());
					updated = true;
				}
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_BACK", SystemInput_ns::IKEY_S, "S", SystemInput_ns::OnHold, [this]()
			{
			//Only if mouse left button is pressed, camera will moved with s.
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IMOUSE_LBUTTON))
				{
					//Position -= Front * velocity;
					_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetFront() * _camera.GetSpeed());
					updated = true;
				}
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_RIGHT", SystemInput_ns::IKEY_D, "D", SystemInput_ns::OnHold, [this]()
			{
			//Only if mouse left button is pressed, camera will moved with d.
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IMOUSE_LBUTTON))
				{
					//Position += Right * velocity;
					_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetRight() * _camera.GetSpeed());
					updated = true;
				}
			});*/

		// Rotation
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ROTATE_CAMERA", SystemInput_ns::IMOUSE_LBUTTON, "ROTATE_CAMERA_CLICK", SystemInput_ns::OnHold, [this]()
			{
				//Only if mouse + alt button is pressed, camera will rotate.
				if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
				{
					//Mouse relative velocity
					glm::vec2 mousePos = SYS_INPUT->GetSystemMousePos().GetRelativeDragVec();

					// Rotation for left and right
					_camera.SetCameraYaw(_camera.GetYaw() + mousePos.x * _camera.GetRotationSensitivity() * ONE_ROT_STEP);

					// Rotation for up and down
					float offsetted = _camera.GetPitch() + mousePos.y * _camera.GetRotationSensitivity() * ONE_ROT_STEP;

					if (offsetted > MAX_PITCH)
						offsetted = MAX_PITCH;

					_camera.SetCameraPitch(offsetted);

					updatedRot = true;
				}
			});
		/*SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ROTATE_CAMERA_RIGHT", SystemInput_ns::IKEY_RIGHT, "RIGHT", SystemInput_ns::OnHold, [this]()
			{
				// Rotation to the right
				_camera.SetCameraYaw(_camera.GetYaw() + ONE_ROT_STEP);
				updatedRot = true;
			});*/

		/*SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ROTATE_CAMERA_UP", SystemInput_ns::IKEY_UP, "UP", SystemInput_ns::OnHold, [this]()
			{
				float offsetted = _camera.GetPitch() + ONE_ROT_STEP;

				if (offsetted > MAX_PITCH)
					offsetted = MAX_PITCH;

				// Rotation to the top
				_camera.SetCameraPitch(offsetted);
				updatedRot = true;
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ROTATE_CAMERA_DOWN", SystemInput_ns::IKEY_DOWN, "DOWN", SystemInput_ns::OnHold, [this]()
			{
				float offsetted = _camera.GetPitch() - ONE_ROT_STEP;

				// Check if out of bounds to prevent flipping
				if (offsetted < MIN_PITCH)
					offsetted = MIN_PITCH;

				// Rotation to the bottom
				_camera.SetCameraPitch(offsetted);
				updatedRot = true;
			});*/
	}

	void CameraSystem::Update()
	{

		// update thrid person camera
		UpdateThirdPersonCamera();


		// Call to activate all keys
		//SYS_INPUT->GetSystemKeyPress().ALL_THE_KEYS();

		// Only update the camera vectors if changes to rotation occurred
		// Function is required as vectors require more elaborate calculation compared to position
		if (updatedRot == true)
		{
			_camera.Update();
			updatedRot = false;
			updated = true;
		}

		// update the view vector
		UpdateViewVector();
	}

	bool CameraSystem::CheckUpdate()
	{
		return updated;
	}

	void CameraSystem::AckUpdate()
	{
		updated = false;
	}

	void CameraSystem::ForceUpdate()
	{
		updated = true;
	}

	glm::mat4 CameraSystem::GetViewMatrix()
	{
		// Temporarily returns sole camera, will change to handle multiple cameras
		return _camera.GetViewMatrix();
	}
	glm::vec3 CameraSystem::GetCurrentCameraPosition()
	{
		return _camera.GetPosition();
	}
	void CameraSystem::SetRotationSensitivity(const float& sensitivity)
	{
		_camera.SetRotationSensitivity(sensitivity);
	}

	void CameraSystem::SetDragSensitivity(const float& sensitivity)
	{
		_camera.SetDragSensitivity(sensitivity);
	}

	void CameraSystem::SetZoomSensitivity(const float& sensitivity)
	{
		_camera.SetZoomSensitivity(sensitivity);
	}

	const float& CameraSystem::GetRotationSensitivity()
	{
		return _camera.GetRotationSensitivity();
	}

	const float& CameraSystem::GetDragSensitivity()
	{
		return _camera.GetDragSensitivity();
	}

	const float& CameraSystem::GetZoomSensitivity()
	{
		return _camera.GetZoomSensitivity();
	}

	Camera& CameraSystem::GetCamera()
	{
		return _camera;
	}
	void CameraSystem::UpdateThirdPersonCamera()
	{
		SYS_INPUT->GetSystemMousePos().SetTheThing(true);
		SYS_INPUT->GetSystemMousePos().SetCursorVisible(false);

		//glm::vec3 tgt = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 eye;
		//float dist = 100.0f;

		NS_GRAPHICS::Camera& cam = NS_GRAPHICS::CameraSystem::GetInstance().GetCamera();
		glm::vec3 camFront = cam.GetFront();
		camFront *= glm::vec3(zoomDistance, zoomDistance, zoomDistance);
		cam.SetCameraPosition(tgt - camFront);

		//NS_GRAPHICS::CameraSystem::GetInstance().ForceUpdate();

		//Mouse relative velocity
		glm::vec2 mousePos = SYS_INPUT->GetSystemMousePos().GetRelativeDragVec();

		// Rotation for left and right
		cam.SetCameraYaw(cam.GetYaw() + mousePos.x * NS_GRAPHICS::ROTATION_SENSITIVITY * NS_GRAPHICS::ONE_ROT_STEP);

		// Rotation for up and down
		float offsetted = cam.GetPitch() + mousePos.y * NS_GRAPHICS::ROTATION_SENSITIVITY * NS_GRAPHICS::ONE_ROT_STEP;

		if (offsetted > NS_GRAPHICS::MAX_PITCH)
			offsetted = NS_GRAPHICS::MAX_PITCH;

		cam.SetCameraPitch(offsetted);

		if (SYS_INPUT->GetSystemMousePos().GetIfScrollUp())
		{
			zoomDistance += NS_GRAPHICS::ZOOM_SENSITIVITY;

			//_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetFront() * ZOOM_SENSITIVITY);
			updated = true;
		}
		else if (SYS_INPUT->GetSystemMousePos().GetIfScrollDown())
		{
			zoomDistance -= NS_GRAPHICS::ZOOM_SENSITIVITY;

			//_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetFront() * ZOOM_SENSITIVITY);
			updated = true;
		}

		updatedRot = true;
		updated = true;
	}
	void CameraSystem::UpdateViewVector()
	{
		glm::vec3 camFront = _camera.GetFront();
		glm::vec3 camPositron = _camera.GetPosition();
		viewVector = camFront - camPositron;
		viewVector = glm::normalize(viewVector);
	}
}