#include "ComponentLight.h"
#include "../Graphics/LightSystem.h"
#include "Components.h"
#include "ComponentManager.h"


ComponentLight::ComponentLight()
	: _isActive{ true },
	_lightID{ -1 }, _type{ NS_GRAPHICS::Lights::DIRECTIONAL },
	_ambient{}, _diffuse{}, _specular{}, _attenuation{}, _cutOff{}, _outerCutOff{},
	_direction{1.f,0.f,0.f}
{
	strcpy_s(ser_name, "LightComponent");
}

ComponentLight::ComponentLight(const int& lightID, const NS_GRAPHICS::Lights& Type)
	: _isActive{ true },
	_lightID{ lightID }, _type{ Type },
	_ambient{}, _diffuse{}, _specular{}, _attenuation{}, _cutOff{}, _outerCutOff{},
	_direction{ 1.f,0.f,0.f }
{
	strcpy_s(ser_name, "LightComponent");
}

void ComponentLight::AssignLight(const int& lightID, const NS_GRAPHICS::Lights& Type)
{
	_lightID = lightID;
	_type = Type;
}

ComponentLight::~ComponentLight()
{
}

glm::vec3 ComponentLight::GetAmbient() const
{
	if(_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			return NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._ambient;
			break;

		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._ambient;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._ambient;
			break;
		}
	}
	
	return glm::vec3();
}

void ComponentLight::SetAmbient(const glm::vec3& ambient)
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._ambient = ambient;
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._ambient = ambient;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._ambient = ambient;
			break;
		}
	}
}

glm::vec3 ComponentLight::GetDiffuse() const
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			return NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._diffuse;
			break;

		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._diffuse;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._diffuse;
			break;
		}
	}

	return glm::vec3();
}

void ComponentLight::SetDiffuse(const glm::vec3& diffuse)
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._diffuse = diffuse;
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._diffuse = diffuse;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._diffuse = diffuse;
			break;
		}
	}
}

glm::vec3 ComponentLight::GetSpecular() const
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			return NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._specular;
			break;

		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._specular;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._specular;
			break;
		}
	}

	return glm::vec3();
}

void ComponentLight::SetSpecular(const glm::vec3& specular)
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._specular = specular;
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._specular = specular;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._specular = specular;
			break;
		}
	}
}

float ComponentLight::GetIntensity()
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._attenuation;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._attenuation;
			break;

		default:
			return 0.f;
		}
	}

	return 0.f;
}

void ComponentLight::SetIntensity(const float& intensity)
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._attenuation = intensity;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._attenuation = intensity;
			break;

		default:
			break;
		}
	}
}

float ComponentLight::GetAttenuation()
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._attenuation;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._attenuation;
			break;

		default:
			return 0.f;
		}
	}

	return 0.f;
}

void ComponentLight::SetAttenuation(const float& attenuation)
{
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._attenuation = attenuation;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._attenuation = attenuation;
			break;

		default:
			break;
		}
	}
}

//// Type of light: POINT/DIRECTIONAL/SPOT
//NS_GRAPHICS::Lights _type;
//
////Variables For Lighting
////Standard variable
//glm::vec3 _ambient;
//glm::vec3 _diffuse;
//glm::vec3 _specular;
//
////For point and spot
//float _attenuation;
//
////For spot
//float _cutOff;
//float _outerCutOff;
void ComponentLight::Read(Value& val)
{
	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		_isActive = val["isActive"].GetBool();

	if (val.FindMember("LightType") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
	{
		std::string lightName = val["LightType"].GetString();
		if (lightName == "DIRECTIONAL")
		{
			_type = NS_GRAPHICS::Lights::DIRECTIONAL;
		}
		else if (lightName == "POINT")
		{
			_type = NS_GRAPHICS::Lights::POINT;
		}
		else if (lightName == "SPOT")
		{
			_type = NS_GRAPHICS::Lights::SPOT;
		}
	}

	//Error checking for json data
	if (val.FindMember("Diffuse") == val.MemberEnd())
		std::cout << "No Diffuse data has been found" << std::endl;
	else
	{
		auto pos = val["Diffuse"].GetArray();

		_diffuse.x = pos[0].GetFloat();
		_diffuse.y = pos[1].GetFloat();
		_diffuse.z = pos[2].GetFloat();
	}


	if (val.FindMember("Ambient") == val.MemberEnd())
		std::cout << "No Ambient data has been found" << std::endl;
	else
	{
		auto scale = val["Ambient"].GetArray();

		_ambient.x = scale[0].GetFloat();
		_ambient.y = scale[1].GetFloat();
		_ambient.z = scale[2].GetFloat();
	}

	if (val.FindMember("Specular") == val.MemberEnd())
		std::cout << "No Specular data has been found" << std::endl;
	else
	{
		auto rotate = val["Specular"].GetArray();

		_specular.x = rotate[0].GetFloat();
		_specular.y = rotate[1].GetFloat();
		_specular.z = rotate[2].GetFloat();
	}

	if (val.FindMember("Attenuation") == val.MemberEnd())
		std::cout << "No Specular data has been found" << std::endl;
	else
	{
		_attenuation = val["Attenuation"].GetFloat();
	}

	if (val.FindMember("CutOff") == val.MemberEnd())
		std::cout << "No Specular data has been found" << std::endl;
	else
	{
		_cutOff = val["CutOff"].GetFloat();
	}

	if (val.FindMember("Attenuation") == val.MemberEnd())
		std::cout << "No Specular data has been found" << std::endl;
	else
	{
		_outerCutOff = val["OuterCutOff"].GetFloat();
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
	diffuse.PushBack(_diffuse.x, global_alloc);
	diffuse.PushBack(_diffuse.y, global_alloc);
	diffuse.PushBack(_diffuse.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Diffuse", diffuse);

	Value ambient(rapidjson::kArrayType);
	ambient.PushBack(_ambient.x, global_alloc);
	ambient.PushBack(_ambient.y, global_alloc);
	ambient.PushBack(_ambient.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Ambient", ambient);

	Value specular(rapidjson::kArrayType);
	specular.PushBack(_specular.x, global_alloc);
	specular.PushBack(_specular.y, global_alloc);
	specular.PushBack(_specular.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Specular", specular);

	NS_SERIALISER::ChangeData(&val, "Attenuation", _attenuation);
	NS_SERIALISER::ChangeData(&val, "CutOff", _cutOff);
	NS_SERIALISER::ChangeData(&val, "OuterCutOff", _outerCutOff);

	return val;
}
