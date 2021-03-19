#include "ComponentListener.h"
#include "Components.h"
#include "ComponentManager.h"

//#include "Components.h"   // inherit required functions

ComponentListener::ComponentListener() : _isActive(false)
{
	strcpy_s(ser_name, "ListenerComponent");
}

ComponentListener::~ComponentListener()
{

}

void ComponentListener::Read(Value& val)
{
	if (val.FindMember("isActive") == val.MemberEnd())
	{
		//std::cout << "No active data has been found" << std::endl;
		TracyMessageL("ComponentListener::Read: No active data has been found");
		SPEEDLOG("ComponentListener::Read: No active data has been found");
	}
	else
		_isActive = val["isActive"].GetBool();
}

inline Value ComponentListener::Write()
{
	Value val(rapidjson::kObjectType);
	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);

	return val;
}
