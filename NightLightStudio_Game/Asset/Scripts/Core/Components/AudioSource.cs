using System;
//For MethodImpl
using System.Runtime.CompilerServices;


namespace Unicorn
{
  public class AudioSource
  {
    // Native handle for this component
    private IntPtr native_handle = IntPtr.Zero;

    public bool isActive
    {
      get { return get_IsActive_Internal(native_handle); }
      set { set_IsActive_Internal(native_handle, value); }
    }

    public AudioData[] audioDatas
    {
      get
      {
        int size = Size();
        AudioData[] temp = new AudioData[size];
        for (int i = 0; i < size; ++i)
          temp[i] = get_Data_Internal(native_handle, i);
        return temp;
      }
      private set { }
    }

    public int Size()
    {
      return get_Size_Internal(native_handle);
    }

    public AudioData GetData(int _index)
    {
      return get_Data_Internal(native_handle, _index);
    }

    //public bool IsPlaying(int _index)
    //{
    //  return get_IsPlaying_Internal(native_handle, _index);
    //}
    //public void Play(int _index)
    //{
    //  Play_Internal(native_handle, _index);
    //}

    //public bool IsMuted(int _index)
    //{
    //  return get_IsMuted_Internal(native_handle, _index);
    //}
    //public void Mute(int _index, bool _mute)
    //{
    //  Mute_Internal(native_handle, _index, _mute);
    //}


    // Getter/Setter for isActive
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool get_IsActive_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_IsActive_Internal(IntPtr native_handle, bool val);

    // Getter/Setter for size
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int get_Size_Internal(IntPtr native_handle);
    // Getter/Setter for audioData
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static AudioData get_Data_Internal(IntPtr native_handle, int _index);
  }
}

