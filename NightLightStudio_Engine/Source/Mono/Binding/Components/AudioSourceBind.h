#pragma once
#include "" // include the component source file.h
#include <mono/metadata/object.h>

namespace Neon
{
  namespace AudioSourceBind
  {
    bool get_Active_Internal(const AudioSource* audioSource);
    void set_Active_Internal(AudioSource* audioSource, bool val);

    MonoString* get_Path_Internal(const AudioSource* audioSource, int index);
    void set_Path_Internal(AudioSource* audioSource, int index, MonoString* val);

    MonoString* get_Name_Internal(const AudioSource* audioSource, int index);
    void set_Name_Internal(AudioSource* audioSource, int index, MonoString* val);

    bool get_PlayOnWake_Internal(const AudioSource* audioSource, int index);
    void set_PlayOnWake_Internal(AudioSource* audioSource, int index, bool val);

    bool get_Loop_Internal(const AudioSource* audioSource, int index);
    void set_Loop_Internal(AudioSource* audioSource, int index, bool val);

    size_t get_PathVecSz_Internal(const AudioSource* audioSource);

    size_t get_NameVecSz_Internal(const AudioSource* audioSource);

    size_t get_PlayOnWakeVecSz_Internal(const AudioSource* audioSource);

    size_t get_LoopVecSz_Internal(const AudioSource* audioSource);

    void BindAudioSource();
  }
}
