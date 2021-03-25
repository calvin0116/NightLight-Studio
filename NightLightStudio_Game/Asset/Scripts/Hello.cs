using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    //public int size;
    //public bool[] MyBoolArr;
    Transform trans;
    int ListenerID;
    Transform lisTrans;
    public string Listener = "Listener";
    public float speed = 1.0f;
    public float rotSpeed = 1.0f;

    public override void Init()
    {
      //MyStringArr = new string[size];
      trans = GetTransform(id);
      ListenerID = GameObjectFind(Listener);
      lisTrans = GetTransform(ListenerID);
    }

    public override void LateInit()
    {
      //MyStringArr[0] = "Hello";
      //MyStringArr[1] = "How";
      //MyStringArr[2] = "YouDoing?";
      //for (int i = 0; i < size; ++i)
      //  Print(MyStringArr[i]);
    }

    public override void Update()
    {
      float realSpeed = speed * RealDT();
      float realRot = rotSpeed * RealDT();
      if (Input.GetKeyHold(VK.IKEY_A))
      {
        Vector3 left = new Vector3( -realSpeed, 0.0f, 0.0f );
        left = left + trans.GetPosition();
        trans.SetPosition(left);
      }
      if (Input.GetKeyHold(VK.IKEY_D))
      {
        Vector3 right = new Vector3(realSpeed, 0.0f, 0.0f);
        right = right + trans.GetPosition();
        trans.SetPosition(right);
      }

      if (Input.GetKeyHold(VK.IKEY_C))
      {
        Vector3 left = new Vector3(0.0f, rotSpeed, 0.0f);
        left = left + lisTrans.GetRotation();
        lisTrans.SetRotation(left);
      }

      if(Input.GetKeyHold(VK.IKEY_V))
      {
        Vector3 right = new Vector3(0.0f, -rotSpeed, 0.0f);
        right = right + lisTrans.GetRotation();
        lisTrans.SetRotation(right);
      }
    }
    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other) { Console.WriteLine("Collision Enter!"); }
    public override void OnCollisionStay(int other) { Console.WriteLine("Collision Stay!"); }
    public override void OnCollisionExit(int other) { Console.WriteLine("Collision Exit!"); }

    public override void OnTriggerEnter(int other) { Console.WriteLine("Trigger Enter!"); }
    public override void OnTriggerStay(int other) { Console.WriteLine("Trigger Stay!"); }
    public override void OnTriggerExit(int other) { Console.WriteLine("Trigger Exit!"); }

    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}