using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {    
    int findID = -1;
    Collider col;
    RigidBody rb;
    Graphics g;

    public override void Init()
    {

    }

    public override void LateInit()
    {
      Console.WriteLine("Hello from C#");
      Console.WriteLine(id);
      findID = GameObjectFind("Entity_0");
      Console.WriteLine("The ID found is :");
      Console.WriteLine(findID);
      Audio.PlayOnce("TestAudio");
      col = GetCollider(findID);
      Console.WriteLine(col.isCollidable);
      Console.WriteLine(col.tag);
      col.setCenter(new Vector3(1.0f, 1.0f, 1.0f));
      Console.WriteLine(col.getCenter().x);
      Console.WriteLine(col.getCenter().y);
      Console.WriteLine(col.getCenter().z);
      //Console.WriteLine(trans.getPosition().x);
      //trans.pos.x = 10.0f;
      rb = GetRigidBody(findID);
      Console.WriteLine(rb.isStatic);
      rb.isStatic = false;
      rb.isGravity = false;
      rb.mass = 100.0f;
      rb.friction = 1.0f;
      g = GetGraphics(findID);
      g.AddModel("\\Asset\\3D model\\mothtest.FBX");
      g.isActive = false;
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