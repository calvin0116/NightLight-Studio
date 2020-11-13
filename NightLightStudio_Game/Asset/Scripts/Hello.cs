using System;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {    
    int findID = -1;
    
    public override void Init()
    {
      Console.WriteLine("Hello from C#");
      Console.WriteLine(id);
      findID = GameObjectFind("Camera");
      Console.WriteLine("The ID found is :");
      Console.WriteLine(findID);
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