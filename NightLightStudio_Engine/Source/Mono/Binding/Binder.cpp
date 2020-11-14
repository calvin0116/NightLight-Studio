#include "AllBindings.h"
#include "Binder.h"


namespace MonoBind
{
  // none custom types
  void BindComponentsGetSet()
  {
    ComponentTransformBind::BindComponentTransform();
    ComponentColliderBind::BindComponentCollider();
  }

  // Custom types
  void BindCustomTypes()
  {
    Vector3DBind::BindVector3D();
  }

  // Systems
  void BindSystems()
  {
    ECSBind::BindECS();
    AudioBind::BindAudio();
    ForceBind::BindForce();
    CameraBind::BindCamera();
  }

  void Bind()
  {
    BindCustomTypes();
    BindComponentsGetSet();
    BindSystems();
  }
}