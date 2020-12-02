using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptEnemy : UniBehaviour
  {
    // Used player components
    RigidBody enemyRB;
    //Variables enemyVar;
    Navigator enemyNavigator; //<- Get Navigator
    // Other entity's components


    // Player Stats, default values
    public static float enemyForce = 100000.0f;  // Move force while human
    public static float maxEnemySpd = 500.0f;
    // spawn point
    public static Vector3 spawnPoint;
    // Move dir for camera to player
    Vector3 moveDir = new Vector3(0.0f, 0.0f, 0.0f);

    //For init
    public bool startPatroling = false;
    public bool startIdling = false;
    public bool startPaused = false;
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
      enemyNavigator = GetNavigator(id);
      // Get default values from variables
      //enemyForce = playerVar.GetFloat(0);
      //maxEnemySpd = playerVar.GetFloat(1);
      //moveForce = humnForce;
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      Decision();
      Move();
      Control();
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

    public void Decision()
    {
      //======Init phase====//
      if (startIdling)
      {
        Console.WriteLine("Start Idle");
        if (isPatroling)
        {
          enemyNavigator.isFollowing = false;
          isPatroling = false;
        }
        startIdling = false;
      }

      if (startPaused)
      {
        Console.WriteLine("Start Pause");
        if (isPatroling)
        {
          enemyNavigator.isPaused = true;
          isPatroling = false;
        }
        startPaused = false;
      }


      if (startPatroling)
      {
        Console.WriteLine("Start Patrol");
        if (enemyNavigator.isPaused)
        {
          //enemyNavigator->SetIsPaused(false);
          enemyNavigator.isPaused = false;

        }
        else
        {
          //enemyNavigator->SetIsFollowing(true);
          enemyNavigator.isFollowing = true;
          isPatroling = false;
        }
        isPatroling = true;
        startPatroling = false;
      }
    }

    // Enemy functions
    public void Move()
    {

      //Console.WriteLine("bla");

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
    public void Control()
    {

      //Key for debug
      if (Input.GetKeyPress(VK.IKEY_M))
      {
        enemyNavigator.isPaused = !enemyNavigator.isPaused;
        /*
        if (isPatroling)
          startPaused = true;
        else
          startPatroling = true;
        */
        Console.WriteLine("Is paused: %i" , enemyNavigator.isPaused);
      }
      //Speed up
      if (Input.GetKeyPress(VK.IKEY_K))
      {
        enemyNavigator.speed += 1.0f;
      }
      //Speed up
      if (Input.GetKeyPress(VK.IKEY_L))
      {
        enemyNavigator.speed += 1.0f;
      }
    }
  }
}