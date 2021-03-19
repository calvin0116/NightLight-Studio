#include "SystemEmitter.h"
#include "../glm/gtc/matrix_transform.hpp" // glm::ortho
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu
#include "../glm/gtx/norm.hpp"

#include "../Component/Components.h" // Access to ECManager
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentCanvas.h"

#include "../Input/SystemInput.h"

#define MAX_PARTICLE 2000

NS_GRAPHICS::EmitterSystem::EmitterSystem() :
	_particleDrawing{ true },
	_isPlaying{ false },
	_paused{ false },
	_textureManager{ nullptr },
	_shaderSystem{ nullptr },
	_cameraSystem{ nullptr }
{
}

NS_GRAPHICS::EmitterSystem::~EmitterSystem()
{
	//clean up
	for (auto& emitter : _emitters)
	{
		delete emitter;
	}
}

void NS_GRAPHICS::EmitterSystem::Init()
{
	//Seed the rng
	srand(static_cast <unsigned> (time(0)));

	_textureManager = &TextureManager::GetInstance();
	_shaderSystem = &ShaderSystem::GetInstance();
	_cameraSystem = &CameraSystem::GetInstance();

	//Set up event to hide particle drawing
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TOGGLE_PARTICLE_DRAW", SystemInput_ns::IKEY_0, "TOGGLE_PARTICLE_DRAW", SystemInput_ns::OnPress, [this]()
	{
		if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_CTRL))
		{
			_particleDrawing = !_particleDrawing;
		}
	});
}

void NS_GRAPHICS::EmitterSystem::Update()
{
	//Draw only if particle is enabled
	if (_particleDrawing || _isPlaying)
	{
		glDepthMask(GL_FALSE);
		_shaderSystem->StartProgram(ShaderSystem::PARTICLE);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		auto itr = G_ECMANAGER->begin<ComponentEmitter>();
		auto itrEnd = G_ECMANAGER->end<ComponentEmitter>();
		while (itr != itrEnd)
		{
			ComponentEmitter* emitter = G_ECMANAGER->getComponent<ComponentEmitter>(itr);

			if (!emitter->_isActive)
			{
				++itr;
				continue;
			}

			if(!_paused)
				UpdateEmitter(emitter, DELTA_T->real_dt);
			Render(emitter);

			itr++;
		}

		//glDisable(GL_BLEND);

		_shaderSystem->StopProgram();
		glDepthMask(GL_TRUE);
	}
}

void NS_GRAPHICS::EmitterSystem::UpdateEmitter(ComponentEmitter* emitter, float dt)
{
	if (_emitters[emitter->_emitterID]->_pause)
	{
		return;
	}
	if (_emitters[emitter->_emitterID]->_play)
	{
		//Update the emitter time
		_emitters[emitter->_emitterID]->AddTime(dt);

		//Get this for position, scale and rotation
		Entity entity = G_ECMANAGER->getEntity(emitter);
		ComponentTransform* transform = entity.getComponent<ComponentTransform>();

		_emitters[emitter->_emitterID]->_emitterPosition = transform->_position;
		_emitters[emitter->_emitterID]->_emitterRotation = transform->_rotation;
		_emitters[emitter->_emitterID]->_emitterScale = transform->_scale;

		//Loop and update all particles that are alive
		size_t particleSize = _emitters[emitter->_emitterID]->_maxParticles;
		for (size_t i = 0; i < particleSize; ++i)
		{
			//Don't update if its dead
			if (!_emitters[emitter->_emitterID]->_particles[i]._alive)
			{
				continue;
			}

			glm::vec3 vel = _emitters[emitter->_emitterID]->_particles[i]._velocity;
			float aliveRatio = _emitters[emitter->_emitterID]->_particles[i]._timeAlive / _emitters[emitter->_emitterID]->_particles[i]._lifespan;

			//Speed increase the longer it is alive
			if (_emitters[emitter->_emitterID]->_speedOverTime && !_emitters[emitter->_emitterID]->_velocityOverTime)
			{
				_emitters[emitter->_emitterID]->_particles[i]._position += vel * _emitters[emitter->_emitterID]->_particles[i]._speed * aliveRatio * dt;
			}
			//Gives velocity the longer it is alive
			else if (!_emitters[emitter->_emitterID]->_speedOverTime && _emitters[emitter->_emitterID]->_velocityOverTime)
			{
				vel += aliveRatio * _emitters[emitter->_emitterID]->_velocity;
				_emitters[emitter->_emitterID]->_particles[i]._position += vel * _emitters[emitter->_emitterID]->_particles[i]._speed * dt;
			}
			//Both
			else if (_emitters[emitter->_emitterID]->_speedOverTime && _emitters[emitter->_emitterID]->_velocityOverTime)
			{
				vel += aliveRatio * _emitters[emitter->_emitterID]->_velocity;
				_emitters[emitter->_emitterID]->_particles[i]._position += vel * _emitters[emitter->_emitterID]->_particles[i]._speed * aliveRatio * dt;
			}
			else
			{
				_emitters[emitter->_emitterID]->_particles[i]._position += vel * _emitters[emitter->_emitterID]->_particles[i]._speed * dt;
			}

			_emitters[emitter->_emitterID]->_particles[i]._timeAlive += dt;

			//Only if is animated
			if (_emitters[emitter->_emitterID]->_isAnimated)
			{
				_emitters[emitter->_emitterID]->_particles[i]._animationTime += dt;

				//Animation rate depends on fps
				if (_emitters[emitter->_emitterID]->_particles[i]._animationTime >= _emitters[emitter->_emitterID]->_animationRate)
				{
					_emitters[emitter->_emitterID]->_particles[i]._animationTime = 0.0f;

					++_emitters[emitter->_emitterID]->_particles[i]._currentFrame;

					//Checks if incremented frame is already out of frame
					if (_emitters[emitter->_emitterID]->_particles[i]._currentFrame >= _emitters[emitter->_emitterID]->_totalFrame)
					{
						if (_emitters[emitter->_emitterID]->_loopAnimation)
						{
							_emitters[emitter->_emitterID]->_particles[i]._currentFrame = 0;
						}
						else
						{
							//Stays at last frame
							--_emitters[emitter->_emitterID]->_particles[i]._currentFrame;
						}
					}
				}
			}

			//Particle should be dead
			if (_emitters[emitter->_emitterID]->_particles[i]._timeAlive >= _emitters[emitter->_emitterID]->_particles[i]._lifespan)
			{
				_emitters[emitter->_emitterID]->_numAlive--;
				_emitters[emitter->_emitterID]->_particles[i]._alive = false;
			}
		}

		//Update emitter
		if (_emitters[emitter->_emitterID]->_timePassed > _emitters[emitter->_emitterID]->_durationPerCycle && !_emitters[emitter->_emitterID]->_loop)
		{
			//Don't Spawn Anymore
			if (_emitters[emitter->_emitterID]->_numAlive == 0)
			{
				_emitters[emitter->_emitterID]->Stop();
			}
		}
		//Duration still within time passed or it is looping
		else
		{
			if (_emitters[emitter->_emitterID]->_emissionOverTime > 0)
			{
				if (_emitters[emitter->_emitterID]->_emitterTime >= _emitters[emitter->_emitterID]->_emissionRate)
				{
					_emitters[emitter->_emitterID]->_emitterTime = 0.0f;
					_emitters[emitter->_emitterID]->RespawnParticle();
				}
			}

			if (_emitters[emitter->_emitterID]->_burst)
			{
				if (_emitters[emitter->_emitterID]->_burstTime >= _emitters[emitter->_emitterID]->_burstRate)
				{
					_emitters[emitter->_emitterID]->_burstTime = 0.0f;
					_emitters[emitter->_emitterID]->InitParticles(_emitters[emitter->_emitterID]->_burstAmount);
				}
			}
		}
	}
}

void NS_GRAPHICS::EmitterSystem::Render(ComponentEmitter* emitter)
{
	if (!_emitters[emitter->_emitterID]->_play)
	{
		return;
	}
	//glm::mat4 viewMat = _cameraSystem->GetViewMatrix();
	//glm::vec3 camPos = glm::inverse(viewMat)[3];
	glm::vec3 camPos = CameraSystem::GetInstance().GetCamera().cameraPos;
	glm::vec3 camRight = CameraSystem::GetInstance().GetCamera().cameraRight;
	glm::vec3 camUp = CameraSystem::GetInstance().GetCamera().cameraUp;
	glm::vec3 camFront = CameraSystem::GetInstance().GetCamera().cameraFront;
	//Fills the Colour and Particle postion buffer
	int toDraw = 0;
	unsigned maxParticle = _emitters[emitter->_emitterID]->_maxParticles;
	for (unsigned i = 0; i < maxParticle; i++) 
	{
		if (_emitters[emitter->_emitterID]->_particles[i]._alive) 
		{
			glm::vec3 pos;
			float scale = 1.0f;

			float aliveRatio = _emitters[emitter->_emitterID]->_particles[i]._timeAlive / _emitters[emitter->_emitterID]->_particles[i]._lifespan;
			_emitters[emitter->_emitterID]->_particles[i]._cameraDistance = glm::length2(_emitters[emitter->_emitterID]->_particles[i]._position - camPos);
			//glm::vec3 particleVec = _emitters[emitter->_emitterID]->_particles[i]._position - camPos;
			//float dotProduct = camFront.x * particleVec.x + camFront.y * particleVec.y + camFront.z * particleVec.z;

			//if (dotProduct <= 0.0f)
			//{
			//	_emitters[emitter->_emitterID]->_particles[i]._cameraDistance = -1.0f;
			//}

			//_emitters[emitter->_emitterID]->_particles[i]._cameraDistance = dotProduct;

			if (_emitters[emitter->_emitterID]->_follow)
			{
				pos = _emitters[emitter->_emitterID]->_particles[i]._position + _emitters[emitter->_emitterID]->_emitterPosition;
			}
			else
			{
				pos = _emitters[emitter->_emitterID]->_particles[i]._position;
			}
			if (_emitters[emitter->_emitterID]->_sizeOverTime)
			{
				scale = aliveRatio * _emitters[emitter->_emitterID]->_particles[i]._size;
			}
			else
			{
				scale = _emitters[emitter->_emitterID]->_particles[i]._size;
			}

			glm::vec4 currColor;
			if (_emitters[emitter->_emitterID]->_colourOverTime)
			{
				currColor = ((1.0f - aliveRatio) * _emitters[emitter->_emitterID]->_particles[i]._colourStart) + ((aliveRatio)*_emitters[emitter->_emitterID]->_particles[i]._colourEnd);
			}
			else
			{
				currColor = _emitters[emitter->_emitterID]->_particles[i]._colourEnd;
			}

			if (_emitters[emitter->_emitterID]->_fade)
			{
				currColor.a *= (1.0f - aliveRatio);
			}

			// Fill the GPU buffer
			_emitters[emitter->_emitterID]->_particlesPosition[toDraw].x = pos.x;
			_emitters[emitter->_emitterID]->_particlesPosition[toDraw].y = pos.y;
			_emitters[emitter->_emitterID]->_particlesPosition[toDraw].z = pos.z;
			_emitters[emitter->_emitterID]->_particlesPosition[toDraw].w = scale;

			_emitters[emitter->_emitterID]->_particlesColour[toDraw] = currColor;
			_emitters[emitter->_emitterID]->_particles[i]._currColour = currColor;

			if (_emitters[emitter->_emitterID]->_isAnimated)
			{
				_emitters[emitter->_emitterID]->_particleFrame[toDraw] = _emitters[emitter->_emitterID]->_particles[i]._currentFrame;
			}
			else
			{
				_emitters[emitter->_emitterID]->_particleFrame[toDraw] = 0;
			}

			toDraw++;
		}
		else
		{
			_emitters[emitter->_emitterID]->_particles[i]._cameraDistance = -1.0f;
		}
	}

	//Sort by cam distance
	_emitters[emitter->_emitterID]->SortParticle();

	glBindVertexArray(_emitters[emitter->_emitterID]->_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _emitters[emitter->_emitterID]->_posBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * toDraw, &_emitters[emitter->_emitterID]->_particlesPosition[0]);

	glBindBuffer(GL_ARRAY_BUFFER, _emitters[emitter->_emitterID]->_colBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * toDraw, &_emitters[emitter->_emitterID]->_particlesColour[0]);

	glBindBuffer(GL_ARRAY_BUFFER, _emitters[emitter->_emitterID]->_animationBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(unsigned) * toDraw, &_emitters[emitter->_emitterID]->_particleFrame[0]);

	//glm::vec3 camRight = glm::vec3(viewMat[0][0], viewMat[1][0], viewMat[2][0]);
	//glm::vec3 camUp = glm::vec3(viewMat[0][1], viewMat[1][1], viewMat[2][1]);
	glUniform3fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "camera_right"), 1, &camRight[0]);
	glUniform3fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "camera_up"), 1, &camUp[0]);

	if (_emitters[emitter->_emitterID]->_isAnimated)
	{
		glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxRow"), _emitters[emitter->_emitterID]->_row);
		glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxColumn"), _emitters[emitter->_emitterID]->_column);
	}
	else
	{
		glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxRow"), 1);
		glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxColumn"), 1);
	}

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_textureManager->BindAlbedoTexture(emitter->_imageID);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, toDraw);
}

unsigned NS_GRAPHICS::EmitterSystem::LoadTexture(std::string filename, bool blackAlpha)
{
	return _textureManager->GetTexture(filename, false, blackAlpha);
}

unsigned NS_GRAPHICS::EmitterSystem::AddEmitter()
{
	size_t index = GetFreeIndex();

	//Full time to resize
	if (index >= _emitters.size())
	{
		Emitter* newEmitter = new Emitter();
		_emitters.push_back(newEmitter);
		newEmitter->InitBuffer();
		_usedStatus.push_back(true);
	}
	else
	{
		_emitters[index] = new Emitter();
		_emitters[index]->InitBuffer();
		_usedStatus[index] = true;
	}

	return (unsigned)index;
}

bool NS_GRAPHICS::EmitterSystem::RemoveEmitterByID(const int& index)
{
	if (_emitters[index] == nullptr || index >= _emitters.size())
	{
		return false;
	}

	delete _emitters[index];
	_emitters[index] = nullptr;
	_usedStatus[index] = false;

	return true;
}

size_t NS_GRAPHICS::EmitterSystem::GetFreeIndex()
{
	return std::distance(_usedStatus.begin(), std::find(_usedStatus.begin(), _usedStatus.end(), false));
}
