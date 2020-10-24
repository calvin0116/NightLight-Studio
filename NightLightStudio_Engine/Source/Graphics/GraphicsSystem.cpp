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

#define DRAW_WITH_COMPONENTS

namespace NS_GRAPHICS
{
	GraphicsSystem::GraphicsSystem()
		: shaderManager{ nullptr },
		modelLoader{ nullptr },
		textureLoader{ nullptr },
		meshManager{ nullptr },
		lightManager{ nullptr },
		debugManager{ nullptr },
		cameraManager{ nullptr },
		hasInit{ false },
		debugDrawing{ false },
		projectionMatrix{ glm::mat4(1.0f) },
		viewMatrix{ glm::mat4(1.0f) },
		testCubeID{ NULL }
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

		Render();
	}

	void GraphicsSystem::Free()
	{
		// Free all OpenGL objects before exit
		// Includes VAO, VBO, EBO, ModelMatrixBO
		meshManager->Free();
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
		meshManager = &MeshManager::GetInstance();
		lightManager = &LightSystem::GetInstance();
		debugManager = &DebugManager::GetInstance();
		cameraManager = &CameraSystem::GetInstance();

		modelLoader->Init();
		
		// Initialize sub systems and managers
		shaderManager->Init();

		// Set default values for view matrix
		// Set here to avoid weird bug at beginning
		SetViewMatrix();

		cameraManager->Init();

		//modelLoader->LoadFBX("cylinder.fbx", null);

		//meshManager->AddMesh();

		modelLoader->LoadModel("box.fbx", "box");

		//Draw Cylinder FBX file 
		modelLoader->LoadModel("cylinder.fbx", "cylinder");
		

		//Entity drawTest = G_ECMANAGER->BuildEntity();
		//ComponentTransform Transform1;
		//Transform1._position = { -100.0f,0.0f,0.0f };
		//drawTest.AttachComponent<ComponentTransform>(Transform1);
		//Mesh* mesh = meshManager->meshes[0];
		//
		////Randomize cylinder colour for better 3d view
		//for (int i = 0; i < mesh->_rgb.size(); ++i)
		//{

		//	mesh->_rgb[i] = glm::vec3((float)i / mesh->_rgb.size(), (mesh->_rgb.size() - (float)i)/ mesh->_rgb.size(), 0.0f);
		//}
		//InitMesh(drawTest, 0);

		//Draw sphere FBX file 
		modelLoader->LoadModel("sphere.fbx", "sphere");

		/*Entity drawTest2 = G_ECMANAGER->BuildEntity();
		ComponentTransform Transform2;
		Transform2._position = { 100.0f,0.0f,0.0f };
		drawTest2.AttachComponent<ComponentTransform>(Transform2);*/
		//Randomize cylinder colour for better 3d view
		/*Mesh* mesh2 = meshManager->meshes[1];
		for (int i = 0; i < mesh2->_rgb.size(); ++i)
		{
			mesh2->_rgb[i] = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
		}*/

		//InitMesh(drawTest2, 1);

		//std::vector<glm::vec3> vectTest;

		//////////////////////////////////////
		// TEST DRAW LOADED MODELS
		/*Entity testdrawSphere = G_ECMANAGER->BuildEntity();
		ComponentTransform testtransformsphere;
		testtransformsphere._position = { 0.f, 0.f,0.f };
		testdrawSphere.AttachComponent<ComponentTransform>(testtransformsphere);

		CreateSphere(testdrawSphere, glm::vec3(1.f,0.f,1.f));*/

		Entity testdrawCylinder = G_ECMANAGER->BuildEntity();
		ComponentTransform testtransformcylinder;
		testtransformcylinder._position = { 0.f, 0.f,0.f };
		testtransformcylinder._rotation = { 0.f, 0.f,0.f };
		testdrawCylinder.AttachComponent<ComponentTransform>(testtransformcylinder);

		CreateCylinder(testdrawCylinder, glm::vec3(0.f, 1.f, 1.f));

		std::ofstream logFile;
		logFile.open("cylinder.txt");

		int MeshSize = meshManager->meshlist["cylinder"]->_vertices.size();
		for (int i=0; i < MeshSize; ++i)
		{
			logFile << "Vertex: X: " << meshManager->meshlist["cylinder"]->_vertices[i].x << " Y: " << 
										meshManager->meshlist["cylinder"]->_vertices[i].y << " Z: " <<
										meshManager->meshlist["cylinder"]->_vertices[i].z << "\n";
		}

		logFile.close();

		////TEST VECTOR MAX SIZE
		//std::cout << "TEST MAX SIZE: " << vectTest.max_size() << std::endl;

		// Set default values for view matrix
		// temporary solution before camera system implementation
		// NOTE THAT CAMERA POSITION MUST/SHOULD BE SOME DISTANCE AWAY FROM OBJECT TO BE ABLE TO SEE
		// Positive Z = Towards Screen 
		/*glm::vec3 cameraPos(0.f, 0.f, 5.f);
		glm::vec3 cameraFront(0.f, 0.f, -1.f);
		glm::vec3 cameraUp(0.f, 1.f, 0.f);

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);*/

		// Set default values for projection matrix
		SetProjectionMatrix();

		// Enable depth buffering
		glEnable(GL_DEPTH_TEST);

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

			Mesh* mesh = meshManager->meshes[graphicsComp->MeshID];

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

	void GraphicsSystem::CreateCube(Entity& entity, const glm::vec3& rgb)
	{
		// Check if graphics component is already exists for obj
		// Assume it's a proper objID for now
		//if (G_ECMANAGER->getEntity(objID));

		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(meshManager->AddMesh("box")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::InitMesh(Entity& entity, unsigned index)
	{
		Mesh* meshToDraw = meshManager->meshes[index];
		glGenVertexArrays(1, &meshToDraw->VAO);
		glGenBuffers(1, &meshToDraw->VBO);
		glGenBuffers(1, &meshToDraw->EBO);
		glGenBuffers(1, &meshToDraw->CBO);
		glGenBuffers(1, &meshToDraw->UVBO);
		glGenBuffers(1, &meshToDraw->ModelMatrixBO);

		glBindVertexArray(meshToDraw->VAO);

		// pos attribute
		glBindBuffer(GL_ARRAY_BUFFER, meshToDraw->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * meshToDraw->_vertices.size(), &meshToDraw->_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);


		// uv attribute
		glBindBuffer(GL_ARRAY_BUFFER, meshToDraw->UVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * meshToDraw->_uv.size(), &meshToDraw->_uv[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(1);

		// color attribute
		glBindBuffer(GL_ARRAY_BUFFER, meshToDraw->CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * meshToDraw->_rgb.size(), &meshToDraw->_rgb[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(2);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshToDraw->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * meshToDraw->_indices.size(), &meshToDraw->_indices[0], GL_STATIC_DRAW);

		// Model Matrix
		glBindBuffer(GL_ARRAY_BUFFER, meshToDraw->ModelMatrixBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

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

		// Unbind buffers and array object
		// Assign to mesh data manager and new graphics component
		// Attach graphics component to object
		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindVertexArray(NULL);*/

		//unsigned currentid = meshManager->AddMesh(mesh);

		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(index));
	}

	void GraphicsSystem::CreateSphere(Entity& entity, const glm::vec3& rgb)
	{
		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(meshManager->AddMesh("sphere")));

		SetMeshColor(entity, rgb);
	}

	void GraphicsSystem::CreateCylinder(Entity& entity, const glm::vec3& rgb)
	{
		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(meshManager->AddMesh("cylinder")));

		SetMeshColor(entity, rgb);
	}

	//unsigned GraphicsSystem::TestCreateCube(const glm::vec3& rgb, const float& midExtent)
	//{
	//	Mesh* mesh = new Mesh();

	//	// Create cube and put into model manager's vector of models

	//	glGenVertexArrays(1, &mesh->VAO);
	//	glGenBuffers(1, &mesh->VBO);
	//	glGenBuffers(1, &mesh->EBO);
	//	glGenBuffers(1, &mesh->CBO);
	//	glGenBuffers(1, &mesh->UVBO);
	//	glGenBuffers(1, &mesh->ModelMatrixBO);

	//	// Vertex position
	//	mesh->_vertices.emplace_back(-midExtent, -midExtent, midExtent);
	//	mesh->_vertices.emplace_back(midExtent, -midExtent, midExtent);
	//	mesh->_vertices.emplace_back(midExtent, midExtent, midExtent);
	//	mesh->_vertices.emplace_back(-midExtent, midExtent, midExtent);
	//	mesh->_vertices.emplace_back(-midExtent, -midExtent, -midExtent);
	//	mesh->_vertices.emplace_back(midExtent, -midExtent, -midExtent);
	//	mesh->_vertices.emplace_back(midExtent, midExtent, -midExtent);
	//	mesh->_vertices.emplace_back(-midExtent, midExtent, -midExtent);

	//	// Vertex color
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
	//	mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);

	//	// Vertex Index
	//	// C cast to make compiler shut up
	//	mesh->_indices.emplace_back((unsigned short)0);
	//	mesh->_indices.emplace_back((unsigned short)1);
	//	mesh->_indices.emplace_back((unsigned short)2);
	//	mesh->_indices.emplace_back((unsigned short)2);
	//	mesh->_indices.emplace_back((unsigned short)3);
	//	mesh->_indices.emplace_back((unsigned short)0);

	//	mesh->_indices.emplace_back((unsigned short)1);
	//	mesh->_indices.emplace_back((unsigned short)5);
	//	mesh->_indices.emplace_back((unsigned short)6);
	//	mesh->_indices.emplace_back((unsigned short)6);
	//	mesh->_indices.emplace_back((unsigned short)2);
	//	mesh->_indices.emplace_back((unsigned short)1);

	//	mesh->_indices.emplace_back((unsigned short)7);
	//	mesh->_indices.emplace_back((unsigned short)6);
	//	mesh->_indices.emplace_back((unsigned short)5);
	//	mesh->_indices.emplace_back((unsigned short)5);
	//	mesh->_indices.emplace_back((unsigned short)4);
	//	mesh->_indices.emplace_back((unsigned short)7);

	//	mesh->_indices.emplace_back((unsigned short)4);
	//	mesh->_indices.emplace_back((unsigned short)0);
	//	mesh->_indices.emplace_back((unsigned short)3);
	//	mesh->_indices.emplace_back((unsigned short)3);
	//	mesh->_indices.emplace_back((unsigned short)7);
	//	mesh->_indices.emplace_back((unsigned short)4);

	//	mesh->_indices.emplace_back((unsigned short)4);
	//	mesh->_indices.emplace_back((unsigned short)5);
	//	mesh->_indices.emplace_back((unsigned short)1);
	//	mesh->_indices.emplace_back((unsigned short)1);
	//	mesh->_indices.emplace_back((unsigned short)0);
	//	mesh->_indices.emplace_back((unsigned short)4);

	//	mesh->_indices.emplace_back((unsigned short)3);
	//	mesh->_indices.emplace_back((unsigned short)2);
	//	mesh->_indices.emplace_back((unsigned short)6);
	//	mesh->_indices.emplace_back((unsigned short)6);
	//	mesh->_indices.emplace_back((unsigned short)7);
	//	mesh->_indices.emplace_back((unsigned short)3);

	//	// Texture coord (redundant for now)
	//	mesh->_uv.emplace_back(1.0f, 1.0f);
	//	mesh->_uv.emplace_back(1.0f, 0.0f);
	//	mesh->_uv.emplace_back(0.0f, 0.0f);
	//	mesh->_uv.emplace_back(1.0f, 1.0f);
	//	mesh->_uv.emplace_back(1.0f, 0.0f);
	//	mesh->_uv.emplace_back(0.0f, 0.0f);
	//	mesh->_uv.emplace_back(1.0f, 1.0f);
	//	mesh->_uv.emplace_back(1.0f, 0.0f);

	//	glBindVertexArray(mesh->VAO);

	//	// pos attribute
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_vertices.size(), &mesh->_vertices[0], GL_STATIC_DRAW);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	//	glEnableVertexAttribArray(0);


	//	// uv attribute
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh->UVBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh->_uv.size(), &mesh->_uv[0], GL_STATIC_DRAW);
	//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	//	glEnableVertexAttribArray(1);

	//	// color attribute
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh->CBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_rgb.size(), &mesh->_rgb[0], GL_STATIC_DRAW);
	//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	//	glEnableVertexAttribArray(2);

	//	// Indices
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->_indices.size(), &mesh->_indices[0], GL_STATIC_DRAW);

	//	// Model Matrix
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	//	glEnableVertexAttribArray(3);
	//	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	//	glEnableVertexAttribArray(4);
	//	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	//	glEnableVertexAttribArray(5);
	//	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	//	glEnableVertexAttribArray(6);
	//	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	//	glVertexAttribDivisor(3, 1);
	//	glVertexAttribDivisor(4, 1);
	//	glVertexAttribDivisor(5, 1);
	//	glVertexAttribDivisor(6, 1);

	//	// Assign to mesh data manager and new graphics component
	//	// Attach graphics component to object

	//	return meshManager->AddMesh(mesh);
	//}

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
		Mesh* mesh = meshManager->meshes[graphicsComp->MeshID];

		// Replace all rgb for every vertex in mesh
		for (auto& i : mesh->_rgb)
		{
			i = rgb;
		}

		// No need to perform glBufferSubData here, update() will perform communication with GPU data
	}
	void GraphicsSystem::LoadMesh(const std::string& path)
	{
		path;
	}
	void GraphicsSystem::AttachMesh(Entity& entity, const unsigned& meshID)
	{
		entity;
		meshID;
	}
	void GraphicsSystem::DetachMesh(Entity& entity)
	{
		entity;
	}
}