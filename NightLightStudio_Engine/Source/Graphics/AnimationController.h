#pragma once
#include <string>
#include <set>
#include "../Component/ComponentAnimation.h"

struct Model;

struct AnimationController
{
	std::string _currAnim;
	std::string _defaultAnim;

	std::set<std::string> _allAnims;

	double _startFrameTime;
	float _animMultiplier = 1.0f;

	bool _play;
	bool _loop;

	double _dt;
	double _endAnimTime;

	void Update(float dt);
	void PlayAnimation(const std::string& newAnim, ComponentAnimation* currComp, bool loop = true, double startFrame = -1.0, double endFrame = -1.0);
};