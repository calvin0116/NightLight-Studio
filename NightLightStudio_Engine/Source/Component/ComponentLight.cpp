#include "ComponentLight.h"
#include "../Graphics/LightSystem.h"
#include "Components.h"
#include "ComponentManager.h"


ComponentLight::ComponentLight()
	: _isActive{ false },
	_lightID{ -1 }, _type{ NS_GRAPHICS::Lights::INVALID_TYPE },
	_ambient{}, _diffuse{ 1.0f,1.0f,1.0f }, _specular{}, _intensity{ 1.0f }, _cutOff{}, _outerCutOff{},
	_direction{ 1.f,0.f,0.f }
{
	strcpy_s(ser_name, "LightComponent");
}

ComponentLight::ComponentLight(const int& lightID, const NS_GRAPHICS::Lights& Type)
	: _isActive{ false },
	_lightID{ lightID }, _type{ Type },
	_ambient{}, _diffuse{ 1.0f,1.0f,1.0f }, _specular{}, _intensity{ 1.0f }, _cutOff{}, _outerCutOff{},
	_direction{ 1.f,0.f,0.f }
{
	strcpy_s(ser_name, "LightComponent");
}

void ComponentLight::AssignLight(const int& lightID, const NS_GRAPHICS::Lights& Type)
{
	_lightID = lightID;
	_type = Type;

	// Save active type, if valid light is provided
	if (Type != NS_GRAPHICS::Lights::INVALID_TYPE)
		_inactive_type = Type;
}

ComponentLight::~ComponentLight()
{
	//NS_GRAPHICS::LightSystem::GetInstance().RemoveLight(_lightID, _type);
}

void ComponentLight::SetActive(const bool& set)
{
	if (_type == NS_GRAPHICS::Lights::INVALID_TYPE)
		return;

	// No action required if set is same as prev
	if (set == _isActive)
		return;

	if (set == true)
	{
		// Activate light (Add and assign light to component)
		if (_lightID == -1)
		{
			ChangeLightType(_inactive_type);
		}
	}
	else
	{
		// Deactivate light (Delete light from component)
		if (_lightID != -1)
		{
			_inactive_type = _type;
			ChangeLightType(NS_GRAPHICS::Lights::INVALID_TYPE);
		}
	}

	// Set isActive variable to set
	_isActive = set;
}

bool ComponentLight::GetActive() const
{
	return _isActive;
}

NS_GRAPHICS::Lights ComponentLight::GetInactiveType() const
{
	return _inactive_type;
}

void ComponentLight::ChangeLightType(const NS_GRAPHICS::Lights& Type)
{
	_type = Type;
	NS_GRAPHICS::LightSystem::GetInstance().ChangeLightType(this, Type);
}

glm::vec3 ComponentLight::GetAmbient() const
{
	if(_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._ambient);
			break;

		case NS_GRAPHICS::Lights::POINT:
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._ambient);
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._ambient);
			break;
		}
	}
	
	return glm::vec3();
}

void ComponentLight::SetAmbient(const glm::vec3& ambient)
{
	_ambient = ambient;
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._ambient = glm::vec4(ambient, 1.f);
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._ambient = glm::vec4(ambient, 1.f);
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._ambient = glm::vec4(ambient, 1.f);
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
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._diffuse);
			break;

		case NS_GRAPHICS::Lights::POINT:
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._diffuse);
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._diffuse);
			break;
		}
	}

	return glm::vec3();
}

void ComponentLight::SetDiffuse(const glm::vec3& diffuse)
{
	_diffuse = diffuse;
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._diffuse = glm::vec4(diffuse, 1.f);
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._diffuse = glm::vec4(diffuse, 1.f);
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._diffuse = glm::vec4(diffuse, 1.f);
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
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._specular);
			break;

		case NS_GRAPHICS::Lights::POINT:
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._specular);
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return glm::vec3(NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._specular);
			break;
		}
	}

	return glm::vec3();
}

void ComponentLight::SetSpecular(const glm::vec3& specular)
{
	_specular = specular;
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::DIRECTIONAL:
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)._specular = glm::vec4(specular, 1.f);
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._specular = glm::vec4(specular, 1.f);
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._specular = glm::vec4(specular, 1.f);
			break;
		}
	}
}

float ComponentLight::GetIntensity()
{
	// Recall that attenuation = 1.f / intensity
	// thus intensity = 1.f / attenuation
	if (_lightID != -1)
	{
		switch (_type)
		{
		case NS_GRAPHICS::Lights::POINT:
			return (1.f / NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)._attenuation);
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return (1.f / NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)._attenuation);
			break;

		default:
			return 0.f;
		}
	}

	return 0.f;
}

void ComponentLight::SetIntensity(const float& intensity)
{
	_intensity = intensity;
	// Calculate intensity to be 1.f / intensity
	// E.g. intensity = 100.f, attenuation = 1.f / 100f = 0.01f
	// E.g. intensity = 10.f, attenuation = 1.f / 10f = 0.1f
	// lesser attenuation = greater range
	float attenuation = 0.f;
	if (intensity > 0.f)
		attenuation = 1.f / intensity;

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

void ComponentLight::Read(Value& val)
{
	// Remove if any
	NS_GRAPHICS::LightSystem::GetInstance().RemoveLight(_lightID, _type);

	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		SetActive(val["isActive"].GetBool());

	if (val.FindMember("LightType") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
	{
		std::string lightName = val["LightType"].GetString();
		if (lightName == "DIRECTIONAL")
		{
			//ChangeLightType(NS_GRAPHICS::Lights::DIRECTIONAL);
			AssignLight(NS_GRAPHICS::LightSystem::GetInstance().AddDirLight(), NS_GRAPHICS::Lights::DIRECTIONAL);
		}
		else if (lightName == "POINT")
		{
			//ChangeLightType(NS_GRAPHICS::Lights::POINT);
			AssignLight(NS_GRAPHICS::LightSystem::GetInstance().AddPointLight(), NS_GRAPHICS::Lights::POINT);
		}
		else if (lightName == "SPOT")
		{
			//ChangeLightType(NS_GRAPHICS::Lights::SPOT);
			AssignLight(NS_GRAPHICS::LightSystem::GetInstance().AddSpotLight(), NS_GRAPHICS::Lights::SPOT);
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
		SetDiffuse(_diffuse);
	}


	if (val.FindMember("Ambient") == val.MemberEnd())
		std::cout << "No Ambient data has been found" << std::endl;
	else
	{
		auto scale = val["Ambient"].GetArray();

		_ambient.x = scale[0].GetFloat();
		_ambient.y = scale[1].GetFloat();
		_ambient.z = scale[2].GetFloat();
		SetAmbient(_ambient);
	}

	if (val.FindMember("Specular") == val.MemberEnd())
		std::cout << "No Specular data has been found" << std::endl;
	else
	{
		auto rotate = val["Specular"].GetArray();

		_specular.x = rotate[0].GetFloat();
		_specular.y = rotate[1].GetFloat();
		_specular.z = rotate[2].GetFloat();
		SetSpecular(_specular);
	}

	if (val.FindMember("Attenuation") == val.MemberEnd()) // Should be intensity
		std::cout << "No Attenuation data has been found" << std::endl;
	else
	{
		_intensity = val["Attenuation"].GetFloat(); // Should be intensity
		SetAttenuation(1.0f / _intensity);
	}

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

	NS_SERIALISER::ChangeData(&val, "Intensity", _intensity); // Should be intensity
	NS_SERIALISER::ChangeData(&val, "CutOff", _cutOff);
	NS_SERIALISER::ChangeData(&val, "OuterCutOff", _outerCutOff);

	return val;
}
