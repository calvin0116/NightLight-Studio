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

            if (dir > _threshold)
            {
                float speed = dir / _time;

                if(dir > 0)
                {
                    _old = _old + speed * dt;
                }
                else
                {
                    _old = _old - speed * dt;
                }

            }
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
    }


    public class ScriptCamera : UniBehaviour
  {
    public string sPlayer = "Player";
    int playerID; // Player ID
    // Components
    Transform playerTrans; // Player's transform
    Transform otherTrans;  // Prop's transform
    Transform camTrans; // Camera's transform
    Variables camVar; // Camera variables comp to get offset

    //Variables
    Vector3 oldTgtPos;
    // Camera values
    Vector3 dir = new Vector3(0.0f, 0.0f, 0.0f); // Direction to move camera
    float Threshold = 10.0f; // Distance to move before camera follows player
    float Time = 0.5f; // Time to move lerp camera
    float offX;
    float offY;
    float offZ;
        
    float offZ_current;

    Lerper offZ_lerp = new Lerper(0.0f, 10.0f, 0.5f);

    Vector3 camOffSet;

    // Scripts
    ScriptPlayer script_player;

    public override void Init()
    {
      // Order matters! Getting component
      playerID = GameObjectFind(sPlayer);
      playerTrans = GetTransform(playerID);
      camTrans = GetTransform(id);
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
      Camera.SetThirdPersonCamTarget(playerTrans.getPosition() + getCamOffsetVec());

        offZ_lerp.old = offZ;
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

      Vector3 tgtPos = Lerp(playerTrans.getPosition()) + getCamOffsetVec();
      //if (script_player.CurrentState == ScriptPlayer.State.Human)
      //  tgtPos = playerTrans.getPosition() + getCamOffsetVec();
      if (script_player.CurrentState == ScriptPlayer.State.Moth)
        tgtPos = playerTrans.getPosition();
      Camera.SetThirdPersonCamTarget(tgtPos);

            
      Console.WriteLine(offZ_current);
      offZ_current = offZ_lerp.Lerp(0.0f, RealDT());

      Camera.SetThirdPersonCamDistance(offZ_current);


      //Camera.SetThirdPersonCamTarget(Lerp(playerTrans.getPosition() + camOffSet));
    }

    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other)
    {
      Console.WriteLine("OnCollisionEnter");
            
    }

    public override void OnCollisionStay(int other)
    {
      Console.WriteLine("OnCollisionStay");
    }
        
    public override void OnCollisionExit(int other)
    {
      Console.WriteLine("OnCollisionExit");
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