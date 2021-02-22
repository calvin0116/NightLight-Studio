#pragma once
#include "Source/IO/Json/JsonFunction.h"
//#include "Source/IO/Json/Parser.h"
#include <string>

struct ISerializable
{
	char			ser_name[126];

	// meta data
	int objId = -1;
	//int objT = -1;
	bool isActive = true;



	virtual void	Read(Value&) { };
	virtual Value	Write() { return Value(rapidjson::kObjectType); };
	virtual Value&	Write(Value& val) { return val; };	
	virtual ISerializable* Clone() { return new ISerializable(); }
	virtual ~ISerializable() {}
};
