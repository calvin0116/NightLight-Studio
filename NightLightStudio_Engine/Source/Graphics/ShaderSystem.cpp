#include "ShaderSystem.h"
#include <fstream>
#include <sstream>
#include "../Window/WndSystem.h"
#include "../glm/gtc/matrix_transform.hpp"
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"

namespace NS_GRAPHICS
{
	ShaderSystem::ShaderSystem()
		: viewProj_uniformBlockLocation{ NULL }, lights_uniformBlockLocation{ NULL },
		ortho_uniformLocation{ NULL }, currentProgramID{ -1 }, programIDIndex{ -1 }
	{
		// Reserve memory to avoid expensive allocations (Reduces load time)
		files.reserve(s_max_shaders);
		programs.reserve(s_max_shaders);
		p_uniformLocations.resize(s_max_shaders);
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
			TracyMessageL(std::string("ShaderSystem::LoadShader: ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ").append(e.what()).c_str());
			SPEEDLOG(std::string("ShaderSystem::LoadShader: ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ").append(e.what()));
			//std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << std::endl;
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

			// Set uniform locations for all programs within same loop
			if (i == ShaderType::UI_SCREENSPACE)
			{
				StartProgram(i);
				ortho_uniformLocation = glGetUniformLocation(GetCurrentProgramHandle(), "ortho_proj");
				StopProgram();
				continue;
			}

			if (i == ShaderType::PBR_LIGHTPASS)
			{
				StartProgram(i);
				p_uniformLocations[i]._gamma = glGetUniformLocation(GetCurrentProgramHandle(), "Gamma");
				StopProgram();
				continue;
			}

			if ((i >= ShaderType::PBR) && (i <= ShaderType::EMISSIVE_ANIMATED_TEXTURED))
			{
				StartProgram(i);

				// General PBR uniforms
				p_uniformLocations[i]._gamma = glGetUniformLocation(GetCurrentProgramHandle(), "Gamma");
				p_uniformLocations[i]._alpha = glGetUniformLocation(GetCurrentProgramHandle(), "Alpha");

				// All emissive shaders
				if ((i >= ShaderType::EMISSIVE) && (i <= ShaderType::EMISSIVE_ANIMATED_TEXTURED))
				{
					p_uniformLocations[i]._emissive = glGetUniformLocation(GetCurrentProgramHandle(), "emissive");
					p_uniformLocations[i]._emissiveIntensity = glGetUniformLocation(GetCurrentProgramHandle(), "emissiveIntensity");
				}
					
				// All animated shaders
				if (i == ShaderType::PBR_ANIMATED
					|| i == ShaderType::PBR_TEXTURED_ANIMATED
					|| i == ShaderType::PBR_TEXTURED_ANIMATED_NONORMALMAP
					|| i == ShaderType::EMISSIVE_ANIMATED
					|| i == ShaderType::EMISSIVE_ANIMATED_TEXTURED)
				{
					p_uniformLocations[i]._jointsMatrix = glGetUniformLocation(GetCurrentProgramHandle(), "jointsMat");
				}
				
				// All shaders which use albedo material(not albedo texture)
				if (i == ShaderType::PBR || i == ShaderType::PBR_ANIMATED || i == ShaderType::EMISSIVE || i == ShaderType::EMISSIVE_ANIMATED)
				{
					p_uniformLocations[i]._albedo = glGetUniformLocation(GetCurrentProgramHandle(), "Albedo");

					// Non-textured PBR uniforms
					if (i == ShaderType::PBR || i == ShaderType::PBR_ANIMATED)
					{
						p_uniformLocations[i]._roughness = glGetUniformLocation(GetCurrentProgramHandle(), "Roughness");
						p_uniformLocations[i]._metallic = glGetUniformLocation(GetCurrentProgramHandle(), "Metallic");
					}
				}
				else
				{
					// Textured PBR uniforms
					p_uniformLocations[i]._metallicControl = glGetUniformLocation(GetCurrentProgramHandle(), "RoughnessControl");
					p_uniformLocations[i]._roughnessControl = glGetUniformLocation(GetCurrentProgramHandle(), "MetallicControl");
				}

				StopProgram();

				continue;
			}
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

		SetTextureLocations();
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
				TracyMessageL(std::string("ShaderSystem::CompileLoadedShaders: Creation of Shader Program Handler failed for Shader no. ")
					.append(std::to_string(i) + ". Please check GLSL source.").c_str());
				SPEEDLOG(std::string("ShaderSystem::CompileLoadedShaders: Creation of Shader Program Handler failed for Shader no. ")
					.append(std::to_string(i) + ". Please check GLSL source."));
				/*
				std::cout
					<< "Creation of Shader Program Handler failed for Shader no. "
					<< i
					<< ". Please check GLSL source."
					<< std::endl;
					*/
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
				TracyMessageL(std::string("ShaderSystem::CompileLoadedShaders: ").append(infoLog).c_str());
				SPEEDLOG(std::string("ShaderSystem::CompileLoadedShaders: ").append(infoLog));
				//std::cout << infoLog << std::endl;
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
				TracyMessageL(std::string("ShaderSystem::CompileLoadedShaders: ").append(infoLog).c_str());
				SPEEDLOG(std::string("ShaderSystem::CompileLoadedShaders: ").append(infoLog));
				//std::cout << infoLog << std::endl;
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
				TracyMessageL(std::string("ShaderSystem::CompileLoadedShaders: ").append(infoLog).c_str());
				SPEEDLOG(std::string("ShaderSystem::CompileLoadedShaders: ").append(infoLog));
				//std::cout << infoLog << std::endl;
			}
			else
			{
				TracyMessageL("ShaderSystem::CompileLoadedShaders: Shader Program successfully linked!");
				SPEEDLOG("ShaderSystem::CompileLoadedShaders: Shader Program successfully linked!");
				//std::cout << "Shader Program successfully linked!" << std::endl;
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

		/*
			IMPORTANT NOTES WHEN ADDING PBR:
			1) Ensure that all uniform sampler2D(if any) are accounted for
			2) Ensure that SetGamma() in light system is set for any new PBR shaders
		*/

		LoadShader(std::string("../NightLightStudio_Game/Shaders/default.vert"),std::string("../NightLightStudio_Game/Shaders/default.frag")); //DEFAULT 0
		LoadShader(std::string("../NightLightStudio_Game/Shaders/grid.vert"),std::string("../NightLightStudio_Game/Shaders/grid.frag")); //GRID 1
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR.vert"), std::string("../NightLightStudio_Game/Shaders/PBR.frag")); //PBR 2
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Animated.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Animated.frag")); //PBR_ANIMATED 3
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Textured.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Textured.frag")); //PBR_TEXTURED 4
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Textured_Animated.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Textured_Animated.frag")); //PBR_TEXTURED_ANIMATED 5
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Textured_NoNormalMap.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Textured_NoNormalMap.frag")); //PBR_TEXTURED_NONORMALMAP 6
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_Textured_Animated_NoNormalMap.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_Textured_Animated_NoNormalMap.frag")); //PBR_TEXTURED_ANIMATED_NONORMALMAP 7
		LoadShader(std::string("../NightLightStudio_Game/Shaders/Emissive.vert"), std::string("../NightLightStudio_Game/Shaders/Emissive.frag")); //EMISSIVE 8
		LoadShader(std::string("../NightLightStudio_Game/Shaders/Emissive_Animated.vert"), std::string("../NightLightStudio_Game/Shaders/Emissive_Animated.frag")); //EMISSIVE_ANIMATED 9
		LoadShader(std::string("../NightLightStudio_Game/Shaders/Emissive_Textured.vert"), std::string("../NightLightStudio_Game/Shaders/Emissive_Textured.frag")); //EMISSIVE_TEXTURED 10
		LoadShader(std::string("../NightLightStudio_Game/Shaders/Emissive_Animated_Textured.vert"), std::string("../NightLightStudio_Game/Shaders/Emissive_Animated_Textured.frag")); //EMISSIVE_ANIMATED_TEXTURED 11
		LoadShader(std::string("../NightLightStudio_Game/Shaders/ui.vert"), std::string("../NightLightStudio_Game/Shaders/ui.frag")); //UI_Screenspace 12
		LoadShader(std::string("../NightLightStudio_Game/Shaders/ui_world.vert"), std::string("../NightLightStudio_Game/Shaders/ui_world.frag")); //UI_WorldSpace 13
		LoadShader(std::string("../NightLightStudio_Game/Shaders/particle.vert"), std::string("../NightLightStudio_Game/Shaders/particle.frag")); //Particle 14
		LoadShader(std::string("../NightLightStudio_Game/Shaders/PBR_LightingPass.vert"), std::string("../NightLightStudio_Game/Shaders/PBR_LightingPass.frag")); // PBR_LIGHTPASS 15
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
			programIDIndex = programType;
		}
	}
	void ShaderSystem::StopProgram()
	{
		glUseProgram(NULL);
		currentProgramID = -1;
		programIDIndex = -1;
	}
	const GLuint& ShaderSystem::GetViewProjectionUniformLocation()
	{
		return viewProj_uniformBlockLocation;
	}
	const GLuint& ShaderSystem::GetLightUniformLocation()
	{
		return lights_uniformBlockLocation;
	}
	const GLuint& ShaderSystem::GetOrthoProjUniformLocation()
	{
		return ortho_uniformLocation;
	}
	GLuint ShaderSystem::GetCurrentProgramHandle()
	{
		return currentProgramID;
	}
	GLint ShaderSystem::GetAlphaLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._alpha;
	}
	GLint ShaderSystem::GetGammaLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._gamma;
	}
	GLint ShaderSystem::GetAlbedoLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._albedo;
	}
	GLint ShaderSystem::GetRoughnessLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._roughness;
	}
	GLint ShaderSystem::GetMetallicLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._metallic;
	}
	GLint ShaderSystem::GetJointsMatrixLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._jointsMatrix;
	}
	GLint ShaderSystem::GetRoughnessControlLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._roughnessControl;
	}
	GLint ShaderSystem::GetMetallicControlLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._metallicControl;
	}
	GLint ShaderSystem::GetEmissiveLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._emissive;
	}
	GLint ShaderSystem::GetEmissiveIntensityLocation() const
	{
		if (programIDIndex == -1)
			return GL_INVALID_VALUE;

		return p_uniformLocations[programIDIndex]._emissiveIntensity;
	}
	void ShaderSystem::SetTextureLocations()
	{
		// Setup uniform values for PBR texture shader
		if (programs.size() >= ShaderSystem::PBR_TEXTURED_ANIMATED)
		{
			StartProgram(ShaderSystem::PBR_TEXTURED);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			glUniform1i(glGetUniformLocation(currentProgramID, "MetallicTex"), 1); // Metallic
			glUniform1i(glGetUniformLocation(currentProgramID, "RoughnessTex"), 2); // Roughness
			glUniform1i(glGetUniformLocation(currentProgramID, "AOTex"), 3); // AO
			glUniform1i(glGetUniformLocation(currentProgramID, "NormalTex"), 4); // Normal
			StopProgram();

			StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			glUniform1i(glGetUniformLocation(currentProgramID, "MetallicTex"), 1); // Metallic
			glUniform1i(glGetUniformLocation(currentProgramID, "RoughnessTex"), 2); // Roughness
			glUniform1i(glGetUniformLocation(currentProgramID, "AOTex"), 3); // AO
			glUniform1i(glGetUniformLocation(currentProgramID, "NormalTex"), 4); // Normal
			StopProgram();

			StartProgram(ShaderSystem::PBR_TEXTURED_NONORMALMAP);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			glUniform1i(glGetUniformLocation(currentProgramID, "MetallicTex"), 1); // Metallic
			glUniform1i(glGetUniformLocation(currentProgramID, "RoughnessTex"), 2); // Roughness
			glUniform1i(glGetUniformLocation(currentProgramID, "AOTex"), 3); // AO
			StopProgram();

			StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED_NONORMALMAP);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			glUniform1i(glGetUniformLocation(currentProgramID, "MetallicTex"), 1); // Metallic
			glUniform1i(glGetUniformLocation(currentProgramID, "RoughnessTex"), 2); // Roughness
			glUniform1i(glGetUniformLocation(currentProgramID, "AOTex"), 3); // AO
			StopProgram();

			StartProgram(ShaderSystem::EMISSIVE_TEXTURED);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			StopProgram();

			StartProgram(ShaderSystem::EMISSIVE_ANIMATED_TEXTURED);
			glUniform1i(glGetUniformLocation(currentProgramID, "AlbedoTex"), 0); // Albedo
			StopProgram();

			//StartProgram(ShaderSystem::PBR_LIGHTPASS);
			//glUniform1i(glGetUniformLocation(currentProgramID, "gPositionAlpha"), 0);    // Position + Alpha
			//glUniform1i(glGetUniformLocation(currentProgramID, "gNormalMetallic"), 1);   // Normal + Metallic
			//glUniform1i(glGetUniformLocation(currentProgramID, "gAlbedoRoughness"), 2);  // Albedo + Roughness
			//glUniform1i(glGetUniformLocation(currentProgramID, "gAmbientOcclusion"), 3); // Ambient Occlusion
			//StopProgram();
		}
	}
}