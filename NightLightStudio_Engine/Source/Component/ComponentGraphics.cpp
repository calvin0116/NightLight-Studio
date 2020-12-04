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
	_aoFileName{}, _aoID{ NULL },
	_specularFileName{}, _specularID{ NULL }
{
	strcpy_s(ser_name ,"GraphicsComponent");
}

ComponentGraphics::ComponentGraphics(const int& meshID)
	: _isActive{ true },
	_modelID{ meshID }, _albedoFileName{}, _albedoID{ NULL },
	_normalFileName{}, _normalID{ NULL },
	_metallicFileName{}, _metallicID{ NULL },
	_roughnessFileName{}, _roughnessID{ NULL },
	_aoFileName{}, _aoID{ NULL },
	_specularFileName{}, _specularID{ NULL }
{
	strcpy_s(ser_name, "GraphicsComponent");
}

void ComponentGraphics::AttachMesh(const int& meshID)
{
	_modelID = meshID;
}

int ComponentGraphics::GetCurrentMeshHandle()
{
	return _modelID;
}

void ComponentGraphics::SetRenderType(const RENDERTYPE& rendertype)
{
	_renderType = rendertype;
}

void ComponentGraphics::AddModel(std::string filename)
{
	if (_modelFileName.toString() != filename && !filename.empty())
	{
		_modelFileName = filename;
		NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadModel(_modelFileName.toString());
		_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(_modelFileName.toString());
	}
}

void ComponentGraphics::AddAlbedoTexture(std::string filename)
{
	if (!filename.empty() && _albedoFileName.toString() != filename)
	{
		_albedoFileName = filename;
		_albedoID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(_albedoFileName.toString(), true);
	}

	if (_albedoID > 0 || _normalID > 0 || _metallicID > 0 || _roughnessID > 0 || _aoID > 0 || _specularID > 0)
	{
		_renderType = RENDERTYPE::TEXTURED;
	}
	else
	{
		_renderType = RENDERTYPE::SOLID;
	}
}

void ComponentGraphics::AddNormalTexture(std::string filename)
{
	if (!filename.empty() && _normalFileName.toString() != filename)
	{
		_normalFileName = filename;
		_normalID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(_normalFileName.toString());
	}

	if (_albedoID > 0 || _normalID > 0 || _metallicID > 0 || _roughnessID > 0 || _aoID > 0 || _specularID > 0)
	{
		_renderType = RENDERTYPE::TEXTURED;
	}
	else
	{
		_renderType = RENDERTYPE::SOLID;
	}
}

void ComponentGraphics::AddMetallicTexture(std::string filename)
{
	if (!filename.empty() && _metallicFileName.toString() != filename)
	{
		_metallicFileName = filename;
		_metallicID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(_metallicFileName.toString());
	}

	if (_albedoID > 0 || _normalID > 0 || _metallicID > 0 || _roughnessID > 0 || _aoID > 0 || _specularID > 0)
	{
		_renderType = RENDERTYPE::TEXTURED;
	}
	else
	{
		_renderType = RENDERTYPE::SOLID;
	}
}

void ComponentGraphics::AddRoughnessTexture(std::string filename)
{
	if (!filename.empty() && _roughnessFileName.toString() != filename)
	{
		_roughnessFileName = filename;
		_roughnessID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(_roughnessFileName.toString());
	}

	if (_albedoID > 0 || _normalID > 0 || _metallicID > 0 || _roughnessID > 0 || _aoID > 0 || _specularID > 0)
	{
		_renderType = RENDERTYPE::TEXTURED;
	}
	else
	{
		_renderType = RENDERTYPE::SOLID;
	}
}

void ComponentGraphics::AddAOTexture(std::string filename)
{
	if (!filename.empty() && _aoFileName.toString() != filename)
	{
		_aoFileName = filename;
		_aoID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(_aoFileName.toString());
	}

	if (_albedoID > 0 || _normalID > 0 || _metallicID > 0 || _roughnessID > 0 || _aoID > 0 || _specularID > 0)
	{
		_renderType = RENDERTYPE::TEXTURED;
	}
	else
	{
		_renderType = RENDERTYPE::SOLID;
	}
}

void ComponentGraphics::AddSpecularTexture(std::string filename)
{
	if (!filename.empty() && _specularFileName.toString() != filename)
	{
		_specularFileName = filename;
		_specularID = NS_GRAPHICS::TextureManager::GetInstance().GetTexture(_specularFileName.toString());
	}

	if (_albedoID > 0 || _normalID > 0 || _metallicID > 0 || _roughnessID > 0 || _aoID > 0 || _specularID > 0)
	{
		_renderType = RENDERTYPE::TEXTURED;
	}
	else
	{
		_renderType = RENDERTYPE::SOLID;
	}
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

	if (val.FindMember("Model") == val.MemberEnd())
		std::cout << "No Model file data has been found" << std::endl;
	else
	{
		_modelFileName = val["Model"].GetString();

		if (!_modelFileName.empty())
		{
			NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadModel(_modelFileName.toString());

			//Checks for the file name
			/*std::string name;
			size_t pos = _modelFileName.toString().rfind("\\");
			//Get just the string after the last path
			if (pos != std::string::npos)
			{
				name = _modelFileName.toString().substr(pos + 1);
			}
			else
			{
				name = _modelFileName.toString();
			}

			//Trim the extension to get the file name
			name.erase(name.rfind("."));dsadsa
			//model->_fileName = s_LocalPathName + name + s_ModelFileType;
			*/
			_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(_modelFileName.toString());

			if (NS_GRAPHICS::ModelManager::GetInstance()._models[_modelID]->_isAnimated)
			{
				//Attach animation component
				Entity* ent = &G_ECMANAGER->getEntity(this);
				if (ent->getComponent<ComponentAnimation>())
				{
					//There is animation
				}
			}
		}
	}

	if (val.FindMember("Albedo") == val.MemberEnd())
		std::cout << "No Texture file data has been found" << std::endl;
	else
	{
		std::string albedo = val["Albedo"].GetString();

		if (!albedo.empty())
		{
			AddAlbedoTexture(albedo);
		}
	}

	if (val.FindMember("Normal") == val.MemberEnd())
		std::cout << "No Normal file data has been found" << std::endl;
	else
	{
		std::string normal = val["Normal"].GetString();

		if (!normal.empty())
		{
			AddNormalTexture(normal);
		}
	}

	if (val.FindMember("Metallic") == val.MemberEnd())
		std::cout << "No Metallic file data has been found" << std::endl;
	else
	{
		std::string metallic = val["Metallic"].GetString();

		if (!metallic.empty())
		{
			AddMetallicTexture(metallic);
		}
	}

	if (val.FindMember("Roughness") == val.MemberEnd())
		std::cout << "No Roughness file data has been found" << std::endl;
	else
	{
		std::string roughness = val["Roughness"].GetString();

		if (!roughness.empty())
		{
			AddRoughnessTexture(roughness);
		}
	}

	if (val.FindMember("AmbientOcclusion") == val.MemberEnd())
		std::cout << "No AmbientOcclusion file data has been found" << std::endl;
	else
	{
		std::string ao = val["AmbientOcclusion"].GetString();

		if (!ao.empty())
		{
			AddAOTexture(ao);
		}
	}

	if (val.FindMember("Specular") == val.MemberEnd())
		std::cout << "No Specular file data has been found" << std::endl;
	else
	{
		std::string specular = val["Specular"].GetString();

		if (!specular.empty())
		{
			AddSpecularTexture(specular);
		}
	}

	//Error checking for json data
	if (val.FindMember("DiffuseMat") == val.MemberEnd())
		std::cout << "No Diffuse data has been found" << std::endl;
	else
	{
		auto pos = val["DiffuseMat"].GetArray();

		_pbrData._albedo.x = pos[0].GetFloat();
		_pbrData._albedo.y = pos[1].GetFloat();
		_pbrData._albedo.z = pos[2].GetFloat();
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
	}
	
	// PBR data
	if (val.FindMember("AlbedoMat") == val.MemberEnd())
		std::cout << "No AlbedoMat data has been found" << std::endl;
	else
	{
		auto albedo = val["AlbedoMat"].GetArray();

		_pbrData._albedo.x = albedo[0].GetFloat();
		_pbrData._albedo.y = albedo[1].GetFloat();
		_pbrData._albedo.z = albedo[2].GetFloat();
	}

	if (val.FindMember("MetallicMat") == val.MemberEnd())
		std::cout << "No MetallicMat data has been found" << std::endl;
	else
	{
		_pbrData._metallic = val["MetallicMat"].GetFloat();
	}

	if (val.FindMember("RoughnessMat") == val.MemberEnd())
		std::cout << "No RoughnessMat data has been found" << std::endl;
	else
	{
		_pbrData._roughness = val["RoughnessMat"].GetFloat();
	}
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
	NS_SERIALISER::ChangeData(&val, "Specular", rapidjson::StringRef(_specularFileName.c_str()));

	Value diffuse(rapidjson::kArrayType);
	diffuse.PushBack(_materialData._diffuse.x, global_alloc);
	diffuse.PushBack(_materialData._diffuse.y, global_alloc);
	diffuse.PushBack(_materialData._diffuse.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "DiffuseMat", diffuse);

	Value ambient(rapidjson::kArrayType);
	ambient.PushBack(_materialData._ambient.x, global_alloc);
	ambient.PushBack(_materialData._ambient.y, global_alloc);
	ambient.PushBack(_materialData._ambient.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "AmbientMat", ambient);

	Value specular(rapidjson::kArrayType);
	specular.PushBack(_materialData._specular.x, global_alloc);
	specular.PushBack(_materialData._specular.y, global_alloc);
	specular.PushBack(_materialData._specular.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "SpecularMat", specular);

	NS_SERIALISER::ChangeData(&val, "ShininessMat", _materialData._shininess);

	// PBR data
	Value albedo(rapidjson::kArrayType);
	albedo.PushBack(_pbrData._albedo.x, global_alloc);
	albedo.PushBack(_pbrData._albedo.y, global_alloc);
	albedo.PushBack(_pbrData._albedo.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "AlbedoMat", albedo);

	NS_SERIALISER::ChangeData(&val, "MetallicMat", _pbrData._metallic);

	NS_SERIALISER::ChangeData(&val, "RoughnessMat", _pbrData._roughness);

	return val;
}
