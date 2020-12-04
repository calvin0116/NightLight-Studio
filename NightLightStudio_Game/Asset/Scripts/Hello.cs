using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    Transform trans;

    public override void Init()
    {
      trans = GetTransform(id);
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {

    }
    public override void FixedUpdate()
    {
      if(Input.GetKeyPress(VK.IKEY_SPACE))
        trans.SetPosition(new Vector3(0.0f, trans.GetPosition().y + 1000.0f, 0.0f));
    }

    public override void OnCollisionEnter(int other) { /*Console.WriteLine("Collision Enter!");*/ }
    public override void OnCollisionStay(int other) { /*Console.WriteLine("Collision Stay!");*/ }
    public override void OnCollisionExit(int other) { /*Console.WriteLine("Collision Exit!");*/ }

    public override void OnTriggerEnter(int other) { /*Console.WriteLine("Trigger Enter!");*/ }
    public override void OnTriggerStay(int other) { /*Console.WriteLine("Trigger Stay!");*/ }
    public override void OnTriggerExit(int other) { /*Console.WriteLine("Trigger Exit!");*/ }

    public override void Exit() { /*Console.WriteLine("Exit from C#");*/ }
  }
}