#include "WndUtils.h"
#include "WndSystem.h"
#include "GraphicsSystem.h"

namespace NS_GRAPHICS
{
	// Global Pointer to Application
	// Other files should not touch this
	GraphicsSystem* pGraphicsSys = &GraphicsSystem::GetInstance();

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

	void GraphicsSystem::Update()
	{
	}

	void GraphicsSystem::Free()
	{
	}

	void GraphicsSystem::Init()
	{
		// Get pointer to all static instances of sub systems and managers
		shaderManager = &ShaderSystem::GetInstance();
		modelLoader = &ModelLoader::GetInstance();
		textureLoader = &TextureLoader::GetInstance();
		modelManager = &ModelManager::GetInstance();
		lightManager = &LightSystem::GetInstance();
		debugManager = &DebugManager::GetInstance();
		cameraManager = &CameraSystem::GetInstance();
	}

	void GraphicsSystem::Render()
	{
		// testing basic gl functionality
		// Clears to yellow bg
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsSystem::ToggleDebugDraw(bool&& set)
	{
		debugDrawing = set;
	}
}