
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class CCTVBehaviour : UniBehaviour
  {
    // Variables
    public bool activate = false;
    public int player_ID;
    public int cam_ID;
    public int p_CamPos;
    public int p_SpawnPos;
    bool isActive;

    //Required Components
    Variables ObjVariables;
    // Rigidbody

    //Variable Component Values
    public static string possessionCamPos;
    public static string possessionSpawnPos;

    // Getting Script
    ScriptPlayer script_Player;
    //ScriptCamera script_Cam;

    public override void Init()
    {
      player_ID = GameObjectFind("Player");
      cam_ID = GameObjectFind("PlayerCamera");

      ObjVariables = GetVariables(id);
      isActive = true;

      possessionCamPos = ObjVariables.GetString(0);
      possessionSpawnPos = ObjVariables.GetString(1);

      p_CamPos = GameObjectFind(possessionCamPos);
      p_SpawnPos = GameObjectFind(possessionSpawnPos);
    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);
      //script_Cam = GetScript(cam_ID);
    }

    public override void Update()
    {
      if (activate == true)
      {
        // Set Player State to possessionstate
        ObjFunction();
      }
    }
    public override void FixedUpdate()
    {

    }

    public override void OnCollisionEnter(int other)
    {
      if (isActive == true)
      {
        if (script_Player.CurrentState == ScriptPlayer.State.Moth/* && other == player_ID && activate == false*/)
        {

          // Set player script nextspawn position == possessionSpawnPos
          script_Player.NextState = ScriptPlayer.State.Possessed;

          Transform p_AfterSpawn = GetTransform(p_SpawnPos);
          Vector3 p_AfterSpawnPos = p_AfterSpawn.GetPosition();
          script_Player.spawnPoint = p_AfterSpawnPos;
          // Set Camera script  position == possessionSpawnPos

          //Transform p_Target = GetTransform(p_CamPos);
          //Vector3 p_TargetPos = p_Target.getPosition();
          script_Player.camScript.tgtID = p_CamPos; // Go and expose other tgt in scriptcamera.

          // Activate energy consumption delay 
          //activate = true;
          //script_Player.CurrentState = ScriptPlayer.State.Possessed;
        }
      }
      if (isActive == false)
      {
        activate = false;
      }
    }
    public void Deactivate()
    {
      isActive = !isActive;
    }
    public void ObjFunction()
    {
      //script_Player.CurrentState == ScriptPlayer.State.Possessed;      
    }

    public override void OnTriggerEnter(int other)
    {

    }

    public override void Exit()
    {

    }
  }
}
