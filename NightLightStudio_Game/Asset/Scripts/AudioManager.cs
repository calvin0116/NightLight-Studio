
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class AudioManager : UniBehaviour
  {
    // Variables  
    int cc2;
    int cc3;

    //Required Components
    //Variables ObjVariables;
   
    // Rigidbody

    //Variable Component Values
  

   //int bgm;


    // Getting Script
   // ScriptPlayer script_Player;
    //ScriptCamera script_Cam;

    public override void Init()
    {
     
      Audio.PlayBGM("6");
      
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
    
    public int PlayAudio (int index)
    {
      int chnl = -1;
      switch (index)
      {
        case 1:
          chnl = Audio.PlayBGM("0");
          break;

        case 2:
          chnl = Audio.PlayBGM("1");
          break;

        case 3:
          chnl = Audio.PlayBGM("2");
          break;

        case 4:
          chnl = Audio.PlayBGM("3");
          break;

        case 5:
          chnl = Audio.PlayBGM("4");
          break;

        case 6:
          chnl = Audio.PlayBGM("5");
          //Audio.SetBGMVol(0.5f);
          break;

        case 7:
          chnl = Audio.PlayBGM("6");
          //Audio.SetBGMVol(0.5f);
          break;
        

      }

      return chnl;
    }


    public int PlaySFX(int index)
    {
      cc2 = 0;
      switch (index)
      {
        case 1:
          cc2 = Audio.PlayBGM("7");
          break;

        case 2:
        Audio.PlayOnce("8");
          break;

      }

      return cc2;
    }




    public override void OnTriggerEnter(int other)
    {
     
      

    }

    public override void Exit()
    {

    }
  }
}
