using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class AudioTest : UniBehaviour
  {
    Transform trans;
    RigidBody rb;
    public string source1;
    public string source2;
    int leftBGM = -1;
    int rightBGM = -1;

    public override void Init()
    {
      trans = GetTransform(id);
      rb = GetRigidBody(id);
      //RigidBody rb1 = GetRigidBody(GameObjectFind("Entity_0"));
      leftBGM = GameObjectFind(source1);
      rightBGM = GameObjectFind(source2);
    }

    public override void LateInit()
    {
      //Print("My ID: " + id.ToString());
    }

    public override void Update()
    {
      if (Input.GetKeyPress(VK.IKEY_Q))
      {
        Print("Q");
        Audio.Play3DOnce("0", leftBGM);
      }
      if (Input.GetKeyPress(VK.IKEY_E))
      {
        Print("E");
        Audio.Play3DOnce("0", rightBGM);
      }
    }
    public override void FixedUpdate()
    {



      Vector3 vel = new Vector3(0.0f, rb.GetVel().y, 0.0f);
      if (Input.GetKeyHold(VK.IKEY_A))
        vel.x = -1000.0f;
      else if (Input.GetKeyHold(VK.IKEY_D))
        vel.x = +1000.0f;
      if (Input.GetKeyHold(VK.IKEY_W))
        vel.z = -1000.0f;
      else if (Input.GetKeyHold(VK.IKEY_S))
        vel.z = 1000.0f;
      rb.SetVel(vel);
    }

    public override void OnCollisionEnter(int other) { /*Console.WriteLine("Collision Enter!");*/ }
    public override void OnCollisionStay(int other) { /*Console.WriteLine("Collision Stay!");*/ }
    public override void OnCollisionExit(int other) { /*Console.WriteLine("Collision Exit!");*/ }

    public override void OnTriggerEnter(int other) { /*Console.WriteLine("Trigger Enter!");*/ }
    public override void OnTriggerStay(int other) { /*Console.WriteLine("Trigger Stay!");*/ }
    public override void OnTriggerExit(int other) { /*Console.WriteLine("Trigger Exit!");*/ }

    public override void Exit() { /*Console.WriteLine("Exit from C#");*/ }
  }
}