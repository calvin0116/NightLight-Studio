#include "ShaderSystem.h"

namespace NS_GRAPHICS
{
	ShaderSystem::ShaderSystem()
	{
		// Reserve memory to avoid expensive allocations (Reduces load time)
		files.reserve(max_shaders);
		programs.reserve(max_shaders);
	}

	ShaderSystem::~ShaderSystem()
	{
		// Delete shader programs
		for (auto i : programs)
		{
			glDeleteProgram(i);
		}
	}

	void ShaderSystem::LoadShader(std::string& fp_vertex, std::string& fp_frag)
	{
		fp_vertex;
		fp_frag;
	}

	void ShaderSystem::LoadShaderVariables()
	{

	}

	void ShaderSystem::CompileLoadedShaders()
	{
		size_t quantity = files.size();

		for (int i = 0; i < quantity; ++i)
		{
			//glLinkProgram();

			// Detach and delete shaders to free up memory
			/*glDetachShader(ProgramID, sVertexID);
			glDeleteShader(sVertexID);

			glDetachShader(ProgramID, sFragmentID);
			glDeleteShader(sFragmentID);*/
		}

		// Discard unnecessary vector of files after compilation
	}
	void ShaderSystem::Init()
	{
		// Load all available shaders
		// Shader path example: ../Shaders/sampleshader.vert
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");
	}

	void ShaderSystem::StartProgram(int& programID)
	{
		glUseProgram(programID);
	}
	void ShaderSystem::StopProgram()
	{
		glUseProgram(NULL);
	}
}