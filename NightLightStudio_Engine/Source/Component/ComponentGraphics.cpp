#include "ComponentGraphics.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Graphics/ModelManager.h"
#include "Components.h"
#include "ComponentManager.h"

ComponentGraphics::ComponentGraphics()
	: _isActive{ true },
	MeshID{ NULL }, _textureFileName{}, _textureID{ NULL }
{
	strcpy_s(ser_name ,"GraphicsComponent");
}

ComponentGraphics::ComponentGraphics(const int& meshID)
	: _isActive{ true },
	MeshID{ meshID }, _textureFileName{}, _textureID{ NULL }
{
	strcpy_s(ser_name, "GraphicsComponent");
}

void ComponentGraphics::AttachMesh(const int& meshID)
{
	MeshID = meshID;
}

ComponentGraphics::~ComponentGraphics()
{
}


//read and write function for initialization from saved files

inline void ComponentGraphics::Read(Value& val) 
{ 
	if (val.FindMember("isActive") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
		_isActive= val["isActive"].GetBool();

	if (val.FindMember("Texture file") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
		_textureFileName = val["Texture file"].GetString();

	if (val.FindMember("Model file") == val.MemberEnd())
		std::cout << "No Model file data has been found" << std::endl;
	else
	{
		_modelFileName = val["Model file"].GetString();
		NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadMesh(_modelFileName);

		//Checks for the file name
		std::string name;
		size_t pos = _modelFileName.rfind("\\");
		//Get just the string after the last path
		if (pos != std::string::npos)
		{
			name = _modelFileName.substr(pos + 1);
		}
		else
		{
			name = _modelFileName;
		}

		//Trim the extension to get the file name
		name.erase(name.rfind("."));
		//model->_fileName = s_LocalPathName + name + s_ModelFileType;

		MeshID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(name);
	}

}

inline Value ComponentGraphics::Write() 
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
	NS_SERIALISER::ChangeData(&val, "Texture file", rapidjson::StringRef(_textureFileName.c_str()) );
	NS_SERIALISER::ChangeData(&val, "Model file", rapidjson::StringRef(_modelFileName.c_str()));
	return val;
}
