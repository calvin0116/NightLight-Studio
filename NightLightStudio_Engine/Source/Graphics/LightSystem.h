#pragma once

#include "../../framework.h"
#include "Lights.h"

namespace NS_GRAPHICS
{
	class LightSystem
	{
		LightSystem();
		~LightSystem();

		std::vector<DirLight*> dLights;
		std::vector<PointLight*> pLights;
		std::vector<SpotLight*> sLights;

	public:
		// Unique Singleton instance
		static LightSystem& GetInstance()
		{
			static LightSystem instance;
			return instance;
		}

		/* TO DO*/
		// Add Light component integration

		// Adds Directional Light to scene
		void AddDirLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

		// Adds Spot Light to scene
		void AddSpotLight(const glm::vec3& position, const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

		// Adds Point Light to scene
		void AddPointLight(const glm::vec3& direction, const glm::vec3& position, const float& cutoff, const float& outercutoff,
						const float& attenuation, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
	};
}
