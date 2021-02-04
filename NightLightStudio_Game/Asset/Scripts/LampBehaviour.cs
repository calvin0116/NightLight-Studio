
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class LampBehaviour : UniBehaviour
  {
    // Variables
    public bool activate = false;
    public int wayPointID;
    public int player_ID;
    public int cam_ID;
    public int light_ID;
    public int p_CamPos;
    public int p_SpawnPos;
    int ai_ID;
    bool isActive;
    public bool isWaypoint;
    bool isOn;
    

    //Required Components
    Variables ObjVariables;
    Light lightSource;
    // Rigidbody

    //Variable Component Values
    //public static string possessionCamPos;
    // public static string possessionSpawnPos;
    public static string mothAIName;
    public static string lightSourceName;
   // public int wayPointID;
   // public static int isSwitchOn;
   // public int onOff;

    // Getting Script
    ScriptPlayer script_Player;
    ScriptMoth script_Moth;
    //ScriptCamera script_Cam;

    public override void Init()
    {
      player_ID = GameObjectFind("Player");
      //cam_ID = GameObjectFind("PlayerCamera");

      ObjVariables = GetVariables(id);
      isActive = true;

     
     
      //wayPointID = ObjVariables.GetInt(0);
      //isSwitchOn = ObjVariables.GetInt(1);
      mothAIName = ObjVariables.GetString(0);
      lightSourceName = ObjVariables.GetString(1);


      ai_ID = GameObjectFind(mothAIName);

      if(lightSourceName != null)
      {
        light_ID = GameObjectFind(lightSourceName);
        lightSource = GetLight(light_ID);
      }
   



    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);
      script_Moth = GetScript(ai_ID);


      if (isWaypoint == true)
      {

        
        script_Moth.ActivateWP(wayPointID);

        if (lightSource != null)
        {
          lightSource.isActive = true;
        }

        isOn = true;
      }
      else
      {

        
        script_Moth.DeactivateWP(wayPointID);
        isOn = false;

        if (lightSource != null)
        {
          lightSource.isActive = false;
        }
      }

    }

    public override void Update()
    {

     



    }
    public override void FixedUpdate()
    {

    }

    public override void OnCollisionEnter(int other)
    {


     


    }



    public override void OnCollisionExit(int other)
    {
      //if (GetTransform(other).tag == 3)
      //{
      //  isActive = true;
      //}
    }


    


    public void SwitchOnFunction()
    {
      Audio.PlayOnce("9");
      isWaypoint = true;
      script_Moth.ActivateWP(wayPointID);

      if (lightSource != null)
      {
        lightSource.isActive = true;
      }
     
      isOn = true;
      

    }

    public void SwitchOffFunction()
    {
      Audio.PlayOnce("10");
      isWaypoint = false;
      script_Moth.DeactivateWP(wayPointID);
      isOn = false;

      if (lightSource != null)
      {
        lightSource.isActive = false;
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
