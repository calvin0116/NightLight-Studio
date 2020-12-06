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
		///////////////////
		/// GRID SETUP
		///////////////////

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
		ShaderSystem::GetInstance().StartProgram(ShaderSystem::GRID);

		_grid._rgba_location = glGetUniformLocation(ShaderSystem::GetInstance().GetCurrentProgramHandle(), "rgba");
		glUniform4fv(_grid._rgba_location, 1, &_grid._rgba[0]);

		ShaderSystem::GetInstance().StopProgram();

		///////////////////

		///////////////////
		/// DEBUG LINE SETUP
		///////////////////

		// Initialize glsl variables
		glGenVertexArrays(1, &_lines._VAO);
		glGenBuffers(1, &_lines._VBO);
		glGenBuffers(1, &_lines._CBO);

		glBindVertexArray(_lines._VAO);

		// pos attribute
		glBindBuffer(GL_ARRAY_BUFFER, _lines._VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _lines.s_MaxBufferSize, nullptr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		// color attribute
		glBindBuffer(GL_ARRAY_BUFFER, _lines._CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _lines.s_MaxBufferSize, nullptr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(1);
	}

	void DebugManager::SetGridLength(const float& length)
	{
		_grid._gridLength = length;
		_grid._update_required = true;
	}

	void DebugManager::SetCellLength(const float& length)
	{
		_grid._cellLength = length;
		_grid._update_required = true;
	}

	void DebugManager::SetGridColor(const glm::vec4& RGBA)
	{
		ShaderSystem::GetInstance().StartProgram(ShaderSystem::GRID);
		_grid._rgba = RGBA;
		glUniform4fv(_grid._rgba_location, 1, &_grid._rgba[0]);
		ShaderSystem::GetInstance().StopProgram();
	}

	void DebugManager::SetGridColor(const float& red, const float& green, const float& blue, const float& alpha)
	{
		SetGridColor(glm::vec4(red, green, blue, alpha));
	}

	void DebugManager::ShowGrid(const bool& set)
	{
		_grid._render_grid = set;
	}

	const float& DebugManager::GetGridLength() const
	{
		return _grid._gridLength;
	}

	const float& DebugManager::GetCellLength() const
	{
		return _grid._cellLength;
	}

	const glm::vec4& DebugManager::GetGridRGBA() const
	{
		return _grid._rgba;
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

	void DebugManager::AddLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& rgb)
	{
		_lines._pos.push_back(start);
		_lines._pos.push_back(end);

		// Color must be pushed twice
		_lines._color.push_back(rgb);
		_lines._color.push_back(rgb);
	}

	void DebugManager::FlushLineData()
	{
		_lines._pos.clear();

		_lines._color.clear();
	}

	void DebugManager::Render()
	{
		// Draw grid
		if (_grid._render_grid)
		{
			// Set line thickness
			glLineWidth(0.2f);

			glBindVertexArray(_grid._VAO);

			if (_grid._update_required == true)
			{
				CalculateGrid();

				// update to GPU
				glBindBuffer(GL_ARRAY_BUFFER, _grid._VBO);

				// Since size of grid can change, we must use glBufferData to delete old buffer and create a new one to accommodate the size
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _grid._gridPoints.size(), &_grid._gridPoints[0], GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
				glEnableVertexAttribArray(0);
			}

			ShaderSystem::GetInstance().StartProgram(ShaderSystem::GRID);

			glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(_grid._gridPoints.size()));

			ShaderSystem::GetInstance().StopProgram();
		}

		//////////////////////////////////////////////////////

		if (!_lines._pos.size())
			return;

		glLineWidth(4.f);

		// Draw debug lines
		glBindVertexArray(_lines._VAO);

		// pos attribute
		glBindBuffer(GL_ARRAY_BUFFER, _lines._VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * _lines._pos.size(), &_lines._pos[0]);

		// color attribute
		glBindBuffer(GL_ARRAY_BUFFER, _lines._CBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * _lines._color.size(), &_lines._color[0]);

		ShaderSystem::GetInstance().StartProgram(ShaderSystem::DEFAULT);

		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(_lines._pos.size()));

		ShaderSystem::GetInstance().StopProgram();

		FlushLineData(); // Clear line data for current frame since we have finished rendering process
	}
}