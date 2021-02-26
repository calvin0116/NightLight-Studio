using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class SceneChanger : UniBehaviour
  {
   
    
    public string sceneName="";

    //Script
   
    public override void Init()
    {
   
    }

    public override void LateInit()
    {
      
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
        SetNextScene(sceneName);

      }
    }


    public override void OnTriggerStay(int other) { Console.WriteLine("Trigger Stay!"); }
    public override void OnTriggerExit(int other) { Console.WriteLine("Trigger Exit!"); }

    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}