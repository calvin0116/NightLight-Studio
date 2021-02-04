using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptEnemy : UniBehaviour
  {
	
	public string sPlayer = "Player";
	Transform playerTrans ;
	int playerID;
	ScriptPlayer playerScript;
	
    // Used player components
    RigidBody enemyRB;
    Variables enemyVar;
	Transform enemyTrans;
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

	public float distToDetectPlayer = 100.0f;
	public float chaseSpeed = 10.0f;
    public bool dectectsPlayer = false;     //Boolean to react to player later on
                                            //Vector3 inVec;
                                            // accumulated dt
    private float accumulatedDt = 0.0f;
	
	//Attacking States
	public float attackDmg = 10.0f;
	public float toAttackHitDur = 25.0f;
	float toAttackHitCD = 0.0f; 

    public override void Init()
    {
      enemyRB = GetRigidBody(id);
      enemyNavigator = GetNavigator(id);
      enemyVar = GetVariables(id);
	  enemyTrans = GetTransform(id);
	  
	  playerID = GameObjectFind(sPlayer);
	  playerTrans = GetTransform(playerID);
      // Get default values from variables
      maxEnemySpd = enemyVar.GetFloat(0);
    }

    public override void LateInit()
    {
		playerScript = GetScript(playerID);
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
      if (otherTransform.tag == 1)
      {

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
	  float dist = (playerTrans.GetPosition() - enemyTrans.GetPosition()).magnitude;
	//Console.WriteLine(enemyNavigator.GetDir().x );	<- For direction of force	
	  if( dist < distToDetectPlayer)
	  {
		  if(!dectectsPlayer)
		  {
			  Console.WriteLine("Detects player");
			  dectectsPlayer = true;
			  startPaused = true;
		  }
	  }
	  else
	  {
		  if(dectectsPlayer)
		  {
			 dectectsPlayer = false;
			startPatroling = true;
		  }
	  }
		
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
		isPaused = true;
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
		isPaused = false;
        isPatroling = true;
        startPatroling = false;
      }
	  
	  

    }

    // Enemy functions
    public void Move()
    {

      //======================//
      //====Update phase=====//
      if (isIdling)   //Stop movement if idle/pause
	  {
		  Console.WriteLine("Naniiiii");
        return;
	  }
	  
	  //Console.WriteLine(isPaused);
	  if(isPaused)
	  {
		  if(dectectsPlayer && !playerScript.isDead)
		  {
			  Vector3 dir = playerTrans.GetPosition() - enemyTrans.GetPosition();
			  if(dir.magnitude < 25.0f)
			  {
				  toAttackHitCD += RealDT();
				  
				  if(toAttackHitCD > toAttackHitDur)
				  {
					playerScript.health -= attackDmg;
					Console.WriteLine("Player killed");
					toAttackHitCD = 0.0f;
				  }
			  }
			  else
			  {
				  toAttackHitCD = 0.0f;
			  }
			  Force.Apply(id, dir.normalized, chaseSpeed);
			  //Console.WriteLine("Going towards player");
		  }
		  
		  if(playerScript.isDead)
		  {
			  startPatroling = true;
		  }
	  }
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
	  
	  enemyNavigator.MoreThenOneWPActive(); // <- Check for more then one way point active
	  
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
	  
	  if (Input.GetKeyPress(VK.IKEY_T))
      {
        enemyNavigator.GoToNextWp();
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
  }
}