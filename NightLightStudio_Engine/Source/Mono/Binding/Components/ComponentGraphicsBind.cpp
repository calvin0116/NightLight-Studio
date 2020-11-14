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
  }

  bool get_isActive_Internal(ComponentGraphics* graphics)
  {
    return graphics->_isActive;
  }

  void set_isActive_Internal(ComponentGraphics* graphics, bool val)
  {
    graphics->_isActive = val;
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
}
