#pragma once
#include "..\glm\common.hpp"
#include "..\glm\gtc\constants.hpp"
#include <vector>
#include "Particle.h"

enum EmitterShapeType
{
	SPHERE, //ONLY THIS FOR NOW
	CONE,
	TOTAL_SHAPE
};

constexpr auto PI = 3.14159265358979323846;

class Emitter
{
public:
	std::vector<Particle> _particles;
	int _numAlive;
	unsigned _lastUsed;
	float _emitterTime;
	float _timePassed;

	//Emitter transform variables
	glm::vec3 _emitterPosition;
	glm::vec3 _emitterRotation;
	glm::vec3 _emitterScale;

	EmitterShapeType _type;
	float _durationPerCycle;
	float _emissionRate;
	unsigned _maxParticles;

	float _burstTime;
	float _burstRate;
	unsigned _burstAmount;

	float _spawnAngle;
	float _radius;

	bool _randomizeSize;
	float _minParticleSize;
	float _maxParticleSize;

	bool _randomizeSpeed;
	float _minParticleSpeed;
	float _maxParticleSpeed;

	bool _randomizeLifespan;
	float _minLifespan;
	float _maxLifespan;

	bool _play;
	bool _preWarm;
	bool _burst;
	bool _loop;
	bool _reverse;
	bool _follow;
	bool _fade;

	bool _velocityOverTime;
	bool _sizeOverTime;
	bool _speedOverTime;
	bool _colourOverTime;

	bool _randomizeColour;
	glm::vec4 _colourA;
	glm::vec4 _colourB;

	glm::vec4 _colourStart;
	glm::vec4 _colourEnd;

	glm::vec3 _velocity;

	unsigned _vao;
	unsigned _vbo;
	unsigned _ebo;
	unsigned _posBuffer;
	unsigned _colBuffer;

	std::vector<glm::vec4> _particlesPosition;
	std::vector<glm::vec4> _particlesColour;

	Emitter();
	~Emitter();

	//Helper functions for emitter to work
	void AddTime(float dt);
	float RandFloat(); // Between 0.0f to 1.0f
	void InitAllParticles(bool prewarm = false);
	void InitParticles(unsigned amount);
	void PreWarmParticles();
	void ResetParticle(size_t index);
	void RespawnParticle();
	unsigned FindUnused();
	

	// Generates a random unit vector3
	glm::vec3 RandomVec3();

	//Prepares buffer
	void InitBuffer();
	//Updates the buffer due to size change
	void UpdateBuffer();
	//Sort Particle Container
	void SortParticle();
	//Update size changes to containers
	void UpdateSize();
	//Update transform stuff
	void UpdateTransform(glm::vec3 pos = glm::vec3(), glm::vec3 rotate = glm::vec3(), glm::vec3 scale = glm::vec3(1.0f, 1.0f,1.0f));

	void Play();
	void Stop();
	
};