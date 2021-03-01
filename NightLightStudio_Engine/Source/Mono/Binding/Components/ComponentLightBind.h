#pragma once
#include "../../../Component/ComponentLight.h"
#include <mono/metadata/object.h>

namespace ComponentLightBind
{
  bool get_isActive_Internal(ComponentLight* trans);
  void set_isActive_Internal(ComponentLight* trans, bool val);

  float get_Intensity_Internal(ComponentLight* trans);
  void set_Intensity_Internal(ComponentLight* trans, float val);

  float get_inCutOff_Internal(ComponentLight* trans);
  void set_inCutOff_Internal(ComponentLight* trans, float val);

  float get_outCutOff_Internal(ComponentLight* trans);
  void set_outCutOff_Internal(ComponentLight* trans, float val);

  float get_Radius_Internal(const ComponentLight* trans);
  void set_Radius_Internal(ComponentLight* trans, float val);

  MonoObject* get_Color_Internal(ComponentLight* trans);
  void set_Color_Internal(ComponentLight* trans, MonoObject* val);

  void ChangeLightType_Internal(ComponentLight* trans, NS_GRAPHICS::Lights val);

  void BindComponentLight();
}
