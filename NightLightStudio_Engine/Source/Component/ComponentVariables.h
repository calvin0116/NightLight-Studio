#pragma once
#include "..\\Math\Vector.h"
//#include "LocalString.h"
#include "LocalVector.h"
#include "..\\..\ISerializable.h"

typedef struct ComponentVariables : public ISerializable //: public IComponent
{
    LocalVector<int> int_list;
    LocalVector<LocalString<DEF_STR_SIZE>> string_list;
    LocalVector<float> float_list;


  ComponentVariables();
  ~ComponentVariables();

  void	Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override;
  Value	Write();
  Value& Write(Value& val) { return val; };
  virtual ComponentVariables* Clone()
  {
    ComponentVariables* newcomp = new ComponentVariables();
    *newcomp = *this;
    return newcomp;
  }

} VariablesComponent;