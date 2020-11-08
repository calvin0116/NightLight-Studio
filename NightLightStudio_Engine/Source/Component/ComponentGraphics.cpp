#include "ComponentGraphics.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Graphics/ModelManager.h"
#include "Components.h"
#include "ComponentManager.h"

ComponentGraphics::ComponentGraphics()
	: _isActive{ true },
	_modelID{ -1 }, _albedoFileName{}, _albedoID{ NULL },
	_normalFileName{}, _normalID{ NULL },
	_metallicFileName{}, _metallicID{ NULL },
	_roughnessFileName{}, _roughnessID{ NULL },
	_aoFileName{}, _aoID{ NULL }
{
	strcpy_s(ser_name ,"GraphicsComponent");
}

ComponentGraphics::ComponentGraphics(const int& meshID)
	: _isActive{ true },
	_modelID{ meshID }, _albedoFileName{}, _albedoID{ NULL },
	_normalFileName{}, _normalID{ NULL },
	_metallicFileName{}, _metallicID{ NULL },
	_roughnessFileName{}, _roughnessID{ NULL },
	_aoFileName{}, _aoID{ NULL }
{
	strcpy_s(ser_name, "GraphicsComponent");
}

void ComponentGraphics::AttachMesh(const int& meshID)
{
	_modelID = meshID;
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

	//if (val.FindMember("Model") == val.MemberEnd())
	//	std::cout << "No Model file data has been found" << std::endl;
	//else
	//{
	//	_modelFileName = val["Model"].GetString();

	//	if (!_modelFileName.empty())
	//	{
	//		NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadModel(_modelFileName.toString());

	//		//Checks for the file name
	//		/*std::string name;
	//		size_t pos = _modelFileName.toString().rfind("\\");
	//		//Get just the string after the last path
	//		if (pos != std::string::npos)
	//		{
	//			name = _modelFileName.toString().substr(pos + 1);
	//		}
	//		else
	//		{
	//			name = _modelFileName.toString();
	//		}

	//		//Trim the extension to get the file name
	//		name.erase(name.rfind("."));
	//		//model->_fileName = s_LocalPathName + name + s_ModelFileType;
	//		*/
	//		_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(_modelFileName.toString());
	//	}
	//}

	if (val.FindMember("Model file") == val.MemberEnd())
		std::cout << "No Model file data has been found" << std::endl;
	else
	{
		_modelFileName = val["Model file"].GetString();

		if (!_modelFileName.empty())
		{
			NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadModel(_modelFileName.toString());
			_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(_modelFileName.toString());
		}
	}

	if (val.FindMember("Texture file") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
		_albedoFileName = val["Texture file"].GetString();

	//if (val.FindMember("Texture") == val.MemberEnd())
	//	std::cout << "No Texture file data has been found" << std::endl;
	//else
	//	_albedoFileName = val["Texture"].GetString();

	if (val.FindMember("Normal") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
		_normalFileName = val["Normal"].GetString();

	if (val.FindMember("Metallic") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
		_metallicFileName = val["Metallic"].GetString();

	if (val.FindMember("Roughness") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
		_roughnessFileName = val["Roughness"].GetString();

	if (val.FindMember("AmbientOcclusion") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
		_aoFileName = val["AmbientOcclusion"].GetString();

}

inline Value ComponentGraphics::Write() 
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
	NS_SERIALISER::ChangeData(&val, "Model", rapidjson::StringRef(_modelFileName.c_str()));
	NS_SERIALISER::ChangeData(&val, "Albedo", rapidjson::StringRef(_albedoFileName.c_str()));
	NS_SERIALISER::ChangeData(&val, "Normal", rapidjson::StringRef(_normalFileName.c_str()));
	NS_SERIALISER::ChangeData(&val, "Metallic", rapidjson::StringRef(_metallicFileName.c_str()));
	NS_SERIALISER::ChangeData(&val, "Roughness", rapidjson::StringRef(_roughnessFileName.c_str()));
	NS_SERIALISER::ChangeData(&val, "AmbientOcclusion", rapidjson::StringRef(_aoFileName.c_str()));

	return val;
}
