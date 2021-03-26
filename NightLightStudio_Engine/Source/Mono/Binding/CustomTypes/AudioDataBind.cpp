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
  }

  int get_Index_Internal(const ComponentLoadAudio::data* data)
  {
    return data->index;
  }
  void set_Index_Internal(ComponentLoadAudio::data* data, int val)
  {
    data->index = val;
  }
}