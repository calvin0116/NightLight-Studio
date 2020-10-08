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

		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_A))
		{
			//Position -= Right * velocity;
			_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetRight());
			updated = true;
			std::cout << "Camera has moved left" << std::endl;
		}

		if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_D))
		{
			//Position += Right * velocity;
			_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetRight());
			updated = true;
			std::cout << "Camera has moved right" << std::endl;
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