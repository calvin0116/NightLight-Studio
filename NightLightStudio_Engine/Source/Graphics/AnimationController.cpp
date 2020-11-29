#include "AnimationController.h"
#include "ModelManager.h"
#include "GraphicsSystem.h"
#include "../Component/ComponentGraphics.h"

void AnimationController::Update(float dt)
{
	if (_play)
	{
		_dt += dt;

		if (_dt >= _endAnimTime)
		{
			if (_loop)
			{
				_dt -= _endAnimTime;
			}
			else
			{
				_play = false;
			}
		}
	}
}

void AnimationController::PlayAnimation(std::string newAnim, ComponentAnimation* currComp, bool loop)
{
	if (_currAnim != newAnim || _play == false)
	{
		if (_allAnims.find(newAnim) != _allAnims.end())
		{
			_currAnim = newAnim;
			_dt = 0;
			_play = true;
			_loop = loop;

			Entity entity = G_ECMANAGER->getEntity(currComp);
			ComponentGraphics* compGraphic = entity.getComponent<ComponentGraphics>();

			if (compGraphic)
			{
				_endAnimTime = NS_GRAPHICS::ModelManager::GetInstance()._models[compGraphic->_modelID]->_animations[newAnim]->_time;
			}
		}
	}
}
