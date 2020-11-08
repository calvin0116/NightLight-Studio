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
    public override void Update() { }
    public override void FixedUpdate() { }
    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}