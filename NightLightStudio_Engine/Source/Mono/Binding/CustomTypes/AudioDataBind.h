#pragma once
#include "..\..\..\Component\ComponentLoadAudio.h"
#include <mono/metadata/object.h>


namespace AudioDataBind
{
  void BindAudioData();

  int get_Index_Internal(const ComponentLoadAudio::data* data);
  void set_Index_Internal(ComponentLoadAudio::data* data, int val);

  bool get_IsBGM_Internal(const ComponentLoadAudio::data* data);
  void set_ISBGM_Internal(ComponentLoadAudio::data* data, bool val);

  bool get_IsLoop_Internal(const ComponentLoadAudio::data* data);
  void set_IsLoop_Internal(ComponentLoadAudio::data* data, bool val);

  bool get_PlayOnAwake_Internal(const ComponentLoadAudio::data* data);
  void set_PlayOnAwake_Internal(ComponentLoadAudio::data* data, bool val);

  float get_Volume_Internal(const ComponentLoadAudio::data* data);
  void set_Volume_Internal(ComponentLoadAudio::data* data, float val);

  bool get_Is3D_Internal(const ComponentLoadAudio::data* data);
  void set_Is3D_Internal(ComponentLoadAudio::data* data, bool val);

  float get_MinDist_Internal(const ComponentLoadAudio::data* data);
  void set_MinDist_Internal(ComponentLoadAudio::data* data, float val);

  float get_MaxDist_Internal(const ComponentLoadAudio::data* data);
  void set_MaxDist_Internal(ComponentLoadAudio::data* data, float val);

  bool get_IsMute_Internal(const ComponentLoadAudio::data* data);
  void set_IsMute_Internal(ComponentLoadAudio::data* data, bool val);

  bool get_IsPause_Internal(const ComponentLoadAudio::data* data);
  void set_IsPause_Internal(ComponentLoadAudio::data* data, bool val);

  // Play audio
  void Play_Internal(ComponentLoadAudio::data* data);
  // Stop Audio
  void Stop_Internal(ComponentLoadAudio::data* data);
}