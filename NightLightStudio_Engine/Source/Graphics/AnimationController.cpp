#include "AnimationController.h"
#include "ModelManager.h"
#include "GraphicsSystem.h"
#include "../Component/ComponentGraphics.h"

void AnimationController::Update(float dt)
{
	if (_play)
	{
		_dt += dt;

		if (_dt >= _endAnimTime || (_dt >= _endFrameTime && _endFrameTime >= 0.0))
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
			}
		}
	}
}

void AnimationController::PlayAnimation(std::string newAnim, ComponentAnimation* currComp, bool loop, double startFrame, double endFrame)
{
	if (_currAnim != newAnim || _play == false)
	{
		if (_allAnims.find(newAnim) != _allAnims.end())
		{
			_currAnim = newAnim;
			_dt = 0.0;
			_play = true;
			_isPlaying = true;
			_loop = loop;
			_startFrameTime = startFrame;
			_endFrameTime = endFrame;

			Entity entity = G_ECMANAGER->getEntity(currComp);
			ComponentGraphics* compGraphic = entity.getComponent<ComponentGraphics>();

			if (compGraphic)
			{
				_endAnimTime = NS_GRAPHICS::ModelManager::GetInstance()._models[compGraphic->_modelID]->_animations[newAnim]->_time;
			}
		}
	}
}
