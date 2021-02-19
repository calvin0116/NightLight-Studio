using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    public Emitter emt;

    public override void Init()
    {
      //emt = GetEmitter(id);
    }

    public override void LateInit()
    {
      //Print(emt.ID.ToString());
    }

    public override void Update()
    {

    }
    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other) {  }
    public override void OnCollisionStay(int other) {  }
    public override void OnCollisionExit(int other) {  }

    public override void OnTriggerEnter(int other) {  }
    public override void OnTriggerStay(int other) {  }
    public override void OnTriggerExit(int other) {  }

    public override void Exit() {  }
  }
}