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
    Transform playerPos;
    Animation anim;

    /// <summary>
    /// ////////////////Place holder Variables
    public static bool flying = false;
    public Vector3 FormalPos;
    public static float mothTime = 0.0f; // time to change state
    //public static float floatTime = 0.0f; // time to float for possessable
    /// ////////////////
    /// </summary>

    // Other entity's components
    public ScriptCamera camScript; // To set camera tgt back to player after becoming human.
    public Transform camTrans; // To get cam position for ray cast

    // Player Stats, default values
    public static float humnForce = 100000.0f;  // Move force while human
    public static float mothSpeed = 10000.0f;  // Move force while moth
    public static float maxEnergy = 15.0f;
    public static float transformTime = 3.5f; // time to change state

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
    bool idle = true;
    bool played = false;
    bool switching = false;
	public bool isDead = false;

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
    // accumulated dt
    private float accumulatedDt = 0.0f;

    //Variables
    string humanModePath;
    string mothModePath;
    int raycastID = 0;
    bool rayCastHit = false;

    public override void Init()
    {
      CurrentState = State.Human;
      NextState = State.Human;

      playerRB = GetRigidBody(id);
      playerCharModel = GetGraphics(id);
      playerVar = GetVariables(id);
      playerPos = GetTransform(id);
      anim = GetAnimation(id);

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
      mothModePath = playerVar.GetString(1);

      curEnergy = maxEnergy;
      moveForce = humnForce;
      canMove = true;
      switching = false;

      fwd = ForwardVector();

      playerCharModel.AddModel(humanModePath);
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      Vector3 camPos = camTrans.GetPosition();
      Vector3 camDir = Camera.GetViewVector();

      raycastID = RayCast(camPos, camPos + camDir * 1000,2);

      if (raycastID != -1)
      {
        Collider col = GetCollider(raycastID);
        //Console.WriteLine(col.tag);
        //generator
        if (col.tag == 2)
        {
          //generator Script
          Generator tmp = GetScript(raycastID);
          //RigidBody rigid = GetRigidBody(raycastID);
          Transform trans = GetTransform(raycastID);
          Vector3 pos = trans.GetPosition();
          trans.SetPosition(new Vector3(pos.x, pos.y + 1, pos.z));

          rayCastHit = true;
          //Console.WriteLine(tmp.name);
          if (Input.GetKeyPress(VK.IMOUSE_RBUTTON))
          {
            tmp.activate = true;
            Console.WriteLine("generator activate");
          }
          if (Input.GetKeyUp(VK.IMOUSE_RBUTTON))
          {
            tmp.activate = false;
            Console.WriteLine("generator deactivate");
          }
          
        }
        //Fan
        else if (col.tag == 3)
        {
          //Fan Script
          FanBehaviour tmp = GetScript(raycastID);
          rayCastHit = true;
          //Console.WriteLine(tmp.name);
          if (Input.GetKeyPress(VK.IMOUSE_RBUTTON))
          {
            tmp.activate = true;
            Console.WriteLine("fan activate");
          }
          if (Input.GetKeyUp(VK.IMOUSE_RBUTTON))
          {
            tmp.activate = false;
            Console.WriteLine("fan deactivate");
          }
        }
        else
        {
          //floatTime = 5;
          rayCastHit = false;
          //non possessable woth collider
          //Transform tmp1 = GetTransform(raycastID);
          //Console.WriteLine("haha");
          //Console.WriteLine(tmp1.name);
        }



      }
      else
      {
        //ray hits nth
        rayCastHit = false;
        //Console.WriteLine("hahahahah");
      }

      //Console.WriteLine(rayCastHit);

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


      if (switching == true)
      {
        //playerCharModel.AddModel(mothModePath);
        // switching = false;
        //  anim.Play("Idle1", true);

        //if (anim.IsFinished("Switch1"))
        //{
        //  //
         
        //  canMove = true;
        //  NextState = State.Moth;
        //  // playerPos.SetPosition(new Vector3(playerPosVect.x, playerPosVect.y + 100, playerPosVect.z));
        //  switching = false;

        //}

      }


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

      if (otherTransform.tag == 1)
      {
        // Always use next state to change state.
        // CurrentState = State.Possessed;

        //canMove = false;
      }
    }

    public override void OnTriggerEnter(int other)
    {
    }

    public override void Exit()
    {
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
            }


            anim.Play("Sneak_Walk", true, 1.9f, 3.108f);

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

            anim.Play("Idle", true, -1f, 10f);
          }
        }
      }

      else if (CurrentState == State.Moth)
      {
        Vector3 rotation = Camera.GetViewVector();

        float r = (float)Math.Sqrt(rotation.x * rotation.x + rotation.z * rotation.z);
        float t = (float)(Math.Atan(rotation.y / r) / Math.PI * 180);
        float p = (float)(Math.Acos(rotation.z / r) / Math.PI * 180);

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


       
        //p = p - 90;

        rotation.y += 90;
       // rotation.x += 90;
       

        GetTransform(id).SetRotation(rotation);


        //GetTransform(id).SetRotation(Camera.GetXZViewVector() + new Vector3(-90.0f, 0.0f, 90.0f));
        playerRB.SetForce(new Vector3(0.0f, 0.0f, 0.0f));
        playerRB.SetVel(fwd * mothSpeed);
        playerRB.SetAccel(new Vector3(0.0f, 0.0f, 0.0f));
      }
    }

    public void ManualStateControl()
    {
      // Manual change
      if (Input.GetKeyPress(VK.IMOUSE_RBUTTON) && (rayCastHit == false)) 
      {

        if (played == false)
        {
          switch (CurrentState)
          {
            case State.Human:
              // Only can change to moth if enough energy
              switching = true;
              canMove = false;
             
              // Run an animation (no loop)
              if ( curEnergy >= EnergyTreshold )
              {
                //anim.Stop();

                played = true;

                anim.Play("Pray", false, -1f, 30f);
              }
              // Prep for state change 
              NextState = State.Moth;
              // Shouldn't have logic here. This function is for setting next state only.
              //Transform p_Target = GetTransform(id);
              //spawnPoint = p_Target.GetPosition();
              break;

            case State.Moth:
              canMove = false;
              NextState = State.Human;

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

      if (flying)
      {
        mothTime += RealDT();
        if (mothTime >= 5.0f) 
        {
          mothTime = 0;
          flying = false;
          NextState = State.Human;
          playerPos.SetPosition(FormalPos);
        }
      }
    }

    public void CheckChangeState()
    {
      // If next state not equal to current state
      if (NextState != CurrentState)
      {
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
              mothTime = 999;
              moveForce = humnForce;
              playerCharModel.AddModel(humanModePath);
              camScript.tgtID = id;
              
              canMove = true;
              camScript.useOffset = true;
              GetTransform(id).SetRotation(new Vector3(-90.0f, 0.0f, 90.0f));
              
              if(PreviousState == State.Possessed)
              {
                Transform p_Target2 = GetTransform(id);
                p_Target2.SetPosition(spawnPoint);
              }
              playerRB.isGravity = true;
              break;

            case State.Moth:

              camScript.useOffset = false;
              //moveForce = mothForce;
              //Set to Moth spawn Eyelevel//// not working
              playerPos = GetTransform(id);
              Vector3 playerPosVect = playerPos.GetPosition(); // Set eye level of player
              FormalPos = playerPosVect;
              flying = true;
              mothTime = 0;
              //Vector3 eyelevel =  new Vector3(0.0f, 10.0f, 0.0f);
              // playerPos.SetPosition(eyelevel);
              //Console.WriteLine("PlayerPos be4");
              //Console.WriteLine(playerPosVect.x);
              //Console.WriteLine(playerPosVect.y);
              //Console.WriteLine(playerPosVect.z);
              //Console.WriteLine("---------");
              playerPos.SetPosition(new Vector3(playerPosVect.x, playerPosVect.y + 80 , playerPosVect.z));
              //playerPos.SetRotation(new Vector3(90.0f, -90.0f, 180.0f));
              //Console.WriteLine("PlayerPos aft");
              //Console.WriteLine(playerPosVect.x);
              //Console.WriteLine(playerPosVect.y);
              //Console.WriteLine(playerPosVect.z);
              //Console.WriteLine("---------");

              //anim.Stop();
              playerCharModel.AddModel(mothModePath);
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
  }
}