#include "ComponentCanvas.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"
#include "../Graphics/UISystem.h"

ComponentCanvas::ComponentCanvas() : _isActive{ true }
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
	_uiElements.sort();
}

void ComponentCanvas::RemoveUI(size_t index)
{
	_uiElements.erase(index);
}

void ComponentCanvas::Sort()
{
	_uiElements.sort();
}

void ComponentCanvas::Read(Value& val)
{
	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		_isActive = val["isActive"].GetBool();

	size_t loopCount = 0;

	if (val.FindMember("NumOfUIElement") == val.MemberEnd())
		std::cout << "No ui data has been found" << std::endl;
	else
		loopCount = val["NumOfUIElement"].GetInt();

	for (size_t i = 0; i < loopCount; ++i)
	{
		//Loads the ui
		UI_Element toPush;
		std::string uiName = std::string("UIName").append(std::to_string(i));

		if (val.FindMember(uiName.c_str()) == val.MemberEnd())
				std::cout << "No ui name data has been found" << std::endl;
			else
				toPush._uiName = val[uiName.c_str()].GetString();

		std::string spriteName = std::string("Sprite").append(std::to_string(i));
		if (val.FindMember(spriteName.c_str()) == val.MemberEnd())
			std::cout << "No sprite data has been found" << std::endl;
		else
		{
			toPush._fileName = val[spriteName.c_str()].GetString();
			toPush.AddTexture(toPush._fileName.toString());
		}

		std::string position = std::string("UIPosition").append(std::to_string(i));
		if (val.FindMember(position.c_str()) == val.MemberEnd())
			std::cout << "No position data has been found" << std::endl;
		else
		{
			auto pos = val[position.c_str()].GetArray();

			toPush._position.x = pos[0].GetFloat();
			toPush._position.y = pos[1].GetFloat();
			toPush._position.z = pos[2].GetFloat();
		}

		std::string uvName = std::string("UISize").append(std::to_string(i));
		if (val.FindMember(uvName.c_str()) == val.MemberEnd())
			std::cout << "No uv data has been found" << std::endl;
		else
		{
			auto uv = val[uvName.c_str()].GetArray();

			toPush._size.x = uv[0].GetFloat();
			toPush._size.y = uv[1].GetFloat();
		}

		std::string col = std::string("UIColour").append(std::to_string(i));
		if (val.FindMember(col.c_str()) == val.MemberEnd())
			std::cout << "No colour data has been found" << std::endl;
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

}

Value ComponentCanvas::Write()
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	NS_SERIALISER::ChangeData(&val, "NumOfUIElement", _uiElements.size());

	for (size_t i = 0; i < _uiElements.size(); ++i)
	{
		NS_SERIALISER::ChangeData(&val, std::string("UIName").append(std::to_string(i)), rapidjson::StringRef(_uiElements.at(i)._uiName.c_str()));
		NS_SERIALISER::ChangeData(&val, std::string("Sprite").append(std::to_string(i)), rapidjson::StringRef(_uiElements.at(i)._fileName.c_str()));

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
	glm::mat4 Translate = glm::translate(glm::mat4(1.f), _position);

	glm::mat4 Scale = glm::scale(glm::mat4(1.f), glm::vec3(_size, 1.0f));

	return (Translate * Scale);
}

void UI_Element::AddTexture(std::string filename)
{
	if (!filename.empty())
	{
		_imageID = NS_GRAPHICS::UISystem::GetInstance().LoadTexture(filename);
	}
}

bool UI_Element::operator<(const UI_Element& rhs)
{
	return _position.z < rhs._position.z;
}
