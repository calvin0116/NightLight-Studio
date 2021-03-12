using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    //public int size;
    public bool[] MyBoolArr;

    public override void Init()
    {
      //MyStringArr = new string[size];
    }

    public override void LateInit()
    {
      //MyStringArr[0] = "Hello";
      //MyStringArr[1] = "How";
      //MyStringArr[2] = "YouDoing?";
      //for (int i = 0; i < size; ++i)
      //  Print(MyStringArr[i]);
    }

    public override void Update()
    {
      if (Input.GetKeyHold(VK.IKEY_V))
      {
      }
      if (Input.GetKeyHold(VK.IKEY_C))
      {
      }
      if (Input.GetKeyPress(VK.IKEY_SPACE))
      {
      }
      if(Input.GetKeyPress(VK.IKEY_F))
      {
      }
    }
    public override void FixedUpdate()
    {
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