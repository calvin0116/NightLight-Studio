using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    public int test = 0;
    public string test2 = "AyeAyeAye!";

    public override void Init()
    {
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
      {
        Print(test2);
        Console.WriteLine(test2);
      }
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