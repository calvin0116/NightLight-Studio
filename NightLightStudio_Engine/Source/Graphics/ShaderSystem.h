#pragma once
#include <vector>
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	class ShaderSystem
	{
		enum class ShaderPrograms
		{
			s_basic = 0,
			s_textured
		};

		unsigned max_shaders = 4;

		struct ShaderFile
		{
			// File data
			std::string vertexShaderData;
			std::string fragmentShaderData;
		};

		ShaderSystem();
		~ShaderSystem();

		// Load shader from file given file paths
		void LoadShader(std::string& fp_vertex, std::string& fp_frag);

		// Setup uniform locations & variables
		void LoadShaderVariables();

		void CompileLoadedShaders();

		////////////////////////////////////////
		// MEMBER VARIABLES
		std::vector<ShaderFile> files;
		std::vector<GLuint> programs;
		std::vector<GLuint> uniform_locations;
		////////////////////////////////////////

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
		void StartProgram(int& programID);

		// Unbind shader program. Render calls will NOT work after this call until a new program is started
		void StopProgram();

	};
}