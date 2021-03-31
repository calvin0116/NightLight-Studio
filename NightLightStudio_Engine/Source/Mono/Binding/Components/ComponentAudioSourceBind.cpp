#include "ComponentAudioSourceBind.h"
#include "../../MonoWrapper.h"
// Tracy
#include "../tracy-master/Tracy.hpp"
// SpeedLog
#include "../../../Log/SpeedLog.h"


namespace ComponentAudioSourceBind
{
  void BindComponentAudioSource()
  {
    MonoWrapper::BindClassFunction(get_IsActive_Internal, "get_IsActive_Internal",
      "AudioSource");
    MonoWrapper::BindClassFunction(set_IsActive_Internal, "set_IsActive_Internal",
      "AudioSource");

    MonoWrapper::BindClassFunction(get_Size_Internal, "get_Size_Internal",
      "AudioSource");
    MonoWrapper::BindClassFunction(get_Data_Internal, "get_Data_Internal",
      "AudioSource");
  }

  bool get_IsActive_Internal(ComponentAudioSource* aud)
  {
    return aud->isActive;
  }

  void set_IsActive_Internal(ComponentAudioSource* aud, bool val)
  {
    aud->isActive = val;
  }

  int get_Size_Internal(ComponentAudioSource* aud)
  {
    return (int)aud->MyAudios.size();
  }

  MonoObject* get_Data_Internal(ComponentAudioSource* aud, int _index)
  {
    if (_index < 0 || _index >= aud->MyAudios.size())
    {
      std::string error = "audioDatas _index out of range: " + std::to_string(_index) + " for " + aud->ser_name + ", id: " + std::to_string(aud->objId);
      TracyMessage(error.c_str(), error.size());
      SPEEDLOG(error);
      return nullptr;
    }
    
    ComponentAudioSource::data& MyData = aud->MyAudios.at(_index);
    MonoObject* monoObj = MonoWrapper::ConstructObject("AudioData");
    MonoWrapper::SetNativeHandle(monoObj, &MyData);

    return monoObj;
  }
}
