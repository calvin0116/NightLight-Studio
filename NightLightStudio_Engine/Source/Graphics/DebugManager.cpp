#include "DebugManager.h"
#include "ShaderSystem.h"
#include "GraphicsSystem.h"

namespace NS_GRAPHICS
{
	DebugManager::DebugManager()
	{

	}

	DebugManager::~DebugManager()
	{

	}

	void DebugManager::Init()
	{
		CalculateGrid();

		// Initialize glsl variables and uniforms
		glGenVertexArrays(1, &_grid._VAO);
		glGenBuffers(1, &_grid._VBO);
		
		glBindVertexArray(_grid._VAO);

		// pos attribute
		glBindBuffer(GL_ARRAY_BUFFER, _grid._VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _grid._gridPoints.size(), &_grid._gridPoints[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		// Set uniforms
		ShaderSystem::GetInstance().StartProgram(2);

		_grid._rgba_location = glGetUniformLocation(ShaderSystem::GetInstance().GetCurrentProgramHandle(), "rgba");
		glUniform4fv(_grid._rgba_location, 1, &_grid._rgba[0]);

		ShaderSystem::GetInstance().StopProgram();
	}

	void DebugManager::SetGridExtents(const float& length)
	{
		_grid._gridLength = length;
		_grid._update_required = true;
	}

	void DebugManager::SetCellExtents(const float& length)
	{
		_grid._cellLength = length;
		_grid._update_required = true;
	}

	void DebugManager::SetGridColor(const glm::vec4& RGBA)
	{
		_grid._rgba = RGBA;
		glUniform4fv(_grid._rgba_location, 1, &_grid._rgba[0]);
	}

	void DebugManager::SetGridColor(const float& red, const float& green, const float& blue, const float& alpha)
	{
		SetGridColor(glm::vec4(red, green, blue, alpha));
	}

	void DebugManager::CalculateGrid()
	{
		// Empty current grid data
		_grid._gridPoints.clear();

		unsigned lines_num = static_cast<unsigned>(_grid._gridLength / _grid._cellLength);

		_grid._gridPoints.reserve(lines_num);

		float max_edge = _grid._gridLength * 0.5f;
		float min_edge = -max_edge;

		// Vertical line iterator
		glm::vec3 verticalLineItr(min_edge, 0.f, min_edge);
		glm::vec3 horizontalLineItr(min_edge, 0.f, min_edge);

		for (unsigned i = 0; i <= lines_num; ++i)
		{
			// Vertical line
			_grid._gridPoints.push_back(verticalLineItr); // Start from top edge
			_grid._gridPoints.push_back(verticalLineItr + glm::vec3(0.f,0.f, _grid._gridLength)); // Ends at bottom edge

			// Horizontal line
			_grid._gridPoints.push_back(horizontalLineItr); // Start from left edge
			_grid._gridPoints.push_back(horizontalLineItr + glm::vec3(_grid._gridLength, 0.f, 0.f)); // End at right edge

			verticalLineItr.x += _grid._cellLength;
			horizontalLineItr.z += _grid._cellLength;
		}

		_grid._update_required = false;
	}

	void DebugManager::Render()
	{
		if (_grid._update_required == true)
			CalculateGrid();

		ShaderSystem::GetInstance().StartProgram(2);

		glBindVertexArray(_grid._VAO);

		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(_grid._gridPoints.size()));

		ShaderSystem::GetInstance().StopProgram();
	}
}