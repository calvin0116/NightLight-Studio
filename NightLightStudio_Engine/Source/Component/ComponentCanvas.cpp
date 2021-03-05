#include "ComponentCanvas.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"
#include "../Graphics/UISystem.h"
#include "../Input/SystemInput.h"
#include "../Window/WndSystem.h"

// Tracy
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../Log/SpeedLog.h"

#pragma warning( disable : 26812 )

ComponentCanvas::ComponentCanvas() : _isActive{ true }, _canvasType{ SCREEN_SPACE }
{
	strcpy_s(ser_name, "CanvasComponent");
}

ComponentCanvas::~ComponentCanvas()
{
	strcpy_s(ser_name, "CanvasComponent");
}

void ComponentCanvas::AddUI()
{
	_uiElements.push_back(UI_Element{});
}

void ComponentCanvas::RemoveUI(size_t index)
{
	_uiElements.erase(index);
}

void ComponentCanvas::Sort()
{
	_uiElements.sort();
}

UI_Element& ComponentCanvas::FindUI(size_t index)
{
	return _uiElements.at(index);
}

UI_Element& ComponentCanvas::FindUIByName(std::string name)
{
	auto it = _uiElements.begin();
	auto end = _uiElements.end();

	while (it != end)
	{
		if ((*it)._uiName.toString() == name)
		{
			return *it;
		}
		++it;
	}
	return *(_uiElements.begin());
}

UI_Element& ComponentCanvas::operator[](size_t index)
{
	return _uiElements.at(index);
}

void ComponentCanvas::Read(Value& val)
{
	if (val.FindMember("isActive") == val.MemberEnd())
	{
		//std::cout << "No active data has been found" << std::endl;
		TracyMessageL("ComponentCanvas::Read: No active data has been found");
		SPEEDLOG("ComponentCanvas::Read: No active data has been found");
	}
	else
		_isActive = val["isActive"].GetBool();

	size_t loopCount = 0;

	if (val.FindMember("NumOfUIElement") == val.MemberEnd())
	{
		//std::cout << "No UI element quantity data has been found" << std::endl;
		TracyMessageL("ComponentCanvas::Read: No UI element quantity data has been found");
		SPEEDLOG("ComponentCanvas::Read: No UI element quantity data has been found");
	}
	else
		loopCount = val["NumOfUIElement"].GetInt();

	if (val.FindMember("CanvasType") == val.MemberEnd())
	{
		//std::cout << "No CanvasType data has been found" << std::endl;
		TracyMessageL("ComponentCanvas::Read: No CanvasType data has been found");
		SPEEDLOG("ComponentCanvas::Read: No CanvasType data has been found");
	}
	else
	{
		std::string canvas = val["CanvasType"].GetString();
		if (canvas == "SCREENSPACE")
		{
			_canvasType = SCREEN_SPACE;
		}
		else if (canvas == "WORLDSPACE")
		{
			_canvasType = WORLD_SPACE;
		}
	}

	_uiElements.clear();

	for (size_t i = 0; i < loopCount; ++i)
	{
		//Loads the ui
		UI_Element toPush;

		std::string uiName = std::string("UIName").append(std::to_string(i));
		if (val.FindMember(uiName.c_str()) == val.MemberEnd())
		{
			//std::cout << "No ui name data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No UI name data has been found");
			SPEEDLOG("ComponentCanvas::Read: No UI name data has been found");
		}
		else
			toPush._uiName = val[uiName.c_str()].GetString();

		std::string spriteName = std::string("Sprite").append(std::to_string(i));
		if (val.FindMember(spriteName.c_str()) == val.MemberEnd())
		{
			//std::cout << "No sprite data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No sprite data has been found");
			SPEEDLOG("ComponentCanvas::Read: No sprite data has been found");
		}
		else
		{
			toPush._fileName = val[spriteName.c_str()].GetString();
			toPush.AddTexture(toPush._fileName.toString());
		}

		std::string uiActive = std::string("UI_IsActive").append(std::to_string(i));
		if (val.FindMember(uiActive.c_str()) == val.MemberEnd())
		{
			//std::cout << "No UI Active data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No UI Active data has been found");
			SPEEDLOG("ComponentCanvas::Read: No UI Active data has been found");
		}	
		else
		{
			toPush._isActive = val[uiActive.c_str()].GetBool();
		}

		std::string animated = std::string("Animated").append(std::to_string(i));
		if (val.FindMember(animated.c_str()) == val.MemberEnd())
		{
			//std::cout << "No Animated data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No Animated data has been found");
			SPEEDLOG("ComponentCanvas::Read: No Animated data has been found");
		}
		else
		{
			toPush._isAnimated = val[animated.c_str()].GetBool();
		}

		std::string row = std::string("Row").append(std::to_string(i));
		if (val.FindMember(row.c_str()) == val.MemberEnd())
		{
			//std::cout << "No row data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No row data has been found");
			SPEEDLOG("ComponentCanvas::Read: No row data has been found");
		}
		else
		{
			toPush._row = val[row.c_str()].GetUint();
		}

		std::string column = std::string("Column").append(std::to_string(i));
		if (val.FindMember(column.c_str()) == val.MemberEnd())
		{
			//std::cout << "No column data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No column data has been found");
			SPEEDLOG("ComponentCanvas::Read: No column data has been found");
		}
		else
		{
			toPush._column = val[column.c_str()].GetUint();
		}

		std::string totalFrame = std::string("TotalFrame").append(std::to_string(i));
		if (val.FindMember(totalFrame.c_str()) == val.MemberEnd())
		{
			//std::cout << "No totalFrame data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No totalFrame data has been found");
			SPEEDLOG("ComponentCanvas::Read: No totalFrame data has been found");
		}
		else
		{
			toPush._totalFrame = val[totalFrame.c_str()].GetUint();
		}

		std::string fps = std::string("FramesPerSecond").append(std::to_string(i));
		if (val.FindMember(fps.c_str()) == val.MemberEnd())
		{
			//std::cout << "No fps data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No fps data has been found");           
			SPEEDLOG("ComponentCanvas::Read: No fps data has been found");
		}
		else
		{
			toPush._framesPerSecond = val[fps.c_str()].GetUint();

			if (toPush._framesPerSecond == 0)
			{
				toPush._animationRate = 0.0f;
			}
			else
			{
				toPush._animationRate = 1.0f / toPush._framesPerSecond;
			}
		}

		std::string play = std::string("Play").append(std::to_string(i));
		if (val.FindMember(play.c_str()) == val.MemberEnd())
		{
			//std::cout << "No play data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No play data has been found");
			SPEEDLOG("ComponentCanvas::Read: No play data has been found");
		}
		else
		{
			toPush._play = val[play.c_str()].GetBool();
		}

		std::string loop = std::string("Loop").append(std::to_string(i));
		if (val.FindMember(loop.c_str()) == val.MemberEnd())
		{
			//std::cout << "No loop data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No loop data has been found");
			SPEEDLOG("ComponentCanvas::Read: No loop data has been found");
		}
		else
		{
			toPush._loop = val[loop.c_str()].GetBool();
		}

		std::string position = std::string("UIPosition").append(std::to_string(i));
		if (val.FindMember(position.c_str()) == val.MemberEnd())
		{
			//std::cout << "No position data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No position data has been found");
			SPEEDLOG("ComponentCanvas::Read: No position data has been found");
		}
		else
		{
			auto pos = val[position.c_str()].GetArray();

			toPush._position.x = pos[0].GetFloat();
			toPush._position.y = pos[1].GetFloat();
			toPush._position.z = pos[2].GetFloat();
		}

		std::string uvName = std::string("UISize").append(std::to_string(i));
		if (val.FindMember(uvName.c_str()) == val.MemberEnd())
		{
			//std::cout << "No uv data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No UV data has been found");
			SPEEDLOG("ComponentCanvas::Read: No UV data has been found");
		}
		else
		{
			auto uv = val[uvName.c_str()].GetArray();

			toPush._size.x = uv[0].GetFloat();
			toPush._size.y = uv[1].GetFloat();
		}

		std::string col = std::string("UIColour").append(std::to_string(i));
		if (val.FindMember(col.c_str()) == val.MemberEnd())
		{
			//std::cout << "No colour data has been found" << std::endl;
			TracyMessageL("ComponentCanvas::Read: No colour data has been found");
			SPEEDLOG("ComponentCanvas::Read: No colour data has been found");
		}
		else
		{
			auto colour = val[col.c_str()].GetArray();

			toPush._colour.x = colour[0].GetFloat();
			toPush._colour.y = colour[1].GetFloat();
			toPush._colour.z = colour[2].GetFloat();
			toPush._colour.w = colour[3].GetFloat();
		}

		_uiElements.push_back(toPush);
	}

	Sort();
}

Value ComponentCanvas::Write()
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	switch (_canvasType)
	{
	case SCREEN_SPACE:
		NS_SERIALISER::ChangeData(&val, "CanvasType", rapidjson::StringRef("SCREENSPACE"));		//custom enum
		break;
	case WORLD_SPACE:
		NS_SERIALISER::ChangeData(&val, "CanvasType", rapidjson::StringRef("WORLDSPACE"));		//custom enum
		break;
	}

	NS_SERIALISER::ChangeData(&val, "NumOfUIElement", _uiElements.size());

	for (size_t i = 0; i < _uiElements.size(); ++i)
	{
		NS_SERIALISER::ChangeData(&val, std::string("UIName").append(std::to_string(i)), rapidjson::StringRef(_uiElements.at(i)._uiName.c_str()));
		NS_SERIALISER::ChangeData(&val, std::string("Sprite").append(std::to_string(i)), rapidjson::StringRef(_uiElements.at(i)._fileName.c_str()));

		NS_SERIALISER::ChangeData(&val, std::string("UI_IsActive").append(std::to_string(i)), _uiElements.at(i)._isActive);
		NS_SERIALISER::ChangeData(&val, std::string("Animated").append(std::to_string(i)), _uiElements.at(i)._isAnimated);
		NS_SERIALISER::ChangeData(&val, std::string("Row").append(std::to_string(i)), _uiElements.at(i)._row);
		NS_SERIALISER::ChangeData(&val, std::string("Column").append(std::to_string(i)), _uiElements.at(i)._column);
		NS_SERIALISER::ChangeData(&val, std::string("TotalFrame").append(std::to_string(i)), _uiElements.at(i)._totalFrame);
		NS_SERIALISER::ChangeData(&val, std::string("FramesPerSecond").append(std::to_string(i)), _uiElements.at(i)._framesPerSecond);

		NS_SERIALISER::ChangeData(&val, std::string("Play").append(std::to_string(i)), _uiElements.at(i)._play);
		NS_SERIALISER::ChangeData(&val, std::string("Loop").append(std::to_string(i)), _uiElements.at(i)._loop);

		Value position(rapidjson::kArrayType);
		position.PushBack(_uiElements.at(i)._position.x, global_alloc);
		position.PushBack(_uiElements.at(i)._position.y, global_alloc);
		position.PushBack(_uiElements.at(i)._position.z, global_alloc);

		NS_SERIALISER::ChangeData(&val, std::string("UIPosition").append(std::to_string(i)), position);

		Value size(rapidjson::kArrayType);
		size.PushBack(_uiElements.at(i)._size.x, global_alloc);
		size.PushBack(_uiElements.at(i)._size.y, global_alloc);

		NS_SERIALISER::ChangeData(&val, std::string("UISize").append(std::to_string(i)), size);

		Value colour(rapidjson::kArrayType);
		colour.PushBack(_uiElements.at(i)._colour.x, global_alloc);
		colour.PushBack(_uiElements.at(i)._colour.y, global_alloc);
		colour.PushBack(_uiElements.at(i)._colour.z, global_alloc);
		colour.PushBack(_uiElements.at(i)._colour.w, global_alloc);

		NS_SERIALISER::ChangeData(&val, std::string("UIColour").append(std::to_string(i)), colour);
	}

	return val;
}


glm::mat4 UI_Element::GetModelMatrix()
{
	unsigned width = NS_WINDOW::SYS_WINDOW->GetResolutionWidth();
	unsigned height = NS_WINDOW::SYS_WINDOW->GetResolutionHeight();

	glm::vec3 pos = _position;
	pos.x *= width;
	pos.y *= height;
	glm::mat4 Translate = glm::translate(glm::mat4(1.f), pos);

	glm::vec2 size = _size;
	size.x *= width;
	size.y *= height;
	glm::mat4 Scale = glm::scale(glm::mat4(1.f), glm::vec3(size, 1.0f));

	return (Translate * Scale);
}

void UI_Element::AddTexture(std::string filename)
{
	if (!filename.empty())
	{
		_imageID = NS_GRAPHICS::UISystem::GetInstance().LoadTexture(filename);
	}
}

void UI_Element::PlayAnimation(bool loop)
{
	if (!_isAnimated || _play)
		return;

	_play = true;
	_loop = loop;
	_currentFrame = 0;

	if (_framesPerSecond == 0)
	{
		_animationRate = 0.0f;
	}
	else
	{
		_animationRate = 1.0f / _framesPerSecond;
	}
}

void UI_Element::StopAnimation()
{
	_play = false;
	_currentFrame = 0;
}

//bool UI_Element::OnClick()
//{
//	glm::vec2 mouse = SYS_INPUT->GetSystemMousePos().GetMousePos();
//	mouse.x = mouse.x - SYS_INPUT->GetSystemMousePos().GetClientRectSize().x * 0.5f;
//	mouse.y = mouse.y - SYS_INPUT->GetSystemMousePos().GetClientRectSize().y * 0.5f;
//
//	glm::vec2 min = glm::vec2{ _position.x - (_size.x * 0.5f), _position.y - (_size.y * 0.5f)};
//	glm::vec2 max = glm::vec2{ _position.x + (_size.x * 0.5f), _position.y + (_size.y * 0.5f)};
//
//	if (min.x < mouse.x && max.x > mouse.x && min.y < mouse.y && max.y > mouse.y)
//	{
//		return true;
//	}
//	return false;
//}

bool UI_Element::OnClick() const
{
	glm::vec2 mouse = SYS_INPUT->GetSystemMousePos().GetMousePos();
	mouse.x = mouse.x - SYS_INPUT->GetSystemMousePos().GetClientRectSize().x * 0.5f;
	mouse.y = mouse.y - SYS_INPUT->GetSystemMousePos().GetClientRectSize().y * 0.5f;

	glm::vec2 min = glm::vec2{ (_position.x - (_size.x * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().x,
								(_position.y - (_size.y * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().y };
	glm::vec2 max = glm::vec2{ (_position.x + (_size.x * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().x,
								(_position.y + (_size.y * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().y };

	if (SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IMOUSE_LBUTTON))
	{
		if (min.x < mouse.x && max.x > mouse.x && min.y < mouse.y && max.y > mouse.y)
		{
			return true;
		}
	}
	return false;
}

bool UI_Element::OnHover() const
{
	glm::vec2 mouse = SYS_INPUT->GetSystemMousePos().GetMousePos();
	mouse.x = mouse.x - SYS_INPUT->GetSystemMousePos().GetClientRectSize().x * 0.5f;
	mouse.y = mouse.y - SYS_INPUT->GetSystemMousePos().GetClientRectSize().y * 0.5f;

	glm::vec2 min = glm::vec2{ (_position.x - (_size.x * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().x,
								(_position.y - (_size.y * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().y };
	glm::vec2 max = glm::vec2{ (_position.x + (_size.x * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().x,
								(_position.y + (_size.y * 0.5f)) * SYS_INPUT->GetSystemMousePos().GetClientRectSize().y };

	if (min.x < mouse.x && max.x > mouse.x && min.y < mouse.y && max.y > mouse.y)
	{
		return true;
	}

	return false;
}

bool UI_Element::operator<(const UI_Element& rhs)
{
	return _position.z < rhs._position.z;
}
