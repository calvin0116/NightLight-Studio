#include "LightSystem.h"
#include "../Component/ComponentLight.h"
#include "../Component/ComponentTransform.h"

namespace NS_GRAPHICS
{
	LightSystem::LightSystem()
	{
		lightblock = new LightBlock();
	}

	LightSystem::~LightSystem()
	{
		delete lightblock;
	}

	void LightSystem::Init()
	{
		// Test add 1 light of each type
		// TEST
		//AddDirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Somewhat working now?
		//AddDirLight(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Somewhat working now?
		//AddPointLight(0.2f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Now this works
		//AddSpotLight(glm::vec3(-1.0f, 0.0f, 0.f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)),
					//0.2f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
	}

	void LightSystem::Update()
	{
		// Push transforms into existing lights
		/* The following must be updated each time this is run:
		   - Direction vector based on model transformation from transform component(if any)
		   - Position based on model transformation component(except directional light)
		*/
		auto itr = G_ECMANAGER->begin<ComponentLight>();
		auto itrEnd = G_ECMANAGER->end<ComponentLight>();
		while (itr != itrEnd)
		{
			ComponentLight* lightcomp = G_ECMANAGER->getComponent<ComponentLight>(itr);

			if (lightcomp->_lightID != -1)
			{
				// get transform component
				ComponentTransform* transformComp = G_ECMANAGER->getEntity(itr).getComponent<ComponentTransform>();

				if (transformComp == nullptr)
					continue;

				// Get specified light from light block
				// Dammit I would make a base class to make things more readable but it'll mess up with the light block alignment
				if (lightcomp->_type == Lights::DIRECTIONAL)
				{
					DirLight& dirlight = lightblock->_dLights[lightcomp->_lightID];
					dirlight._direction = transformComp->GetModelMatrix() * glm::vec4(1.f, 0.f, 0.f, 0.f); // w = 0 for vector
				}
				else if (lightcomp->_type == Lights::POINT)
				{
					PointLight& pointlight = lightblock->_pLights[lightcomp->_lightID];
					pointlight.position = transformComp->GetModelMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f); // w = 1 for point
				}
				else if (lightcomp->_type == Lights::SPOT)
				{
					SpotLight& spotlight = lightblock->_sLights[lightcomp->_lightID];
					spotlight._direction = transformComp->GetModelMatrix() * glm::vec4(1.f, 0.f, 0.f, 0.f); // w = 0 for vector
					spotlight.position = transformComp->GetModelMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f); // w = 1 for point
				}
			}
			++itr;
		}
	}

	LightBlock*& LightSystem::GetLightBlock()
	{
		return lightblock;
	}

	int LightSystem::AddDirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		// Locate available slot for light
		int id = 0;

		for (; id < s_MaxLights; ++id)
		{
			if (dLights_tracker[id] == false)
				break;
		}

		// Not enough available lights, return invalid id
		if (id >= s_MaxLights)
			return -1;

		lightblock->_dLights[id]._direction = direction;
		lightblock->_dLights[id]._ambient = ambient;
		lightblock->_dLights[id]._diffuse = diffuse;
		lightblock->_dLights[id]._specular = specular;

		// Set tracker
		dLights_tracker[id] = true;

		lightblock->_dLights_Num++;
		return id;
	}

	int LightSystem::AddPointLight(const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		// Locate available slot for light
		int id = 0;

		for (; id < s_MaxLights; ++id)
		{
			if (pLights_tracker[id] == false)
				break;
		}

		// Not enough available lights, return invalid id
		if (id >= s_MaxLights)
			return -1;

		lightblock->_pLights[id]._attenuation = attenuation;
		lightblock->_pLights[id]._ambient = ambient;
		lightblock->_pLights[id]._diffuse = diffuse;
		lightblock->_pLights[id]._specular = specular;

		// Set tracker
		pLights_tracker[id] = true;

		lightblock->_pLights_Num++;
		return id;
	}

	int LightSystem::AddSpotLight(const glm::vec3& direction, const float& cutoff, const float& outercutoff,
									const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		// Locate available slot for light
		int id = 0;

		for (; id < s_MaxLights; ++id)
		{
			if (sLights_tracker[id] == false)
				break;
		}

		// Not enough available lights, return invalid id
		if (id >= s_MaxLights)
			return -1;

		lightblock->_sLights[id]._direction = direction;
		lightblock->_sLights[id]._cutOff = cutoff;
		lightblock->_sLights[id]._outerCutOff = outercutoff;
		lightblock->_sLights[id]._attenuation = attenuation;
		lightblock->_sLights[id]._ambient = ambient;
		lightblock->_sLights[id]._diffuse = diffuse;
		lightblock->_sLights[id]._specular = specular;

		// Set tracker
		sLights_tracker[id] = true;

		lightblock->_sLights_Num++;
		return id;
	}
	void LightSystem::AttachLightComponent(Entity& entity, Lights lightType)
	{
		// Ensure that previous light is deleted before assigning new light type
		switch (lightType)
		{
		case Lights::DIRECTIONAL:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddDirLight(), Lights::DIRECTIONAL));
				
			break;
		case Lights::POINT:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddPointLight(), Lights::POINT));
			break;
		case Lights::SPOT:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddSpotLight(), Lights::SPOT));
			break;
		default:
			if (entity.getComponent<ComponentLight>() == nullptr)
				entity.AttachComponent<ComponentLight>(ComponentLight(AddDirLight(), Lights::DIRECTIONAL));
			break;
		}
	}

	void LightSystem::DetachLightComponent(Entity& entity)
	{
		ComponentLight* lightcomponent = entity.getComponent<ComponentLight>();

		if (lightcomponent != nullptr)
		{
			// Delete current light from local data
			// Locate light in light tracker and set to false
			if (lightcomponent->_lightID != -1)
			{
				switch (lightcomponent->_type)
				{
				case Lights::DIRECTIONAL:
					dLights_tracker[lightcomponent->_lightID] = false;
					break;
				case Lights::POINT:
					pLights_tracker[lightcomponent->_lightID] = false;
					break;
				case Lights::SPOT:
					sLights_tracker[lightcomponent->_lightID] = false;
					break;
				default:
					break;
				}
			}
			// Delete component
			entity.RemoveComponent<ComponentLight>();
		}
	}

	void LightSystem::ChangeLightType(Entity& entity, Lights lightType)
	{
		ComponentLight* lightcomponent = entity.getComponent<ComponentLight>();

		if (lightcomponent->_type == lightType)
			return;

		if (lightcomponent != nullptr)
		{
			// Delete current light from local data
			// Locate light in light tracker and set to false
			if (lightcomponent->_lightID != -1)
			{
				switch (lightcomponent->_type)
				{
				case Lights::DIRECTIONAL:
					dLights_tracker[lightcomponent->_lightID] = false;
					break;
				case Lights::POINT:
					pLights_tracker[lightcomponent->_lightID] = false;
					break;
				case Lights::SPOT:
					sLights_tracker[lightcomponent->_lightID] = false;
					break;
				default:
					break;
				}
			}

			switch (lightType)
			{
			case Lights::DIRECTIONAL:
				lightcomponent->AssignLight(AddDirLight(), Lights::DIRECTIONAL);
				break;
			case Lights::POINT:
				lightcomponent->AssignLight(AddPointLight(), Lights::POINT);
				break;
			case Lights::SPOT:
				lightcomponent->AssignLight(AddSpotLight(), Lights::SPOT);
				break;
			default:
				break;
			}

#ifdef _DEBUG
			if (lightcomponent->_lightID == -1)
				std::cout << "ERROR: Failed to create light component, please check Light System" << std::endl;
#endif
		}
	}

	DirLight& LightSystem::GetDirLight(const int& id)
	{
		return lightblock->_dLights[id];
	}
	SpotLight& LightSystem::GetSpotLight(const int& id)
	{
		return lightblock->_sLights[id];
	}
	PointLight& LightSystem::GetPointLight(const int& id)
	{
		return lightblock->_pLights[id];
	}
}