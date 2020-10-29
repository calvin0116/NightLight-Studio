#include "LightSystem.h"
#include "../Component/ComponentLight.h"

namespace NS_GRAPHICS
{
	LightSystem::LightSystem()
	{
		dLights.reserve(s_MaxLights);
		sLights.reserve(s_MaxLights);
		pLights.reserve(s_MaxLights);
	}

	LightSystem::~LightSystem()
	{
		for (auto& d : dLights)
			delete d;

		for (auto& s : sLights)
			delete s;

		for (auto& p : pLights)
			delete p;
	}

	int LightSystem::AddDirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		dLights.push_back(new DirLight(direction, ambient, diffuse, specular));

		return static_cast<int>(dLights.size() - 1);
	}

	int LightSystem::AddPointLight(const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		pLights.push_back(new PointLight(attenuation, ambient, diffuse, specular));

		return static_cast<int>(pLights.size() - 1);
	}

	int LightSystem::AddSpotLight(const glm::vec3& direction, const float& cutoff, const float& outercutoff,
									const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		sLights.push_back(new SpotLight(direction, cutoff, outercutoff, attenuation, ambient, diffuse, specular));

		return static_cast<int>(sLights.size() - 1);
	}
	void LightSystem::AttachLightComponent(Entity& entity, Lights lightType)
	{
		switch (lightType)
		{
		case Lights::DIRECTIONAL:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddDirLight(), Lights::DIRECTIONAL));
			else
				entity.getComponent<ComponentLight>()->AssignLight(AddDirLight(),Lights::DIRECTIONAL);
				
			break;
		case Lights::POINT:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddPointLight(), Lights::POINT));
			else
				entity.getComponent<ComponentLight>()->AssignLight(AddPointLight(), Lights::POINT);
			break;
		case Lights::SPOT:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddSpotLight(), Lights::SPOT));
			else
				entity.getComponent<ComponentLight>()->AssignLight(AddSpotLight(), Lights::SPOT);
			break;
		default:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddDirLight(), Lights::DIRECTIONAL));
			else
				entity.getComponent<ComponentLight>()->AssignLight(AddDirLight(), Lights::DIRECTIONAL);
			break;
		}
	}

	void LightSystem::DetachLightComponent(Entity& entity)
	{
		ComponentLight* lightcomponent = entity.getComponent<ComponentLight>();

		if (lightcomponent != nullptr)
		{
			// Delete current light from local data
			NS_GRAPHICS::Lights lightType = lightcomponent->_type;



			// Delete component
			entity.RemoveComponent<ComponentLight>();
		}
			
	}

	void LightSystem::ChangeLightType(Entity& entity, Lights lightType)
	{
		// TO DO
		entity, lightType;
	}

	DirLight*& LightSystem::GetDirLight(const int& id)
	{
		return dLights[id];
	}
	SpotLight*& LightSystem::GetSpotLight(const int& id)
	{
		return sLights[id];
	}
	PointLight*& LightSystem::GetPointLight(const int& id)
	{
		return pLights[id];
	}
}