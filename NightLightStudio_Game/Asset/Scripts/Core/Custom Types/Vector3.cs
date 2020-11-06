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
          X = get_Axis_Internal(this.native_handle, 0);
        return X;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
          set_Axis_Internal(this.native_handle, value, 0);
        X = value;
      }
    }

    public float y
    {
      get
      {
        if (native_handle != IntPtr.Zero)
          Y = get_Axis_Internal(this.native_handle, 1);
        return Y;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
          set_Axis_Internal(this.native_handle, value, 1);
        Y = value;
      }
    }

    public float z
    {
      get
      {
        if (native_handle != IntPtr.Zero)
          Z = get_Axis_Internal(this.native_handle, 2);
        return Z;
      }
      set
      {
        if (native_handle != IntPtr.Zero)
          set_Axis_Internal(this.native_handle, value, 2);
        Z = value;
      }
    }
    
    public static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta)
    {
      Vector3 dir = target - current;
      dir.z = 0.0f;
      float dist = dir.magnitude;
      
      if(dist <= maxDistanceDelta || dist == 0.0f)
        return new Vector3(target.x, target.y, current.z);
    
      return current + dir / dist * maxDistanceDelta;          
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
        return x*x + y*y;
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
        Vector3 result = new Vector3();
        result = this;
        return result / mag;
      }
    }
  
    public static Vector3 operator +(Vector3 lhs, Vector3 rhs)
    {
      Vector3 result = new Vector3(lhs.x + rhs.x, lhs.y + rhs.y,
      lhs.z);
      return result;
    }
  
    public static Vector3 operator -(Vector3 lhs, Vector3 rhs)
    {
      Vector3 result = new Vector3(lhs.x - rhs.x, lhs.y - rhs.y,
      lhs.z);
      return result;
    }
    
    public static Vector3 operator *(Vector3 vec, float f)
    {
      Vector3 result = new Vector3(vec.x * f, vec.y * f, vec.z);
      return result;
    }

    public static float operator *(Vector3 vec1, Vector3 vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    public static Vector3 operator *(float f, Vector3 vec)
    {
      return vec * f;
    }
    
    public static Vector3 operator /(Vector3 vec, float f)
    {
      return vec * (1 / f);
    }
    
    public static bool operator ==(Vector3 lhs, Vector3 rhs)
    {
      return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    
    public static bool operator !=(Vector3 lhs, Vector3 rhs)
    {
      return !(lhs == rhs);
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
    public extern static float get_Axis_Internal(IntPtr native_handle, int index);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void set_Axis_Internal(IntPtr native_handle, float val, int index);
  }
}
