using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class LightBehaviour : UniBehaviour
  {
    public int player_ID;
    int canvasID;
    //  public string canvasName;
    float dtimer;
    bool inLight;
    bool gameOver;
    ScriptPlayer script_Player;

    //Script
    CanvasMenu HUD;
    public override void Init()
    {
      
      canvasID = GameObjectFind("Canvas");
      player_ID = GameObjectFind("Player");
    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);
      HUD = GetScript(canvasID);
    }

    public override void Update()
    {

      if (inLight == true)
      {
        dtimer += DT();

        


        if (dtimer > 1.5f)
        {
          gameOver = true;
        }

      }

      else
      {
        dtimer = 0;
        
        gameOver = false;
      }


      if (gameOver == true)
      {
        HUD.GameOver();
      }
    }
    public override void FixedUpdate()
    {
      
      
    }

   

    public override void OnTriggerEnter(int other) 
    {

      if (GetTransform(other).tag == 200 && script_Player.CurrentState != ScriptPlayer.State.Moth)
      {
       
        inLight = true;
        HUD.Dying();
       // script_Player.humnForce /=  2;

      }
    }


    public override void OnTriggerStay(int other) { Console.WriteLine("Trigger Stay!"); }
   
    public override void OnTriggerExit(int other)
    {

      if (GetTransform(other).tag == 200)
      {

        inLight = false;
        HUD.Safe();
       // script_Player.humnForce *= 2;
      }

      
    }

    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}