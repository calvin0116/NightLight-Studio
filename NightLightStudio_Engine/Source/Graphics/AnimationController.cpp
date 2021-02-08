#include "AnimationController.h"
#include "ModelManager.h"
#include "GraphicsSystem.h"
#include "../Component/ComponentGraphics.h"

void AnimationController::Update(float dt)
{
	if (_play)
	{
		_dt += dt * _animMultiplier;

		if (_dt >= _endAnimTime)
		{
			if (_loop)
			{
				if (_startFrameTime >= 0)
				{
					_dt = _startFrameTime;
				}
				else
				{
					_dt = 0.0;
				}
			}
			else
			{
				_play = false;
				_currAnim = "";
			}
		}
	}
}

void AnimationController::PlayAnimation(const std::string& newAnim, ComponentAnimation* currComp, bool loop, double startFrame, double endFrame)
{
	if (_currAnim != newAnim || _play == false)
	{
		if (_allAnims.find(newAnim) != _allAnims.end())
		{
			_currAnim = newAnim;
			_dt = 0.0;
			_play = true;
			_loop = loop;
			_startFrameTime = startFrame;
			if (_startFrameTime >= 0.0)
			{
				_dt = startFrame;
			}

			Entity entity = G_ECMANAGER->getEntity(currComp);
			ComponentGraphics* compGraphic = entity.getComponent<ComponentGraphics>();

			if (compGraphic)
			{
				if (endFrame >= 0.0)
				{
					_endAnimTime = endFrame;
				}
				else
				{
					_endAnimTime = NS_GRAPHICS::ModelManager::GetInstance()._models[compGraphic->_modelID]->_animations[newAnim]->_time;
				}
			}
		}
	}
}
