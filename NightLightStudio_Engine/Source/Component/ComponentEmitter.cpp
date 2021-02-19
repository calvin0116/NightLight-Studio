#include "ComponentEmitter.h"
#include "../Graphics/SystemEmitter.h"
#include "../Graphics/TextureManager.h"
#include "Components.h"
#include "ComponentManager.h"
#pragma warning( disable : 26812 )

ComponentEmitter::ComponentEmitter()
	: _isActive{ true }, _emitterID{ -1 }
{
	strcpy_s(ser_name, "EmitterComponent");
}

ComponentEmitter::ComponentEmitter(const int& emitterID)
	: _isActive{ true }
	, _emitterID{ emitterID }
{
	strcpy_s(ser_name, "EmitterComponent");
}

void ComponentEmitter::SetEmitterID(const int& emitterID)
{
	_emitterID = emitterID;
}

int ComponentEmitter::GetEmitterID()
{
	return _emitterID;
}

void ComponentEmitter::AddTexture(std::string filename)
{
	if (!filename.empty() && _image.toString() != filename)
	{
		_image = filename;
		_imageID = NS_GRAPHICS::EmitterSystem::GetInstance().LoadTexture(_image.toString());
	}
}

void ComponentEmitter::Play()
{
	Entity entity = G_ECMANAGER->getEntity(this);
	ComponentTransform* trans = entity.getComponent<ComponentTransform>();
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->UpdateTransform(trans->_position, trans->_rotation, trans->_scale);
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->Play();
}

void ComponentEmitter::Stop()
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->Stop();
}

EmitterShapeType ComponentEmitter::GetShapeType()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_type;
}

void ComponentEmitter::SetShapeType(EmitterShapeType shape)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_type = shape;
}

float ComponentEmitter::GetDuration()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_durationPerCycle;
}

void ComponentEmitter::SetDuration(float duration)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_durationPerCycle = duration;
}

unsigned ComponentEmitter::GetEmissionRate()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_emissionOverTime;
}

void ComponentEmitter::SetEmissionRate(unsigned emission)
{
	if (emission <= 0)
	{
		NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_emissionOverTime = 0;
		NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_emissionRate = 0.0f;
	}
	else
	{
		NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_emissionOverTime = emission;
		NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_emissionRate = 1.0f / NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_emissionOverTime;
	}	
}

unsigned ComponentEmitter::GetMaxParticle()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxParticles;
}

void ComponentEmitter::SetMaxParticle(unsigned max)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxParticles = max;
}

float ComponentEmitter::GetBurstRate()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_burstRate;
}

void ComponentEmitter::SetBurstRate(float burst)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_burstRate = burst;
}

unsigned ComponentEmitter::GetBurstAmount()
{
	return 	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_burstAmount;
}

void ComponentEmitter::SetBurstAmount(unsigned amount)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_burstAmount = amount;
}

float ComponentEmitter::GetAngle()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_spawnAngle;
}

void ComponentEmitter::SetAngle(float angle)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_spawnAngle = angle;
}

float ComponentEmitter::GetRadius()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_radius;
}

void ComponentEmitter::SetRadius(float radius)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_radius = radius;
}

bool ComponentEmitter::GetRandomizeSize()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_randomizeSize;
}

void ComponentEmitter::SetRandomizeSize(bool size)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_randomizeSize = size;
}

float ComponentEmitter::GetMinParticleSize()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_minParticleSize;
}

void ComponentEmitter::SetMinParticleSize(float min)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_minParticleSize = min;
}

float ComponentEmitter::GetMaxParticleSize()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxParticleSize;
}

void ComponentEmitter::SetMaxParticleSize(float max)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxParticleSize = max;
}

bool ComponentEmitter::GetRandomizeSpeed()
{
	return  NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_randomizeSpeed;
}

void ComponentEmitter::SetRandomizeSpeed(bool speed)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_randomizeSpeed = speed;
}

float ComponentEmitter::GetMinParticleSpeed()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_minParticleSpeed;
}

void ComponentEmitter::SetMinParticleSpeed(float min)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_minParticleSpeed = min;
}

float ComponentEmitter::GetMaxParticleSpeed()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxParticleSpeed;
}

void ComponentEmitter::SetMaxParticleSpeed(float max)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxParticleSpeed = max;
}

bool ComponentEmitter::GetRandomizeLife()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_randomizeLifespan;
}

void ComponentEmitter::SetRandomizeLife(bool life)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_randomizeLifespan = life;
}

float ComponentEmitter::GetMinParticleLife()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_minLifespan;
}

void ComponentEmitter::SetMinParticleLife(float min)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_minLifespan = min;
}

float ComponentEmitter::GetMaxParticleLife()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxLifespan;
}

void ComponentEmitter::SetMaxParticleLife(float max)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_maxLifespan = max;
}

bool ComponentEmitter::GetPrewarm()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_preWarm;
}

void ComponentEmitter::SetPrewarm(bool prewarm)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_preWarm = prewarm;
}

bool ComponentEmitter::GetBurst()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_burst;
}

void ComponentEmitter::SetBurst(bool burst)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_burst = burst;
}

bool ComponentEmitter::GetLoop()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_loop;
}

void ComponentEmitter::SetLoop(bool loop)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_loop = loop;
}

bool ComponentEmitter::GetFollow()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_follow;
}

void ComponentEmitter::SetFollow(bool follow)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_follow = follow;
}

bool ComponentEmitter::GetFade()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_fade;
}

void ComponentEmitter::SetFade(bool fade)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_fade = fade;
}

bool ComponentEmitter::GetVelocityOverTime()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_velocityOverTime;
}

void ComponentEmitter::SetVelocityOverTime(bool vel)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_velocityOverTime = vel;
}

bool ComponentEmitter::GetSizeOverTime()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_sizeOverTime;
}

void ComponentEmitter::SetSizeOverTime(bool size)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_sizeOverTime = size;
}

bool ComponentEmitter::GetSpeedOverTime()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_speedOverTime;
}

void ComponentEmitter::SetSpeedOverTime(bool speed)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_speedOverTime = speed;
}

bool ComponentEmitter::GetColourOverTime()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourOverTime;
}

void ComponentEmitter::SetColourOverTime(bool colour)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourOverTime = colour;
}

glm::vec4 ComponentEmitter::GetColourA()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourA;
}

void ComponentEmitter::SetColourA(glm::vec4 colour)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourA = colour;
}

glm::vec4 ComponentEmitter::GetColourB()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourB;
}

void ComponentEmitter::SetColourB(glm::vec4 colour)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourB = colour;
}

glm::vec4 ComponentEmitter::GetColourStart()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourStart;
}

void ComponentEmitter::SetColourStart(glm::vec4 colour)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourStart = colour;
}

glm::vec4 ComponentEmitter::GetColourEnd()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourEnd;
}

void ComponentEmitter::SetColourEnd(glm::vec4 colour)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_colourEnd = colour;
}

glm::vec3 ComponentEmitter::GetVelocity()
{
	return NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_velocity;
}

void ComponentEmitter::SetVelocity(glm::vec3 velocity)
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->_velocity = velocity;
}

ComponentEmitter::~ComponentEmitter()
{
}

void ComponentEmitter::Read(Value& val)
{
	//If doesn't exist
	if (_emitterID < 0)
	{
		_emitterID = NS_GRAPHICS::EmitterSystem::GetInstance().AddEmitter();
	}

	NS_GRAPHICS::EmitterSystem& emitSys = NS_GRAPHICS::EmitterSystem::GetInstance();

	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		_isActive = val["isActive"].GetBool();

	if (val.FindMember("Image") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
	{
		std::string image = val["Image"].GetString();

		if (!image.empty())
		{
			AddTexture(image);
		}
	}

	if (val.FindMember("EmitterType") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
	{
		std::string emitterType = val["EmitterType"].GetString();
		if (emitterType == "SPHERE")
		{
			emitSys._emitters[_emitterID]->_type = EmitterShapeType::SPHERE;
		}
		else if (emitterType == "CONE")
		{
			emitSys._emitters[_emitterID]->_type = EmitterShapeType::CONE;
		}
	}

	if (val.FindMember("DurationPerCycle") == val.MemberEnd())
		std::cout << "No duration data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_durationPerCycle = val["DurationPerCycle"].GetFloat();
	}

	if (val.FindMember("EmissionRateOverTime") == val.MemberEnd())
		std::cout << "No emission data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_emissionOverTime = val["EmissionRateOverTime"].GetUint();

		if (emitSys._emitters[_emitterID]->_emissionOverTime == 0)
		{
			emitSys._emitters[_emitterID]->_emissionRate = 0.0f;
		}
		else
		{
			emitSys._emitters[_emitterID]->_emissionRate = 1.0f / emitSys._emitters[_emitterID]->_emissionOverTime;
		}
	}

	if (val.FindMember("MaxParticle") == val.MemberEnd())
		std::cout << "No particle data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_maxParticles = val["MaxParticle"].GetUint();
		emitSys._emitters[_emitterID]->UpdateSize();
	}

	if (val.FindMember("BurstRate") == val.MemberEnd())
		std::cout << "No BurstRate data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_burstRate = val["BurstRate"].GetFloat();
	}

	if (val.FindMember("BurstAmount") == val.MemberEnd())
		std::cout << "No BurstAmount data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_burstAmount = val["BurstAmount"].GetUint();
	}

	if (val.FindMember("Angle") == val.MemberEnd())
		std::cout << "No angle data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_spawnAngle = val["Angle"].GetFloat();
	}

	if (val.FindMember("Radius") == val.MemberEnd())
		std::cout << "No radius data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_radius = val["Radius"].GetFloat();
	}

	if (val.FindMember("RandomizeSize") == val.MemberEnd())
		std::cout << "No RandomizeSize data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_randomizeSize = val["RandomizeSize"].GetBool();
	}

	if (val.FindMember("MinimumSize") == val.MemberEnd())
		std::cout << "No MinimumSize data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_minParticleSize = val["MinimumSize"].GetFloat();
	}

	if (val.FindMember("MaximumSize") == val.MemberEnd())
		std::cout << "No MaximumSize data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_maxParticleSize = val["MaximumSize"].GetFloat();
	}

	if (val.FindMember("RandomizeSpeed") == val.MemberEnd())
		std::cout << "No RandomizeSize data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_randomizeSpeed = val["RandomizeSpeed"].GetBool();
	}

	if (val.FindMember("MinimumSpeed") == val.MemberEnd())
		std::cout << "No MinimumSize data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_minParticleSpeed = val["MinimumSpeed"].GetFloat();
	}

	if (val.FindMember("MaximumSpeed") == val.MemberEnd())
		std::cout << "No MaximumSize data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_maxParticleSpeed = val["MaximumSpeed"].GetFloat();
	}

	if (val.FindMember("RandomizeLife") == val.MemberEnd())
		std::cout << "No RandomizeLife data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_randomizeLifespan = val["RandomizeLife"].GetBool();
	}

	if (val.FindMember("MinimumLife") == val.MemberEnd())
		std::cout << "No MinimumLife data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_minLifespan = val["MinimumLife"].GetFloat();
	}

	if (val.FindMember("MaximumLife") == val.MemberEnd())
		std::cout << "No MaximumLife data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_maxLifespan = val["MaximumLife"].GetFloat();
	}

	if (val.FindMember("Play") == val.MemberEnd())
		std::cout << "No Play data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_play = val["Play"].GetBool();
	}

	if (val.FindMember("PreWarm") == val.MemberEnd())
		std::cout << "No PreWarm data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_preWarm = val["PreWarm"].GetBool();
	}

	if (val.FindMember("Burst") == val.MemberEnd())
		std::cout << "No Burst data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_burst = val["Burst"].GetBool();
	}

	if (val.FindMember("Loop") == val.MemberEnd())
		std::cout << "No Loop data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_loop = val["Loop"].GetBool();
	}

	if (val.FindMember("Reverse") == val.MemberEnd())
		std::cout << "No Reverse data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_reverse = val["Reverse"].GetBool();
	}

	if (val.FindMember("Follow") == val.MemberEnd())
		std::cout << "No Follow data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_follow = val["Follow"].GetBool();
	}

	if (val.FindMember("Fade") == val.MemberEnd())
		std::cout << "No Fade data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_fade = val["Fade"].GetBool();
	}

	if (val.FindMember("VelocityOverTime") == val.MemberEnd())
		std::cout << "No VelocityOverTime data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_velocityOverTime = val["VelocityOverTime"].GetBool();
	}

	if (val.FindMember("SizeOverTime") == val.MemberEnd())
		std::cout << "No SizeOverTime data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_sizeOverTime = val["SizeOverTime"].GetBool();
	}

	if (val.FindMember("SpeedOverTime") == val.MemberEnd())
		std::cout << "No SpeedOverTime data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_speedOverTime = val["SpeedOverTime"].GetBool();
	}

	if (val.FindMember("ColourOverTime") == val.MemberEnd())
		std::cout << "No ColourOverTime data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_colourOverTime = val["ColourOverTime"].GetBool();
	}

	if (val.FindMember("RandomizeColour") == val.MemberEnd())
		std::cout << "No RandomizeColour data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_randomizeColour = val["RandomizeColour"].GetBool();
	}

	if (val.FindMember("ColourA") == val.MemberEnd())
		std::cout << "No ColourA data has been found" << std::endl;
	else
	{
		auto col = val["ColourA"].GetArray();

		emitSys._emitters[_emitterID]->_colourA.x = col[0].GetFloat();
		emitSys._emitters[_emitterID]->_colourA.y = col[1].GetFloat();
		emitSys._emitters[_emitterID]->_colourA.z = col[2].GetFloat();
		emitSys._emitters[_emitterID]->_colourA.w = col[3].GetFloat();
	}

	if (val.FindMember("ColourB") == val.MemberEnd())
		std::cout << "No ColourB data has been found" << std::endl;
	else
	{
		auto col = val["ColourB"].GetArray();

		emitSys._emitters[_emitterID]->_colourB.x = col[0].GetFloat();
		emitSys._emitters[_emitterID]->_colourB.y = col[1].GetFloat();
		emitSys._emitters[_emitterID]->_colourB.z = col[2].GetFloat();
		emitSys._emitters[_emitterID]->_colourB.w = col[3].GetFloat();
	}

	if (val.FindMember("ColourStart") == val.MemberEnd())
		std::cout << "No ColourStart data has been found" << std::endl;
	else
	{
		auto col = val["ColourStart"].GetArray();

		emitSys._emitters[_emitterID]->_colourStart.x = col[0].GetFloat();
		emitSys._emitters[_emitterID]->_colourStart.y = col[1].GetFloat();
		emitSys._emitters[_emitterID]->_colourStart.z = col[2].GetFloat();
		emitSys._emitters[_emitterID]->_colourStart.w = col[3].GetFloat();
	}

	if (val.FindMember("ColourEnd") == val.MemberEnd())
		std::cout << "No ColourEnd data has been found" << std::endl;
	else
	{
		auto col = val["ColourEnd"].GetArray();

		emitSys._emitters[_emitterID]->_colourEnd.x = col[0].GetFloat();
		emitSys._emitters[_emitterID]->_colourEnd.y = col[1].GetFloat();
		emitSys._emitters[_emitterID]->_colourEnd.z = col[2].GetFloat();
		emitSys._emitters[_emitterID]->_colourEnd.w = col[3].GetFloat();
	}

	if (val.FindMember("Velocity") == val.MemberEnd())
		std::cout << "No Velocity data has been found" << std::endl;
	else
	{
		auto vel = val["Velocity"].GetArray();

		emitSys._emitters[_emitterID]->_velocity.x = vel[0].GetFloat();
		emitSys._emitters[_emitterID]->_velocity.y = vel[1].GetFloat();
		emitSys._emitters[_emitterID]->_velocity.z = vel[2].GetFloat();
	}
}

Value ComponentEmitter::Write()
{
	Value val(rapidjson::kObjectType);
	if (_emitterID < 0)
	{
		return val;
	}

	NS_GRAPHICS::EmitterSystem& emitSys = NS_GRAPHICS::EmitterSystem::GetInstance();

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	NS_SERIALISER::ChangeData(&val, "Image", rapidjson::StringRef(_image.c_str()));

	switch (emitSys._emitters[_emitterID]->_type)
	{
	case EmitterShapeType::SPHERE:
		NS_SERIALISER::ChangeData(&val, "EmitterType", rapidjson::StringRef("SPHERE"));		//custom enum
		break;
	case EmitterShapeType::CONE:
		NS_SERIALISER::ChangeData(&val, "EmitterType", rapidjson::StringRef("CONE"));		//custom enum
		break;
	default:
		break;
	}

	NS_SERIALISER::ChangeData(&val, "DurationPerCycle", emitSys._emitters[_emitterID]->_durationPerCycle);
	NS_SERIALISER::ChangeData(&val, "EmissionRateOverTime", emitSys._emitters[_emitterID]->_emissionOverTime);

	NS_SERIALISER::ChangeData(&val, "MaxParticle", emitSys._emitters[_emitterID]->_maxParticles);

	NS_SERIALISER::ChangeData(&val, "BurstRate", emitSys._emitters[_emitterID]->_burstRate);
	NS_SERIALISER::ChangeData(&val, "BurstAmount", emitSys._emitters[_emitterID]->_burstAmount);

	NS_SERIALISER::ChangeData(&val, "Angle", emitSys._emitters[_emitterID]->_spawnAngle);
	NS_SERIALISER::ChangeData(&val, "Radius", emitSys._emitters[_emitterID]->_radius);

	NS_SERIALISER::ChangeData(&val, "RandomizeSize", emitSys._emitters[_emitterID]->_randomizeSize);
	NS_SERIALISER::ChangeData(&val, "MinimumSize", emitSys._emitters[_emitterID]->_minParticleSize);
	NS_SERIALISER::ChangeData(&val, "MaximumSize", emitSys._emitters[_emitterID]->_maxParticleSize);

	NS_SERIALISER::ChangeData(&val, "RandomizeSpeed", emitSys._emitters[_emitterID]->_randomizeSpeed);
	NS_SERIALISER::ChangeData(&val, "MinimumSpeed", emitSys._emitters[_emitterID]->_minParticleSpeed);
	NS_SERIALISER::ChangeData(&val, "MaximumSpeed", emitSys._emitters[_emitterID]->_maxParticleSpeed);

	NS_SERIALISER::ChangeData(&val, "RandomizeLife", emitSys._emitters[_emitterID]->_randomizeLifespan);
	NS_SERIALISER::ChangeData(&val, "MinimumLife", emitSys._emitters[_emitterID]->_minLifespan);
	NS_SERIALISER::ChangeData(&val, "MaximumLife", emitSys._emitters[_emitterID]->_maxLifespan);

	NS_SERIALISER::ChangeData(&val, "Play", emitSys._emitters[_emitterID]->_play);
	NS_SERIALISER::ChangeData(&val, "PreWarm", emitSys._emitters[_emitterID]->_preWarm);
	NS_SERIALISER::ChangeData(&val, "Burst", emitSys._emitters[_emitterID]->_burst);
	NS_SERIALISER::ChangeData(&val, "Loop", emitSys._emitters[_emitterID]->_loop);
	NS_SERIALISER::ChangeData(&val, "Reverse", emitSys._emitters[_emitterID]->_reverse);
	NS_SERIALISER::ChangeData(&val, "Follow", emitSys._emitters[_emitterID]->_follow);
	NS_SERIALISER::ChangeData(&val, "Fade", emitSys._emitters[_emitterID]->_fade);

	NS_SERIALISER::ChangeData(&val, "VelocityOverTime", emitSys._emitters[_emitterID]->_velocityOverTime);
	NS_SERIALISER::ChangeData(&val, "SizeOverTime", emitSys._emitters[_emitterID]->_sizeOverTime);
	NS_SERIALISER::ChangeData(&val, "SpeedOverTime", emitSys._emitters[_emitterID]->_speedOverTime);
	NS_SERIALISER::ChangeData(&val, "ColourOverTime", emitSys._emitters[_emitterID]->_colourOverTime);

	NS_SERIALISER::ChangeData(&val, "RandomizeColour", emitSys._emitters[_emitterID]->_randomizeColour);

	Value colA(rapidjson::kArrayType);
	colA.PushBack(emitSys._emitters[_emitterID]->_colourA.x, global_alloc);
	colA.PushBack(emitSys._emitters[_emitterID]->_colourA.y, global_alloc);
	colA.PushBack(emitSys._emitters[_emitterID]->_colourA.z, global_alloc);
	colA.PushBack(emitSys._emitters[_emitterID]->_colourA.w, global_alloc);

	NS_SERIALISER::ChangeData(&val, "ColourA", colA);

	Value colB(rapidjson::kArrayType);
	colB.PushBack(emitSys._emitters[_emitterID]->_colourB.x, global_alloc);
	colB.PushBack(emitSys._emitters[_emitterID]->_colourB.y, global_alloc);
	colB.PushBack(emitSys._emitters[_emitterID]->_colourB.z, global_alloc);
	colB.PushBack(emitSys._emitters[_emitterID]->_colourB.w, global_alloc);

	NS_SERIALISER::ChangeData(&val, "ColourB", colB);

	Value colStart(rapidjson::kArrayType);
	colStart.PushBack(emitSys._emitters[_emitterID]->_colourStart.x, global_alloc);
	colStart.PushBack(emitSys._emitters[_emitterID]->_colourStart.y, global_alloc);
	colStart.PushBack(emitSys._emitters[_emitterID]->_colourStart.z, global_alloc);
	colStart.PushBack(emitSys._emitters[_emitterID]->_colourStart.w, global_alloc);

	NS_SERIALISER::ChangeData(&val, "ColourStart", colStart);

	Value colEnd(rapidjson::kArrayType);
	colEnd.PushBack(emitSys._emitters[_emitterID]->_colourEnd.x, global_alloc);
	colEnd.PushBack(emitSys._emitters[_emitterID]->_colourEnd.y, global_alloc);
	colEnd.PushBack(emitSys._emitters[_emitterID]->_colourEnd.z, global_alloc);
	colEnd.PushBack(emitSys._emitters[_emitterID]->_colourEnd.w, global_alloc);

	NS_SERIALISER::ChangeData(&val, "ColourEnd", colEnd);

	Value vel(rapidjson::kArrayType);
	vel.PushBack(emitSys._emitters[_emitterID]->_velocity.x, global_alloc);
	vel.PushBack(emitSys._emitters[_emitterID]->_velocity.y, global_alloc);
	vel.PushBack(emitSys._emitters[_emitterID]->_velocity.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Velocity", vel);

	return val;
}

ComponentEmitter* ComponentEmitter::Clone()
{
	ComponentEmitter* newcomp = new ComponentEmitter();
	*newcomp = *this;
	_emitterID = NS_GRAPHICS::EmitterSystem::GetInstance().AddEmitter();
	return newcomp;
}
