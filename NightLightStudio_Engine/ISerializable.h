#pragma once
#include "Source/IO/Json/JsonHeader.h"


struct ISerializable
{
	virtual void	Read(Value& val) {};
	virtual Value	Write() { return Value(); };
	virtual Value&	Write(Value& val) { return val; };
};