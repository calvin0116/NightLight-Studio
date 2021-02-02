#include "Emitter.h"
#include <math.h>
#include "../glm/gtc/matrix_transform.hpp" // glm::ortho
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu
#include "../glm/glm.hpp"   // glm::mat4
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../Window/WndSystem.h"
#include "../Window/WndUtils.h"
#pragma warning( disable : 26812 )

float Emitter::RandFloat()
{
	//Always between 0 and 1.0f
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

Emitter::Emitter():
	_particles{}, _numAlive{ 0 }, _lastUsed{ 0 }, _emitterTime{ 0.0f }, _timePassed{ 0.0f }, _emitterPosition{}, _emitterRotation{},
	_type{ EmitterShapeType::SPHERE }, _durationPerCycle{ 5.0f }, _emissionRate{ 1.0f }, _maxParticles{ 100 }, //Main particle data
	_spawnAngle{ 30.0f }, _radius{ 100.0f }, //For cone and circle
	_randomizeSize{ false }, _minParticleSize{ 10 }, _maxParticleSize{ 100 }, //Particle Size
	_randomizeSpeed{ false }, _minParticleSpeed{ 10 }, _maxParticleSpeed{ 100 }, //Particle Speed
	_randomizeLifespan{ false }, _minLifespan{ 1.0f }, _maxLifespan{ 5.0f }, //Particle Lifespan
	_play{ false }, _preWarm{ false }, _burst{ false }, _loop{ false }, _reverse{ false }, _follow{ false }, _fade{ false },//System Booleans
	_velocityOverTime{ false }, _sizeOverTime{ false }, _speedOverTime{ false }, _colourOverTime{ false },  //Type Booleans
	_randomizeColour{ false }, _colourA{ 1.0f,1.0f,1.0f,1.0f }, _colourB{ 1.0f,1.0f,1.0f,1.0f },
	_colourStart{ 1.0f,1.0f,1.0f,1.0f }, _colourEnd{ 1.0f,1.0f,1.0f,1.0f }, _velocity{ 0.0f,0.0f,0.0f }, //Data if boolean is true
	_vao{ 0 }, _vbo{ 0 }, _ebo{ 0 }, _posBuffer{ 0 }, _colBuffer{ 0 }, _particlesPosition{}, _particlesColour{}
{
	_particles.resize(_maxParticles);
	_particlesPosition.resize(_maxParticles);
	_particlesColour.resize(_maxParticles);
}

Emitter::~Emitter()
{
}

void Emitter::AddTime(float dt)
{
	_emitterTime += dt;
	_timePassed += dt;
}

void Emitter::InitParticles()
{
	if (_numAlive == _maxParticles)
	{
		return;
	}

	switch (_type)
	{
		case SPHERE:
		{
			for (size_t i = 0; i < _maxParticles; ++i)
			{
				//Randomize spawning location within sprite boundary
				glm::vec3 spawnPos = glm::vec3(0.0f);
				if (!_follow)
				{
					spawnPos = _emitterPosition;
				}
				glm::vec3 randVec = RandomVec3() * _emitterScale * _radius;
				spawnPos += randVec;
				_particles[i]._position = spawnPos;

				//Unit Length Direction
				_particles[i]._velocity = glm::normalize(randVec);

				//Randomize speed
				if (_randomizeSpeed)
				{
					_particles[i]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
				}
				else
				{
					_particles[i]._speed = _maxParticleSpeed;
				}

				//Randomize lifespan
				if (_randomizeLifespan)
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
				}
				else
				{
					_particles[i]._timeAlive = 0.0f;
					_particles[i]._lifespan = _maxLifespan;
				}

				if (_colourOverTime)
				{
					//Set starting and ending colour
					_particles[i]._colourStart = _colourStart;
					_particles[i]._colourEnd = _colourEnd;
				}
				else if (_randomizeColour)
				{
					//Either A or B
					_particles[i]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
				}
				else
				{
					_particles[i]._colourEnd = _colourB;
				}

				//Randomize size
				if (_randomizeSize)
				{
					_particles[i]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
				}
				else
				{
					_particles[i]._size = _maxParticleSize;
				}

				//Remember to set it alive
				_particles[i]._alive = true;

				//Increase alive count
				++_numAlive;
			}
		}
	}
}

void Emitter::ResetParticle(size_t index)
{
	if (_numAlive == _maxParticles)
	{
		return;
	}

	switch (_type)
	{
		case SPHERE:
		{
			//Randomize spawning location within sprite boundary
			glm::vec3 spawnPos = glm::vec3(0.0f);
			if (!_follow)
			{
				spawnPos = _emitterPosition;
			}
			glm::vec3 randVec = RandomVec3() * _emitterScale * _radius;
			spawnPos += randVec;
			_particles[index]._position = spawnPos;

			//Unit Length Direction
			_particles[index]._velocity = glm::normalize(randVec);

			//Randomize speed
			if (_randomizeSpeed)
			{
				_particles[index]._speed = (RandFloat() * (_maxParticleSpeed - _minParticleSpeed)) + _minParticleSpeed;
			}
			else
			{
				_particles[index]._speed = _maxParticleSpeed;
			}

			//Randomize lifespan
			if (_randomizeLifespan)
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = ((_maxLifespan - _minLifespan) * RandFloat()) + _minLifespan;
			}
			else
			{
				_particles[index]._timeAlive = 0.0f;
				_particles[index]._lifespan = _maxLifespan;
			}

			if (_colourOverTime)
			{
				//Set starting and ending colour
				_particles[index]._colourStart = _colourStart;
				_particles[index]._colourEnd = _colourEnd;
			}
			else if (_randomizeColour)
			{
				//Either A or B
				_particles[index]._colourEnd = RandFloat() > 0.5f ? _colourB : _colourA;
			}
			else
			{
				_particles[index]._colourEnd = _colourB;
			}

			//Randomize size
			if (_randomizeSize)
			{
				_particles[index]._size = ((_maxParticleSize - _minParticleSize) * RandFloat()) + _minParticleSize;
			}
			else
			{
				_particles[index]._size = _maxParticleSize;
			}

			//Remember to set it alive
			_particles[index]._alive = true;

			//Increase alive count
			++_numAlive;
		}
	}
}

void Emitter::RespawnParticle()
{
	ResetParticle(FindUnused());
}

unsigned Emitter::FindUnused()
{
	for (unsigned i = _lastUsed; i < _maxParticles; ++i)
	{
		if (_particles[i]._alive)
		{
			_lastUsed = i;
			return i;
		}
	}

	for (unsigned i = 0; i < _lastUsed; ++i)
	{
		if (_particles[i]._alive)
		{
			_lastUsed = i;
			return i;
		}
	}
}

glm::vec3 Emitter::RandomVec3()
{
	//Discard method
	//float x, y, z;
	//float d = 10.0f;
	//
	//while (d > 1.0f)
	//{
	//	x = RandFloat() * 2 - 1.0f;
	//	y = RandFloat() * 2 - 1.0f;
	//	z = RandFloat() * 2 - 1.0f;
	//	d = x * x + y * y + z * z;
	//} 

	//Polar Coordinate Method
	float theta = RandFloat() * 2 * PI;
	float v = RandFloat();
	float phi = acos((2 * v) - 1);
	float r = pow(RandFloat(), 0.33333333333f);
	float x = r * sin(phi) * cos(theta);
	float y = r * sin(phi) * sin(theta);
	float z = r * cos(phi);

	return glm::vec3(x, y, z);
	//return glm::vec3();
}

void Emitter::InitBuffer()
{
	//Init sprite here
	const unsigned numOfSides = 4;

	glm::vec3 particleVertice[numOfSides] = {
		//Top Right
		{
			glm::vec3(0.5f,	0.5f, 0.0f)
		},
		//Top Left
		{
			glm::vec3(-0.5f, 0.5f, 0.0f)
		},
		//Bottom Left
		{
			glm::vec3(-0.5f, -0.5f, 0.0f)
		},
		//Bottom Right
		{
			glm::vec3(0.5f, -0.5f, 0.0f)
		}
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		2, 3, 0  // second triangle
	};

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenBuffers(1, &_posBuffer);
	glGenBuffers(1, &_colBuffer);

	//Fixed Won't change
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numOfSides, &particleVertice[0], GL_STATIC_DRAW);

	//Buffer orphan
	glBindBuffer(GL_ARRAY_BUFFER, _posBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _colBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _maxParticles, NULL, GL_DYNAMIC_DRAW);

	// original square polygon 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	// centre positions and size of all particle
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	// colour
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Emitter::UpdateBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, _posBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _colBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _maxParticles, NULL, GL_DYNAMIC_DRAW);
}

void Emitter::SortParticle()
{
	std::sort(_particles.begin(), _particles.end());
}

void Emitter::UpdateSize()
{
	_particles.resize(_maxParticles);
	_particlesPosition.resize(_maxParticles);
	_particlesColour.resize(_maxParticles);

	UpdateBuffer();
}

void Emitter::Play()
{
	if (_play)
	{
		return;
	}

	_play = true;
	_emitterTime = 0.0f;
	_timePassed = 0.0f;

	if (_preWarm)
	{
		InitParticles();
	}
}

void Emitter::Stop()
{
	_play = false;
	_emitterTime = 0.0f;
	_timePassed = 0.0f;
	_numAlive = 0;

	//Kills all particles
	for (size_t i = 0; i < _maxParticles; ++i)
	{
		_particles[i]._lifespan = 0.0f;
	}
}
