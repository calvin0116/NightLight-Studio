#pragma once
#include <mono/metadata/object.h>

namespace ECSBind
{
  int GameObjectFind(MonoString* _name);
  MonoObject* GetTransform(int id);

  void BindECS();    
}
