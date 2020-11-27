#pragma once

#include "../../framework.h"
#include "../Component/ComponentManager.h"
#include "../Component/ComponentLight.h"
#include <array>

#include "Lights.h"

namespace NS_GRAPHICS
{
	static const size_t s_MaxLights = 30;

	// 3440
	struct LightBlock
	{
		// 3408 total
		DirLight _dLight[1];				// 0 - 47
		PointLight _pLights[s_MaxLights];   // 48 - 1487
		SpotLight _sLights[s_MaxLights];	// 1488 - 3407
		// 0 - 3407

		// Number of lights currently in scene
		int _dLights_Num; // 3408 - 3411
		int _pLights_Num; // 3412 - 3415
		int _sLights_Num; // 3416 - 3419
		// 3420

		float _dummyPadding1 = 0.f; // 3420 - 3423
		// Compiler will align view pos to 7216
		glm::vec4 _viewPos; // 3424 - 3439

		LightBlock()
			: _dLights_Num{ NULL },
			_pLights_Num{ NULL },
			_sLights_Num{ NULL },
			_viewPos()
		{}
		~LightBlock() = default;
	};

	class LightSystem
	{
		LightSystem();
		~LightSystem();

		LightBlock* lightblock;

	public:
		// Unique Singleton instance
		static LightSystem& GetInstance()
		{
			static LightSystem instance;
			return instance;
		}

		void Init();

		void Update();

		// Get lightblock from lightsystem
		LightBlock*& GetLightBlock();

		void SortLights(const Lights& lightType, const int& deletedIndex);

		void SetAllDirectionalLights(const bool& setter);

		void SetAllPointLights(const bool& setter);

		void SetAllLights(const bool& setter);

		// NEW INTERFACE FOR LIGHTS

		// Return true if light is proper set, else return false
		bool SetLight(const Lights& type, ComponentLight* const& comp);

		// Sets directional light to provided component
		void SetDirectionalLight(ComponentLight* const& comp);

		// Sets point light to provided component
		void SetPointLight(ComponentLight* const& comp);

		// Sets spot light to provided component
		void SetSpotLight(ComponentLight* const& comp);

		// Removes tracker from light component / deletes light
		void RemoveLight(ComponentLight* const& comp);

		// Sets active for component, requires special functionality due to uniform block
		// uses inactive type to store active state properties
		void SetActive(ComponentLight* const& comp, const bool& set);

		// Remove light helper
		void RemoveLightHelper(const Lights& type, const int& id);
	};
}