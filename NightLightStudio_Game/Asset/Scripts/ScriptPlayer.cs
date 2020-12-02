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
    // Other entity's components

    // Player Stats, default values
    public static float humnForce = 100000.0f;  // Move force while human
    public static float mothSpeed = 10000.0f;  // Move force while moth
    public static float maxEnergy = 15.0f;
    public static float transformTime = 1.0f; // time to change state
    public static float maxHumnSpd = 500.0f;
    //public static float maxMothSpd = 100.0f;
    public static float moveForce = humnForce; // Current force
    public static float curEnergy = maxEnergy;
    // Minimum Energy required to change to moth
    public static float EnergyTreshold = 0.0f;
    public static float EnergyDrain = 1.0f;
    public static float EnergyGain = 1.0f;
    bool canMove;

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
    public State CurrentState = State.Human;
    public State NextState = State.Human;
    //Vector3 inVec;
    // accumulated dt
    private float accumulatedDt = 0.0f;

    //Variables
    string humanModePath;
    string mothModePath;

    public override void Init()
    {
      playerRB = GetRigidBody(id);
      playerCharModel = GetGraphics(id);
      playerVar = GetVariables(id);
      playerPos = GetTransform(id);


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
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      if (canMove == true)
      {
        Move();
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

      if (otherTransform.Tag == 1)
      {
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
        if (movedW || movedA || movedS || movedD)
        {
          //Console.WriteLine("Applying");

          //Vector3 noYVel = new Vector3(playerRB.GetVel().x, 0.0f, playerRB.GetVel().z);
          if (playerRB.GetVel().sqrMagnitude >= maxHumnSpd)
          {
            playerRB.SetForce(new Vector3(0.0f, playerRB.GetForce().y, 0.0f));
            playerRB.SetAccel(new Vector3(0.0f, playerRB.GetAccel().y, 0.0f));
            playerRB.SetVel(moveDir.normalized * maxHumnSpd);
            //playerRB.SetVel(new Vector3(0.0f, 0.0f, 0.0f));
          }
          else
          {
            Force.Apply(id, moveDir.normalized, moveForce);
            ///Force.Apply(id, new Vector3(0.0f,0.0f,0.0f), moveForce);
          }
        }
        else
        {
          //Console.WriteLine("Check");
          //Vector3 zeroVec = new Vector3(0.0f, 0.0f, 0.0f);
          playerRB.SetForce(new Vector3(0.0f, playerRB.GetForce().y, 0.0f));
          playerRB.SetAccel(new Vector3(0.0f, playerRB.GetAccel().y, 0.0f));
          playerRB.SetVel(new Vector3(0.0f, playerRB.GetVel().y, 0.0f));
        }
      }

      else if (CurrentState == State.Moth)
      {
        playerRB.SetForce(new Vector3(0.0f, 0.0f, 0.0f));
        playerRB.SetVel(fwd * mothSpeed);
        playerRB.SetAccel(new Vector3(0.0f, 0.0f, 0.0f));
      }
    }

    public void ManualStateControl()
    {
      // Manual change
      if (Input.GetKeyPress(VK.IMOUSE_RBUTTON))
      {
        switch (CurrentState)
        {
          case State.Human:
            // Only can change to moth if enough energy
            if (curEnergy >= EnergyTreshold)
              NextState = State.Moth;



            Transform p_Target = GetTransform(id);
            spawnPoint = p_Target.getPosition();

            break;
          case State.Moth:
            NextState = State.Human;
            break;

          case State.Possessed:
            NextState = State.Human;

            Transform p_Target2 = GetTransform(id);
            //p_Target2.setPosition(spawnPoint);

            break;
        }
      }
    }

    public void AutoStateControl()
    {
      if (CurrentState != State.Human)
      {
        // Reduce energy when not in human state
        curEnergy -= EnergyDrain * RealDT();
        // Change back to human if current energy is not enough
        if (curEnergy <= 0.0f)
        {
          curEnergy = 0.0f;
          NextState = State.Human;
        }
      }
      else
      {
        // Regain energy when in human state
        curEnergy += EnergyGain * RealDT();
        // Cap at maxEnergy
        if (curEnergy >= maxEnergy)
          curEnergy = maxEnergy;
      }
    }

    public void CheckChangeState()
    {
      if (NextState == State.Possessed && NextState != CurrentState)
      {
        CurrentState = NextState;
        // Logic for possessed state

        playerCharModel.AddModel(humanModePath);
        canMove = false;
      }
      else if (NextState != CurrentState)
      {

        if (accumulatedDt >= transformTime)
        {
          accumulatedDt = 0.0f;
          CurrentState = NextState;
          Console.WriteLine(CurrentState);
          switch (CurrentState)
          {
            case State.Human:
              moveForce = humnForce;
              playerCharModel.AddModel(humanModePath);
              playerRB.isGravity = true;
              canMove = true;
              break;
            case State.Moth:
              //moveForce = mothForce;
              playerCharModel.AddModel(mothModePath);

              playerRB.isGravity = false;

              //Set to Moth spawn Eyelevel//// not working
              playerPos = GetTransform(id);
              Vector3 playerPosVect = playerPos.getPosition();

              //Vector3 eyelevel =  new Vector3(0.0f, 10.0f, 0.0f);
              // playerPos.setPosition(eyelevel);
              playerPos.setPosition(new Vector3(playerPosVect.x+1000.0f, playerPosVect.y + 1000.0f, playerPosVect.z));
              canMove = true;

              break;

            case State.Possessed:
              break;

          }


        }
        else
        {
          canMove = false;
          accumulatedDt += RealDT();
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