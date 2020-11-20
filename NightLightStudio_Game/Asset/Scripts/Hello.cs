using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class Hello : UniBehaviour
  {    
    public int findID = -1;
    Light l;
    
    public override void Init()
    {

    }

    public override void LateInit()
    {
      findID = GameObjectFind("Entity_0");
      l = GetLight(findID);

      Vector3 diffuse = new Vector3(1.0f, 1.0f, 1.0f);
      l.SetDiffuse(diffuse);
      l.ChangeLightType(Light.Type.POINT);
    }

    public override void Update()
    {
      // Console.WriteLine("Update");
      // Force.Apply(id, Vector3.up, 2000.0f);
    }
    public override void FixedUpdate() { }
    public override void Exit() { Console.WriteLine("Exit from C#"); }
  }
}