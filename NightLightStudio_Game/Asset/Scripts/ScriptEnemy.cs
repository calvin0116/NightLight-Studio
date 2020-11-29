using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptEnemy : UniBehaviour
  {
    public enum State
    {
      Human = 0,
      Moth,
      Possessed
    }
    // Used player components
    RigidBody enemyRB;
    Variables enemyVar;
    Variables enemyTrans;
    //Navigator enemyNavigator; <- Get Navigator
    // Other entity's components


        // Player Stats, default values
        public static float enemyForce = 100000.0f;  // Move force while human
    public static float maxEnemySpd = 500.0f;
    //public static float maxMothSpd = 100.0f;
    public static float moveForce = humnForce; // Current force
    // spawn point
    public static Vector3 spawnPoint;
    // Move dir for camera to player
    public Vector3 fwd;
    public Vector3 rht;
    Vector3 moveDir = new Vector3(0.0f, 0.0f, 0.0f);

        //For init
        public bool startPatroling = false;
        public bool startsIdling = false;
        public bool startsPaused = false;
        //For update
    public bool isPatroling = true;
    public bool isIdling = false;
    public bool isPaused = false;

    public bool dectectsPlayer = false;     //Boolean to react to player later on
        //Vector3 inVec;
    // accumulated dt
    private float accumulatedDt = 0.0f;

    public override void Init()
    {
      enemyRB = GetRigidBody(id);
      enemyVar = GetVariables(id);
    //enemyNavigator = GetNavigator(id);
    // Get default values from variables
    enemyForce = playerVar.GetFloat(0);
    maxEnemySpd = playerVar.GetFloat(1);
      moveForce = humnForce;
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      Move();
      ManualStateControl();
      AutoStateControl();
      CheckChangeState();
      if (Input.GetKeyPress(VK.IMOUSE_LBUTTON))
      {
        Console.WriteLine(curEnergy);
      }
      //Console.WriteLine(GetTransform(IDisposab);

    }
    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other)
    {
      //Transform tag
      // tag 1 = Possessable
      // 
      Transform otherTransform = GetTransform(other);
      if (otherTransform.Tag == 1)
      {
        
      }
    }

    public override void OnTriggerEnter(int other)
    {
    }

    public override void Exit()
    {
    }

    // Enemy functions
    public void Move()
    {
        //======Init phase====//
        if (startsIdling)
        {
            if (isPatroling)
            {
                //enemyNavigator->SetIsFollowing(false);
                isPatroling = false;
            }
            startsIdling = false;
        }

        if (startsPaused)
        {
            if (isPatroling)
            {
                enemyNavigator->SetIsPaused(true);
                isPatroling = false;
            }
            startsPaused = false;
        }


        if (startsPatroling)
        {
            // if (enemyNavigator->GetIsPaused())
            {
                // enemyNavigator->SetIsPaused(false);

            }
            // else
            {
                //enemyNavigator->SetIsFollowing(true);
                isPatroling = false;
            }
            isPatroling = true;
            startsPaused = startsPatroling;
        }
        //======================//
        //====Update phase=====//
        if (isIdling || isPaused)   //Stop movement if idle/pause
            return;
        if (isPatroling)
        {
                //Any unique movement behaviour can be stated here
        }
        //=================//
    }
  }
}