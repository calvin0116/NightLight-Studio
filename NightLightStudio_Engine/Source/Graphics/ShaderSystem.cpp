#include "ShaderSystem.h"
#include <fstream>
#include <sstream>
#include "../Window/WndSystem.h"
#include "../glm/gtc/matrix_transform.hpp"

namespace NS_GRAPHICS
{
	ShaderSystem::ShaderSystem()
		: viewProj_uniformBlockLocation{ NULL }, lights_uniformBlockLocation{ NULL }, currentProgramID{ -1 }
	{
		// Reserve memory to avoid expensive allocations (Reduces load time)
		files.reserve(s_max_shaders);
		programs.reserve(s_max_shaders);
		uniform_locations.reserve(s_max_shaders);
	}

	ShaderSystem::~ShaderSystem()
	{
		// Delete shader programs
		for (auto i : programs)
		{
			glDeleteProgram(i);
		}

		// Delete uniform location buffers
		glDeleteBuffers(1, &viewProj_uniformBlockLocation);
		glDeleteBuffers(1, &lights_uniformBlockLocation);
	}

	void ShaderSystem::LoadShader(const std::string& fp_vertex, const std::string& fp_frag)
	{
		std::stringstream ss_vert_data; // Used as medium for file data from reader to string (vertex)
		std::stringstream ss_frag_data; // Used as medium for file data from reader to string (fragment)
		std::ifstream vert_reader;		// Used to read vertex file data
		std::ifstream frag_reader;		// Used to read fragment file data

		// Allow exceptions to be thrown if file reading fails
		vert_reader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		frag_reader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vert_reader.open(fp_vertex);
			frag_reader.open(fp_frag);

			ss_vert_data << vert_reader.rdbuf();
			ss_frag_data << frag_reader.rdbuf();

			vert_reader.close();
			frag_reader.close();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << std::endl;
		}

		files.emplace_back(ss_vert_data.str(), ss_frag_data.str());
	}

	void ShaderSystem::LoadShaderVariables()
	{
		// Load uniforms, particularly view and projection matrix
		size_t quantity = programs.size();

		for (int i = 0; i < quantity; ++i)
		{
			// Getting the uniform location for view and projection matrix for uniform block for each unique program
			GLuint matrix_uniform_block_index = NULL;
			GLuint lights_uniform_block_index = NULL;

			matrix_uniform_block_index = glGetUniformBlockIndex(programs[i], "Matrices");
			lights_uniform_block_index = glGetUniformBlockIndex(programs[i], "LightCalcBlock");

			// Bind found uniform block index to program
			glUniformBlockBinding(programs[i], matrix_uniform_block_index, 0);
			glUniformBlockBinding(programs[i], lights_uniform_block_index, 1);

			uniform_locations.push_back(matrix_uniform_block_index);
			lights_uniform_locations.push_back(lights_uniform_block_index);
		}

		// Setup single uniform buffer for all programs based on binded uniform block indices
		glGenBuffers(1, &viewProj_uniformBlockLocation);
		glBindBuffer(GL_UNIFORM_BUFFER, viewProj_uniformBlockLocation);
		glBufferData(GL_UNIFORM_BUFFER, s_view_projection_buffer_size, NULL, GL_STATIC_DRAW); // allocate enough for 2 matrices
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, viewProj_uniformBlockLocation, 0, s_view_projection_buffer_size);

		// Set uniform locations for lighting shader
		glGenBuffers(1, &lights_uniformBlockLocation);
		glBindBuffer(GL_UNIFORM_BUFFER, lights_uniformBlockLocation);
		glBufferData(GL_UNIFORM_BUFFER, s_lights_buffer_size, NULL, GL_STATIC_DRAW); // allocate enough for lights variables
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 1, lights_uniformBlockLocation, 0, s_lights_buffer_size);

		// Setup uniform values for PBR texture shader
		if (quantity >= ShaderSystem::PBR_TEXTURED)
		{
			StartProgram(ShaderSystem::PBR_TEXTURED);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			glUniform1i(glGetUniformLocation(currentProgramID, "MetallicTex"), 1); // Metallic
			glUniform1i(glGetUniformLocation(currentProgramID, "RoughnessTex"), 2); // Roughness
			glUniform1i(glGetUniformLocation(currentProgramID, "AOTex"), 3); // AO
			StopProgram();

			StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			glUniform1i(glGetUniformLocation(currentProgramID, "MetallicTex"), 1); // Metallic
			glUniform1i(glGetUniformLocation(currentProgramID, "RoughnessTex"), 2); // Roughness
			glUniform1i(glGetUniformLocation(currentProgramID, "AOTex"), 3); // AO
			StopProgram();
		}
	}

	bool ShaderSystem::CompileLoadedShaders()
	{
		size_t quantity = files.size();

		for (int i = 0; i < quantity; ++i)
		{
			// Create base program
			GLuint program_handler = glCreateProgram();

			GLenum glErr = glGetError();

			if (glErr != GL_NO_ERROR)
			{
				std::cout
					<< "Creation of Shader Program Handler failed for Shader no. "
					<< i
					<< ". Please check GLSL source."
					<< std::endl;
			}

			GLint VShader_Compiled = 0;

			// Create and compile vertex shader
			const char* vertData = files[i].vertexShaderData.c_str(); // Must be const char* for glShaderSource
			GLuint sVertexID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(sVertexID, 1, &vertData, NULL);
			glCompileShader(sVertexID);
			glGetShaderiv(sVertexID, GL_COMPILE_STATUS, &VShader_Compiled);

			if (!VShader_Compiled)
			{
				GLchar infoLog[512];
				glGetShaderInfoLog(sVertexID, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
			}

			GLint FShader_Compiled = 0;

			// Create and compile fragment shader
			const char* fragData = files[i].fragmentShaderData.c_str();
			GLuint sFragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(sFragmentID, 1, &fragData, NULL);
			glCompileShader(sFragmentID);
			glGetShaderiv(sFragmentID, GL_COMPILE_STATUS, &FShader_Compiled);

			if (!FShader_Compiled)
			{
				GLchar infoLog[512];
				glGetShaderInfoLog(sFragmentID, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
			}

			glAttachShader(program_handler, sVertexID);
			glAttachShader(program_handler, sFragmentID);

			GLint Shader_Program_Linked = 0;
			glLinkProgram(program_handler);
			glGetProgramiv(program_handler, GL_LINK_STATUS, &Shader_Program_Linked);

			if (!Shader_Program_Linked)
			{
				GLchar infoLog[512];
				glGetProgramInfoLog(program_handler, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
			}
			else
			{
				std::cout << "Shader Program successfully linked!" << std::endl;
			}

			// Detach and delete shaders to free up memory
			glDetachShader(program_handler, sVertexID);
			glDeleteShader(sVertexID);

			glDetachShader(program_handler, sFragmentID);
			glDeleteShader(sFragmentID);

			programs.push_back(program_handler);
		}

		// Discard unnecessary vector of files after compilation

		// Success
		return true;
	}
	void ShaderSystem::Init()
	{
		// Load all available shaders

		// Shader path example: ../Resources/Shaders/default.vertxs

		LoadShader(std::string("../NightLightStudio_Game/Shaders/default.vert"),std::string("../NightLightStudio_Game/Shaders/uniformsolid.frag")); //DEFAULT
		LoadShader(std::string("../NightLightStudio_Game/Shaders/grid.vert"),std::string("../NightLightStudio_Game/Shaders/grid.frag")); //GRID
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR.vert"), std::string("../NightLightStudio_Game/Shaders/PBR.frag")); //PBR
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Animated.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Animated.frag")); //PBR_ANIMATED
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Textured.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Textured.frag")); //PBR_TEXTURED
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Textured_Animated.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Textured_Animated.frag")); //PBR_TEXTURED_ANIMATED
		LoadShader(std::string("../NightLightStudio_Game/Shaders/ui.vert"), std::string("../NightLightStudio_Game/Shaders/ui.frag")); //UI
		//LoadShader("../NightLightStudio_Game/Shaders/","../NightLightStudio_Game/Shaders/");
		//LoadShader("","");

		if(CompileLoadedShaders())
			LoadShaderVariables();
	}

	void ShaderSystem::StartProgram(const int& programType)
	{
		if (programType >= 0)
		{
			if (programs[programType] == (unsigned)currentProgramID)
				return;

			glUseProgram(programs[programType]);
			currentProgramID = programs[programType];
		}
	}
	void ShaderSystem::StopProgram()
	{
		glUseProgram(NULL);
		currentProgramID = -1;
	}
	const GLuint& ShaderSystem::GetViewProjectionUniformLocation()
	{
		return viewProj_uniformBlockLocation;
	}
	const GLuint& ShaderSystem::GetLightUniformLocation()
	{
		return lights_uniformBlockLocation;
	}
	GLuint ShaderSystem::GetCurrentProgramHandle()
	{
		return currentProgramID;
	}
}