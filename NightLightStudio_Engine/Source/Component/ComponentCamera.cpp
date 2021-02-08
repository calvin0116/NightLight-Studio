#include "ComponentCamera.h"
#include "Components.h"
#include "ComponentManager.h"

//ComponentCamera::ComponentCamera()
//	: _isActive{ true }, _cameraID{ -1 }, _flag{ COLOUR },
//	_colour{ 0.5f,0.5f,0.5f }, _fov{ 45.0f }, _near{ 1.0f }, _far{ 50000.0f },
//	_x{}, _y{}, _w{}, _h{}, _depth{ 0 }
//{
//	strcpy_s(ser_name, "CameraComponent");
//}

//ComponentCamera::ComponentCamera(const int& cameraID)
//	: _isActive{ true }, _cameraID{ cameraID }, _flag{ COLOUR },
//	_colour{ 0.5f,0.5f,0.5f }, _fov{ 45.0f }, _near{ 1.0f }, _far{ 50000.0f },
//	_x{}, _y{}, _w{}, _h{}, _depth{ 0 }
//{
//	strcpy_s(ser_name, "CameraComponent");
//}

ComponentCamera::ComponentCamera() : _isActive(true), _Offset(0.0f, 0.0f, 0.0f), _orientation(0.0f, 0.0f, 0.0f, 0.0f), _lerp(false), _followTgt(false), _tgt(""),
_rotation_sensitivity(1000.0f), _drag_sensitivity(200.0f), _zoom_sensitivity(50.0f),
_yaw(0.0f), _pitch(0.0f), _roll(0.0f)
{
	strcpy_s(ser_name, "CameraComponent");
}

ComponentCamera::~ComponentCamera()
{
}


//read and write function for initialization from saved files

//void ComponentCamera::SetCurrentCamera()
//{
//	if (_cameraID == -1)
//		_cameraID = NS_GRAPHICS::CameraSystem::GetInstance().CreateCamera(cameraPos, cameraTarget, pitch, yaw);
//
//	NS_GRAPHICS::CameraSystem::GetInstance().SetCurrentCamera(_cameraID);
//}

inline void ComponentCamera::Read(Value& val)
{
	//if (val.FindMember("isActive") == val.MemberEnd())
	//	std::cout << "No active data has been found" << std::endl;
	//else
	//	_isActive = val["isActive"].GetBool();

	/*if (val.FindMember("FlagType") == val.MemberEnd())
		std::cout << "No active data has been found" << std::endl;
	else
	{
		std::string flagType = val["FlagType"].GetString();
		if (flagType == "SKYBOX")
		{
			_flag = SKYBOX;
		}
		else if (flagType == "COLOUR")
		{
			_flag = COLOUR;
		}

		if (val.FindMember("BackgroundColour") == val.MemberEnd())
			std::cout << "No colour data has been found" << std::endl;
		else
		{
			auto pos = val["BackgroundColour"].GetArray();

			_colour.x = pos[0].GetFloat();
			_colour.y = pos[1].GetFloat();
			_colour.z = pos[2].GetFloat();
		}
	}*/

	//if (val.FindMember("FieldOfView") == val.MemberEnd())
	//	std::cout << "No fov data has been found" << std::endl;
	//else
	//{
	//	_fov = val["FieldOfView"].GetFloat();
	//}

	//if (val.FindMember("NearPlane") == val.MemberEnd())
	//	std::cout << "No near plane data has been found" << std::endl;
	//else
	//{
	//	_near = val["NearPlane"].GetFloat();
	//}

	//if (val.FindMember("FarPlane") == val.MemberEnd())
	//	std::cout << "No far plane data has been found" << std::endl;
	//else
	//{
	//	_far = val["FarPlane"].GetFloat();
	//}

	/*if (val.FindMember("XOffset") == val.MemberEnd())
		std::cout << "No x offset data has been found" << std::endl;
	else
	{
		_x = val["XOffset"].GetFloat();
	}

	if (val.FindMember("YOffset") == val.MemberEnd())
		std::cout << "No y offset data has been found" << std::endl;
	else
	{
		_y = val["YOffset"].GetFloat();
	}*/

	//if (val.FindMember("Width") == val.MemberEnd())
	//	std::cout << "No width data has been found" << std::endl;
	//else
	//{
	//	_w = val["Width"].GetFloat();
	//}

	//if (val.FindMember("Height") == val.MemberEnd())
	//	std::cout << "No height data has been found" << std::endl;
	//else
	//{
	//	_h = val["Height"].GetFloat();
	//}

	//if (val.FindMember("Depth") == val.MemberEnd())
	//	std::cout << "No depth data has been found" << std::endl;
	//else
	//{
	//	_depth = val["Depth"].GetInt();
	//}
}

inline Value ComponentCamera::Write()
{
	//Value val(rapidjson::kObjectType);

	//NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	//switch (_flag)
	//{
	//case SKYBOX:
	//	NS_SERIALISER::ChangeData(&val, "FlagType", rapidjson::StringRef("SKYBOX"));		//custom enum
	//	break;
	//case COLOUR:
	//	NS_SERIALISER::ChangeData(&val, "FlagType", rapidjson::StringRef("COLOUR"));		//custom enum
	//	break;
	//default:
	//	break;
	//}

	//Value colour(rapidjson::kArrayType);
	//colour.PushBack(_colour.x, global_alloc);
	//colour.PushBack(_colour.y, global_alloc);
	//colour.PushBack(_colour.z, global_alloc);

	//NS_SERIALISER::ChangeData(&val, "BackgroundColour", colour);

	//NS_SERIALISER::ChangeData(&val, "FieldOfView", _fov);
	//NS_SERIALISER::ChangeData(&val, "NearPlane", _near);
	//NS_SERIALISER::ChangeData(&val, "FarPlane", _far);

	//NS_SERIALISER::ChangeData(&val, "XOffset", _x);
	//NS_SERIALISER::ChangeData(&val, "YOffset", _y);
	//NS_SERIALISER::ChangeData(&val, "Width", _w);
	//NS_SERIALISER::ChangeData(&val, "Height", _h);

	//NS_SERIALISER::ChangeData(&val, "Depth", _depth);

	//return val;
}
