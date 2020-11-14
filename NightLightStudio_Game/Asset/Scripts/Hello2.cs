using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello2 : UniBehaviour
  {
    int oID = -1;
    Hello script;

    public override void Init()
    {
      oID = GameObjectFind("Entity_0");
      Console.WriteLine(oID);
      script = GetScript(oID);
      script.findID = GameObjectFind("Entity_1");
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      // Console.WriteLine("Update");
      // Force.Apply(id, Vector3.up, 2000.0f);
    }
    public override void FixedUpdate() { }
    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}