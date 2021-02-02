#include "ComponentEmitter.h"
#include "../Graphics/SystemEmitter.h"
#include "../Graphics/TextureManager.h"
#include "Components.h"
#include "ComponentManager.h"
#pragma warning( disable : 26812 )

ComponentEmitter::ComponentEmitter()
	: _isActive{ true }
{
	strcpy_s(ser_name, "EmitterComponent");
}

ComponentEmitter::ComponentEmitter(const int& emitterID)
	: _isActive{ true }
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
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->Play();
}

void ComponentEmitter::Stop()
{
	NS_GRAPHICS::EmitterSystem::GetInstance()._emitters[_emitterID]->Stop();
}

ComponentEmitter::~ComponentEmitter()
{
}

void ComponentEmitter::Read(Value& val)
{
	_emitterID = NS_GRAPHICS::EmitterSystem::GetInstance().AddEmitter();
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
	}

	if (val.FindMember("DurationPerCycle") == val.MemberEnd())
		std::cout << "No duration data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_durationPerCycle = val["DurationPerCycle"].GetFloat();
	}

	if (val.FindMember("EmissionRate") == val.MemberEnd())
		std::cout << "No emission data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_emissionRate = val["EmissionRate"].GetFloat();
	}

	if (val.FindMember("MaxParticle") == val.MemberEnd())
		std::cout << "No particle data has been found" << std::endl;
	else
	{
		emitSys._emitters[_emitterID]->_maxParticles = val["MaxParticle"].GetUint();
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

	NS_GRAPHICS::EmitterSystem& emitSys = NS_GRAPHICS::EmitterSystem::GetInstance();

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	NS_SERIALISER::ChangeData(&val, "Image", rapidjson::StringRef(_image.c_str()));

	switch (emitSys._emitters[_emitterID]->_type)
	{
	case EmitterShapeType::SPHERE:
		NS_SERIALISER::ChangeData(&val, "EmitterType", rapidjson::StringRef("SPHERE"));		//custom enum
		break;
	default:
		break;
	}

	NS_SERIALISER::ChangeData(&val, "DurationPerCycle", emitSys._emitters[_emitterID]->_durationPerCycle);
	NS_SERIALISER::ChangeData(&val, "EmissionRate", emitSys._emitters[_emitterID]->_emissionRate);

	NS_SERIALISER::ChangeData(&val, "MaxParticle", emitSys._emitters[_emitterID]->_maxParticles);

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
