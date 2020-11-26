#include "ComponentLight.h"
#include "../Graphics/LightSystem.h"
#include "Components.h"
#include "ComponentManager.h"


//ComponentLight::ComponentLight()
//	: _isActive{ false },
//	_lightID{ -1 }, _type{ NS_GRAPHICS::Lights::INVALID_TYPE },
//	_ambient{}, _diffuse{ 1.0f,1.0f,1.0f }, _specular{}, _intensity{ 1.0f }, _cutOff{}, _outerCutOff{},
//	_direction{ 1.f,0.f,0.f }
//{
//	strcpy_s(ser_name, "LightComponent");
//}

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
	// Nothing must be called here because no
	//NS_GRAPHICS::LightSystem::GetInstance().RemoveLight(_lightID, _type);
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

void ComponentLight::SetType(const NS_GRAPHICS::Lights& type)
{
	if (GetActive() == false)
	{
		_inactiveType = type;
		return;
	}

	if (_type != type)
	{
		// call light system to assign light to component
		// this function will set the type for the component itself
		NS_GRAPHICS::LightSystem::GetInstance().SetLight(type, this);

		// After changing light type, store to inactive type variable
		// This is used to aid active/inactive setter
		_inactiveType = _type;
	}
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
			//ChangeLightType(NS_GRAPHICS::Lights::DIRECTIONAL);
			//AssignLight(NS_GRAPHICS::LightSystem::GetInstance().AddDirLight(), NS_GRAPHICS::Lights::DIRECTIONAL);
			SetType(NS_GRAPHICS::Lights::DIRECTIONAL);
		}
		else if (lightName == "POINT")
		{
			//ChangeLightType(NS_GRAPHICS::Lights::POINT);
			//AssignLight(NS_GRAPHICS::LightSystem::GetInstance().AddPointLight(), NS_GRAPHICS::Lights::POINT);
			SetType(NS_GRAPHICS::Lights::POINT);
		}
		else if (lightName == "SPOT")
		{
			//ChangeLightType(NS_GRAPHICS::Lights::SPOT);
			//AssignLight(NS_GRAPHICS::LightSystem::GetInstance().AddSpotLight(), NS_GRAPHICS::Lights::SPOT);
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
		//SetDiffuse(_diffuse);
	}


	//if (val.FindMember("Ambient") == val.MemberEnd())
	//	std::cout << "No Ambient data has been found" << std::endl;
	//else
	//{
	//	//auto scale = val["Ambient"].GetArray();

	//	//_ambient.x = scale[0].GetFloat();
	//	//_ambient.y = scale[1].GetFloat();
	//	//_ambient.z = scale[2].GetFloat();
	//	//SetAmbient(_ambient);
	//}

	//if (val.FindMember("Specular") == val.MemberEnd())
	//	std::cout << "No Specular data has been found" << std::endl;
	//else
	//{
	//	//auto rotate = val["Specular"].GetArray();

	//	//_specular.x = rotate[0].GetFloat();
	//	//_specular.y = rotate[1].GetFloat();
	//	//_specular.z = rotate[2].GetFloat();
	//	//SetSpecular(_specular);
	//}

	//if (val.FindMember("Attenuation") == val.MemberEnd()) // Should be intensity
	//	std::cout << "No Attenuation data has been found" << std::endl;
	//else
	//{
	//	_intensity = val["Attenuation"].GetFloat(); // Should be intensity
	//	SetIntensity(_intensity);
	//}

	if (val.FindMember("Intensity") == val.MemberEnd()) // Should be intensity
		std::cout << "No Intensity data has been found" << std::endl;
	else
	{
		_intensity = val["Intensity"].GetFloat(); // Should be intensity
		SetIntensity(_intensity);
	}

	if (val.FindMember("CutOff") == val.MemberEnd())
		std::cout << "No CutOff data has been found" << std::endl;
	else
	{
		_cutOff = val["CutOff"].GetFloat();
		// Assign cutoff to spot light, if any
	}

	if (val.FindMember("OuterCutOff") == val.MemberEnd())
		std::cout << "No OuterCutOff data has been found" << std::endl;
	else
	{
		_outerCutOff = val["OuterCutOff"].GetFloat();
		// Assign outercuttoff to spotlight, if any
	}
}

Value ComponentLight::Write()
{
	Value val(rapidjson::kObjectType);

	switch (_type)
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

	/*Value ambient(rapidjson::kArrayType);
	ambient.PushBack(_ambient.x, global_alloc);
	ambient.PushBack(_ambient.y, global_alloc);
	ambient.PushBack(_ambient.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Ambient", ambient);

	Value specular(rapidjson::kArrayType);
	specular.PushBack(_specular.x, global_alloc);
	specular.PushBack(_specular.y, global_alloc);
	specular.PushBack(_specular.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Specular", specular);*/

	NS_SERIALISER::ChangeData(&val, "Intensity", _intensity); // Should be intensity
	NS_SERIALISER::ChangeData(&val, "CutOff", _cutOff);
	NS_SERIALISER::ChangeData(&val, "OuterCutOff", _outerCutOff);

	return val;
}
