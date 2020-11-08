#include "CameraSystem.h"
#include "../Input/SystemInput.h"

#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

#include "../IO/Json/Config.h"

#define TESTMOVETARGET 1

namespace NS_GRAPHICS
{
	CameraSystem::CameraSystem()
		: _camera(),
		updatedRot{ false },
		updated{ false },
		zoomDistance{ 100.0f },
		tgt(0.0f, 0.0f, 0.0f),
		useThridPersonCam{false},
		canThridPersonCamRotate{true},
		canThridPersonCamZoom{true},
		viewVector{}
	{
	}

	CameraSystem::~CameraSystem()
	{

	}

	void CameraSystem::Init()
	{
		_camera.SetDragSensitivity(CONFIG_DATA->GetConfigData()._positionSensitivity);
		_camera.SetRotationSensitivity(CONFIG_DATA->GetConfigData()._rotationSensitivity);
		_camera.SetZoomSensitivity(CONFIG_DATA->GetConfigData()._zoomSensitivity);
		// Initialize all required cameras(if any)
		// Currently only one test camera, thus no initialization required

		// Register keys required
		// Move camera based on axis vectors
		
		//Needed for alt key checking in input system
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ALT_REGISTER", SystemInput_ns::IKEY_ALT);

#if TESTMOVETARGET == 1
#define TEST_TARGET_MOVE_STEP 0.5f;

		// toggle thrid person camera
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TOGGLE_THIRD_PERSONCAM", SystemInput_ns::IKEY_P, "TOGGLE_THIRD_PERSONCAM", SystemInput_ns::OnPress, [this]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
			{
				ToggleUseThridPersonCam();

			}
		});

		//SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_FRONT", SystemInput_ns::IKEY_UP, "TARGET_MOVE_FRONT", SystemInput_ns::OnHold, [this]()
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_FRONT", SystemInput_ns::IKEY_W, "TARGET_MOVE_FRONT", SystemInput_ns::OnHold, [this]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
			{
				if (useThridPersonCam)
				{
					tgt += GetXZViewVector() * TEST_TARGET_MOVE_STEP;
				}
			}
		});
		//SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_BACK", SystemInput_ns::IKEY_DOWN, "TARGET_MOVE_BACK", SystemInput_ns::OnHold, [this]()
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_BACK", SystemInput_ns::IKEY_S, "TARGET_MOVE_BACK", SystemInput_ns::OnHold, [this]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
			{
				if (useThridPersonCam)
				{
					tgt += GetXZViewVector_Back() * TEST_TARGET_MOVE_STEP;
				}
			}
		});
		//SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_LEFT", SystemInput_ns::IKEY_LEFT, "TARGET_MOVE_LEFT", SystemInput_ns::OnHold, [this]()
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_LEFT", SystemInput_ns::IKEY_A, "TARGET_MOVE_LEFT", SystemInput_ns::OnHold, [this]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
			{
				if (useThridPersonCam)
				{
					tgt += GetXZViewVector_Left() * TEST_TARGET_MOVE_STEP;
				}
			}
		});
		//SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_RIGHT", SystemInput_ns::IKEY_RIGHT, "TARGET_MOVE_RIGHT", SystemInput_ns::OnHold, [this]()
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TARGET_MOVE_RIGHT", SystemInput_ns::IKEY_D, "TARGET_MOVE_RIGHT", SystemInput_ns::OnHold, [this]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_ALT))
			{
				if (useThridPersonCam)
				{
					tgt += GetXZViewVector_Right() * TEST_TARGET_MOVE_STEP;
				}
			}
		});
#endif

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

		// update thrid person camera
		if(useThridPersonCam)
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
		// cursor setting
		SYS_INPUT->GetSystemMousePos().SetToCenter(true);
		SYS_INPUT->GetSystemMousePos().SetCursorVisible(false);

		// set camera position
		NS_GRAPHICS::Camera& cam = NS_GRAPHICS::CameraSystem::GetInstance().GetCamera();
		glm::vec3 camFront = cam.GetFront();
		camFront *= glm::vec3(zoomDistance, zoomDistance, zoomDistance);
		cam.SetCameraPosition(tgt - camFront);

		//NS_GRAPHICS::CameraSystem::GetInstance().ForceUpdate();

		// camera rotation control
		if (canThridPersonCamRotate)
		{
			//Mouse relative velocity
			glm::vec2 mousePos = SYS_INPUT->GetSystemMousePos().GetRelativeDragVec();

			// Rotation for left and right
			cam.SetCameraYaw(cam.GetYaw() + mousePos.x * NS_GRAPHICS::ROTATION_SENSITIVITY * NS_GRAPHICS::ONE_ROT_STEP);

			// Rotation for up and down
			float offsetted = cam.GetPitch() + mousePos.y * NS_GRAPHICS::ROTATION_SENSITIVITY * NS_GRAPHICS::ONE_ROT_STEP;

			if (offsetted > NS_GRAPHICS::MAX_PITCH)
				offsetted = NS_GRAPHICS::MAX_PITCH;

			if (offsetted < NS_GRAPHICS::MIN_PITCH)
				offsetted = NS_GRAPHICS::MIN_PITCH;

			cam.SetCameraPitch(offsetted);
		}

		// camera zoom control
		if (canThridPersonCamZoom)
		{
			if (SYS_INPUT->GetSystemMousePos().GetIfScrollUp())
			{
				zoomDistance -= NS_GRAPHICS::ZOOM_SENSITIVITY;
			}
			else if (SYS_INPUT->GetSystemMousePos().GetIfScrollDown())
			{
				zoomDistance += NS_GRAPHICS::ZOOM_SENSITIVITY;
			}
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
	glm::vec3 CameraSystem::GetViewVector()
	{
		return viewVector;
	}
	glm::vec3 CameraSystem::GetXZViewVector()
	{
		glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);
		return vec;
	}
	glm::vec3 CameraSystem::GetXZViewVector_Back()
	{
		glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);

		glm::quat quaternion(glm::radians(glm::vec3(0.0f, 180.0f, 0.0f)));
		glm::mat4 rotate = glm::mat4_cast(quaternion);

		vec = rotate * glm::vec4(vec, 1.0f);
		return vec;
	}
	glm::vec3 CameraSystem::GetXZViewVector_Left()
	{
		glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);

		glm::quat quaternion(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
		glm::mat4 rotate = glm::mat4_cast(quaternion);

		vec = rotate * glm::vec4(vec, 1.0f);
		return vec;
	}
	glm::vec3 CameraSystem::GetXZViewVector_Right()
	{
		glm::vec3 vec(viewVector.x, 0.0f, viewVector.z);

		glm::quat quaternion(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));
		glm::mat4 rotate = glm::mat4_cast(quaternion);

		vec = rotate * glm::vec4(vec, 1.0f);
		return vec;
	}
	void CameraSystem::SetUseThridPersonCam(bool set)
	{
		if (useThridPersonCam == set) return;

		useThridPersonCam = set;

		if (!useThridPersonCam)
		{
			// turn off cursor thingy
			SYS_INPUT->GetSystemMousePos().SetToCenter(false);
			SYS_INPUT->GetSystemMousePos().SetCursorVisible(true);
		}
		else
		{
			// this is so the mse cursor update wun cause the camera to move a huge distance when this is toggled
			SYS_INPUT->GetSystemMousePos().SetToCenter(true);
			SYS_INPUT->GetSystemMousePos().Update();

			// update the tgt pos or the camera direction will be wrong when this is toggled
			glm::vec3 camFront = _camera.GetFront();
			camFront *= glm::vec3(zoomDistance, zoomDistance, zoomDistance);
			tgt = _camera.GetPosition() + camFront;
			tgt.y = 0.0f; // set ht to 0 for now

		}
	}
	void CameraSystem::ToggleUseThridPersonCam()
	{
		SetUseThridPersonCam(!useThridPersonCam);
	}
	void CameraSystem::SetThridPersonCamCanRotateAnot(bool _set)
	{
		canThridPersonCamRotate = _set;
	}
	void CameraSystem::SetThridPersonCamCanZoomAnot(bool _set)
	{
		canThridPersonCamZoom = _set;
	}
	void CameraSystem::SetThridPersonCamTarget(glm::vec3 _tgt)
	{
		tgt = _tgt;
	}
	void CameraSystem::SetThridPersonCamDistance(float _dist)
	{
		zoomDistance = _dist;
	}
}