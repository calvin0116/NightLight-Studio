#pragma once
#include <vector>
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	class ShaderSystem
	{
		

		unsigned max_shaders = 4;

		struct ShaderFile
		{
			// File data
			std::string vertexShaderData;
			std::string fragmentShaderData;

			ShaderFile(const std::string& v_shaderData, const std::string& f_shaderData)
				: vertexShaderData(v_shaderData), fragmentShaderData(f_shaderData){}
			~ShaderFile() = default;
		};

		ShaderSystem();
		~ShaderSystem();

		// Load shader from file given file paths
		void LoadShader(const std::string& fp_vertex, const std::string& fp_frag);

		// Compile loaded shaders into shader programs
		bool CompileLoadedShaders();

		// Setup uniform locations & variables
		void LoadShaderVariables();

		////////////////////////////////////////
		// MEMBER VARIABLES
		std::vector<ShaderFile> files;
		std::vector<GLuint> programs;
		std::vector<GLuint> uniform_locations; // Uniform block ID for View & Projection Matrix
		////////////////////////////////////////

		GLuint viewProj_uniformBlockLocation;

	public:

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
	};
}