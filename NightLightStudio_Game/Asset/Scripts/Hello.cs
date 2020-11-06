using System;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    public override void Init() { Console.WriteLine("Hello from C#"); }
    public override void Update() { }
    public override void FixedUpdate() { }
    public override void Exit() { }
  }
}