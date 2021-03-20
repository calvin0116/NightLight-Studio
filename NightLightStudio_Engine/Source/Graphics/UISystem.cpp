#include "UISystem.h"
#include "../glm/gtc/matrix_transform.hpp" // glm::ortho
#include "../glm/gtc/type_ptr.hpp"         // Cast to type pointer for communication with gpu

#include "../Component/Components.h" // Access to ECManager
#include "../Component/ComponentTransform.h"
#include "../Component/ComponentCanvas.h"

#include "../Input/SystemInput.h"

//Based on localvector on ui canvas
#define MAX_INSTANCE_NUMBER 32

NS_GRAPHICS::UISystem::UISystem():
	_uiDrawing{ true },
	_isPlaying{ false },
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
		//Top Right
		{
			glm::vec3(0.5f,	0.5f, 0.0f),//position
			glm::vec2(1.0f,	0.0f)//texture
		},
		//Top Left
		{
			glm::vec3(-0.5f, 0.5f, 0.0f),//position
			glm::vec2(0.0f, 0.0f)//texture
		},
		//Bottom Left
		{
			glm::vec3(-0.5f, -0.5f, 0.0f),//position
			glm::vec2(0.0f, 1.0f)//texture
		},
		//Bottom Right
		{
			glm::vec3(0.5f, -0.5f, 0.0f),//position
			glm::vec2(1.0f,	1.0f)//texture
		}
	};

	//float vertices[] = {
	//	 first triangle
	//	 0.5f,  0.5f, 0.0f,   0.f,0.f, // top right
	//	 0.5f, -0.5f, 0.0f,  1.0f, 1.0f, // bottom right
	//	-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, // top left 
	//	 second triangle	 
	//	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, // bottom right
	//	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom left
	//	-0.5f,  0.5f, 0.0f,  0.0f, 1.0f // top left
	//};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		2, 3, 0  // second triangle
	};

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenBuffers(1, &_mmbo);

	glBindVertexArray(_vao);

	// image attributes
	// reference object constant variables
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ImageData) * numOfSides, &image[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ImageData), (void*)0);
	glEnableVertexAttribArray(0);
	// uv attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImageData), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	// Model Matrix
	glBindBuffer(GL_ARRAY_BUFFER, _mmbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_INSTANCE_NUMBER, nullptr, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::vec3 worldVertex[numOfSides] = {
		//Top Right
		{
			glm::vec3(0.5f,	0.5f, 0.0f)//position
		},
		//Top Left
		{
			glm::vec3(-0.5f, 0.5f, 0.0f)//position
		},
		//Bottom Left
		{
			glm::vec3(-0.5f, -0.5f, 0.0f)//position
		},
		//Bottom Right
		{
			glm::vec3(0.5f, -0.5f, 0.0f)//position
		}
	};


	glGenVertexArrays(1, &_vaoWorld);
	glGenBuffers(1, &_vboWorld);
	glGenBuffers(1, &_eboWorld);

	glBindVertexArray(_vaoWorld);
	glBindBuffer(GL_ARRAY_BUFFER, _vboWorld);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numOfSides, &worldVertex[0], GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);

	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboWorld);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_textureManager = &TextureManager::GetInstance();
	_shaderSystem = &ShaderSystem::GetInstance();

	//Set up event to hide particle drawing
	SYS_INPUT->GetSystemKeyPress().CreateNewEvent("TOGGLE_UI_DRAW", SystemInput_ns::IKEY_9, "TOGGLE_UI_DRAW", SystemInput_ns::OnPress, [this]()
		{
			if (SYS_INPUT->GetSystemKeyPress().GetKeyHold(SystemInput_ns::IKEY_CTRL))
			{
				_uiDrawing = !_uiDrawing;
			}
		});

	// test using current perspective matrix
	//SetUIMatrix(NS_WINDOW::SYS_WINDOW->GetResolutionWidth(), NS_WINDOW::SYS_WINDOW->GetResolutionHeight());
}

void NS_GRAPHICS::UISystem::Update()
{
	if (_uiDrawing || _isPlaying)
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
			UI_Element& ui = canvas->_uiElements.at(i);

			//Skips not active and no texture
			if (!ui._isActive)
			{
				ui._mouseEnter = false;
				ui._mouseStay = false;
				ui._mouseExit = false;
				continue;
			}
			if (!ui._imageID)
			{
				continue;
			}

			//Only if is animated
			if (ui._isAnimated)
			{
				//Updates UI
				if (ui._play)
				{
					//Updates time
					ui._timePassed += DELTA_T->real_dt;

					//Animation rate depends on fps
					if (ui._timePassed >= ui._animationRate)
					{
						ui._timePassed = 0.0f;

						if (ui._reverse)
						{
							--ui._currentFrame;

							if (ui._currentFrame >= ui._totalFrame)
							{
								if (ui._loop)
								{
									ui._currentFrame = ui._totalFrame-1;
								}
								else
								{
									ui._play = false;
									ui._currentFrame++;
								}
							}
						}
						else
						{
							++ui._currentFrame;

							if (ui._currentFrame >= ui._totalFrame)
							{
								if (ui._loop)
								{
									ui._currentFrame = 0;
								}
								else
								{
									ui._play = false;
									ui._currentFrame--;
								}
							}
						}
					}
				}
			}

			if (ui._type == BUTTON)
			{
				ui.CheckMouseCollision();
			}

			//if (ui.OnEnter())
			//{
			//	TracyMessageL(std::string("Enter ").append(ui._uiName.c_str()).c_str());
			//	std::cout << std::string("Enter ").append(ui._uiName.c_str()).c_str() << std::endl;
			//}

			//if (ui.OnExit())
			//{
			//	TracyMessageL(std::string("Exit ").append(ui._uiName.c_str()).c_str());
			//	std::cout << std::string("Exit ").append(ui._uiName.c_str()).c_str() << std::endl;
			//}
			
			if (canvas->_canvasType == SCREEN_SPACE)
			{
				glm::mat4 ModelMatrix = ui.GetModelMatrix();

				//For now ui only got picture
				_shaderSystem->StartProgram(ShaderSystem::ShaderType::UI_SCREENSPACE); // textured program
				glBindVertexArray(_vao);

				// Update model and uniform for material
				glUniform4fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "colour"), 1, &ui._colour[0]);

				glBindBuffer(GL_ARRAY_BUFFER, _mmbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &ModelMatrix);

				//Should work
				if (ui._isAnimated)
				{
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxRow"), ui._row);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxColumn"), ui._column);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "currentFrame"), ui._currentFrame);
				}
				else
				{
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxRow"), 1);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxColumn"), 1);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "currentFrame"), 0);
				}

				// Bind textures
				// bind diffuse map
				_textureManager->BindAlbedoTexture(ui._imageID);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				_shaderSystem->StopProgram();
			}
			else if (canvas->_canvasType == WORLD_SPACE)
			{
				//glm::mat4 ModelMatrix = canvas->_uiElements.at(i).GetModelMatrix();
				//glm::mat4 ModelMatrix(1.0f);

				Entity entity = G_ECMANAGER->getEntity(canvas);
				ComponentTransform* trans = entity.getComponent<ComponentTransform>();

				glm::vec3 pos = ui._position;
				pos += trans->_position;
				//glm::mat4 Translate = glm::translate(glm::mat4(1.f), pos);

				glm::vec2 size = ui._size;
				size.x *= trans->_scale.x;
				size.y *= trans->_scale.y;
				//glm::mat4 Scale = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.0f));

				//glm::quat Quaternion(glm::radians(trans->_rotation));
				//glm::mat4 Rotate = glm::mat4_cast(Quaternion);

				//ModelMatrix = Translate;	

				_shaderSystem->StartProgram(ShaderSystem::ShaderType::UI_WORLDSPACE); // textured program

				glBindVertexArray(_vaoWorld);

				glUniform3fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "ui_pos"), 1, &pos[0]);
				glUniform3fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "ui_size"), 1, &size[0]);

				glm::vec3 camRight = CameraSystem::GetInstance().GetCamera().cameraRight;
				glm::vec3 camUp = CameraSystem::GetInstance().GetCamera().cameraUp;
				glUniform3fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "camera_right"), 1, &camRight[0]);
				glUniform3fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "camera_up"), 1, &camUp[0]);

				// Update model and uniform for material
				glUniform4fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "colour"), 1, &ui._colour[0]);

				//Should work
				if (ui._isAnimated)
				{
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxRow"), ui._row);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxColumn"), ui._column);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "currentFrame"), ui._currentFrame);
				}
				else
				{
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxRow"), 1);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "maxColumn"), 1);
					glUniform1ui(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "currentFrame"), 0);
				}

				// Bind textures
				// bind diffuse map
				_textureManager->BindAlbedoTexture(ui._imageID);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				_shaderSystem->StopProgram();
			}
			
		}

		itr++;
	}

	//_shaderSystem->
	//(ShaderSystem::ShaderType::UI_SCREENSPACE); // textured program

	//glBindVertexArray(_vao);

	//glm::vec4 tempCol = {1.0f, 1.0f, 1.0f, 0.4f};

	////glDrawElementsInstanced()
	//_textureManager->BindDiffuseTexture(LoadTexture("NightLight_1Mx1M.png"));
	//glUniform4fv(glGetUniformLocation(_shaderSystem->GetCurrentProgramHandle(), "colour"), 1, &tempCol[0]);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	////glDrawArrays(GL_TRIANGLES, 0, 6);

	//_shaderSystem->StopProgram();
}

unsigned NS_GRAPHICS::UISystem::LoadTexture(std::string filename)
{
	return _textureManager->GetTexture(filename);
}
