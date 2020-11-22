#pragma once
#include "..\\Math\Vector.h"

#include "..\\..\ISerializable.h"

typedef struct ComponentVariables : public ISerializable //: public IComponent
{


  ComponentVariables();
  ~ComponentVariables();

  void	Read(Value& val);
  Value	Write();
  Value& Write(Value& val) { return val; };
  virtual ComponentVariables* Clone()
  {
    ComponentVariables* newcomp = new ComponentVariables();
    *newcomp = *this;
    return newcomp;
  }

} VariablesComponent;