using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptCamera : UniBehaviour
  {
    public string sPlayer = "Player";
    int playerID; // Player ID
    // Trans to follow position
    Transform playerTrans;
    Transform otherTrans;

    public override void Init()
    {
      // Order matters!
      playerID = GameObjectFind(sPlayer);
      playerTrans = GetTransform(playerID);

      Camera.SetUseThirdPersonCam(true);
      Camera.SetThirdPersonCamCanRotateAnot(true);
      Camera.SetThirdPersonCamCanZoomAnot(false);
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

    public override void OnCollisionEnter(int other)
    {
    }

    public override void OnTriggerEnter(int other)
    {
    }

    public override void Exit()
    {
      Camera.SetUseThirdPersonCam(false);
      Camera.SetThirdPersonCamCanRotateAnot(true);
      Camera.SetThirdPersonCamCanZoomAnot(true);
    }
  }
}