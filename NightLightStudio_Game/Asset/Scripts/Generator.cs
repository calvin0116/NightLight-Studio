
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Generator : UniBehaviour
  {
    // Variables
    public bool activate = false;
    public int player_ID;
    public int cam_ID;
    public int output_ID;
    public int p_CamPos;
    public int p_SpawnPos;
    bool isActive;
    public bool isWaypoint;
    bool isOn;
    bool isLamp;


    //Required Components
    Variables ObjVariables;
    // Rigidbody

    //Variable Component Values
    public static string possessionCamPos;
    public static string possessionSpawnPos;
    public static string output;
    public static int type;

    // Getting Script
    ScriptPlayer script_Player;
    LampBehaviour script_Lamp;
    FanBehaviour script_Fan;
    //ScriptCamera script_Cam;

    public override void Init()
    {
      player_ID = GameObjectFind("Player");
      cam_ID = GameObjectFind("PlayerCamera");

      ObjVariables = GetVariables(id);
      isActive = true;

     possessionCamPos = ObjVariables.GetString(0);
     possessionSpawnPos = ObjVariables.GetString(1);
     output = ObjVariables.GetString(2);
     type = ObjVariables.GetInt(0);
     

      p_CamPos = GameObjectFind(possessionCamPos);
      p_SpawnPos = GameObjectFind(possessionSpawnPos);
      output_ID = GameObjectFind(output);

      




    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);


      if (type == 0)
      {
        script_Fan = GetScript(output_ID);
      }

      else
      {
        script_Lamp = GetScript(output_ID);
      }

      activate = true;

    }

    public override void Update()
    {

      if (activate == true)
      {
        //Set Player State to possessionstate

        if (isOn == false)
        {
          SwitchOnFunction();
        }
        else
        {
          SwitchOffFunction();
        }
        //activate = false;

        //if (script_Player.CurrentState == ScriptPlayer.State.Human)
        //{
        //  //
          
        //}


      }



      


      //if (activate == false || (isActive == false && script_Player.CurrentState == ScriptPlayer.State.Possessed) )
      //{
      //  SwitchOffFunction();

      //  //push out player
      //  if (script_Player.CurrentState == ScriptPlayer.State.Possessed)
      //  {
      //    script_Player.NextState = ScriptPlayer.State.Human;
      //  }

      //}



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


      // Setting player spawn & Cam location
      if (isActive == true && other == player_ID)
      {
        if (script_Player.CurrentState == ScriptPlayer.State.Moth)
        {
           activate = true;

          // Set player script nextspawn position == possessionSpawnPos
          script_Player.NextState = ScriptPlayer.State.Possessed;

          Vector3 p_AfterSpawnPos = GetTransform(p_SpawnPos).GetPosition();
          script_Player.spawnPoint = p_AfterSpawnPos;        

         // Set Camera script  position == possessionSpawnPos
          script_Player.camScript.tgtID = p_CamPos;  // Go and expose other tgt in scriptcamera.
         

        }

        


      }
    }


    public override void OnCollisionExit(int other)
    {
      if (GetTransform(other).tag == 3)
      {
        isActive = false;

       // SwitchOffFunction();
      }



    }


    public void SwitchOnFunction()
    {
     
      if (type == 0)
      {
        script_Fan.SwitchOnFunction();
      }

      else
      {
        script_Lamp.ToggleSwitch();
      }

      isOn = true;
    }

    public void SwitchOffFunction()
    {
    

      if (type == 0)
      {
        script_Fan.SwitchOffFunction();
      }

      else
      {
        script_Lamp.ToggleSwitch();
      }

      isOn = false;
    }



    public override void OnTriggerEnter(int other)
    {

    }

    public override void Exit()
    {

    } 
  }
}
