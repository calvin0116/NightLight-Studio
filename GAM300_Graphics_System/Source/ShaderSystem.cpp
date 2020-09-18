#include "ShaderSystem.h"

namespace NS_GRAPHICS
{
	ShaderSystem::ShaderSystem()
	{
	}

	ShaderSystem::~ShaderSystem()
	{

	}

	void ShaderSystem::LoadShader(std::string& s_vertex, std::string& s_frag)
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
	}
	void ShaderSystem::Init()
	{
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");
	}

	void ShaderSystem::StartProgram(int& programID)
	{
	}
	void ShaderSystem::StopProgram()
	{
	}
}