#pragma once
#include "../glm/glm.hpp"   // glm::vec3, glm::vec4

#include "../Window/WndUtils.h" // gl functions

namespace NS_GRAPHICS
{
	class DebugManager
	{
		DebugManager();
		~DebugManager();

		struct DebugGrid
		{
			GLuint _VAO = NULL;
			GLuint _VBO = NULL;

			std::vector<glm::vec3> _gridPoints;

			glm::vec4 _rgba = glm::vec4(0.f,0.f,0.f,0.5f);
			GLint _rgba_location = NULL;

			float _gridLength = 100.f;
			float _cellLength = 1.f;

			bool _update_required = false;
		};

		DebugGrid _grid;

	public:
		// Unique Singleton instance
		static DebugManager& GetInstance()
		{
			static DebugManager instance;
			return instance;
		}

		// Set all uniform locations and pushes buffer into shader
		void Init();

		// Sets size of grid
		void SetGridExtents(const float& length);

		// Sets cell size
		void SetCellExtents(const float& length);

		void SetGridColor(const glm::vec4& RGBA);

		void SetGridColor(const float& red, const float& green, const float& blue, const float& alpha);

		void CalculateGrid();

		void Render();
	};
}
