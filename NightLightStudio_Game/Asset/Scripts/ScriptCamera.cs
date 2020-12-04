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
    float Threshold = 0.1f; // Distance to move before camera follows player
    //float Time = 0.5f; // Time to move lerp camera
    Vector3 Time = new Vector3(0.03f, 0.03f, 0.03f);
    float offX;
    float offY;
    float offZ;
    float offZ_min;
    float offZ_current;

    bool isCollide = false;
    int colThresholdMax = 10;
    int colThresholdCurrent = 0;

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
      offZ_min = camVar.GetFloat(3);
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

        //set transform pos
      //Vector3 camPos = Camera.GetPosition();
      //camTrans.SetPosition(camPos);
    }

    public override void LateInit()
    {
      // Get script
      script_player = GetScript(playerID);
    }

    public override void Update()
    {
      Vector3 tgtTPos = GetTgtFromID(tgtID);

      Vector3 tgtPos = Lerp(tgtTPos + getCamOffsetVec());
      oldTgtPos = tgtPos;
      //if (script_player.CurrentState == ScriptPlayer.State.Human)
      //  tgtPos = playerTrans.GetPosition() + getCamOffsetVec();
      //if (script_player.CurrentState == ScriptPlayer.State.Moth)
      //  tgtPos = GetTgtFromID(playerID);
      Camera.SetThirdPersonCamTarget(tgtPos);
      //Console.WriteLine(offZ_current);


      
      Vector3 camPos = Camera.GetPosition();

      Vector3 midv = tgtTPos - camPos;
      midv.x = midv.x * 0.5f;
      midv.y = midv.y * 0.5f;
      midv.z = midv.z * 0.5f;
      Vector3 mid = camPos + midv; // midpoint
            
      //Console.WriteLine(Camera.GetViewVector().x);
      //Console.WriteLine(Camera.GetViewVector().y);
      //Console.WriteLine(Camera.GetViewVector().z);


      camTrans.SetPosition(mid);


        // https://community.khronos.org/t/converting-a-3d-vector-into-three-euler-angles/49889/2

        //You can post it in 3D coordinates. You must think of a sphere, rather than just a circle.
        //Let r = radius, t = angle on x-y plane, & p = angle off of z-axis. Then you get:

        //x = r * sin§ * cos(t)
        //y = r * sin§ * sin(t)
        //z = r * cos§

        //If you already have x,y,z and want to switch it back, this is the conversion:

        //r = sqrt(xx + yy + z*z)
        //t = arctan(y/x)
        //p = arccos(z/r)
        //*For computing p, it’s easier to compute r first, then use it as the denominator (assuming ![x = y = z = 0]).

        Vector3 rotation = Camera.GetViewVector();

        float r = (float)Math.Sqrt(rotation.x * rotation.x + rotation.z * rotation.z);
        float t = (float) (Math.Atan(rotation.y / r) / Math.PI * 180);
		float p = (float) (Math.Acos(rotation.z / r) / Math.PI * 180);
        t = t + 90;
		if (rotation.x < 0)
		{
                //rotation = new Vector3(0, -p, 0);
                rotation = new Vector3(-t, -p, 0);
        }
		else
		{
                //rotation = new Vector3(0, p, 0);
                rotation = new Vector3(-t, p, 0);
        }

      //Vector3 rot = new Vector3(90.0f, 0.0f, 0.0f);
      camTrans.SetRotation(rotation);

      Vector3 sca = new Vector3(8.0f, offZ_current/* * 0.5f*/, 8.0f);
      camTrans.SetScale(sca);

      if (isCollide)
      {
        offZ_current = offZ_lerp.Lerp(offZ_min, RealDT());
        colThresholdCurrent = 0;


        //Console.WriteLine("isCollide");
      }
      else
      {
        

        if(colThresholdCurrent > colThresholdMax)
        { 
          offZ_current = offZ_lerp.Lerp(offZ, RealDT());

          //Console.WriteLine(">threshold");
        }
        else
        {
          ++colThresholdCurrent;
          //Console.WriteLine("++threshold");
        }
      }
      //Console.WriteLine("offZ_current" + offZ_current);
      Camera.SetThirdPersonCamDistance(offZ_current);
      //Camera.SetThirdPersonCamTarget(Lerp(playerTrans.GetPosition() + camOffSet));
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
      //isCollide = true;
    }

    public override void OnCollisionExit(int other)
    {
      //Console.WriteLine("OnCollisionExit");
      //isCollide = false;
    }

    public override void OnTriggerEnter(int other)
    {
        if(other != playerID)
        {
            Console.WriteLine("OnTriggerEnter");
            isCollide = true;
        }
    }

    public override void OnTriggerStay(int other)
    {
        if(other != playerID)
        {
            Console.WriteLine("OnTriggerStay");
            isCollide = true;
        }
    }

    public override void OnTriggerExit(int other)
    {
        if (other != playerID)
        {
            Console.WriteLine("OnTriggerExit");
            isCollide = false;
        }
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
      return trans.GetPosition();
    }
  }
}