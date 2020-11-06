#include "LightSystem.h"
#include "../Component/ComponentLight.h"

namespace NS_GRAPHICS
{
	LightSystem::LightSystem()
	{
		lightblock = new LightBlock();

		/*dLights.reserve(s_MaxLights);
		sLights.reserve(s_MaxLights);
		pLights.reserve(s_MaxLights);*/
	}

	LightSystem::~LightSystem()
	{
		/*for (auto& d : dLights)
			delete d;

		for (auto& s : sLights)
			delete s;

		for (auto& p : pLights)
			delete p;*/

		delete lightblock;
	}

	void LightSystem::Init()
	{
		// Test add 1 light of each type
		// TEST
		AddDirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Somewhat working now?
		//AddPointLight(0.2f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)); // Now this works
		//AddSpotLight(glm::vec3(-1.0f, 0.0f, 0.f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)),
					//0.2f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
	}

	LightBlock*& LightSystem::GetLightBlock()
	{
		return lightblock;
	}

	int LightSystem::AddDirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		//dLights.push_back(new DirLight(direction, ambient, diffuse, specular));
		
		//return static_cast<int>(dLights.size() - 1);

		lightblock->_dLights[lightblock->_dLights_Num]._direction = direction;
		lightblock->_dLights[lightblock->_dLights_Num]._ambient = ambient;
		lightblock->_dLights[lightblock->_dLights_Num]._diffuse = diffuse;
		lightblock->_dLights[lightblock->_dLights_Num]._specular = specular;

		return lightblock->_dLights_Num++;
	}

	int LightSystem::AddPointLight(const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		//pLights.push_back(new PointLight(attenuation, ambient, diffuse, specular));

		//return static_cast<int>(pLights.size() - 1);

		lightblock->_pLights[lightblock->_pLights_Num]._attenuation = attenuation;
		lightblock->_pLights[lightblock->_pLights_Num]._ambient = ambient;
		lightblock->_pLights[lightblock->_pLights_Num]._diffuse = diffuse;
		lightblock->_pLights[lightblock->_pLights_Num]._specular = specular;

		return lightblock->_pLights_Num++;
	}

	int LightSystem::AddSpotLight(const glm::vec3& direction, const float& cutoff, const float& outercutoff,
									const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		//sLights.push_back(new SpotLight(direction, cutoff, outercutoff, attenuation, ambient, diffuse, specular));

		//return static_cast<int>(sLights.size() - 1);

		lightblock->_sLights[lightblock->_sLights_Num]._direction = direction;
		lightblock->_sLights[lightblock->_sLights_Num]._cutOff = cutoff;
		lightblock->_sLights[lightblock->_sLights_Num]._outerCutOff = outercutoff;
		lightblock->_sLights[lightblock->_sLights_Num]._attenuation = attenuation;
		lightblock->_sLights[lightblock->_sLights_Num]._ambient = ambient;
		lightblock->_sLights[lightblock->_sLights_Num]._diffuse = diffuse;
		lightblock->_sLights[lightblock->_sLights_Num]._specular = specular;

		return lightblock->_sLights_Num++;
	}
	void LightSystem::AttachLightComponent(Entity& entity, Lights lightType)
	{
		// Ensure that previous light is deleted before assigning new light type
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
			//NS_GRAPHICS::Lights lightType = lightcomponent->_type;

			// TODO

			// Delete component
			entity.RemoveComponent<ComponentLight>();
		}
			
	}

	void LightSystem::ChangeLightType(Entity& entity, Lights lightType)
	{
		// TO DO
		entity, lightType;
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