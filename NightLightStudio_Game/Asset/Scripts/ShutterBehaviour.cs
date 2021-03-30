
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ShutterBehaviour : UniBehaviour
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
    Graphics lightModel;
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

      if(lightSourceName != "")
      {
        light_ID = GameObjectFind(lightSourceName);
        lightSource = GetLight(light_ID);
        lightModel = GetGraphics(light_ID);
         //Print(light_ID.ToString());
      }
      else
      {
        
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
          lightModel.isActive = true;
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
          lightModel.isActive = false;
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
      Print("ShutterOpen....");
      if (light_ID != -1)
      {
        
        lightModel.isActive = true;
        lightSource.isActive = true;
      }
      

      //if (lightSourceName != "")
      //{
      //  Print("I Got Light......");
      //  lightSource.isActive = true;
      //}

      //Audio.PlayOnce("9");
      isWaypoint = true;
      script_Moth.ActivateWP(wayPointID);

     
     
      isOn = true;
      

    }

    public void SwitchOffFunction()
    {
      Print("ShutterClosed....");
      //Audio.PlayOnce("10");
      isWaypoint = false;
      script_Moth.DeactivateWP(wayPointID);
      isOn = false;

      if (light_ID != -1)
      {
      
        lightModel.isActive = false;
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
