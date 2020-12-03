#include "ComponentLight.h"
#include "../Graphics/LightSystem.h"
#include "Components.h"
#include "ComponentManager.h"

ComponentLight::ComponentLight()
	: _isActive{ false },
	_lightID{ -1 }, _type{ NS_GRAPHICS::Lights::INVALID_TYPE },
	_PBRdiffuse{ 1.f, 1.f, 1.f}, 
	_intensity{ 1.0f },
	_direction{ 1.f,0.f,0.f },
	_radius{ 10.f },
	_cutOff{}, _outerCutOff{}
	
{
	strcpy_s(ser_name, "LightComponent");
}

ComponentLight::~ComponentLight()
{
}

bool ComponentLight::GetActive() const
{
	return _isActive;
}

NS_GRAPHICS::Lights ComponentLight::GetType() const
{
	return _type;
}

NS_GRAPHICS::Lights ComponentLight::GetInactiveType() const
{
	return _inactiveType;
}

int ComponentLight::GetLightID() const
{
	return _lightID;
}

glm::vec3 ComponentLight::GetColor()
{
	return _PBRdiffuse;
}

float ComponentLight::GetIntensity()
{
	// Recall that attenuation = 1.f / intensity
	// thus intensity = 1.f / attenuation
	return _intensity;
}

float ComponentLight::GetRadius() const
{
	return _radius;
}

float ComponentLight::GetOuterCutOff() const
{
	return _outerCutOff;
}

float ComponentLight::GetCutOff() const
{
	return _cutOff;
}

void ComponentLight::SetLightID(const int& id)
{
	_lightID = id;
}

bool ComponentLight::SetType(const NS_GRAPHICS::Lights& type)
{
	if (GetActive() == false)
	{
		_inactiveType = type;
		return true;
	}

	if (_type != type)
	{
		// call light system to assign light to component
		// this function will set the type for the component itself
		bool checkSet = NS_GRAPHICS::LightSystem::GetInstance().SetLight(type, this);

		// After changing light type, store to inactive type variable
		// This is used to aid active/inactive setter
		_inactiveType = _type;

		return checkSet;
	}

	// JIC
	return true;
}

void ComponentLight::LS_SetType(const NS_GRAPHICS::Lights& type)
{
	_type = type;
}

void ComponentLight::SetColor(const glm::vec3& color)
{
	_PBRdiffuse = color;

	// Do not need to update in uniform block here
	// light system update will take care of that
}

void ComponentLight::SetIntensity(const float& intensity)
{
	_intensity = intensity;
}

void ComponentLight::SetActive(const bool& set)
{
	NS_GRAPHICS::LightSystem::GetInstance().SetActive(this, set);
}

void ComponentLight::LS_SetActive(const bool& set)
{
	_isActive = set;
}

void ComponentLight::SetRadius(const float& radius)
{
	_radius = radius;
}

void ComponentLight::SetOuterCutOff(const float& outercutoff)
{
	_outerCutOff = outercutoff;
}

void ComponentLight::SetCutOff(const float& cutoff)
{
	_cutOff = cutoff;
}

void ComponentLight::Read(Value& val)
{
	if (val.FindMember("LightType") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
	{
		std::string lightName = val["LightType"].GetString();
		if (lightName == "DIRECTIONAL")
		{
			SetType(NS_GRAPHICS::Lights::DIRECTIONAL);
		}
		else if (lightName == "POINT")
		{
			SetType(NS_GRAPHICS::Lights::POINT);
		}
		else if (lightName == "SPOT")
		{
			SetType(NS_GRAPHICS::Lights::SPOT);
		}
	}

	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		SetActive(val["isActive"].GetBool());

	//Error checking for json data
	if (val.FindMember("Diffuse") == val.MemberEnd())
		std::cout << "No Diffuse data has been found" << std::endl;
	else
	{
		auto diffuse = val["Diffuse"].GetArray();

		_PBRdiffuse.x = diffuse[0].GetFloat();
		_PBRdiffuse.y = diffuse[1].GetFloat();
		_PBRdiffuse.z = diffuse[2].GetFloat();
	}

	if (val.FindMember("Intensity") == val.MemberEnd())
		std::cout << "No Intensity data has been found" << std::endl;
	else
	{
		_intensity = val["Intensity"].GetFloat();
	}

	if (val.FindMember("Radius") == val.MemberEnd())
		std::cout << "No Radius data has been found" << std::endl;
	else
	{
		_radius = val["Radius"].GetFloat();
	}

	if (val.FindMember("CutOff") == val.MemberEnd())
		std::cout << "No CutOff data has been found" << std::endl;
	else
	{
		_cutOff = val["CutOff"].GetFloat();
	}

	if (val.FindMember("OuterCutOff") == val.MemberEnd())
		std::cout << "No OuterCutOff data has been found" << std::endl;
	else
	{
		_outerCutOff = val["OuterCutOff"].GetFloat();
	}
}

Value ComponentLight::Write()
{
	Value val(rapidjson::kObjectType);

	NS_GRAPHICS::Lights lighttype = NS_GRAPHICS::Lights::INVALID_TYPE;

	lighttype = (_isActive == false ? _inactiveType : _type);

	switch (lighttype)
	{
	case NS_GRAPHICS::Lights::DIRECTIONAL:
		NS_SERIALISER::ChangeData(&val, "LightType", rapidjson::StringRef("DIRECTIONAL"));		//custom enum
		break;
	case NS_GRAPHICS::Lights::POINT:
		NS_SERIALISER::ChangeData(&val, "LightType", rapidjson::StringRef("POINT"));		//custom enum
		break;
	case NS_GRAPHICS::Lights::SPOT:
		NS_SERIALISER::ChangeData(&val, "LightType", rapidjson::StringRef("SPOT"));		//custom enum
		break;
	default:
		break;
	}

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	Value diffuse(rapidjson::kArrayType);
	diffuse.PushBack(_PBRdiffuse.x, global_alloc);
	diffuse.PushBack(_PBRdiffuse.y, global_alloc);
	diffuse.PushBack(_PBRdiffuse.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Diffuse", diffuse);

	NS_SERIALISER::ChangeData(&val, "Radius", _radius);
	NS_SERIALISER::ChangeData(&val, "Intensity", _intensity);
	NS_SERIALISER::ChangeData(&val, "CutOff", _cutOff);
	NS_SERIALISER::ChangeData(&val, "OuterCutOff", _outerCutOff);

	return val;
}
