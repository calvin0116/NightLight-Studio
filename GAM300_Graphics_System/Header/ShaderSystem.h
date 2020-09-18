#pragma once
#include <vector>
#include "WndUtils.h"

namespace NS_GRAPHICS
{
	class ShaderSystem
	{
		struct ShaderFile
		{
			std::string vertexShader;
			std::string fragmentShader;
		};

		ShaderSystem();
		~ShaderSystem();

		// Load shader from file
		void LoadShader(std::string& s_vertex, std::string& s_frag);
		void CompileLoadedShaders();

		std::vector<ShaderFile> files;
		std::vector<GLuint> programs;

	public:
		// Unique Singleton instance
		static ShaderSystem& GetInstance()
		{
			static ShaderSystem instance;
			return instance;
		}

		void Init();

		void StartProgram(int& programID);
		void StopProgram();

	};
}