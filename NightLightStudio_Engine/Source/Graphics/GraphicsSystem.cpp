#include "../Window/WndUtils.h"
#include "../Window/WndSystem.h"
#include "GraphicsSystem.h"

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
		debugDrawing{ false }
	{
	}

	GraphicsSystem::~GraphicsSystem()
	{
	}

	bool GraphicsSystem::Update()
	{
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
	}

	void GraphicsSystem::Render()
	{
		// testing basic gl functionality
		// Clears to gray bg
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Sample draw cube
	}

	void GraphicsSystem::ToggleDebugDraw(bool&& set)
	{
		debugDrawing = set;
	}
}