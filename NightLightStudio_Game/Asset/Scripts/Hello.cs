using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {
    public float vol = 1.0f;
    int chnl;

    public override void Init()
    {
      chnl = Audio.PlayBGM("0");
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      if (Input.GetKeyHold(VK.IKEY_V))
      {
        vol += 0.1f;
        Audio.SetVol(chnl, vol);
      }
      if (Input.GetKeyHold(VK.IKEY_C))
      {
        vol -= 0.1f;
        Audio.SetVol(chnl, vol);
      }
      if (Input.GetKeyPress(VK.IKEY_SPACE))
      {
        vol = 1.0f;
        Audio.SetVol(chnl, vol);
      }
      if(Input.GetKeyPress(VK.IKEY_F))
      {
        Audio.FadeOut(chnl, 3.0f);
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