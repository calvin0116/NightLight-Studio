#pragma once
#include <mono/metadata/object.h>
#include <string>

namespace ECSBind
{
  // Error check for entity get.
  bool  CheckCompGet(const void* comp, const std::string& type, const int& id);
  // Find object ID from name
  int   GameObjectFind(MonoString* _name);
  // Delta Time
  float dt();
  float realDt();
  // Scene change
  void  SetNextScene(MonoString* scene_name);
  // Pause game
  void	SetPause(bool _val);
  bool	GetPause();
  // Print in console
  void  Print(MonoString* text);
  // Ray cast
  int   RayCastIntersect(MonoObject* origin, MonoObject* endPos, MonoObject* intersect, int pos);
  int   RayCast(MonoObject* origin, MonoObject* endPos, int pos);
  void  RayTest(MonoObject* origin, MonoObject* end);
  // Get components from object ID
  MonoObject* GetScript(int id);
  MonoObject* GetScriptComp(int id);
  MonoObject* GetTransform(int id);
  MonoObject* GetCollider(int id);
  MonoObject* GetRigidBody(int id);
  MonoObject* GetGraphics(int id);
  MonoObject* GetLight(int id);
  MonoObject* GetNavigator(int id);
  MonoObject* GetAnimation(int id);
  MonoObject* GetCanvas(int id);
  MonoObject* GetEmitter(int id);
  MonoObject* GetAudioSource(int id);
  MonoObject* GetVariables(int id); // Use this for now until reflection is out

  void BindECS();
}
