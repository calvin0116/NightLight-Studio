using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{

  public class Lerper
  {
    float _old;
    float _threshold;
    float _time;

    public Lerper(float __old, float __threshold, float __time)
    {
      _old = __old;
      _threshold = __threshold;
      _time = __time;
    }

    public float Lerp(float other, float dt)
    {
      float dir = other - _old;

      //Console.WriteLine("dir" + dir);

      if (Math.Abs(dir) > _threshold)
      {
        float speed = dir / _time;

        //Console.WriteLine("spd" + speed);

        _old = _old + speed * dt;

        //if (dir > 0) // im a goddamn idiot
        //{
        //    _old = _old + speed * dt;
        //}
        //else
        //{
        //    _old = _old - speed * dt;
        //}

      }

      //Console.WriteLine("old" + _old);
      return _old;
    }

    public float old
    {
      get
      {
        return _old;
      }
      set
      {
        _old = value;
      }
    }

    public float threshold
    {
      get
      {
        return _threshold;
      }
      set
      {
        _threshold = value;
      }
    }
  }


  public class ScriptCamera : UniBehaviour
  {
    // Use this to change camera's tgt
    public int tgtID;

    public string sPlayer = "Player";
    int playerID; // Player ID
    // Components
    //Transform playerTrans; // Player's transform
    //Transform otherTrans;  // Prop's transform
    Transform camTrans; // Camera's transform
    Variables camVar; // Camera variables comp to get offset

    //Variables
    public Vector3 oldTgtPos { get; private set; }
    //public Vector3 camTgt;
    // Camera values
    Vector3 dir = new Vector3(0.0f, 0.0f, 0.0f); // Direction to move camera
    float Threshold = 10.0f; // Distance to move before camera follows player
    //float Time = 0.5f; // Time to move lerp camera
    Vector3 Time = new Vector3(0.03f, 0.03f, 0.5f);
    float offX;
    float offY;
    float offZ;

    float offZ_current;

    bool isCollide = false;

    Lerper offZ_lerp = new Lerper(0.0f, 10.0f, 0.5f); // old, thresold, spd

    Vector3 camOffSet;

    // Scripts
    ScriptPlayer script_player;

    public override void Init()
    {
      // Order matters! Getting component
      playerID = GameObjectFind(sPlayer);
      //playerTrans = GetTransform(playerID);
      camTrans = GetTransform(id);
      camVar = GetVariables(id);
      offX = camVar.GetFloat(0);
      offY = camVar.GetFloat(1);
      offZ = camVar.GetFloat(2);
      camOffSet = new Vector3(offX, offY, offZ);

      Camera.SetUseThirdPersonCam(true);
      Camera.SetThirdPersonCamCanRotateAnot(true);
      Camera.SetThirdPersonCamCanZoomAnot(false);
      // Init camera position, start off at player pos
      tgtID = playerID;
      oldTgtPos = GetTgtFromID(tgtID) + camOffSet;
      Camera.SetThirdPersonCamTarget(GetTgtFromID(tgtID) + getCamOffsetVec());
      offZ_lerp.old = offZ;

      // Set tgt id
      tgtID = playerID;
    }

    public override void LateInit()
    {
      // Get script
      script_player = GetScript(playerID);
    }

    public override void Update()
    {
      Vector3 camPos = Camera.GetPosition();
      camTrans.setPosition(camPos);
      Vector3 tgtPos = Lerp(GetTgtFromID(tgtID)) + getCamOffsetVec();
      //if (script_player.CurrentState == ScriptPlayer.State.Human)
      //  tgtPos = playerTrans.getPosition() + getCamOffsetVec();
      //if (script_player.CurrentState == ScriptPlayer.State.Moth)
      //  tgtPos = GetTgtFromID(playerID);
      Camera.SetThirdPersonCamTarget(tgtPos);
      //Console.WriteLine(offZ_current);

      if (isCollide)
      {
        offZ_current = offZ_lerp.Lerp(0.0f, RealDT());
      }
      else
      {
        offZ_current = offZ_lerp.Lerp(offZ, RealDT());
      }
      //Console.WriteLine("offZ_current" + offZ_current);
      Camera.SetThirdPersonCamDistance(offZ_current);
      //Camera.SetThirdPersonCamTarget(Lerp(playerTrans.getPosition() + camOffSet));
    }

    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other)
    {
      //Console.WriteLine("OnCollisionEnter");
    }

    public override void OnCollisionStay(int other)
    {
      //Console.WriteLine("OnCollisionStay");
      isCollide = true;
    }

    public override void OnCollisionExit(int other)
    {
      //Console.WriteLine("OnCollisionExit");
      isCollide = false;
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

    public Vector3 getCamOffsetVec()
    {
      Vector3 vecx, vecy;
      vecx = Camera.GetRightVector() * camOffSet.x;
      vecy = Camera.GetUpVector() * camOffSet.y;
      //vecx = Camera.GetRightVector() * camOffSet.z; // z is distance
      Vector3 vec = vecx + vecy;
      return vec;
    }

    public Vector3 Lerp(Vector3 other)
    {

      dir = other - oldTgtPos;

      if (Math.Abs(dir.x) > Threshold)
      {
        float speed = dir.x / Time.x;
        oldTgtPos.x = oldTgtPos.x + (speed * RealDT());
      }
      if (Math.Abs(dir.y) > Threshold)
      {
        float speed = dir.y / Time.y;
        oldTgtPos.y = oldTgtPos.y + (speed * RealDT());
      }
      if (Math.Abs(dir.z) > Threshold)
      {
        float speed = dir.z / Time.z;
        oldTgtPos.z = oldTgtPos.z + (speed * RealDT());
      }
      return oldTgtPos;

      //dir = otherPos - oldTgtPos;
      //float mag = dir.magnitude;
      //if (mag > Threshold)
      //{
      //  float speed = mag / Time;
      //  Vector3 LerpPos = oldTgtPos + (dir.normalized * (speed * RealDT()));
      //  oldTgtPos = LerpPos;
      //  return LerpPos;
      //}
      //return oldTgtPos;
    }

    public Vector3 GetTgtFromID(int id)
    {
      Transform trans = GetTransform(id);
      return trans.getPosition();
    }
  }
}