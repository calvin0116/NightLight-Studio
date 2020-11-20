#pragma once

#include "Camera.h"
#include "..\Core\DeltaTime.h"

namespace NS_GRAPHICS
{
	// Const variables used for handling camera movement

	class CameraSystem
	{
		CameraSystem();
		~CameraSystem();

		// Currently only 1 camera
		// Will be changed to vector of cameras in the future
		Camera _camera;

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

		glm::mat4 GetViewMatrix();

		glm::vec3 GetCurrentCameraPosition();

		void SetRotationSensitivity(const float& sensitivity);
		void SetDragSensitivity(const float& sensitivity);
		void SetZoomSensitivity(const float& sensitivity);

		const float& GetRotationSensitivity();
		const float& GetDragSensitivity();
		const float& GetZoomSensitivity();

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

		// set weather to use thrid person cam                     //Expose
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


		void SavePosition();
		void MoveToSavedPosition();
	};
}
