
using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class WindBehaviour : UniBehaviour
  {
    // Variables
    bool activate = false;
    public float dirX;
    public float dirY;
    public float dirZ;
    public Vector3 dir;



    //Required Components
    Variables ObjVariables;
    // Rigidbody

    //Variable Component Values
    public static float windStrength;

    // Getting Script
    ScriptPlayer script_Player;
    //ScriptCamera script_Cam;

    public override void Init()
    {
    

      ObjVariables = GetVariables(id);
      windStrength = ObjVariables.GetFloat(0);
      dirX = ObjVariables.GetFloat(1);
      dirY = ObjVariables.GetFloat(2);
      dirZ = ObjVariables.GetFloat(3);
      dir = new Vector3(dirX, dirY, dirZ);
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
    
    public void Activate()
    {
      activate = true;
      Console.WriteLine("ActivateWind Power");
    }

    public void Deactivate()
    {
      activate = false;
    }
   

    public override void OnTriggerStay(int other)
    {
    
      //Tailsmen Object tag = 2 
      if(GetTransform(other).tag == 2 && activate==true)
      {
        

        Force.Apply(other,dir , windStrength);
       

      }

    }

    public override void Exit()
    {

    }
  }
}
