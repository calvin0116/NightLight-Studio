using System;
//For MethodImpl
using System.Runtime.CompilerServices;

namespace Unicorn
{
  public class Vector3
  {
    private IntPtr native_handle = IntPtr.Zero;

    private float X = 0.0f;
    private float Y = 0.0f;
    private float Z = 0.0f;

    public Vector3() : this(0.0f, 0.0f, 0.0f) { }

    public Vector3(float _x, float _y, float _z)
    {
      X = _x;
      Y = _y;
      Z = _z;
    }
    
    // Copy constructor.
    public Vector3(Vector3 rhs)
    {
        X = rhs.x;
        Y = rhs.y;
        Z = rhs.z;
        native_handle = IntPtr.Zero;
    }

    public float x
    {
      get
      {
        if (native_handle != IntPtr.Zero)
        {
          Console.WriteLine("InteropGetX");
          X = get_xAxis_Internal(this.native_handle);
        }
        return X;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
        {
          Console.WriteLine("InteropSetX");
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
          Console.WriteLine("InteropGetY");
          Y = get_xAxis_Internal(this.native_handle);
        }
        return Y;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
        {
          Console.WriteLine("InteropSetY");
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
          Console.WriteLine("InteropGetZ");
          Z = get_zAxis_Internal(this.native_handle);
        }
        return Z;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
        {
          Console.WriteLine("InteropSetZ");
          set_zAxis_Internal(this.native_handle, value);
        }
        Z = value;
      }
    }
  
    public static Vector3 down
    {
      get
      {
        return new Vector3(0.0f, -1.0f, 0.0f);
      }
    }
  
    public static Vector3 left
    {
      get
      {
        return new Vector3(-1.0f, 0.0f, 0.0f);
      }
    }
  
    public static Vector3 right
    {
      get
      {
        return new Vector3(1.0f, 0.0f, 0.0f);
      }
    }
  
    public static Vector3 up
    {
      get
      {
        return new Vector3(0.0f, 1.0f, 0.0f);
      }
    }
    
    public float sqrMagnitude
    {
      get
      {
        return x*x + y*y + z*z;
      }
    }
    
    public float magnitude
    {
      get
      {
        return (float)Math.Sqrt((double)sqrMagnitude);  
      }    
    }
    
    public Vector3 normalized
    {
      get
      {
        float mag = magnitude;
        if(mag == 0)
        {
          return new Vector3(0.0f, 0.0f, 0.0f);
        }

        Vector3 result = new Vector3();
        result = this;
        return result / mag;
      }
    }
  
    public static Vector3 operator +(Vector3 lhs, Vector3 rhs)
    {
      Vector3 result = new Vector3(lhs.x + rhs.x, lhs.y + rhs.y,
      lhs.z + rhs.z);
      return result;
    }
  
    public static Vector3 operator -(Vector3 lhs, Vector3 rhs)
    {
      Vector3 result = new Vector3(lhs.x - rhs.x, lhs.y - rhs.y,
      lhs.z - rhs.z);
      return result;
    }

    // Unary -
    public static Vector3 operator -(Vector3 rhs)
    {
      Vector3 result = new Vector3(-rhs.x, -rhs.y, -rhs.z);
      return result;
    }

    public static Vector3 operator *(Vector3 vec, float f)
    {
      Vector3 result = new Vector3(vec.x * f, vec.y * f, vec.z * f);
      return result;
    }

    public static Vector3 operator *(float f, Vector3 vec)
    {
      return vec * f;
    }

    // Dot product
    public static float operator *(Vector3 vec1, Vector3 vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }
    
    public static Vector3 operator /(Vector3 vec, float f)
    {
      return vec * (1 / f);
    }
    
    public static bool operator ==(Vector3 lhs, Vector3 rhs)
    {
      return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
    
    public static bool operator !=(Vector3 lhs, Vector3 rhs)
    {
      return !(lhs == rhs);
    }

    public static Vector3 Cross(Vector3 lhs, Vector3 rhs)
    {
      Vector3 result = new Vector3();
      result.x = lhs.y * rhs.z - lhs.z * rhs.y;
      result.y = lhs.z * rhs.x - lhs.x * rhs.z;
      result.z = lhs.x * rhs.y - lhs.y * rhs.x;
      return result;
    }

    public override bool Equals(object obj)
    {
      return this == (Vector3)obj;
    }
    
    public override int GetHashCode()
    {
      return 0;
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
  }
}
