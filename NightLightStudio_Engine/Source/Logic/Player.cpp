#pragma once
#include "Player.h"
#include "../Component/ComponentCScript.h"
#include "../Messaging/SystemBroadcaster.h"
#include "../Messaging/Messages/MessageScriptRequest.h"
#include "CScripts/PossessScript.h"
#include "SpawnPoint.h"
#include "../Core/DeltaTime.h"
#include "../Graphics/LightSystem.h"
#include "../Component/ComponentGraphics.h"

#include "CScripts/FanScript.h"

////need to be edited by designer
//#define PLAYER_MOVE_MAG 3000.0f
//#define PLAYER_FLY_MAG 4.0f
//#define PLAYER_MAX_ENERGY 15
//#define PLAYER_ENERGY_REGEN 1
//#define PLAYER_POSSESS_ENERGY_DRAIN 1
//#define PLAYER_MOTH_ENERGY_DRAIN 1
////#define CAMERA_DISTANCE 4
//#define CAMERA_DISTANCE 500.0f
//#define CAMERA_OFFSET_X 50.0f
//#define CAMERA_OFFSET_Y 10.0f
//#define PLAYER_MAX_SPEED 500.0f

/// <control mapping>
////////////////////////////////////////////////////////////////////////////////////////////
#define WALKLEFT SystemInput_ns::IKEY_A
#define WALKRIGHT SystemInput_ns::IKEY_D
#define WALKFRONT SystemInput_ns::IKEY_W
#define WALKBACK SystemInput_ns::IKEY_S
#define POSSESS SystemInput_ns::IMOUSE_RBUTTON
/// ////////////////////////////////////////////////////////////////////////////////////////
/// </control mapping>

Player::Player()
	:comCol(nullptr), comRigid(nullptr), comTrans(nullptr),comLight(nullptr),
	_playerState(PLAYERSTATE::HUMAN), _prevPlayerState(PLAYERSTATE::HUMAN),
	_front(0), camera(nullptr), possessStateCamera(nullptr), _playerEnergy(PLAYER_MAX_ENERGY)
{
	spawnPoint._rotation = { 0,0,0 };
	spawnPoint._position = { 0,0,0 };
	spawnPoint._scale = { 1,1,1 };
}

void Player::Init()
{
	// Pass in entity name that script is attached to !! Name must be unique, duplicate names will result in getting the one with lower Entity ID !!
	MessageScriptRequest msg(1);
	// Process the request
	GLOBAL_SYSTEM_BROADCAST.ProcessMessage(msg);
	// Get script from msg.
	camera = dynamic_cast<AllScripts::CameraScript*>(msg._pScript);
	camera->SetActive(true);

	playerEntity = MyID;
	comCol = MyID.getComponent<ColliderComponent>();
	comRigid = MyID.getComponent<RigidBodyComponent>();
	comTrans = MyID.getComponent<TransformComponent>();
	//set this as a point light, scale to 0.001
	comLight = MyID.getComponent<LightComponent>();
	comLight->ChangeLightType(NS_GRAPHICS::Lights::POINT);
	comLight->SetIntensity(1000);

	//comLight->change
	playerStats = MyID.getComponent<PlayerStatsComponent>();
	//Player stats
	PLAYER_MOVE_MAG							= playerStats->player_move_mag;
	PLAYER_FLY_MAG							= playerStats->player_fly_mag;
	PLAYER_MAX_ENERGY						= playerStats->player_max_energy;
	PLAYER_ENERGY_REGEN					= 1;
	PLAYER_POSSESS_ENERGY_DRAIN	= playerStats->player_possess_energy_drain;
	PLAYER_MOTH_ENERGY_DRAIN		= playerStats->player_moth_energy_drain;
	PLAYER_MAX_SPEED						= playerStats->player_max_speed;
	// Camera Variables
	CAMERA_DISTANCE							= playerStats->camera_distance;
	CAMERA_OFFSET_X							= playerStats->camera_offset.x;
	CAMERA_OFFSET_Y							= playerStats->camera_offset.y;
	//for state change
	init_CAMERA_DISTANCE =					CAMERA_DISTANCE;
	init_CAMERA_OFFSET_X =					CAMERA_OFFSET_X;

	// /*To Do*/ need to set a max force for the player

	//initially the player is a human with human controls
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk1", WALKFRONT, "WalkFront", SystemInput_ns::OnHold, [this]()
		{
			glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector();
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);



		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop1", WALKFRONT, "StopFront", SystemInput_ns::OnRelease, [this]()
		{
			comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
			comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk2", WALKLEFT, "WalkLeft", SystemInput_ns::OnHold, [this]()
		{

			glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Left();
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop2", WALKLEFT, "Stopleft", SystemInput_ns::OnRelease, [this]()
		{
			comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
			comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk3", WALKBACK, "WalkBack", SystemInput_ns::OnHold, [this]()
		{
			//negative direction
			glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Back();
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop3", WALKBACK, "StopBack", SystemInput_ns::OnRelease, [this]()
		{
			comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
			comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk4", WALKRIGHT, "WalkRight", SystemInput_ns::OnHold, [this]()
		{
			glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Right();
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop4", WALKRIGHT, "StopRight", SystemInput_ns::OnRelease, [this]()
		{
			comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
			comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
		});
	//state change control
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Become Butterfly", POSSESS, "moth", SystemInput_ns::OnPress, [this]()
		{
			if (_playerState == PLAYERSTATE::HUMAN)
			{
				changeState(PLAYERSTATE::MOTH);
			}
			else if (_playerState == PLAYERSTATE::MOTH)
			{
				changeState(PLAYERSTATE::HUMAN);
			}
			else if (_playerState == PLAYERSTATE::POSSESSED || _playerState == PLAYERSTATE::POSSESSED_FAN)
			{
				changeState(PLAYERSTATE::HUMAN);
			}
      //else if (_playerState == PLAYERSTATE::POSSESSED_FAN)
      //{
      //  //std::cout << "Exiting POSSESED FAN" << std::endl;
      //  changeState()
      //}
		});
}

void Player::Update()
{
	/*std::cout << "Speed is X:" <<comRigid->velocity.x << "   Y:"<<comRigid->velocity.y << "  Z:"<<comRigid->velocity.z << std::endl;*/
  if (camera == nullptr)
    return;
	switch (_playerState)
	{
	case PLAYERSTATE::HUMAN:
	{

		//std::cout << "Player State is : Human" << std::endl;
		 // update camera position with player position
		float speedMag = comRigid->velocity.length();
		if (speedMag > PLAYER_MAX_SPEED)
		{
			comRigid->velocity = (comRigid->velocity / speedMag) * PLAYER_MAX_SPEED;
		}
		//refill energy
		if (_playerEnergy <= PLAYER_MAX_ENERGY)
		{
			float realDt = DELTA_T->dt / CLOCKS_PER_SEC;
			_playerEnergy += realDt * PLAYER_ENERGY_REGEN;
		}
		
		//camera follow
		camera->SetTarget(comTrans->_position);
		camera->SetDistance(CAMERA_DISTANCE);
		camera->SetTargetOffsetXY(CAMERA_OFFSET_X, CAMERA_OFFSET_Y);
    camera->SetRotate(true);
		break;
	}
		
	case PLAYERSTATE::MOTH:
	{
		//std::cout << "Player State is : moth" << std::endl;
		//reduce energy when flying
		float realDt = DELTA_T->dt / CLOCKS_PER_SEC;
		_playerEnergy -= realDt * PLAYER_MOTH_ENERGY_DRAIN;
		if (_playerEnergy <= 0)
		{
			changeState(PLAYERSTATE::HUMAN);
		}

		// update camera position with player position
		camera->SetTarget(comTrans->_position);
		camera->SetDistance(CAMERA_DISTANCE);
		camera->SetTargetOffsetXY(CAMERA_OFFSET_X, CAMERA_OFFSET_Y);
		//uncontrolable motion
		comRigid->velocity =  (NS_GRAPHICS::CameraSystem::GetInstance().GetViewVector()* PLAYER_FLY_MAG);
		
		break;
	}
		
	case PLAYERSTATE::POSSESSED:
	{
		//std::cout << "Player State is : possessed" << std::endl;
		//reduce energy
		float realDt = DELTA_T->dt / CLOCKS_PER_SEC;
		_playerEnergy -= realDt;
		if (_playerEnergy <= 0)
		{
			changeState(PLAYERSTATE::HUMAN);
		}
	}
		break;
  case PLAYERSTATE::POSSESSED_FAN:
  {
    // reduce energy
    float realDt = DELTA_T->dt / CLOCKS_PER_SEC;
    _playerEnergy -= realDt;
    if (_playerEnergy <= 0)
    {
      changeState(PLAYERSTATE::HUMAN);
    }
    break;
  }
	default:
		break;
	}
}

void Player::Exit()
{
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk1");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk2");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk3");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk4");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop1");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop2");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop3");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop4");
	SYS_INPUT->GetSystemKeyPress().RemoveEvent("Become Butterfly");
}

PLAYERSTATE Player::getState()
{
	return _playerState;
}

float Player::getPlayerEnergy()
{
	return _playerEnergy;
}

void Player::changeState(PLAYERSTATE state)
{
	_prevPlayerState = _playerState;
	_playerState = state;
	switch (_playerState)
	{
	case PLAYERSTATE::HUMAN:
	{
		comTrans->_scale *= 3.0f;
		//enable rigid body
		comRigid->isStatic = false;
		comRigid->isGravity = true;

		if (_prevPlayerState == PLAYERSTATE::MOTH)
		{
			CAMERA_DISTANCE = init_CAMERA_DISTANCE;
			CAMERA_OFFSET_X = init_CAMERA_OFFSET_X;

			// stop player from sliding if moving before the state change;
			comRigid->velocity = 0;
			comRigid->acceleration = 0;
		}
		else if (_prevPlayerState == PLAYERSTATE::POSSESSED)
		{
			// disable pos cam
			possessStateCamera->SetActive(false);

			// do camera settings on player camera
			camera->SetActive(true);
			camera->SetRotate(true);
			//camera->SetZoom(true);

			// change player state
			changeState(PLAYERSTATE::HUMAN);

			// turn off player collider
			MyID.getComponent<ComponentCollider>()->isCollidable = true;

			// set rigid body to static
			MyID.getComponent<ComponentRigidBody>()->isStatic = false;
			
		}
    else if (_prevPlayerState == PLAYERSTATE::POSSESSED_FAN)
    {

      camera->SetTarget(comTrans->_position);
      camera->SetDistance(init_CAMERA_DISTANCE);
      //camera->SetRotate(false);
      // turn off collider
      MyID.getComponent<ComponentCollider>()->isCollide = true;
      MyID.getComponent<ComponentCollider>()->isCollidable = true;
      //MyID.getComponent<ComponentGraphics>()->_isActive = true;
      comRigid->isStatic = false;
      comRigid->isActive = true;
    }
		//player is a human with human controls
	//initially the player is a human with human controls
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk1", WALKFRONT, "WalkFront", SystemInput_ns::OnHold, [this]()
			{
				glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector();
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop1", WALKFRONT, "StopFront", SystemInput_ns::OnRelease, [this]()
			{
				comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
				comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk2", WALKLEFT, "WalkLeft", SystemInput_ns::OnHold, [this]()
			{

				glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Left();
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop2", WALKLEFT, "Stopleft", SystemInput_ns::OnRelease, [this]()
			{
				comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
				comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk3", WALKBACK, "WalkBack", SystemInput_ns::OnHold, [this]()
			{
				//negative direction
				glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Back();
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop3", WALKBACK, "StopBack", SystemInput_ns::OnRelease, [this]()
			{
				comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
				comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk4", WALKRIGHT, "WalkRight", SystemInput_ns::OnHold, [this]()
			{
				glm::vec3 force = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector_Right();
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, force, PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Stop4", WALKRIGHT, "StopRight", SystemInput_ns::OnRelease, [this]()
			{
				comRigid->acceleration = (0.0f, comRigid->acceleration.y, 0.0f);
				comRigid->velocity = (0.0f, comRigid->velocity.y, 0.0f);
			});


		break;
	}

	case PLAYERSTATE::MOTH:
	{
		comTrans->_scale /= 3.0f;
		//reset camera
		CAMERA_DISTANCE = 100;
		CAMERA_OFFSET_X = 0;
		//enable rigid body for player to move
		comRigid->isStatic = false;
		comRigid->isGravity = false;
		// stop control for player
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk4");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop4");

		//light!
		comLight->_isActive = false;
		break;
	}

	case PLAYERSTATE::POSSESSED:
	{
		comTrans->_scale /= 3.0f;
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk4");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Stop4");

		comRigid->isStatic = true;

		/*TO DO*/ //camera will change angle to possess state
		break;
	}
		
	default:
		break;
	}
}

void Player::setSpawnPoint(ComponentTransform& _spawnPoint)
{
	spawnPoint = _spawnPoint;
}

bool Player::enterPossession()
{
	//check if the player is colliding with any object
	//check any object is possessable
	return true;
}

void Player::OnCollisionEnter(Entity other)
{
	// find script
  ComponentCScript* otherScript = other.getComponent<ComponentCScript>();
  TransformComponent* otherTransComp = other.getComponent<TransformComponent>();
  IScript* tmp = nullptr;
  if(otherScript)
	  tmp = otherScript->_pScript;
	// script not found return
	if (tmp == nullptr) return;
	// check script
	if (typeid(*tmp).hash_code() == typeid(PossessScript).hash_code())
	{
		// confirmed that player has collided with entity with possess script

		//if player is not a moth do nothing
		if (getState() != PLAYERSTATE::MOTH)
			return;


		// get possess script
		//PossessScript* posScr = reinterpret_cast<PossessScript*>(tmp);

		// set camera to use possess script entity's camera
		// get transform tag
		ComponentTransform* comTransform = other.getComponent<ComponentTransform>();
		std::vector<Entity> entityContainer =  G_ECMANAGER->getEntityTagContainer(comTransform->_entityName.toString());
		// find tag
		for (Entity& entity : entityContainer )
		{
			IScript* otherScr = entity.getComponent<ComponentCScript>()->_pScript;
			if (typeid(*otherScr).hash_code() == typeid(AllScripts::CameraScript).hash_code())
			{
				// get camera script
				AllScripts::CameraScript* posCam = reinterpret_cast<AllScripts::CameraScript*>(otherScr);
				// do camera settings
				posCam->SetActive(true);
				posCam->SetRotate(false);
				posCam->SetZoom(false);
				// set cam var
				possessStateCamera = posCam;
			}
			if (typeid(*otherScr).hash_code() == typeid(SpawnPoint).hash_code())
			{
				// get spawn script
				SpawnPoint* spawn = reinterpret_cast<SpawnPoint*>(otherScr);

				// set player spawn position to possess spawn transfrom
				ComponentTransform* myTf = MyID.getComponent<ComponentTransform>();
				// get the transform
				Entity possessSpawnEntity = entity;
				ComponentTransform* possessSpawnTransform = possessSpawnEntity.getComponent< ComponentTransform>();
				// set the data
				myTf->_position = possessSpawnTransform->_position;
				myTf->_rotation = possessSpawnTransform->_rotation;
			}
		}

		// do camera settings on player camera
		camera->SetActive(false);
		camera->SetRotate(false);
		camera->SetZoom(false);

		// change player state
		changeState(PLAYERSTATE::POSSESSED);

		// turn off player collider
		MyID.getComponent<ComponentCollider>()->isCollidable = false;

		// set rigid body to static
		ComponentRigidBody* rgb = MyID.getComponent<ComponentRigidBody>();
		rgb->isStatic = true;
		rgb->velocity = NlMath::Vector3D(0.0f, 0.0f, 0.0f);
		rgb->acceleration = NlMath::Vector3D(0.0f, 0.0f, 0.0f);

	}
  else if(typeid(*tmp).hash_code() == typeid(FanScript).hash_code())
  {
    //if player is not a moth do nothing
    if (getState() != PLAYERSTATE::MOTH)
      return;
    camera->SetTarget(otherTransComp->_position);
    camera->SetDistance(init_CAMERA_DISTANCE);
    //camera->SetRotate(false);
    // turn off collider
    MyID.getComponent<ComponentCollider>()->isCollide = false;
    MyID.getComponent<ComponentCollider>()->isCollidable = false;
    //MyID.getComponent<ComponentGraphics>()->_isActive = false;
    // set rigid body to static
    comRigid->isStatic = true;
    comRigid->isActive = false;
    comRigid->velocity = NlMath::Vector3D(0.0f, 0.0f, 0.0f);
    comRigid->acceleration = NlMath::Vector3D(0.0f, 0.0f, 0.0f);
  }
}
