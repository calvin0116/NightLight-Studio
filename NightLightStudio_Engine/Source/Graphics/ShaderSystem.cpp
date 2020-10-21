#include "ShaderSystem.h"
#include <fstream>
#include <sstream>

namespace NS_GRAPHICS
{
	ShaderSystem::ShaderSystem()
		: viewProj_uniformBlockLocation{ NULL }
	{
		// Reserve memory to avoid expensive allocations (Reduces load time)
		files.reserve(max_shaders);
		programs.reserve(max_shaders);
		uniform_locations.reserve(max_shaders);
	}

	ShaderSystem::~ShaderSystem()
	{
		// Delete shader programs
		for (auto i : programs)
		{
			glDeleteProgram(i);
		}
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
			GLuint uniform_block_index = NULL;

			uniform_block_index = glGetUniformBlockIndex(programs[i], "Matrices");

			// Bind found uniform block index to program
			glUniformBlockBinding(programs[i], uniform_block_index, 0);

			uniform_locations.push_back(uniform_block_index);
		}

		// Setup single uniform buffer for all programs based on binded uniform block indices
		glGenBuffers(1, &viewProj_uniformBlockLocation);
		glBindBuffer(GL_UNIFORM_BUFFER, viewProj_uniformBlockLocation);
		glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW); // allocate enough for 2 matrices
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, viewProj_uniformBlockLocation, 0, 128);
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
		LoadShader(std::string("../Resources/Shaders/default.vert"),std::string("../Resources/Shaders/uniformsolid.frag"));
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");
		//LoadShader("","");

		if(CompileLoadedShaders())
			LoadShaderVariables();
	}

	void ShaderSystem::StartProgram(const int& programType)
	{
		glUseProgram(programs[programType]);
	}
	void ShaderSystem::StopProgram()
	{
		glUseProgram(NULL);
	}
	const GLuint& ShaderSystem::GetViewProjectionUniformLocation()
	{
		return viewProj_uniformBlockLocation;
	}
}