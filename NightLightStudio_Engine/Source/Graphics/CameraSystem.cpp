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
				_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetFront() * _camera.GetZoomSensitivity());
				updated = true;
			}
		});

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
}