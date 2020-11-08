using System;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {    
    public override void Init()
    {
      Console.WriteLine("Hello from C#");
      Console.WriteLine(id);
      // Force.Apply(id, Vector3.down, 1.0f);
    }
    public override void Update()
    {
      // Console.WriteLine("Update");
    }
    public override void FixedUpdate() { }
    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}