using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class CanvasTest : UniBehaviour
  {
    public string UIName = "";
    public string UIName2 = "";
    Canvas cnvs;
    UIElement ui1;
    UIElement ui2;

    public override void Init()
    {
      cnvs = GetCanvas(id);
      Print(id.ToString());
      ui1 = cnvs.FindUI(UIName);
      ui2 = cnvs.FindUI(UIName2);
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      if (ui1.OnClick())
        Print("Click Blue!");
      if (ui2.OnClick())
        Print("Click Red!");
      if (Input.GetKeyPress(VK.IKEY_SPACE))
      {
        cnvs.isActive = !cnvs.isActive;
      }
      if (ui1.OnEnter())
        Print("ENter");
      if (ui1.OnExit())
        Print("Exit");
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