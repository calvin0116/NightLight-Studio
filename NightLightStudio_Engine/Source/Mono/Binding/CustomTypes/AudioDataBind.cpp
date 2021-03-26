#include "AudioDataBind.h"
#include "..\..\MonoWrapper.h"


namespace AudioDataBind
{
  void BindAudioData()
  {
    MonoWrapper::BindClassFunction(get_Index_Internal, "get_Index_Internal",
      "AudioData");
    MonoWrapper::BindClassFunction(set_Index_Internal, "set_Index_Internal",
      "AudioData");

    MonoWrapper::BindClassFunction(Play_Internal, "Play_Internal", "AudioData");
  }

  int get_Index_Internal(const ComponentLoadAudio::data* data)
  {
    return data->index;
  }
  void set_Index_Internal(ComponentLoadAudio::data* data, int val)
  {
    data->index = val;
  }

  bool get_IsBGM_Internal(const ComponentLoadAudio::data* data)
  {
    return data->isBGM;
  }
  void set_ISBGM_Internal(ComponentLoadAudio::data* data, bool val)
  {
    data->isBGM = val;
  }

  bool get_IsLoop_Internal(const ComponentLoadAudio::data* data)
  {
    return data->isLoop;
  }
  void set_IsLoop_Internal(ComponentLoadAudio::data* data, bool val)
  {
    data->SetLoop(val);
  }

  bool get_PlayOnAwake_Internal(const ComponentLoadAudio::data* data)
  {
    return data->playOnAwake;
  }
  void set_PlayOnAwake_Internal(ComponentLoadAudio::data* data, bool val)
  {
    data->playOnAwake = val;
  }

  float get_Volume_Internal(const ComponentLoadAudio::data* data)
  {
    return data->volume;
  }
  void set_Volume_Internal(ComponentLoadAudio::data* data, float val)
  {
    data->SetVolume(val);
  }

  bool get_Is3D_Internal(const ComponentLoadAudio::data* data)
  {
    return data->is3D;
  }
  void set_Is3D_Internal(ComponentLoadAudio::data* data, bool val)
  {
    data->Set3D(val);
  }

  float get_MinDist_Internal(const ComponentLoadAudio::data* data)
  {
    return data->minDist;
  }
  void set_MinDist_Internal(ComponentLoadAudio::data* data, float val)
  {
    data->SetMinDist(val);
  }

  float get_MaxDist_Internal(const ComponentLoadAudio::data* data)
  {
    return data->maxDist;
  }
  void set_MaxDist_Internal(ComponentLoadAudio::data* data, float val)
  {
    data->SetMaxDist(val);
  }

  bool get_IsMute_Internal(const ComponentLoadAudio::data* data)
  {
    return data->GetMute();
  }
  void set_IsMute_Internal(ComponentLoadAudio::data* data, bool val)
  {
    data->SetMute(val);
  }

  bool get_IsPause_Internal(const ComponentLoadAudio::data* data)
  {
    return data->GetPause();
  }
  void set_IsPause_Internal(ComponentLoadAudio::data* data, bool val)
  {
    data->SetPause(val);
  }

  void Play_Internal(ComponentLoadAudio::data* data)
  {
    data->Play();
  }
}