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
			bool _render_grid = true;
		};

		struct DebugLines
		{
			// Note that number of lines allowed is s_MaxBufferSize / 2
			static const unsigned s_MaxBufferSize = 1000000;

			GLuint _VAO = NULL;
			GLuint _VBO = NULL;
			GLuint _CBO = NULL;

			// Debug draw lines
			std::vector<glm::vec3> _pos; // Actual line position
			std::vector<glm::vec3> _color; // Line color
		};

		// Editor Grid
		DebugGrid _grid;
		DebugLines _lines;

	public:
		// Unique Singleton instance
		static DebugManager& GetInstance()
		{
			static DebugManager instance;
			return instance;
		}

		// Set all uniform locations and pushes buffer into shader
		void Init();


		/////////////////////////////////////
		/// GRID FUNCTIONS
		/////////////////////////////////////

		// Sets size of grid
		void SetGridLength(const float& length);

		// Sets cell size
		void SetCellLength(const float& length);

		// Set grid color
		void SetGridColor(const glm::vec4& RGBA);

		void SetGridColor(const float& red, const float& green, const float& blue, const float& alpha);

		void ShowGrid(const bool& set);

		const float& GetGridLength() const;

		const float& GetCellLength() const;

		const glm::vec4& GetGridRGBA() const;

		void CalculateGrid();

		/////////////////////////////////////

		/////////////////////////////////////
		/// DEBUG LINE FUNCTIONS
		/////////////////////////////////////

		// Adds line to draw for next frame
		void AddLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& rgb);

		void FlushLineData();

		/////////////////////////////////////

		void Render();
	};
}
