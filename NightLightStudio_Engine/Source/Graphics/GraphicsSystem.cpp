#include "../Window/WndUtils.h"
#include "../Window/WndSystem.h"
#include "GraphicsSystem.h"

#include "../Component/Components.h" // Access to ECManager
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentGraphics.h"

#include "../glm/gtc/matrix_transform.hpp" // glm::perspective
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu

#include <fstream> //Temporary

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define DRAW_DEBUG_GRID
#define PBR_DRAWING

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
		textureManager{ nullptr },
		uiManager{ nullptr },
		animManager{ nullptr },
		_hasInit{ false },
		_debugDrawing{ false },
		_uiDrawing{ false },
		_hdrID{ 0 },
		_projectionMatrix{ glm::mat4(1.0f) },
		_viewMatrix{ glm::mat4(1.0f) },
		_orthoMatrix{ glm::mat4(1.0f) },
		_testTimeElapsed{0.0f}
	{
	}

	GraphicsSystem::~GraphicsSystem()
	{
	}

	void GraphicsSystem::Update()
	{
		cameraManager->Update();
		//0.01f ms to s
		//_testTimeElapsed += DELTA_T->real_dt * DT_SCALE;

		//std::cout << "Time Passed : " << _testTimeElapsed << std::endl;

		if (cameraManager->CheckUpdate())
		{
			SetViewMatrix();
			cameraManager->AckUpdate();
		}

		// Update lights
		UpdateLights();

		// Update Animation
		animManager->Update();

		Render();

		uiManager->Update();

		//std::cout << "Time current passed: " << _testTimeElapsed << std::endl;
	}

	void GraphicsSystem::Free()
	{
		// Free all OpenGL objects before exit
		// Includes VAO, VBO, EBO, ModelMatrixBO
		modelManager->Free();
		textureManager->Free();
		animManager->Free();
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
		textureManager = &TextureManager::GetInstance();
		uiManager = &UISystem::GetInstance();
		animManager = &AnimationSystem::GetInstance();

		//modelLoader->Init();
		
		// Initialize sub systems and managers
		shaderManager->Init();

		// Set default values for view matrix
		// Set here to avoid weird bug at beginning
		SetViewMatrix();

		cameraManager->Init();

		lightManager->Init();

		uiManager->Init();
		
		shaderManager->StartProgram(ShaderSystem::PBR);

		GLint blockSize;

		glGetActiveUniformBlockiv(shaderManager->GetCurrentProgramHandle(), 0,
			GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		std::cout << "Light Uniform Block Size: " << blockSize << std::endl;
		std::cout << "Light Uniform Block Size(CPU): " << sizeof(LightBlock) << std::endl;
		std::cout << "Directional Light Size(CPU): " << sizeof(DirLight) << std::endl;
		std::cout << "Point Light Size(CPU): " << sizeof(PointLight) << std::endl;
		std::cout << "Spot Light Size(CPU): " << sizeof(SpotLight) << std::endl;

		shaderManager->StopProgram();

		// Set default values for projection matrix
		SetProjectionMatrix();
		SetUIMatrix(NS_WINDOW::SYS_WINDOW->GetResolutionWidth(), NS_WINDOW::SYS_WINDOW->GetResolutionHeight());

		debugManager->Init();

		// Enable depth buffering
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Passes if the fragment's depth value is less than the stored depth value.
		// This is the default, but we will call this function to be explicit
		glDepthFunc(GL_LESS);

		SetLineThickness();

		//Set Grid Last Settings
		//If not is not available
		if (CONFIG_DATA->GetConfigData()._cellSize != 0.0f)
		{
			glm::vec4 gridColour = glm::vec4(CONFIG_DATA->GetConfigData()._gridColourRed,
				CONFIG_DATA->GetConfigData()._gridColourGreen,
				CONFIG_DATA->GetConfigData()._gridColourBlue,
				CONFIG_DATA->GetConfigData()._gridColourAlpha);

			debugManager->SetGridColor(gridColour);

			debugManager->SetCellLength(CONFIG_DATA->GetConfigData()._cellSize);
			debugManager->SetGridLength(CONFIG_DATA->GetConfigData()._gridSize);
		}

	}

	void GraphicsSystem::Exit()
	{
		DestroyInstance();
	}

	void GraphicsSystem::GameExit()
	{
		// Call for lights clear
		lightManager->RemoveAllLights();
	}

	void GraphicsSystem::Render()
	{
		// testing basic gl functionality
		// Clears to gray bg
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef PBR_DRAWING
		// Perform PBR update here
		// One for solids, one for textured
		auto itr = G_ECMANAGER->begin<ComponentGraphics>();
		auto itrEnd = G_ECMANAGER->end<ComponentGraphics>();
		while (itr != itrEnd)
		{
			ComponentGraphics* graphicsComp = reinterpret_cast<ComponentGraphics*>(*itr);

			Entity entity = G_ECMANAGER->getEntity(itr);
			
			ComponentAnimation* animComp = entity.getComponent<ComponentAnimation>();

			if (graphicsComp->_modelID < 0)
			{
				++itr;
				continue;
			}

			if (!graphicsComp->_isActive)
			{
				++itr;
				continue;
			}

			Model* model = modelManager->_models[graphicsComp->_modelID];

			// get transform component
			ComponentTransform* transformComp = G_ECMANAGER->getEntity(itr).getComponent<ComponentTransform>();

			glm::mat4 ModelMatrix = transformComp->GetModelMatrix();

			if (animComp)
			{
				if (animComp->_isActive)
				{
					glm::mat4 identity(1.0f);
					float dt = static_cast<float>(animManager->_animControllers[animComp->_controllerID]->_dt);
					std::string& currAnimation = animManager->_animControllers[animComp->_controllerID]->_currAnim;
					if (!currAnimation.empty())
					{
						model->GetPose(currAnimation, model->_rootBone, dt, identity, model->_globalInverseTransform);
					}
				}
			}

			if (graphicsComp->_renderType == RENDERTYPE::SOLID)
			{
				if (model->_isAnimated)
				{
					shaderManager->StartProgram(ShaderSystem::PBR_ANIMATED);
				}
				else
				{
					shaderManager->StartProgram(ShaderSystem::PBR); // solid program
				}

				// Update model and uniform for material
				glUniform3fv(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "Albedo"), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
				glUniform1f(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "Roughness"), graphicsComp->_pbrData._roughness);
				glUniform1f(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "Metallic"), graphicsComp->_pbrData._metallic);

				if (model->_isAnimated)
				{
					glUniformMatrix4fv(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "jointsMat"), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
					for (auto& mesh : model->_animatedMeshes)
					{
						glBindVertexArray(mesh->VAO);
						glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

						glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
					}
				}
				else
				{
					for (auto& mesh : model->_meshes)
					{
						glBindVertexArray(mesh->VAO);
						glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

						glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
					}
				}

				shaderManager->StopProgram();
			}
			else // textured program
			{
				if (model->_isAnimated)
				{
					shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
					//glm::mat4 identity(1.0f);
					//model->GetPose("Take 001", model->_rootBone, _testTimeElapsed, identity, model->_globalInverseTransform);
				}
				else
				{
					shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED); // textured program
				}
				

				// Roughness Control
				glUniform1f(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "RoughnessControl"), graphicsComp->_pbrData._roughness);
				glUniform1f(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "MetallicControl"), graphicsComp->_pbrData._metallic);

				// Bind textures
				// bind diffuse map
				textureManager->BindAlbedoTexture(graphicsComp->_albedoID);

				// bind metallic map
				textureManager->BindMetallicTexture(graphicsComp->_metallicID);

				// bind roughness map
				textureManager->BindRoughnessTexture(graphicsComp->_roughnessID);

				// bind ao map
				textureManager->BindAmbientOcclusionTexture(graphicsComp->_aoID);

				// bind normal map
				textureManager->BindNormalTexture(graphicsComp->_normalID);

				if (model->_isAnimated)
				{
					glUniformMatrix4fv(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "jointsMat"), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
					for (auto& mesh : model->_animatedMeshes)
					{
						glBindVertexArray(mesh->VAO);

						glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

						glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
					}
				}
				else
				{
					for (auto& mesh : model->_meshes)
					{
						glBindVertexArray(mesh->VAO);

						glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

						glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
					}
				}

				shaderManager->StopProgram();
			}
			itr++;
		}
#endif

#ifdef DRAW_DEBUG_GRID
		debugManager->Render();
#endif
	}

	void GraphicsSystem::ToggleDebugDraw(const bool& set)
	{
		_debugDrawing = set;
	}

	void GraphicsSystem::SetViewMatrix()
	{
		// Update to view matrix in shader program
		glBindBuffer(GL_UNIFORM_BUFFER, shaderManager->GetViewProjectionUniformLocation());
		glm::mat4 viewmatrix = cameraManager->GetViewMatrix();

		glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, glm::value_ptr(viewmatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GraphicsSystem::SetProjectionMatrix(const float& fov, const float& aspect_ratio, const float& near_plane, const float& far_plane)
	{
		_projectionMatrix = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);

		// Update only if changes are made
		glBindBuffer(GL_UNIFORM_BUFFER, shaderManager->GetViewProjectionUniformLocation());
		glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(_projectionMatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GraphicsSystem::SetUIMatrix(const int& width, const int& height, const float& near_plane, const float& far_plane)
	{
		shaderManager->StartProgram(ShaderSystem::UI);

		//Scales with screen or not?
		//float ratioWidth = (float)width / NS_WINDOW::SYS_WINDOW->GetAppWidth();
		//float ratioHeight = (float)height / NS_WINDOW::SYS_WINDOW->GetAppHeight();

		_orthoMatrix = glm::ortho(
			(float)-width * 0.5f,
			(float)width * 0.5f,
			(float)-height * 0.5f,
			(float)height * 0.5f,
			near_plane,
			far_plane);

		glUniformMatrix4fv(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "ortho_proj"), 1, GL_FALSE, &_orthoMatrix[0][0]);

		shaderManager->StopProgram();
	}

	void GraphicsSystem::UpdateLights()
	{
		// Update light components in light block(CPU)

		// Shader program must be started to fill in uniform data
		//shaderManager->
		(1);

		lightManager->Update();

		// Update view/camera position in light uniform block
		LightBlock* lightblock = lightManager->GetLightBlock();

		lightblock->_viewPos = glm::vec4(cameraManager->GetCurrentCameraPosition(), 1.f);

		// Update light uniform block(GPU)
		glBindBuffer(GL_UNIFORM_BUFFER, shaderManager->GetLightUniformLocation());
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlock), &*lightblock);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//shaderManager->StopProgram();
	}

	void GraphicsSystem::CreateCube(Entity& entity, const glm::vec3& rgb)
	{
		// Check if graphics component is already exists for obj
		// Assume it's a proper objID for now
		//if (G_ECMANAGER->getEntity(objID));

		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel("rotatedCube.fbx")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::CreateSphere(Entity& entity, const glm::vec3& rgb)
	{
		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel("sphere.fbx")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::CreateCylinder(Entity& entity, const glm::vec3& rgb)
	{
		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel("cylinder.fbx")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::SetLineThickness(const float& thickness)
	{
		glLineWidth(thickness);
	}

	void GraphicsSystem::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& rgb)
	{
		debugManager->AddLine(start, end, rgb);
	}

	void GraphicsSystem::SetMeshColor(Entity& entity, const glm::vec3& rgb)
	{
		ComponentGraphics* graphicsComp = entity.getComponent<ComponentGraphics>();

		if (graphicsComp == nullptr)
		{
#ifdef _DEBUG
			std::cout << "ERROR: Graphics component does not exist in given entity, failed to change mesh color" << std::endl;
#endif
			return;
		}

		// Set diffuse and ambient to selected rgb
		graphicsComp->_materialData._ambient = rgb;
		graphicsComp->_materialData._diffuse = rgb;

		// No need to perform glBufferSubData here, update() will perform communication with GPU data
	}
	void GraphicsSystem::LoadModel(const std::string& path)
	{
		modelLoader->LoadModel(path);
	}
	void GraphicsSystem::AttachModel(Entity& entity, const std::string& modelName)
	{
		if (entity.getComponent<ComponentGraphics>() == nullptr)
		{
			entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel(modelName)));
			entity.getComponent<ComponentGraphics>()->_modelFileName = modelName.c_str();
		}
		else
		{
			entity.getComponent<ComponentGraphics>()->_modelID = modelManager->AddModel(modelName);
			entity.getComponent<ComponentGraphics>()->_modelFileName = modelName.c_str();
		}
	}
	void GraphicsSystem::DetachMesh(Entity& entity)
	{
		if (entity.getComponent<ComponentGraphics>() != nullptr)
			entity.getComponent<ComponentGraphics>()->_modelID = -1;
	}

	glm::mat4 GraphicsSystem::GetInverseViewMatrix()
	{
		return (glm::inverse(cameraManager->GetViewMatrix()) * glm::inverse(_projectionMatrix));
	}
	void GraphicsSystem::SetHDRTexture(const std::string& filename)
	{
		_hdrID = textureManager->GetTexture(filename);
	}
}