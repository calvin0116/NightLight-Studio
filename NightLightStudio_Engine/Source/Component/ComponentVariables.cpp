#include "ComponentVariables.h"
#include "Components.h"

ComponentVariables::ComponentVariables()
{
	strcpy_s(ser_name,"ComponentVariables");
}

ComponentVariables::~ComponentVariables()
{
}

inline void ComponentVariables::Read(Value& val)
{
}

inline Value ComponentVariables::Write()
{
  return Value();
}

