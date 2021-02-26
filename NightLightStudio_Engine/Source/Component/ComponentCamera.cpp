#include "ComponentCamera.h"
#include "Components.h"
#include "ComponentManager.h"

ComponentCamera::ComponentCamera()
	: _isActive{ false }, _data()/*_cameraID{ -1 }, _flag{ COLOUR },
	_colour{ 0.5f,0.5f,0.5f }, _fov{ 45.0f }, _near{ 1.0f }, _far{ 50000.0f },
	_x{}, _y{}, _w{}, _h{}, _depth{ 0 }*/
{
	strcpy_s(ser_name, "CameraComponent");
}

//ComponentCamera::ComponentCamera(const int& cameraID)
//	: _isActive{ true }, _data(cameraID)/*_cameraID{ cameraID }, _flag{ COLOUR },
//	_colour{ 0.5f,0.5f,0.5f }, _fov{ 45.0f }, _near{ 1.0f }, _far{ 50000.0f },
//	_x{}, _y{}, _w{}, _h{}, _depth{ 0 }*/
//{
//	strcpy_s(ser_name, "CameraComponent");
//}

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
	if (val.FindMember("isActive") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
		_isActive = val["isActive"].GetBool();


	if (val.FindMember("cameraFOV") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
		_data.cameraFOV = val["cameraFOV"].GetFloat();

	if (val.FindMember("cameraTarget") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
	{
		auto vec3 = val["cameraTarget"].GetArray();

		_data.cameraTarget.x = vec3[0].GetFloat();
		_data.cameraTarget.y = vec3[1].GetFloat();
		_data.cameraTarget.z = vec3[2].GetFloat();
	}

	if (val.FindMember("cameraPos") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
	{
		auto vec3 = val["cameraPos"].GetArray();

		_data.cameraPos.x = vec3[0].GetFloat();
		_data.cameraPos.y = vec3[1].GetFloat();
		_data.cameraPos.z = vec3[2].GetFloat();
	}

	if (val.FindMember("cameraRight") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
	{
		auto vec3 = val["cameraRight"].GetArray();

		_data.cameraRight.x = vec3[0].GetFloat();
		_data.cameraRight.y = vec3[1].GetFloat();
		_data.cameraRight.z = vec3[2].GetFloat();
	}

	if (val.FindMember("cameraFront") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
	{
		auto vec3 = val["cameraFront"].GetArray();

		_data.cameraFront.x = vec3[0].GetFloat();
		_data.cameraFront.y = vec3[1].GetFloat();
		_data.cameraFront.z = vec3[2].GetFloat();
	}

	if (val.FindMember("cameraUp") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
	{
		auto vec3 = val["cameraUp"].GetArray();

		_data.cameraUp.x = vec3[0].GetFloat();
		_data.cameraUp.y = vec3[1].GetFloat();
		_data.cameraUp.z = vec3[2].GetFloat();
	}

	if (val.FindMember("_rotation_sensitivity") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
		_data._rotation_sensitivity = val["_rotation_sensitivity"].GetFloat();

	if (val.FindMember("_drag_sensitivity") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
		_data._drag_sensitivity = val["_drag_sensitivity"].GetFloat();

	if (val.FindMember("_zoom_sensitivity") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
		_data._zoom_sensitivity = val["_zoom_sensitivity"].GetFloat();

	if (val.FindMember("cameraYaw") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
		_data.cameraYaw = val["cameraYaw"].GetFloat();

	if (val.FindMember("cameraPitch") == val.MemberEnd())
		;//std::cout << "No active data has been found" << std::endl;
	else
		_data.cameraPitch = val["cameraPitch"].GetFloat();
}

inline Value ComponentCamera::Write()
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);		//Bool

	NS_SERIALISER::ChangeData(&val, "cameraFOV", _data.cameraFOV);

	Value tgt(rapidjson::kArrayType);
	tgt.PushBack(_data.cameraTarget.x, global_alloc);
	tgt.PushBack(_data.cameraTarget.y, global_alloc);
	tgt.PushBack(_data.cameraTarget.z, global_alloc);
	NS_SERIALISER::ChangeData(&val, "cameraTarget", tgt);

	Value pos(rapidjson::kArrayType);
	pos.PushBack(_data.cameraPos.x, global_alloc);
	pos.PushBack(_data.cameraPos.y, global_alloc);
	pos.PushBack(_data.cameraPos.z, global_alloc);
	NS_SERIALISER::ChangeData(&val, "cameraPos", pos);

	Value right(rapidjson::kArrayType);
	right.PushBack(_data.cameraRight.x, global_alloc);
	right.PushBack(_data.cameraRight.y, global_alloc);
	right.PushBack(_data.cameraRight.z, global_alloc);
	NS_SERIALISER::ChangeData(&val, "cameraRight", right);

	Value front(rapidjson::kArrayType);
	front.PushBack(_data.cameraFront.x, global_alloc);
	front.PushBack(_data.cameraFront.y, global_alloc);
	front.PushBack(_data.cameraFront.z, global_alloc);
	NS_SERIALISER::ChangeData(&val, "cameraFront", front);

	Value up(rapidjson::kArrayType);
	up.PushBack(_data.cameraUp.x, global_alloc);
	up.PushBack(_data.cameraUp.y, global_alloc);
	up.PushBack(_data.cameraUp.z, global_alloc);
	NS_SERIALISER::ChangeData(&val, "cameraUp", up);

	NS_SERIALISER::ChangeData(&val, "_rotation_sensitivity", _data._rotation_sensitivity);

	NS_SERIALISER::ChangeData(&val, "_drag_sensitivity", _data._drag_sensitivity);

	NS_SERIALISER::ChangeData(&val, "_zoom_sensitivity", _data._zoom_sensitivity);

	NS_SERIALISER::ChangeData(&val, "cameraYaw", _data.cameraYaw);

	NS_SERIALISER::ChangeData(&val, "cameraPitch", _data.cameraPitch);

	return val;
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
