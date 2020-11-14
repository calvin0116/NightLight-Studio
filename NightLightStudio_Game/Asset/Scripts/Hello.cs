using System;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {    
    int findID = -1;
    Transform trans;
    Vector3 upVec;

    public override void Init()
    {
      Console.WriteLine("Hello from C#");
      Console.WriteLine(id);
      findID = GameObjectFind("Entity_0");
      Console.WriteLine("The ID found is :");
      Console.WriteLine(findID);
      Audio.PlayOnce("TestAudio");
      trans = GetTransform(findID);
      Console.WriteLine(trans.Name);
      //upVec = Camera.
      upVec = Camera.GetRightVector();
      Console.WriteLine(upVec.x);
      Console.WriteLine(upVec.y);
      Console.WriteLine(upVec.z);
      //Console.WriteLine(trans.getPosition().x);
      //trans.pos.x = 10.0f;
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