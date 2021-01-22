
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class LampBehaviour : UniBehaviour
  {
    // Variables
    public bool activate = false;
    public int player_ID;
    public int cam_ID;
    public int p_CamPos;
    public int p_SpawnPos;
    int ai_ID;
    bool isActive;
    public bool isWaypoint;
    bool isOn;


    //Required Components
    Variables ObjVariables;
    // Rigidbody

    //Variable Component Values
    public static string possessionCamPos;
    public static string possessionSpawnPos;
    public static string mothAIName;
    public static int wayPointID;
    public static int onOff;

    // Getting Script
    ScriptPlayer script_Player;
    ScriptMoth script_Moth;
    //ScriptCamera script_Cam;

    public override void Init()
    {
      player_ID = GameObjectFind("Player");
      cam_ID = GameObjectFind("PlayerCamera");

      ObjVariables = GetVariables(id);
      isActive = true;

      possessionCamPos = ObjVariables.GetString(0);
      possessionSpawnPos = ObjVariables.GetString(1);
      mothAIName = ObjVariables.GetString(2);
      wayPointID = ObjVariables.GetInt(0);
      onOff = ObjVariables.GetInt(1);

      if (onOff < 1)
      {
        isOn = false;
      }
      else
      {
        isOn = true;
      }

      p_CamPos = GameObjectFind(possessionCamPos);
      p_SpawnPos = GameObjectFind(possessionSpawnPos);
      ai_ID= GameObjectFind(mothAIName);




    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);
      script_Moth= GetScript(ai_ID);
      //script_Cam = GetScript(cam_ID);
	  
	  if (isWaypoint == true)
      {
        SwitchOffFunction();
      }

      else
      {
        SwitchOnFunction();
      }
	  
    }

    public override void Update()
    {

      //if (activate == true)
      //{
      //   Set Player State to possessionstate
      //  SwitchOnFunction();
      //}

      //if((isActive == false && script_Player.CurrentState == ScriptPlayer.State.Possessed)|| activate ==false )
      //{
      //   SwitchOffFunction();

      //   push out player
      //  if(script_Player.CurrentState == ScriptPlayer.State.Possessed)
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



      if (isActive == true && GetTransform(other).tag == 200)
      {
		  
        if (script_Player.CurrentState == ScriptPlayer.State.Moth/* && other == player_ID && activate == false*/)
        {

          // Set player script nextspawn position == possessionSpawnPos
          script_Player.NextState = ScriptPlayer.State.Possessed;

          Transform p_AfterSpawn = GetTransform(p_SpawnPos);
          Vector3 p_AfterSpawnPos = p_AfterSpawn.GetPosition();
          script_Player.spawnPoint = p_AfterSpawnPos;
          // Set Camera script  position == possessionSpawnPos

          
          script_Player.camScript.tgtID = p_CamPos; // Go and expose other tgt in scriptcamera.
      
        }

        if (isWaypoint == true)
        {
          SwitchOffFunction();
        }

        else
        {
          SwitchOnFunction();
        }

      }

      
    }



    public override void OnCollisionExit(int other)
    {
      if (GetTransform(other).tag == 3)
      {
        isActive = true;
      }
    }


    public void SwitchOnFunction()
    {
      isWaypoint = true;
      script_Moth.ActivateWP(wayPointID);

    }

    public void SwitchOffFunction()
    {
      isWaypoint = false;
      script_Moth.DeactivateWP(wayPointID);
    }



    public override void OnTriggerEnter(int other)
    {

    }

    public override void Exit()
    {

    }
	
  }
  
}
