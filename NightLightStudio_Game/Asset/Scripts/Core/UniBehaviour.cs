using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Unicorn
{
  public class UniBehaviour
  {
    //Entity id
    private uint id = 0;

    public virtual void Awake() { }
    public virtual void Start() { }
    public virtual void Update(float dt) { }
    public virtual void FixedUpdate(float dt) { }

    public virtual void OnEnable() { }
    public virtual void OnDisable() { }

    public virtual void OnCollisionEnter(uint other) { }
    public virtual void OnCollisionStay(uint other) { }
    public virtual void OnCollisionExit(uint other) { }

    public virtual void OnTriggerEnter(uint other) { }
    public virtual void OnTriggerStay(uint other) { }
    public virtual void OnTriggerExit(uint other) { }
  }
}
