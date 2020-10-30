#include "AllBindings.h"
#include "Binder.h"


namespace MonoBind
{
  void BindCustomTypes()
  {
  }

  void BindComponentsGetSet()
  {
  }

  void Bind()
  {
    BindCustomTypes();
    BindComponentsGetSet();
    AudioBind::BindAudio();
  }
}