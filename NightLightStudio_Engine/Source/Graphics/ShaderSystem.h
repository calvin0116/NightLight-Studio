#pragma once
#include <vector>
#include "../Window/WndUtils.h"
#include "LightSystem.h"

namespace NS_GRAPHICS
{
	class ShaderSystem
	{
		static const unsigned s_max_shaders = 16;
		static const unsigned s_view_projection_buffer_size = 128; // Inclusive of padding
		static const unsigned s_lights_buffer_size = sizeof(LightBlock); // Inclusive of padding

		struct ShaderFile
		{
			// File data
			std::string vertexShaderData;
			std::string fragmentShaderData;

			ShaderFile(const std::string& v_shaderData, const std::string& f_shaderData)
				: vertexShaderData(v_shaderData), fragmentShaderData(f_shaderData){}
			~ShaderFile() = default;
		};

		struct UniformLocations
		{
			// UPDATE ALL UNIFORM LOCATIONS REQUIRED FOR ANY SHADER PROGRAM HERE
			GLint _alpha;
			GLint _gamma;

			GLint _albedo;
			GLint _roughness;
			GLint _metallic;

			GLint _jointsMatrix;

			GLint _roughnessControl;
			GLint _metallicControl;

			GLint _emissive;
			GLint _emissiveIntensity;
		};

		ShaderSystem();
		~ShaderSystem();

		// Load shader from file given file paths
		void LoadShader(const std::string& fp_vertex, const std::string& fp_frag);

		// Compile loaded shaders into shader programs
		bool CompileLoadedShaders();

		// Setup uniform locations & variables
		void LoadShaderVariables();

		void SetTextureLocations();

		////////////////////////////////////////
		// MEMBER VARIABLES
		std::vector<ShaderFile> files;
		std::vector<GLuint> programs;
		std::vector<UniformLocations> p_uniformLocations; // Uniform locations for all programs
		////////////////////////////////////////

		GLuint viewProj_uniformBlockLocation;
		GLuint lights_uniformBlockLocation;

		GLuint ortho_uniformLocation;

		// if no current program is binded, id is -1
		int currentProgramID;

		int programIDIndex;

	public:

		//Made it explicit abit
		enum ShaderType
		{
			DEFAULT,
			GRID,
			PBR,
			PBR_ANIMATED,
			PBR_TEXTURED,
			PBR_TEXTURED_ANIMATED,
			PBR_TEXTURED_NONORMALMAP,
			PBR_TEXTURED_ANIMATED_NONORMALMAP,
			EMISSIVE,
			EMISSIVE_ANIMATED,
			EMISSIVE_TEXTURED,
			EMISSIVE_ANIMATED_TEXTURED,
			UI_SCREENSPACE,
			UI_WORLDSPACE,
			PARTICLE,
			PBR_LIGHTPASS,
			TOTAL_SHADER //If shader type is this means GG
		};

		// Unique Singleton instance
		static ShaderSystem& GetInstance()
		{
			static ShaderSystem instance;
			return instance;
		}

		// Loads all vertex and fragment files required for shaders
		void Init();

		// Binds shader program belonging to given ID. MUST BE STARTED BEFORE ANY RENDER CALLS
		// ID represents the program type(NOT ACTUAL ID) requested
		void StartProgram(const int& programType);

		// Unbind shader program. Render calls will NOT work after this call until a new program is started
		void StopProgram();

		// Gets the uniform location of the current program
		// If currentProgram is 0(no bound program), -1 will be returned(largest possible unsigned)
		const GLuint& GetViewProjectionUniformLocation();

		// Gets the uniform location of the current program
		// If currentProgram is 0(no bound program), -1 will be returned(largest possible unsigned)
		const GLuint& GetLightUniformLocation();

		const GLuint& GetOrthoProjUniformLocation();

		GLuint GetCurrentProgramHandle();

		GLint GetAlphaLocation() const;

		GLint GetGammaLocation() const;

		GLint GetAlbedoLocation() const;

		GLint GetRoughnessLocation() const;

		GLint GetMetallicLocation() const;

		GLint GetJointsMatrixLocation() const;

		GLint GetRoughnessControlLocation() const;

		GLint GetMetallicControlLocation() const;

		GLint GetEmissiveLocation() const;

		GLint GetEmissiveIntensityLocation() const;
	};
}