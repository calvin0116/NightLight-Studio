#pragma once
#include <mono/metadata/object.h>

namespace ECSBind
{
  // Find object ID from name
  int GameObjectFind(MonoString* _name);
  // Get components from object ID
  MonoObject* GetTransform(int id);
  MonoObject* GetCollider(int id);

  void BindECS();    
}
