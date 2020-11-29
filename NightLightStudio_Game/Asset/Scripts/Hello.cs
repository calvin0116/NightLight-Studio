using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    Navigator nav;
    
    public override void Init()
    {
      nav = GetNavigator(id);
    }

    public override void LateInit()
    {
      Console.WriteLine(nav.speed);
      Console.WriteLine(nav.isFollowing);
      Console.WriteLine(nav.isPaused);
    }

    public override void Update()
    {
    }
    public override void FixedUpdate() { }

    public override void OnCollisionEnter(int other) { Console.WriteLine("Collision Enter!"); }
    public override void OnCollisionStay(int other) { Console.WriteLine("Collision Stay!"); }
    public override void OnCollisionExit(int other) { Console.WriteLine("Collision Exit!"); }

    public override void OnTriggerEnter(int other) { Console.WriteLine("Trigger Enter!"); }
    public override void OnTriggerStay(int other) { Console.WriteLine("Trigger Stay!"); }
    public override void OnTriggerExit(int other) { Console.WriteLine("Trigger Exit!"); }

    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}