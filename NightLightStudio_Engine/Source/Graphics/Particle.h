#pragma once
#include "..\glm\glm.hpp"

struct Particle
{
	glm::vec3 _position;
	glm::vec3 _velocity;
	glm::vec4 _colourStart;
	glm::vec4 _colourEnd;
	glm::vec4 _currColour;
	float _timeAlive;
	float _lifespan;
	float _size;
	float _speed;
	float _cameraDistance;
	float _animationTime;
	unsigned _currentFrame;
	bool _alive;

	Particle();
	~Particle() = default;

	bool operator<(const Particle& rhs) const;
};
