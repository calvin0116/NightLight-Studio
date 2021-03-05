#pragma once

#include "Camera.h"
#include "Shapes.h"
#include "..\Core\DeltaTime.h"
#include <vector>
// For messaging/event
#include "../Messaging/SystemReceiver.h"
#include "../Messaging/Messages/MessageTogglePlay.h"

namespace NS_GRAPHICS
{
	// Const variables used for handling camera movement

	class CameraSystem
	{
		CameraSystem();
		~CameraSystem();

		// Currently only 1 camera
		// Main camera
		Camera _camera;
		Camera _editorCam;
		MyFrustum m_Frustum;

		// Vector of cameras, used to cycle through scene cameras
		//std::vector<Camera> _cameras;

		// Check if scene camera is active, if any
		// Editor camera will be used by default if no scene camera is available
		//bool _activeSceneCamera = false;

		// Camera ID for selected active scene camera
		int _sceneCameraID = -1;

		// Check if rotation has been changed
		bool updatedRot;

		// Check if view matrix has updated at all
		bool updated;

		// Zoom distance
		float zoomDistance;

		// the target the camera is looking at
		// tgt [] <---- >o camera
		glm::vec3 tgt;

		// viewVector
		// tgt [] <---- >o camera
		//          ^
		//          |
		//        this vector
		glm::vec3 viewVector;

		// use thrid person cam?
		bool useThridPersonCam;
		// can the cam rotate?
		bool canThridPersonCamRotate;
		// can the cam zoom?
		bool canThridPersonCamZoom;

		float theThridPersonCamPitch;
		float theThridPersonCamYaw;

		glm::vec3 savedTgt;
		float savedPitch;
		float savedYaw;
		// For receiving event/message
		SystemMessaging::SystemReceiver r;
		bool _isPlaying;
		bool _Inited = false;
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

		void AckUpdate();

		// Editor camera functions
		glm::mat4 GetViewMatrix();

		glm::vec3 GetCurrentCameraPosition();

		void SetRotationSensitivity(const float& sensitivity);
		void SetDragSensitivity(const float& sensitivity);
		void SetZoomSensitivity(const float& sensitivity);
		void SetFOV(const float& fov);
		float GetFOV();

		const float& GetRotationSensitivity();
		const float& GetDragSensitivity();
		const float& GetZoomSensitivity();

		////////////////////////////////////////////
		///NEW QUATERNION CAMERA FUNCTIONS
		////////////////////////////////////////////
		
		// Set current active camera
		//void SetCurrentCamera(const int& cameraID);
		
		// Create camera based on provided parameters
		//int CreateCamera(const glm::vec3& position = glm::vec3(0.f,0.f,0.f), const glm::vec3& target = glm::vec3(0.f, 0.f, 0.f),
						//const float& pitch = 0.f, const float& yaw = 0.f);

		// Getter for specified scene camera camera, used to access its properties
		// If no camera exists, return dummy camera
		//Camera& GetSceneCamera(const int& cameraID);

		// Returns current active camera, regardless of editor or game camera
		//Camera& GetCurrentCamera();

		////////////////////////////////////////////


		// Added by LJM
		void ForceUpdate();
		// Added by LJM
		Camera& GetCamera();

		// update the theird person camera
		void UpdateThirdPersonCamera();
		// update the view vector, view vector is normalsied
		void UpdateViewVector();

		// get upvector                                      //Expose
		glm::vec3 GetUpVector();                             //Expose
		// get rightvector                                   //Expose
		glm::vec3 GetRightVector();                          //Expose

		// get viewVector
		glm::vec3 GetViewVector();                           // Expose
		// get viewVector on XZ plane                        // Expose
		glm::vec3 GetXZViewVector();                         // Expose
		// get viewVector on XZ plane, rotate @Y 180 deg     // Expose
		glm::vec3 GetXZViewVector_Back();                    // Expose
		// get viewVector on XZ plane, rotate @Y 90 deg      // Expose
		glm::vec3 GetXZViewVector_Left();                    // Expose
		// get viewVector on XZ plane, rotate @Y -90 deg     // Expose
		glm::vec3 GetXZViewVector_Right();                   // Expose

		// set third person cam as current active                  //Expose
		void SetUseThridPersonCam(bool set);                       //Expose
		// toggle weather to use thrid person cam                  //Expose
		void ToggleUseThridPersonCam();                            //Expose
		// toggle weather thrid person cam can rotate              //Expose
		void SetThridPersonCamCanRotateAnot(bool set);             //Expose
		// toggle weather thrid person cam can zoom                //Expose
		void SetThridPersonCamCanZoomAnot(bool set);               //Expose
		// set the target location of the thrid person cam         //Expose
		void SetThridPersonCamTarget(glm::vec3 tgt);               //Expose
		// set the camera distance of the thrid person cam         //Expose
		void SetThridPersonCamDistance(float dist);                //Expose
		// set pitch and yaw                                       //Expose
		void SetThridPersonCamPitchAndYaw(float pitch, float yaw); //Expose

		//void SavePosition();
		//void MoveToSavedPosition();
		void CalculateFrustum(Camera& cam);

		void HandleTogglePlay(MessageTogglePlay&);
	};
}
