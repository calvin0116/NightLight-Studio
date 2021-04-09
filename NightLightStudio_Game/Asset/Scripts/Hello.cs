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
    AudioSource audSource;
    //AudioData[] myDatas;
    AudioData MyData;
    int ListenerID;
    Transform lisTrans;
    public string Listener = "Listener";
    public float speed = 1.0f;
    public float rotSpeed = 1.0f;

    bool sfx = false;
    bool bgm = false;
    bool all = false;

    public override void Init()
    {
      trans = GetTransform(id);
      audSource = GetAudioSource(id);
      //myDatas = audSource.audioDatas;
      MyData = audSource.GetData(0);
      ListenerID = GameObjectFind(Listener);
      lisTrans = GetTransform(ListenerID);
    }

    public override void LateInit()
    {
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

      if (Input.GetKeyHold(VK.IKEY_Q))
      {
        Vector3 left = new Vector3(-realSpeed, 0.0f, 0.0f);
        left = left + lisTrans.GetPosition();
        lisTrans.SetPosition(left);
      }
      if (Input.GetKeyHold(VK.IKEY_E))
      {
        Vector3 right = new Vector3(realSpeed, 0.0f, 0.0f);
        right = right + lisTrans.GetPosition();
        lisTrans.SetPosition(right);
      }

      if (Input.GetKeyPress(VK.IKEY_U))
      {
        Print(Audio.GetMuteSFX().ToString());
        Print(Audio.GetMuteBGM().ToString());
        Print(Audio.GetMuteMASTER().ToString());
      }

      if (Input.GetKeyPress(VK.IKEY_I))
      {
        sfx = !sfx;
        Print(sfx.ToString());
        Audio.MuteSFX(sfx);
      }

      if (Input.GetKeyPress(VK.IKEY_O))
      {
        bgm = !bgm;
        Print(bgm.ToString());
        Audio.MuteBGM(bgm);
      }

      if (Input.GetKeyPress(VK.IKEY_P))
      {
        all = !all;
        Print(all.ToString());
        Audio.MuteMASTER(all);
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