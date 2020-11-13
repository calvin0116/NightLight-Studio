#include "AllBindings.h"
#include "Binder.h"


namespace MonoBind
{
  void BindCustomTypes()
  {
    Vector3DBind::BindVector3D();
  }

  void BindComponentsGetSet()
  {
  }

  void Bind()
  {
    BindCustomTypes();
    BindComponentsGetSet();
    ECSBind::BindECS();
    AudioBind::BindAudio();
    ForceBind::BindForce();
  }
}