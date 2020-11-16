#pragma once
#include "../../../Component/ComponentGraphics.h"
#include <mono/metadata/object.h>

namespace ComponentGraphicsBind
{
  bool get_isActive_Internal(ComponentGraphics* graphics);
  void set_isActive_Internal(ComponentGraphics* graphics, bool val);
  void csAddModel(ComponentGraphics* graphics, MonoString* file);
  void csAddAlbedoTexture(ComponentGraphics* graphics, MonoString* file);
  void csAddNormalTexture(ComponentGraphics* graphics, MonoString* file);
  void csAddMetallicTexture(ComponentGraphics* graphics, MonoString* file);
  void csAddRoughnessTexture(ComponentGraphics* graphics, MonoString* file);
  void csAddAOTexture(ComponentGraphics* graphics, MonoString* file);
  void csAddSpecularTexture(ComponentGraphics* graphics, MonoString* file);

  void BindComponentGraphics();
}