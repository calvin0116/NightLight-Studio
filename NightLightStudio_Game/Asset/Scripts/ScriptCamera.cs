using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptCamera : UniBehaviour
  {
    public string sPlayer = "Player";
    int playerID; // Player ID
    // Components
    Transform playerTrans; // Player's transform
    Transform otherTrans;  // Prop's transform
    Variables camVar; // Camera variables comp to get offset

    Vector3 oldTgtPos;

    // Camera values
    Vector3 dir = new Vector3(0.0f, 0.0f, 0.0f); // Direction to move camera
    float Threshold = 50.0f; // Distance to move before camera follows player
    float Time = 0.5f; // Time to move lerp camera
    float offX;
    float offY;
    float offZ;
    Vector3 camOffSet;

    public override void Init()
    {
      // Order matters! Getting component
      playerID = GameObjectFind(sPlayer);
      playerTrans = GetTransform(playerID);
      camVar = GetVariables(id);
      offX = camVar.GetFloat(0);
      offY = camVar.GetFloat(1);
      offZ = camVar.GetFloat(2);
      camOffSet = new Vector3(offX, offY, offZ);

      Camera.SetUseThirdPersonCam(true);
      Camera.SetThirdPersonCamCanRotateAnot(true);
      Camera.SetThirdPersonCamCanZoomAnot(false);
      // Init camera position
      oldTgtPos = playerTrans.getPosition() + camOffSet;
      Camera.SetThirdPersonCamTarget(playerTrans.getPosition() + camOffSet);
    }

    public override void LateInit()
    {

    }

    public override void Update()
    {
      Camera.SetThirdPersonCamTarget(Lerp(playerTrans.getPosition() + camOffSet));
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

    public Vector3 Lerp(Vector3 otherPos)
    {
      dir = otherPos - oldTgtPos;
      float mag = dir.magnitude;
      if (mag > Threshold)
      {
        float speed = mag / Time;
        Vector3 LerpPos = oldTgtPos + (dir.normalized * (speed * RealDT()));
        oldTgtPos = LerpPos;
        return LerpPos;
      }
      return oldTgtPos;
    }
  }
}