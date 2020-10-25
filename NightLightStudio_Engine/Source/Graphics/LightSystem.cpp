#include "LightSystem.h"

namespace NS_GRAPHICS
{
	LightSystem::LightSystem()
	{

	}

	LightSystem::~LightSystem()
	{

	}

	void LightSystem::AddDirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		//dLights.emplace_back(direction, ambient, diffuse, specular);
		direction, ambient, diffuse, specular;
	}

	void LightSystem::AddSpotLight(const glm::vec3& position, const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		//sLights.emplace_back(position, attenuation, ambient, diffuse, specular);
		position, attenuation, ambient, diffuse, specular;
	}

	void LightSystem::AddPointLight(const glm::vec3& direction, const glm::vec3& position, const float& cutoff, const float& outercutoff,
									const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	{
		//pLights.emplace_back(direction, position, cutoff, outercutoff, attenuation, ambient, diffuse, specular);
		direction, position, cutoff, outercutoff, attenuation, ambient, diffuse, specular;
	}
}