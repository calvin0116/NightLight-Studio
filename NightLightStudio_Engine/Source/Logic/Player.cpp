#pragma once
#include "Player.h"
#include "../Component/ComponentCScript.h"

#define PLAYER_MOVE_MAG 1000.0f

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
	:comCol(nullptr), comRigid(nullptr), comTrans(nullptr), _playerState(PLAYERSTATE::HUMAN), _prevPlayerState(PLAYERSTATE::HUMAN), _front(0)
{
	spawnPoint._rotation = { 0,0,0 };
	spawnPoint._position = { 0,0,0 };
	spawnPoint._scale = { 1,1,1 };
}

void Player::Init()
{
	playerEntity = MyID;
	comCol = MyID.getComponent<ColliderComponent>();
	comRigid = MyID.getComponent<RigidBodyComponent>();
	comTrans = MyID.getComponent<TransformComponent>();
	//initially the player is a human with human controls
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk1", WALKFRONT, "WalkFront", SystemInput_ns::OnHold, [this]()
		{
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector(), PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk2", WALKLEFT, "WalkLeft", SystemInput_ns::OnHold, [this]()
		{
			
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3( -currentCameraDirection.x, 0,0), PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk3", WALKBACK, "WalkBack", SystemInput_ns::OnHold, [this]()
		{
			//negative direction
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(-currentCameraDirection.x, 0, -currentCameraDirection.z), PLAYER_MOVE_MAG);
		});
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk4", WALKRIGHT, "WalkRight", SystemInput_ns::OnHold, [this]()
		{
			NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(currentCameraDirection.x, 0, 0), PLAYER_MOVE_MAG);
		});
	//state change control
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Become Butterfly", POSSESS, "butterfly", SystemInput_ns::OnPress, [this]()
		{
			if (_playerState == PLAYERSTATE::HUMAN)
			{
				changeState(PLAYERSTATE::BUTTERFLY);
			}
			if (_playerState == PLAYERSTATE::BUTTERFLY)
			{
				changeState(PLAYERSTATE::HUMAN);
			}
			if (_playerState == PLAYERSTATE::POSSESSED)
			{
				changeState(PLAYERSTATE::BUTTERFLY);
			}
			
		});
}

void Player::Update()
{
	switch (_playerState)
	{
	case PLAYERSTATE::HUMAN:
	{
		/*TO DO*/ // take camera position as front
		_front = NS_GRAPHICS::CameraSystem::GetInstance().GetXZViewVector();

		break;
	}
		
	case PLAYERSTATE::BUTTERFLY:
	{
		_front = currentCameraDirection;
		
		break;
	}
		
	case PLAYERSTATE::POSSESSED:
	{
		//player will change to possessed state if it collides with the possessed
	}
		break;
	default:
		break;
	}
}

void Player::Exit()
{
}

PLAYERSTATE Player::getState()
{
	return _playerState;
}

void Player::changeState(PLAYERSTATE state)
{
	_prevPlayerState = _playerState;
	_playerState = state;
	switch (_playerState)
	{
	case PLAYERSTATE::HUMAN:
	{
		if (_prevPlayerState == PLAYERSTATE::POSSESSED)
		{
			/*TO DO*/ 
			//set player's transform to spawn point transform
			
		}
		//player is a human with human controls
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk1", WALKFRONT, "WalkFront", SystemInput_ns::OnHold, [this]()
			{
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(currentCameraDirection.x, 0, currentCameraDirection.z), PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk2", WALKLEFT, "WalkLeft", SystemInput_ns::OnHold, [this]()
			{
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(-currentCameraDirection.x, 0, 0), PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk3", WALKBACK, "WalkBack", SystemInput_ns::OnHold, [this]()
			{
				//negative direction
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(-currentCameraDirection.x, 0, -currentCameraDirection.z), PLAYER_MOVE_MAG);
			});
		SYS_INPUT->GetSystemKeyPress().CreateNewEvent("Walk4", WALKRIGHT, "WalkRight", SystemInput_ns::OnHold, [this]()
			{
				NS_PHYSICS::USE_THE_FORCE.addForce(playerEntity, NlMath::Vec3(currentCameraDirection.x, 0, 0), PLAYER_MOVE_MAG);
			});
		break;
	}

	case PLAYERSTATE::BUTTERFLY:
	{
		// player
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk4");
		break;
	}

	case PLAYERSTATE::POSSESSED:
	{
		
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk1");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk2");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk3");
		SYS_INPUT->GetSystemKeyPress().RemoveEvent("Walk4");

		//camera
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
	//ComponentCScript* tmp = other.getComponent<ComponentCScript>();
	//if (tmp != nullptr)
	//{
	//	tmp->_iTag == 101;
	//	tmp->_iTag == 102;
	//	tmp->_iTag == 103;
	//	tmp->_iTag == 104;
	//}
}
