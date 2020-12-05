#include "ECSBind.h"
// mono wrapper functions
#include "../../MonoWrapper.h" 
// Get components etc
#include "../../../Component/Components.h" 
// Delta time
#include "../../../Core/DeltaTime.h"
// Scene change
#include "../../../Core/SceneManager.h"
 // For editor console cout
#include "../../../Editor/SystemEditor.h"

#define CSDebug

namespace ECSBind
{
  void BindECS()
  {
    MonoWrapper::BindClassFunction(GameObjectFind, "GameObjectFind", "UniBehaviour");
    MonoWrapper::BindClassFunction(dt, "DT", "UniBehaviour");
    MonoWrapper::BindClassFunction(realDt, "RealDT", "UniBehaviour");
    MonoWrapper::BindClassFunction(SetNextScene, "SetNextScene", "UniBehaviour");
    MonoWrapper::BindClassFunction(Print, "Print", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetScript, "GetScript", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetTransform, "GetTransform", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetCollider, "GetCollider", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetRigidBody, "GetRigidBody", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetGraphics, "GetGraphics", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetLight, "GetLight", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetNavigator, "GetNavigator", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetAnimation, "GetAnimation", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetCanvas, "GetCanvas", "UniBehaviour");
    MonoWrapper::BindClassFunction(GetVariables, "GetVariables", "UniBehaviour");
  }
  // For debugging in C#
  bool CheckCompGet(const void* comp, const std::string& type, const int& id)
  {
    if (comp == nullptr)
    {
#ifdef CSDebug
      ED_OUT(type + " returned nullptr on Entity ID: " + std::to_string(id));
#endif
      return true;
    }
    return false;
  }

  int GameObjectFind(MonoString* _name)
  {
    std::string sName = MonoWrapper::ToString(_name);
    Entity en = G_ECMANAGER->getEntity(sName);
#ifdef CSDebug
    if (en.getId() == -1)
      ED_OUT("GameObjectFind(): Can't find " + sName + "!");
#endif
    return en.getId();
  }

  // Delta Time
  float dt()
  {
    return DELTA_T->dt;
  }

  float realDt()
  {
    return DELTA_T->dt / CLOCKS_PER_SEC;
  }

  void SetNextScene(MonoString* scene_name)
  {
    NS_SCENE::SYS_SCENE_MANAGER->SetNextScene(MonoWrapper::ToString(scene_name));
  }

  void Print(MonoString* text)
  {
    ED_OUT(MonoWrapper::ToString(text));
  }

  MonoObject* GetScript(int id)
  {
    Entity en = G_ECMANAGER->getEntity(id);
    ScriptComponent* script = en.getComponent<ScriptComponent>();
    CheckCompGet(script, "GetScript()", id);
    if (script && script->_MonoData._pInstance)
      return script->_MonoData._pInstance; // Found script
    return nullptr; // nope
  }

  MonoObject* GetTransform(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("Transform");
    Entity en = G_ECMANAGER->getEntity(id);
    TransformComponent* trans = en.getComponent<TransformComponent>();
    if (CheckCompGet(trans, "GetTransform()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, trans);
    return monoObj;
  }

  MonoObject* GetCollider(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("Collider");
    Entity en = G_ECMANAGER->getEntity(id);
    ColliderComponent* col = en.getComponent<ColliderComponent>();
    if (CheckCompGet(col, "GetCollider()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, col);
    return monoObj;
  }

  MonoObject* GetRigidBody(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("RigidBody");
    Entity en = G_ECMANAGER->getEntity(id);
    RigidBodyComponent* rb = en.getComponent<RigidBodyComponent>();
    if (CheckCompGet(rb, "GetRigidBody()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, rb);
    return monoObj;
  }

  MonoObject* GetGraphics(int id)
  {
    // Create C# side component
    MonoObject* monoObj = MonoWrapper::ConstructObject("Graphics");
    Entity en = G_ECMANAGER->getEntity(id);
    GraphicsComponent* grph = en.getComponent<GraphicsComponent>();
    if (CheckCompGet(grph, "GetGraphics()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, grph);

    return monoObj;
  }

  MonoObject* GetLight(int id)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Light");
    Entity en = G_ECMANAGER->getEntity(id);
    LightComponent* lc = en.getComponent<LightComponent>();
    if (CheckCompGet(lc, "GetLight()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, lc);

    return monoObj;
  }

  MonoObject* GetNavigator(int id)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Navigator");
    Entity en = G_ECMANAGER->getEntity(id);
    NavigatorComponent* nav = en.getComponent<NavigatorComponent>();
    if (CheckCompGet(nav, "GetNav()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, nav);

    return monoObj;
  }

  MonoObject* GetAnimation(int id)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Animation");
    Entity en = G_ECMANAGER->getEntity(id);
    AnimationComponent* anim = en.getComponent<AnimationComponent>();
    if (CheckCompGet(anim, "GetAnimation()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, anim);

    return monoObj;
  }

  MonoObject* GetCanvas(int id)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Canvas");
    Entity en = G_ECMANAGER->getEntity(id);
    CanvasComponent* cnvs = en.getComponent<CanvasComponent>();
    if (CheckCompGet(cnvs, "GetCanvas()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, cnvs);

    return monoObj;
  }

  MonoObject* GetVariables(int id)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Variables");
    Entity en = G_ECMANAGER->getEntity(id);
    VariablesComponent* var = en.getComponent<VariablesComponent>();
    if (CheckCompGet(var, "GetVariables()", id))
      return nullptr;
    MonoWrapper::SetNativeHandle(monoObj, var);
    return monoObj;
  }
}