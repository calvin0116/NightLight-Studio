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
    Transform playerTrans; // Player's transform
    Transform otherTrans;  // Prop's transform
    Vector3 oldTgtPos;

    // Direction to move camera
    Vector3 dir = new Vector3(0.0f, 0.0f, 0.0f);
    float Threshold = 100.0f; // Distance to move before camera follows player
    float FollowTime = 1.0f; // How long for the camera to catch up to player

    public override void Init()
    {
      // Order matters!
      playerID = GameObjectFind(sPlayer);
      playerTrans = GetTransform(playerID);

      Camera.SetUseThirdPersonCam(true);
      Camera.SetThirdPersonCamCanRotateAnot(true);
      Camera.SetThirdPersonCamCanZoomAnot(false);
      // Init camera position
      oldTgtPos = playerTrans.getPosition();
      Camera.SetThirdPersonCamTarget(playerTrans.getPosition());
    }

    public override void LateInit()
    {

    }

    public override void Update()
    {
      SetDir(playerTrans.getPosition());
      if(toLerp(Threshold))
        Camera.SetThirdPersonCamTarget(Lerp());
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

    public void SetDir(Vector3 otherPos)
    {
      dir = otherPos - oldTgtPos;
    }

    public bool toLerp(float threshold)
    {
      if (dir.magnitude <= threshold)
        return false;
      return true;
    }

    public Vector3 Lerp()
    {
      Vector3 LerpPos = oldTgtPos + (dir * RealDT());
      oldTgtPos = LerpPos;
      return LerpPos;
    }
  }
}