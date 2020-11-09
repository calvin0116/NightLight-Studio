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
  }
}
