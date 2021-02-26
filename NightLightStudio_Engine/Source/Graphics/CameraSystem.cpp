#include "CameraSystem.h"
#include "../Input/SystemInput.h"

#include "../../glm/gtc/matrix_transform.hpp"

#include "../IO/Json/Config.h"

#include "GraphicsSystem.h"
// Camera comp
#include "../Component/ComponentCamera.h"

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
		viewVector{},
		savedTgt(0.0f, 0.0f, 0.0f),
		savedPitch(0.0f),
		savedYaw(0.0f),
		//_activeSceneCamera{ false },
		theThridPersonCamPitch{ 0.f },
		theThridPersonCamYaw{ 0.f }
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

		//Not working properly
		//glm::vec3 newPos = glm::vec3(CONFIG_DATA->GetConfigData()._lastCamPosX, CONFIG_DATA->GetConfigData()._lastCamPosY, CONFIG_DATA->GetConfigData()._lastCamPosZ);
		//_camera.SetCameraPosition(newPos);

		//_camera.SetCameraPitch(CONFIG_DATA->GetConfigData()._lastCamPitch);
		//_camera.SetCameraYaw(CONFIG_DATA->GetConfigData()._lastCamYaw);
		// Initialize all required cameras(if any)
		// Currently only one test camera, thus no initialization required

		// Register keys required
		// Move camera based on axis vectors
		
		//Needed for alt key checking in input system
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("ALT_REGISTER", SystemInput_ns::IKEY_ALT);

#if TESTMOVETARGET == 1
#define TEST_TARGET_MOVE_STEP 0.5f;

		// toggle thrid person camera
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TOGGLE_THIRD_PERSONCAM", SystemInput_ns::IKEY_T, "TOGGLE_THIRD_PERSONCAM", SystemInput_ns::OnPress, [this]()
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

					CONFIG_DATA->GetConfigData()._lastCamPosX = _camera.GetPosition().x;
					CONFIG_DATA->GetConfigData()._lastCamPosY = _camera.GetPosition().y;
					CONFIG_DATA->GetConfigData()._lastCamPosZ = _camera.GetPosition().z;
				}
			});

		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("MOVE_CAMERA_Z", SystemInput_ns::IKEY_ALT, "Z_CAMERA_MOVE", SystemInput_ns::OnHold, [this]()
		{
			//Only if mouse wheel + alt button is pressed, camera will move.
			if (SYS_INPUT->GetSystemMousePos().GetIfScrollUp())
			{
				_camera.SetCameraPosition(_camera.GetPosition() + _camera.GetFront() * _camera.GetZoomSensitivity());
				updated = true;

				CONFIG_DATA->GetConfigData()._lastCamPosX = _camera.GetPosition().x;
				CONFIG_DATA->GetConfigData()._lastCamPosY = _camera.GetPosition().y;
				CONFIG_DATA->GetConfigData()._lastCamPosZ = _camera.GetPosition().z;
			}
			else if (SYS_INPUT->GetSystemMousePos().GetIfScrollDown())
			{
				_camera.SetCameraPosition(_camera.GetPosition() - _camera.GetFront() * _camera.GetZoomSensitivity());
				updated = true;

				CONFIG_DATA->GetConfigData()._lastCamPosX = _camera.GetPosition().x;
				CONFIG_DATA->GetConfigData()._lastCamPosY = _camera.GetPosition().y;
				CONFIG_DATA->GetConfigData()._lastCamPosZ = _camera.GetPosition().z;
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

					while (offsetted > MAX_PITCH)
						offsetted = MAX_PITCH - _camera.GetRotationSensitivity() * ONE_ROT_STEP;

					_camera.SetCameraPitch(offsetted);

					updatedRot = true;

					CONFIG_DATA->GetConfigData()._lastCamPitch = _camera.GetPitch();
					CONFIG_DATA->GetConfigData()._lastCamYaw = _camera.GetYaw();
				}
			});

		// For messaging/event system
		r.AttachHandler("TogglePlay", &CameraSystem::HandleTogglePlay, this);
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

		// update the view vector
		UpdateViewVector();


		// update thrid person camera
		if (useThridPersonCam)
			UpdateThirdPersonCamera();
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

  void CameraSystem::SetFOV(const float& fov)
  {
    _camera.SetCameraFOV(fov);
    GraphicsSystem::GetInstance()->SetProjectionMatrix(fov);
  }

	float CameraSystem::GetFOV()
	{
		return _camera.GetCameraFOV();
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

	//void CameraSystem::SetCurrentCamera(const int& cameraID)
	//{
	//	_sceneCameraID = cameraID;
	//}

	//int CameraSystem::CreateCamera(const glm::vec3& position, const glm::vec3& target, const float& pitch, const float& yaw)
	//{
	//	Camera newCam;

	//	newCam.SetCameraPosition(position);
	//	newCam.SetCameraPitch(pitch);
	//	newCam.SetCameraYaw(yaw);
	//	newCam.SetCameraTarget(target);

	//	_cameras.push_back(newCam);

	//	return (int)_cameras.size() - 1;
	//}
	/*
	Camera& CameraSystem::GetSceneCamera(const int& cameraID)
	{
		if (cameraID >= 0 && cameraID < _cameras.size())
			return _cameras[cameraID];

		//Camera dummyCam;

		//return dummyCam;
	}*/

	//Camera& CameraSystem::GetCurrentCamera()
	//{
	//	if (!_activeSceneCamera)
	//		return _camera; // Editor Camera
	//	else
	//		return _cameras[_sceneCameraID];
	//}

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
		//_camera.SetCameraPosition(pos);

		//NS_GRAPHICS::CameraSystem::GetInstance().ForceUpdate();

		// camera rotation control
		if (canThridPersonCamRotate)
		{
			//Mouse relative velocity
			glm::vec2 mousePos = SYS_INPUT->GetSystemMousePos().GetRelativeDragVec();

			// this can limit motion - me was testing
			//if (mousePos.x >  0.05f)
			//	mousePos.x =  0.05f;
			//if (mousePos.y >  0.05f)
			//	mousePos.y =  0.05f;
			//if (mousePos.x < -0.05f)
			//	mousePos.x = -0.05f;
			//if (mousePos.y < -0.05f)
			//	mousePos.y = -0.05f;

			// Rotation for left and right
			_camera.SetCameraYaw(_camera.GetYaw() + mousePos.x * NS_GRAPHICS::ROTATION_SENSITIVITY * NS_GRAPHICS::ONE_ROT_STEP);

			// Rotation for up and down
			float offsetted = _camera.GetPitch() + mousePos.y * NS_GRAPHICS::ROTATION_SENSITIVITY * NS_GRAPHICS::ONE_ROT_STEP;

			if (offsetted > NS_GRAPHICS::MAX_PITCH)
				offsetted = NS_GRAPHICS::MAX_PITCH;

			if (offsetted < NS_GRAPHICS::MIN_PITCH)
				offsetted = NS_GRAPHICS::MIN_PITCH;

			_camera.SetCameraPitch(offsetted);

			//std::cout << _camera.GetYaw() << ", " << _camera.GetPitch() << std::endl;
			//std::cout << mousePos.x << ", " << mousePos.y << std::endl;
		}
		else
		{
			//glm::vec3 viewVec = _camera.GetFront();
			//_camera.SetCameraYaw(atan2(viewVec.x, viewVec.z));

			//float offsetted = asin(-viewVec.y);

			//if (offsetted > NS_GRAPHICS::MAX_PITCH)
			//	offsetted = NS_GRAPHICS::MAX_PITCH;

			//if (offsetted < NS_GRAPHICS::MIN_PITCH)
			//	offsetted = NS_GRAPHICS::MIN_PITCH;

			//_camera.SetCameraPitch(offsetted);


			if (theThridPersonCamPitch > NS_GRAPHICS::MAX_PITCH)
				theThridPersonCamPitch = NS_GRAPHICS::MAX_PITCH;

			if (theThridPersonCamPitch < NS_GRAPHICS::MIN_PITCH)
				theThridPersonCamPitch = NS_GRAPHICS::MIN_PITCH;

			_camera.SetCameraPitch(theThridPersonCamPitch);
			_camera.SetCameraYaw(theThridPersonCamYaw);

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
		viewVector = _camera.GetFront();
	}
	glm::vec3 CameraSystem::GetUpVector()
	{
		return _camera.GetCameraUp();
	}
	glm::vec3 CameraSystem::GetRightVector()
	{
		return _camera.GetRight();
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
			//tgt.y = 0.0f; // set ht to 0 for now


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

	void CameraSystem::SetThridPersonCamPitchAndYaw(float pitch, float yaw)
	{

		theThridPersonCamPitch = pitch;

		theThridPersonCamYaw = yaw;
	}

	//void CameraSystem::SavePosition()
	//{
	//	savedTgt = tgt;
	//	savedPitch = _camera.GetPitch();
	//	savedYaw = _camera.GetYaw();
	//}

	//void CameraSystem::MoveToSavedPosition()
	//{
	//	tgt = savedTgt;

	//	theThridPersonCamPitch = savedPitch;
	//	theThridPersonCamYaw = savedYaw;

	//	_camera.SetCameraPitch(theThridPersonCamPitch);
	//	_camera.SetCameraYaw(theThridPersonCamYaw);

	//	SetUseThridPersonCam(false);
	//}

	void CameraSystem::HandleTogglePlay(MessageTogglePlay& msg)
	{
		// Handle msg here. Only Before Play MSG
		if (msg.GetID() != "TogglePlay")
			return;
		_isPlaying = msg.isPlaying;
		if (_isPlaying && _Inited == false)
		{
			_Inited = true;
			//MyGameInit();
			_editorCam = _camera;
			auto itr1 = G_ECMANAGER->begin<ComponentCamera>();
			auto itrEnd1 = G_ECMANAGER->end<ComponentCamera>();
			for (; itr1 != itrEnd1; ++itr1)
			{
				ComponentCamera* myComp = G_ECMANAGER->getComponent<ComponentCamera>(itr1);
				 //Only 1 active camera during gameplay.
				if (myComp->_isActive)
				{
					_camera = myComp->_data;
					break;
				}
			}
		}
		else if (!_isPlaying)
		{
			//ReleaseSounds();
			_Inited = false;
			_camera = _editorCam;
		}
	}
}