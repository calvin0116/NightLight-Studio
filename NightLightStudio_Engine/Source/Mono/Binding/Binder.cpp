#include "AllBindings.h"
#include "Binder.h"


namespace MonoBind
{
  // none custom types
  void BindComponentsGetSet()
  {
    ComponentTransformBind::BindComponentTransform();
    ComponentColliderBind::BindComponentCollider();
    ComponentRigidBodyBind::BindComponentRigidBody();
    ComponentGraphicsBind::BindComponentGraphics();
    ComponentLightBind::BindComponentLight();
    ComponentNavigatorBind::BindComponentNavigator();
    ComponentAnimationBind::BindComponentAnimation();
    ComponentCanvasBind::BindComponentCanvas();
    // Temporary
    ComponentVariablesBind::BindComponentVariables();
  }

  // Custom types
  void BindCustomTypes()
  {
    Vector3DBind::BindVector3D();
    UIElementBind::BindUIElement();
  }

  // Systems
  void BindSystems()
  {
    ECSBind::BindECS();
    AudioBind::BindAudio();
    ForceBind::BindForce();
    CameraBind::BindCamera();
    InputBind::BindInput();
  }

  void Bind()
  {
    BindCustomTypes();
    BindComponentsGetSet();
    BindSystems();
  }
}