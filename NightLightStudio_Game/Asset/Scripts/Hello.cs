using System;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {    
    public override void Init()
    {
      Console.WriteLine("Hello from C#");
      Console.WriteLine(id);
    }
    public override void Update()
    {
      // Console.WriteLine("Update");
      Force.Apply(id, Vector3.up, 2000.0f);
    }
    public override void FixedUpdate() { }
    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}