#pragma once
#include <mono/metadata/object.h>

namespace ECSBind
{
  int GameObjectFind(MonoString* _name);

  void BindECS();    
}
