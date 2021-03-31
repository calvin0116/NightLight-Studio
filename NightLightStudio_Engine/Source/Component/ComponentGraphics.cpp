#include "ComponentGraphics.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Graphics/ModelManager.h"
#include "Components.h"
#include "ComponentManager.h"
#include <fstream>

ComponentGraphics::ComponentGraphics()
	: _isActive{ true },
	_renderEmission{ false },
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

void ComponentGraphics::SetModelID(const int& modelID)
{
	_modelID = modelID;
}

int ComponentGraphics::GetModelID()
{
	return _modelID;
}

void ComponentGraphics::SetRenderType(const RENDERTYPE& rendertype)
{
	_renderType = rendertype;
}

void ComponentGraphics::ActivateEmissive(const bool& set)
{
	_renderEmission = set;
}

bool ComponentGraphics::CheckEmissiveActivation() const
{
	return _renderEmission;
}

void ComponentGraphics::SetEmissive(const glm::vec3& rgb)
{
	_pbrData._emissive = rgb;
}

glm::vec3 ComponentGraphics::GetEmissive() const
{
	return _pbrData._emissive;
}

void ComponentGraphics::SetEmissiveIntensity(const float& intensity)
{
	if (intensity > 1.f)
	{
		// prevent negative value
		_pbrData._emissiveIntensity = 0.1f;
		return;
	}

	_pbrData._emissiveIntensity = 1.f - intensity;
}

float ComponentGraphics::GetEmissiveIntensity() const
{
	return 1.f - _pbrData._emissiveIntensity;
}

void ComponentGraphics::AddModel(std::string filename)
{
	if (_modelFileName.toString() != filename && !filename.empty())
	{
		_modelFileName = filename;
		NS_GRAPHICS::GraphicsSystem::GetInstance()->LoadModel(_modelFileName.toString());
		_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(_modelFileName.toString());
	}

	//if (NS_GRAPHICS::ModelManager::GetInstance()._models[_modelID]->_isAnimated)
	//{
	//	//Attach animation component
	//	Entity* ent = &G_ECMANAGER->getEntity(this);
	//	ComponentAnimation* anim = ent->getComponent<ComponentAnimation>();
	//	if (anim)
	//	{
	//		//Existing anim component
	//		anim->_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();
	//		AnimationController* animCtrl =  NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID];
	//		for (auto& anims : NS_GRAPHICS::ModelManager::GetInstance()._models[_modelID]->_animations)
	//		{
	//			animCtrl->_allAnims.insert(anims.first);
	//		}
	//	}
	//	else
	//	{
	//		ent->AttachComponent<ComponentAnimation>();
	//		anim = ent->getComponent<ComponentAnimation>();
	//		anim->_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();
	//	}
	//}
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

void ComponentGraphics::SetAlpha(const float& alpha)
{
	_alpha = alpha;
}

float ComponentGraphics::GetAlpha() const
{
	return _alpha;
}

ComponentGraphics::~ComponentGraphics()
{
}


//read and write function for initialization from saved files

inline void ComponentGraphics::Read(Value& val) 
{ 
	if (val.FindMember("isActive") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No active data has been found");
		SPEEDLOG("ComponentGraphics::Read: No active data has been found");
		//std::cout << "No active data has been found" << std::endl;
	}
	else
		_isActive= val["isActive"].GetBool();

	if (val.FindMember("renderEmission") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No emission activeness data has been found");
		SPEEDLOG("ComponentGraphics::Read: No emission activeness data has been found");
		//std::cout << "No emission activeness data has been found" << std::endl;
	}
	else
		_renderEmission = val["renderEmission"].GetBool();


	if (val.FindMember("Emissive") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Emissive data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Emissive data has been found");
		//std::cout << "No Emissive data has been found" << std::endl;
	}
	else
	{
		auto emissive = val["Emissive"].GetArray();

		_pbrData._emissive.x = emissive[0].GetFloat();
		_pbrData._emissive.y = emissive[1].GetFloat();
		_pbrData._emissive.z = emissive[2].GetFloat();
	}

	if (val.FindMember("emissiveIntensity") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No emissiveIntensity data has been found");
		SPEEDLOG("ComponentGraphics::Read: No emissiveIntensity data has been found");
		//std::cout << "No emissiveIntensity data has been found" << std::endl;
	}
	else
	{
		_pbrData._emissiveIntensity = val["emissiveIntensity"].GetFloat();
	}


	if (val.FindMember("Model") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Model file data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Model file data has been found");
		//std::cout << "No Model file data has been found" << std::endl;
	}
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
			name.erase(name.rfind("."));
			//model->_fileName = s_LocalPathName + name + s_ModelFileType;
			*/
			_modelID = NS_GRAPHICS::ModelManager::GetInstance().AddModel(_modelFileName.toString());

			//Only if valid model
			//if (_modelID >= 0)
			//{
				//if (NS_GRAPHICS::ModelManager::GetInstance()._models[_modelID]->_isAnimated)
				//{
					//Entity entity = G_ECMANAGER->getEntity(this);

					//if (entity.getComponent<ComponentAnimation>() == nullptr)
					//{
					//	entity.AttachComponent<ComponentAnimation>();
					//	ComponentAnimation* anim = entity.getComponent<ComponentAnimation>();
					//	anim->_controllerID = NS_GRAPHICS::AnimationSystem::GetInstance().AddAnimController();
					//	AnimationController* animCtrl = NS_GRAPHICS::AnimationSystem::GetInstance()._animControllers[anim->_controllerID];
					//	for (auto& anims : NS_GRAPHICS::ModelManager::GetInstance()._models[_modelID]->_animations)
					//	{
					//		animCtrl->_allAnims.insert(anims.first);
					//	}
					//}
				//}
			//}
		}
	}

	if (val.FindMember("Albedo") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Texture file data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Texture file data has been found");
		//std::cout << "No Texture file data has been found" << std::endl;
	}
	else
	{
		std::string albedo = val["Albedo"].GetString();

		if (!albedo.empty())
		{
			AddAlbedoTexture(albedo);
		}
	}

	if (val.FindMember("Normal") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Normal file data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Normal file data has been found");
		//std::cout << "No Normal file data has been found" << std::endl;
	}
	else
	{
		std::string normal = val["Normal"].GetString();

		if (!normal.empty())
		{
			AddNormalTexture(normal);
		}
	}

	if (val.FindMember("Metallic") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Metallic file data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Metallic file data has been found");
		//std::cout << "No Metallic file data has been found" << std::endl;
	}
	else
	{
		std::string metallic = val["Metallic"].GetString();

		if (!metallic.empty())
		{
			AddMetallicTexture(metallic);
		}
	}

	if (val.FindMember("Roughness") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Roughness file data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Roughness file data has been found");
		//std::cout << "No Roughness file data has been found" << std::endl;
	}
	else
	{
		std::string roughness = val["Roughness"].GetString();

		if (!roughness.empty())
		{
			AddRoughnessTexture(roughness);
		}
	}

	if (val.FindMember("AmbientOcclusion") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No AmbientOcclusion file data has been found");
		SPEEDLOG("ComponentGraphics::Read: No AmbientOcclusion file data has been found");
		//std::cout << "No AmbientOcclusion file data has been found" << std::endl;
	}
	else
	{
		std::string ao = val["AmbientOcclusion"].GetString();

		if (!ao.empty())
		{
			AddAOTexture(ao);
		}
	}

	if (val.FindMember("Specular") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Specular file data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Specular file data has been found");
		//std::cout << "No Specular file data has been found" << std::endl;
	}
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
	{
		TracyMessageL("ComponentGraphics::Read: No Diffuse data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Diffuse data has been found");
		//std::cout << "No Diffuse data has been found" << std::endl;
	}
	else
	{
		auto pos = val["DiffuseMat"].GetArray();

		_pbrData._albedo.x = pos[0].GetFloat();
		_pbrData._albedo.y = pos[1].GetFloat();
		_pbrData._albedo.z = pos[2].GetFloat();
	}

	if (val.FindMember("AmbientMat") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Ambient data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Ambient data has been found");
		//std::cout << "No Ambient data has been found" << std::endl;
	}
	else
	{
		auto scale = val["AmbientMat"].GetArray();

		_materialData._ambient.x = scale[0].GetFloat();
		_materialData._ambient.y = scale[1].GetFloat();
		_materialData._ambient.z = scale[2].GetFloat();
	}

	if (val.FindMember("SpecularMat") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Specular data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Specular data has been found");
		//std::cout << "No Specular data has been found" << std::endl;
	}
	else
	{
		auto rotate = val["SpecularMat"].GetArray();

		_materialData._specular.x = rotate[0].GetFloat();
		_materialData._specular.y = rotate[1].GetFloat();
		_materialData._specular.z = rotate[2].GetFloat();
	}

	if (val.FindMember("ShininessMat") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Shininess data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Shininess data has been found");
		//std::cout << "No Shininess data has been found" << std::endl;
	}
	else
	{
		_materialData._shininess = val["ShininessMat"].GetFloat();
	}
	
	// PBR data
	if (val.FindMember("AlbedoMat") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No AlbedoMat data has been found");
		SPEEDLOG("ComponentGraphics::Read: No AlbedoMat data has been found");
		//std::cout << "No AlbedoMat data has been found" << std::endl;
	}
	else
	{
		auto albedo = val["AlbedoMat"].GetArray();

		_pbrData._albedo.x = albedo[0].GetFloat();
		_pbrData._albedo.y = albedo[1].GetFloat();
		_pbrData._albedo.z = albedo[2].GetFloat();
	}

	if (val.FindMember("MetallicMat") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No MetallicMat data has been found");
		SPEEDLOG("ComponentGraphics::Read: No MetallicMat data has been found");
		//std::cout << "No MetallicMat data has been found" << std::endl;
	}
	else
	{
		_pbrData._metallic = val["MetallicMat"].GetFloat();
	}

	if (val.FindMember("RoughnessMat") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No RoughnessMat data has been found");
		SPEEDLOG("ComponentGraphics::Read: No RoughnessMat data has been found");
		//std::cout << "No RoughnessMat data has been found" << std::endl;
	}
	else
	{
		_pbrData._roughness = val["RoughnessMat"].GetFloat();
	}

	if (val.FindMember("Alpha") == val.MemberEnd())
	{
		TracyMessageL("ComponentGraphics::Read: No Alpha data has been found");
		SPEEDLOG("ComponentGraphics::Read: No Alpha data has been found");
		//std::cout << "No Alpha data has been found" << std::endl;
	}
	else
	{
		_alpha = val["Alpha"].GetFloat();
	}
}

inline Value ComponentGraphics::Write() 
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool
	NS_SERIALISER::ChangeData(&val, "renderEmission", _renderEmission);		//Bool

	NS_SERIALISER::ChangeData(&val, "emissiveIntensity", _pbrData._emissiveIntensity);

	Value emissive(rapidjson::kArrayType);
	emissive.PushBack(_pbrData._emissive.x, global_alloc);
	emissive.PushBack(_pbrData._emissive.y, global_alloc);
	emissive.PushBack(_pbrData._emissive.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Emissive", emissive);

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

	NS_SERIALISER::ChangeData(&val, "Alpha", _alpha);

	return val;
}

bool ComponentGraphics::SaveMaterialDataFile(std::string filepath)
{
	if (filepath.size())
	{
		// Write Material Data to a file
		std::ofstream saveFile(filepath, std::ios::trunc);
		if (saveFile.is_open())
		{
			// Mostly just for easy access
			saveFile << _pbrData._albedo.x << " " <<  _pbrData._albedo.y << " " << _pbrData._albedo.z << "\n";
			saveFile << _pbrData._metallic << "\n";
			saveFile << _pbrData._roughness << "\n";
			saveFile << _alpha << "\n";
			saveFile.close();

			return true;
		}
	}
	return false;
}

bool ComponentGraphics::LoadMaterialDataFile(std::string filepath)
{
	if (filepath.size())
	{
		std::ifstream loadFile(filepath);
		if (loadFile.is_open())
		{
			std::string word;
			loadFile >> word; _pbrData._albedo.x = std::stof(word);
			loadFile >> word; _pbrData._albedo.y = std::stof(word);
			loadFile >> word; _pbrData._albedo.z = std::stof(word);
			loadFile >> word; _pbrData._metallic = std::stof(word);
			loadFile >> word; _pbrData._roughness = std::stof(word);
			loadFile >> word; _alpha = std::stof(word);
			return true;
		}
	}
	return false;
}
