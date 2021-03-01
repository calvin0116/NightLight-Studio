using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
  public class Vector4
  {
    private IntPtr native_handle = IntPtr.Zero;

    private float X = 0.0f;
    private float Y = 0.0f;
    private float Z = 0.0f;
    private float W = 0.0f;

    public Vector4() : this(0.0f, 0.0f, 0.0f, 0.0f) { }

    public Vector4(float _x, float _y, float _z, float _w)
    {
      X = _x;
      Y = _y;
      Z = _z;
      W = _w;
    }
    
    // Copy constructor.
    public Vector4(Vector4 rhs)
    {
        X = rhs.x;
        Y = rhs.y;
        Z = rhs.z;
        W = rhs.w;
        native_handle = IntPtr.Zero;
    }

    public float x
    {
      get
      {
        if (native_handle != IntPtr.Zero)
        {
          X = get_xAxis_Internal(this.native_handle);
        }
        return X;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
        {
          set_xAxis_Internal(this.native_handle, value);
        }
        X = value;
      }
    }

    public float y
    {
      get
      {
        if (native_handle != IntPtr.Zero)
        {
          Y = get_xAxis_Internal(this.native_handle);
        }
        return Y;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
        {
          set_xAxis_Internal(this.native_handle, value);
        }
        Y = value;
      }
    }

    public float z
    {
      get
      {
        if (native_handle != IntPtr.Zero)
        {
          Z = get_zAxis_Internal(this.native_handle);
        }
        return Z;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
        {
          set_zAxis_Internal(this.native_handle, value);
        }
        Z = value;
      }
    }

    public float w
    {
      get
      {
        if (native_handle != IntPtr.Zero)
        {
          W = get_wAxis_Internal(this.native_handle);
        }
        return W;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
        {
          set_wAxis_Internal(this.native_handle, value);
        }
        W = value;
      }
    }
  
    public static Vector4 operator +(Vector4 lhs, Vector4 rhs)
    {
      Vector4 result = new Vector4(lhs.x + rhs.x, lhs.y + rhs.y,
      lhs.z + rhs.z, lhs.w + rhs.w);
      return result;
    }

    public static Vector4 operator +(Vector4 lhs, Vector3 rhs)
    {
      Vector4 result = new Vector4(lhs.x + rhs.x, lhs.y + rhs.y,
      lhs.z + rhs.z, lhs.w);
      return result;
    }

    public static Vector4 operator +(Vector3 lhs, Vector4 rhs)
    {
      return rhs + lhs;
    }

    public static Vector4 operator -(Vector4 lhs, Vector4 rhs)
    {
      Vector4 result = new Vector4(lhs.x - rhs.x, lhs.y - rhs.y,
      lhs.z - rhs.z, lhs.w - rhs.w);
      return result;
    }

    public static Vector4 operator -(Vector4 lhs, Vector3 rhs)
    {
      Vector4 result = new Vector4(lhs.x - rhs.x, lhs.y - rhs.y,
      lhs.z - rhs.z, lhs.w);
      return result;
    }

    public static Vector4 operator -(Vector3 lhs, Vector4 rhs)
    {
      Vector4 result = new Vector4(lhs.x - rhs.x, lhs.y - rhs.y,
      lhs.z - rhs.z, -rhs.w);
      return result;
    }

    // Unary -
    public static Vector4 operator -(Vector4 rhs)
    {
      Vector4 result = new Vector4(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
      return result;
    }

    public static Vector4 operator *(Vector4 vec, float f)
    {
      Vector4 result = new Vector4(vec.x * f, vec.y * f, vec.z * f, vec.w * f);
      return result;
    }

    public static Vector4 operator *(float f, Vector4 vec)
    {
      return vec * f;
    }

    public static Vector4 operator /(Vector4 vec, float f)
    {
      return vec * (1 / f);
    }

    // Dot product
    public static float operator *(Vector4 vec1, Vector4 vec2)
    {
      return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }
    public static float operator *(Vector4 vec1, Vector3 vec2)
    {
      return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }
    public static float operator *(Vector3 vec1, Vector4 vec2)
    {
      return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }
    
    public static bool operator ==(Vector4 lhs, Vector4 rhs)
    {
      return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
    }
    
    public static bool operator !=(Vector4 lhs, Vector4 rhs)
    {
      return !(lhs == rhs);
    }

    public override bool Equals(object obj)
    {
      return this == (Vector4)obj;
    }

    public override int GetHashCode()
    {
      return 4;
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_xAxis_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_xAxis_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_yAxis_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_yAxis_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_zAxis_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_zAxis_Internal(IntPtr native_handle, float val);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float get_wAxis_Internal(IntPtr native_handle);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_wAxis_Internal(IntPtr native_handle, float val);
  }
}
