using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Unicorn
{
  public class UniBehaviour
  {
    //Entity id
    public int id = 0;

    // public virtual void Awake() { }
    public virtual void Init() { }
    public virtual void LateInit() { }
    public virtual void Update() { }
    public virtual void FixedUpdate() { }
    public virtual void Exit() { }

    public virtual void OnEnable() { }
    public virtual void OnDisable() { }

    public virtual void OnCollisionEnter(int other) { }
    public virtual void OnCollisionStay(int other) { }
    public virtual void OnCollisionExit(int other) { }

    public virtual void OnTriggerEnter(int other) { }
    public virtual void OnTriggerStay(int other) { }
    public virtual void OnTriggerExit(int other) { }

    // Useful functions
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int GameObjectFind(string name);
    // Delta time
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float DT();
    // Delta time in seconds
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float RealDT();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static dynamic GetScript(int id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Transform GetTransform(int id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Collider GetCollider(int id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static RigidBody GetRigidBody(int id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Graphics GetGraphics(int id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Light GetLight(int id);

    // Temporary till reflection comes out
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Variables GetVariables(int id);
  }
}
