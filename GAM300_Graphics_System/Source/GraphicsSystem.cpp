#include "WndUtils.h"
#include "WndSystem.h"
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
		hasInit{ false }
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
	}

	void GraphicsSystem::ToggleDebugDraw(bool&& set)
	{
	}
}