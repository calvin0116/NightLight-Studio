#pragma once
#include "..\..\..\Component\ComponentLoadAudio.h"
#include <mono/metadata/object.h>


namespace AudioDataBind
{
  void BindAudioData();

  int get_Index_Internal(const ComponentLoadAudio::data* data);
  void set_Index_Internal(ComponentLoadAudio::data* data, int val);
}