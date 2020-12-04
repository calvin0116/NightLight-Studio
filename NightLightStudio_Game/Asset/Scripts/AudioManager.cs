
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class AudioManager : UniBehaviour
  {
    // Variables  

    //Required Components
    //Variables ObjVariables;
   
    // Rigidbody

    //Variable Component Values
  

   int bgm;


    // Getting Script
   // ScriptPlayer script_Player;
    //ScriptCamera script_Cam;

    public override void Init()
    {
    

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

          Audio.PlayOnce("0");
          

         break;
        case 2:

         // Audio.PlayOnce(audioPath2);

          break;
        case 3:

         // Audio.PlayOnce(audioPath3);

          break;
        case 4:

         // Audio.PlayOnce(audioPath4);

          break;
        case 5:

        //  Audio.PlayOnce(audioPath5);

          break;

        case 6:

          bgm = Audio.PlayBGM("5");

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
