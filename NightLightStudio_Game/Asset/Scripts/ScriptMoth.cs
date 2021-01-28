using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptMoth : UniBehaviour
  {
    // Used player components
    RigidBody enemyRB;
    Variables enemyVar;
    Navigator enemyNavigator; //<- Get Navigator
   // Transform myPosition;
    // Other entity's components
    public int player_ID;
    ScriptPlayer script_Player;

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

    bool invalidPath;

    //for Behaviour
    float pathSwitchTimer;
    float decisionTimer;
    bool thinking;
    bool possessed;
    bool activate;


    public override void Init()
    {

      enemyRB = GetRigidBody(id);
      enemyNavigator = GetNavigator(id);
      enemyVar = GetVariables(id);
      //myPosition = GetTransform(id);

      player_ID = GameObjectFind("Player");
      // Get default values from variables
      maxEnemySpd = enemyVar.GetFloat(0);


      //enemyNavigator.NavState = 0;
      thinking = true;

    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);
    }

    public override void Update()
    {
      Decision();
      Move();
      Control();

      if (activate == true)
      {
        //Set Player State to possessionstate
        SwitchOnFunction();

        if (script_Player.CurrentState == ScriptPlayer.State.Human)
        {
          SwitchOffFunction();
          activate = false;
        }


      }



      if (enemyNavigator.NavState == 0)
      {
        pathSwitchTimer += RealDT();

        if(pathSwitchTimer > 0.5f)
        {
          enemyNavigator.NavState = 1;
          pathSwitchTimer = 0;

          if(thinking== false)
          {
            thinking = true;
          }
          
        }

      }

      //// -------Dismount logic
      //if (possessed == true)
      //{
      //  if(script_Player.CurrentState == ScriptPlayer.State.Moth)
      //}



    }


    public void ActivateWP(int ID)
    {
      enemyNavigator.SetWpActive(ID, true);
      Print("Start");
    }

    public void DeactivateWP(int ID)
    {
      enemyNavigator.SetWpActive(ID, false);
      Print("off");
    }


    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other)
    {
      //Transform tag
      // tag 1 = Possessable
      // 
      //Transform otherTransform = GetTransform(other);
      if (GetTransform(other).tag == 200)
      {

        if (script_Player.CurrentState == ScriptPlayer.State.Moth/* && other == player_ID && activate == false*/)
        {

          // Set player script nextspawn position == possessionSpawnPos
          //script_Player.NextState = ScriptPlayer.State.Possessed;


          // Set Camera script  position == possessionSpawnPos
          // script_Player.camScript.tgtID = id; // Go and expose other tgt in scriptcamera.


          activate = true;

         // possessed = true;

        }

      }






    }

    public override void OnTriggerEnter(int other)
    {
    }

    public override void Exit()
    {
    }

    //If need a ai state like behaviour, just rough idea only
    public void Decision()
    {
      //======Init phase====//
      //Print("Thinking");

      //---- Moth Logic----//

      if (thinking == true && invalidPath ==false )
      {
        decisionTimer += RealDT();

        if (decisionTimer > 4.0f)
        {
          if (enemyNavigator.MoreThenOneWPActive())
          {
            enemyNavigator.NavState = 0;
            
            decisionTimer = 0;
            Print("target");
            thinking = false;
          }

          else
          {
            Print("No target");
            decisionTimer = 0;
            Decision();
          }

          
          
        }

      }

      //else if (thinking == true && invalidPath == true)
      //{
      //  thinking = false;
      //}


      // Wait for 1s 
      // Check if there is another waypoint active

      // if yes ()
      // change to mode & back



      if (startIdling)
      {
        
      }

      if (startPaused)
      {
        
      }


      if (startPatroling)
      {
        
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
      //Pause and unpause movement
      if (Input.GetKeyPress(VK.IKEY_M))
      {
        enemyNavigator.isPaused = !enemyNavigator.isPaused;
        /*
        if (isPatroling)
          startPaused = true;
        else
          startPatroling = true;
        */
      }

      // 0 -> Patrol
      // 1 -> Circling at cur_wp
      if (Input.GetKeyPress(VK.IKEY_Y))
      {
        enemyNavigator.NavState = 0;
      }
      if (Input.GetKeyPress(VK.IKEY_U))
      {
        enemyNavigator.NavState = 1;
      }

      //Set active to specify wp
      if (Input.GetKeyPress(VK.IKEY_J))
      {
        enemyNavigator.SetWpActive(1, false);
      }
      //Speed up
      if (Input.GetKeyPress(VK.IKEY_K))
      {
        if (enemyNavigator.speed < maxEnemySpd)
        {
          Console.WriteLine("Increasing....");
          enemyNavigator.speed += 0.5f;
        }
      }
      //Slow down
      if (Input.GetKeyPress(VK.IKEY_L))
      {
        if (enemyNavigator.speed > 1.0f)
          enemyNavigator.speed -= 0.5f;
      }
    }

    public void SwitchOnFunction()
    {

      GetTransform(player_ID).SetPosition(GetTransform(id).GetPosition());


    }


    public void SwitchOffFunction()
    {

    }

  }
}