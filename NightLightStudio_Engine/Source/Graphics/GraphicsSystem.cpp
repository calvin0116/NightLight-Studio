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
//#define TEST_CUBE_DRAW_WITHOUT_COMPONENT

#define DRAW_WITH_COMPONENTS

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
		modelManager->Free();
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

		// Set default values for view matrix
		// Set here to avoid weird bug at beginning
		SetViewMatrix();

		cameraManager->Init();

		/*Model* model = new Model();
		modelLoader->LoadFBX("test.fbx",model);
		delete model;*/

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

#ifdef TEST_CUBE_DRAW_WITHOUT_COMPONENT
		testCubeID = TestCreateCube(glm::vec3(1.0f, 0.5f, 1.0f), 1.f);
#endif
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

		// Sample draw cube
		//shaderManager->StartProgram(0);

		//GLuint VAO = NULL;
		//GLuint VBO = NULL;
		//GLuint EBO = NULL;
		//GLuint ModelMatrixBO = NULL;

		//glGenVertexArrays(1, &VAO);
		//glGenBuffers(1, &VBO);
		//glGenBuffers(1, &ModelMatrixBO);

		//float cube_vertices[] = {
		//	// positions		// texture coords	// color
		//	-1.0, -1.0,  1.0,	1.0f, 1.0f,			1.0f, 0.0f, 0.0f,
		//	 1.0, -1.0,  1.0,	1.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		//	 1.0,  1.0,  1.0,	0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
		//	-1.0,  1.0,  1.0,	1.0f, 1.0f,			1.0f, 1.0f, 1.0f,
		//	-1.0, -1.0, -1.0,	1.0f, 0.0f,			1.0f, 0.0f, 0.0f,
		//	 1.0, -1.0, -1.0,	0.0f, 0.0f,			0.0f, 1.0f, 0.0f,
		//	 1.0,  1.0, -1.0,	1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		//	-1.0,  1.0, -1.0,	1.0f, 0.0f,			1.0f, 1.0f, 1.0f
		//};

		//unsigned short cube_elements[] = {
		//	// front
		//	0, 1, 2,
		//	2, 3, 0,
		//	// right
		//	1, 5, 6,
		//	6, 2, 1,
		//	// back
		//	7, 6, 5,
		//	5, 4, 7,
		//	// left
		//	4, 0, 3,
		//	3, 7, 4,
		//	// bottom
		//	4, 5, 1,
		//	1, 0, 4,
		//	// top
		//	3, 2, 6,
		//	6, 7, 3
		//};

		//// Provide rotation in radians
		//glm::mat4 testmodelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(15.f), glm::vec3(0.0f,1.0f,0.f));

		//glBindVertexArray(VAO);

		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

		//// pos attribute
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);

		//// uv attribute
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);

		//// color attribute
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		//glEnableVertexAttribArray(2);

		//glGenBuffers(1, &EBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

		//glBindBuffer(GL_ARRAY_BUFFER, ModelMatrixBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &testmodelMatrix, GL_DYNAMIC_DRAW);

		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)0);
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)(sizeof(glm::vec4)));
		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)(2 * sizeof(glm::vec4)));
		//glEnableVertexAttribArray(6);
		//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(testmodelMatrix), (void*)(3 * sizeof(glm::vec4)));

		//glVertexAttribDivisor(3, 1);
		//glVertexAttribDivisor(4, 1);
		//glVertexAttribDivisor(5, 1);
		//glVertexAttribDivisor(6, 1);

		//glDrawElements(GL_TRIANGLES, 36 * sizeof(unsigned short), GL_UNSIGNED_SHORT, 0);

		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
		//glDeleteBuffers(1, &EBO);
		//glDeleteBuffers(1, &ModelMatrixBO);

#ifdef TEST_CUBE_DRAW_WITHOUT_COMPONENT

		shaderManager->StartProgram(0);

		for (auto& i : modelManager->meshes)
		{
			// Provide rotation in radians
			// test model matrix
		    glm::mat4 testmodelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(15.f), glm::vec3(0.0f,1.0f,0.f));

			glBindVertexArray(i->VAO);

			// We will only substitute Color and ModelMatrix Data
			glBindBuffer(GL_ARRAY_BUFFER, i->CBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * i->_rgb.size(), &i->_rgb[0]);
			
			glBindBuffer(GL_ARRAY_BUFFER, i->ModelMatrixBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &testmodelMatrix);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		}

		shaderManager->StopProgram();
#endif

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

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

			itr++;
		}

		//if (modelManager->meshes.size())
		//{
		//	Mesh* mesh = modelManager->meshes[0];

		//	glm::mat4 testmodelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(15.f), glm::vec3(0.0f, 1.0f, 0.f));

		//	glBindVertexArray(mesh->VAO);

		//	// We will only substitute Color and ModelMatrix Data
		//	glBindBuffer(GL_ARRAY_BUFFER, mesh->CBO);
		//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * mesh->_rgb.size(), &mesh->_rgb[0]);

		//	glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
		//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &testmodelMatrix);

		//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//}
		

		shaderManager->StopProgram();

		// This works
		//shaderManager->StartProgram(0);

		//for (auto& i : modelManager->meshes)
		//{
		//	// Provide rotation in radians
		//	// test model matrix
		//	glm::mat4 testmodelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(15.f), glm::vec3(0.0f, 1.0f, 0.f));

		//	glBindVertexArray(i->VAO);

		//	// We will only substitute Color and ModelMatrix Data
		//	glBindBuffer(GL_ARRAY_BUFFER, i->CBO);
		//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * i->_rgb.size(), &i->_rgb[0]);

		//	glBindBuffer(GL_ARRAY_BUFFER, i->ModelMatrixBO);
		//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &testmodelMatrix);

		//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//}

		//shaderManager->StopProgram();
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

	void GraphicsSystem::CreateCube(Entity& entity, const glm::vec3& rgb, const float& midExtent)
	{
		// Check if graphics component is already exists for obj
		// Assume it's a proper objID for now
		//if (G_ECMANAGER->getEntity(objID));

		Mesh* mesh = new Mesh();

		// Create cube and put into model manager's vector of models

		glGenVertexArrays(1, &mesh->VAO);
		glGenBuffers(1, &mesh->VBO);
		glGenBuffers(1, &mesh->EBO);
		glGenBuffers(1, &mesh->CBO);
		glGenBuffers(1, &mesh->UVBO);
		glGenBuffers(1, &mesh->ModelMatrixBO);

		// Vertex position
		mesh->_vertices.emplace_back(-midExtent, -midExtent, midExtent);
		mesh->_vertices.emplace_back(midExtent, -midExtent, midExtent);
		mesh->_vertices.emplace_back(midExtent, midExtent, midExtent);
		mesh->_vertices.emplace_back(-midExtent, midExtent, midExtent);
		mesh->_vertices.emplace_back(-midExtent, -midExtent, -midExtent);
		mesh->_vertices.emplace_back(midExtent, -midExtent, -midExtent);
		mesh->_vertices.emplace_back(midExtent, midExtent, -midExtent);
		mesh->_vertices.emplace_back(-midExtent, midExtent, -midExtent);

		// Vertex color
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);

		// Vertex Index
		// C cast to make compiler shut up
		mesh->_indices.emplace_back((unsigned short)0);
		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)0);

		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)1);

		mesh->_indices.emplace_back((unsigned short)7);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)4);
		mesh->_indices.emplace_back((unsigned short)7);

		mesh->_indices.emplace_back((unsigned short)4);
		mesh->_indices.emplace_back((unsigned short)0);
		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)7);
		mesh->_indices.emplace_back((unsigned short)4);

		mesh->_indices.emplace_back((unsigned short)4);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)0);
		mesh->_indices.emplace_back((unsigned short)4);

		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)7);
		mesh->_indices.emplace_back((unsigned short)3);

		// Texture coord (redundant for now)
		mesh->_uv.emplace_back(1.0f, 1.0f);
		mesh->_uv.emplace_back(1.0f, 0.0f);
		mesh->_uv.emplace_back(0.0f, 0.0f);
		mesh->_uv.emplace_back(1.0f, 1.0f);
		mesh->_uv.emplace_back(1.0f, 0.0f);
		mesh->_uv.emplace_back(0.0f, 0.0f);
		mesh->_uv.emplace_back(1.0f, 1.0f);
		mesh->_uv.emplace_back(1.0f, 0.0f);

		glBindVertexArray(mesh->VAO);

		// pos attribute
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_vertices.size(), &mesh->_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		
		// uv attribute
		glBindBuffer(GL_ARRAY_BUFFER, mesh->UVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* mesh->_uv.size(), &mesh->_uv[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(1);

		// color attribute
		glBindBuffer(GL_ARRAY_BUFFER, mesh->CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* mesh->_rgb.size(), &mesh->_rgb[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(2);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->_indices.size(), &mesh->_indices[0], GL_STATIC_DRAW);

		// Model Matrix
		glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
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
		
		unsigned currentid = modelManager->AddMesh(mesh);

		entity.AttachComponent<ComponentGraphics>(ComponentGraphics(currentid));
	}

	unsigned GraphicsSystem::TestCreateCube(const glm::vec3& rgb, const float& midExtent)
	{
		Mesh* mesh = new Mesh();

		// Create cube and put into model manager's vector of models

		glGenVertexArrays(1, &mesh->VAO);
		glGenBuffers(1, &mesh->VBO);
		glGenBuffers(1, &mesh->EBO);
		glGenBuffers(1, &mesh->CBO);
		glGenBuffers(1, &mesh->UVBO);
		glGenBuffers(1, &mesh->ModelMatrixBO);

		// Vertex position
		mesh->_vertices.emplace_back(-midExtent, -midExtent, midExtent);
		mesh->_vertices.emplace_back(midExtent, -midExtent, midExtent);
		mesh->_vertices.emplace_back(midExtent, midExtent, midExtent);
		mesh->_vertices.emplace_back(-midExtent, midExtent, midExtent);
		mesh->_vertices.emplace_back(-midExtent, -midExtent, -midExtent);
		mesh->_vertices.emplace_back(midExtent, -midExtent, -midExtent);
		mesh->_vertices.emplace_back(midExtent, midExtent, -midExtent);
		mesh->_vertices.emplace_back(-midExtent, midExtent, -midExtent);

		// Vertex color
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);
		mesh->_rgb.emplace_back(rgb.x, rgb.y, rgb.z);

		// Vertex Index
		// C cast to make compiler shut up
		mesh->_indices.emplace_back((unsigned short)0);
		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)0);

		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)1);

		mesh->_indices.emplace_back((unsigned short)7);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)4);
		mesh->_indices.emplace_back((unsigned short)7);

		mesh->_indices.emplace_back((unsigned short)4);
		mesh->_indices.emplace_back((unsigned short)0);
		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)7);
		mesh->_indices.emplace_back((unsigned short)4);

		mesh->_indices.emplace_back((unsigned short)4);
		mesh->_indices.emplace_back((unsigned short)5);
		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)1);
		mesh->_indices.emplace_back((unsigned short)0);
		mesh->_indices.emplace_back((unsigned short)4);

		mesh->_indices.emplace_back((unsigned short)3);
		mesh->_indices.emplace_back((unsigned short)2);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)6);
		mesh->_indices.emplace_back((unsigned short)7);
		mesh->_indices.emplace_back((unsigned short)3);

		// Texture coord (redundant for now)
		mesh->_uv.emplace_back(1.0f, 1.0f);
		mesh->_uv.emplace_back(1.0f, 0.0f);
		mesh->_uv.emplace_back(0.0f, 0.0f);
		mesh->_uv.emplace_back(1.0f, 1.0f);
		mesh->_uv.emplace_back(1.0f, 0.0f);
		mesh->_uv.emplace_back(0.0f, 0.0f);
		mesh->_uv.emplace_back(1.0f, 1.0f);
		mesh->_uv.emplace_back(1.0f, 0.0f);

		glBindVertexArray(mesh->VAO);

		// pos attribute
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_vertices.size(), &mesh->_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);


		// uv attribute
		glBindBuffer(GL_ARRAY_BUFFER, mesh->UVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh->_uv.size(), &mesh->_uv[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(1);

		// color attribute
		glBindBuffer(GL_ARRAY_BUFFER, mesh->CBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->_rgb.size(), &mesh->_rgb[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(2);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->_indices.size(), &mesh->_indices[0], GL_STATIC_DRAW);

		// Model Matrix
		glBindBuffer(GL_ARRAY_BUFFER, mesh->ModelMatrixBO);
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

		// Assign to mesh data manager and new graphics component
		// Attach graphics component to object

		return modelManager->AddMesh(mesh);
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
		Mesh* mesh = modelManager->meshes[graphicsComp->MeshID];

		// Replace all rgb for every vertex in mesh
		for (auto& i : mesh->_rgb)
		{
			i = rgb;
		}

		// No need to perform glBufferSubData here, update() will perform communication with GPU data
	}
}