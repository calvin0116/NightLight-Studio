
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class AudioManager : UniBehaviour
  {
    // Variables
    int cc1;
    int cc2;
    int cc3;
    int cc4;
    int cc5;
    int cc6;

    //Required Components
    Variables ObjVariables;
   
    // Rigidbody

    //Variable Component Values
    public static String audioPath1;
    public static String audioPath2;
    public static String audioPath3;
    public static String audioPath4;
    public static String audioPath5;
    public static String audioPath6;


    // Getting Script
    ScriptPlayer script_Player;
    //ScriptCamera script_Cam;

    public override void Init()
    {
    

      ObjVariables = GetVariables(id);

      audioPath1 = ObjVariables.GetString(0);
      audioPath2 = ObjVariables.GetString(1);
      audioPath3 = ObjVariables.GetString(2);
      audioPath4 = ObjVariables.GetString(3);
      audioPath5 = ObjVariables.GetString(4);
      audioPath6 = ObjVariables.GetString(5);

      cc1 = 



    }

    public override void LateInit()
    {
 
      //script_Cam = GetScript(cam_ID);
    }

    public override void Update()
    {
      
    }
    public override void FixedUpdate()
    {

    }
    
    public void PlayAudio (int index)
    {
      switch (index)
      {

        case 1:

          Audio.PlayOnce(audioPath1);
          Console.WriteLine("playing1)

         break;
        case 2:

          Audio.PlayOnce(audioPath2);

          break;
        case 3:

          Audio.PlayOnce(audioPath3);

          break;
        case 4:

          Audio.PlayOnce(audioPath4);

          break;
        case 5:

          Audio.PlayOnce(audioPath5);

          break;

        case 6:

          Audio.PlayOnce(audioPath6);

          break;


      }
    }
   

    public override void OnTriggerEnter(int other)
    {
     
      

    }

    public override void Exit()
    {

    }
  }
}
