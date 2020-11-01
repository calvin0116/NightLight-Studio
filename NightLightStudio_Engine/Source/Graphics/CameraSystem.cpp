#include "CameraSystem.h"
#include "../Input/SystemInput.h"

namespace NS_GRAPHICS
{
	CameraSystem::CameraSystem()
		: _camera(),
		updatedRot{ false },
		updated{ false }
	{
	}

	CameraSystem::~CameraSystem()
	{

	}

	void CameraSystem::Init()
	{
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
					glm::vec3 newCameraOffset = _camera.GetCameraUp() * -mousePos.y * POSITION_SENSITIVITY;
					//newCameraOffset *= DELTA_T->dt;
					//_camera.SetCameraPosition(_camera.GetPosition() + newCameraOffset);

					//handles leftright
					newCameraOffset += _camera.GetRight() * -mousePos.x * POSITION_SENSITIVITY;
					//newCameraOffset *= DELTA_T->dt;

					//Position += CameraUp * velocity;
					_camera.SetCameraPosition(_camera.GetPosition() + newCameraOffset);
					updated = true;
				}
			});

		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_Z", SystemInput_ns::IKEY_ALT, "Z_CAMERA_MOVE", SystemInput_ns::OnHold, [this]()
		{
			//Only if mouse wheel + alt button is pressed, camera will move.
			//NO CAMERA SPEED AS IT IS TOO FAST FOR FORWARD MOVEMENT
			if (SYS_INPUT->GetSystemMousePos().GetIfScrollUp())
			{
				_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetFront());
				updated = true;
			}
			else if (SYS_INPUT->GetSystemMousePos().GetIfScrollDown())
			{
				_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetFront());
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
					_camera.SetCameraYaw(_camera.GetYaw() + mousePos.x * ROTATION_SENSITIVITY * ONE_ROT_STEP);

					// Rotation for up and down
					float offsetted = _camera.GetPitch() + mousePos.y * ROTATION_SENSITIVITY * ONE_ROT_STEP;

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
	Camera& CameraSystem::GetCamera()
	{
		return _camera;
	}
}