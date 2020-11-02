#include "../Window/WndUtils.h"
#include "../Window/WndSystem.h"
#include "GraphicsSystem.h"

#include "../Component/Components.h" // Access to ECManager
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentGraphics.h"

#include "../glm/gtc/matrix_transform.hpp" // glm::perspective
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu

// Comment away for whichever is required
// Only either one should be running at a time

#include <fstream> //Temporary

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

//#define DRAW_WITH_COMPONENTS
#define DRAW_WITH_LIGHTS
#define DRAW_DEBUG_GRID

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
		_hasInit{ false },
		_debugDrawing{ false },
		_projectionMatrix{ glm::mat4(1.0f) },
		_viewMatrix{ glm::mat4(1.0f) }
	{
	}

	GraphicsSystem::~GraphicsSystem()
	{
	}

	void GraphicsSystem::Update()
	{
		cameraManager->Update();

		if (cameraManager->CheckUpdate())
		{
			SetViewMatrix();
			cameraManager->AckUpdate();
		}

		// Update lights
		UpdateLights();

		Render();
	}

	void GraphicsSystem::Free()
	{
		// Free all OpenGL objects before exit
		// Includes VAO, VBO, EBO, ModelMatrixBO
		modelManager->Free();
		textureManager->Free();
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

		modelLoader->Init();
		
		// Initialize sub systems and managers
		shaderManager->Init();

		// Set default values for view matrix
		// Set here to avoid weird bug at beginning
		SetViewMatrix();

		cameraManager->Init();

		lightManager->Init();
		
		////////////////////////////////////////////////////
		/// Commented to test level editor drag and drop
		
		modelLoader->LoadModel("rotatedCube.fbx", "box");

		//Draw Cylinder FBX file 
		modelLoader->LoadModel("cylinder.fbx", "cylinder");
		
		//Draw sphere FBX file 
		modelLoader->LoadModel("sphere.fbx", "sphere");

		modelLoader->LoadModel("incense_pot_Model.fbx", "pot");

		textureManager->GetTexture("Test.jpg");
		

		//std::cout << "light block size: " <<  sizeof(LightBlock) << std::endl;

		//////////////////////////////////////
		/// TEST DRAW LOADED MODELS
		//////////////////////////////////////
		/*Entity testdrawSphere = G_ECMANAGER->BuildEntity();
		ComponentTransform testtransformsphere;
		testtransformsphere._position = { 2.f, 0.f,0.f };
		testdrawSphere.AttachComponent<ComponentTransform>(testtransformsphere);

		CreateSphere(testdrawSphere, glm::vec3(0.f,1.f,1.f));*/

		//modelLoader->LoadModel(".\\incense_pot_model_custom.obj", "pot");

		//Entity testDrawIncense = G_ECMANAGER->BuildEntity();
		//ComponentTransform testtransformIncense;
		//testtransformIncense._position = { 10.f, 0.f,0.f };
		//testDrawIncense.AttachComponent<ComponentTransform>(testtransformIncense);
		//testDrawIncense.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel("pot")));

		//Entity testDrawIncense2 = G_ECMANAGER->BuildEntity();
		//ComponentTransform testtransformIncense2;
		//testtransformIncense2._position = { 3.f, 0.f,0.f };
		//testDrawIncense2.AttachComponent<ComponentTransform>(testtransformIncense2);
		//testDrawIncense2.AttachComponent<ComponentGraphics>(ComponentGraphics(meshManager->AddMesh("pot2")));

		//Entity testDrawIncense3 = G_ECMANAGER->BuildEntity();
		//ComponentTransform testtransformIncense3;
		//testtransformIncense3._position = { 3.f, 0.f,0.f };
		//testDrawIncense3.AttachComponent<ComponentTransform>(testtransformIncense3);
		//testDrawIncense3.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddMesh("pot3")));

		//Entity testDrawIncense4 = G_ECMANAGER->BuildEntity();
		//ComponentTransform testtransformIncense4;
		//testtransformIncense4._position = { 3.f, 0.f,0.f };
		//testDrawIncense4.AttachComponent<ComponentTransform>(testtransformIncense4);
		//testDrawIncense4.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddMesh("pot4")));

		/*Entity testdrawCube = G_ECMANAGER->BuildEntity();
		ComponentTransform testtransformcube;
		testtransformcube._position = { 10.f, 0.f,0.f };
		testtransformcube._scale = { 10.f, 10.f,10.f };
		testdrawCube.AttachComponent<ComponentTransform>(testtransformcube);

		CreateCube(testdrawCube, glm::vec3(0.f, 1.f, 1.f));

		Entity testdrawCylinder = G_ECMANAGER->BuildEntity();
		ComponentTransform testtransformcylinder;
		testtransformcylinder._position = { -2.f, 0.f,0.f };
		testtransformcylinder._rotation = { 0.f, 0.f,0.f };
		testdrawCylinder.AttachComponent<ComponentTransform>(testtransformcylinder);

		CreateCylinder(testdrawCylinder, glm::vec3(0.f, 1.f, 1.f));*/

		shaderManager->StartProgram(1);

		GLint blockSize;

		glGetActiveUniformBlockiv(shaderManager->GetCurrentProgramHandle(), 1,
			GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		std::cout << "Light Uniform Block Size: " << blockSize << std::endl;
		std::cout << "Light Uniform Block Size(CPU): " << sizeof(LightBlock) << std::endl;
		std::cout << "Directional Light Size(CPU): " << sizeof(DirLight) << std::endl;
		std::cout << "Spot Light Size(CPU): " << sizeof(SpotLight) << std::endl;
		std::cout << "Point Light Size(CPU): " << sizeof(PointLight) << std::endl;

		shaderManager->StopProgram();

		//std::ofstream logFile;
		//logFile.open("custom.txt");

		//modelLoader->LoadModel("incense_pot_Model.fbx", "incense_pot_model");

		//int lineCount = 0;
		//int MeshSize = modelManager->_modelList["incense_pot_model"]->_meshes.size();
		//for (int i=0; i < MeshSize; ++i)
		//{
		//	int verticeSize = modelManager->_modelList["incense_pot_model"]->_meshes[i]->_vertices.size();
		//	for (int x = 0; x < verticeSize; ++x)
		//	{
		//		logFile << "Vertex: X: " << modelManager->_modelList["incense_pot_model"]->_meshes[i]->_vertices[x].x << " Y: " <<
		//			modelManager->_modelList["incense_pot_model"]->_meshes[i]->_vertices[x].y << " Z: " <<
		//			modelManager->_modelList["incense_pot_model"]->_meshes[i]->_vertices[x].z << "\n";

		//		lineCount++;
		//	}

		//	logFile << std::endl;
		//}

		//logFile << lineCount << std::endl;

		//logFile.close();

		////TEST VECTOR MAX SIZE
		//std::cout << "TEST MAX SIZE: " << vectTest.max_size() << std::endl;

		////////////////////////
		/// Text Texture Loader
		////////////////////////
		//textureLoader->LoadTexture("Test.jpg");

		// Set default values for projection matrix
		SetProjectionMatrix();

		debugManager->Init();

		// Enable depth buffering
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Passes if the fragment's depth value is less than the stored depth value.
		// This is the default, but we will call this function to be explicit
		glDepthFunc(GL_LESS);

		SetLineThickness();
	}

	void GraphicsSystem::Exit()
	{
		DestroyInstance();
	}

	void GraphicsSystem::Render()
	{
		// testing basic gl functionality
		// Clears to gray bg
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef DRAW_WITH_COMPONENTS
		shaderManager->StartProgram(0);

		auto itr = G_ECMANAGER->begin<ComponentGraphics>();
		auto itrEnd = G_ECMANAGER->end<ComponentGraphics>();
		while (itr != itrEnd)
		{
			ComponentGraphics* graphicsComp = reinterpret_cast<ComponentGraphics*>(*itr);

			Mesh* mesh = modelManager->meshes[graphicsComp->MeshID];

			// get transform component
			ComponentTransform* transformComp = G_ECMANAGER->getEntity(itr).getComponent<ComponentTransform>();

			glm::mat4 ModelMatrix = transformComp->GetModelMatrix();

			glBindVertexArray(mesh->VAO);

			// We will only substitute Color and ModelMatrix Data
			glBindBuffer(GL_ARRAY_BUFFER, mesh->CBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * mesh->_rgb.size(), &mesh->_rgb[0]);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

			glDrawElements(GL_TRIANGLES, (unsigned)mesh->_indices.size(), GL_UNSIGNED_SHORT, 0);

			itr++;
		}

		shaderManager->StopProgram();
#endif

#ifdef DRAW_WITH_LIGHTS
		shaderManager->StartProgram(1);

		auto itr = G_ECMANAGER->begin<ComponentGraphics>();
		auto itrEnd = G_ECMANAGER->end<ComponentGraphics>();
		while (itr != itrEnd)
		{
			ComponentGraphics* graphicsComp = reinterpret_cast<ComponentGraphics*>(*itr);

			Model* model = modelManager->_models[graphicsComp->MeshID];

			// get transform component
			ComponentTransform* transformComp = G_ECMANAGER->getEntity(itr).getComponent<ComponentTransform>();

			glm::mat4 ModelMatrix = transformComp->GetModelMatrix();
			
			for (auto& mesh : model->_meshes)
			{
				glBindVertexArray(mesh->VAO);

				// Update model and uniform for material
				glUniform3fv(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "ambient"), 1, &graphicsComp->_materialData._ambient[0]); // ambient
				glUniform3fv(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "diffuse"), 1, &graphicsComp->_materialData._diffuse[0]); // diffuse
				glUniform3fv(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "specular"), 1, &graphicsComp->_materialData._specular[0]); // specular
				glUniform1f(glGetUniformLocation(shaderManager->GetCurrentProgramHandle(), "shininess"), graphicsComp->_materialData._shininess);

				glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

				glDrawElements(GL_TRIANGLES, (unsigned)mesh->_indices.size(), GL_UNSIGNED_SHORT, 0);
			}
			itr++;
		}

		shaderManager->StopProgram();
#endif

#ifdef DRAW_DEBUG_GRID
		SetLineThickness(0.2f);
		debugManager->Render();
		SetLineThickness();
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

	void GraphicsSystem::UpdateLights()
	{
		// Update light uniform block(CPU)
		// Update view/camera position in light uniform block
		LightBlock* lightblock = lightManager->GetLightBlock();

		lightblock->_viewPos = cameraManager->GetCurrentCameraPosition();

		glBindBuffer(GL_UNIFORM_BUFFER, shaderManager->GetLightUniformLocation());
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlock), &*lightblock);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GraphicsSystem::CreateCube(Entity& entity, const glm::vec3& rgb)
	{
		// Check if graphics component is already exists for obj
		// Assume it's a proper objID for now
		//if (G_ECMANAGER->getEntity(objID));

		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel("box")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::CreateSphere(Entity& entity, const glm::vec3& rgb)
	{
		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel("sphere")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::CreateCylinder(Entity& entity, const glm::vec3& rgb)
	{
		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel("cylinder")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::SetLineThickness(const float& thickness)
	{
		glLineWidth(thickness);
	}

	void GraphicsSystem::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& rgb)
	{
		shaderManager->StartProgram(0);

		GLuint VAO = NULL;
		GLuint VBO = NULL;
		GLuint CBO = NULL;
		GLuint UVBO = NULL;
		GLuint ModelMatrixBO = NULL;

		std::vector<glm::vec3> _vertices;
		std::vector<glm::vec2> _uv;
		std::vector<glm::vec3> _rgb;

		_vertices.push_back(start);
		_vertices.push_back(end);

		_uv.emplace_back(1.f, 0.f);
		_uv.emplace_back(1.f, 0.f);

		_rgb.push_back(rgb);
		_rgb.push_back(rgb);

		glm::mat4 modelmatrix(1.f);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &CBO);
		glGenBuffers(1, &UVBO);
		glGenBuffers(1, &ModelMatrixBO);

		glBindVertexArray(VAO);

		// pos attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);


		// uv attribute
		glBindBuffer(GL_ARRAY_BUFFER, UVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _uv.size(), &_uv[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(1);

		// color attribute
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _rgb.size(), &_rgb[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(2);

		// Model Matrix
		glBindBuffer(GL_ARRAY_BUFFER, ModelMatrixBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelmatrix, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glDrawArrays(GL_LINES, 0, 2);

		glDeleteBuffers(1, &ModelMatrixBO);
		glDeleteBuffers(1, &UVBO);
		glDeleteBuffers(1, &CBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);

		shaderManager->StopProgram();
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
		
		// Get pointer to current mesh
		Model* model = modelManager->_models[graphicsComp->MeshID];

		// Replace all rgb for every vertex in mesh
		for (auto& mesh : model->_meshes)
		{
			for (auto& i : mesh->_rgb)
			{
				i = rgb;
			}
		}

		// Set diffuse and ambient to selected rgb
		graphicsComp->_materialData._ambient = rgb;
		graphicsComp->_materialData._diffuse = rgb;

		// No need to perform glBufferSubData here, update() will perform communication with GPU data
	}
	void GraphicsSystem::LoadMesh(const std::string& path, const std::string& name)
	{
		modelLoader->LoadModel(path, name);
	}
	void GraphicsSystem::AttachMesh(Entity& entity, const std::string& meshName)
	{
		if (entity.getComponent<ComponentGraphics>() == nullptr)
			entity.AttachComponent<ComponentGraphics>(ComponentGraphics(modelManager->AddModel(meshName)));
		else
			entity.getComponent<ComponentGraphics>()->MeshID = modelManager->AddModel(meshName);
	}
	void GraphicsSystem::DetachMesh(Entity& entity)
	{
		if (entity.getComponent<ComponentGraphics>() != nullptr)
			entity.getComponent<ComponentGraphics>()->MeshID = -1;
	}

	glm::mat4 GraphicsSystem::GetInverseViewMatrix()
	{
		return (glm::inverse(cameraManager->GetViewMatrix()) * glm::inverse(_projectionMatrix));
	}
}