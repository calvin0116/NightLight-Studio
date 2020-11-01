#include "ComponentLoadAudio.h"

//#include "Components.h"   // inherit required functions

ComponentLoadAudio::ComponentLoadAudio()
{
	strcpy_s(ser_name, "AudioManager");
}

ComponentLoadAudio::~ComponentLoadAudio()
{

}

inline Value ComponentLoadAudio::Write() { 
	Value val(rapidjson::kObjectType);
	//NS_SERIALISER::ChangeData(&val, my, rapidjson::StringRef(_modelFileName.c_str()));
	return val;
}
