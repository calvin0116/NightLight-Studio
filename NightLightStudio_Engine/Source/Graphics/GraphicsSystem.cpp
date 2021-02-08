#include "../Window/WndUtils.h"
#include "../Window/WndSystem.h"
#include "GraphicsSystem.h"

#include "../Component/Components.h" // Access to ECManager
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentGraphics.h"

#include "../glm/gtc/matrix_transform.hpp" // glm::perspective
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu

#include <fstream> //Temporary
#include "../Input/SystemInput.h"

#include <algorithm> // std::sort

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

//#define _DEFERRED_SHADING

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
		emitterManager->Update();

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
#ifdef _DEBUG
			std::cout << "ERROR: Window System not initialized before Graphics System, please check WndSystem initialization." << std::endl;
#endif
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
		emitterManager = &EmitterSystem::GetInstance();

		//modelLoader->Init();
		
		// Initialize sub systems and managers
		shaderManager->Init();

		// Set default values for view matrix
		// Set here to avoid weird bug at beginning
		SetViewMatrix();

		cameraManager->Init();

		lightManager->Init();

		uiManager->Init();

		emitterManager->Init();
		
		/*shaderManager->StartProgram(ShaderSystem::PBR);

		GLint blockSize;

		glGetActiveUniformBlockiv(shaderManager->GetCurrentProgramHandle(), 0,
			GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		std::cout << "Light Uniform Block Size: " << blockSize << std::endl;
		std::cout << "Light Uniform Block Size(CPU): " << sizeof(LightBlock) << std::endl;
		std::cout << "Directional Light Size(CPU): " << sizeof(DirLight) << std::endl;
		std::cout << "Point Light Size(CPU): " << sizeof(PointLight) << std::endl;
		std::cout << "Spot Light Size(CPU): " << sizeof(SpotLight) << std::endl;

		shaderManager->StopProgram();*/

		// Set default values for projection matrix
		SetProjectionMatrix();
		SetUIMatrix(NS_WINDOW::SYS_WINDOW->GetResolutionWidth(), NS_WINDOW::SYS_WINDOW->GetResolutionHeight());

		debugManager->Init();

		// Enable depth buffering, this prevents fragments behind another fragment to be rendered
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);

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

#ifdef _DEFERRED_SHADING
		// Set up frame buffers
		bool checkFBO = InitFrameBuffers();

#ifdef _DEBUG
		if (!checkFBO)
			std::cout << "ERROR: Unable to load frame buffer object (FN: NS_GRAPHICS::GraphicsSystem::InitFrameBuffers()" << std::endl;
#endif
		InitScreenQuad();
#endif
	}

	bool GraphicsSystem::InitFrameBuffers()
	{
		// Generate frame buffer object
		glGenFramebuffers(1, &_geometryBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _geometryBuffer);

		// Generate data buffers for geometry buffer + render targets

		// Position
		glGenTextures(1, &_rtPositionAlpha);
		glBindTexture(GL_TEXTURE_2D, _rtPositionAlpha);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _rtPositionAlpha, 0);

		// Calculated normals/normal map + metallic
		glGenTextures(1, &_rtNormalMapAndMetallic);
		glBindTexture(GL_TEXTURE_2D, _rtNormalMapAndMetallic);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _rtNormalMapAndMetallic, 0);
		
		// Albedo/Diffuse map + roughness
		glGenTextures(1, &_rtAlbedoMapAndRoughness);
		glBindTexture(GL_TEXTURE_2D, _rtAlbedoMapAndRoughness);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _rtAlbedoMapAndRoughness, 0);

		// Ambient Occlusion
		glGenTextures(1, &_rtAmbientOcclusion);
		glBindTexture(GL_TEXTURE_2D, _rtAmbientOcclusion);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _rtAmbientOcclusion, 0);

		// Tell opengl which color attachments to use for rendering
		unsigned int _renderTargets[4]
			= { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, _renderTargets);

		// Depth buffer
		glGenRenderbuffers(1, &_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return false;
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Frame buffers setup is complete, return true
		return true;
	}

	void GraphicsSystem::InitScreenQuad()
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &screenQuadVAO);
		glGenBuffers(1, &screenQuadVBO);
		glBindVertexArray(screenQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	void GraphicsSystem::Exit()
	{
		DestroyInstance();
	}

	void GraphicsSystem::GameExit()
	{
		// Call for lights clear upon scene exit
		lightManager->RemoveAllLights();
	}

	void GraphicsSystem::Render()
	{
		// testing basic gl functionality
		// Clears to gray bg
		glClearColor(0.03f, 0.086f, 0.102f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef PBR_DRAWING
		// Perform PBR update here
		// One for solids, one for textured
		auto itr = G_ECMANAGER->begin<ComponentGraphics>();
		auto itrEnd = G_ECMANAGER->end<ComponentGraphics>();

		// Steps for deferred shading
		/*
			1) Bind geometry buffer(GL_FRAMEBUFFER)
			2) Activate geometry pass shaders
			3) Render as per normal to geometry pass shaders, BUT WITHOUT LIGHTS
			4) Unbind geometry buffer/ set frame buffer to 0
			5) call glClear again
			6) Activate lighting pass shaders
			7) Activate and bind render targets to textures(glActiveTexture, glBindTexture)
			8) Set required uniforms and variables for light calculation
			9) Render 2D screen filled quad (to draw scene onto a texture, which will serve as our "screen/view")
			10) Bind geometry buffer again, this time with GL_READ_FRAMEBUFFER command
			11) Bind GL_DRAW_FRAMEBUFFER to 0, this will set write to default framebuffer
			12) 
			// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
			// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
			// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
			glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			13) Unbind framebuffer/ set frame buffer to 0
			14) **** MIGHT WANT TO RENDER UI AND SUCH AFTER INSTEAD

			// To deal with blending, perhaps turn off blending while rendering to fbo, then turn on again for transparent objects
		*/

		// To deal with transparent items
		//std::vector<ComponentGraphics*> _blended;
#ifdef _DEFERRED_SHADING
		///////////////////////////////////////
		// START OF DEFERRED SHADING

		// Frame buffer was initiallly set up, so no further initialization is required
		glBindFramebuffer(GL_FRAMEBUFFER, _geometryBuffer);

		// Must be done once again for frame buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate geometry pass shaders and render models (textured and non-textured)
		// This will write to geometry buffer first
		///////////////////////////////////////////
		// START OF DRAWING BASE MODELS

		while (itr != itrEnd)
		{
			ComponentGraphics* graphicsComp = G_ECMANAGER->getComponent<ComponentGraphics>(itr);

			/*if (graphicsComp->GetAlpha() < 1.f)
			{
				_blended.push_back(graphicsComp);
				++itr;
				continue;
			}*/


			Entity entity = G_ECMANAGER->getEntity(itr);

			ComponentAnimation* animComp = entity.getComponent<ComponentAnimation>();

			if (graphicsComp->_modelID < 0)
			{
				++itr;
				//++compItr;
				continue;
			}

			if (!graphicsComp->_isActive)
			{
				++itr;
				//++compItr;
				continue;
			}

			Model* model = modelManager->_models[graphicsComp->_modelID];

			// get transform component
			ComponentTransform* transformComp = entity.getComponent<ComponentTransform>();

			glm::mat4 ModelMatrix = transformComp->GetModelMatrix();

			if (animComp)
			{
				if (animComp->_isActive && model->_isAnimated)
				{
					if (animManager->_animControllers[animComp->_controllerID]->_play)
					{
						glm::mat4 identity(1.0f);
						double dt = animManager->_animControllers[animComp->_controllerID]->_dt;
						std::string& currAnimation = animManager->_animControllers[animComp->_controllerID]->_currAnim;
						if (!currAnimation.empty())
						{
							model->GetPose(currAnimation, model->_rootBone->_rootJoint, dt, identity, model->_globalInverseTransform);
						}
					}
					else
					{
						std::string& defaultAnimation = animManager->_animControllers[animComp->_controllerID]->_defaultAnim;

						if (!defaultAnimation.empty())
						{
							animManager->_animControllers[animComp->_controllerID]->PlayAnimation(defaultAnimation, animComp);

							glm::mat4 identity(1.0f);
							double dt = animManager->_animControllers[animComp->_controllerID]->_dt;
							model->GetPose(defaultAnimation, model->_rootBone->_rootJoint, dt, identity, model->_globalInverseTransform);
						}
					}
				}

				// Test Codes
				//if (entity.getComponent<ComponentTransform>()->_entityName.toString() == "Player")
				//{
				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_K))
				//	{
				//		graphicsComp->AddModel("player_character_animationsworks.fbx");
				//	}
				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_L))
				//	{
				//		graphicsComp->AddModel("New_Moth_B4.fbx");
				//	}
				//}

				//if (entity.getComponent<ComponentTransform>()->_entityName.toString() == "Player")
				//{
				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_J))
				//	{
				//		animComp->PlayAnimation("Switch1");
				//	}

				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_K))
				//	{
				//		animComp->PlayAnimation("Walk1", true, 1.9, 3.108);
				//	}

				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_L))
				//	{
				//		animComp->PlayAnimation("Walk1");
				//	}
				//}
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

				// Update alpha
				glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

				// Update model and uniform for material
				glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
				glUniform1f(shaderManager->GetRoughnessLocation(), graphicsComp->_pbrData._roughness);
				glUniform1f(shaderManager->GetMetallicLocation(), graphicsComp->_pbrData._metallic);

				if (model->_isAnimated)
				{
					glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
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
					// If normal map does not exist
					if (!graphicsComp->_normalID)
						shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED_NONORMALMAP);
					else
						shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
					//glm::mat4 identity(1.0f);
					//model->GetPose("Take 001", model->_rootBone, _testTimeElapsed, identity, model->_globalInverseTransform);
				}
				else
				{
					if (!graphicsComp->_normalID)
						shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_NONORMALMAP);
					else
						shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED); // textured program
				}

				// Update alpha
				glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

				// Roughness Control
				glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);
				glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

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
					glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
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
			++itr;
		}

		// END OF DRAWING BASE MODELS
		///////////////////////////////////////////

		// Unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate lighting pass shaders
		shaderManager->StartProgram(ShaderSystem::PBR_LIGHTPASS);

		// Set up processed G-buffer for light calculation
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _rtPositionAlpha);
		glUniform1i(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "gPositionAlpha"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _rtNormalMapAndMetallic);
		glUniform1i(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "gNormalMetallic"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _rtAlbedoMapAndRoughness);
		glUniform1i(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "gAlbedoRoughness"), 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, _rtAmbientOcclusion);
		glUniform1i(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "gAmbientOcclusion"), 3);

		// Render to a 2D screen quad for our view
		RenderScreenQuad();

		// Bind geometry buffer again, this time with GL_READ_FRAMEBUFFER command
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _geometryBuffer);

		// Bind GL_DRAW_FRAMEBUFFER to 0, this will set write to default framebuffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

#ifdef _DEBUG
		// Setting for GIT so that it doesn't look weird
		// Copies info(in this case, depth data)to a user-defined region of read framebuffer to user-defined region of draw framebuffer
		glBlitFramebuffer(0, 0, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height, 0, 0, CONFIG_DATA->GetConfigData().width * 2, static_cast<int>(static_cast<float>(CONFIG_DATA->GetConfigData().height) * 1.9f), GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
#endif

#ifndef _DEBUG
		// Setting for SVN so it doesn't look weird
		glBlitFramebuffer(0, 0, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height, 0, 0, CONFIG_DATA->GetConfigData().width, CONFIG_DATA->GetConfigData().height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
#endif
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Then render other non-light affected stuff as per normal
		// In here, we try to render transparent objects
		// Enable glBlend here and disable afterwards
		// Requires use of old shaders to render
		// In other words, make way for older shaders to utilize blending as we are no longer in the lighting pass

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//auto blendedItr = _blended.begin();
		//auto blendedItrEnd = _blended.end();

		//// Render objects with transparency
		//while (blendedItr != blendedItrEnd)
		//{
		//	ComponentGraphics* graphicsComp = *blendedItr;

		//	Entity entity = G_ECMANAGER->getEntity(graphicsComp);

		//	ComponentAnimation* animComp = entity.getComponent<ComponentAnimation>();

		//	if (graphicsComp->_modelID < 0)
		//	{
		//		++blendedItr;
		//		continue;
		//	}

		//	if (!graphicsComp->_isActive)
		//	{
		//		++blendedItr;
		//		continue;
		//	}

		//	Model* model = modelManager->_models[graphicsComp->_modelID];

		//	// get transform component
		//	ComponentTransform* transformComp = entity.getComponent<ComponentTransform>();

		//	glm::mat4 ModelMatrix = transformComp->GetModelMatrix();

		//	if (animComp)
		//	{
		//		if (animComp->_isActive && model->_isAnimated)
		//		{
		//			glm::mat4 identity(1.0f);
		//			double dt = animManager->_animControllers[animComp->_controllerID]->_dt;
		//			std::string& currAnimation = animManager->_animControllers[animComp->_controllerID]->_currAnim;
		//			if (!currAnimation.empty())
		//			{
		//				model->GetPose(currAnimation, model->_rootBone->_rootJoint, dt, identity, model->_globalInverseTransform);
		//			}
		//		}
		//	}

		//	if (graphicsComp->_renderType == RENDERTYPE::SOLID)
		//	{
		//		if (model->_isAnimated)
		//		{
		//			shaderManager->StartProgram(ShaderSystem::PBR_ANIMATED);
		//		}
		//		else
		//		{
		//			shaderManager->StartProgram(ShaderSystem::PBR); // solid program
		//		}

		//		// Update alpha
		//		glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

		//		// Update model and uniform for material
		//		glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
		//		glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);
		//		glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

		//		if (model->_isAnimated)
		//		{
		//			glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
		//			for (auto& mesh : model->_animatedMeshes)
		//			{
		//				glBindVertexArray(mesh->VAO);
		//				glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
		//				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

		//				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
		//			}
		//		}
		//		else
		//		{
		//			for (auto& mesh : model->_meshes)
		//			{
		//				glBindVertexArray(mesh->VAO);
		//				glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
		//				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

		//				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
		//			}
		//		}

		//		shaderManager->StopProgram();
		//	}
		//	else // textured program
		//	{
		//		if (model->_isAnimated)
		//		{
		//			// If normal map does not exist
		//			if (!graphicsComp->_normalID)
		//				shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED_NONORMALMAP);
		//			else
		//				shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
		//			//glm::mat4 identity(1.0f);
		//			//model->GetPose("Take 001", model->_rootBone, _testTimeElapsed, identity, model->_globalInverseTransform);
		//		}
		//		else
		//		{
		//			if (!graphicsComp->_normalID)
		//				shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_NONORMALMAP);
		//			else
		//				shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED); // textured program
		//		}

		//		// Update alpha
		//		glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

		//		// Roughness Control
		//		glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);
		//		glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

		//		// Bind textures
		//		// bind diffuse map
		//		textureManager->BindAlbedoTexture(graphicsComp->_albedoID);

		//		// bind metallic map
		//		textureManager->BindMetallicTexture(graphicsComp->_metallicID);

		//		// bind roughness map
		//		textureManager->BindRoughnessTexture(graphicsComp->_roughnessID);

		//		// bind ao map
		//		textureManager->BindAmbientOcclusionTexture(graphicsComp->_aoID);

		//		// bind normal map
		//		textureManager->BindNormalTexture(graphicsComp->_normalID);

		//		if (model->_isAnimated)
		//		{
		//			glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
		//			for (auto& mesh : model->_animatedMeshes)
		//			{
		//				glBindVertexArray(mesh->VAO);

		//				glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
		//				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

		//				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
		//			}
		//		}
		//		else
		//		{
		//			for (auto& mesh : model->_meshes)
		//			{
		//				glBindVertexArray(mesh->VAO);

		//				glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
		//				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

		//				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->_indices.size()), GL_UNSIGNED_INT, 0);
		//			}
		//		}

		//		shaderManager->StopProgram();
		//	}
		//	++blendedItr;
		//}

		//glDisable(GL_BLEND);


#ifdef DRAW_DEBUG_GRID
		debugManager->Render(); // Render opaque objects first, in this case, our grid is most definitely opaque
#endif
		// END OF DEFERRED SHADING
		///////////////////////////////////////
#endif

#ifndef _DEFERRED_SHADING

#ifdef DRAW_DEBUG_GRID
		debugManager->Render(); // Render opaque objects first, in this case, our grid is most definitely opaque
#endif
		// Extra "queue" for blended objects
		std::vector<ComponentGraphics*> _blended;

		//while(compItr != compitrEnd)
		while (itr != itrEnd)
		{
			ComponentGraphics* graphicsComp = G_ECMANAGER->getComponent<ComponentGraphics>(itr);
			//ComponentGraphics* graphicsComp = *compItr;

			if (graphicsComp->GetAlpha() < 1.f)
			{
				_blended.push_back(graphicsComp);
				++itr;
				continue;
			}


			Entity entity = G_ECMANAGER->getEntity(itr);
			//Entity entity = G_ECMANAGER->getEntity(graphicsComp);
			
			ComponentAnimation* animComp = entity.getComponent<ComponentAnimation>();

			if (graphicsComp->_modelID < 0)
			{
				++itr;
				//++compItr;
				continue;
			}

			if (!graphicsComp->_isActive)
			{
				++itr;
				//++compItr;
				continue;
			}

			Model* model = modelManager->_models[graphicsComp->_modelID];

			// get transform component
			ComponentTransform* transformComp = entity.getComponent<ComponentTransform>();

			glm::mat4 ModelMatrix = transformComp->GetModelMatrix();

			if (animComp)
			{
				if (animComp->_isActive && model->_isAnimated)
				{
					if (animManager->_animControllers[animComp->_controllerID]->_play)
					{
						glm::mat4 identity(1.0f);
						double dt = animManager->_animControllers[animComp->_controllerID]->_dt;
						std::string& currAnimation = animManager->_animControllers[animComp->_controllerID]->_currAnim;
						if (!currAnimation.empty())
						{
							model->GetPose(currAnimation, model->_rootBone->_rootJoint, dt, identity, model->_globalInverseTransform);
						}
					}
					else
					{
						std::string& defaultAnimation = animManager->_animControllers[animComp->_controllerID]->_defaultAnim;					
						
						if (!defaultAnimation.empty())
						{
							animManager->_animControllers[animComp->_controllerID]->PlayAnimation(defaultAnimation, animComp);

							glm::mat4 identity(1.0f);
							double dt = animManager->_animControllers[animComp->_controllerID]->_dt;
							model->GetPose(defaultAnimation, model->_rootBone->_rootJoint, dt, identity, model->_globalInverseTransform);
						}
					}
				}

				// Test Codes
				//if (entity.getComponent<ComponentTransform>()->_entityName.toString() == "Player")
				//{
				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_K))
				//	{
				//		graphicsComp->AddModel("player_character_animationsworks.fbx");
				//	}
				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_L))
				//	{
				//		graphicsComp->AddModel("New_Moth_B4.fbx");
				//	}
				//}

				//if (entity.getComponent<ComponentTransform>()->_entityName.toString() == "Player")
				//{
				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_J))
				//	{
				//		animComp->PlayAnimation("Switch1");
				//	}

				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_K))
				//	{
				//		animComp->PlayAnimation("Walk1", true, 1.9, 3.108);
				//	}

				//	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IKEY_L))
				//	{
				//		animComp->PlayAnimation("Walk1");
				//	}
				//}
			}

			if (graphicsComp->_renderType == RENDERTYPE::SOLID)
			{
				if (model->_isAnimated)
				{
					// Check if emission material is activated
					if (graphicsComp->CheckEmissiveActivation())
					{
						// Deactivate backface culling

						shaderManager->StartProgram(ShaderSystem::EMISSIVE_ANIMATED);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo

					}
					else
					{
						shaderManager->StartProgram(ShaderSystem::PBR_ANIMATED);

						// Get uniforms done only if non-emissive shaders are activated
						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
						glUniform1f(shaderManager->GetRoughnessLocation(), graphicsComp->_pbrData._roughness);
						glUniform1f(shaderManager->GetMetallicLocation(), graphicsComp->_pbrData._metallic);
					}
				}
				else
				{
					if (graphicsComp->CheckEmissiveActivation())
					{
						shaderManager->StartProgram(ShaderSystem::EMISSIVE);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
					}
					else
					{
						shaderManager->StartProgram(ShaderSystem::PBR); // solid program

						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
						glUniform1f(shaderManager->GetRoughnessLocation(), graphicsComp->_pbrData._roughness);
						glUniform1f(shaderManager->GetMetallicLocation(), graphicsComp->_pbrData._metallic);
					}
				}

				// Update alpha
				glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

				// Update model and uniform for material
				//glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
				//glUniform1f(shaderManager->GetRoughnessLocation(), graphicsComp->_pbrData._roughness);
				//glUniform1f(shaderManager->GetMetallicLocation(), graphicsComp->_pbrData._metallic);

				if (model->_isAnimated)
				{
					glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
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
					// Check if emission material is activated
					if (graphicsComp->CheckEmissiveActivation())
					{
						shaderManager->StartProgram(ShaderSystem::EMISSIVE_ANIMATED_TEXTURED);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						textureManager->BindAlbedoTexture(graphicsComp->_albedoID);
					}
					else
					{
						// If normal map does not exist
						if (!graphicsComp->_normalID)
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED_NONORMALMAP);
						else
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
						//glm::mat4 identity(1.0f);
						//model->GetPose("Take 001", model->_rootBone, _testTimeElapsed, identity, model->_globalInverseTransform);

						// Get uniforms done only if non-emissive shaders are activated
						// Roughness Control
						glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);

						// Metallic Control
						glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

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
					}
				}
				else
				{
					// Check if emission material is activated
					if (graphicsComp->CheckEmissiveActivation())
					{
						shaderManager->StartProgram(ShaderSystem::EMISSIVE_TEXTURED);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						textureManager->BindAlbedoTexture(graphicsComp->_albedoID);
					}
					else
					{
						if (!graphicsComp->_normalID)
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_NONORMALMAP);
						else
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED); // textured program

						// Get uniforms done only if non-emissive shaders are activated
						// Roughness Control
						glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);

						// Metallic Control
						glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

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
					}
				}
				
				// Update alpha
				glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

				//// Roughness Control
				//glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);

				//// Metallic Control
				//glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

				//// Bind textures
				//// bind diffuse map
				//textureManager->BindAlbedoTexture(graphicsComp->_albedoID);

				//// bind metallic map
				//textureManager->BindMetallicTexture(graphicsComp->_metallicID);

				//// bind roughness map
				//textureManager->BindRoughnessTexture(graphicsComp->_roughnessID);

				//// bind ao map
				//textureManager->BindAmbientOcclusionTexture(graphicsComp->_aoID);

				//// bind normal map
				//textureManager->BindNormalTexture(graphicsComp->_normalID);

				if (model->_isAnimated)
				{
					glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
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
			++itr;
			//++compItr;
		}

		// Iterators for alpha blending objects
		auto blendedItr = _blended.begin();
		auto blendedItrEnd = _blended.end();

		// Render objects with transparency
		while (blendedItr != blendedItrEnd)
		{
			ComponentGraphics* graphicsComp = *blendedItr;

			Entity entity = G_ECMANAGER->getEntity(graphicsComp);

			ComponentAnimation* animComp = entity.getComponent<ComponentAnimation>();

			if (graphicsComp->_modelID < 0)
			{
				++blendedItr;
				continue;
			}

			if (!graphicsComp->_isActive)
			{
				++blendedItr;
				continue;
			}

			Model* model = modelManager->_models[graphicsComp->_modelID];

			// get transform component
			ComponentTransform* transformComp = entity.getComponent<ComponentTransform>();

			glm::mat4 ModelMatrix = transformComp->GetModelMatrix();

			if (animComp)
			{
				if (animComp->_isActive && model->_isAnimated)
				{
					glm::mat4 identity(1.0f);
					double dt = animManager->_animControllers[animComp->_controllerID]->_dt;
					std::string& currAnimation = animManager->_animControllers[animComp->_controllerID]->_currAnim;
					if (!currAnimation.empty())
					{
						model->GetPose(currAnimation, model->_rootBone->_rootJoint, dt, identity, model->_globalInverseTransform);
					}
				}
			}

			if (graphicsComp->_renderType == RENDERTYPE::SOLID)
			{
				if (model->_isAnimated)
				{
					// Check if emission material is activated
					if (graphicsComp->CheckEmissiveActivation())
					{
						shaderManager->StartProgram(ShaderSystem::EMISSIVE_ANIMATED);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
					}
					else
					{
						shaderManager->StartProgram(ShaderSystem::PBR_ANIMATED);

						// Get uniforms done only if non-emissive shaders are activated
						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
						glUniform1f(shaderManager->GetRoughnessLocation(), graphicsComp->_pbrData._roughness);
						glUniform1f(shaderManager->GetMetallicLocation(), graphicsComp->_pbrData._metallic);
					}
				}
				else
				{
					if (graphicsComp->CheckEmissiveActivation())
					{
						shaderManager->StartProgram(ShaderSystem::EMISSIVE);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
					}
					else
					{
						shaderManager->StartProgram(ShaderSystem::PBR); // solid program

						glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
						glUniform1f(shaderManager->GetRoughnessLocation(), graphicsComp->_pbrData._roughness);
						glUniform1f(shaderManager->GetMetallicLocation(), graphicsComp->_pbrData._metallic);
					}
				}

				// Update alpha
				glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

				//// Update model and uniform for material
				//glUniform3fv(shaderManager->GetAlbedoLocation(), 1, &graphicsComp->_pbrData._albedo[0]); // albedo
				//glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);
				//glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

				if (model->_isAnimated)
				{
					glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
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
					// Check if emission material is activated
					if (graphicsComp->CheckEmissiveActivation())
					{
						shaderManager->StartProgram(ShaderSystem::EMISSIVE_ANIMATED_TEXTURED);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						textureManager->BindAlbedoTexture(graphicsComp->_albedoID);
					}
					else
					{
						// If normal map does not exist
						if (!graphicsComp->_normalID)
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED_NONORMALMAP);
						else
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_ANIMATED);
						//glm::mat4 identity(1.0f);
						//model->GetPose("Take 001", model->_rootBone, _testTimeElapsed, identity, model->_globalInverseTransform);

						// Get uniforms done only if non-emissive shaders are activated
						// Roughness Control
						glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);

						// Metallic Control
						glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

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
					}
				}
				else
				{
					// Check if emission material is activated
					if (graphicsComp->CheckEmissiveActivation())
					{
						shaderManager->StartProgram(ShaderSystem::EMISSIVE_TEXTURED);
						glUniform3fv(shaderManager->GetEmissiveLocation(), 1, &graphicsComp->_pbrData._emissive[0]); // emissive
						glUniform1f(shaderManager->GetEmissiveIntensityLocation(), graphicsComp->_pbrData._emissiveIntensity);

						textureManager->BindAlbedoTexture(graphicsComp->_albedoID);
					}
					else
					{
						if (!graphicsComp->_normalID)
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED_NONORMALMAP);
						else
							shaderManager->StartProgram(ShaderSystem::PBR_TEXTURED); // textured program

						// Get uniforms done only if non-emissive shaders are activated
						// Roughness Control
						glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);

						// Metallic Control
						glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

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
					}
				}

				// Update alpha
				glUniform1f(shaderManager->GetAlphaLocation(), graphicsComp->GetAlpha());

				//// Roughness Control
				//glUniform1f(shaderManager->GetRoughnessControlLocation(), graphicsComp->_pbrData._roughness);
				//glUniform1f(shaderManager->GetMetallicControlLocation(), graphicsComp->_pbrData._metallic);

				//// Bind textures
				//// bind diffuse map
				//textureManager->BindAlbedoTexture(graphicsComp->_albedoID);

				//// bind metallic map
				//textureManager->BindMetallicTexture(graphicsComp->_metallicID);

				//// bind roughness map
				//textureManager->BindRoughnessTexture(graphicsComp->_roughnessID);

				//// bind ao map
				//textureManager->BindAmbientOcclusionTexture(graphicsComp->_aoID);

				//// bind normal map
				//textureManager->BindNormalTexture(graphicsComp->_normalID);

				if (model->_isAnimated)
				{
					glUniformMatrix4fv(shaderManager->GetJointsMatrixLocation(), MAX_BONE_COUNT, GL_FALSE, glm::value_ptr(model->_poseTransform[0]));
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
			++blendedItr;
		}
#endif

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
		shaderManager->StartProgram(ShaderSystem::UI_SCREENSPACE);

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

		glUniformMatrix4fv(shaderManager->GetOrthoProjUniformLocation(), 1, GL_FALSE, &_orthoMatrix[0][0]);

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

	void GraphicsSystem::DrawXYCircle(const glm::vec3& center, const float& radius, const int& segments, const glm::vec3& rgb)
	{
		if (segments <= 3)
			return;

		float pi2 = 2.f * PI;
		
		for (int i = 0; i < segments; i++)
		{
			float theta0 = pi2 * static_cast<float>(i) / static_cast<float>(segments);     // p0
			float theta1 = pi2 * static_cast<float>(i + 1) / static_cast<float>(segments); // p1

			float x0 = radius * cosf(theta0) + center.x;
			float y0 = radius * sinf(theta0) + center.y;

			float x1 = radius * cosf(theta1) + center.x;
			float y1 = radius * sinf(theta1) + center.y;

			DrawLine(glm::vec3(x0, y0, center.z), glm::vec3(x1, y1, center.z), rgb);
		}
	}

	void GraphicsSystem::DrawXZCircle(const glm::vec3& center, const float& radius, const int& segments, const glm::vec3& rgb)
	{
		if (segments <= 3)
			return;

		float pi2 = 2.f * PI;

		for (int i = 0; i < segments; i++)
		{
			float theta0 = pi2 * static_cast<float>(i) / static_cast<float>(segments);     // p0
			float theta1 = pi2 * static_cast<float>(i + 1) / static_cast<float>(segments); // p1

			float x0 = radius * cosf(theta0) + center.x;
			float z0 = radius * sinf(theta0) + center.z;

			float x1 = radius * cosf(theta1) + center.x;
			float z1 = radius * sinf(theta1) + center.z;

			DrawLine(glm::vec3(x0, center.y, z0), glm::vec3(x1, center.y, z1), rgb);
		}
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
	void GraphicsSystem::RenderScreenQuad()
	{
		glBindVertexArray(screenQuadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}