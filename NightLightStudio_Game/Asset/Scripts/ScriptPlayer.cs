using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptPlayer : UniBehaviour
  {
    public float moveForce = 200000.0f;
    //Vector3 inVec;

    public override void Init()
    {
      //inVec = new Vector3(0.0f, 0.0f, -1.0f);
    }

    public override void LateInit()
    {
      Console.WriteLine("Inited ScriptPlayer");
    }

    public override void Update()
    {
      Move();
    }
    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other)
    {
    }

    public override void OnTriggerEnter(int other)
    {
    }

    public override void Exit()
    {
    }

    // Player functions
    public void Move()
    {
      // Forward
      if (Input.GetKeyHold(VK.IKEY_W))
        Force.Apply(id, ForwardVector(), moveForce);
      // Left
      if (Input.GetKeyHold(VK.IKEY_A))
        Force.Apply(id, -Camera.GetRightVector(), moveForce);
      // Backward
      if (Input.GetKeyHold(VK.IKEY_S))
        Force.Apply(id, -ForwardVector(), moveForce);
      // Right
      if (Input.GetKeyHold(VK.IKEY_D))
        Force.Apply(id, Camera.GetRightVector(), moveForce);
    }

    // Player functions
    public Vector3 ForwardVector()
    {
      return Vector3.Cross(Camera.GetUpVector(), Camera.GetRightVector());
    }
  }
}