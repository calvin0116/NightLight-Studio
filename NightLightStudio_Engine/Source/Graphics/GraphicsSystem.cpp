#include "../Window/WndUtils.h"
#include "../Window/WndSystem.h"
#include "GraphicsSystem.h"

#include "../glm/gtc/matrix_transform.hpp" // glm::perspective
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu

namespace NS_GRAPHICS
{
	GraphicsSystem::GraphicsSystem()
		: shaderManager{ nullptr },
		modelLoader{ nullptr },
		textureLoader{ nullptr },
		modelManager{ nullptr },
		lightManager{ nullptr },
		debugManager{ nullptr },
		cameraManager{ nullptr },
		hasInit{ false },
		debugDrawing{ false },
		projectionMatrix{ glm::mat4(1.0f) },
		viewMatrix{ glm::mat4(1.0f) }
	{
	}

	GraphicsSystem::~GraphicsSystem()
	{
	}

	bool GraphicsSystem::Update()
	{
		cameraManager->Update();

		if (cameraManager->CheckUpdate())
		{
			SetViewMatrix();
		}

		Render();

		return true;
	}

	void GraphicsSystem::Free()
	{
	}

	void GraphicsSystem::Init()
	{
		// If client window has not been initialized, return error without initializing sub systems
		if (!NS_WINDOW::SYS_WINDOW->HasInit())
		{
			std::cout << "ERROR: Window System not initialized before Graphics System, please check WndSystem initialization." << std::endl;
			return;
		}

		// Get pointer to all static instances of sub systems and managers
		shaderManager = &ShaderSystem::GetInstance();
		modelLoader = &ModelLoader::GetInstance();
		textureLoader = &TextureLoader::GetInstance();
		modelManager = &ModelManager::GetInstance();
		lightManager = &LightSystem::GetInstance();
		debugManager = &DebugManager::GetInstance();
		cameraManager = &CameraSystem::GetInstance();

		modelLoader->Init();
		
		// Initialize sub systems and managers
		shaderManager->Init();

		cameraManager->Init();

		// Set default values for view matrix
		// temporary solution before camera system implementation
		// NOTE THAT CAMERA POSITION MUST/SHOULD BE SOME DISTANCE AWAY FROM OBJECT TO BE ABLE TO SEE
		// Positive Z = Towards Screen 
		/*glm::vec3 cameraPos(0.f, 0.f, 5.f);
		glm::vec3 cameraFront(0.f, 0.f, -1.f);
		glm::vec3 cameraUp(0.f, 1.f, 0.f);

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);*/

		// Set default values for view matrix
		SetViewMatrix();

		// Set default values for projection matrix
		SetProjectionMatrix();

		// Enable depth buffering
		glEnable(GL_DEPTH_TEST);

		// Passes if the fragment's depth value is less than the stored depth value.
		// This is the default, but we will call this function to be explicit
		glDepthFunc(GL_LESS);
	}

	void GraphicsSystem::Render()
	{
		// testing basic gl functionality
		// Clears to gray bg
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Sample draw cube
		shaderManager->StartProgram(0);

		GLuint VAO = NULL;
		GLuint VBO = NULL;
		GLuint ModelMatrixBO = NULL;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &ModelMatrixBO);

		float vertices[] = {
			// positions           // texture coords
			 0.5f, -0.5f, 0.0f,    1.0f, 1.0f,   // top right
			-0.5f, -0.5f, 0.0f,    1.0f, 0.0f,   // bottom right
			 0.0f,  0.5f, 0.0f,    0.0f, 0.0f,   // bottom left
		};

		// Provide rotation in radians
		glm::mat4 testmodelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(15.f), glm::vec3(0.0f,0.0f,1.f));

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// pos attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// uv attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, ModelMatrixBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &testmodelMatrix, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void GraphicsSystem::ToggleDebugDraw(const bool& set)
	{
		debugDrawing = set;
	}

	void GraphicsSystem::SetViewMatrix()
	{
		// Update to view matrix in shader program
		glBindBuffer(GL_UNIFORM_BUFFER, shaderManager->GetViewProjectionUniformLocation());
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(cameraManager->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GraphicsSystem::SetProjectionMatrix(const float& fov, const float& aspect_ratio, const float& near_plane, const float& far_plane)
	{
		projectionMatrix = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);

		// Update only if changes are made
		glBindBuffer(GL_UNIFORM_BUFFER, shaderManager->GetViewProjectionUniformLocation());
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(projectionMatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GraphicsSystem::CreateCube(const glm::vec3& rgb, const float& edgeLength)
	{
		// Create cube and put into model manager's vector of models
	}
}