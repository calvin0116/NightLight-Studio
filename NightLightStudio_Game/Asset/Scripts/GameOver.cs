using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class GameOver : UniBehaviour
  {
   
    int canvasID;
 
    Variables triggerVar;
    public string canvasName;

    //Script
    CanvasMenu HUD;
    public override void Init()
    {
      triggerVar = GetVariables(id);
      canvasName = triggerVar.GetString(0);
      canvasID = GameObjectFind(canvasName);
    }

    public override void LateInit()
    {
      HUD = GetScript(canvasID);
    }

    public override void Update()
    {

    }
    public override void FixedUpdate()
    {
      
    }

   

    public override void OnTriggerEnter(int other) 
    {

      if (GetTransform(other).tag == 200)
      {
        Print("win");
        HUD.GameOver();

      }
    }


    public override void OnTriggerStay(int other) { Console.WriteLine("Trigger Stay!"); }
    public override void OnTriggerExit(int other) { Console.WriteLine("Trigger Exit!"); }

    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}