using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptPlayer : UniBehaviour
  {
    public enum State
    {
      Human = 0,
      Moth,
      Possessed
    }

    // Used player components
    RigidBody playerRB;
    Variables playerVar;
    Graphics playerCharModel;
    Collider playerCol;
    Transform playerTrans;
    Animation anim;
    Emitter playerEmt;
    AudioManager script_AM;
    Generator script_G;
    ScriptMoth script_M;

    /// <summary>
    /// ////////////////Place holder Variables
    public static bool flying = false;
    public Vector3 FormalPos;
    /// ////////////////
    /// </summary>

    // Other entity's components
    public ScriptCamera camScript; // To set camera tgt back to player after becoming human.
    public Transform camTrans; // To get cam position for ray cast


    // Player Stats, default values
    public static float humnForce = 100000.0f;  // Move force while human
    public static float mothSpeed = 10000.0f;  // Move force while moth
    public static float maxEnergy = 15.0f;
    public static float transformTime = 2f; // time to change state

    public static float maxHumnSpd = 500.0f;
    //public static float maxMothSpd = 100.0f;
    public static float moveForce = humnForce; // Current force
    public static float curEnergy = maxEnergy;
    // Minimum Energy required to change to moth
    public static float EnergyTreshold = 0.0f;
    public static float EnergyDrain = 1.0f;
    public static float EnergyGain = 1.0f;
    bool canMove;
    // Animation
    //bool idle = true;
    bool played = false;
    bool switching = false;

    // spawn point
    public Vector3 spawnPoint;
    // Move dir for camera to player
    public Vector3 fwd;
    public Vector3 rht;
    Vector3 moveDir = new Vector3(0.0f, 0.0f, 0.0f);
    bool movedW = false;
    bool movedA = false;
    bool movedS = false;
    bool movedD = false;
    // Player State
    public State CurrentState { get; private set; }
    public State NextState = State.Human;
    State PreviousState;
    //Vector3 inVec;
    //for Morth AI
    Vector3 flyDir = new Vector3(0.0f, 0.0f, 0.0f);
    Vector3 morthPos = new Vector3(0.0f, 0.0f, 0.0f);
    // accumulated dt
    private float accumulatedDt = 0.0f;
    float moth_currentSpeed;

    //Variables
    string humanModePath;
    //string mothModePath;
    int raycastID = 0;
    int raycastID2 = 0;
    bool rayCastHit = false;
    bool rayCastHit2 = false;
    bool hitMorth = false;
    bool hitGenerator = false;
    //bool exitMoth = false;
    // Audio
    int aMID;
    bool isPlaying;
    int chnl = -1;
	
	//Player states, for enemy interaction
	public bool isDead = false;
	public float health = 20.0f;

    // User Experiences 
    float fb_Timer;
    public bool activating;

    public override void Init()
    {

      aMID = GameObjectFind("AudioManager");



      CurrentState = State.Human;
      NextState = State.Human;

      playerRB = GetRigidBody(id);
      playerCharModel = GetGraphics(id);
      playerCol = GetCollider(id);
      playerVar = GetVariables(id);
      playerTrans = GetTransform(id);
      anim = GetAnimation(id);
      playerEmt = GetEmitter(id);

      // Get scripts
      camScript = GetScript(GameObjectFind("PlayerCamera"));
      camTrans = GetTransform(GameObjectFind("PlayerCamera"));

      // Get default values from variables (5Floats) (2 Strings)
      humnForce = playerVar.GetFloat(0);
      mothSpeed = playerVar.GetFloat(1);
      maxEnergy = playerVar.GetFloat(2);
      transformTime = playerVar.GetFloat(3);
      maxHumnSpd = playerVar.GetFloat(4);

      humanModePath = playerVar.GetString(0);
      //mothModePath = playerVar.GetString(1);

      curEnergy = maxEnergy;
      moveForce = humnForce;
      canMove = true;
      switching = false;



      playerCharModel.AddModel(humanModePath);
    }

    public override void LateInit()
    {
      script_AM = GetScript(aMID);
    }

    public override void Update()
    {

      Raycasting();

        if (Input.GetKeyPress(VK.IMOUSE_RBUTTON) && hitGenerator)
        {
          hitGenerator = false;
          activating = true;
          anim.Play("Pray", false, 10f, 30f);

        }
        if (Input.GetKeyUp(VK.IMOUSE_RBUTTON))
        {
          activating = false;
        }

      if (activating)
      {
        fb_Timer += DT();
        if (fb_Timer > 1.3f)
        {
          script_G.activate = true;
          activating = false;
        }

      }
      else 
      {
        fb_Timer = 0;
      }


      if (canMove == true)
      {
        Move();
      }
      else
      {
        if (CurrentState == State.Human)
        {
          playerRB.SetForce(new Vector3(0.0f, playerRB.GetForce().y, 0.0f));
          playerRB.SetAccel(new Vector3(0.0f, playerRB.GetAccel().y, 0.0f));
          playerRB.SetVel(new Vector3(0.0f, playerRB.GetVel().y, 0.0f));
        }
        else
        {
          playerRB.SetForce(new Vector3(0.0f, 0.0f, 0.0f));
          playerRB.SetAccel(new Vector3(0.0f, 0.0f, 0.0f));
          playerRB.SetVel(new Vector3(0.0f, 0.0f, 0.0f));
        }
      }

      ManualStateControl();

      AutoStateControl();

      CheckChangeState();

      //Print(exitMoth.ToString());

      if (health <= 0.0f)
      {
        isDead = true;
      }

    }
    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other)
    {
        Collider otherCol = GetCollider(other);

        if (otherCol.tag == 5 && CurrentState == State.Moth /*&& exitMoth*/)
        {
            NextState = State.Human;
            //exitMoth = false;
        }

    }

    public override void OnCollisionExit(int other)
    {
    
    }

    public override void OnTriggerEnter(int other)
    {
        Collider otherCol = GetCollider(other);

        if (otherCol.tag == 5 && CurrentState == State.Moth /*&& exitMoth*/)
        {
            NextState = State.Human;
            //exitMoth = false;
        }

    }

        public override void Exit()
    {
    }

    public void Raycasting()
    {
      Vector3 camPos = camTrans.GetPosition();
      Vector3 camDir = Camera.GetViewVector();

      raycastID = RayCast(camPos + camDir * 400, camPos + camDir * 1000, 1);
      raycastID2 = RayCast(camPos + camDir * 400, camPos + camDir * 1000, 2);
      //RayTest(camPos + camPos * 400, camPos + camDir * 1000);

    
      if (raycastID != -1)
      {
        Collider col = GetCollider(raycastID);
        //Console.WriteLine(col.tag);
        //generator
        if (col.tag == 2)
        {
          //generator Script
          script_G = GetScript(raycastID); ;
          Transform trans = GetTransform(raycastID);
          Vector3 pos = trans.GetPosition();
          trans.SetPosition(new Vector3(pos.x, pos.y + fb_Timer * 1.5f, pos.z));
          hitGenerator = true;
          rayCastHit = true;

        }
        else if (col.tag == 3)
        {
          //morth Script
          Transform trans = GetTransform(raycastID);
          script_M = GetScript(raycastID);
          //Transform trans = GetTransform(raycastID);
          if (Input.GetKeyPress(VK.IMOUSE_RBUTTON))
          {
            script_M.PauseAI();
            hitMorth = true;
            morthPos = trans.GetPosition();
          }
          

          //Console.WriteLine("hit");
          rayCastHit = true;

        }
        else
        {
         
          rayCastHit = false;
        }


      }
      else
      {
        //ray hits nth
        rayCastHit = false;
        //Console.WriteLine("hahahahah");
      }

     
      if (raycastID2 != -1)
      {
     
        Collider col = GetCollider(raycastID2);
        //
        //generator
        if (col.tag == 2)
        {
          
          //generator Script
          script_G = GetScript(raycastID2);
          //RigidBody rigid = GetRigidBody(raycastID);
          Transform trans = GetTransform(raycastID2);
          Vector3 pos = trans.GetPosition();
          trans.SetPosition(new Vector3(pos.x, pos.y + fb_Timer * 1.5f, pos.z));
          hitGenerator = true;
          rayCastHit2 = true;
        }
        else if (col.tag == 3)
        {
          //morth Script
          script_M = GetScript(raycastID2);
          Transform trans = GetTransform(raycastID2);
          
          
          if (Input.GetKeyPress(VK.IMOUSE_RBUTTON))
          {
            script_M.PauseAI();
            morthPos = trans.GetPosition();
             hitMorth = true;
          }


          rayCastHit2 = true;

        }
        else
        {
          
          rayCastHit2 = false;

        }

      }
      else
      {
        //ray hits nth
        rayCastHit2 = false;
        //Console.WriteLine("hahahahah");
      }

    }


    // Player functions
    public void Move()
    {
      // Cant move while possessed, just return
      if (CurrentState == State.Possessed)
        return;


      // Get dir vectors, assume state is moth
      fwd = ForwardVector();
      rht = Camera.GetRightVector();


      // If state is human
      if (CurrentState == State.Human)
      {
        // No y axis
        fwd.y = 0.0f;
        rht.y = 0.0f;
        fwd = fwd.normalized;
        rht = rht.normalized;

        // Reset values
        moveDir = new Vector3(0.0f, 0.0f, 0.0f);
        movedW = false;
        movedA = false;
        movedS = false;
        movedD = false;

        if (Input.GetKeyHold(VK.IKEY_W))
        {
          moveDir += fwd;
          movedW = true;
          //Force.Apply(id, fwd, moveForce);
        }
        else if (Input.GetKeyUp(VK.IKEY_W))
        {
          movedW = false;
        }
        // Left
        if (Input.GetKeyHold(VK.IKEY_A))
        {
          moveDir += -rht;
          movedA = true;

          //Force.Apply(id, -rht, moveForce);
        }
        else if (Input.GetKeyUp(VK.IKEY_A))
        {

          movedA = false;
        }
        // Backward
        if (Input.GetKeyHold(VK.IKEY_S))
        {

          moveDir += -fwd;
          movedS = true;
          //Force.Apply(id, -fwd, moveForce);
        }
        else if (Input.GetKeyUp(VK.IKEY_S))
        {

          movedS = false;
        }
        // Right
        if (Input.GetKeyHold(VK.IKEY_D))
        {

          moveDir += rht;
          movedD = true;
          //Force.Apply(id, rht, moveForce);
        }
        else if (Input.GetKeyUp(VK.IKEY_D))
        {

          movedD = false;
        }

        // Check any movement
        if (movedW || movedA || movedS || movedD)
        {
          playerRB.isGravity = true;
          //Console.WriteLine("Applying");

          Vector3 rotation = Camera.GetViewVector();

          float r = (float)Math.Sqrt(rotation.x * rotation.x + rotation.z * rotation.z);
          //float t = (float)(Math.Atan(rotation.y / r) / Math.PI * 180);
          float p = (float)(Math.Acos(rotation.z / r) / Math.PI * 180);

          // t = t + 90;


          if (rotation.x < 0)
          {
            //rotation = new Vector3(0, -p, 0);
            rotation = new Vector3(-p, 0, 90);
          }
          else
          {

            //rotation = new Vector3(0, p, 0);
            rotation = new Vector3(p, 0, 90);
          }

          rotation.x += 90;
         // p = p + 90;

          GetTransform(id).SetRotation(rotation);


          if (switching == false)
          {

            if (anim.IsFinished())
            {
              anim.Stop();
              isPlaying = false;
            }


            anim.Play("Sneak_Walk", true, 1.9f, 3.108f);
            //


            if (isPlaying == false)
            {
              chnl = script_AM.PlaySFX(1);
              isPlaying = true;
            }

          }


          ////Console.WriteLine("Applying");

          Vector3 noYVel = new Vector3(playerRB.GetVel().x, 0.0f, playerRB.GetVel().z);
          //Console.WriteLine(playerRB.GetVel().magnitude.ToString());
          if (noYVel.magnitude >= maxHumnSpd)
          {
            //Console.WriteLine("reach limit");
            playerRB.SetForce(new Vector3(0.0f, playerRB.GetForce().y, 0.0f));
            playerRB.SetAccel(new Vector3(0.0f, playerRB.GetAccel().y, 0.0f));
            playerRB.SetVel(new Vector3(moveDir.normalized.x * maxHumnSpd, playerRB.GetVel().y, moveDir.normalized.z * maxHumnSpd));
          }
          else
          {
            Force.Apply(id, moveDir.normalized, moveForce);
          }
        }
        else
        {

          playerRB.SetForce(new Vector3(0.0f, playerRB.GetForce().y, 0.0f));
          playerRB.SetAccel(new Vector3(0.0f, playerRB.GetAccel().y, 0.0f));
          playerRB.SetVel(new Vector3(0.0f, playerRB.GetVel().y, 0.0f));

          if (switching == false)
          {
            if (anim.IsFinished())
            {
              anim.Stop();

            }
            if (!activating)
            {
              anim.Play("Idle_2", true, -1f, 10f);
            }
            
            //Audio.Stop(chnl);
            isPlaying = false;
          }
        }
      }

      else if (CurrentState == State.Moth)
      {
        //remove force
        playerRB.SetForce(new Vector3(0.0f, 0.0f, 0.0f));

        if(moth_currentSpeed <= mothSpeed)
        {
          moth_currentSpeed*=1.2f;
        }
        else
        {
          moth_currentSpeed = mothSpeed;
        }
        playerRB.SetVel(flyDir * moth_currentSpeed);
        playerRB.SetAccel(new Vector3(0.0f, 0.0f, 0.0f));
      }
    }

    public void ManualStateControl()
    {
      // Manual change
      if (Input.GetKeyPress(VK.IMOUSE_RBUTTON))
      {
        
        if (played == false)
        {
          switch (CurrentState)
          {
            case State.Human:

              if (hitMorth)
              { 
                

                // Only can change to moth if raycast hit
                switching = true;
                canMove = false;

                // Run an animation (no loop)
                if (curEnergy >= EnergyTreshold)
                {
                  //anim.Stop();

                  played = true;

                  if (isPlaying == false)
                  {
                    script_AM.PlaySFX(2);
                    isPlaying = true;
                  }

                  anim.Play("Pray", false, 10f, 30f);
                }


                isPlaying = false;
                // Prep for state change
                
                NextState = State.Moth;
              }

              // Shouldn't have logic here. This function is for setting next state only.
              //Transform p_Target = GetTransform(id);
              //spawnPoint = p_Target.GetPosition();
              break;

            case State.Moth:

              hitMorth = false;
              canMove = true;
                            CastToGround();
              //NextState = State.Human;




              break;
            case State.Possessed:

              NextState = State.Human;
              // Same thing, no logic here.

              break;
          }
        }
      }

    }

    public void AutoStateControl()
    {
      //if (CurrentState != State.Human)
      //{
      //  // Reduce energy when not in human state
      //  curEnergy -= EnergyDrain * RealDT();
      //  // Change back to human if current energy is not enough
      //  if (curEnergy <= 0.0f)
      //  {
      //    curEnergy = 0.0f;
      //    NextState = State.Human;
      //  }
      //}
      //else
      //{
      //  // Regain energy when in human state
      //  curEnergy += EnergyGain * RealDT();
      //  // Cap at maxEnergy
      //  if (curEnergy >= maxEnergy)
      //    curEnergy = maxEnergy;
      //}

      if (CurrentState == State.Moth)
      {
        playerCol.isCollidable = false;
        playerEmt.Play();
      }
      if (CurrentState == State.Human)
      {
        playerCol.isCollidable = true;
        playerEmt.Stop();
      }
    }

    public void CheckChangeState()
    {
      // If next state not equal to current state
      if (NextState != CurrentState)
      {
        //Audio.Stop(chnl);
        isPlaying = false;
        // Changing to possessed state. It's here because going possessed state has no delay.
        if (NextState == State.Possessed)
        {
          // Logic for possessed state
          CurrentState = NextState;
          //playerCharModel.AddModel(humanModePath);
          canMove = false;
        }

        else if (accumulatedDt >= transformTime)
        {
          accumulatedDt = 0.0f;

          PreviousState = CurrentState;
          CurrentState = NextState;
          //
          switch (CurrentState)
          {
            case State.Human:
              moveForce = humnForce;
              playerCharModel.isActive = true;
              //playerCharModel.AddModel(humanModePath);
              camScript.tgtID = id;

              canMove = true;
              camScript.useOffset = true;
              GetTransform(id).SetRotation(new Vector3(-90.0f, 0.0f, 90.0f));

              if(PreviousState == State.Possessed)
              {
                Transform p_Target2 = GetTransform(id);
                p_Target2.SetPosition(spawnPoint);
              }
                            CastToGround();
              if (flying)
              {
                flying = false;
                //playerTrans.SetPosition(FormalPos);
                //Camera.SetFOV(200);
              }
              playerRB.isGravity = true;
              break;

            case State.Moth:

             
              camScript.useOffset = false;
              //Set to Moth spawn Eyelevel
              playerTrans = GetTransform(id);
              Vector3 playerPosVect = playerTrans.GetPosition(); // Set eye level of player
              FormalPos = playerPosVect;
              flying = true;
              moth_currentSpeed = 1;
              
              //Vector3 eyelevel =  new Vector3(0.0f, 10.0f, 0.0f);
              // playerTrans.SetPosition(eyelevel);
              //Console.WriteLine("PlayerPos be4");
              //Console.WriteLine(playerPosVect.x);
              //Console.WriteLine(playerPosVect.y);
              //Console.WriteLine(playerPosVect.z);
              //Console.WriteLine("---------");
              playerTrans.SetPosition(new Vector3(playerPosVect.x, playerPosVect.y + 100f , playerPosVect.z));
                            //playerTrans.SetRotation(new Vector3(90.0f, -90.0f, 180.0f));
                            //Console.WriteLine("PlayerPos aft");
                            //Console.WriteLine(playerPosVect.x);
                            //Console.WriteLine(playerPosVect.y);
                            //Console.WriteLine(playerPosVect.z);
                            //Console.WriteLine("---------");
                            
              Vector3 newDir = morthPos - playerTrans.GetPosition();
              flyDir = newDir.normalized;
              //anim.Stop();
              playerCharModel.isActive = false;
              playerRB.isGravity = false;

              canMove = true;
              played = false;
              switching = false;

              break;
          }
        }
        else
        {
          canMove = false;
          if(NextState == State.Human)
          {
            accumulatedDt += RealDT()*5.0f;
          }
          else
          {
            accumulatedDt += RealDT();
          }
        }
      }
    }

    // Player functions
    public Vector3 ForwardVector()
    {
      return Vector3.Cross(Camera.GetUpVector(), Camera.GetRightVector());
    }


    public void CastToGround()
    {
        //exitMoth = true;
        Vector3 castDir = new Vector3(0.0f, -1.0f, 0.0f);
        flyDir = castDir;
        //playerRB.isGravity = true;
        script_M.activate = false;
        ////player position
        //Vector3 playerPos = playerTrans.GetPosition();
        ////cast to ground vector
        //
        ////get position for ground object
        //int Id = RayCast(playerPos, playerPos + castDir * 2000, 2);

        //if (Id != -1)
        //{

        //    //Transform groundTrans = GetTransform(Id);
        //    //Vector3 groundPos = groundTrans.GetPosition();
        //    //Vector3 newdir = playerPos - groundPos;

        //}
        //else
        //{

        //}


    }
  }
}
