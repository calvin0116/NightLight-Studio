
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class FanBehaviour : UniBehaviour
  {
    // Variables
    public bool activate = false;
    public int player_ID;
    public int cam_ID;
    public int p_CamPos;
    public int p_SpawnPos;
    public int windID;
    bool isActive;

    //Required Components
    Variables ObjVariables;
    // Rigidbody

    //Variable Component Values
    public static string possessionCamPos;
    public static string possessionSpawnPos;
    public static string windObjName;

    // Getting Script
    ScriptPlayer script_Player;
    WindBehaviour script_WindB;

    public override void Init()
    {
      player_ID = GameObjectFind("Player");
      //player_ID.transform.postion=
      // GetTransform(player_ID).GetPosition();
      cam_ID = GameObjectFind("PlayerCamera");

      ObjVariables = GetVariables(id);
      isActive = true;

      possessionCamPos = ObjVariables.GetString(0);
      possessionSpawnPos = ObjVariables.GetString(1);
      windObjName = ObjVariables.GetString(2);

      p_CamPos = GameObjectFind(possessionCamPos);
      p_SpawnPos = GameObjectFind(possessionSpawnPos);
      windID = GameObjectFind(windObjName);
    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);
      script_WindB = GetScript(windID);
    }

    public override void Update()
    {
      if (activate == true)
      {
        //Set Player State to possessionstate
        SwitchOnFunction();

        //if (script_Player.CurrentState == ScriptPlayer.State.Human)
        //{
        //  SwitchOffFunction();
        //  activate = false;
        //}


      }

     


    }
    public override void FixedUpdate()
    {

    }

    public override void OnCollisionEnter(int other)
    {

      // Disable Function
      if (GetTransform(other).tag == 3)
      {
        isActive = false;
        Print("Disabled");
      }

      if (isActive == true && other== player_ID)
      {

        

        if (script_Player.CurrentState == ScriptPlayer.State.Moth/* && other == player_ID && activate == false*/)
        {

          //Audio.PlayOnce("13");
          activate = true;
          // Set player script nextspawn position == possessionSpawnPos
          script_Player.NextState = ScriptPlayer.State.Possessed;


          Vector3 p_AfterSpawnPos = GetTransform(p_SpawnPos).GetPosition();
          script_Player.spawnPoint = p_AfterSpawnPos;
          // Set Camera script  position == possessionSpawnPos

        
          script_Player.camScript.tgtID = p_CamPos; // Go and expose other tgt in scriptcamera.

          // Activate energy consumption delay 
         
          //script_Player.CurrentState = ScriptPlayer.State.Possessed;

        }
      }

      //if (isActive == false)
      //{
      //  activate = false;
      //}
    }

    public override void OnCollisionExit(int other) 
    {
      if (other == player_ID)
      {
        activate = false;
      }

      if (GetTransform(other).tag == 3)
      {
        isActive = true;
      }


    }



    public void Deactivate()
    {
      isActive = !isActive;
    }
    public void SwitchOnFunction()
    {
      //Audio.PlayOnce("11");
      script_WindB.Activate();     
    }

    public void SwitchOffFunction()
    {
      //Audio.PlayOnce("12");
      script_WindB.Deactivate();
    }


    public override void OnTriggerEnter(int other)
    {

    }

    public override void Exit()
    {

    }
  }
}
