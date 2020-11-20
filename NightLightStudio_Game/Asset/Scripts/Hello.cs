using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {    
    public int findID = -1;
    
    public override void Init()
    {
      Console.WriteLine("Key");
    }

    public override void LateInit()
    {
      findID = GameObjectFind("Entity_0");
    }

    public override void Update()
    {
      // Console.WriteLine("Update");
      // Force.Apply(id, Vector3.up, 2000.0f);
      if(Input.GetKeyPress(VK.IKEY_W))
        Console.WriteLine("W Pressed!");
      if(Input.GetKeyHold(VK.IKEY_W))
        Console.WriteLine("W Hold!");
      if(Input.GetKeyUp(VK.IKEY_W))
        Console.WriteLine("W released!");
    }
    public override void FixedUpdate() { }
    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}