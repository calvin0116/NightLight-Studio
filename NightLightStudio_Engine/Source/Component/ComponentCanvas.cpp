#include "ComponentCanvas.h"

ComponentCanvas::ComponentCanvas() : _isActive{ true }
{
}

ComponentCanvas::~ComponentCanvas()
{
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

void ComponentCanvas::Read(Value& val)
{
	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		_isActive = val["isActive"].GetBool();

	//if (val.FindMember("Model") == val.MemberEnd())
	//	std::cout << "No Model file data has been found" << std::endl;
	//else
	//{
	//	
	//}

	//if (val.FindMember("Albedo") == val.MemberEnd())
	//	std::cout << "No Texture file data has been found" << std::endl;
	//else
	//{
	//	
	//}

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
			toPush._fileName = val[spriteName.c_str()].GetString();

		std::string position = std::string("UIPosition").append(std::to_string(i));
		if (val.FindMember(position.c_str()) == val.MemberEnd())
			std::cout << "No Diffuse data has been found" << std::endl;
		else
		{
			auto pos = val[position.c_str()].GetArray();

			toPush._position.x = pos[0].GetFloat();
			toPush._position.y = pos[1].GetFloat();
			toPush._position.z = pos[2].GetFloat();
		}

	/*	UISize;
		UIColour;*/
	}

	//if (val.FindMember("Metallic") == val.MemberEnd())
	//	std::cout << "No Metallic file data has been found" << std::endl;
	//else
	//	_metallicFileName = val["Metallic"].GetString();

	//if (val.FindMember("Roughness") == val.MemberEnd())
	//	std::cout << "No Roughness file data has been found" << std::endl;
	//else
	//	_roughnessFileName = val["Roughness"].GetString();

	//if (val.FindMember("AmbientOcclusion") == val.MemberEnd())
	//	std::cout << "No AmbientOcclusion file data has been found" << std::endl;
	//else
	//	_aoFileName = val["AmbientOcclusion"].GetString();

	//if (val.FindMember("Specular") == val.MemberEnd())
	//	std::cout << "No Specular file data has been found" << std::endl;
	//else
	//{
	//	_specularFileName = val["Specular"].GetString();

	//	if (!_specularFileName.empty())
	//	{
	//		_specularID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(_specularFileName.toString());
	//	}
	//}

	//Error checking for json data
	/*if (val.FindMember("DiffuseMat") == val.MemberEnd())
		std::cout << "No Diffuse data has been found" << std::endl;
	else
	{
		auto pos = val["DiffuseMat"].GetArray();

		_materialData._diffuse.x = pos[0].GetFloat();
		_materialData._diffuse.y = pos[1].GetFloat();
		_materialData._diffuse.z = pos[2].GetFloat();
	}

	if (val.FindMember("AmbientMat") == val.MemberEnd())
		std::cout << "No Ambient data has been found" << std::endl;
	else
	{
		auto scale = val["AmbientMat"].GetArray();

		_materialData._ambient.x = scale[0].GetFloat();
		_materialData._ambient.y = scale[1].GetFloat();
		_materialData._ambient.z = scale[2].GetFloat();
	}

	if (val.FindMember("SpecularMat") == val.MemberEnd())
		std::cout << "No Specular data has been found" << std::endl;
	else
	{
		auto rotate = val["SpecularMat"].GetArray();

		_materialData._specular.x = rotate[0].GetFloat();
		_materialData._specular.y = rotate[1].GetFloat();
		_materialData._specular.z = rotate[2].GetFloat();
	}

	if (val.FindMember("ShininessMat") == val.MemberEnd())
		std::cout << "No Shininess data has been found" << std::endl;
	else
	{
		_materialData._shininess = val["ShininessMat"].GetFloat();
	}*/
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

		NS_SERIALISER::ChangeData(&val, std::string("UIColour").append(std::to_string(i)), colour);
	}

	return val;
}


bool UI_Element::operator<(const UI_Element& rhs)
{
	return _position.z < rhs._position.z;
}
