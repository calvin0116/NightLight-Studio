
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class AudioEvent : UniBehaviour
  {
    // Variables
    int aMID;
    bool isPlaying;

    //Required Components
    Variables ObjVariables;

    // Rigidbody

    //Variable Component Values
    public static int audioIndex;


    // Getting Script


    AudioManager script_AM;

    public override void Init()
    {
      aMID = GameObjectFind("AudioManager");

      ObjVariables = GetVariables(id);

      audioIndex = ObjVariables.GetInt(0);




    }

    public override void LateInit()
    {

      script_AM = GetScript(aMID);
    }

    public override void Update()
    {

    }
    public override void FixedUpdate()
    {

    }





    public override void OnTriggerEnter(int other)
    {

      if (GetTransform(other).tag == 0)
      {
        Console.WriteLine("playing1");
        if (isPlaying == false)
        {
          script_AM.PlayAudio(audioIndex);
          isPlaying = true;
        }

      }

    }

    public override void Exit()
    {

    }
  }
}
