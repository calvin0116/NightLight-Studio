#include "UISystem.h"
#include "../glm/gtc/matrix_transform.hpp" // glm::ortho
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu

#include "../Component/Components.h" // Access to ECManager
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentCanvas.h"

NS_GRAPHICS::UISystem::UISystem():
	_uiDrawing{ true },
	_uiMatrix{ 1.0f },
	_vao{ 0 },
	_vbo{ 0 },
	_ebo{ 0 },
	_mmbo{ 0 },
	_textureManager{ nullptr },
	_shaderSystem{ nullptr }
{
}

NS_GRAPHICS::UISystem::~UISystem()
{
}

void NS_GRAPHICS::UISystem::Init()
{
	//Init sprite here
	const unsigned numOfSides = 4;

	ImageData image[numOfSides] = {
		{
			glm::vec3(-0.5f, 0.5f, 0.0f),//position
			glm::vec2(0.0f, 0.0f)//texture
		},
		{
			glm::vec3(0.5f,	0.5f, 0.0f),//position
			glm::vec2(1.0f,	0.0f)//texture
		},
		{
			glm::vec3(0.5f, -0.5f, 0.0f),//position
			glm::vec2(1.0f, 1.0f)//texture
		},
		{
			glm::vec3(-0.5f, -0.5f, 0.0f),//position
			glm::vec2(0.0f,	1.0f)//texture
		}
	};

	unsigned short indices[] = { 0,1,2,
								 2,3,0 };

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenBuffers(1, &_mmbo);

	glBindVertexArray(_vao);

	// image attributes
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ImageData) * numOfSides, &image[0], GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ImageData), (void*)0);
	glEnableVertexAttribArray(0);
	// uv attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImageData), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 6, &indices[0], GL_STATIC_DRAW);

	// Model Matrix
	glBindBuffer(GL_ARRAY_BUFFER, _mmbo);
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_textureManager = &TextureManager::GetInstance();
	_shaderSystem = &ShaderSystem::GetInstance();

	SetUIMatrix(NS_WINDOW::SYS_WINDOW->GetResolutionWidth(), NS_WINDOW::SYS_WINDOW->GetResolutionHeight());
}

void NS_GRAPHICS::UISystem::Update()
{
	if (_uiDrawing)
	{
		RenderUI();
	}
}

void NS_GRAPHICS::UISystem::RenderUI()
{
	auto itr = G_ECMANAGER->begin<ComponentCanvas>();
	auto itrEnd = G_ECMANAGER->end<ComponentCanvas>();
	while (itr != itrEnd)
	{
		ComponentCanvas* canvas = reinterpret_cast<ComponentCanvas*>(*itr);

		if (!canvas->_isActive)
		{
			++itr;
			continue;
		}

		size_t uiAmount = canvas->_uiElements.size();
		for (size_t i = 0; i < uiAmount; ++i)
		{
			if (!canvas->_uiElements.at(i)._imageID)
			{
				continue;
			}

			glm::mat4 ModelMatrix = canvas->_uiElements.at(i).GetModelMatrix();

			//For now ui only got picture
			_shaderSystem->StartProgram(ShaderSystem::ShaderType::UI); // textured program
			glBindVertexArray(_vao);

			// Update model and uniform for material
			glUniform4fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "colour"), 1, &canvas->_uiElements.at(i)._colour[0]);

			glBindBuffer(GL_ARRAY_BUFFER, _mmbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

			// Bind textures
			// bind diffuse map
			_textureManager->BindDiffuseTexture(canvas->_uiElements.at(i)._imageID);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

			_shaderSystem->StopProgram();
		}

		itr++;
	}
}

void NS_GRAPHICS::UISystem::SetUIMatrix(const int& width, const int& height, const float& near_plane, const float& far_plane)
{
	float ratioWidth = (float)width / NS_WINDOW::SYS_WINDOW->GetAppWidth();
	float ratioHeight = (float)height / NS_WINDOW::SYS_WINDOW->GetAppHeight();

	_uiMatrix = glm::ortho( 0.0f, (float)width,0.0f, (float)height ,near_plane,far_plane);
	/*	(float)-width * ratioWidth * 0.5f,
		(float)width * ratioWidth * 0.5f,
		(float)-height * ratioHeight * 0.5f,
		(float)height * ratioHeight * 0.5f,*/


	// Update only if changes are made
	glBindBuffer(GL_UNIFORM_BUFFER, _shaderSystem->GetViewProjectionUniformLocation());
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, glm::value_ptr(_uiMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

unsigned NS_GRAPHICS::UISystem::LoadTexture(std::string filename)
{
	return _textureManager->GetTexture(filename);
}
