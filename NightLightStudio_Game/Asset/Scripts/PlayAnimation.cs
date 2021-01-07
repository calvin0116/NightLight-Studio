using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class PlayAnimation: UniBehaviour
  {
    Graphics playerCharModel;
    Animation anim;

    public override void Init()
    {
      anim = GetAnimation(id);
      playerCharModel = GetGraphics(id);

    }

    public override void LateInit()
    {
    }

    public override void Update()
    {

    }
    public override void FixedUpdate()
    {
      if(Input.GetKeyPress(VK.IKEY_Q))
        anim.Play("Walk1", true, 1.9f, 3.108f);
      if (Input.GetKeyPress(VK.IKEY_W))
        anim.Play("Idle1", true, -1f,30f);
      if (Input.GetKeyPress(VK.IKEY_E))
        anim.Play("Switch1", false, -1f, 30f);

    }

    public override void OnCollisionEnter(int other) { Console.WriteLine("Collision Enter!"); }
    public override void OnCollisionStay(int other) { Console.WriteLine("Collision Stay!"); }
    public override void OnCollisionExit(int other) { Console.WriteLine("Collision Exit!"); }

    public override void OnTriggerEnter(int other) { Console.WriteLine("Trigger Enter!"); }
    public override void OnTriggerStay(int other) { Console.WriteLine("Trigger Stay!"); }
    public override void OnTriggerExit(int other) { Console.WriteLine("Trigger Exit!"); }

    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}