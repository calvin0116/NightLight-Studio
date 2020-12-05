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
      {
        cnvs.isActive = !cnvs.isActive;
        //Print(ui1.isActive.ToString());
        //Vector3 a = new Vector3(0.0f, 0.0f, 0.0f);
        //ui1.SetColour(a);
      }
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