#include "ComponentGraphicsBind.h"
#include "../../MonoWrapper.h"


namespace ComponentGraphicsBind
{
  void BindComponentGraphics()
  {
    MonoWrapper::BindClassFunction(get_isActive_Internal, "get_isActive_Internal",
      "Graphics");
    MonoWrapper::BindClassFunction(set_isActive_Internal, "set_isActive_Internal",
      "Graphics");

    MonoWrapper::BindClassFunction(get_renderEmission_Internal, "get_renderEmission_Internal",
      "Graphics");
    MonoWrapper::BindClassFunction(set_renderEmission_Internal, "set_renderEmission_Internal",
      "Graphics");

    MonoWrapper::BindClassFunction(get_EmissiveColor_Internal, "get_EmissiveColor_Internal",
      "Graphics");
    MonoWrapper::BindClassFunction(set_EmissiveColor_Internal, "set_EmissiveColor_Internal",
      "Graphics");

    MonoWrapper::BindClassFunction(get_EmissiveIntensity_Internal, "get_EmissiveIntensity_Internal",
      "Graphics");
    MonoWrapper::BindClassFunction(set_EmissiveIntensity_Internal, "set_EmissiveIntensity_Internal",
      "Graphics");

    MonoWrapper::BindClassFunction(csAddModel, "csAddModel",
      "Graphics");

    MonoWrapper::BindClassFunction(csAddAlbedoTexture, "csAddAlbedoTexture",
      "Graphics");

    MonoWrapper::BindClassFunction(csAddNormalTexture, "csAddNormalTexture",
      "Graphics");

    MonoWrapper::BindClassFunction(csAddMetallicTexture, "csAddMetallicTexture",
      "Graphics");

    MonoWrapper::BindClassFunction(csAddRoughnessTexture, "csAddRoughnessTexture",
      "Graphics");

    MonoWrapper::BindClassFunction(csAddAOTexture, "csAddAOTexture",
      "Graphics");

    MonoWrapper::BindClassFunction(csAddSpecularTexture, "csAddSpecularTexture",
      "Graphics");

    MonoWrapper::BindClassFunction(csGetAlpha, "get_Alpha_Internal",
      "Graphics");
    MonoWrapper::BindClassFunction(csSetAlpha, "set_Alpha_Internal",
      "Graphics");
  }

  bool get_isActive_Internal(ComponentGraphics* graphics)
  {
    return graphics->_isActive;
  }

  void set_isActive_Internal(ComponentGraphics* graphics, bool val)
  {
    graphics->_isActive = val;
  }



  bool get_renderEmission_Internal(ComponentGraphics* graphics)
  {
    return graphics->CheckEmissiveActivation();
  }
  void set_renderEmission_Internal(ComponentGraphics* graphics, bool val)
  {
    graphics->ActivateEmissive(val);
  }

  MonoObject* get_EmissiveColor_Internal(ComponentGraphics* graphics)
  {
    MonoObject* obj = MonoWrapper::ConstructObject("Vector3");
    glm::vec3 vec3 = graphics->GetEmissive();
    MonoWrapper::SetObjectFieldValue(obj, "X", vec3.x);
    MonoWrapper::SetObjectFieldValue(obj, "Y", vec3.y);
    MonoWrapper::SetObjectFieldValue(obj, "Z", vec3.z);
    return obj;
  }
  void set_EmissiveColor_Internal(ComponentGraphics* graphics, MonoObject* obj)
  {
    glm::vec3 vec3;
    vec3.x = MonoWrapper::GetObjectFieldValue<float>(obj, "X");
    vec3.y = MonoWrapper::GetObjectFieldValue<float>(obj, "Y");
    vec3.z = MonoWrapper::GetObjectFieldValue<float>(obj, "Z");
    graphics->SetEmissive(vec3);
  }

  float get_EmissiveIntensity_Internal(ComponentGraphics* graphics)
  {
    return graphics->GetEmissiveIntensity();
  }
  void set_EmissiveIntensity_Internal(ComponentGraphics* graphics, float val)
  {
    graphics->SetEmissiveIntensity(val);
  }

  void csAddModel(ComponentGraphics* graphics, MonoString* file)
  {
    graphics->AddModel(MonoWrapper::ToString(file));
  }

  void csAddAlbedoTexture(ComponentGraphics* graphics, MonoString* file)
  {
    graphics->AddAlbedoTexture(MonoWrapper::ToString(file));
  }

  void csAddNormalTexture(ComponentGraphics* graphics, MonoString* file)
  {
    graphics->AddNormalTexture(MonoWrapper::ToString(file));
  }

  void csAddMetallicTexture(ComponentGraphics* graphics, MonoString* file)
  {
    graphics->AddMetallicTexture(MonoWrapper::ToString(file));
  }

  void csAddRoughnessTexture(ComponentGraphics* graphics, MonoString* file)
  {
    graphics->AddRoughnessTexture(MonoWrapper::ToString(file));
  }

  void csAddAOTexture(ComponentGraphics* graphics, MonoString* file)
  {
    graphics->AddAOTexture(MonoWrapper::ToString(file));
  }

  void csAddSpecularTexture(ComponentGraphics* graphics, MonoString* file)
  {
    graphics->AddSpecularTexture(MonoWrapper::ToString(file));
  }

  float csGetAlpha(ComponentGraphics* graphics)
  {
    return graphics->GetAlpha();
  }

  void csSetAlpha(ComponentGraphics* graphics, float alpha)
  {
    graphics->SetAlpha(alpha);
  }
}
