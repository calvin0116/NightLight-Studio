#include "ComponentListener.h"
#include "Components.h"
#include "ComponentManager.h"

//#include "Components.h"   // inherit required functions

ComponentListener::ComponentListener() : _isActive(false), _front{0.0f, 0.0f, 1.0f}
{
	strcpy_s(ser_name, "ListenerComponent");
}

ComponentListener::~ComponentListener()
{

}

void ComponentListener::Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman)
{
	g_ecman;
	for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
	{
		if (itr->name == "isActive")
		{
			_isActive = val["isActive"].GetBool();
		}
		if (itr->name == "front")
		{
			auto front = val["front"].GetArray();

			_front.x = front[0].GetFloat();
			_front.y = front[1].GetFloat();
			_front.z = front[2].GetFloat();
		}
	}

		
}

inline Value ComponentListener::Write()
{
	Value val(rapidjson::kObjectType);
	NS_SERIALISER::ChangeData(&val, "isActive", _isActive);

	Value front(rapidjson::kArrayType);
	front.PushBack(_front.x, global_alloc);
	front.PushBack(_front.y, global_alloc);
	front.PushBack(_front.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "front", front);

	return val;
}
