#pragma once
#include "../../../Component/ComponentLoadAudio.h"
#include <mono/metadata/object.h>

namespace ComponentAudioSourceBind
{
  bool get_IsActive_Internal(ComponentAudioSource* aud);
  void set_IsActive_Internal(ComponentAudioSource* aud, bool val);
  // Array of audio datas
  int get_Size_Internal(ComponentAudioSource* aud);
  MonoObject* get_Data_Internal(ComponentAudioSource* aud, int _index);
  //void set_Data_Internal(ComponentAudioSource* aud, MonoObject* obj, int _index);

  void BindComponentAudioSource();
}
