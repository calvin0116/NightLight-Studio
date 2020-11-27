#pragma once
#include <mono/metadata/object.h>

namespace ECSBind
{
  // Find object ID from name
  int GameObjectFind(MonoString* _name);
  // Delta Time
  float dt();
  float realDt();
  // Scene change
  void SetNextScene(MonoString* scene_name);
  // Get components from object ID
  MonoObject* GetScript(int id);
  MonoObject* GetTransform(int id);
  MonoObject* GetCollider(int id);
  MonoObject* GetRigidBody(int id);
  MonoObject* GetGraphics(int id);
  MonoObject* GetLight(int id);
  MonoObject* GetVariables(int id);

  void BindECS();    
}
