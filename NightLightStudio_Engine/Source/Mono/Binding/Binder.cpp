#include "AllBindings.h"
#include "Binder.h"


namespace MonoBind
{
  // Custom types
  void BindCustomTypes()
  {
    Vector3DBind::BindVector3D();
  }

  // none custom types
  void BindComponentsGetSet()
  {
    ComponentTransformBind::BindComponentTransform();
  }

  void BindSystems()
  {
    // Systems
    ECSBind::BindECS();
    AudioBind::BindAudio();
    ForceBind::BindForce();
  }

  void Bind()
  {
    BindCustomTypes();
    BindComponentsGetSet();
    BindSystems();
  }
}