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
	double _endFrameTime;

	bool _play;
	bool _loop;
	bool _isPlaying;

	double _dt;
	double _endAnimTime;

	void Update(float dt);
	void PlayAnimation(std::string newAnim, ComponentAnimation* currComp, bool loop = true, double startFrame = -1, double endFrame = -1);
};