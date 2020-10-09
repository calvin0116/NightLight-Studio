#include "CameraSystem.h"
#include "../Input/SystemInput.h"

namespace NS_GRAPHICS
{
	CameraSystem::CameraSystem()
		: _camera(),
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

		// Call to activate all keys
		//SYS_INPUT->GetSystemKeyPress().ALL_THE_KEYS();

		// Register keys required
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_FRONT", SystemInput_ns::IKEY_W, "W", SystemInput_ns::OnPress, nullptr);
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_LEFT", SystemInput_ns::IKEY_A, "A", SystemInput_ns::OnPress, nullptr);
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_BACK", SystemInput_ns::IKEY_S, "S", SystemInput_ns::OnPress, nullptr);
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_RIGHT", SystemInput_ns::IKEY_D, "D", SystemInput_ns::OnPress, nullptr);
	}

	void CameraSystem::Update()
	{
		// Call to activate all keys
		//SYS_INPUT->GetSystemKeyPress().ALL_THE_KEYS();

		// Move camera based on axis vectors

		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_A))
		{
			//Position -= Right * velocity;
			_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetRight() * _camera.GetSpeed());
			updated = true;
			std::cout << "Camera has moved left" << std::endl;
		}

		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_D))
		{
			//Position += Right * velocity;
			_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetRight() * _camera.GetSpeed());
			updated = true;
			std::cout << "Camera has moved right" << std::endl;
		}

		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_W))
		{
			//Position += Front * velocity;
			_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetFront() * _camera.GetSpeed());
			updated = true;
			std::cout << "Camera has moved frontwards" << std::endl;
		}

		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_S))
		{
			//Position -= Front * velocity;
			_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetFront() * _camera.GetSpeed());
			updated = true;
			std::cout << "Camera has moved backwards" << std::endl;
		}

		// Move vector via rotation about a point
		// Should be based on offset of mouse x and y from prev frame
		// Should also check if right mouse button is held
		if (NULL)
		{
			//float offset;
			//offset = _camera.GetSpeed() * mouse_xoffset;

			// Rotation about X axis
			//_camera.GetYaw += offset;

			// Constrain to prevent screen flip
			if (_camera.GetYaw() < 0.f)
				_camera.SetCameraYaw(_camera.GetYaw() + TWO_NUM_STEPS_PI);

			else if (_camera.GetYaw() > TWO_NUM_STEPS_PI)
				_camera.SetCameraYaw(_camera.GetYaw() - TWO_NUM_STEPS_PI);
		}

		if (NULL)
		{
			//float offset;
			//offset = _camera.GetSpeed() * mouse_yoffset;

			// Rotation about Y axis
			//cameraPitch += offset;

			// No need to set constraint here
		}

		// Only update the camera properties if changes occurred
		if (updated == true)
		{
			_camera.Update();
		}
	}

	bool CameraSystem::CheckUpdate()
	{
		return updated;
	}

	glm::mat4 CameraSystem::GetViewMatrix()
	{
		// Temporarily returns sole camera, will change to handle multiple cameras
		return _camera.GetViewMatrix();
	}
}