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

    MonoWrapper::BindClassFunction(get_Attenuation_Internal, "get_Attenuation_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_Attenuation_Internal, "set_Attenuation_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_inCutOff_Internal, "get_inCutOff_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_inCutOff_Internal, "set_inCutOff_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_outCutOff_Internal, "get_outCutOff_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_outCutOff_Internal, "set_outCutOff_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_Ambient_Internal, "get_Ambient_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_Ambient_Internal, "set_Ambient_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_Diffuse_Internal, "get_Diffuse_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_Diffuse_Internal, "set_Diffuse_Internal",
      "Light");

    MonoWrapper::BindClassFunction(get_Specular_Internal, "get_Specular_Internal",
      "Light");
    MonoWrapper::BindClassFunction(set_Specular_Internal, "set_Specular_Internal",
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

  float get_Attenuation_Internal(ComponentLight* trans)
  {
      // Note that this has been changed to intensity only
    return trans->GetIntensity();
  }

  void set_Attenuation_Internal(ComponentLight* trans, float val)
  {// Note that this has been changed to intensity only
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

  MonoObject* get_Ambient_Internal(ComponentLight* trans)
  {
      // Changed to just color
    MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
    glm::vec3 vec = trans->GetColor();
    MonoWrapper::SetObjectFieldValue(monoVec, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoVec, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoVec, "Z", vec.z);
    return monoVec;
  }

  void set_Ambient_Internal(ComponentLight* trans, MonoObject* val)
  {
      // Changed to just color
    glm::vec3 vec;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    trans->SetColor(vec);
  }

  MonoObject* get_Diffuse_Internal(ComponentLight* trans)
  {
    MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
    glm::vec3 vec = trans->GetColor();
    MonoWrapper::SetObjectFieldValue(monoVec, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoVec, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoVec, "Z", vec.z);
    return monoVec;
  }

  void set_Diffuse_Internal(ComponentLight* trans, MonoObject* val)
  {
    glm::vec3 vec;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    trans->SetColor(vec);
  }

  MonoObject* get_Specular_Internal(ComponentLight* trans)
  {
    MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
    glm::vec3 vec = trans->GetColor();
    MonoWrapper::SetObjectFieldValue(monoVec, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoVec, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoVec, "Z", vec.z);
    return monoVec;
  }

  void set_Specular_Internal(ComponentLight* trans, MonoObject* val)
  {
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
