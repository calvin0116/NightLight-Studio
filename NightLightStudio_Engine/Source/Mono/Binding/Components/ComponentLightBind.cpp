#include "ComponentLightBind.h"
#include "../../MonoWrapper.h"


namespace ComponentLightBind
{
  void BindComponentLight()
  {
    MonoWrapper::BindClassFunction(get_isActive_Internal, "get_isActive_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_isActive_Internal, "set_isActive_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_Intensity_Internal, "get_Intensity_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_Intensity_Internal, "set_Intensity_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_inCutOff_Internal, "get_inCutOff_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_inCutOff_Internal, "set_inCutOff_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_outCutOff_Internal, "get_outCutOff_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_outCutOff_Internal, "set_outCutOff_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_Radius_Internal, "get_Radius_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_Radius_Internal, "set_Radius_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_Color_Internal, "get_Color_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_Color_Internal, "set_Color_Internal",
      "Light");

    MonoWrapper::BindClassFunction(ChangeLightType_Internal, "ChangeLightType_Internal",
      "Light");
  }

  bool get_isActive_Internal(ComponentLight* trans)
  {
    return trans->GetActive();
  }

  void set_isActive_Internal(ComponentLight* trans, bool val)
  {
    //trans->_isActive = val;
    trans->SetActive(val);
  }

  float get_Intensity_Internal(ComponentLight* trans)
  {
    return trans->GetIntensity();
  }

  void set_Intensity_Internal(ComponentLight* trans, float val)
  {
    trans->SetIntensity(val);
  }

  float get_inCutOff_Internal(ComponentLight* trans)
  {
    return trans->GetCutOff();
  }
  void set_inCutOff_Internal(ComponentLight* trans, float val)
  {
    //trans->_cutOff = val;
    trans->SetCutOff(val);
  }

  float get_outCutOff_Internal(ComponentLight* trans)
  {
    return trans->GetOuterCutOff();
  }

  void set_outCutOff_Internal(ComponentLight* trans, float val)
  {
    //trans->_outerCutOff = val;
    trans->SetOuterCutOff(val);
  }

  float get_Radius_Internal(const ComponentLight* trans)
  {
    return trans->GetRadius();
  }

  void set_Radius_Internal(ComponentLight* trans, float val)
  {
    trans->SetRadius(val);
  }

  MonoObject* get_Color_Internal(ComponentLight* trans)
  {
      // Changed to just color
    MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
    glm::vec3 vec = trans->GetColor();
    MonoWrapper::SetObjectFieldValue(monoVec, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoVec, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoVec, "Z", vec.z);
    return monoVec;
  }

  void set_Color_Internal(ComponentLight* trans, MonoObject* val)
  {
      // Changed to just color
    glm::vec3 vec;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    trans->SetColor(vec);
  }

  void ChangeLightType_Internal(ComponentLight* trans, NS_GRAPHICS::Lights val)
  {
    //trans->ChangeLightType(val);
    trans->SetType(val);
  }
}
