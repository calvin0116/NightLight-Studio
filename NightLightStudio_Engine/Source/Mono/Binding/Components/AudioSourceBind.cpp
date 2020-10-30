#include "AudioSourceBind.h"
#include "mono/MonoWrapper.h"

namespace Neon
{
  namespace AudioSourceBind
  {
    void BindAudioSource()
    {
      MonoWrapper::BindClassFunction(get_Active_Internal, "get_Active_Internal",
        "AudioSource");
      MonoWrapper::BindClassFunction(set_Active_Internal, "set_Active_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_Path_Internal, "get_Path_Internal",
        "AudioSource");
      MonoWrapper::BindClassFunction(set_Path_Internal, "set_Path_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_Name_Internal, "get_Name_Internal",
        "AudioSource");
      MonoWrapper::BindClassFunction(set_Name_Internal, "set_Name_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_PlayOnWake_Internal, "get_PlayOnWake_Internal",
        "AudioSource");
      MonoWrapper::BindClassFunction(set_PlayOnWake_Internal, "set_PlayOnWake_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_Loop_Internal, "get_Loop_Internal",
        "AudioSource");
      MonoWrapper::BindClassFunction(set_Loop_Internal, "set_Loop_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_PathVecSz_Internal, "get_PathVecSz_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_NameVecSz_Internal, "get_NameVecSz_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_PlayOnWakeVecSz_Internal, "get_PlayOnWakeVecSz_Internal",
        "AudioSource");

      MonoWrapper::BindClassFunction(get_LoopVecSz_Internal, "get_LoopVecSz_Internal",
        "AudioSource");
    }

    bool get_Active_Internal(const AudioSource* audioSource)
    {
      return audioSource->m_bComponentIsActive;
    }

    void set_Active_Internal(AudioSource* audioSource, bool val)
    {
      audioSource->m_bComponentIsActive = val;
    }

    MonoString* get_Path_Internal(const AudioSource* audioSource, int index)
    {
      return MonoWrapper::ToMonoString(audioSource->m_AudioPath[index]);
    }

    void set_Path_Internal(AudioSource* audioSource, int index, MonoString* val)
    {
      audioSource->m_AudioPath[index] = MonoWrapper::ToMyString(val);
    }

    MonoString* get_Name_Internal(const AudioSource* audioSource, int index)
    {
      return MonoWrapper::ToMonoString(audioSource->m_AudioName[index]);
    }

    void set_Name_Internal(AudioSource* audioSource, int index, MonoString* val)
    {
      audioSource->m_AudioName[index] = MonoWrapper::ToMyString(val);
    }

    bool get_PlayOnWake_Internal(const AudioSource* audioSource, int index)
    {
      return audioSource->m_PlayOnWake[index];
    }

    void set_PlayOnWake_Internal(AudioSource* audioSource, int index, bool val)
    {
      audioSource->m_PlayOnWake[index] = val;
    }

    bool get_Loop_Internal(const AudioSource* audioSource, int index)
    {
      return audioSource->m_Loop[index];
    }

    void set_Loop_Internal(AudioSource* audioSource, int index, bool val)
    {
      audioSource->m_Loop[index] = val;
    }

    size_t get_PathVecSz_Internal(const AudioSource* audioSource)
    {
      return audioSource->m_AudioPath.size();
    }

    size_t get_NameVecSz_Internal(const AudioSource* audioSource)
    {
      return audioSource->m_AudioName.size();
    }

    size_t get_PlayOnWakeVecSz_Internal(const AudioSource* audioSource)
    {
      return audioSource->m_PlayOnWake.size();
    }

    size_t get_LoopVecSz_Internal(const AudioSource* audioSource)
    {
      return audioSource->m_Loop.size();
    }
  }
}
