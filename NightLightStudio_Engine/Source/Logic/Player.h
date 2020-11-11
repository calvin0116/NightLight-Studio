#pragma once
#include "../Math/Vector.h"
#include "../Math/Matrix4x4.h"
#include "../Component/ComponentRigidBody.h"
#include "../Component/ComponentCollider.h"
#include "../Component/ComponentTransform.h"
#include "../Physics/SystemPhysics.h"
#include "../Input/SystemInput.h"
#include "../../glm/gtx/euler_angles.hpp"
#include "CScripts/IScript.h"
#include "CameraScript.h"
#include "../Graphics/CameraSystem.h"
#include "../Component/ComponentPlayerStats.h"



enum class PLAYERSTATE
{
	HUMAN = 0,
	MOTH,
	POSSESSED
};

class Player : public IScript
{
	Entity playerEntity;
	AllScripts::CameraScript*camera;
	AllScripts::CameraScript* possessStateCamera;
	float _playerEnergy;

	NlMath::Vector3D currentCameraDirection;

	//keep a copy of the component
	ComponentCollider* comCol;
	ComponentRigidBody* comRigid;
	ComponentTransform* comTrans;
	ComponentTransform spawnPoint;
	ComponentPlayerStats* playerStats;

	PLAYERSTATE _prevPlayerState;
	PLAYERSTATE _playerState;
	NlMath::Vector3D _front;
	
public:
  // Expose variables
  //Player stats
  float PLAYER_MOVE_MAG;
  float PLAYER_FLY_MAG;
  int		PLAYER_MAX_ENERGY;
  int		PLAYER_ENERGY_REGEN;
  int		PLAYER_POSSESS_ENERGY_DRAIN;
  int		PLAYER_MOTH_ENERGY_DRAIN;
  // Camera Variables
  float CAMERA_DISTANCE;
  float CAMERA_OFFSET_X;
  float CAMERA_OFFSET_Y;
  float PLAYER_MAX_SPEED;

	Player();
	void Init() override;
	void Update() override;
	void Exit() override;
	PLAYERSTATE getState();
	void changeState(PLAYERSTATE state);
	void setSpawnPoint(ComponentTransform& _spawnPoint);
	bool enterPossession();
	virtual void OnCollisionEnter(Entity other) override;
};