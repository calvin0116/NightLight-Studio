using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class AudioTest : UniBehaviour
  {
    Transform trans;
    RigidBody rb;
    int leftBGM = -1;
    int rightBGM = -1;
    int ent = -100;

    public override void Init()
    {
      trans = GetTransform(id);
      if (rb == null)
        Print("NULL");
      rb = GetRigidBody(id);
      RigidBody rb1 = GetRigidBody(GameObjectFind("Entity_0"));
      if (rb1 == null)
        Print("rb1 null");
      Print(ent.ToString());
    }

    public override void LateInit()
    {
      Print("My ID: " + id.ToString());
    }

    public override void Update()
    {

    }
    public override void FixedUpdate()
    {
      if (Input.GetKeyPress(VK.IKEY_Q))
        Audio.Play3DOnce("0", GameObjectFind("Left"));
      if (Input.GetKeyPress(VK.IKEY_E))
        Audio.Play3DOnce("0", GameObjectFind("Right"));
      if (Input.GetKeyPress(VK.IKEY_Z))
        leftBGM = Audio.Play3DLoop("0", GameObjectFind("Left"));
      if (Input.GetKeyPress(VK.IKEY_C))
        rightBGM = Audio.Play3DLoop("0", GameObjectFind("Right"));
      if (Input.GetKeyPress(VK.IKEY_X))
      {
        Audio.Stop(leftBGM);
        Audio.Stop(rightBGM);
      }
      Vector3 vel = new Vector3(0.0f, rb.GetVel().y, 0.0f);
      if (Input.GetKeyHold(VK.IKEY_A))
        vel.x = -100.0f;
      else if (Input.GetKeyHold(VK.IKEY_D))
        vel.x = +100.0f;
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