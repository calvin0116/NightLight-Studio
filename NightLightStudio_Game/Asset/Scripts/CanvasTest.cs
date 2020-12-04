using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class CanvasTest : UniBehaviour
  {

    Canvas cnvs;
    UIElement ui1;

    public override void Init()
    {
      cnvs = GetCanvas(id);
      Print(id.ToString());
      ui1 = cnvs.FindUI("Cart");
      if (ui1.native_handle == IntPtr.Zero)
        Print("Null ptr!");
      else
        Print("Value received: " + ui1.native_handle.ToString());
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      if (ui1.OnClick())
      {
        Print("Click Blue!");
      }
      if (Input.GetKeyPress(VK.IKEY_SPACE))
        Print("Derp");
    }
    public override void FixedUpdate()
    {
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