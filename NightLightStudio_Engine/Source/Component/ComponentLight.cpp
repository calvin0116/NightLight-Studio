#include "ComponentLight.h"
#include "../Graphics/LightSystem.h"


ComponentLight::ComponentLight()
	: _isActive{ true },
	_lightID{ -1 },
	_type{ NS_GRAPHICS::Lights::DIRECTIONAL }
{
}

ComponentLight::ComponentLight(const int& lightID, const NS_GRAPHICS::Lights& Type)
	: _isActive{ true },
	_lightID{ lightID },
	_type{ Type }
{
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
			return NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)->_ambient;
			break;

		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)->_ambient;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)->_ambient;
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
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)->_ambient = ambient;
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)->_ambient = ambient;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)->_ambient = ambient;
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
			return NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)->_diffuse;
			break;

		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)->_diffuse;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)->_diffuse;
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
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)->_diffuse = diffuse;
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)->_diffuse = diffuse;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)->_diffuse = diffuse;
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
			return NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)->_specular;
			break;

		case NS_GRAPHICS::Lights::POINT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)->_specular;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			return NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)->_specular;
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
			NS_GRAPHICS::LightSystem::GetInstance().GetDirLight(_lightID)->_specular = specular;
			break;

		case NS_GRAPHICS::Lights::POINT:
			NS_GRAPHICS::LightSystem::GetInstance().GetPointLight(_lightID)->_specular = specular;
			break;

		case NS_GRAPHICS::Lights::SPOT:
			NS_GRAPHICS::LightSystem::GetInstance().GetSpotLight(_lightID)->_specular = specular;
			break;
		}
	}
}

void ComponentLight::Read()
{
}

void ComponentLight::Write()
{
}
